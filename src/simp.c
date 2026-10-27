#include <math.h>
#include <string.h>
#include "cas.h"

bool cas_deg;

/* While logging, simp() records each local rewrite as a sub-step and ORs
 * its kind into seen, which tidy() turns into the step caption. */
enum { K_CALC = 1, K_LIKE = 2, K_POWERS = 4, K_DIST = 8, K_ROOT = 16, K_EXACT = 32, K_LOG = 64 };
bool cas_logging;
#define logging cas_logging
static uint8_t seen;

static void note(uint8_t kind, const char *cap, node_t *from, node_t *to) {
    seen |= kind;
    if (logging) substep(cap, NULL, eq(from, to));
}

static node_t *sprod(node_t **u, int n);
static node_t *ssum(node_t **u, int n);
static node_t *spow(node_t *v, node_t *w);

/* ---- powers and roots ---- */

static bool isrational(const node_t *e) {
    return e->type == N_NUM;
}

/* n = m^q * rest with m as large as possible (trial division) */
static int32_t rootpart(int32_t n, int q, int32_t *rest) {
    int32_t m = 1;
    *rest = n;
    if (n > 1000000) return 1;   /* ponytail: no factoring past a million */
    for (int32_t p = 2; p * p <= *rest || (q == 1 && p <= *rest); p++) {
        int32_t pq = 1;
        for (int i = 0; i < q; i++) pq *= p;
        if (pq > *rest) break;
        while (*rest % pq == 0) {
            *rest /= pq;
            m *= p;
        }
    }
    return m;
}

/* a/b ^ (p/q) for a positive rational and non-integer exponent */
static node_t *radical(const node_t *v, int32_t p, int32_t q) {
    int32_t a = v->v.q.num, b = v->v.q.den;
    if (p < 0) {
        int32_t t = a;
        a = b, b = t, p = -p;
    }
    node_t *whole = npow(frac(a, b), p / q);
    const int32_t r = p % q;
    /* (a/b)^(r/q) = (a^r * b^(q-r))^(1/q) / b */
    node_t *in = nmul(npow(num(a), r), npow(num(b), q - r));
    if (!isint(in, NULL)) return NULL;
    int32_t rest, m = rootpart(in->v.q.num, q, &rest);
    node_t *out = nmul(whole, frac(m, b));
    if (rest == 1) return out;
    node_t *root = pw(num(rest), frac(1, q));
    return isone(out) ? root : mul(out, root);
}

static node_t *spow(node_t *v, node_t *w) {
    int32_t n;
    if (iszero(v)) {
        if (isnum(w) && numval(w) > 0) return num(0);
        if (isnum(w) && numval(w) < 0) fail("Division by zero");
        return pw(v, w);
    }
    if (isone(v)) return v;
    if (iszero(w)) return num(1);
    if (isone(w)) return v;
    if (isnum(v) && isnum(w) && (v->type == N_FLT || w->type == N_FLT)) {
        if (numval(v) < 0 && numval(w) != floor(numval(w))) return pw(v, w);
        return flt(pow(numval(v), numval(w)));
    }
    if (isint(w, &n)) {
        if (isnum(v)) {
            node_t *r = npow(v, n);
            if (n > 1 && v->type == N_NUM) note(K_CALC, "Calculate", pw(v, w), r);
            return r;
        }
        if (v->type == N_CONST && v->op == C_I) {
            static const int8_t ip[] = {1, 0, -1, 0};
            const int m = ((n % 4) + 4) % 4;
            return m & 1 ? (m == 1 ? v : neg(v)) : num(ip[m]);
        }
        if (v->type == N_POW) {
            node_t *ex[] = {v->k[1], w};
            return spow(v->k[0], sprod(ex, 2));
        }
        if (v->type == N_MUL) {
            node_t *f[v->n];
            for (int i = 0; i < v->n; i++) f[i] = spow(v->k[i], w);
            return sprod(f, v->n);
        }
        return pw(v, w);
    }
    if (isrational(v) && isrational(w)) {
        const int32_t p = w->v.q.num, q = w->v.q.den;
        if (v->v.q.num < 0) {
            if (!(q & 1)) return pw(v, w);   /* even root of a negative: not real */
            node_t *r = spow(nmul(v, num(-1)), w);
            return p & 1 ? sprod((node_t *[]){num(-1), r}, 2) : r;
        }
        node_t *r = radical(v, p, q);
        if (!r) return pw(v, w);
        if (!same(r, pw(v, w))) {
            note(K_ROOT, "Simplify the root", pw(v, w), r);
            if (r->type == N_MUL) return sprod(r->k, r->n);
        }
        return r;
    }
    /* (x^a)^b = x^(ab), except when that would drop an absolute value */
    if (v->type == N_POW && isnum(w) && isnum(v->k[1])) {
        int32_t in;
        if (!(isint(v->k[1], &in) && !(in & 1))) {
            node_t *ex[] = {v->k[1], w};
            return spow(v->k[0], sprod(ex, 2));
        }
    }
    /* e^(ln u) = u */
    if (v->type == N_CONST && v->op == C_E && w->type == N_FN && w->op == F_LN) {
        note(K_LOG, "e and ln cancel", pw(v, w), w->k[0]);
        return w->k[0];
    }
    return pw(v, w);
}

