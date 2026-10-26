#include <math.h>
#include <string.h>
#include "cas.h"

/* Derivatives and integrals are DERIV / INTEG nodes in the tree. Each pass
 * rewrites every outermost one by one rule, so the steps show the parts
 * still to do. */

static const char *rule;   /* name of the rule the last dstep/istep used */
static node_t *ruleArg;    /* what its '@' stands for */

static node_t *D(node_t *f, node_t *x) {
    return mk(N_DERIV, 2, f, x);
}

static node_t *I(node_t *f, node_t *x) {
    return mk(N_INTEG, 2, f, x);
}

static bool isx(const node_t *e, const node_t *x) {
    return same(e, x);
}

/* splits a product into the factors free of x (c) and the rest */
static node_t *split(node_t *f, node_t *x, node_t **rest) {
    if (f->type != N_MUL) {
        *rest = f;
        return has(f, x) ? num(1) : f;
    }
    node_t *c[48], *r[48];
    int nc = 0, nr = 0;
    for (int i = 0; i < f->n; i++)
        if (has(f->k[i], x)) r[nr++] = f->k[i];
        else c[nc++] = f->k[i];
    *rest = nr == 0 ? num(1) : nr == 1 ? r[0] : mkv(N_MUL, nr, r);
    return nc == 0 ? num(1) : nc == 1 ? c[0] : simp(mkv(N_MUL, nc, c));
}

/* g' times the outer derivative, leaving d/dx(g) when g is not x */
static node_t *chain(node_t *outer, node_t *g, node_t *x) {
    if (isx(g, x)) return outer;
    rule = "Chain rule";
    return mul(outer, D(g, x));
}

/* ---- derivatives ---- */

static node_t *dstep(node_t *f, node_t *x) {
    if (!has(f, x)) {
        rule = "The derivative of a constant is 0";
        return num(0);
    }
    if (isx(f, x)) {
        rule = "Power rule";
        return num(1);
    }
    switch (f->type) {
        case N_ADD: {
            node_t *t[64];
            for (int i = 0; i < f->n; i++) t[i] = D(f->k[i], x);
            rule = "Differentiate term by term";
            return mkv(N_ADD, f->n, t);
        }
        case N_MUL: {
            node_t *rest, *c = split(f, x, &rest);
            if (!isone(c)) {
                rule = "Take out the constant";
                return mul(c, D(rest, x));
            }
            /* a fraction with x on top and bottom: quotient rule */
            const node_t *top[32], *bot[32];
            int nt, nb;
            fracparts(f, top, &nt, bot, &nb);
            if (nb && nt) {
                node_t *n = nt == 1 ? (node_t *)top[0] : mkv(N_MUL, nt, (node_t **)top);
                node_t *d = nb == 1 ? (node_t *)bot[0] : mkv(N_MUL, nb, (node_t **)bot);
                if (has(n, x) && has(d, x)) {
                    rule = "Quotient rule";
                    return dvd(sub(mul(D(n, x), d), mul(n, D(d, x))), pw(d, num(2)));
                }
            }
            node_t *u = f->k[0], *v = f->n == 2 ? f->k[1] : mkv(N_MUL, f->n - 1, f->k + 1);
            rule = "Product rule";
            return add(mul(D(u, x), v), mul(u, D(v, x)));
        }
        case N_POW: {
            node_t *b = f->k[0], *e = f->k[1];
            if (!has(e, x)) {
                rule = "Power rule";
                return chain(simp(mul(e, pw(b, add(e, num(-1))))), b, x);
            }
            if (b->type == N_CONST && b->op == C_E) {
                rule = "The derivative of @ is itself";
                ruleArg = pw(b, x);
                return chain(f, e, x);
            }
            if (!has(b, x)) {
                rule = "Differentiate the power: multiply by ln @";
                ruleArg = b;
                return chain(mul(f, fn(F_LN, b)), e, x);
            }
            rule = "Write it as a power of e";
            return mul(f, D(mul(e, fn(F_LN, b)), x));
        }
        case N_FN: {
            node_t *g = f->k[f->n - 1], *o;
            switch (f->op) {
                case F_SIN: rule = "The derivative of sin is cos"; o = fn(F_COS, g); break;
                case F_COS: rule = "The derivative of cos is -sin"; o = neg(fn(F_SIN, g)); break;
                case F_TAN: rule = "The derivative of tan is 1/cos\xb2"; o = pw(fn(F_COS, g), num(-2)); break;
                case F_ASIN: rule = "The derivative of arcsin"; o = pw(sub(num(1), pw(g, num(2))), frac(-1, 2)); break;
                case F_ACOS: rule = "The derivative of arccos"; o = neg(pw(sub(num(1), pw(g, num(2))), frac(-1, 2))); break;
                case F_ATAN: rule = "The derivative of arctan"; o = pw(add(num(1), pw(g, num(2))), num(-1)); break;
                case F_LN: rule = "The derivative of ln x is 1/x"; o = pw(g, num(-1)); break;
                case F_LOG: rule = "The derivative of log x is 1/(x ln 10)"; o = pw(mul(g, fn(F_LN, num(10))), num(-1)); break;
                case F_LOGB: rule = "The derivative of a log"; o = pw(mul(g, fn(F_LN, f->k[0])), num(-1)); break;
                default: rule = "The derivative of |x|"; o = dvd(g, f); break;
            }
            return chain(simp(o), g, x);
        }
    }
    fail("Can't differentiate this");
    return NULL;
}

