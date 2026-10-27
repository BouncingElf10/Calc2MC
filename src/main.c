#include <graphx.h>
#include <keypadc.h>
#include <string.h>
#include <time.h>
#include "cas.h"
#include "draw.h"
#include "edit.h"

/* ---- keys ---- */

/* an action is a character to type (< 0x80), a token (TOKEN + T_*), a box
 * (A_BOX + B_*) or one of the commands below */
enum {
    A_BOX = 0x100,
    K_NONE = 0x200, K_UP, K_DOWN, K_LEFT, K_RIGHT, K_ENTER, K_QUIT, K_BACK, K_DEL, K_CLEARIN,
    K_MODE, K_MATH, K_2ND, K_ALPHA, K_SQUARE, K_RECIP, K_EPOW, K_TENPOW,
};

typedef struct {
    uint8_t group, mask;
    uint16_t normal, second;
    char alpha;
} keydef_t;

static const keydef_t keys[] = {
    {7, kb_Up, K_UP, K_UP, 0}, {7, kb_Down, K_DOWN, K_DOWN, 0},
    {7, kb_Left, K_LEFT, K_LEFT, 0}, {7, kb_Right, K_RIGHT, K_RIGHT, 0},
    {6, kb_Enter, K_ENTER, K_ENTER, 0}, {6, kb_Clear, K_QUIT, K_QUIT, 0},
    {6, kb_Add, '+', '+', 0}, {6, kb_Sub, '-', '-', 'w'}, {6, kb_Mul, '*', '*', 'r'},
    {6, kb_Div, '/', 'e', 'm'}, {6, kb_Power, A_BOX + B_POW, TOKEN + T_PI, 'h'},
    {1, kb_Yequ, K_BACK, K_BACK, 0}, {1, kb_2nd, K_2ND, K_2ND, 0}, {1, kb_Mode, K_MODE, K_MODE, 0},
    {1, kb_Del, K_DEL, K_CLEARIN, 0},
    {2, kb_Sto, '=', '=', 'x'}, {2, kb_Ln, TOKEN + T_LN, K_EPOW, 's'}, {2, kb_Log, TOKEN + T_LOG, K_TENPOW, 'n'},
    {2, kb_Square, K_SQUARE, A_BOX + B_SQRT, 'i'}, {2, kb_Recip, K_RECIP, K_RECIP, 'd'},
    {2, kb_Math, K_MATH, K_MATH, 'a'}, {2, kb_Alpha, K_ALPHA, K_ALPHA, 0},
    {3, kb_0, '0', '0', 0}, {3, kb_1, '1', '1', 'y'}, {3, kb_4, '4', '4', 't'}, {3, kb_7, '7', '7', 'o'},
    {3, kb_Comma, ',', ',', 'j'}, {3, kb_Sin, TOKEN + T_SIN, TOKEN + T_ASIN, 'e'}, {3, kb_Apps, K_NONE, K_NONE, 'b'},
    {3, kb_GraphVar, 'x', 'x', 0},
    {4, kb_DecPnt, '.', '.', 0}, {4, kb_2, '2', '2', 'z'}, {4, kb_5, '5', '5', 'u'}, {4, kb_8, '8', '8', 'p'},
    {4, kb_LParen, '(', '(', 'k'}, {4, kb_Cos, TOKEN + T_COS, TOKEN + T_ACOS, 'f'}, {4, kb_Prgm, K_NONE, K_NONE, 'c'},
    {5, kb_Chs, '-', '-', 0}, {5, kb_3, '3', '3', 0}, {5, kb_6, '6', '6', 'v'}, {5, kb_9, '9', '9', 'q'},
    {5, kb_RParen, ')', ')', 'l'}, {5, kb_Tan, TOKEN + T_TAN, TOKEN + T_ATAN, 'g'},
};
#define NKEYS (sizeof keys / sizeof keys[0])

static uint8_t shift;   /* 1: 2nd, 2: alpha, 3: alpha lock */
static int held = -1;
static clock_t heldAt, lastRepeat;

static bool repeats(uint16_t a) {
    return a == K_UP || a == K_DOWN || a == K_LEFT || a == K_RIGHT || a == K_DEL;
}