/* ---- products ---- */

static node_t *basef(node_t *f) {
    return f->type == N_POW ? f->k[0] : f;
}

static node_t *expof(node_t *f) {
    return f->type == N_POW ? f->k[1] : num(1);
}

static void sort(node_t **a, int n, bool desc) {
    for (int i = 1; i < n; i++) {
        node_t *t = a[i];
        int j = i;
        while (j && (desc ? before(a[j - 1], t) : before(t, a[j - 1]))) {
            a[j] = a[j - 1];
            j--;
        }
        a[j] = t;
    }
}

#define MAXOPS 32
#define MAXGRP 16   /* different bases or terms in one product or sum */

static int flatten(node_t **u, int n, uint8_t type, node_t **out) {
    int m = 0;
    for (int i = 0; i < n; i++) {
        if (u[i]->type == type)
            for (int j = 0; j < u[i]->n; j++) {
                if (m == MAXOPS) fail("Too complex");
                out[m++] = u[i]->k[j];
            }
        else {
            if (m == MAXOPS) fail("Too complex");
            out[m++] = u[i];
        }
    }
    return m;
}

static node_t *sprod(node_t **u, int n) {
    node_t *f[MAXOPS], *base[MAXGRP];
    uint8_t g[MAXOPS], nex[MAXGRP];   /* g: the group of each factor */
    int nb = 0, nnum = 0;
    n = flatten(u, n, N_MUL, f);
    node_t *c = num(1);
    for (int i = 0; i < n; i++) {
        if (isnum(f[i])) {
            c = nmul(c, f[i]);
            nnum++;
            continue;
        }
        node_t *b = basef(f[i]);
        int j = 0;
        while (j < nb && !same(base[j], b)) j++;
        if (j == nb) {
            if (nb == MAXGRP) fail("Too complex");
            base[nb] = b, nex[nb++] = 0;
        }
        if (nex[j] == 8) fail("Too complex");
        nex[j]++;
        g[i] = j;
    }
    if (iszero(c)) return c;

    node_t *r[MAXOPS];
    int nr = 0;
    bool again = false;
    for (int j = 0; j < nb; j++) {
        node_t *from[8], *ex[8];
        int k = 0;
        for (int i = 0; i < n; i++)
            if (!isnum(f[i]) && g[i] == j) from[k] = f[i], ex[k++] = expof(f[i]);
        node_t *p = k == 1 ? from[0] : spow(base[j], ssum(ex, k));
        if (k > 1) note(K_POWERS, "Add the exponents", mkv(N_MUL, k, from), p);
        if (isnum(p) || p->type == N_MUL) again = true;
        if (!isone(p)) r[nr++] = p;
    }
    if (again) {
        r[nr++] = c;
        return sprod(r, nr);
    }
    if (nnum > 1 && logging) {
        int k = 0;
        bool sign = false;
        for (int i = 0; i < n; i++)   /* f is done with, so the numbers gather in it */
            if (isnum(f[i])) f[k++] = f[i], sign |= isval(f[i], -1) || isone(f[i]);
        if (!(sign && k == 2)) note(K_CALC, "Calculate", mkv(N_MUL, k, f), c);
    }
    if (!nr) return c;
    sort(r, nr, false);
    /* a number times a sum: distribute */
    if (nr == 1 && r[0]->type == N_ADD && !isone(c)) {
        node_t *s = r[0];
        for (int i = 0; i < s->n; i++) f[i] = sprod((node_t *[]){c, s->k[i]}, 2);
        node_t *d = ssum(f, s->n);
        note(K_DIST, "Distribute", mul(c, s), d);
        return d;
    }
    if (isone(c)) return nr == 1 ? r[0] : mkv(N_MUL, nr, r);
    memmove(r + 1, r, nr * sizeof *r);
    r[0] = c;
    return mkv(N_MUL, nr + 1, r);
}

