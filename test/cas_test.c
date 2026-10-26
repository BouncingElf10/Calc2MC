#include <stdio.h>
#include <string.h>
#include "cas.h"
#include "edit.h"

static char mem[60000];
static char buf[512];
static int fails, checks;

static void check(const char *what, const char *got, const char *want) {
    checks++;
    if (strcmp(got, want)) {
        printf("FAIL %s\n  got:  %s\n  want: %s\n", what, got, want);
        fails++;
    }
}

static node_t *in(const char *s) {
    int at;
    cas_reset();
    node_t *e = parse(s, &at);
    if (!e) printf("parse error in %s at %d: %s\n", s, at, cas_err);
    return e;
}

/* parse then print, no simplification */
static void raw(const char *s, const char *want) {
    check(s, print(in(s), buf, sizeof buf), want);
}

/* parse then simplify */
static void sim(const char *s, const char *want) {
    node_t *e = in(s);
    check(s, e ? print(simp(e), buf, sizeof buf) : cas_err, want);
}

/* runs an action; "answer | caption; caption; ..." of the top-level steps */
static void run(const char *s, uint8_t action, char v, const char *lo, const char *hi, const char *want) {
    node_t *e = in(s);
    node_t *l = lo ? parse(lo, NULL) : NULL, *h = hi ? parse(hi, NULL) : NULL;
    if (!e || !cas_run(e, action, v, l, h)) {
        check(s, cas_err ? cas_err : "?", want);
        return;
    }
    char out[1024];
    print(cas_answer, out, sizeof out);
    if (!cas_answer->n && cas_answer->type == N_OR) strcpy(out, cas_answer->op ? "all" : "none");
    strcat(out, " |");
    for (int i = 0; i < nsteps; i++)
        if (steps[i].parent == -1) {
            char a[128] = "";
            const char *at = strchr(steps[i].cap, '@');
            strcat(out, " ");
            if (at) {
                strncat(out, steps[i].cap, at - steps[i].cap);
                strcat(out, print(steps[i].arg, a, sizeof a));
                strcat(out, at + 1);
            } else {
                strcat(out, steps[i].cap);
            }
            strcat(out, ";");
        }
    if (strncmp(out, want, strlen(want))) {
        printf("FAIL %s\n  got:  %s\n  want: %s\n", s, out, want);
        fails++;
    }
    checks++;
}

static void solve(const char *s, const char *want) {
    run(s, A_SOLVE, 'x', NULL, NULL, want);
}

/* prints all steps of a problem, for looking at */
static void show(const char *s, uint8_t action, char v, const char *lo, const char *hi) {
    node_t *e = in(s);
    node_t *l = lo ? parse(lo, NULL) : NULL, *h = hi ? parse(hi, NULL) : NULL;
    if (!e || !cas_run(e, action, v, l, h)) {
        printf("%s: error %s\n", s, cas_err);
        return;
    }
    printf("== %s\n", s);
    for (int i = 0; i < nsteps; i++) {
        char a[256];
        if (steps[i].parent == -2) continue;
        printf("%s%s", steps[i].parent >= 0 ? "      . " : "  ", steps[i].cap ? steps[i].cap : "");
        if (steps[i].arg) printf(" [%s]", print(steps[i].arg, a, sizeof a));
        if (steps[i].e) printf("  ->  %s", print(steps[i].e, buf, sizeof buf));
        printf("\n");
    }
    printf("  answer: %s", print(cas_answer, buf, sizeof buf));
    if (cas_approx) printf("  ~ %s", print(cas_approx, buf, sizeof buf));
    printf("   (%u bytes)\n", (unsigned)cas_used());
}

static ed_t ed;

static void edCheck(const char *what, const char *want) {
    char s[256];
    uint16_t map[256];
    ed_serialize(&ed, s, map, sizeof s);
    check(what, s, want);
}