static uint16_t getkey(void) {
    kb_Scan();
    int k = -1;
    for (unsigned i = 0; i < NKEYS; i++)
        if (kb_Data[keys[i].group] & keys[i].mask) {
            k = (int)i;
            break;
        }
    if (k < 0) {
        held = -1;
        return K_NONE;
    }
    const keydef_t *d = &keys[k];
    const clock_t now = clock();
    if (k == held) {
        if (!repeats(d->normal) || now - heldAt < CLOCKS_PER_SEC * 2 / 5 || now - lastRepeat < CLOCKS_PER_SEC / 16)
            return K_NONE;
        lastRepeat = now;
        return d->normal;
    }
    held = k;
    heldAt = lastRepeat = now;
    if (d->normal == K_2ND) {
        shift = shift == 1 ? 0 : 1;
        return K_NONE;
    }
    if (d->normal == K_ALPHA) {
        shift = shift == 1 ? 3 : shift ? 0 : 2;   /* 2nd alpha locks */
        return K_NONE;
    }
    const uint8_t s = shift;
    if (shift < 3) shift = 0;
    if (s >= 2) {
        if (d->group == 3 && d->mask == kb_GraphVar) return A_BOX + B_FRAC;   /* alpha x,t,θ,n */
        if (d->alpha) return (uint16_t)d->alpha;
        return d->normal >= K_NONE ? d->normal : K_NONE;
    }
    return s == 1 ? d->second : d->normal;
}

/* ---- state ---- */

enum { S_EDIT, S_MENU, S_MATH, S_RANGE, S_STEPS };
static uint8_t screen;

static ed_t ed, range;
static char input[200];
static uint16_t map[200];
static const char *msg;   /* an error under the input */

#define POOL 46000
static uint8_t pool[POOL];

typedef struct {
    char label[24];
    uint8_t action;
    char var;
} item_t;
static item_t menu[12];
static uint8_t nmenu, sel;

static uint8_t action;
static char solveVar;
static bool useRange;
static node_t *problem, *lo, *hi;

/* steps screen: the rows shown, top-level steps and open sub-steps */
typedef struct {
    int16_t step;
    uint8_t num;   /* step number, 0 for a sub-step */
} vis_t;
static vis_t vis[MAX_STEPS];
static uint8_t nvis, cur, first;
static bool open[MAX_STEPS];
static int pan;

static const char *const mathItems[] = {
    "n-th root", "Absolute value", "Log with a base", "Derivative d/dx",
    "Integral", "Integral with bounds", "Limit", "Infinity",
    "Combinations nCr", "Permutations nPr", "Factorial !",
};
static const uint16_t mathActs[] = {
    A_BOX + B_ROOT, A_BOX + B_ABS, A_BOX + B_LOGB, A_BOX + B_DERIV,
    A_BOX + B_INT, A_BOX + B_DEFINT, A_BOX + B_LIM, TOKEN + T_INF,
    A_BOX + B_NCR, A_BOX + B_NPR, '!',
};
#define NMATH (sizeof mathItems / sizeof mathItems[0])

/* ---- layout helpers ---- */

#define W GFX_LCD_WIDTH
#define H GFX_LCD_HEIGHT
#define FOOT 14

static dim_t say(const char *s, int x, int y, uint8_t f, uint8_t c) {
    pen(true, f, c);
    return text(s, x, y);
}

static dim_t sizeOf(const char *s, uint8_t f) {
    pen(false, f, C_INK);
    return text(s, 0, 0);
}

static dim_t show(const node_t *e, int x, int y, uint8_t f, uint8_t c) {
    pen(true, f, c);
    return node(e, x, y);
}

static dim_t sizeN(const node_t *e, uint8_t f) {
    pen(false, f, C_INK);
    return node(e, 0, 0);
}

static dim_t capSize(const step_t *s) {
    pen(false, F_SMALL, C_SEC);
    return caption(s->cap, s->arg, 0, 0);
}

static dim_t capShow(const step_t *s, int x, int y) {
    pen(true, F_SMALL, C_SEC);
    return caption(s->cap, s->arg, x, y);
}

static void fill(int x, int y, int w, int h, uint8_t c) {
    gfx_SetColor(c);
    gfx_FillRectangle(x, y, w, h);
}

/* key hints: pairs of key name and what it does */
static void footer(const char *const *hints) {
    fill(0, H - FOOT, W, FOOT, C_PAPER);
    gfx_SetColor(C_RULE);
    gfx_HorizLine(0, H - FOOT, W);
    int x = 6;
    const int y = H - 4;
    for (int i = 0; hints[i]; i += 2) {
        x += say(hints[i], x, y, F_SMALL, C_INK).w + 3;
        x += say(hints[i + 1], x, y, F_SMALL, C_SEC).w + 9;
    }
}

