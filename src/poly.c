#include <string.h>
#include "cas.h"

/* e as c[0] + c[1]x + ... + c[deg]x^deg, coefficients free of x.
 * x may be any subexpression (sin(x) for a substitution). */
bool topoly(node_t *e, const node_t *x, poly_t *p) {
    memset(p, 0, sizeof *p);
    for (int i = 0; i <= MAXDEG; i++) p->c[i] = num(0);
    e = expand(e);
    node_t *one = e;
    node_t **terms = e->type == N_ADD ? e->k : &one;
    const int n = e->type == N_ADD ? e->n : 1;
    for (int i = 0; i < n; i++) {
        node_t *t = terms[i], *f1 = t;
        node_t **f = t->type == N_MUL ? t->k : &f1;
        const int nf = t->type == N_MUL ? t->n : 1;
        node_t *c[48];
        int nc = 0;
        int32_t k = 0;
        for (int j = 0; j < nf; j++) {
            int32_t m;
            if (same(f[j], x)) k += 1;
            else if (f[j]->type == N_POW && same(f[j]->k[0], x) && isint(f[j]->k[1], &m) && m > 0) k += m;
            else if (has(f[j], x)) return false;
            else c[nc++] = f[j];
        }
        if (k > MAXDEG) return false;
        node_t *cf = nc ? simp(nc == 1 ? c[0] : mkv(N_MUL, nc, c)) : num(1);
        p->c[k] = simp(add(p->c[k], cf));
    }
    p->deg = 0;
    for (int i = MAXDEG; i > 0; i--)
        if (!iszero(p->c[i])) {
            p->deg = i;
            break;
        }
    return true;
}

node_t *frompoly(const poly_t *p, node_t *x) {
    node_t *t[MAXDEG + 1];
    int n = 0;
    for (int i = p->deg; i >= 0; i--) {
        if (iszero(p->c[i])) continue;
        node_t *xp = i == 0 ? num(1) : i == 1 ? x : pw(x, num(i));
        t[n++] = i == 0 ? p->c[i] : isone(p->c[i]) ? xp : mul(p->c[i], xp);
    }
    if (!n) return num(0);
    return simp(n == 1 ? t[0] : mkv(N_ADD, n, t));
}

bool rational(const poly_t *p) {
    for (int i = 0; i <= p->deg; i++)
        if (p->c[i]->type != N_NUM) return false;
    return true;
}

static node_t *horner(const poly_t *p, node_t *r) {
    node_t *v = num(0);
    for (int i = p->deg; i >= 0; i--) v = nadd(nmul(v, r), p->c[i]);
    return v;
}

/* p / (x - r) for a root r */
void synth(const poly_t *p, node_t *r, poly_t *out) {
    memset(out, 0, sizeof *out);
    for (int i = 0; i <= MAXDEG; i++) out->c[i] = num(0);
    node_t *carry = num(0);
    for (int i = p->deg; i > 0; i--) {
        carry = nadd(nmul(carry, r), p->c[i]);
        out->c[i - 1] = carry;
    }
    out->deg = p->deg - 1;
}