/* ---- sums ---- */

/* term = coefficient * rest */
static node_t *coef(node_t *t, node_t **rest) {
    if (t->type == N_MUL && isnum(t->k[0])) {
        *rest = t->n == 2 ? t->k[1] : mkv(N_MUL, t->n - 1, t->k + 1);
        return t->k[0];
    }
    *rest = t;
    return num(1);
}

static node_t *withcoef(node_t *c, node_t *rest) {
    if (isone(c)) return rest;
    if (rest->type != N_MUL) return mk(N_MUL, 2, c, rest);
    node_t *k[MAXOPS + 1];
    k[0] = c;
    memcpy(k + 1, rest->k, rest->n * sizeof *k);
    return mkv(N_MUL, rest->n + 1, k);
}

/* ssum() never runs inside itself, so its tables are static to keep them
 * off the 4 KB stack */
static node_t *ssum(node_t **u, int n) {
    static node_t *t[MAXOPS], *rest[MAXGRP], *cf[MAXGRP], *grp[MAXGRP][4], *out[MAXOPS], *nums[MAXOPS];
    static uint8_t ng[MAXGRP];
    int nr = 0, nnum = 0;
    n = flatten(u, n, N_ADD, t);
    node_t *c = num(0);
    for (int i = 0; i < n; i++) {
        if (isnum(t[i])) {
            c = nadd(c, t[i]);
            nnum++;
            continue;
        }
        node_t *r, *k = coef(t[i], &r);
        int j = 0;
        while (j < nr && !same(rest[j], r)) j++;
        if (j == nr) {
            if (nr == MAXGRP) fail("Too complex");
            rest[nr] = r, cf[nr] = num(0), ng[nr++] = 0;
        }
        cf[j] = nadd(cf[j], k);
        if (ng[j] < 4) grp[j][ng[j]++] = t[i];
    }
    int no = 0;
    for (int j = 0; j < nr; j++) {
        node_t *term = iszero(cf[j]) ? NULL : withcoef(cf[j], rest[j]);
        if (ng[j] > 1) note(K_LIKE, "Combine like terms", mkv(N_ADD, ng[j], grp[j]), term ? term : num(0));
        if (term) out[no++] = term;
    }
    if (nnum > 1 && logging) {
        int k = 0;
        bool zero = false;
        for (int i = 0; i < n; i++)
            if (isnum(t[i])) nums[k++] = t[i], zero |= iszero(t[i]);
        if (!zero) note(K_CALC, "Calculate", mkv(N_ADD, k, nums), c);
    }
    sort(out, no, true);
    if (!iszero(c)) out[no++] = c;
    if (!no) return num(0);
    return no == 1 ? out[0] : mkv(N_ADD, no, out);
}

/* ---- functions ---- */

/* the argument as a rational multiple of pi (or of 180 degrees) */
static bool piMultiple(node_t *a, int32_t *p, int32_t *q) {
    node_t *k = NULL;
    if (cas_deg) {
        if (a->type == N_NUM) k = nmul(a, frac(1, 180));
    } else if (iszero(a)) {
        k = num(0);
    } else if (a->type == N_CONST && a->op == C_PI) {
        k = num(1);
    } else if (a->type == N_MUL && a->n == 2 && a->k[0]->type == N_NUM
               && a->k[1]->type == N_CONST && a->k[1]->op == C_PI) {
        k = a->k[0];
    }
    if (!k || k->type != N_NUM) return false;
    *p = k->v.q.num, *q = k->v.q.den;
    return true;
}

/* sin of a whole number of degrees that is a multiple of 30 or 45 */
static node_t *sindeg(int32_t d) {
    d = ((d % 360) + 360) % 360;
    int r = d % 180;
    if (r > 90) r = 180 - r;
    node_t *v;
    switch (r) {
        case 0: return num(0);
        case 30: v = frac(1, 2); break;
        case 45: v = mul(frac(1, 2), pw(num(2), frac(1, 2))); break;
        case 60: v = mul(frac(1, 2), pw(num(3), frac(1, 2))); break;
        case 90: v = num(1); break;
        default: return NULL;
    }
    return d > 180 ? sprod((node_t *[]){num(-1), v}, 2) : v;
}