static void strip(const char *title) {
    fill(0, 0, W, 20, C_TEAL);
    say(title, 8, 15, F_BODY, C_PAPER);
    const char *mode = cas_deg ? "DEG" : "RAD";
    int x = W - 8 - sizeOf(mode, F_SMALL).w;
    say(mode, x, 14, F_SMALL, C_PAPER);
    if (shift) {
        const char *s = shift == 1 ? "2nd" : shift == 2 ? "alpha" : "ALPHA";
        x -= sizeOf(s, F_SMALL).w + 10;
        say(s, x, 14, F_SMALL, C_PAPER);
    }
}

#define TAB 18   /* the numbered margin tab; menus mark their row with it too */

#define ROWS 8   /* a longer list scrolls */

/* a list of choices; the chosen one gets the filled margin tab */
static void list(const char *title, int n, const char *(*label)(int)) {
    const int rows = n < ROWS ? n : ROWS, from = sel < rows ? 0 : sel - rows + 1;
    const int rowH = 20, h = 26 + rows * rowH, top = (H - FOOT - h) / 2 + 8;
    fill(8, top, W - 16, h, C_PAPER);
    gfx_SetColor(C_RULE);
    gfx_Rectangle(8, top, W - 16, h);
    say(title, 9 + TAB + 8, top + 16, F_SMALL, C_SEC);
    for (int i = 0; i < rows; i++) {
        const int y = top + 22 + i * rowH;
        if (from + i == sel) {
            fill(9, y, W - 18, rowH, C_PALE);
            fill(9, y, TAB, rowH, C_TEAL);
        }
        say(label(from + i), 9 + TAB + 8, y + 14, F_BODY, C_INK);
    }
    if (from + rows < n) say(G_DOWN, W - 24, top + 16, F_SMALL, C_TEAL);   /* more below */
}

static const char *menuLabel(int i) {
    return menu[i].label;
}

static const char *mathLabel(int i) {
    return mathItems[i];
}

/* ---- the editor ---- */

static int scrollX, ruleY;

static void drawEditor(void) {
    strip("Math");
    const int y = 92;
    const dim_t d = (pen(false, F_BODY, C_INK), edrow(&ed, 0, 0, y, true));
    /* keep the cursor on screen */
    pen(true, F_BODY, C_INK);
    edrow(&ed, 0, 10 - scrollX, y, true);
    if (curX < 10) scrollX -= 10 - curX + 30;
    if (curX > W - 10) scrollX += curX - (W - 10) + 30;
    if (scrollX < 0 || d.w < W - 20) scrollX = 0;
    if (ed_empty(&ed)) say("Type a problem", 14, y, F_BODY, C_SEC);
    gfx_SetColor(C_TEAL);
    gfx_VertLine(curX, curTop, curBot - curTop + 1);
    gfx_SetColor(C_RULE);
    ruleY = y + d.d + 8;
    gfx_HorizLine(8, ruleY, W - 16);
    if (msg) say(msg, 10, ruleY + 14, F_SMALL, C_ERR);
    static const char *const hints[] = {"enter", "go", "math", "templates", "alpha x", "fraction", "clear", "quit", NULL};
    footer(hints);
}

/* ---- running the engine ---- */

static bool hasTrig(const char *s) {
    return strstr(s, "sin(") || strstr(s, "cos(") || strstr(s, "tan(");
}

static void rowText(const ed_t *e, int row, char *out, int size) {
    /* a slot of the interval box, written out on its own */
    ed_t tmp = *e;
    tmp.rows[0] = tmp.rows[row];
    tmp.rows[0].box = 0xFF;
    uint16_t m[64];
    ed_serialize(&tmp, out, m, size);
}

static void buildSteps(void) {
    nvis = 0;
    uint8_t num = 0;
    for (int i = 0; i < nsteps; i++) {
        if (steps[i].parent != -1) continue;
        vis[nvis++] = (vis_t){(int16_t)i, ++num};
        if (open[i])
            for (int j = 0; j < nsteps; j++)
                if (steps[j].parent == i) vis[nvis++] = (vis_t){(int16_t)j, 0};
    }
}

