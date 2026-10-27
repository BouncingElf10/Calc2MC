#include <math.h>
#include <string.h>
#include "cas.h"

node_t *cas_answer, *cas_approx;

#define MAXSOL 24
typedef struct {
    node_t *v[MAXSOL];
    int n;
} sols_t;

static node_t *X, *LO, *HI;
static bool none;   /* a branch proved there is no solution */
static bool every;  /* the equation holds for every value */
static int level;

static void addsol(sols_t *s, node_t *v) {
    v = simp(v);
    for (int i = 0; i < s->n; i++)
        if (same(s->v[i], v)) return;
    if (s->n < MAXSOL) s->v[s->n++] = v;
}

static node_t *orlist(node_t **v, int n) {
    node_t *k[MAXSOL];
    for (int i = 0; i < n; i++) k[i] = eq(X, v[i]);
    node_t *e = mkv(N_OR, n, k);
    return e;
}

static int terms(node_t *e, node_t **out) {
    if (e->type != N_ADD) {
        out[0] = e;
        return 1;
    }
    memcpy(out, e->k, e->n * sizeof *out);
    return e->n;
}

static node_t *sumof(node_t **t, int n) {
    return n == 0 ? num(0) : n == 1 ? t[0] : simp(mkv(N_ADD, n, t));
}

static bool isneg(const node_t *e) {
    if (isnum(e)) return numval(e) < 0;
    return e->type == N_MUL && isnum(e->k[0]) && numval(e->k[0]) < 0;
}

/* logs "cap" with the raw both-sides form as a sub-step; returns the tidy result */
static node_t *both(const char *cap, node_t *arg, node_t *l, node_t *r) {
    node_t *raw = eq(l, r);
    substep("Do the same to both sides", NULL, raw);
    node_t *res = simp(raw);
    step(cap, arg, res);
    return res;
}

static node_t *subtractBoth(node_t *l, node_t *r, node_t *t) {
    if (isneg(t)) return both("Add @ to both sides", negate(t), add(l, negate(t)), add(r, negate(t)));
    return both("Subtract @ from both sides", t, sub(l, t), sub(r, t));
}

static node_t *divideBoth(node_t *l, node_t *r, node_t *c) {
    if (isval(c, -1)) return both("Multiply both sides by -1", NULL, neg(l), neg(r));
    return both("Divide both sides by @", c, dvd(l, c), dvd(r, c));
}

static void solveEq(node_t *l, node_t *r, sols_t *s);

/* solves each equation one level down, so its steps become sub-steps */
static void branch(node_t **eqs, int n, sols_t *s) {
    cas_depth++;
    for (int i = 0; i < n; i++) solveEq(eqs[i]->k[0], eqs[i]->k[1], s);
    cas_depth--;
}

/* ---- polynomials ---- */

/* the terms of e with x (or without), as one sum; NULL when there are none */
static node_t *part(node_t *e, bool withX) {
    node_t *t[e->type == N_ADD ? e->n : 1];
    int n = terms(e, t), m = 0;
    for (int i = 0; i < n; i++)
        if (has(t[i], X) == withX) t[m++] = t[i];
    return m ? sumof(t, m) : NULL;
}

static void linear(node_t *l, node_t *r, sols_t *s) {
    node_t *e = eq(l, r), *rx = part(r, true);
    if (rx) e = subtractBoth(e->k[0], e->k[1], rx);
    if (!has(e->k[0], X)) {   /* the x terms ended up on the right */
        e = eq(e->k[1], e->k[0]);
        step("Swap the sides", NULL, e);
    }
    node_t *lc = part(e->k[0], false);
    if (lc) e = subtractBoth(e->k[0], e->k[1], lc);
    poly_t p;
    topoly(e->k[0], X, &p);
    if (!same(e->k[0], X)) {
        if (!same(simp(mul(p.c[1], X)), e->k[0])) e = both("Simplify", NULL, mul(p.c[1], X), e->k[1]);
        if (!isone(p.c[1])) e = divideBoth(e->k[0], e->k[1], p.c[1]);
    }
    addsol(s, e->k[1]);
}