static node_t *trigExact(uint8_t f, node_t *a) {
    int32_t p, q;
    if (!piMultiple(a, &p, &q) || 180 % q) return NULL;
    const int32_t d = 180 / q * p;
    if (d % 30 && d % 45) return NULL;
    node_t *s = sindeg(d), *c = sindeg(d + 90);
    if (f == F_SIN) return s;
    if (f == F_COS) return c;
    if (iszero(c)) fail("tan is undefined there");
    return simp(dvd(s, c));
}

static node_t *angle(int32_t d) {
    return cas_deg ? num(d) : simp(mul(frac(d, 180), cnst(C_PI)));
}

static node_t *invExact(uint8_t f, node_t *v) {
    static const int16_t sinA[] = {-90, -60, -45, -30, 0, 30, 45, 60, 90};
    static const int16_t cosA[] = {0, 30, 45, 60, 90, 120, 135, 150, 180};
    static const int16_t tanA[] = {-60, -45, -30, 0, 30, 45, 60};
    const int16_t *t = f == F_ASIN ? sinA : f == F_ACOS ? cosA : tanA;
    const int n = f == F_ATAN ? 7 : 9;
    for (int i = 0; i < n; i++) {
        node_t *s = sindeg(t[i]), *c = sindeg(t[i] + 90);
        node_t *x = f == F_ASIN ? s : f == F_ACOS ? c : simp(dvd(s, c));
        if (same(simp(x), v)) return angle(t[i]);
    }
    return NULL;
}

static double toRad(double x) {
    return cas_deg ? x * M_PI / 180 : x;
}

static double fromRad(double x) {
    return cas_deg ? x * 180 / M_PI : x;
}

static double evalfn(uint8_t f, double x, double y) {
    switch (f) {
        case F_SIN: return sin(toRad(x));
        case F_COS: return cos(toRad(x));
        case F_TAN: return tan(toRad(x));
        case F_ASIN: return fromRad(asin(x));
        case F_ACOS: return fromRad(acos(x));
        case F_ATAN: return fromRad(atan(x));
        case F_LN: return log(x);
        case F_LOG: return log10(x);
        case F_LOGB: return log(y) / log(x);
        case F_NCR: case F_NPR: {
            double v = 1;
            for (int i = 0; i < y; i++) v *= f == F_NCR ? (x - i) / (i + 1) : x - i;
            return v;
        }
        case F_FACT: {
            double v = 1;
            for (int i = 2; i <= x; i++) v *= i;
            return v;
        }
        default: return fabs(x);
    }
}

/* ---- counting ---- */

static node_t *bang(int32_t n) {
    return fn(F_FACT, num(n));
}

/* a decimal past what a float holds */
static node_t *fits(node_t *v) {
    if (v->type == N_FLT && !(fabs(v->v.f) < 1e37)) fail("Too large");
    return v;
}

/* nCr and nPr for a whole number r: n(n-1)...(n-r+1), over r! for nCr */
static node_t *counting(uint8_t f, node_t *self, node_t *n, node_t *r) {
    const bool C = f == F_NCR;
    int32_t k, m;
    if (!isint(r, &k)) return NULL;
    if (k < 0) fail("r can't be negative");
    const bool whole = isint(n, &m) && m >= 0;
    if (whole && k > m) fail("r can't be bigger than n");
    if (!isnum(n) && k > MAXDEG) return NULL;   /* in n it stays a polynomial the solver can take */
    if (k > 100) fail("Too complex");
    node_t *from = self;
    if (whole) {
        node_t *ff = dvd(bang(m), C ? mul(bang(k), bang(m - k)) : bang(m - k));
        note(K_EXACT, C ? "Use nCr = n!/(r!(n\x96r)!)" : "Use nPr = n!/(n\x96r)!", from, ff);
        from = ff;
        if (C && m - k < k) k = m - k;   /* the bigger factorial on the bottom cancels */
    }
    node_t *t[MAXDEG], *v = num(1);
    int32_t kf = 1;   /* k!, only needed while k <= MAXDEG */
    for (int i = 0; i < k; i++) {
        node_t *fi = !i ? n : isnum(n) ? nadd(n, num(-i)) : add(n, num(-i));
        if (i < MAXDEG) t[i] = fi, kf *= i + 1;
        if (isnum(n)) v = nmul(v, C ? nmul(fi, frac(1, i + 1)) : fi);
    }
    if (!k) return num(1);
    const char *rule = C ? "Use nCr = n(n\x96" "1)...(n\x96r+1)/r!" : "Use nPr = n(n\x96" "1)...(n\x96r+1)";
    if (!isnum(n)) {
        node_t *top = k == 1 ? t[0] : mkv(N_MUL, k, t);
        note(K_EXACT, rule, from, C && k > 1 ? dvd(top, bang(k)) : top);
        return simp(C ? dvd(top, num(kf)) : top);
    }
    if (whole && k <= MAXDEG) {
        node_t *top = k == 1 ? t[0] : mkv(N_MUL, k, t);
        node_t *shown = C && k > 1 ? dvd(top, bang(k)) : top;
        if (m - k > 1) {
            seen |= K_EXACT;
            if (logging) substep("Cancel @", bang(m - k), eq(from, shown));
        } else {
            note(K_EXACT, "Write out the factorial", from, shown);
        }
        from = shown;
    }
    if (whole) {
        if (!same(from, v)) note(K_CALC, "Calculate", from, v);
    } else {
        note(K_EXACT, rule, from, v);
    }
    return fits(v);
}