static void editor(void) {
    ed_clear(&ed);
    ed_text(&ed, "2x+");
    ed_box(&ed, B_FRAC);
    ed_text(&ed, "1");
    ed_right(&ed);
    ed_text(&ed, "x+1");
    edCheck("typing in a fraction", "2x+frac((1),(x+1))");
    ed_right(&ed);
    ed_text(&ed, "=3");
    edCheck("leaving a fraction", "2x+frac((1),(x+1))=3");

    ed_clear(&ed);
    ed_text(&ed, "x");
    ed_box(&ed, B_POW);
    ed_text(&ed, "2");
    ed_text(&ed, "+1");
    edCheck("exponent keeps typing", "x^((2+1))");
    ed_right(&ed);
    ed_text(&ed, "-x");
    edCheck("out of the exponent", "x^((2+1))-x");

    ed_clear(&ed);
    ed_text(&ed, "x");
    ed_box(&ed, B_POW);
    ed_del(&ed);
    edCheck("del removes an empty exponent", "x");
    check("cursor after del", ed.row == 0 && ed.pos == 1 ? "ok" : "bad", "ok");

    ed_clear(&ed);
    ed_box(&ed, B_FRAC);
    ed_text(&ed, "2x");
    ed_down(&ed);
    ed_del(&ed);
    edCheck("del on an empty denominator keeps the top", "2x");

    ed_clear(&ed);
    ed_text(&ed, "y");
    ed_box(&ed, B_SQRT);
    ed_text(&ed, "4");
    ed_right(&ed);
    ed_del(&ed);
    check("del after a box steps in", ed.row != 0 ? "in" : "out", "in");

    ed_clear(&ed);
    ed_box(&ed, B_DEFINT);
    ed_text(&ed, "0");
    ed_up(&ed);
    ed_text(&ed, "1");
    ed_right(&ed);
    ed_text(&ed, "x");
    ed_box(&ed, B_POW);
    ed_text(&ed, "2");
    edCheck("definite integral", "int((x^((2))),(x),(0),(1))");

    ed_clear(&ed);
    ed_text(&ed, "ax");
    ed_insert(&ed, TOKEN + T_SIN);
    ed_text(&ed, "x)=c");
    char s[128], v[8];
    uint16_t map[128];
    ed_serialize(&ed, s, map, sizeof s);
    check("letters multiply", s, "a*x*sin(x)=c");
    v[ed_vars(s, v, 8)] = 0;
    check("variables", v, "xac");
    cas_reset();
    int at;
    check("editor text parses", print(parse(s, &at), buf, sizeof buf), "axsin(x)=c");
}

static void parseErr(const char *s, int pos) {
    int at = -1;
    cas_reset();
    if (parse(s, &at) || at != pos) {
        printf("FAIL parse error for %s: at %d, want %d\n", s, at, pos);
        fails++;
    }
}