/* (-b ± d)/(2a) with a common integer factor cancelled */
static node_t *pmform(node_t *mb, node_t *rootD, node_t *a2) {
    int32_t x, y, z, k = 1;
    node_t *rest = rootD;
    if (rootD->type == N_MUL && isint(rootD->k[0], &y)) rest = rootD->n == 2 ? rootD->k[1] : mkv(N_MUL, rootD->n - 1, rootD->k + 1);
    else y = 1;
    if (isint(mb, &x) && isint(a2, &z)) {
        int32_t g = y < 0 ? -y : y, h = x < 0 ? -x : x, m = z < 0 ? -z : z;
        while (h) { int32_t t = g % h; g = h; h = t; }
        h = g, g = m;
        while (h) { int32_t t = g % h; g = h; h = t; }
        k = g;
        if (z < 0) k = -k;
    }
    node_t *top = mk(N_PM, 2, frac(x / k, 1), simp(mul(num(y / k), rest)));
    if (!isint(mb, NULL) || !isint(a2, NULL)) top = mk(N_PM, 2, mb, rootD), k = 1;
    node_t *den = isint(a2, NULL) ? num(z / k) : a2;
    if (isint(top->k[0], &x) && x == 0) top = mk(N_PM, 1, top->k[1]);
    return isone(den) ? top : dvd(top, den);
}

/* (-b ± root)/2a kept as one fraction instead of simp's split sum */
static void addroot(sols_t *s, node_t *mb, node_t *root, node_t *a2, int sign) {
    node_t *f = pmform(mb, root, a2);
    node_t *top = f->type == N_PM ? f : f->k[0];
    node_t *den = f->type == N_PM ? NULL : f->k[1]->k[0];
    node_t *r = top->k[top->n - 1];
    if (sign < 0) r = simp(neg(r));
    node_t *v = top->n == 2 && !iszero(top->k[0]) ? mk(N_ADD, 2, top->k[0], r) : r;
    if (den) v = dvd(v, den);
    for (int i = 0; i < s->n; i++)
        if (same(s->v[i], v)) return;
    if (s->n < MAXSOL) s->v[s->n++] = v;
}

static void quadratic(const poly_t *p, sols_t *s) {
    node_t *a = p->c[2], *b = p->c[1], *c = p->c[0];
    node_t *A = var('a'), *B = var('b'), *C = var('c');
    substep("The quadratic formula", NULL,
            eq(X, dvd(mk(N_PM, 2, neg(B), pw(sub(pw(B, num(2)), mul(num(4), mul(A, C))), frac(1, 2))), mul(num(2), A))));
    node_t *disc = sub(pw(b, num(2)), mul(num(4), mul(a, c)));
    step("Use the quadratic formula", NULL,
         eq(X, dvd(mk(N_PM, 2, neg(b), pw(disc, frac(1, 2))), mul(num(2), a))));
    node_t *D = simp(disc);
    step("Work out b\xb2\x96" "4ac", NULL, eq(disc, D));
    node_t *a2 = simp(mul(num(2), a)), *mb = simp(neg(b));
    if (isnum(D) && numval(D) < 0) {
        step("b\xb2\x96" "4ac < 0, so there are no real solutions", NULL, NULL);
        node_t *iroot = simp(mul(cnst(C_I), pw(simp(neg(D)), frac(1, 2))));
        step("The complex solutions", NULL, eq(X, pmform(mb, iroot, a2)));
        none = true;
        return;
    }
    if (iszero(D)) {
        step("b\xb2\x96" "4ac = 0, so there is one solution", NULL, eq(X, simp(dvd(mb, a2))));
        addsol(s, dvd(mb, a2));
        return;
    }
    node_t *rootD = simp(pw(D, frac(1, 2)));
    if (!isnum(rootD)) step("Simplify", NULL, eq(X, pmform(mb, rootD, a2)));
    addroot(s, mb, rootD, a2, 1);
    addroot(s, mb, rootD, a2, -1);
}

static void numeric(node_t *d, sols_t *s);
static bool peel(node_t *l, node_t *r, sols_t *s);

