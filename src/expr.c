#include <math.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "cas.h"

static uint8_t *mem0, *memp, *memend;
static uintptr_t stackFloor;
const char *cas_err;
jmp_buf cas_jb;

step_t steps[MAX_STEPS];
int nsteps;

void fail(const char *msg) {
    cas_err = msg;
    longjmp(cas_jb, 1);
}

void cas_init(void *mem, size_t size) {
    mem0 = mem;
    memend = mem0 + size;
    /* the CE stack is 4 KB and running past it crashes the calculator; the
     * budget scales with pointer size so host tests stop at the same depth */
    char top;
    stackFloor = (uintptr_t)&top - sizeof(void *) * 1000;
    cas_reset();
}

void cas_reset(void) {
    memp = mem0;
    nsteps = 0;
    cas_depth = 0;
    cas_logging = false;
    cas_err = NULL;
}

size_t cas_used(void) {
    return memp - mem0;
}

/* frees everything allocated since the mark */
uint8_t *cas_mark(void) {
    return memp;
}

void cas_release(uint8_t *m) {
    memp = m;
}

void cas_deeper(void) {
    char probe;
    if ((uintptr_t)&probe < stackFloor) fail("Too complex");
}

static void *alloc(size_t n) {
    cas_deeper();
    n = (n + sizeof(void *) - 1) & ~(sizeof(void *) - 1);
    if ((size_t)(memend - memp) < n) fail("Too complex");
    void *p = memp;
    memp += n;
    return p;
}

static node_t *newnode(uint8_t type, int n) {
    node_t *e = alloc(sizeof *e);
    e->type = type;
    e->op = 0;
    e->n = n;
    e->k = n ? alloc(n * sizeof(node_t *)) : NULL;
    return e;
}

/* ---- steps ---- */

static void logstep(const char *cap, node_t *arg, node_t *e, int16_t parent) {
    if (nsteps == MAX_STEPS) return;   /* ponytail: extra steps are dropped; raise MAX_STEPS if it bites */
    steps[nsteps++] = (step_t){cap, arg, e, parent};
}

void substep(const char *cap, node_t *arg, node_t *e) {
    logstep(cap, arg, e, -2);   /* -2: waiting for its step */
}

int cas_depth;

void step(const char *cap, node_t *arg, node_t *e) {
    if (cas_depth) {
        substep(cap, arg, e);
        return;
    }
    if (nsteps == MAX_STEPS) return;
    for (int i = 0; i < nsteps; i++)
        if (steps[i].parent == -2) steps[i].parent = nsteps;
    logstep(cap, arg, e, -1);
}

/* ---- numbers ---- */