static bool isneg(const node_t *e) {
    if (isnum(e)) return numval(e) < 0;
    return e->type == N_MUL && isnum(e->k[0]) && numval(e->k[0]) < 0;
}

static node_t *sfn(uint8_t f, node_t **a, int n) {
    node_t *x = a[n - 1];
    node_t *self = n == 2 ? mk(N_FN, 2, a[0], a[1]) : fn(f, x);
    self->op = f;
    if (n == 1 && x->type == N_FLT) return flt(evalfn(f, x->v.f, 0));
    if (n == 2 && isnum(a[0]) && isnum(x) && (a[0]->type == N_FLT || x->type == N_FLT))
        return flt(evalfn(f, numval(a[0]), numval(x)));

    node_t *r = NULL;
    const char *cap = "Exact value";
    uint8_t kind = K_EXACT;
    switch (f) {
        case F_SIN: case F_COS: case F_TAN:
            if (x->type == N_FN && x->op == f + 3) return x->k[0];
            if (isneg(x)) {
                node_t *p = fn(f, simp(negate(x)));
                p = sfn(f, p->k, 1);
                return f == F_COS ? p : sprod((node_t *[]){num(-1), p}, 2);
            }
            r = trigExact(f, x);
            break;
        case F_ASIN: case F_ACOS: case F_ATAN:
            r = invExact(f, x);
            break;
        case F_LN:
            if (isone(x)) r = num(0);
            else if (x->type == N_CONST && x->op == C_E) r = num(1);
            else if (x->type == N_POW && x->k[0]->type == N_CONST && x->k[0]->op == C_E)
                r = x->k[1], cap = "ln and e cancel", kind = K_LOG;
            else if (iszero(x) || (isnum(x) && numval(x) < 0)) fail("ln is undefined there");
            break;
        case F_LOG: case F_LOGB: {
            node_t *b = f == F_LOG ? num(10) : a[0];
            int32_t k;
            if (isone(x)) r = num(0);
            else if (same(x, b)) r = num(1);
            else if (x->type == N_POW && same(x->k[0], b)) r = x->k[1], kind = K_LOG, cap = "log and power cancel";
            else if (isint(x, NULL) && isint(b, &k) && k > 1) {
                node_t *p = num(1);
                for (int e = 0; e < 31 && numval(p) <= numval(x); e++, p = nmul(p, b))
                    if (same(p, x)) r = num(e);
            }
            if (!r && isnum(x) && numval(x) <= 0) fail("log is undefined there");
            break;
        }
        case F_NCR: case F_NPR:
            if ((r = counting(f, self, a[0], x))) return r;
            break;
        case F_FACT: {
            int32_t m;
            if (!isnum(x)) break;
            if (!isint(x, &m) || m < 0) fail("! needs a whole number");
            if (m > 100) fail("Too large");
            node_t *v = num(1), *from = self;
            for (int i = m; i > 1; i--) v = nmul(v, num(i));
            if (m >= 3 && m <= MAXDEG) {
                node_t *t[MAXDEG];
                for (int i = 0; i < m; i++) t[i] = num(m - i);
                from = mkv(N_MUL, m, t);
                note(K_EXACT, "Multiply down to 1", self, from);
            }
            note(K_CALC, "Calculate", from, v);
            return fits(v);
        }
        case F_ABS:
            if (isnum(x)) r = numval(x) < 0 ? nmul(x, num(-1)) : x, cap = "Calculate", kind = K_CALC;
            else if (isneg(x)) return sfn(f, (node_t *[]){simp(negate(x))}, 1);
            break;
    }
    if (!r) return self;
    note(kind, cap, self, r);
    return r;
}