static node_t *resolveD(node_t *e);

/* full derivative without steps */
node_t *derive(node_t *f, node_t *x) {
    return simp(resolveD(dstep(f, x)));
}

static node_t *resolveD(node_t *e) {
    if (e->type == N_DERIV) return derive(e->k[0], e->k[1]);
    if (!e->n) return e;
    node_t *k[e->n];
    for (int i = 0; i < e->n; i++) k[i] = resolveD(e->k[i]);
    node_t *r = mkv(e->type, e->n, k);
    r->op = e->op;
    return r;
}

/* ---- integrals ---- */

static node_t *integrate(node_t *f, node_t *x);

/* g = a x + b: returns a, or NULL */
static node_t *linear(node_t *g, node_t *x) {
    poly_t p;
    if (!topoly(g, x, &p) || p.deg != 1 || has(p.c[1], x)) return NULL;
    return p.c[1];
}

/* integral of a single function of a linear argument */
static node_t *table(node_t *f, node_t *x) {
    node_t *g, *a;
    if (f->type == N_POW) {
        node_t *b = f->k[0], *e = f->k[1];
        if (!has(e, x) && (a = linear(b, x))) {
            if (isval(e, -1)) {
                rule = isx(b, x) ? "The integral of 1/x is ln|x|" : "The integral of 1/(ax+b)";
                return simp(dvd(fn(F_LN, fn(F_ABS, b)), a));
            }
            rule = "Power rule: add 1 to the power, divide by it";
            node_t *n1 = simp(add(e, num(1)));
            return simp(dvd(pw(b, n1), mul(n1, a)));
        }
        if (!has(b, x) && (a = linear(e, x))) {
            if (b->type == N_CONST && b->op == C_E) {
                rule = isone(a) ? "The integral of @ is itself" : "Integrate the power of e, divide by @";
                ruleArg = isone(a) ? f : a;
                return simp(dvd(f, a));
            }
            rule = "Integrate the power: divide by ln @";
            ruleArg = b;
            return simp(dvd(f, mul(a, fn(F_LN, b))));
        }
        /* 1/cos^2 */
        if (isval(e, -2) && b->type == N_FN && b->op == F_COS && (a = linear(b->k[0], x))) {
            rule = "The integral of 1/cos\xb2 is tan";
            return simp(dvd(fn(F_TAN, b->k[0]), a));
        }
        /* 1/(1+x^2) and 1/sqrt(1-x^2) */
        if (isval(e, -1) || (e->type == N_NUM && e->v.q.num == -1 && e->v.q.den == 2)) {
            poly_t p;
            if (topoly(b, x, &p) && p.deg == 2 && iszero(p.c[1]) && isone(p.c[0])) {
                if (isval(e, -1) && isone(p.c[2])) {
                    rule = "The integral of 1/(1+x\xb2) is arctan";
                    return fn(F_ATAN, x);
                }
                if (!isval(e, -1) && isval(p.c[2], -1)) {
                    rule = "The integral of 1/\x1a(1\x96x\xb2) is arcsin";
                    return fn(F_ASIN, x);
                }
            }
        }
        return NULL;
    }
    if (f->type != N_FN || !(a = linear(g = f->k[f->n - 1], x))) return NULL;
    switch (f->op) {
        case F_SIN: rule = "The integral of sin is -cos"; return simp(dvd(neg(fn(F_COS, g)), a));
        case F_COS: rule = "The integral of cos is sin"; return simp(dvd(fn(F_SIN, g), a));
        case F_TAN: rule = "The integral of tan is -ln|cos|"; return simp(dvd(neg(fn(F_LN, fn(F_ABS, fn(F_COS, g)))), a));
        case F_LN: rule = "The integral of ln x is x ln x - x"; return simp(dvd(sub(mul(g, f), g), a));
    }
    return NULL;
}