static int32_t gcd32(int32_t a, int32_t b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b) {
        int32_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* integer coefficients with no common factor; returns the factor taken out */
node_t *primitive(poly_t *p) {
    int32_t l = 1, g = 0;
    for (int i = 0; i <= p->deg; i++) {
        const int32_t d = p->c[i]->v.q.den;
        l = l / gcd32(l, d) * d;
    }
    for (int i = 0; i <= p->deg; i++) {
        p->c[i] = nmul(p->c[i], num(l));
        g = gcd32(g, p->c[i]->v.q.num);
    }
    if (p->c[p->deg]->v.q.num < 0) g = -g;
    for (int i = 0; i <= p->deg; i++) p->c[i] = nmul(p->c[i], frac(1, g));
    return frac(g, l);
}

/* a rational root of an integer polynomial, or NULL */
node_t *ratroot(const poly_t *p) {
    if (iszero(p->c[0])) return num(0);
    int32_t a0 = p->c[0]->v.q.num, an = p->c[p->deg]->v.q.num;
    if (a0 < 0) a0 = -a0;
    if (an < 0) an = -an;
    if (a0 > 100000 || an > 100000) return NULL;   /* ponytail: divisor search capped */
    for (int32_t q = 1; q <= an; q++) {
        if (an % q) continue;
        for (int32_t pp = 1; pp <= a0; pp++) {
            if (a0 % pp || gcd32(pp, q) != 1) continue;
            for (int s = 1; s >= -1; s -= 2) {
                node_t *r = frac(s * pp, q);
                if (iszero(horner(p, r))) return r;
            }
        }
    }
    return NULL;
}

/* a fraction of polynomials with the common factors cancelled, logging steps */
node_t *cancel(node_t *e, node_t *x) {
    if (e->type != N_MUL) return e;
    node_t *top[48], *bot = NULL;
    int nt = 0;
    for (int i = 0; i < e->n; i++)
        if (!bot && e->k[i]->type == N_POW && isval(e->k[i]->k[1], -1) && has(e->k[i]->k[0], x)) bot = e->k[i]->k[0];
        else top[nt++] = e->k[i];
    if (!bot) return e;
    node_t *n = nt == 1 ? top[0] : simp(mkv(N_MUL, nt, top));
    poly_t pn, pd;
    if (!topoly(n, x, &pn) || !topoly(bot, x, &pd) || !rational(&pn) || !rational(&pd)) return e;
    node_t *common[MAXDEG];
    int nc = 0;
    while (pd.deg >= 1 && pn.deg >= 1) {
        poly_t pp = pd;
        primitive(&pp);
        node_t *r = ratroot(&pp);
        if (!r || !iszero(simp(subst(frompoly(&pn, x), x, r)))) break;
        poly_t qn, qd;
        synth(&pn, r, &qn);
        synth(&pd, r, &qd);
        pn = qn, pd = qd;
        common[nc++] = simp(sub(x, r));
    }
    if (!nc) return e;
    node_t *cf = nc == 1 ? common[0] : mkv(N_MUL, nc, common);
    step("Factor the top and bottom", NULL, dvd(mul(cf, frompoly(&pn, x)), mul(cf, frompoly(&pd, x))));
    node_t *r = simp(dvd(frompoly(&pn, x), frompoly(&pd, x)));
    step("Cancel @", cf, r);
    return r;
}

/* (q x - p) for the root p/q */
static node_t *linfactor(node_t *x, node_t *r) {
    const int32_t p = r->v.q.num, q = r->v.q.den;
    node_t *qx = q == 1 ? x : mul(num(q), x);
    return p ? simp(add(qx, num(-p))) : qx;
}

/* Factors a polynomial with rational coefficients, logging steps.
 * Returns the factored form (not simplified, so it stays factored). */
node_t *factorpoly(poly_t p, node_t *x) {
    node_t *f[MAXDEG + 2];
    int nf = 0;
    node_t *c = primitive(&p);

    /* common factor: a number and a power of x */
    int low = 0;
    while (low < p.deg && iszero(p.c[low])) low++;
    if (!isone(c) || low) {
        node_t *common = isone(c) ? NULL : c;
        node_t *xs = low ? (low == 1 ? x : pw(x, num(low))) : NULL;
        node_t *cf = common && xs ? mul(common, xs) : common ? common : xs;
        poly_t rest = p;
        for (int i = 0; i <= MAXDEG - low; i++) rest.c[i] = rest.c[i + low];
        for (int i = MAXDEG - low + 1; i <= MAXDEG; i++) rest.c[i] = num(0);
        rest.deg -= low;
        p = rest;
        if (common) f[nf++] = common;
        if (xs) f[nf++] = xs;
        step("Take out the common factor @", cf, mul(cf, frompoly(&p, x)));
    }

    while (p.deg >= 3) {
        node_t *r = ratroot(&p);
        if (!r) break;
        poly_t q;
        synth(&p, r, &q);
        node_t *lf = linfactor(x, r);
        if (r->v.q.den != 1) {   /* keep integer coefficients: divide q by the root's denominator */
            for (int i = 0; i <= q.deg; i++) q.c[i] = nmul(q.c[i], frac(1, r->v.q.den));
        }
        substep("@ is a root, so divide by its factor", eq(x, r), mul(lf, frompoly(&q, x)));
        f[nf++] = lf;
        p = q;
        node_t *shown[MAXDEG + 2];
        memcpy(shown, f, nf * sizeof *f);
        shown[nf] = frompoly(&p, x);
        step("Factor using a root", NULL, mkv(N_MUL, nf + 1, shown));
    }

    if (p.deg == 2) {
        node_t *r1 = ratroot(&p);
        if (r1) {
            poly_t q;
            synth(&p, r1, &q);
            node_t *r2 = simp(neg(dvd(q.c[0], q.c[1])));
            node_t *a = p.c[2], *b = p.c[1], *cc = p.c[0];
            const int32_t q1 = r1->v.q.den, q2 = r2->v.q.den;
            node_t *k = nmul(a, frac(1, q1 * q2));
            node_t *f1 = linfactor(x, r1), *f2 = linfactor(x, r2);
            const char *cap;
            if (same(r1, r2)) {
                cap = "Perfect square";
                f[nf++] = isone(k) ? pw(f1, num(2)) : mul(k, pw(f1, num(2)));
            } else {
                cap = iszero(b) ? "Difference of two squares" : "Factor";
                if (!isone(k)) f[nf++] = k;
                f[nf++] = f1;
                f[nf++] = f2;
                if (!iszero(b) && !iszero(cc)) {
                    /* ac method: split bx, group in pairs */
                    node_t *m1 = nmul(nmul(k, num(-q1)), num(r2->v.q.num));
                    node_t *m2 = nmul(nmul(k, num(-r1->v.q.num)), num(q2));
                    node_t *x2 = pw(x, num(2));
                    substep("Split the middle term", NULL,
                            mk(N_ADD, 4, simp(mul(a, x2)), simp(mul(m1, x)), simp(mul(m2, x)), cc));
                    node_t *g1 = simp(mul(mul(k, num(q1)), x)), *g2 = simp(mul(k, num(-r1->v.q.num)));
                    substep("Group in pairs", NULL, add(mul(g1, f2), mul(g2, f2)));
                }
            }
            node_t *res = nf == 1 ? f[0] : mkv(N_MUL, nf, f);
            step(cap, NULL, res);
            return res;
        }
    }
    if (p.deg >= 1) f[nf++] = frompoly(&p, x);
    return nf == 1 ? f[0] : mkv(N_MUL, nf, f);
}