static bool run(void) {
    cas_reset();
    int at;
    problem = parse(input, &at);
    if (!problem) {
        msg = cas_err;
        ed_seek(&ed, at < (int)strlen(input) ? map[at] : (uint16_t)ed.rows[0].n);
        screen = S_EDIT;
        return false;
    }
    lo = hi = NULL;
    if (useRange) {
        char a[64], b[64];
        rowText(&range, range.boxes[0].slot[0], a, sizeof a);
        rowText(&range, range.boxes[0].slot[1], b, sizeof b);
        lo = parse(a, &at);
        hi = lo ? parse(b, &at) : NULL;
        if (!hi) {
            msg = "Check the interval";
            screen = S_RANGE;
            return false;
        }
    }
    if (!cas_run(problem, action, solveVar, lo, hi)) {
        msg = cas_err ? cas_err : "Can't do this";
        screen = S_EDIT;
        return false;
    }
    memset(open, 0, sizeof open);
    buildSteps();
    cur = first = 0;
    pan = 0;
    screen = S_STEPS;
    return true;
}

/* enter in the editor: an error, a menu, or straight to the steps */
static void go(void) {
    msg = NULL;
    if (ed_empty(&ed)) return;
    ed_serialize(&ed, input, map, sizeof input);
    cas_reset();
    int at;
    if (!parse(input, &at)) {
        msg = cas_err;
        ed_seek(&ed, at < (int)strlen(input) ? map[at] : (uint16_t)ed.rows[0].n);
        return;
    }
    useRange = false;
    if (strstr(input, "der(") || strstr(input, "int(") || strstr(input, "lim(")) {
        action = A_CALC;
        solveVar = 'x';
        run();
        return;
    }
    /* counting has one thing to do: work it out */
    if (!strchr(input, '=') && (strstr(input, "ncr(") || strstr(input, "npr(") || strchr(input, '!'))) {
        action = A_SIMPLIFY;
        solveVar = 'x';
        run();
        return;
    }
    nmenu = 0;
    if (strchr(input, '=')) {
        char v[8];
        const int n = ed_vars(input, v, 8);
        for (int i = 0; i < n; i++) {
            item_t *m = &menu[nmenu++];
            strcpy(m->label, "Solve for  ");
            m->label[10] = v[i];
            m->action = A_SOLVE;
            m->var = v[i];
        }
        if (!n) {
            msg = "There is no letter to solve for";
            return;
        }
    } else {
        static const char *const names[] = {"Simplify", "Expand", "Factor", "Approximate"};
        static const uint8_t acts[] = {A_SIMPLIFY, A_EXPAND, A_FACTOR, A_APPROX};
        char v[8];
        const char x = ed_vars(input, v, 8) ? v[0] : 'x';
        for (int i = 0; i < 4; i++) {
            strcpy(menu[nmenu].label, names[i]);
            menu[nmenu].action = acts[i];
            menu[nmenu++].var = x;
        }
    }
    sel = 0;
    screen = S_MENU;
}

/* 0 to a full turn in the current angle unit */
static void fullTurn(void) {
    ed_clear(&range);
    ed_box(&range, B_RANGE);
    ed_text(&range, "0");
    ed_right(&range);
    if (cas_deg) {
        ed_text(&range, "360");
    } else {
        ed_text(&range, "2");
        ed_insert(&range, TOKEN + T_PI);
    }
    range.row = range.boxes[0].slot[0];
    range.pos = 0;
}

static bool isFullTurn(void) {
    char a[16], b[16];
    rowText(&range, range.boxes[0].slot[0], a, sizeof a);
    rowText(&range, range.boxes[0].slot[1], b, sizeof b);
    return !strcmp(a, "0") && !strcmp(b, cas_deg ? "360" : "2pi");
}

static void choose(void) {
    action = menu[sel].action;
    solveVar = menu[sel].var;
    if (action == A_SOLVE && hasTrig(input)) {
        fullTurn();
        rangeVar = solveVar;
        useRange = true;
        msg = NULL;
        screen = S_RANGE;
        return;
    }
    run();
}

/* ---- the steps ---- */

#define AREA (W - TAB - 10)