/* LIATE order: lower is picked first as u */
static int liate(node_t *f, node_t *x) {
    if (f->type == N_FN && (f->op == F_LN || f->op == F_LOG || f->op == F_LOGB)) return 0;
    if (f->type == N_FN && f->op >= F_ASIN && f->op <= F_ATAN) return 1;
    poly_t p;
    if (topoly(f, x, &p)) return 2;
    if (f->type == N_FN) return 3;
    return 4;
}

static node_t *uvar;

/* f = c * outer(g) * g' for some inner g: returns c * outer(u) in u */
static node_t *usub(node_t *f, node_t *x, node_t **gout) {
    node_t *cands[16];
    int nc = 0;
    node_t *one = f;
    node_t **fs = f->type == N_MUL ? f->k : &one;
    const int nf = f->type == N_MUL ? f->n : 1;
    for (int i = 0; i < nf && nc < 16; i++) {
        node_t *h = fs[i];
        if (h->type == N_FN && !isx(h->k[h->n - 1], x)) cands[nc++] = h->k[h->n - 1];
        if (h->type == N_POW && has(h->k[0], x) && !isx(h->k[0], x) && !has(h->k[1], x)) cands[nc++] = h->k[0];
        if (h->type == N_POW && !has(h->k[0], x) && !linear(h->k[1], x) && has(h->k[1], x)) cands[nc++] = h->k[1];
        /* sin(x)^n cos(x): u is sin(x) */
        if (h->type == N_POW && h->k[0]->type == N_FN && isx(h->k[0]->k[0], x)) cands[nc++] = h->k[0];
        /* sin(x) cos(x): u is sin(x) */
        if (h->type == N_FN && h->op != F_ABS && isx(h->k[h->n - 1], x)) cands[nc++] = h;
    }
    for (int i = 0; i < nc; i++) {
        node_t *g = cands[i];
        node_t *dg = derive(g, x);
        if (iszero(dg)) continue;
        node_t *u = uvar;
        node_t *in = simp(subst(simp(dvd(f, dg)), g, u));
        if (!has(in, x)) {
            *gout = g;
            return in;
        }
    }
    return NULL;
}