static void polynomial(node_t *l, node_t *r, poly_t *p, sols_t *s) {
    if (p->deg == 1) {
        linear(l, r, s);
        return;
    }
    node_t *d = frompoly(p, X);
    if (!iszero(r) || !same(l, d)) step("Move everything to the left", NULL, eq(d, num(0)));
    if (!rational(p)) {
        if (p->deg == 2) quadratic(p, s);
        else fail("Can't solve this");
        return;
    }
    /* x^n = c: take the root directly */
    bool pure = true;
    for (int i = 1; i < p->deg; i++) pure &= iszero(p->c[i]);
    if (pure && !iszero(p->c[0])) {
        node_t *lhs = simp(mul(p->c[p->deg], pw(X, num(p->deg)))), *rhs = simp(neg(p->c[0]));
        if (!same(l, lhs) || !same(r, rhs)) step("Move the number to the right", NULL, eq(lhs, rhs));
        peel(lhs, rhs, s);
        return;
    }
    if (rational(p)) {
        node_t *c = primitive(p);
        if (isval(c, -1)) step("Multiply both sides by -1", NULL, eq(frompoly(p, X), num(0)));
        else if (!isone(c)) step("Divide both sides by @", c, eq(frompoly(p, X), num(0)));
    } else if (isneg(p->c[p->deg])) {
        for (int i = 0; i <= p->deg; i++) p->c[i] = simp(neg(p->c[i]));
        step("Multiply both sides by -1", NULL, eq(frompoly(p, X), num(0)));
    }
    node_t *f = factorpoly(*p, X);
    node_t *one = f;
    node_t **fs = f->type == N_MUL ? f->k : &one;
    const int nf = f->type == N_MUL ? f->n : 1;
    node_t *zs[MAXDEG + 2];
    int nz = 0;
    for (int i = 0; i < nf; i++)
        if (has(fs[i], X)) zs[nz++] = eq(fs[i]->type == N_POW ? fs[i]->k[0] : fs[i], num(0));
    if (nz > 1 || (nz == 1 && f->type == N_MUL)) {
        step("Set each factor to 0", NULL, nz > 1 ? mkv(N_OR, nz, zs) : zs[0]);
        branch(zs, nz, s);
        return;
    }
    poly_t q;
    topoly(zs[0]->k[0], X, &q);
    if (q.deg == 2) quadratic(&q, s);
    else numeric(zs[0]->k[0], s);
}

/* ---- numeric fallback ---- */

static void numeric(node_t *d, sols_t *s) {
    double lo = LO ? approx(LO, 0, 0, &(bool){true}) : -100, hi = HI ? approx(HI, 0, 0, &(bool){true}) : 100;
    const int N = 2000;
    double px = lo, pf = 0;
    bool pok = false;
    node_t *found[MAXSOL];
    int n = 0;
    for (int i = 0; i <= N && n < MAXSOL; i++) {
        const double x = lo + (hi - lo) * i / N;
        bool ok = true;
        const double fx = approx(d, X->op, x, &ok);
        if (ok && fx == 0) {
            found[n++] = flt(x);
        } else if (ok && pok && (fx < 0) != (pf < 0)) {
            double a = px, b = x, fa = pf;
            for (int j = 0; j < 60; j++) {
                const double m = (a + b) / 2;
                bool ok2 = true;
                const double fm = approx(d, X->op, m, &ok2);
                if ((fm < 0) == (fa < 0)) a = m, fa = fm;
                else b = m;
            }
            /* a jump, not a root: the function is large there */
            bool ok3 = true;
            const double v = approx(d, X->op, (a + b) / 2, &ok3);
            if (ok3 && fabs(v) < 1e-3) found[n++] = flt((a + b) / 2);
        }
        px = x, pf = fx, pok = ok;
    }
    if (!n) {
        step("No solutions found", NULL, NULL);
        none = true;
        return;
    }
    for (int i = 0; i < n; i++) addsol(s, found[i]);
    step("Solved numerically", NULL, orlist(found, n));
}

/* ---- trig ---- */

static node_t *period(uint8_t f) {
    node_t *full = cas_deg ? num(360) : mul(num(2), cnst(C_PI));
    return f == F_TAN ? simp(mul(frac(1, 2), full)) : simp(full);
}

static void sortsols(sols_t *s);