/* ---- driver ---- */

static node_t *simp1(node_t *e) {
    if (!e->n) return e;
    node_t *k[e->n];
    for (int i = 0; i < e->n; i++) k[i] = simp(e->k[i]);
    switch (e->type) {
        case N_ADD: return ssum(k, e->n);
        case N_MUL: return sprod(k, e->n);
        case N_POW: return spow(k[0], k[1]);
        case N_FN: return sfn(e->op, k, e->n);
        case N_PM:
            if (iszero(k[1])) return k[0];
            break;
    }
    node_t *r = mkv(e->type, e->n, k);
    r->op = e->op;
    return r;
}

node_t *simp(node_t *e) {
    uint8_t *m = cas_mark();
    const int ns = nsteps;
    node_t *r = simp1(e);
    if (!same(r, e)) return r;
    if (nsteps == ns) cas_release(m);
    return e;
}

/* simp() that records what it did as one step with sub-steps */
node_t *tidy(node_t *e, const char *cap) {
    const bool was = logging;
    logging = !cas_depth;
    seen = 0;
    const int ns = nsteps;
    node_t *r = simp(e);
    logging = was;
    if (r == e || !seen) {   /* nothing worth a step: only reordered */
        nsteps = ns;
        return r;
    }
    if (!cap)
        cap = seen & K_LIKE ? "Combine like terms"
            : seen & K_DIST ? "Expand"
            : seen & K_POWERS ? "Simplify the powers"
            : seen & K_ROOT ? "Simplify the roots"
            : seen & (K_EXACT | K_LOG) ? "Evaluate"
            : "Calculate";
    step(cap, NULL, r);
    return r;
}

/* ---- expanding ---- */

static node_t *xprod(node_t *r, node_t *s) {
    if (r->type == N_ADD) {
        node_t *t[r->n];
        for (int i = 0; i < r->n; i++) t[i] = xprod(r->k[i], s);
        return ssum(t, r->n);
    }
    if (s->type == N_ADD) return xprod(s, r);
    return sprod((node_t *[]){r, s}, 2);
}

node_t *expand(node_t *e) {
    if (!e->n) return e;
    node_t *k[e->n];
    for (int i = 0; i < e->n; i++) k[i] = expand(e->k[i]);
    int32_t n;
    switch (e->type) {
        case N_ADD:
            return ssum(k, e->n);
        case N_MUL: {
            node_t *p = k[0];
            for (int i = 1; i < e->n; i++) p = xprod(p, k[i]);
            return p->type == N_MUL ? simp(p) : p;
        }
        case N_POW:
            if (k[0]->type == N_ADD && isint(k[1], &n) && n > 1 && n <= 8) {
                node_t *p = k[0];
                for (int i = 1; i < n; i++) p = xprod(p, k[0]);
                return p;
            }
            return spow(k[0], k[1]);
    }
    node_t *r = mkv(e->type, e->n, k);
    r->op = e->op;
    return simp(r);
}

/* ---- numbers ---- */

double approx(const node_t *e, char v, double at, bool *ok) {
    double a, b;
    switch (e->type) {
        case N_NUM: case N_FLT: return numval(e);
        case N_VAR:
            if (e->op != v) *ok = false;
            return at;
        case N_CONST:
            if (e->op == C_PI) return M_PI;
            if (e->op == C_E) return M_E;
            *ok = false;
            return 0;
        case N_ADD:
            a = 0;
            for (int i = 0; i < e->n; i++) a += approx(e->k[i], v, at, ok);
            return a;
        case N_MUL:
            a = 1;
            for (int i = 0; i < e->n; i++) a *= approx(e->k[i], v, at, ok);
            return a;
        case N_POW:
            a = approx(e->k[0], v, at, ok);
            b = approx(e->k[1], v, at, ok);
            if (a < 0 && b != floor(b)) {
                /* odd roots of negatives are real */
                const double r = 1 / b;
                if (fabs(r - floor(r + .5)) < 1e-9 && ((long)floor(r + .5) & 1)) return -pow(-a, b);
                *ok = false;
            }
            return pow(a, b);
        case N_FN:
            a = approx(e->k[0], v, at, ok);
            b = e->n > 1 ? approx(e->k[1], v, at, ok) : 0;
            return e->n > 1 ? evalfn(e->op, a, b) : evalfn(e->op, a, 0);
    }
    *ok = false;
    return 0;
}