/* an expression in the space it has: smaller font or split lines when wide */
static int block(const node_t *e, int x, int top, int w, bool draw, uint8_t font, int scroll) {
    dim_t d = sizeN(e, font);
    if (d.w > w && font != F_SMALL) {
        const dim_t s = sizeN(e, F_SMALL);
        if (s.w <= w) font = F_SMALL, d = s;
    }
    if (d.w > w && (e->type == N_EQ || (e->type == N_OR && e->n > 1))) {
        int h = 0;
        for (int i = 0; i < e->n; i++) {
            const node_t *part = e->k[i];
            int at = x;
            if (e->type == N_EQ && i) {
                const dim_t eqd = sizeOf("= ", font);
                const dim_t pd = sizeN(part, font);
                if (draw) say("= ", x, top + h + pd.a, font, C_INK);
                at += eqd.w;
            }
            h += block(part, at, top + h, w - (at - x), draw, font, scroll) + 2;
        }
        return h - 2;
    }
    if (draw) show(e, x - (d.w > w ? scroll : 0), top + d.a, font, C_INK);
    return d.a + d.d;
}

static int entry(int i, int top, bool draw) {
    const step_t *s = &steps[vis[i].step];
    const bool sub = !vis[i].num;
    const int x = TAB + 6 + (sub ? 10 : 0), w = AREA - (sub ? 10 : 0);
    int h = 4;
    const bool hasSubs = !sub && vis[i].step > 0 && steps[vis[i].step - 1].parent == vis[i].step;
    if (s->cap) {
        dim_t c = capSize(s);
        if (draw) {
            const int cx = capShow(s, x, top + h + c.a).w;
            if (hasSubs) say(open[vis[i].step] ? " " G_DOWN : " " G_RIGHT, x + cx, top + h + c.a, F_SMALL, C_TEAL);
        }
        h += c.a + c.d + 2;
    }
    if (s->e) h += block(s->e, x, top + h, w, draw, F_BODY, i == cur ? pan : 0) + 2;
    return h + 3;
}

static int answerTop, viewTop;