static int64_t gcd64(int64_t a, int64_t b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b) {
        int64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

node_t *flt(double f) {
    node_t *e = newnode(N_FLT, 0);
    e->v.f = f;
    return e;
}

static node_t *q64(int64_t n, int64_t d) {
    if (!d) fail("Division by zero");
    if (d < 0) n = -n, d = -d;
    const int64_t g = gcd64(n, d);
    if (g > 1) n /= g, d /= g;
    if (n > INT32_MAX || n < -INT32_MAX || d > INT32_MAX) return flt((double)n / (double)d);
    node_t *e = newnode(N_NUM, 0);
    e->v.q.num = (int32_t)n;
    e->v.q.den = (int32_t)d;
    return e;
}

node_t *num(int32_t n) {
    return q64(n, 1);
}

node_t *frac(int32_t n, int32_t d) {
    return q64(n, d);
}

bool isnum(const node_t *e) {
    return e->type == N_NUM || e->type == N_FLT;
}

bool isint(const node_t *e, int32_t *out) {
    if (e->type != N_NUM || e->v.q.den != 1) return false;
    if (out) *out = e->v.q.num;
    return true;
}

bool isval(const node_t *e, int32_t n) {
    return e->type == N_NUM && e->v.q.den == 1 && e->v.q.num == n;
}

bool iszero(const node_t *e) {
    return isval(e, 0) || (e->type == N_FLT && e->v.f == 0);
}

bool isone(const node_t *e) {
    return isval(e, 1);
}

double numval(const node_t *e) {
    return e->type == N_NUM ? (double)e->v.q.num / e->v.q.den : e->v.f;
}

node_t *nadd(const node_t *a, const node_t *b) {
    if (a->type == N_FLT || b->type == N_FLT) return flt(numval(a) + numval(b));
    return q64((int64_t)a->v.q.num * b->v.q.den + (int64_t)b->v.q.num * a->v.q.den,
               (int64_t)a->v.q.den * b->v.q.den);
}

node_t *nmul(const node_t *a, const node_t *b) {
    if (a->type == N_FLT || b->type == N_FLT) return flt(numval(a) * numval(b));
    return q64((int64_t)a->v.q.num * b->v.q.num, (int64_t)a->v.q.den * b->v.q.den);
}

node_t *npow(const node_t *a, int32_t n) {
    if (a->type == N_FLT) return flt(pow(a->v.f, n));
    if (n < 0) {
        if (!a->v.q.num) fail("Division by zero");
        return npow(q64(a->v.q.den, a->v.q.num), -n);
    }
    node_t *r = num(1);
    while (n--) {
        r = nmul(r, a);
        if (r->type == N_FLT) return flt(pow(numval(a), n + 1) * r->v.f);
    }
    return r;
}

/* ---- building ---- */

node_t *var(char c) {
    node_t *e = newnode(N_VAR, 0);
    e->op = c;
    return e;
}

node_t *cnst(uint8_t c) {
    node_t *e = newnode(N_CONST, 0);
    e->op = c;
    return e;
}

node_t *mkv(uint8_t type, int n, node_t **k) {
    node_t *e = newnode(type, n);
    memcpy(e->k, k, n * sizeof *k);
    return e;
}

node_t *mk(uint8_t type, int n, ...) {
    node_t *e = newnode(type, n);
    va_list ap;
    va_start(ap, n);
    for (int i = 0; i < n; i++) e->k[i] = va_arg(ap, node_t *);
    va_end(ap);
    return e;
}

node_t *fn(uint8_t f, node_t *a) {
    node_t *e = mk(N_FN, 1, a);
    e->op = f;
    return e;
}

node_t *add(node_t *a, node_t *b) { return mk(N_ADD, 2, a, b); }
node_t *mul(node_t *a, node_t *b) { return mk(N_MUL, 2, a, b); }
node_t *pw(node_t *a, node_t *b) { return mk(N_POW, 2, a, b); }
node_t *eq(node_t *a, node_t *b) { return mk(N_EQ, 2, a, b); }
node_t *neg(node_t *a) { return mul(num(-1), a); }
node_t *sub(node_t *a, node_t *b) { return add(a, neg(b)); }
node_t *dvd(node_t *a, node_t *b) { return mul(a, pw(b, num(-1))); }

/* ---- comparing ---- */

bool same(const node_t *a, const node_t *b) {
    if (a == b) return true;
    if (a->type != b->type || a->op != b->op || a->n != b->n) return false;
    if (a->type == N_NUM) return a->v.q.num == b->v.q.num && a->v.q.den == b->v.q.den;
    if (a->type == N_FLT) return a->v.f == b->v.f;
    for (int i = 0; i < a->n; i++)
        if (!same(a->k[i], b->k[i])) return false;
    return true;
}

/* symbols order as: pi, e, i, infinity, then letters */
static int symkey(const node_t *e) {
    return e->type == N_CONST ? e->op : 16 + e->op;
}

static bool issym(const node_t *e) {
    return e->type == N_VAR || e->type == N_CONST;
}

/* compare operand lists from the last operand backwards */
static bool listbefore(node_t *const *u, int m, node_t *const *v, int n) {
    while (m && n) {
        if (!same(u[m - 1], v[n - 1])) return before(u[m - 1], v[n - 1]);
        m--, n--;
    }
    return m < n;
}

/* sums are stored highest term first, so they compare from the front */
static bool sumbefore(node_t *const *u, int m, node_t *const *v, int n) {
    for (int i = 0; i < m && i < n; i++)
        if (!same(u[i], v[i])) return before(u[i], v[i]);
    return m < n;
}

/* Cohen's canonical order for operands of sums and products. Products
 * store their operands in this order, sums in the reverse. */
bool before(const node_t *u, const node_t *v) {
    if (isnum(u) && isnum(v)) return numval(u) < numval(v);
    if (isnum(u)) return true;
    if (isnum(v)) return false;
    if (issym(u) && issym(v)) return symkey(u) < symkey(v);
    if (u->type == v->type) {
        switch (u->type) {
            case N_ADD:
                return sumbefore(u->k, u->n, v->k, v->n);
            case N_MUL:
                return listbefore(u->k, u->n, v->k, v->n);
            case N_POW:
                if (!same(u->k[0], v->k[0])) return before(u->k[0], v->k[0]);
                return before(u->k[1], v->k[1]);
            case N_FN:
                if (u->op != v->op) return u->op < v->op;
                for (int i = 0; i < u->n && i < v->n; i++)
                    if (!same(u->k[i], v->k[i])) return before(u->k[i], v->k[i]);
                return u->n < v->n;
            default:
                for (int i = 0; i < u->n && i < v->n; i++)
                    if (!same(u->k[i], v->k[i])) return before(u->k[i], v->k[i]);
                return u->n < v->n;
        }
    }
    node_t *one = (node_t *)v;
    switch (u->type) {
        case N_MUL:
            if (v->type == N_POW || v->type == N_ADD || v->type == N_FN || issym(v))
                return listbefore(u->k, u->n, &one, 1);
            break;
        case N_POW:
            if (v->type == N_ADD || v->type == N_FN || issym(v)) {
                if (!same(u->k[0], v)) return before(u->k[0], v);
                return before(u->k[1], num(1));
            }
            break;
        case N_ADD:
            if (v->type == N_FN || issym(v)) return sumbefore(u->k, u->n, &one, 1);
            break;
        case N_FN:
            if (issym(v)) return false;
            break;
        default:
            if (u->type > N_FN || v->type > N_FN) return u->type < v->type;
    }
    return !before(v, u);
}

bool has(const node_t *e, const node_t *x) {
    if (same(e, x)) return true;
    for (int i = 0; i < e->n; i++)
        if (has(e->k[i], x)) return true;
    return false;
}

bool hasvar(const node_t *e, char v) {
    if (e->type == N_VAR) return e->op == v;
    for (int i = 0; i < e->n; i++)
        if (hasvar(e->k[i], v)) return true;
    return false;
}

node_t *subst(node_t *e, const node_t *from, node_t *to) {
    if (same(e, from)) return to;
    if (!e->n) return e;
    node_t *k[e->n];
    bool changed = false;
    for (int i = 0; i < e->n; i++) {
        k[i] = subst(e->k[i], from, to);
        changed |= k[i] != e->k[i];
    }
    if (!changed) return e;
    node_t *r = mkv(e->type, e->n, k);
    r->op = e->op;
    return r;
}

/* ---- parser ---- */

static const char *src, *ps;

static void perr(const char *msg) {
    fail(msg);
}

static void skip(void) {
    while (*ps == ' ') ps++;
}

static bool accept(char c) {
    skip();
    if (*ps != c) return false;
    ps++;
    return true;
}

static void expect(char c) {
    if (!accept(c)) perr(c == ')' ? "A ) is missing" : "This doesn't make sense here");
}

/* names in the order they are tried, longest first where one is a prefix */
static const struct { const char *s; uint8_t type, op, args; } names[] = {
    {"asin", N_FN, F_ASIN, 1}, {"acos", N_FN, F_ACOS, 1}, {"atan", N_FN, F_ATAN, 1},
    {"sin", N_FN, F_SIN, 1}, {"cos", N_FN, F_COS, 1}, {"tan", N_FN, F_TAN, 1},
    {"logb", N_FN, F_LOGB, 2}, {"log", N_FN, F_LOG, 1}, {"ln", N_FN, F_LN, 1},
    {"abs", N_FN, F_ABS, 1}, {"ncr", N_FN, F_NCR, 2}, {"npr", N_FN, F_NPR, 2},
    {"sqrt", N_POW, 2, 1}, {"root", N_POW, 0, 2},
    {"frac", N_MUL, 0, 2}, {"der", N_DERIV, 0, 2}, {"int", N_INTEG, 0, 0},
    {"lim", N_LIMIT, 0, 3}, {"pi", N_CONST, C_PI, 0}, {"inf", N_CONST, C_INF, 0},
};

static node_t *pexpr(void);
static node_t *psum(void);
static node_t *punary(void);

static node_t *pnumber(void) {
    int64_t n = 0, d = 1;
    bool dot = false;
    for (; (*ps >= '0' && *ps <= '9') || *ps == '.'; ps++) {
        if (*ps == '.') {
            if (dot) perr("A number has two decimal points");
            dot = true;
            continue;
        }
        if (n > 99999999) perr("Number too long");
        n = n * 10 + (*ps - '0');
        if (dot) d *= 10;
    }
    return q64(n, d);
}

static node_t *patom(void) {
    cas_deeper();
    skip();
    const char c = *ps;
    if ((c >= '0' && c <= '9') || c == '.') return pnumber();
    if (accept('(')) {
        node_t *e = pexpr();
        expect(')');
        return e;
    }
    if (accept('|')) {
        node_t *e = psum();
        expect('|');
        return fn(F_ABS, e);
    }
    if (c >= 'a' && c <= 'z') {
        for (unsigned i = 0; i < sizeof names / sizeof names[0]; i++) {
            const size_t len = strlen(names[i].s);
            if (strncmp(ps, names[i].s, len)) continue;
            ps += len;
            if (names[i].type == N_CONST) return cnst(names[i].op);
            expect('(');
            node_t *a[4];
            int n = 0;
            do {
                if (n == 4) perr("Too many arguments");
                a[n++] = pexpr();
            } while (accept(','));
            expect(')');
            const int want = names[i].args;
            if (want ? n != want : n != 2 && n != 4) perr("Wrong number of arguments");
            switch (names[i].type) {
                case N_FN: {
                    node_t *e = n == 2 ? mk(N_FN, 2, a[0], a[1]) : fn(names[i].op, a[0]);
                    e->op = names[i].op;
                    return e;
                }
                case N_POW:   /* sqrt(x) or root(n, x) */
                    return n == 1 ? pw(a[0], frac(1, 2)) : pw(a[1], dvd(num(1), a[0]));
                case N_MUL:   /* a fraction of two numbers is just a number */
                    if (a[0]->type == N_NUM && a[1]->type == N_NUM && !iszero(a[1]))
                        return nmul(a[0], npow(a[1], -1));
                    return dvd(a[0], a[1]);
                default:
                    if (a[1]->type != N_VAR) perr("Expected a variable");
                    return mkv(names[i].type, n, a);
            }
        }
        ps++;
        return c == 'e' ? cnst(C_E) : var(c);
    }
    perr(c ? "This doesn't make sense here" : "Something is missing at the end");
    return NULL;
}

static node_t *ppower(void) {
    node_t *b = patom();
    while (accept('!')) b = fn(F_FACT, b);
    if (accept('^')) return pw(b, punary());
    return b;
}

static node_t *punary(void) {
    if (accept('-')) return neg(punary());
    return ppower();
}

static bool atomstart(void) {
    skip();
    const char c = *ps;
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || c == '(' || c == '.';
}

static node_t *pterm(void) {
    node_t *e = punary();
    for (;;) {
        if (accept('*')) e = mul(e, punary());
        else if (accept('/')) e = dvd(e, punary());
        else if (atomstart()) e = mul(e, ppower());
        else return e;
    }
}

static node_t *psum(void) {
    node_t *e = pterm();
    for (;;) {
        if (accept('+')) e = add(e, pterm());
        else if (accept('-')) e = sub(e, pterm());
        else return e;
    }
}

static node_t *pexpr(void) {
    node_t *e = psum();
    if (accept('=')) e = eq(e, psum());
    return e;
}

node_t *parse(const char *s, int *errpos) {
    src = ps = s;
    if (setjmp(cas_jb)) {
        if (errpos) *errpos = (int)(ps - src);
        return NULL;
    }
    node_t *e = pexpr();
    skip();
    if (*ps) perr(*ps == ')' ? "This ) has no ( to match" : "This doesn't make sense here");
    return e;
}

/* ---- display order (shared with the drawing code) ---- */

bool negative(const node_t *e) {
    if (isnum(e)) return numval(e) < 0;
    return e->type == N_MUL && isnum(e->k[0]) && numval(e->k[0]) < 0;
}

/* -e without a -1 factor: flips the numeric coefficient */
node_t *negate(const node_t *e) {
    if (isnum(e)) return nmul(e, num(-1));
    if (e->type != N_MUL || !isnum(e->k[0])) return mul(num(-1), (node_t *)e);
    node_t *c = nmul(e->k[0], num(-1));
    if (!isone(c)) {
        node_t *r = mkv(N_MUL, e->n, e->k);
        r->k[0] = c;
        return r;
    }
    return e->n == 2 ? e->k[1] : mkv(N_MUL, e->n - 1, e->k + 1);
}

static int rank(const node_t *f) {
    if (isnum(f)) return 0;
    if (f->type == N_CONST && f->op == C_PI) return 1;
    const node_t *b = f->type == N_POW && isnum(f->k[1]) ? f->k[0] : f;
    return b->type == N_VAR ? 2 : 3;
}

/* sum terms as stored, with nested raw sums flattened */
static int flatsum(const node_t *e, const node_t **out, int n) {
    for (int i = 0; i < e->n && n < 64; i++)
        if (e->k[i]->type == N_ADD) n = flatsum(e->k[i], out, n);
        else out[n++] = e->k[i];
    return n;
}

static bool varpower(const node_t *e) {
    if (e->type == N_VAR) return e->op != 'k' && e->op != 'C';
    if (e->type == N_POW) return isnum(e->k[1]) && varpower(e->k[0]);
    if (e->type == N_MUL)
        for (int i = 0; i < e->n; i++)
            if (varpower(e->k[i])) return true;
    return false;
}

/* Terms with the integer parameter k go last (pi/6 + 2pik), and so does +C.
 * A sum that is
 * not a polynomial starts with a positive term (sin x - cos x). */
int sumterms(const node_t *e, const node_t **out) {
    const int n = flatsum(e, out, 0);
    const node_t *k[64];
    int m = 0, j = 0;
    bool poly = false;
    for (int i = 0; i < n; i++) poly |= varpower(out[i]);
    if (n == 2 && negative(out[0]) && !negative(out[1]) && !hasvar(out[1], 'C')) {   /* 1 - x^2, not -x^2 + 1 */
        const node_t *t = out[0];
        out[0] = out[1];
        out[1] = t;
    } else if (!poly && negative(out[0]))
        for (int i = 1; i < n; i++)
            if (!negative(out[i]) && !isnum(out[i])) {
                const node_t *t = out[i];
                memmove(out + 1, out, i * sizeof *out);
                out[0] = t;
                break;
            }
    for (int i = 0; i < n; i++)
        if (hasvar(out[i], 'k') || (out[i]->type == N_VAR && out[i]->op == 'C')) k[m++] = out[i];
        else out[j++] = out[i];
    memcpy(out + j, k, m * sizeof *k);
    return n;
}

/* Splits a product into numerator and denominator factors in display order.
 * Returns the sign (1 or -1); the numeric coefficient's magnitude is split too. */
int fracparts(const node_t *e, const node_t **top, int *nt, const node_t **bot, int *nb) {
    const node_t *f[32];
    int n = 0, sign = 1;
    *nt = *nb = 0;
    if (e->type == N_MUL)
        for (int i = 0; i < e->n && n < 32; i++)
            if (e->k[i]->type == N_MUL)   /* a raw nested product */
                for (int j = 0; j < e->k[i]->n && n < 32; j++) f[n++] = e->k[i]->k[j];
            else
                f[n++] = e->k[i];
    else
        f[n++] = e;
    for (int r = 0; r < 4; r++)
        for (int i = 0; i < n; i++) {
            const node_t *x = f[i];
            if (rank(x) != r) continue;
            if (isnum(x)) {
                if (x == f[0] && numval(x) < 0) {
                    sign = -1;
                    x = nmul(x, num(-1));
                    if (isone(x) && n > 1) continue;
                }
                if (x->type == N_NUM && x->v.q.den != 1) {
                    if (x->v.q.num != 1) top[(*nt)++] = num(x->v.q.num);
                    bot[(*nb)++] = num(x->v.q.den);
                } else {
                    top[(*nt)++] = x;
                }
            } else if (x->type == N_POW && negative(x->k[1])) {
                node_t *ex = negate(x->k[1]);
                bot[(*nb)++] = isone(ex) ? x->k[0] : pw(x->k[0], ex);
            } else {
                top[(*nt)++] = x;
            }
        }
    return sign;
}

/* ---- linear printer ---- */

typedef struct { char *p, *end; } out_t;

static void put(out_t *o, const char *s) {
    while (*s && o->p < o->end - 1) *o->p++ = *s++;
    *o->p = 0;
}

static void pr(out_t *o, const node_t *e);

static bool atomic(const node_t *e) {
    return e->type == N_VAR || e->type == N_CONST || e->type == N_FN
        || (e->type == N_NUM && e->v.q.den == 1 && e->v.q.num >= 0)
        || (e->type == N_FLT && e->v.f >= 0);
}

static void prwrap(out_t *o, const node_t *e, bool paren) {
    if (paren) put(o, "(");
    pr(o, e);
    if (paren) put(o, ")");
}

static void prfactors(out_t *o, const node_t **f, int n) {
    for (int i = 0; i < n; i++) {
        const node_t *x = f[i];
        const bool p = n > 1 && (x->type == N_ADD || x->type == N_PM || (i && negative(x)));
        if (i && isnum(x)) put(o, "*");
        prwrap(o, x, p);
    }
}

static void prprod(out_t *o, const node_t *e) {
    const node_t *top[32], *bot[32];
    int nt, nb;
    if (fracparts(e, top, &nt, bot, &nb) < 0) put(o, "-");
    if (!nt) put(o, "1");
    else if (nb && nt == 1) prwrap(o, top[0], top[0]->type == N_ADD || top[0]->type == N_PM);
    else prfactors(o, top, nt);
    if (!nb) return;
    put(o, "/");
    const bool p = nb > 1 || !(atomic(bot[0]) || bot[0]->type == N_POW);
    if (p) put(o, "(");
    prfactors(o, bot, nb);
    if (p) put(o, ")");
}

static const char *const fnames[] = {"sin", "cos", "tan", "asin", "acos", "atan", "ln", "log", "logb", "abs", "ncr", "npr"};

static void pr(out_t *o, const node_t *e) {
    char buf[24];
    switch (e->type) {
        case N_NUM:
            if (e->v.q.den == 1) snprintf(buf, sizeof buf, "%ld", (long)e->v.q.num);
            else snprintf(buf, sizeof buf, "%ld/%ld", (long)e->v.q.num, (long)e->v.q.den);
            put(o, buf);
            break;
        case N_FLT:
            snprintf(buf, sizeof buf, "%.6g", e->v.f);
            put(o, buf);
            break;
        case N_VAR:
            buf[0] = e->op, buf[1] = 0;
            put(o, buf);
            break;
        case N_CONST:
            put(o, (const char *[]){"pi", "e", "i", "inf"}[e->op]);
            break;
        case N_ADD: {
            const node_t *t[64];
            const int n = sumterms(e, t);
            for (int i = 0; i < n; i++) {
                if (i && negative(t[i])) {
                    const node_t *m = negate(t[i]);
                    put(o, "-");
                    prwrap(o, m, m->type == N_ADD);
                    continue;
                }
                if (i) put(o, "+");
                prwrap(o, t[i], t[i]->type == N_PM);
            }
            break;
        }
        case N_MUL:
            if (e->n == 2 && isval(e->k[0], -1) && !isnum(e->k[1]) && e->k[1]->type != N_MUL) {
                put(o, "-");
                prwrap(o, e->k[1], e->k[1]->type == N_ADD || e->k[1]->type == N_PM);
            } else {
                prprod(o, e);
            }
            break;
        case N_POW: {
            const node_t *b = e->k[0], *x = e->k[1];
            if (x->type == N_NUM && x->v.q.num == 1 && x->v.q.den == 2) {
                put(o, "sqrt("), pr(o, b), put(o, ")");
            } else if (negative(x)) {
                prprod(o, e);
            } else {
                prwrap(o, b, !atomic(b) || (b->type == N_NUM && b->v.q.den != 1));
                put(o, "^");
                prwrap(o, x, !atomic(x));
            }
            break;
        }
        case N_FN:
            if (e->op == F_ABS) {
                put(o, "|"), pr(o, e->k[0]), put(o, "|");
                break;
            }
            if (e->op == F_FACT) {
                prwrap(o, e->k[0], !atomic(e->k[0]) || e->k[0]->type == N_FN);
                put(o, "!");
                break;
            }
            put(o, fnames[e->op]);
            put(o, "(");
            for (int i = 0; i < e->n; i++) {
                if (i) put(o, ",");
                pr(o, e->k[i]);
            }
            put(o, ")");
            break;
        case N_EQ:
            pr(o, e->k[0]), put(o, "="), pr(o, e->k[1]);
            break;
        case N_OR:
            for (int i = 0; i < e->n; i++) {
                if (i) put(o, " or ");
                pr(o, e->k[i]);
            }
            break;
        case N_PM: {   /* a +- b, or +-b on its own */
            const node_t *b = e->k[e->n - 1];
            if (e->n == 2) pr(o, e->k[0]);
            put(o, "+-");
            prwrap(o, b, b->type == N_ADD || negative(b));
            break;
        }
        case N_DERIV:
            put(o, "d/d"), pr(o, e->k[1]), put(o, "("), pr(o, e->k[0]), put(o, ")");
            break;
        case N_INTEG:
            put(o, "int");
            if (e->n == 4) put(o, "["), pr(o, e->k[2]), put(o, ","), pr(o, e->k[3]), put(o, "]");
            put(o, "("), pr(o, e->k[0]), put(o, ")d"), pr(o, e->k[1]);
            break;
        case N_LIMIT:
            put(o, "lim["), pr(o, e->k[1]), put(o, "->"), pr(o, e->k[2]), put(o, "]("), pr(o, e->k[0]), put(o, ")");
            break;
        case N_EVAL:
            put(o, "["), pr(o, e->k[0]), put(o, "]_"), prwrap(o, e->k[1], !atomic(e->k[1]));
            put(o, "^"), prwrap(o, e->k[2], !atomic(e->k[2]));
            break;
    }
}

char *print(const node_t *e, char *buf, size_t size) {
    out_t o = {buf, buf + size};
    *buf = 0;
    if (e) pr(&o, e);
    return buf;
}