static void inInterval(node_t *g, sols_t *s) {
    bool ok = true;
    const double lo = approx(LO, 0, 0, &ok), hi = approx(HI, 0, 0, &ok);
    const double g0 = approx(g, 'k', 0, &ok), g1 = approx(g, 'k', 1, &ok);
    if (!ok || g1 == g0) return;
    const double step = g1 - g0;
    long k0 = (long)ceil((fmin(lo, hi) - g0) / step - 1e-9), k1 = (long)floor((fmax(lo, hi) - g0) / step + 1e-9);
    if (k0 > k1) { long t = k0; k0 = k1; k1 = t; }
    for (long k = k0; k <= k1 && k - k0 < 40; k++) {
        node_t *v = simp(subst(g, var('k'), num((int32_t)k)));
        bool ok2 = true;
        const double x = approx(v, 0, 0, &ok2);
        if (ok2 && x >= lo - 1e-9 && x <= hi + 1e-9) addsol(s, v);
    }
}

static void trig(uint8_t f, node_t *in, node_t *r, sols_t *s) {
    if (isnum(r) && f != F_TAN && fabs(numval(r)) > 1) {
        step("sin and cos are always between -1 and 1, so there is no solution", NULL, NULL);
        none = true;
        return;
    }
    node_t *inv = fn(f + 3, r);
    node_t *a = simp(inv);
    step("Find the base angle", NULL, eq(inv, a));
    node_t *P = period(f), *k = var('k');
    node_t *half = cas_deg ? num(180) : cnst(C_PI);
    node_t *g[2];
    int n = 1;
    g[0] = simp(add(a, mul(P, k)));
    if (f == F_SIN) g[n++] = simp(add(sub(half, a), mul(P, k)));
    if (f == F_COS && !iszero(a) && !same(a, half)) g[n++] = simp(add(neg(a), mul(P, k)));
    if (n == 2 && same(g[0], g[1])) n = 1;
    node_t *ge[2];
    for (int i = 0; i < n; i++) ge[i] = eq(in, g[i]);
    step(f == F_TAN ? "tan repeats every @" : "Add every full turn: @", P, n == 1 ? ge[0] : mkv(N_OR, n, ge));
    sols_t gs = {.n = 0};
    if (same(in, X)) {
        for (int i = 0; i < n; i++) addsol(&gs, g[i]);
    } else {
        branch(ge, n, &gs);
        step("Solve for @", X, orlist(gs.v, gs.n));
    }
    if (!LO) {
        for (int i = 0; i < gs.n; i++) addsol(s, gs.v[i]);
        return;
    }
    const int before = s->n;
    for (int i = 0; i < gs.n; i++) inInterval(gs.v[i], s);
    sols_t got = {.n = 0};
    for (int i = before; i < s->n; i++) got.v[got.n++] = s->v[i];
    sortsols(&got);
    memcpy(s->v + before, got.v, got.n * sizeof *got.v);
    step("Keep the solutions in the interval", NULL, s->n > before ? orlist(s->v + before, s->n - before) : NULL);
    if (s->n == before) none = true;
}

/* ---- peeling one operation off ---- */

static bool polyIn(node_t *e) {
    poly_t p;
    return topoly(e, X, &p);
}

static void noSolution(const char *why) {
    step(why, NULL, NULL);
    none = true;
}

static bool positive(node_t *e) {
    return !isnum(e) || numval(e) > 0;
}

static bool isroot(const node_t *e) {
    return e->type == N_POW && isnum(e->k[1]) && numval(e->k[1]) > 0 && numval(e->k[1]) < 1;
}