int main(int argc, char **argv) {
    if (argc > 1) {   /* ct "problem" [var [lo hi]]: show its steps with plenty of memory */
        static char big[4000000];
        cas_init(big, sizeof big);
        show(argv[1], strchr(argv[1], '=') ? A_SOLVE : A_CALC, argc > 2 ? argv[2][0] : 'x',
             argc > 4 ? argv[3] : NULL, argc > 4 ? argv[4] : NULL);
        return 0;
    }
    cas_init(mem, sizeof mem);

    raw("2x+3=7", "2x+3=7");
    raw("x^2-5x+6", "x^2-5x+6");
    raw("2sin(x)^2-sin(x)-1=0", "2sin(x)^2-sin(x)-1=0");
    raw("frac(1,x+1)", "1/(x+1)");
    raw("sqrt(x+2)", "sqrt(x+2)");
    raw("2x(x+1)", "2x(x+1)");
    raw("-x^2", "-x^2");
    raw("0.25x", "x/4");
    raw("der(x^2sin(x),x)", "d/dx(x^2sin(x))");
    raw("int(x^2,x,0,1)", "int[0,1](x^2)dx");
    raw("lim(frac(x^2-4,x-2),x,2)", "lim[x->2]((x^2-4)/(x-2))");
    raw("pix", "pix");
    raw("e^(2x)", "e^(2x)");
    sim("2+3", "5");
    sim("3x+2x", "5x");
    sim("x+1-(x+1)", "0");
    sim("x*x^2", "x^3");
    sim("2(x+1)", "2x+2");
    sim("x^2-5x+6+x", "x^2-4x+6");
    sim("sqrt(12)", "2sqrt(3)");
    sim("sqrt(8)/2", "sqrt(2)");
    sim("frac(1,sqrt(3))", "sqrt(3)/3");
    sim("root(3,8)", "2");
    sim("sin(pi/6)", "1/2");
    sim("cos(3pi/4)", "-sqrt(2)/2");
    sim("tan(pi/3)", "sqrt(3)");
    sim("sin(-x)", "-sin(x)");
    sim("asin(1/2)", "pi/6");
    sim("ln(e^(2x))", "2x");
    sim("e^(ln(x))", "x");
    sim("log(1000)", "3");
    sim("logb(2,8)", "3");
    sim("frac(6,4)", "3/2");
    sim("frac(2x,4)", "x/2");
    sim("x^0", "1");
    sim("(2x)^2", "4x^2");
    sim("0.5+0.25", "3/4");
    sim("x e^x", "xe^x");
    sim("2sin(x)^2-sin(x)-1", "2sin(x)^2-sin(x)-1");
    sim("frac(1,x)x", "1");
    solve("2x+3=7", "x=2 | Subtract 3 from both sides; Divide both sides by 2;");
    solve("5x+3=2x+9", "x=2 | Subtract 2x from both sides; Subtract 3 from both sides; Divide both sides by 3;");
    solve("x^2-5x+6=0", "x=2 or x=3 |");
    solve("x^2+x-1=0", "x=");
    solve("x^2+2x+5=0", "none |");
    solve("sqrt(x+2)=x", "x=2 |");
    run("ax+b=c", A_SOLVE, 'x', NULL, NULL, "x=");
    run("ax+b=c", A_SOLVE, 'a', NULL, NULL, "a=");
    solve("x^3-6x^2+11x-6=0", "x=1 or x=2 or x=3 |");
    solve("frac(1,x)+frac(1,x+1)=frac(3,2)", "x=");
    solve("e^(2x)-3e^x+2=0", "x=0 or x=ln(2) |");
    solve("2^x=8", "x=3 |");
    solve("ln(x)=2", "x=e^2 |");
    solve("|x-1|=3", "x=-2 or x=4 |");
    solve("x^2=9", "x=-3 or x=3 |");
    run("2sin(x)^2-sin(x)-1=0", A_SOLVE, 'x', "0", "2pi", "x=pi/2 or x=7pi/6 or x=11pi/6 |");
    run("sin(x)=frac(1,2)", A_SOLVE, 'x', "0", "2pi", "x=pi/6 or x=5pi/6 |");
    solve("x^5+x-3=0", "x=1.13");
    run("der(x^2sin(x),x)", A_CALC, 'x', NULL, NULL, "x^2cos(x)+2xsin(x) | Product rule;");
    run("der(sin(x^2),x)", A_CALC, 'x', NULL, NULL, "2xcos(x^2) |");
    run("der(frac(x,x+1),x)", A_CALC, 'x', NULL, NULL, "1/(x+1)^2 |");
    run("der(e^(3x)ln(x),x)", A_CALC, 'x', NULL, NULL, "");
    run("int(xe^x,x)", A_CALC, 'x', NULL, NULL, "xe^x-e^x+C |");
    run("int(2xcos(x^2),x)", A_CALC, 'x', NULL, NULL, "sin(x^2)+C |");
    run("int(x^2,x,0,1)", A_CALC, 'x', NULL, NULL, "1/3 |");
    run("int(3x^2+2x+1,x)", A_CALC, 'x', NULL, NULL, "x^3+x^2+x+C |");
    run("int(frac(1,x^2-1),x)", A_CALC, 'x', NULL, NULL, "");
    run("int(e^xsin(x),x)", A_CALC, 'x', NULL, NULL, "");
    run("int(ln(x),x)", A_CALC, 'x', NULL, NULL, "xln(x)-x+C |");
    run("lim(frac(x^2-4,x-2),x,2)", A_CALC, 'x', NULL, NULL, "4 |");
    run("lim(frac(3x^2+1,x^2-2),x,inf)", A_CALC, 'x', NULL, NULL, "3 |");
    run("lim(frac(sin(x),x),x,0)", A_CALC, 'x', NULL, NULL, "1 |");
    run("int(e^(x^2),x,0,1)", A_CALC, 'x', NULL, NULL, "1.46265 |");
    run("3x+2x-4+1", A_SIMPLIFY, 'x', NULL, NULL, "5x-3 | Combine like terms;");
    run("(x+1)^2", A_EXPAND, 'x', NULL, NULL, "x^2+2x+1 |");
    run("x^2-9", A_FACTOR, 'x', NULL, NULL, "(x-3)(x+3) | Difference of two squares;");
    run("2x^2+4x", A_FACTOR, 'x', NULL, NULL, "2x(x+2) |");
    run("x^3-6x^2+11x-6", A_FACTOR, 'x', NULL, NULL, "(x-1)(x-2)(x-3) |");
    run("sqrt(2)", A_APPROX, 'x', NULL, NULL, "1.41421 |");
    show("2x+3=7", A_SOLVE, 'x', NULL, NULL);
    show("x^2+x-1=0", A_SOLVE, 'x', NULL, NULL);
    show("sqrt(x+2)=x", A_SOLVE, 'x', NULL, NULL);
    show("2sin(x)^2-sin(x)-1=0", A_SOLVE, 'x', "0", "2pi");
    show("frac(1,x)+frac(1,x+1)=frac(3,2)", A_SOLVE, 'x', NULL, NULL);
    editor();
    parseErr("3+", 2);
    parseErr("(x+1", 4);
    parseErr("x)", 1);

    printf("%d checks, %d failed\n", checks, fails);
    return fails != 0;
}