static node_t *partial(node_t *f, node_t *x) {
    const node_t *top[32], *bot[32];
    int nt, nb;
    if (fracparts(f, top, &nt, bot, &nb) < 0 || !nb) return NULL;
    node_t *n = nt == 0 ? num(1) : nt == 1 ? (node_t *)top[0] : simp(mkv(N_MUL, nt, (node_t **)top));
    node_t *d = nb == 1 ? (node_t *)bot[0] : simp(mkv(N_MUL, nb, (node_t **)bot));
    if (fracparts(f, top, &nt, bot, &nb) < 0) n = simp(neg(n));
    poly_t pn, pd;
    if (!topoly(n, x, &pn) || !topoly(d, x, &pd) || !rational(&pn) || !rational(&pd) || pd.deg < 1) return NULL;
    if (pn.deg >= pd.deg) {   /* long division first */
        poly_t q = {.deg = pn.deg - pd.deg}, r = pn;
        for (int i = 0; i <= MAXDEG; i++) q.c[i] = num(0);
        for (int k = pn.deg - pd.deg; k >= 0; k--) {
            node_t *c = nmul(r.c[k + pd.deg], npow(pd.c[pd.deg], -1));
            q.c[k] = c;
            for (int j = 0; j <= pd.deg; j++) r.c[k + j] = nadd(r.c[k + j], nmul(c, nmul(pd.c[j], num(-1))));
        }
        r.deg = pd.deg - 1;
        while (r.deg > 0 && iszero(r.c[r.deg])) r.deg--;
        rule = "Divide the polynomials";
        node_t *res = frompoly(&q, x);
        node_t *rem = frompoly(&r, x);
        return iszero(rem) ? res : add(res, dvd(rem, d));
    }
    if (pd.deg != 2 || pn.deg > 1) return NULL;
    poly_t pp = pd;
    primitive(&pp);
    node_t *r1 = ratroot(&pp);
    if (!r1) return NULL;
    poly_t q;
    synth(&pd, r1, &q);
    node_t *r2 = simp(neg(dvd(q.c[0], q.c[1])));
    if (same(r1, r2)) return NULL;
    /* n/(a(x-r1)(x-r2)) = A/(x-r1) + B/(x-r2), A = n(r1)/(a(r1-r2)) */
    node_t *a = pd.c[2];
    node_t *A = simp(dvd(subst(n, x, r1), mul(a, sub(r1, r2))));
    node_t *B = simp(dvd(subst(n, x, r2), mul(a, sub(r2, r1))));
    rule = "Split into partial fractions";
    return add(dvd(A, simp(sub(x, r1))), dvd(B, simp(sub(x, r2))));
}

/* e^(ax) sin(bx) style: by parts twice brings the integral back */
static node_t *cyclic(node_t *f, node_t *x) {
    if (f->type != N_MUL || f->n != 2) return NULL;
    node_t *ex = NULL, *tr = NULL;
    for (int i = 0; i < 2; i++) {
        node_t *h = f->k[i];
        if (h->type == N_POW && h->k[0]->type == N_CONST && h->k[0]->op == C_E) ex = h;
        if (h->type == N_FN && (h->op == F_SIN || h->op == F_COS)) tr = h;
    }
    node_t *a, *b;
    if (!ex || !tr || !(a = linear(ex->k[1], x)) || !(b = linear(tr->k[0], x))) return NULL;
    node_t *s = fn(F_SIN, tr->k[0]), *c = fn(F_COS, tr->k[0]);
    node_t *top = tr->op == F_SIN ? sub(mul(a, s), mul(b, c)) : add(mul(a, c), mul(b, s));
    rule = "By parts twice, the integral comes back: solve for it";
    return simp(dvd(mul(ex, top), add(pw(a, num(2)), pw(b, num(2)))));
}

static int parts;   /* by-parts rounds so far */