static bool peel(node_t *l, node_t *r, sols_t *s) {
    if (!has(l, X) || (has(r, X) && isroot(r) && !isroot(l))) {
        node_t *t = l;
        l = r, r = t;
    }
    if (has(r, X) && !isroot(l) && l->type != N_ADD) return false;
    int32_t n;
    node_t *e;
    switch (l->type) {
        case N_ADD: {
            node_t *t[l->n], *rest[l->n];
            int nt = terms(l, t), nx = 0, nhard = 0, nr = 0;
            for (int i = 0; i < nt; i++) {
                nx += has(t[i], X);
                nhard += has(t[i], X) && !polyIn(t[i]);
            }
            if (nx != 1 && nhard != 1) return false;
            for (int i = 0; i < nt; i++)
                if (nx == 1 ? !has(t[i], X) : !(has(t[i], X) && !polyIn(t[i]))) rest[nr++] = t[i];
            e = subtractBoth(l, r, sumof(rest, nr));
            break;
        }
        case N_MUL: {
            node_t *c[l->n];
            int nc = 0, nx = 0;
            for (int i = 0; i < l->n; i++)
                if (has(l->k[i], X)) nx++;
                else c[nc++] = l->k[i];
            if (nx != 1 || has(r, X)) return false;
            e = divideBoth(l, r, nc == 1 ? c[0] : simp(mkv(N_MUL, nc, c)));
            break;
        }
        case N_POW: {
            node_t *b = l->k[0], *x = l->k[1];
            if (has(b, X) && !has(x, X)) {
                if (isint(x, &n) && n > 1) {
                    if (isnum(r) && numval(r) < 0 && !(n & 1)) {
                        noSolution("An even power is never negative, so there is no solution");
                        return true;
                    }
                    node_t *root = simp(pw(r, frac(1, n)));
                    const char *cap = n == 2 ? "Take the square root of both sides" : "Take the root of both sides";
                    if (n & 1) {
                        e = both(cap, NULL, b, root);
                        break;
                    }
                    step(cap, NULL, eq(b, iszero(root) ? root : mk(N_PM, 1, root)));
                    if (iszero(root)) {
                        e = eq(b, root);
                        break;
                    }
                    node_t *br[] = {eq(b, root), eq(b, simp(neg(root)))};
                    if (same(b, X)) {
                        addsol(s, br[0]->k[1]);
                        addsol(s, br[1]->k[1]);
                        return true;
                    }
                    branch(br, 2, s);
                    return true;
                }
                if (x->type == N_NUM && x->v.q.num == 1) {   /* a root */
                    if (isnum(r) && numval(r) < 0 && !(x->v.q.den & 1)) {
                        noSolution("A square root is never negative, so there is no solution");
                        return true;
                    }
                    const int32_t q = x->v.q.den;
                    e = both(q == 2 ? "Square both sides" : "Raise both sides to the power @", num(q),
                             b, pw(r, num(q)));
                    e = eq(e->k[0], expand(e->k[1]));
                    break;
                }
                if (isnum(x)) {
                    node_t *inv = simp(dvd(num(1), x));
                    e = both("Raise both sides to the power @", inv, b, pw(r, inv));
                    break;
                }
                return false;
            }
            if (!has(b, X) && has(x, X)) {
                if (!positive(r)) {
                    noSolution("A power of a positive number is never 0 or negative");
                    return true;
                }
                if (b->type == N_CONST && b->op == C_E) e = both("Take ln of both sides", NULL, x, fn(F_LN, r));
                else {
                    node_t *lg = mk(N_FN, 2, b, r);
                    lg->op = F_LOGB;
                    e = both("Take log base @ of both sides", b, x, lg);
                }
                break;
            }
            return false;
        }
        case N_FN: {
            node_t *in = l->k[l->n - 1];
            switch (l->op) {
                case F_LN: e = both("Rewrite as a power of e", NULL, in, pw(cnst(C_E), r)); break;
                case F_LOG: e = both("Rewrite as a power of 10", NULL, in, pw(num(10), r)); break;
                case F_LOGB: e = both("Rewrite as a power of @", l->k[0], in, pw(l->k[0], r)); break;
                case F_ABS: {
                    if (isnum(r) && numval(r) < 0) {
                        noSolution("An absolute value is never negative, so there is no solution");
                        return true;
                    }
                    node_t *br[] = {eq(in, r), eq(in, simp(neg(r)))};
                    step("Split into two cases", NULL, mkv(N_OR, iszero(r) ? 1 : 2, br));
                    branch(br, iszero(r) ? 1 : 2, s);
                    return true;
                }
                case F_SIN: case F_COS: case F_TAN:
                    trig(l->op, in, r, s);
                    return true;
                case F_ASIN: e = both("Take sin of both sides", NULL, in, fn(F_SIN, r)); break;
                case F_ACOS: e = both("Take cos of both sides", NULL, in, fn(F_COS, r)); break;
                case F_ATAN: e = both("Take tan of both sides", NULL, in, fn(F_TAN, r)); break;
                default: return false;
            }
            break;
        }
        default:
            return false;
    }
    solveEq(e->k[0], e->k[1], s);
    return true;
}

/* ---- substitution ---- */

static node_t *kernel;

