#ifndef CAS_H
#define CAS_H

#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#undef iszero   /* the CE math.h has a macro of this name */

/* Expression nodes are immutable once built, so trees share subtrees and a
 * step can keep a pointer to the whole expression at that point. */
enum {
    N_NUM,     /* exact rational q.num / q.den */
    N_FLT,     /* decimal f */
    N_VAR,     /* op = letter */
    N_CONST,   /* op = C_* */
    N_ADD, N_MUL, N_POW,   /* POW: base, exponent */
    N_FN,      /* op = F_*; LOGB is (base, arg) */
    N_EQ,      /* left, right */
    N_OR,      /* list of equations: x = a or x = b */
    N_PM,      /* a ± b */
    N_DERIV,   /* f, var */
    N_INTEG,   /* f, var [, lower, upper] */
    N_LIMIT,   /* f, var, target */
    N_EVAL,    /* [F] from a to b: F, a, b */
};

enum { C_PI, C_E, C_I, C_INF };

enum { F_SIN, F_COS, F_TAN, F_ASIN, F_ACOS, F_ATAN, F_LN, F_LOG, F_LOGB, F_ABS,
       F_NCR, F_NPR, F_FACT };   /* NCR and NPR are (n, r) */

typedef struct node node_t;
struct node {
    uint8_t type, op, n;   /* n = number of children */
    node_t **k;
    union {
        struct { int32_t num, den; } q;
        double f;
    } v;
};

/* ---- memory ---- */

/* Hands the engine its memory; the nodes and child arrays are carved from it.
 * Every problem starts with cas_reset(), which frees all nodes at once. */
void cas_init(void *mem, size_t size);
void cas_reset(void);
size_t cas_used(void);

/* Set when an engine call fails (pool full, division by zero, parse error...) */
extern const char *cas_err;

/* ---- building (expr.c) ---- */

node_t *num(int32_t n);
node_t *frac(int32_t n, int32_t d);     /* normalised */
node_t *flt(double f);
node_t *var(char c);
node_t *cnst(uint8_t c);
node_t *fn(uint8_t f, node_t *a);
node_t *mk(uint8_t type, int n, ...);   /* raw node with n children, no simplification */
node_t *mkv(uint8_t type, int n, node_t **k);
node_t *add(node_t *a, node_t *b);      /* raw a + b */
node_t *sub(node_t *a, node_t *b);      /* raw a + (-1)b */
node_t *mul(node_t *a, node_t *b);      /* raw a * b */
node_t *dvd(node_t *a, node_t *b);      /* raw a * b^-1 */
node_t *pw(node_t *a, node_t *b);       /* raw a ^ b */
node_t *neg(node_t *a);                 /* raw (-1)a */
node_t *eq(node_t *a, node_t *b);

bool isnum(const node_t *e);            /* NUM or FLT */
bool isint(const node_t *e, int32_t *out);
bool iszero(const node_t *e);
bool isone(const node_t *e);
bool isval(const node_t *e, int32_t n);
double numval(const node_t *e);
bool same(const node_t *a, const node_t *b);
bool before(const node_t *a, const node_t *b);   /* canonical order */
bool has(const node_t *e, const node_t *x);      /* x occurs in e */
bool hasvar(const node_t *e, char v);
node_t *subst(node_t *e, const node_t *from, node_t *to);   /* raw */

/* exact rational arithmetic on NUM/FLT nodes */
node_t *nadd(const node_t *a, const node_t *b);
node_t *nmul(const node_t *a, const node_t *b);
node_t *npow(const node_t *a, int32_t n);

/* parser: linear text from the editor; on error returns NULL, sets cas_err
 * and *errpos to the offending character */
node_t *parse(const char *s, int *errpos);

/* linear text, used by the tests */
char *print(const node_t *e, char *buf, size_t size);

/* display order, shared by the printer and the drawing code */
node_t *negate(const node_t *e);
bool negative(const node_t *e);   /* shown with a leading minus */
int sumterms(const node_t *e, const node_t **out);
int fracparts(const node_t *e, const node_t **top, int *nt, const node_t **bot, int *nb);

/* inside the engine: abort the current call with a message */
#include <setjmp.h>
extern jmp_buf cas_jb;
void fail(const char *msg);
void cas_deeper(void);   /* fails when the stack is nearly used up */
extern bool cas_logging;   /* simp() records its rewrites as sub-steps */
uint8_t *cas_mark(void);
void cas_release(uint8_t *m);

/* ---- simplification (simp.c) ---- */

extern bool cas_deg;    /* trig in degrees */

node_t *simp(node_t *e);       /* automatic simplification, no steps */
node_t *tidy(node_t *e, const char *cap);   /* simp() logged as one step; cap NULL picks one */
node_t *expand(node_t *e);
node_t *factor(node_t *e);     /* logs steps */
double approx(const node_t *e, char v, double at, bool *ok);

/* ---- polynomials (poly.c) ---- */

#define MAXDEG 8
typedef struct {
    node_t *c[MAXDEG + 1];
    int deg;
} poly_t;

bool topoly(node_t *e, const node_t *x, poly_t *p);
node_t *frompoly(const poly_t *p, node_t *x);
bool rational(const poly_t *p);              /* all coefficients exact rationals */
node_t *primitive(poly_t *p);                /* make integer, coprime; returns the factor */
node_t *ratroot(const poly_t *p);            /* p integer */
void synth(const poly_t *p, node_t *r, poly_t *out);
node_t *factorpoly(poly_t p, node_t *x);     /* logs steps */
node_t *cancel(node_t *e, node_t *x);        /* common factors of a fraction, logs steps */

/* ---- steps ---- */

/* A caption may contain one '@', where arg is drawn. Sub-steps logged with
 * substep() attach to the next step(). */
typedef struct {
    const char *cap;
    node_t *arg;
    node_t *e;
    int16_t parent;   /* -1 for a top-level step */
} step_t;

#define MAX_STEPS 160
extern step_t steps[MAX_STEPS];
extern int nsteps;

void step(const char *cap, node_t *arg, node_t *e);
extern int cas_depth;   /* > 0 while solving a part: step() logs sub-steps */
void substep(const char *cap, node_t *arg, node_t *e);

/* ---- top level ---- */

enum { A_SIMPLIFY, A_EXPAND, A_FACTOR, A_APPROX, A_SOLVE, A_CALC };

/* Runs one action and fills steps[]. The last top-level step holds the
 * answer. Solve uses var v and, for trig equations, the interval lo..hi. */
bool cas_run(node_t *e, uint8_t action, char v, node_t *lo, node_t *hi);
extern node_t *cas_answer, *cas_approx;

#endif