static node_t *istep(node_t *f, node_t *x) {
    node_t *r;
    if (!has(f, x)) {
        rule = "The integral of a constant";
        return mul(f, x);
    }
    if (f->type == N_ADD) {
        node_t *t[64];
        for (int i = 0; i < f->n; i++) t[i] = I(f->k[i], x);
        rule = "Integrate term by term";
        return mkv(N_ADD, f->n, t);
    }
    node_t *rest, *c = split(f, x, &rest);
    if (!isone(c)) {
        rule = "Take out the constant";
        return mul(c, I(rest, x));
    }
    if (isx(f, x)) {
        rule = "Power rule: add 1 to the power, divide by it";
        return simp(mul(frac(1, 2), pw(x, num(2))));
    }
    if ((r = table(f, x))) return r;
    /* sin^2 and cos^2: lower the power */
    if (f->type == N_POW && isval(f->k[1], 2) && f->k[0]->type == N_FN
        && (f->k[0]->op == F_SIN || f->k[0]->op == F_COS) && linear(f->k[0]->k[0], x)) {
        const bool s = f->k[0]->op == F_SIN;
        node_t *c2 = fn(F_COS, simp(mul(num(2), f->k[0]->k[0])));
        rule = s ? "Use sin²x = (1  cos 2x)/2" : "Use cos²x = (1 + cos 2x)/2";
        return I(simp(mul(frac(1, 2), s ? sub(num(1), c2) : add(num(1), c2))), x);
    }
    /* a product or power of polynomials: expand */
    poly_t p;
    if (f->type != N_ADD && topoly(f, x, &p)) {
        rule = "Expand";
        return I(frompoly(&p, x), x);
    }
    if ((r = partial(f, x))) {
        if (r->type == N_ADD) {
            node_t *t[64];
            for (int i = 0; i < r->n; i++) t[i] = I(r->k[i], x);
            return mkv(N_ADD, r->n, t);
        }
        return I(r, x);
    }
    node_t *g;
    if ((r = usub(f, x, &g))) {
        node_t *F = integrate(r, uvar);
        if (F) {
            substep("Let u = @", g, eq(mk(N_DERIV, 2, uvar, x), derive(g, x)));
            substep("The integral becomes", NULL, I(r, uvar));
            substep("Integrate", NULL, F);
            rule = "Substitute u = @";
            ruleArg = g;
            return simp(subst(F, uvar, g));
        }
    }
    if ((r = cyclic(f, x))) return r;
    if (f->type == N_MUL && parts < 3) {
        /* by parts: u is the LIATE-first factor */
        for (int pick = 0; pick <= 4; pick++)
            for (int i = 0; i < f->n; i++) {
                if (liate(f->k[i], x) != pick) continue;
                node_t *u = f->k[i];
                node_t *dv[48];
                int nd = 0;
                for (int j = 0; j < f->n; j++)
                    if (j != i) dv[nd++] = f->k[j];
                node_t *dvv = nd == 1 ? dv[0] : simp(mkv(N_MUL, nd, dv));
                node_t *v = integrate(dvv, x);
                if (!v) continue;
                node_t *du = derive(u, x);
                parts++;
                substep("u = @", u, eq(mk(N_DERIV, 2, var('u'), x), du));
                substep("dv = @, so", dvv, eq(var('v'), v));
                rule = "Integrate by parts with u = @";
                ruleArg = u;
                return sub(simp(mul(u, v)), I(simp(mul(v, du)), x));
            }
    }
    /* ln x alone, or u = 1 */
    if (f->type == N_FN && f->op == F_LN && parts < 3) {
        parts++;
        rule = "Integrate by parts with u = @";
        ruleArg = f;
        return sub(simp(mul(x, f)), I(simp(mul(x, derive(f, x))), x));
    }
    return NULL;
}

static node_t *resolveI(node_t *e);

static int idepth;

/* full integral without steps, or NULL */
static node_t *integrate(node_t *f, node_t *x) {
    if (++idepth > 8) {
        idepth--;
        return NULL;
    }
    const int ns = nsteps;
    const bool was = cas_logging;
    cas_logging = false;
    node_t *r = istep(f, x);
    if (r) r = resolveI(r);
    nsteps = ns;   /* sub-steps of the inner work are not shown */
    cas_logging = was;
    idepth--;
    return r ? simp(r) : NULL;
}

static node_t *resolveI(node_t *e) {
    if (e->type == N_INTEG) return integrate(e->k[0], e->k[1]);
    if (!e->n) return e;
    node_t *k[e->n];
    for (int i = 0; i < e->n; i++) {
        k[i] = resolveI(e->k[i]);
        if (!k[i]) return NULL;
    }
    node_t *r = mkv(e->type, e->n, k);
    r->op = e->op;
    return r;
}

/* ---- passes with steps ---- */

static int nrules;
static const char *lastRule;
static node_t *lastArg;


static bool calcNode(const node_t *e) {
    return e->type == N_DERIV || e->type == N_INTEG;
}

static node_t *pass(node_t *e) {
    if (calcNode(e)) {
        node_t *f = e->k[0], *x = e->k[1];
        rule = NULL;
        ruleArg = NULL;
        uvar = var('u');
        node_t *r = e->type == N_DERIV ? dstep(f, x) : istep(f, x);
        if (!r) fail("No method found for this integral");
        nrules++;
        lastRule = rule;
        lastArg = ruleArg;
        substep(rule, ruleArg, eq(e, r));
        return r;
    }
    if (!e->n) return e;
    node_t *k[e->n];
    for (int i = 0; i < e->n; i++) k[i] = pass(e->k[i]);
    node_t *r = mkv(e->type, e->n, k);
    r->op = e->op;
    return r;
}