/* finds the one non-polynomial piece of e that contains x */
static bool findKernel(node_t *e) {
    if (!has(e, X) || same(e, X)) return true;
    const bool leaf = e->type == N_FN || (e->type == N_POW && !has(e->k[0], X));
    if (leaf) {
        if (!kernel) kernel = e;
        return same(kernel, e);
    }
    if (e->type == N_POW && !isint(e->k[1], NULL)) return false;
    for (int i = 0; i < e->n; i++)
        if (!findKernel(e->k[i])) return false;
    return true;
}

/* e^(nx) -> (e^x)^n so exponentials share one kernel */
static node_t *splitExp(node_t *e, node_t *base, node_t *g) {
    if (e->type == N_POW && same(e->k[0], base) && has(e->k[1], X)) {
        node_t *m = simp(dvd(e->k[1], g));
        int32_t k;
        if (isint(m, &k) && k > 1) return pw(pw(base, g), m);
    }
    if (!e->n) return e;
    node_t *k[e->n];
    for (int i = 0; i < e->n; i++) k[i] = splitExp(e->k[i], base, g);
    node_t *r = mkv(e->type, e->n, k);
    r->op = e->op;
    return r;
}

static node_t *firstTrig(node_t *e) {
    if (e->type == N_FN && (e->op == F_SIN || e->op == F_COS) && has(e, X)) return e;
    for (int i = 0; i < e->n; i++) {
        node_t *f = firstTrig(e->k[i]);
        if (f) return f;
    }
    return NULL;
}

static node_t *firstExp(node_t *e) {
    if (e->type == N_POW && !has(e->k[0], X) && has(e->k[1], X)) return e;
    for (int i = 0; i < e->n; i++) {
        node_t *f = firstExp(e->k[i]);
        if (f) return f;
    }
    return NULL;
}

/* sin^2 -> 1 - cos^2 (or the other way) when one of them only appears squared */
static node_t *pythag(node_t *e, uint8_t from, node_t *in) {
    if (e->type == N_POW && e->k[0]->type == N_FN && e->k[0]->op == from && same(e->k[0]->k[0], in)) {
        int32_t n;
        if (isint(e->k[1], &n) && !(n & 1))
            return pw(sub(num(1), pw(fn(from == F_SIN ? F_COS : F_SIN, in), num(2))), num(n / 2));
    }
    if (!e->n) return e;
    node_t *k[e->n];
    for (int i = 0; i < e->n; i++) k[i] = pythag(e->k[i], from, in);
    node_t *r = mkv(e->type, e->n, k);
    r->op = e->op;
    return r;
}

static bool substitute(node_t *d, sols_t *s) {
    node_t *ex = firstExp(d);
    if (ex) {
        node_t *g = ex->k[1];
        /* smallest multiple: use the exponent divided by its numeric coefficient */
        if (g->type == N_MUL && isnum(g->k[0])) g = g->n == 2 ? g->k[1] : mkv(N_MUL, g->n - 1, g->k + 1);
        d = splitExp(d, ex->k[0], g);   /* not simplified: that would undo it */
    }
    kernel = NULL;
    if (!findKernel(d) || !kernel) {
        /* sin and cos of the same thing: rewrite one in terms of the other */
        node_t *sinx = firstTrig(d);
        if (!sinx) return false;
        node_t *in = sinx->k[0];
        for (uint8_t from = F_SIN; from <= F_COS; from++) {
            node_t *t = simp(expand(simp(pythag(d, from, in))));
            kernel = NULL;
            if (findKernel(t) && kernel && kernel->type == N_FN) {
                step(from == F_SIN ? "Use sin\xb2x = 1 \x96 cos\xb2x" : "Use cos\xb2x = 1 \x96 sin\xb2x", NULL, eq(t, num(0)));
                d = t;
                break;
            }
            kernel = NULL;
        }
        if (!kernel) return false;
    }
    poly_t p;
    node_t *u = var('u'), *k = kernel, *saveX = X;
    node_t *du = subst(d, kernel, u);
    if (has(du, X) || !topoly(du, u, &p) || p.deg < 2) return false;
    step("Let u = @", k, eq(frompoly(&p, u), num(0)));
    X = u;
    sols_t us = {.n = 0};
    solveEq(frompoly(&p, u), num(0), &us);
    X = saveX;
    if (!us.n) return true;
    node_t *back = mkv(N_OR, us.n, us.v);
    for (int i = 0; i < us.n; i++) back->k[i] = eq(k, us.v[i]);
    step("Put back u = @", k, us.n == 1 ? back->k[0] : back);
    const bool hadNone = none;
    for (int i = 0; i < us.n; i++) {
        none = false;
        solveEq(k, us.v[i], s);
    }
    none = hadNone || (!s->n);
    return true;
}