static void drawSteps(void) {
    /* problem strip */
    dim_t pd = sizeN(problem, F_BODY);
    uint8_t pf = F_BODY;
    if (pd.w > W - 70) pd = sizeN(problem, F_SMALL), pf = F_SMALL;
    int sh = pd.a + pd.d + 8;
    if (sh < 20) sh = 20;
    if (sh > 60) sh = 60;
    fill(0, 0, W, sh, C_TEAL);
    show(problem, 8, (sh - pd.a - pd.d) / 2 + pd.a, pf, C_PAPER);
    const char *mode = cas_deg ? "DEG" : "RAD";
    int rx = W - 8 - sizeOf(mode, F_SMALL).w;
    say(mode, rx, sh / 2 + 4, F_SMALL, C_PAPER);
    if (lo) {
        /* lo <= x <= hi */
        int iw = sizeN(hi, F_SMALL).w;
        rx -= iw + 10;
        show(hi, rx, sh / 2 + 4, F_SMALL, C_PAPER);
        const char v[8] = {' ', '\x1c', ' ', solveVar, ' ', '\x1c', ' ', 0};
        rx -= sizeOf(v, F_SMALL).w + 2;
        say(v, rx, sh / 2 + 4, F_SMALL, C_PAPER);
        iw = sizeN(lo, F_SMALL).w;
        rx -= iw + 2;
        show(lo, rx, sh / 2 + 4, F_SMALL, C_PAPER);
    }

    /* answer box */
    answerTop = sh + 6;
    const int ax = 14, aw = W - 30;
    int ah = block(cas_answer, ax, 0, aw, false, F_BODY, 0);
    dim_t apx = {0, 0, 0};
    int apxW = 0;
    if (cas_approx) {
        const node_t *one = cas_approx;
        for (int i = 0; i < (cas_approx->type == N_OR ? cas_approx->n : 1); i++) {
            one = cas_approx->type == N_OR ? cas_approx->k[i] : cas_approx;
            apxW += sizeN(one, F_SMALL).w + 8;
        }
        apx = sizeOf(G_APPROX " ", F_SMALL);
        apxW += apx.w;
    }
    const dim_t aline = sizeN(cas_answer, F_BODY);
    const bool below = cas_approx && aline.w + apxW + 10 > aw;
    const int boxH = ah + 10 + (below ? 14 : 0);
    fill(8, answerTop, W - 16, boxH, C_TINT);
    fill(8, answerTop, 3, boxH, C_TEAL);
    block(cas_answer, ax + 4, answerTop + 5, aw - 4, true, F_BODY, 0);
    if (cas_approx) {
        int x = below ? ax + 4 : W - 14 - apxW;
        const int y = below ? answerTop + 5 + ah + 11 : answerTop + 5 + aline.a;
        x += say(G_APPROX " ", x, y, F_SMALL, C_TEAL).w;
        for (int i = 0; i < (cas_approx->type == N_OR ? cas_approx->n : 1); i++) {
            const node_t *one = cas_approx->type == N_OR ? cas_approx->k[i] : cas_approx;
            if (i) x += say(",", x, y, F_SMALL, C_TEAL).w + 4;
            x += show(one, x, y, F_SMALL, C_TEAL).w;
        }
    }
    viewTop = answerTop + boxH + 4;
    const int viewBot = H - FOOT - 1;

    /* keep the chosen step in view */
    if (cur < first) first = cur;
    for (;;) {
        int y = viewTop;
        for (int i = first; i <= cur; i++) y += entry(i, 0, false);
        if (y <= viewBot || first == cur) break;
        first++;
    }
    clip(viewTop, viewBot);
    int y = viewTop;
    int i = first;
    for (; i < nvis; i++) {
        const int h = entry(i, 0, false);
        if (y + h > viewBot && i != first) break;
        if (i == cur) fill(0, y, W, h, C_PALE);
        if (vis[i].num) {
            char n[4];
            n[0] = vis[i].num >= 10 ? (char)('0' + vis[i].num / 10) : (char)('0' + vis[i].num);
            n[1] = vis[i].num >= 10 ? (char)('0' + vis[i].num % 10) : 0;
            n[2] = 0;
            if (i == cur) fill(0, y, TAB, h, C_TEAL);
            const int nw = sizeOf(n, F_SMALL).w;
            say(n, (TAB - nw) / 2, y + 13, F_SMALL, i == cur ? C_PAPER : C_TEAL);
        } else {
            gfx_SetColor(C_SUBRULE);
            gfx_VertLine(TAB + 8, y + 2, h - 4);
        }
        entry(i, y, true);
        y += h;
    }
    clip(0, H);
    /* enter only does something on a step with sub-steps */
    const int16_t s = vis[cur].step;
    const bool subs = vis[cur].num && s > 0 && steps[s - 1].parent == s;
    const char *hints[] = {"enter", open[s] ? "close" : "open", G_UPDOWN, "step", "y=", "back",
                           "mode", cas_deg ? "rad" : "deg", "clear", "quit", NULL};
    footer(subs ? hints : hints + 2);
    if (i < nvis) say(G_DOWN, W - 12, H - 4, F_SMALL, C_TEAL);   /* more below */
}

/* ---- main ---- */

static void render(void) {
    uint8_t *m = cas_mark();   /* drawing makes a few temporary nodes */
    if (setjmp(cas_jb)) {
        cas_release(m);
        return;
    }
    fill(0, 0, W, H, C_PAPER);
    switch (screen) {
        case S_EDIT:
            drawEditor();
            break;
        case S_MENU: {
            drawEditor();
            list(strchr(input, '=') ? "Solve the equation" : "What to do", nmenu, menuLabel);
            static const char *const hints[] = {G_UPDOWN, "choose", "enter", "go", "y=", "back", NULL};
            footer(hints);
            break;
        }
        case S_MATH: {
            drawEditor();
            list("Insert a template", NMATH, mathLabel);
            static const char *const hints[] = {G_UPDOWN, "choose", "enter", "insert", "y=", "back", NULL};
            footer(hints);
            break;
        }
        case S_RANGE: {
            drawEditor();
            const int top = ruleY + 6, h = 70;   /* under the equation it is for */
            fill(8, top, W - 16, h, C_PAPER);
            gfx_SetColor(C_RULE);
            gfx_Rectangle(8, top, W - 16, h);
            say("Find the solutions in", 18, top + 16, F_SMALL, C_SEC);
            pen(true, F_BODY, C_INK);
            edrow(&range, 0, 22, top + 44, true);
            gfx_SetColor(C_TEAL);
            gfx_VertLine(curX, curTop, curBot - curTop + 1);
            if (msg) say(msg, 18, top + h - 6, F_SMALL, C_ERR);
            static const char *const hints[] = {"enter", "solve", G_UPDOWN, "move", "y=", "back", NULL};
            footer(hints);
            break;
        }
        case S_STEPS:
            drawSteps();
            break;
    }
    cas_release(m);
}