static bool hasCalc(const node_t *e) {
    if (calcNode(e)) return true;
    for (int i = 0; i < e->n; i++)
        if (hasCalc(e->k[i])) return true;
    return false;
}

/* simplifies the parts of e that are already worked out */
static node_t *tidyParts(node_t *e) {
    if (calcNode(e)) return e;
    if (!hasCalc(e)) return simp(e);
    if (!e->n) return e;
    node_t *k[e->n];
    for (int i = 0; i < e->n; i++) k[i] = tidyParts(e->k[i]);
    node_t *r = mkv(e->type, e->n, k);
    r->op = e->op;
    return r;
}

node_t *limit(node_t *e);

static node_t *definite(node_t *F, node_t *x, node_t *a, node_t *b) {
    step("Evaluate between the bounds", NULL, mk(N_EVAL, 3, F, a, b));
    node_t *Fb = subst(F, x, b), *Fa = subst(F, x, a);
    step("Put in the bounds", NULL, sub(Fb, Fa));
    return tidy(sub(Fb, Fa), "Calculate");
}

static double simpson(node_t *f, char v, double a, double b, bool *ok) {
    const int n = 200;
    const double h = (b - a) / n;
    double s = approx(f, v, a, ok) + approx(f, v, b, ok);
    for (int i = 1; i < n; i++) s += approx(f, v, a + i * h, ok) * (i & 1 ? 4 : 2);
    return s * h / 3;
}

static node_t *antider(node_t *e, node_t *lo, node_t *hi, node_t *x);

node_t *calc(node_t *e) {
    if (e->type == N_LIMIT) return limit(e);
    /* a definite integral: find the antiderivative, then put in the bounds */
    node_t *lo = NULL, *hi = NULL, *x = NULL;
    if (e->type == N_INTEG && e->n == 4) {
        x = e->k[1], lo = simp(e->k[2]), hi = simp(e->k[3]);
        node_t *f = e->k[0];
        jmp_buf saved;
        memcpy(saved, cas_jb, sizeof saved);
        const int ns = nsteps;
        if (setjmp(cas_jb)) {   /* no antiderivative: Simpson's rule */
            memcpy(cas_jb, saved, sizeof saved);
            nsteps = ns;
            bool ok = true;
            const double v = simpson(f, x->op, approx(lo, 0, 0, &ok), approx(hi, 0, 0, &ok), &ok);
            if (!ok) fail("Can't evaluate this integral");
            node_t *r = flt(v);
            step("No exact method found, so it was worked out numerically", NULL, r);
            return r;
        }
        node_t *r = antider(I(f, x), lo, hi, x);
        memcpy(cas_jb, saved, sizeof saved);
        return r;
    }
    return antider(e, NULL, NULL, NULL);
}

static node_t *antider(node_t *e, node_t *lo, node_t *hi, node_t *x) {
    const bool indefinite = e->type == N_INTEG;
    if (indefinite && !x) x = e->k[1];
    parts = 0;
    for (int round = 0; hasCalc(e); round++) {
        if (round == 24) fail("Too many steps");
        nrules = 0;
        node_t *r = tidyParts(pass(e));
        if (nrules == 1) {
            nsteps--;   /* one rule: its sub-step is the step itself */
            step(lastRule, lastArg, r);
        } else {
            step("Work out each part", NULL, r);
        }
        e = r;
    }
    e = tidy(e, "Simplify");
    if (lo) return definite(e, x, lo, hi);
    if (indefinite) {
        node_t *r = mk(N_ADD, 2, e, var('C'));
        step("Add the constant of integration", NULL, r);
        return r;
    }
    return e;
}

/* ---- limits ---- */