/* ---- the solver ---- */

static bool hasDenominator(node_t *e) {
    if (e->type == N_POW && has(e->k[0], X) && isnum(e->k[1]) && numval(e->k[1]) < 0) return true;
    for (int i = 0; i < e->n; i++)
        if (hasDenominator(e->k[i])) return true;
    return false;
}

static int denominators(node_t *e, node_t **out, int n) {
    if (e->type == N_POW && has(e->k[0], X) && isnum(e->k[1]) && numval(e->k[1]) < 0) {
        for (int i = 0; i < n; i++)
            if (same(out[i], e->k[0])) return n;
        if (n < 8) out[n++] = e->k[0];
        return n;
    }
    for (int i = 0; i < e->n; i++) n = denominators(e->k[i], out, n);
    return n;
}

static void solveEq(node_t *l, node_t *r, sols_t *s) {
    if (++level > 16) fail("Can't solve this");
    node_t *e = tidy(eq(l, r), NULL);
    l = e->k[0], r = e->k[1];
    if (!has(e, X)) {
        if (same(l, r)) step("This is true for every @", X, NULL), every = true;
        else noSolution("This is never true, so there is no solution");
        level--;
        return;
    }
    node_t *d = simp(sub(l, r));
    poly_t p;

    /* already factored and equal to 0 */
    if (iszero(r) && l->type == N_MUL) {
        node_t *zs[16];
        int nz = 0;
        for (int i = 0; i < l->n && nz < 16; i++)
            if (has(l->k[i], X)) zs[nz++] = eq(l->k[i], num(0));
        if (nz > 1) {
            step("Set each factor to 0", NULL, mkv(N_OR, nz, zs));
            branch(zs, nz, s);
            level--;
            return;
        }
    }
    if (!hasDenominator(d) && topoly(d, X, &p) && p.deg >= 1) {
        polynomial(l, r, &p, s);
    } else if (hasDenominator(d)) {
        node_t *dens[8], *tm[d->type == N_ADD ? d->n : 1];
        const int nd = denominators(d, dens, 0);
        node_t *lcd = nd == 1 ? dens[0] : simp(mkv(N_MUL, nd, dens));
        const int nt = terms(d, tm);
        int32_t l = 1;   /* numeric denominators too, so the result has whole numbers */
        for (int i = 0; i < nt; i++) {
            node_t *c = tm[i]->type == N_MUL ? tm[i]->k[0] : tm[i];
            if (c->type == N_NUM) {
                int32_t a = l, b = c->v.q.den;
                while (b) { int32_t t = a % b; a = b; b = t; }
                l = l / a * c->v.q.den;
            }
        }
        if (l > 1) lcd = simp(mul(num(l), lcd));
        for (int i = 0; i < nt; i++) tm[i] = simp(mul(tm[i], lcd));   /* cancels before expanding */
        node_t *n = expand(mkv(N_ADD, nt, tm));
        if (hasDenominator(n)) {
            numeric(d, s);
            level--;
            return;
        }
        if (!iszero(r)) step("Move everything to the left", NULL, eq(d, num(0)));
        step("Multiply both sides by @", lcd, eq(n, num(0)));
        sols_t t = {.n = 0};
        solveEq(n, num(0), &t);
        for (int i = 0; i < t.n; i++) {
            bool bad = false;
            for (int j = 0; j < nd; j++) bad |= iszero(simp(subst(dens[j], X, t.v[i])));
            if (bad) step("Reject @: it makes a denominator 0", eq(X, t.v[i]), NULL);
            else addsol(s, t.v[i]);
        }
        if (!s->n) none = true;
    } else if (!peel(l, r, s) && !substitute(d, s)) {
        numeric(d, s);
    }
    level--;
}

/* ---- top level ---- */