static void edit(ed_t *e, uint16_t k) {
    switch (k) {
        case K_LEFT: ed_left(e); break;
        case K_RIGHT: ed_right(e); break;
        case K_UP: ed_up(e); break;
        case K_DOWN: ed_down(e); break;
        case K_DEL: ed_del(e); break;
        case K_SQUARE:
            if (ed_box(e, B_POW)) ed_insert(e, '2'), ed_right(e);
            break;
        case K_RECIP:
            if (ed_box(e, B_POW)) ed_text(e, "-1"), ed_right(e);
            break;
        case K_EPOW:
            if (ed_insert(e, 'e')) ed_box(e, B_POW);
            break;
        case K_TENPOW:
            if (ed_text(e, "10")) ed_box(e, B_POW);
            break;
        default:
            if (k >= A_BOX && k < K_NONE) ed_box(e, (uint8_t)(k - A_BOX));
            else if (k < A_BOX) ed_insert(e, (uint8_t)k);
    }
}

int main(void) {
    gfx_Begin();
    gfx_SetDrawBuffer();
    draw_init();
    cas_init(pool, sizeof pool);
    ed_clear(&ed);
    do kb_Scan(); while (kb_AnyKey());   /* the key that started the program */

    bool dirty = true;
    for (;;) {
        if (dirty) {
            render();
            gfx_SwapDraw();
            dirty = false;
        }
        const uint16_t k = getkey();
        if (k == K_NONE) {
            /* the 2nd / alpha indicator changes without an action */
            static uint8_t lastShift;
            if (shift != lastShift) lastShift = shift, dirty = true;
            continue;
        }
        if (k == K_QUIT) break;
        dirty = true;
        if (k == K_MODE) {
            /* the default interval follows the unit; one typed in stays */
            const bool turn = useRange && isFullTurn();
            cas_deg = !cas_deg;
            if (turn) fullTurn();
            if (screen == S_STEPS) {
                const uint8_t c = cur;
                if (run()) cur = c < nvis ? c : 0;
            }
            continue;
        }
        switch (screen) {
            case S_EDIT:
                if (k == K_ENTER) go();
                else if (k == K_MATH) sel = 0, screen = S_MATH;
                else if (k == K_CLEARIN) ed_clear(&ed), msg = NULL;
                else if (k != K_BACK) edit(&ed, k), msg = NULL;
                break;
            case S_MENU:
            case S_MATH: {
                const uint8_t n = screen == S_MENU ? nmenu : NMATH;
                if (k == K_UP) sel = sel ? sel - 1 : n - 1;
                else if (k == K_DOWN) sel = sel + 1 < n ? sel + 1 : 0;
                else if (k == K_BACK) screen = S_EDIT;
                else if (k == K_ENTER) {
                    if (screen == S_MENU) {
                        choose();
                    } else {
                        screen = S_EDIT;
                        edit(&ed, mathActs[sel]);
                    }
                }
                break;
            }
            case S_RANGE:
                if (k == K_ENTER) run();
                else if (k == K_BACK) screen = S_MENU, msg = NULL;
                else if (k == K_CLEARIN) {
                    /* keep the box, empty the slot */
                    row_t *r = &range.rows[range.row];
                    r->n = 0;
                    range.pos = 0;
                } else if (k != K_DEL || range.pos || range.rows[range.row].n) {
                    edit(&range, k);
                    if (range.row == 0) {   /* stay inside the interval box */
                        const box_t *b = &range.boxes[0];
                        range.row = range.pos ? b->slot[1] : b->slot[0];
                        range.pos = range.pos ? range.rows[b->slot[1]].n : 0;
                    }
                }
                break;
            case S_STEPS:
                if (k == K_UP && cur) cur--, pan = 0;
                else if (k == K_DOWN && cur + 1 < nvis) cur++, pan = 0;
                else if (k == K_RIGHT) pan += 40;
                else if (k == K_LEFT) pan = pan > 40 ? pan - 40 : 0;
                else if (k == K_BACK) screen = S_EDIT;
                else if (k == K_ENTER && vis[cur].num) {
                    const int16_t s = vis[cur].step;
                    open[s] = !open[s];
                    buildSteps();
                }
                break;
        }
    }
    gfx_End();
    return 0;
}