node_t *limit(node_t *e) {
    node_t *f = simp(e->k[0]), *x = e->k[1], *a = simp(e->k[2]);
    const bool inf = a->type == N_CONST && a->op == C_INF;
    const bool minf = a->type == N_MUL && a->n == 2 && isval(a->k[0], -1) && a->k[1]->type == N_CONST && a->k[1]->op == C_INF;
    const node_t *top[32], *bot[32];
    int nt, nb;
    for (int round = 0; round < 4; round++) {
        const int sign = fracparts(f, top, &nt, bot, &nb);
        node_t *n = nt == 0 ? num(1) : nt == 1 ? (node_t *)top[0] : simp(mkv(N_MUL, nt, (node_t **)top));
        node_t *d = nb == 0 ? num(1) : nb == 1 ? (node_t *)bot[0] : simp(mkv(N_MUL, nb, (node_t **)bot));
        if (sign < 0) n = simp(neg(n));
        if (inf || minf) {
            poly_t pn, pd;
            if (topoly(n, x, &pn) && topoly(d, x, &pd)) {
                node_t *xm = pw(x, num(pd.deg));
                if (pd.deg) step("Divide the top and bottom by @", pd.deg == 1 ? x : xm,
                                 dvd(simp(expand(dvd(n, xm))), simp(expand(dvd(d, xm)))));
                node_t *r;
                if (pn.deg < pd.deg) r = num(0);
                else if (pn.deg == pd.deg) r = simp(dvd(pn.c[pn.deg], pd.c[pd.deg]));
                else {
                    const bool pos = numval(simp(dvd(pn.c[pn.deg], pd.c[pd.deg]))) > 0;
                    const bool odd = (pn.deg - pd.deg) & 1;
                    r = (pos != (minf && odd)) ? cnst(C_INF) : neg(cnst(C_INF));
                }
                step("Terms with x on the bottom go to 0", NULL, r);
                return r;
            }
            /* numeric estimate */
            bool ok = true;
            const double s = minf ? -1 : 1;
            const double v1 = approx(f, x->op, s * 1e4, &ok), v2 = approx(f, x->op, s * 1e6, &ok);
            if (!ok) fail("Can't work out this limit");
            if (fabs(v2) > 1e8) {
                node_t *r = v2 > 0 ? cnst(C_INF) : neg(cnst(C_INF));
                step("The values grow without bound", NULL, r);
                return r;
            }
            if (fabs(v1 - v2) > 1e-3 * (1 + fabs(v2))) fail("Can't work out this limit");
            node_t *r = flt(v2);
            step("Estimated from values of x far out", NULL, r);
            return r;
        }
        node_t *dn = simp(subst(d, x, a));
        if (!iszero(dn)) {
            node_t *r = simp(subst(f, x, a));
            step("Put in @", eq(x, a), r);
            return r;
        }
        node_t *nn = simp(subst(n, x, a));
        if (!iszero(nn)) {
            bool ok = true;
            const double av = approx(a, 0, 0, &ok), h = 1e-6;
            const double l = approx(f, x->op, av - h, &ok), r = approx(f, x->op, av + h, &ok);
            if (ok && l > 0 && r > 0) {
                step("The bottom goes to 0 and the values grow without bound", NULL, cnst(C_INF));
                return cnst(C_INF);
            }
            if (ok && l < 0 && r < 0) {
                step("The bottom goes to 0 and the values fall without bound", NULL, neg(cnst(C_INF)));
                return neg(cnst(C_INF));
            }
            fail("The limit does not exist");
        }
        /* 0/0 */
        poly_t pn, pd;
        if (topoly(n, x, &pn) && topoly(d, x, &pd) && rational(&pn) && rational(&pd)) {
            node_t *lin = simp(sub(x, a));
            poly_t qn, qd;
            synth(&pn, a, &qn);
            synth(&pd, a, &qd);
            node_t *fn_ = frompoly(&qn, x), *fd = frompoly(&qd, x);
            step("0/0: factor the top and bottom", NULL,
                 dvd(mul(lin, fn_), isone(fd) ? lin : mul(lin, fd)));
            f = simp(dvd(fn_, fd));
            step("Cancel @", lin, mk(N_LIMIT, 3, f, x, a));
            continue;
        }
        node_t *dn2 = derive(n, x), *dd2 = derive(d, x);
        step("0/0: use L'Hopital's rule", NULL, mk(N_LIMIT, 3, dvd(dn2, dd2), x, a));
        f = simp(dvd(dn2, dd2));
    }
    fail("Can't work out this limit");
    return NULL;
}