static void sortsols(sols_t *s) {
    for (int i = 1; i < s->n; i++) {
        node_t *t = s->v[i];
        bool ok = true;
        const double tv = approx(t, 0, 0, &ok);
        int j = i;
        while (j) {
            bool ok2 = true;
            if (approx(s->v[j - 1], 0, 0, &ok2) <= tv || !ok || !ok2) break;
            s->v[j] = s->v[j - 1];
            j--;
        }
        s->v[j] = t;
    }
}

static bool works(node_t *orig, node_t *v) {
    node_t *d = simp(subst(sub(orig->k[0], orig->k[1]), X, v));
    bool ok = true;
    double scale = 1;
    const double val = approx(d, 0, 0, &ok);
    node_t *lv = simp(subst(orig->k[0], X, v));
    scale += fabs(approx(lv, 0, 0, &(bool){true}));
    return !ok || fabs(val) < 1e-6 * scale;
}

/* nCr, nPr and ! only count whole numbers of things */
static bool counts(const node_t *e) {
    if (e->type == N_FN && e->op >= F_NCR) return true;
    for (int i = 0; i < e->n; i++)
        if (counts(e->k[i])) return true;
    return false;
}

static void solveTop(node_t *e, sols_t *s) {
    if (e->type != N_EQ) e = eq(e, num(0));
    level = 0;
    none = every = false;
    solveEq(e->k[0], e->k[1], s);
    /* check every answer in the original equation */
    sols_t ok = {.n = 0};
    const bool whole = counts(e);
    for (int i = 0; i < s->n; i++) {
        int32_t k;
        if (whole && !(isint(s->v[i], &k) && k >= 0))
            step("Reject @: it has to be a whole number", eq(X, s->v[i]), NULL);
        else if (hasvar(s->v[i], 'k') || works(e, s->v[i])) ok.v[ok.n++] = s->v[i];
        else step("Check: @ doesn't work in the original equation", eq(X, s->v[i]), NULL);
    }
    *s = ok;
    sortsols(s);
}

static node_t *approxList(node_t **v, int n) {
    node_t *k[MAXSOL];
    bool exact = true;
    for (int i = 0; i < n; i++) {
        bool ok = true;
        const double a = approx(v[i], 0, 0, &ok);
        if (!ok || hasvar(v[i], 'k')) return NULL;
        exact &= isint(v[i], NULL) || v[i]->type == N_FLT;
        k[i] = flt(a);
    }
    return exact ? NULL : mkv(N_OR, n, k);
}

node_t *derive(node_t *e);
node_t *calc(node_t *e);

bool cas_run(node_t *e, uint8_t action, char v, node_t *lo, node_t *hi) {
    nsteps = 0;
    cas_depth = 0;
    cas_answer = cas_approx = NULL;
    cas_err = NULL;
    if (setjmp(cas_jb)) {
        cas_depth = 0;
        return false;
    }
    X = var(v);
    LO = lo, HI = hi;
    switch (action) {
        case A_SOLVE: {
            sols_t s = {.n = 0};
            solveTop(e, &s);
            if (!s.n) {
                cas_answer = mk(N_OR, 0);
                cas_answer->op = every;   /* 1: every value works */
            } else {
                cas_answer = orlist(s.v, s.n);
                cas_approx = approxList(s.v, s.n);
            }
            return true;
        }
        case A_SIMPLIFY:
            cas_answer = cancel(tidy(e, NULL), X);
            break;
        case A_EXPAND:
            cas_answer = expand(e);
            step("Expand", NULL, cas_answer);
            break;
        case A_FACTOR: {
            node_t *s = simp(e);
            poly_t p;
            if (topoly(s, X, &p) && rational(&p) && p.deg >= 1) cas_answer = factorpoly(p, X);
            else cas_answer = s;
            break;
        }
        case A_APPROX: {
            bool ok = true;
            const double a = approx(simp(e), 0, 0, &ok);
            if (!ok) fail("Can't evaluate this");
            cas_answer = flt(a);
            step("Evaluate", NULL, cas_answer);
            break;
        }
        case A_CALC:
            cas_answer = calc(e);
            break;
    }
    if (!cas_answer) fail("Can't do this");
    if (action != A_APPROX) {
        bool ok = true;
        const double a = approx(cas_answer, 0, 0, &ok);
        if (ok && !isint(cas_answer, NULL) && cas_answer->type != N_FLT) cas_approx = flt(a);
    }
    return true;
}
