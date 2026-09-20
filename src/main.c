#include <graphx.h>
#include <keypadc.h>
#include <sys/timers.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "content.h"
#include "search.h"

enum { C_BG = 1, C_PANEL, C_LINE, C_TEXT, C_DIM, C_TEAL, C_AMBER, C_VIOLET, C_ROSE, C_TRANSP = 255 };

/* keys; letters come back as their lowercase char */
enum { K_NONE, K_UP, K_DOWN, K_LEFT, K_RIGHT, K_OK, K_BACK, K_DEL };

enum { M_HOME, M_PAGE, M_SEARCH, M_LINK, M_COMPARE };   /* M_LINK: picking a link on a page */

enum { R_HEAD, R_NAME, R_EXPR, R_NOTE, R_TERM, R_TEXT, R_GROUP, R_ITEM, R_SUB, R_TOTAL,
       R_PRO, R_CON };
static const uint8_t rowH[]  = {20, 14, 10, 10, 15, 10, 14, 10, 12, 16, 10, 10};
static const uint8_t rowGap[] = {0,  0,  0,  3,  0,  0,  0,  0,  0,  0,  2,  2};   /* above the first line */

#define L 10          /* left margin of page body */
#define RIGHT 306     /* text never goes past this */
#define BODY_Y 24
#define BODY_H 198
#define FOOT_Y 224

typedef struct {
    const char *s;
    uint8_t len;
    uint8_t kind;
    bool cont;        /* continuation line of a wrapped item */
} row_t;

static row_t rows[300];
static int nRows, maxTop, totalH;
static int hlRow = -1;   /* row a search jumped to */

#define MAX_HITS 40
#define SHOWN 9          /* results on screen */
static hit_t hits[MAX_HITS];
static int nHits;
static char query[21];
static int qlen;

/* compare menu: every comparison, with a topic row before each new topic */
typedef struct { const char *label; int8_t sect; } menu_t;   /* sect < 0: topic */
static menu_t menu[N_COMPARE + 8];
static int nMenu;
#define MENU_SHOWN 11    /* rows on screen */
#define MENU_H 18

/* links: phrases in formulas that name another formula or definition */
typedef struct {
    const char *s;        /* inside a content line, so rows can find it */
    uint8_t len, sect;
    const char *target;   /* the target's '@' line */
} link_t;
static link_t links[96];
static int nLinks, curLink = -1;

typedef struct { const char *line; uint8_t sect; } item_t;
static item_t items[256];   /* every '@' line by first letter, formulas first */
static int nItems, bucketStart[28];

static int bucket(char c) {
    if (c >= 'A' && c <= 'Z') c += 32;
    return c >= 'a' && c <= 'z' ? c - 'a' : 26;
}

/* where clear goes back to */
typedef struct { uint8_t mode, sect; int top; } place_t;
static place_t history[16];
static int nHistory;

/* 8x8 glyphs: times, divide, minus, up, down, left, right, magnifier */
static const uint8_t glyphs[8][8] = {
    {0x00, 0x00, 0xD8, 0x70, 0x70, 0xD8, 0x00, 0x00},
    {0x00, 0x30, 0x00, 0xFC, 0xFC, 0x00, 0x30, 0x00},
    {0x00, 0x00, 0x00, 0xFC, 0xFC, 0x00, 0x00, 0x00},
    {0x00, 0x10, 0x38, 0x7C, 0xFE, 0x00, 0x00, 0x00},
    {0x00, 0xFE, 0x7C, 0x38, 0x10, 0x00, 0x00, 0x00},
    {0x08, 0x18, 0x38, 0x78, 0x38, 0x18, 0x08, 0x00},
    {0x20, 0x30, 0x38, 0x3C, 0x38, 0x30, 0x20, 0x00},
    {0x70, 0x88, 0x88, 0x88, 0x70, 0x0C, 0x06, 0x00},
};
static uint8_t spacing[256];

static int rowHeight(const row_t *r) {
    return rowH[r->kind] + (r->cont ? 0 : rowGap[r->kind]);
}

static void setupGfx(void) {
    static const uint8_t pal[][3] = {
        {0x00, 0x00, 0x00},
        {0xF5, 0xF6, 0xF8},   /* background */
        {0xE3, 0xE7, 0xED},   /* panels */
        {0xC5, 0xCC, 0xD6},   /* lines, key caps */
        {0x1B, 0x22, 0x30},   /* text */
        {0x5B, 0x65, 0x76},   /* secondary text */
        {0x0A, 0x70, 0x67},   /* teal accent */
        {0xA0, 0x52, 0x00},   /* amber accent */
        {0x6A, 0x45, 0xE0},   /* violet accent */
        {0xB0, 0x2E, 0x3E},   /* rose accent */
    };
    for (uint8_t i = 0; i < sizeof pal / 3; i++)
        gfx_palette[i] = gfx_RGBTo1555(pal[i][0], pal[i][1], pal[i][2]);

    for (int i = 0; i < 256; i++) spacing[i] = gfx_GetCharWidth((char)i);
    for (int i = 0; i < 8; i++) {
        gfx_SetCharData(i + 1, glyphs[i]);
        spacing[i + 1] = i < 3 ? 7 : 8;   /* math signs match '+' */
    }
    gfx_SetFontSpacing(spacing);
    gfx_SetTextBGColor(C_TRANSP);
    gfx_SetTextTransparentColor(C_TRANSP);
}

static int textW(const char *s, int len) {
    int w = 0;
    while (len--) w += spacing[(uint8_t)*s++];
    return w;
}

static int text(const char *s, int len, int x, int y, uint8_t color) {
    gfx_SetTextFGColor(color);
    gfx_SetTextXY(x, y);
    while (len--) gfx_PrintChar(*s++);
    return gfx_GetTextX();
}

/* writes n (1..99) into p, returns the end */
static char *putNum(char *p, int n) {
    if (n > 9) *p++ = '0' + n / 10;
    *p++ = '0' + n % 10;
    return p;
}

static int str(const char *s, int x, int y, uint8_t color) {
    return text(s, strlen(s), x, y, color);
}

enum { G_FORMULA, G_LAYOUT, G_DEF, G_COMPARE };
static const char *const groupName[] = {"FORMULAS", "LAYOUTS", "DEFINITIONS", "COMPARE"};
static const uint8_t groupColor[] = {C_TEAL, C_VIOLET, C_AMBER, C_ROSE};
static const int groupStart[] = {0, N_FORMULA, N_FORMULA + N_LAYOUT,
                                 N_FORMULA + N_LAYOUT + N_DEF};
static const int groupSize[] = {N_FORMULA, N_LAYOUT, N_DEF, N_COMPARE};

static int group(int sect) {
    return sect < groupStart[G_LAYOUT] ? G_FORMULA
         : sect < groupStart[G_DEF] ? G_LAYOUT
         : sect < groupStart[G_COMPARE] ? G_DEF : G_COMPARE;
}

static uint8_t accent(int sect) {
    return groupColor[group(sect)];
}

/* Home is formulas | definitions in two columns, then a bottom row of the
 * two layouts and the compare menu. Relies on N_FORMULA == N_DEF. */
#define HOME_ROWS (N_FORMULA + 1)
#define H_COMPARE N_SECTIONS   /* the tile that opens the compare menu */

static int homeCols(int row) {
    return row == N_FORMULA ? N_LAYOUT + 1 : 2;
}

static int cell(int col, int row) {
    if (row != N_FORMULA) return col ? groupStart[G_DEF] + row : row;
    return col == N_LAYOUT ? H_COMPARE : groupStart[G_LAYOUT] + col;
}

/* tile geometry; the bottom row holds three, each just wide enough */
static const uint8_t botX[] = {8, 111, 230}, botW[] = {100, 116, 82};
static void tileBox(int col, int row, int *x, int *w) {
    *x = row == N_FORMULA ? botX[col] : col ? 164 : 8;
    *w = row == N_FORMULA ? botW[col] : 148;
}

/* ---- page layout ---- */

static int kindX(uint8_t kind) {
    switch (kind) {
        case R_EXPR: return L + 20;
        case R_NOTE: return L + 16;
        case R_TERM: case R_TEXT: return L + 10;
        case R_PRO: case R_CON: return L + 18;
        case R_ITEM: case R_SUB: return L + 12;
        default: return L;
    }
}

static void addRows(const char *s, uint8_t kind) {
    const int width = RIGHT - kindX(kind);
    bool cont = false;

    while (*s && nRows < (int)(sizeof rows / sizeof rows[0])) {
        while (*s == ' ') s++;
        int fit = 0, w = 0;
        /* grow word by word while the line still fits */
        while (s[fit]) {
            int end = fit;
            while (s[end] == ' ') end++;
            while (s[end] && s[end] != ' ') end++;
            const int ww = textW(s + fit, end - fit);
            if (w + ww > width) break;
            w += ww;
            fit = end;
        }
        if (!fit) {   /* single word wider than the line: hard break */
            while (s[fit] && textW(s, fit + 1) <= width) fit++;
            if (!fit) fit = 1;
        }
        rows[nRows++] = (row_t){s, (uint8_t)fit, kind, cont};
        s += fit;
        cont = true;
    }
}

static void findLinks(const char *text, const char *self) {
    for (const char *p = text; *p; p++) {
        if (!isWordChar(*p) || (p > text && isWordChar(p[-1]))) continue;
        int best = 0, bi = 0;
        const int b = bucket(*p);   /* only names starting with this letter */
        for (int i = bucketStart[b]; i < bucketStart[b + 1]; i++) {   /* longest wins, formulas first */
            if (items[i].line == self) continue;
            const int n = phraseAt(p, items[i].line + 1);
            if (n > best) { best = n; bi = i; }
        }
        if (best && nLinks < (int)(sizeof links / sizeof links[0])) {
            links[nLinks++] = (link_t){p, (uint8_t)best, items[bi].sect, items[bi].line};
            p += best - 1;
        }
    }
}

static void layout(int sect) {
    const bool formulas = group(sect) != G_DEF;
    const char *item = NULL;
    nRows = nLinks = 0;
    hlRow = curLink = -1;
    for (const char *const *p = sections[sect].lines; *p; p++) {
        const char *s = *p;
        if (*s == '@') item = s;
        if (formulas && (*s == '=' || *s == '>' || *s == '+' || *s == '*'))
            findLinks(s + 1, item);
        switch (*s) {
            case '#': addRows(s + 1, R_HEAD); break;
            case '@': addRows(s + 1, formulas ? R_NAME : R_TERM); break;
            case '=': addRows(s + 1, R_EXPR); break;
            case '!': addRows(s + 1, R_NOTE); break;
            case '%': addRows(s + 1, R_HEAD); break;   /* search ignores it */
            case '~': addRows(s + 1, R_PRO); break;
            case '-': addRows(s + 1, R_CON); break;
            case '^': case '>': case '+': case '*':   /* balance sheet lines never wrap */
                if (nRows < (int)(sizeof rows / sizeof rows[0]))
                    rows[nRows++] = (row_t){s + 1, (uint8_t)strlen(s + 1),
                        *s == '^' ? R_GROUP : *s == '>' ? R_ITEM : *s == '+' ? R_SUB : R_TOTAL, false};
                break;
            default: addRows(s, R_TEXT); break;
        }
    }
    /* the last scroll position shows the final row at the bottom */
    int h = 0;
    maxTop = nRows;
    while (maxTop > 0 && h + rowHeight(&rows[maxTop - 1]) <= BODY_H)
        h += rowHeight(&rows[--maxTop]);
    totalH = h;
    for (int i = 0; i < maxTop; i++) totalH += rowHeight(&rows[i]);
}

/* ---- drawing ---- */

static void roundRect(int x, int y, int w, int h, uint8_t color, uint8_t corner) {
    gfx_SetColor(color);
    gfx_FillRectangle_NoClip(x, y, w, h);
    gfx_SetColor(corner);
    gfx_SetPixel(x, y);
    gfx_SetPixel(x + w - 1, y);
    gfx_SetPixel(x, y + h - 1);
    gfx_SetPixel(x + w - 1, y + h - 1);
}

static void keycap(int *x, const char *key, const char *label) {
    int w = gfx_GetStringWidth(key) + 8;
    if (w < 22) w = 22;
    roundRect(*x, FOOT_Y + 3, w, 11, C_LINE, C_PANEL);
    str(key, *x + (w - gfx_GetStringWidth(key)) / 2, FOOT_Y + 5, C_TEXT);
    if (!*label) { *x += w + 8; return; }
    *x = str(label, *x + w + 4, FOOT_Y + 5, C_DIM) + 14;
}

static void footer(int mode) {
    int x = 6;
    gfx_SetColor(C_PANEL);
    gfx_FillRectangle_NoClip(0, FOOT_Y, 320, 240 - FOOT_Y);
    if (mode == M_HOME) {
        keycap(&x, G_UP G_DOWN G_LEFT G_RIGHT, "choose");
        keycap(&x, "enter", "open");
        keycap(&x, "clear", "quit");
    } else if (mode == M_LINK) {
        keycap(&x, G_UP G_DOWN G_LEFT G_RIGHT, "pick");
        keycap(&x, "enter", "go");
        keycap(&x, "clear", "cancel");
    } else if (mode == M_PAGE && nLinks) {
        keycap(&x, G_UP G_DOWN, "");
        keycap(&x, G_LEFT G_RIGHT, "topic");
        keycap(&x, "enter", "links");
        keycap(&x, "clear", "back");
    } else if (mode == M_COMPARE) {
        keycap(&x, G_UP G_DOWN, "choose");
        keycap(&x, "enter", "open");
        keycap(&x, "clear", "back");
    } else if (mode == M_SEARCH) {
        keycap(&x, G_UP G_DOWN, "");
        keycap(&x, "enter", "open");
        keycap(&x, "del", "erase");
        keycap(&x, "clear", "exit");
    } else {
        keycap(&x, G_UP G_DOWN, "scroll");
        keycap(&x, G_LEFT G_RIGHT, "prev/next");
        keycap(&x, "clear", "back");
    }
}

/* "label\tvalue" with the value right-aligned; returns the value column's left edge */
static int sheetLine(const row_t *r, int x, int y, uint8_t color) {
    const char *tab = memchr(r->s, '\t', r->len);
    const int n = tab ? tab - r->s : r->len;
    text(r->s, n, x, y, color);
    if (tab) {
        const int vl = r->len - n - 1;
        text(tab + 1, vl, RIGHT - 4 - textW(tab + 1, vl), y, color);
    }
    return RIGHT - 44;
}

static int rowOf(const char *s) {
    for (int i = 0; i < nRows; i++)
        if (s >= rows[i].s && s < rows[i].s + rows[i].len) return i;
    return 0;
}

/* scroll so row i is fully on screen */
static int reveal(int i, int top) {
    if (i < top) return i;
    for (;;) {
        int h = 0;
        for (int j = top; j <= i; j++) h += rowHeight(&rows[j]);
        if (h <= BODY_H || top >= maxTop) return top;
        top++;
    }
}

static int pill(int x, int y, const char *s, uint8_t bg) {
    const int w = gfx_GetStringWidth(s) + 8;
    roundRect(x, y, w, 12, bg, C_PANEL);
    str(s, x + 4, y + 2, C_BG);
    return x + w;
}

static void groupHeader(int g, int x, int y, int w) {
    str(groupName[g], x + 2, y, groupColor[g]);
    gfx_SetColor(C_LINE);
    gfx_HorizLine_NoClip(x, y + 11, w);
}

/* draws s cut to fit in w pixels */
static void fitText(const char *s, int x, int y, int w, uint8_t color) {
    int n = 0, used = 0;
    while (s[n] && used + spacing[(uint8_t)s[n]] <= w) used += spacing[(uint8_t)s[n++]];
    text(s, n, x, y, color);
}

static void drawHome(int sel) {
    gfx_FillScreen(C_BG);
    gfx_SetColor(C_PANEL);
    gfx_FillRectangle_NoClip(0, 0, 320, 26);
    str("IB Business Management", 8, 9, C_TEXT);
    pill(292, 7, "SL", C_AMBER);
    roundRect(188, 6, 98, 14, C_LINE, C_PANEL);   /* looks like a search box */
    str("\x08 type to find", 193, 9, C_DIM);

    groupHeader(G_FORMULA, 8, 32, 148);
    groupHeader(G_DEF, 164, 32, 148);
    groupHeader(G_LAYOUT, 8, 183, 219);
    groupHeader(G_COMPARE, 230, 183, 82);

    for (int row = 0; row < HOME_ROWS; row++) {
        for (int col = 0; col < homeCols(row); col++) {
            const int i = cell(col, row);
            const int y = row < N_FORMULA ? 48 + row * 22 : 198;
            const uint8_t acc = accent(i);
            const bool on = i == sel;
            int x, w;
            tileBox(col, row, &x, &w);

            roundRect(x, y, w, 19, on ? acc : C_PANEL, C_BG);
            if (!on) {
                gfx_SetColor(acc);
                gfx_FillRectangle_NoClip(x, y + 1, 3, 17);
            }
            int tx = x + (row == N_FORMULA ? 8 : 10);
            if (group(i) == G_DEF && i < groupStart[G_COMPARE] - 1) {   /* unit number */
                const char num[2] = {'1' + i - groupStart[G_DEF], 0};
                tx = str(num, tx, y + 6, on ? C_BG : acc) + 6;
            }
            str(i == H_COMPARE ? "Decisions" : sections[i].title,
                tx, y + 6, on ? C_BG : C_TEXT);
        }
    }
    footer(M_HOME);
}

static void drawSearch(int sel, int top) {
    gfx_FillScreen(C_BG);
    gfx_SetColor(C_PANEL);
    gfx_FillRectangle_NoClip(0, 0, 320, 22);
    const int qx = str(query, pill(6, 5, "\x08 SEARCH", C_TEXT) + 8, 7, C_TEXT);
    gfx_SetColor(C_TEAL);
    gfx_FillRectangle_NoClip(qx + 1, 5, 2, 12);   /* cursor */

    if (!qlen || !nHits) {
        static const char *const help[] = {
            "Type with the letter keys (the green letters); no need to press alpha. 0 is a space.",
            "!Typos and half words are fine: \"brek evn\" finds break-even, \"wrk cap\" finds working capital.",
            NULL
        };
        static const char *const none[] = {"No matches. Try fewer or different letters.", NULL};
        int y = BODY_Y + 6;
        nRows = 0;
        for (const char *const *p = qlen ? none : help; *p; p++)
            addRows(**p == '!' ? *p + 1 : *p, **p == '!' ? R_NOTE : R_TEXT);
        for (int i = 0; i < nRows; i++) {
            const row_t *r = &rows[i];
            if (r->kind == R_NOTE) {
                if (!r->cont) y += 4;
                gfx_SetColor(C_AMBER);
                gfx_FillRectangle_NoClip(L + 8, y - 1, 2, 10);
            }
            text(r->s, r->len, kindX(r->kind), y, r->kind == R_NOTE ? C_DIM : C_TEXT);
            y += 10;
        }
        nRows = 0;   /* the page rebuilds its rows when opened */
    }

    for (int i = top; i < nHits && i < top + SHOWN; i++) {
        const int y = BODY_Y + 2 + (i - top) * 22;
        const int s = hits[i].sect;
        if (i == sel) {
            roundRect(6, y, 300, 21, C_PANEL, C_BG);
            gfx_SetColor(accent(s));
            gfx_FillRectangle_NoClip(6, y + 1, 3, 19);
        }
        fitText(hits[i].line + 1, 16, y + 2, 286, hits[i].line[0] == '#' ? C_AMBER : accent(s));
        str(sections[s].title, 16, y + 12, C_DIM);
    }
    if (nHits > SHOWN) {   /* scrollbar */
        const int track = SHOWN * 22 - 2, th = track * SHOWN / nHits;
        gfx_SetColor(C_PANEL);
        gfx_FillRectangle_NoClip(313, BODY_Y + 2, 3, track);
        gfx_SetColor(C_TEXT);
        gfx_FillRectangle_NoClip(313, BODY_Y + 2 + (track - th) * top / (nHits - SHOWN), 3, th);
    }
    footer(M_SEARCH);
}

static void drawCompare(int sel, int top) {
    gfx_FillScreen(C_BG);
    gfx_SetColor(C_PANEL);
    gfx_FillRectangle_NoClip(0, 0, 320, 22);
    str("What changes if you switch?", pill(6, 5, "COMPARE", C_ROSE) + 8, 7, C_TEXT);

    for (int i = top; i < nMenu && i < top + MENU_SHOWN; i++) {
        const int y = BODY_Y + (i - top) * MENU_H;
        if (menu[i].sect < 0) {   /* topic */
            const int end = str(menu[i].label, L, y + 7, C_DIM) + 6;
            gfx_SetColor(C_LINE);
            gfx_HorizLine_NoClip(end, y + 11, RIGHT - end);
            continue;
        }
        if (i == sel) {
            roundRect(6, y, 300, 17, C_PANEL, C_BG);
            gfx_SetColor(C_ROSE);
            gfx_FillRectangle_NoClip(6, y + 1, 3, 15);
        }
        fitText(sections[menu[i].sect].title, 16, y + 4, 286, C_TEXT);
    }

    if (nMenu > MENU_SHOWN) {   /* scrollbar */
        const int track = MENU_SHOWN * MENU_H - 2, th = track * MENU_SHOWN / nMenu;
        gfx_SetColor(C_PANEL);
        gfx_FillRectangle_NoClip(313, BODY_Y, 3, track);
        gfx_SetColor(C_ROSE);
        gfx_FillRectangle_NoClip(313, BODY_Y + (track - th) * top / (nMenu - MENU_SHOWN), 3, th);
    }
    footer(M_COMPARE);
}

static void drawPage(int sect, int top) {
    const int g = group(sect);
    const uint8_t acc = groupColor[g];

    gfx_FillScreen(C_BG);
    gfx_SetColor(C_PANEL);
    gfx_FillRectangle_NoClip(0, 0, 320, 22);
    const int tx = pill(6, 5, curLink < 0 ? groupName[g] : "GO TO", acc);
    if (curLink < 0) fitText(sections[sect].title, tx + 8, 7, 260 - tx, C_TEXT);   /* stops before n/m */
    else {
        const link_t *l = &links[curLink];
        const int ax = str(G_RIGHT, tx + 6, 7, C_DIM) + 4;
        fitText(l->target + 1, ax, 7, 290 - ax, accent(l->sect));
    }
    char pos[6], *q = pos;   /* "3/25" */
    q = putNum(q, sect - groupStart[g] + 1);
    *q++ = '/';
    *putNum(q, groupSize[g]) = 0;
    str(pos, 314 - gfx_GetStringWidth(pos), 7, C_DIM);

    int y = BODY_Y;
    for (int i = top; i < nRows && y + rowHeight(&rows[i]) <= BODY_Y + BODY_H; i++) {
        const row_t *r = &rows[i];
        const int h = rowHeight(r);
        const int ty = y + h - 9;
        const int x = kindX(r->kind);

        if (i == hlRow) {
            gfx_SetColor(C_PANEL);
            gfx_FillRectangle_NoClip(L - 6, y + h - 12, RIGHT - L + 10, 13);
            gfx_SetColor(acc);
            gfx_FillRectangle_NoClip(L - 6, y + h - 12, 2, 13);
        }

        /* links on this row: dotted underline, or a box when picked */
        const int lty = r->kind == R_TOTAL ? y + 3 : ty;
        for (int j = 0; j < nLinks; j++) {
            const char *a = links[j].s, *b = a + links[j].len;
            if (a >= r->s + r->len || b <= r->s) continue;
            if (a < r->s) a = r->s;
            if (b > r->s + r->len) b = r->s + r->len;
            const int lx = x + textW(r->s, a - r->s), lw = textW(a, b - a);
            if (j == curLink) {
                roundRect(lx - 2, lty - 2, lw + 3, 12, C_LINE, C_LINE);
            } else {
                gfx_SetColor(C_DIM);
                for (int d = 0; d < lw - 1; d += 2) gfx_SetPixel(lx + d, lty + 8);
            }
        }

        switch (r->kind) {
            case R_HEAD: {
                const int end = text(r->s, r->len, x, ty, C_AMBER) + 6;
                gfx_SetColor(C_LINE);
                gfx_HorizLine_NoClip(end, ty + 3, RIGHT - end);
                break;
            }
            case R_NAME:
                text(r->s, r->len, x, ty, acc);
                break;
            case R_EXPR:
                if (!r->cont) str("=", L + 8, ty, C_DIM);
                text(r->s, r->len, x, ty, C_TEXT);
                break;
            case R_NOTE:
                gfx_SetColor(C_AMBER);
                gfx_FillRectangle_NoClip(L + 8, ty - 1, 2, 10);
                text(r->s, r->len, x, ty, C_DIM);
                break;
            case R_PRO:
            case R_CON:
                if (!r->cont)
                    str(r->kind == R_PRO ? "+" : MINUS, L + 8, ty,
                        r->kind == R_PRO ? C_TEAL : C_ROSE);
                text(r->s, r->len, x, ty, C_TEXT);
                break;
            case R_TERM:
                if (!r->cont) {
                    gfx_SetColor(acc);
                    gfx_FillRectangle_NoClip(L, ty + 2, 4, 4);
                }
                text(r->s, r->len, x, ty, C_TEAL);
                break;
            case R_GROUP:
                sheetLine(r, x, ty, acc);
                break;
            case R_ITEM:
                sheetLine(r, x, ty, C_TEXT);
                break;
            case R_SUB:   /* rule above the value */
                gfx_SetColor(C_DIM);
                gfx_HorizLine_NoClip(sheetLine(r, x, ty, C_TEXT), y + 1, 40);
                break;
            case R_TOTAL: {   /* accountant's double underline */
                const int vx = sheetLine(r, x, y + 3, acc);
                gfx_SetColor(acc);
                gfx_HorizLine_NoClip(vx, y + 12, 40);
                gfx_HorizLine_NoClip(vx, y + 14, 40);
                break;
            }
            default:
                text(r->s, r->len, x, ty, C_TEXT);
                break;
        }
        y += h;
    }

    if (maxTop > 0) {   /* scrollbar */
        const int track = BODY_H - 4;
        int th = track * BODY_H / totalH;
        if (th < 10) th = 10;
        gfx_SetColor(C_PANEL);
        gfx_FillRectangle_NoClip(313, BODY_Y + 2, 3, track);
        gfx_SetColor(acc);
        gfx_FillRectangle_NoClip(313, BODY_Y + 2 + (track - th) * top / maxTop, 3, th);
    }
    footer(curLink < 0 ? M_PAGE : M_LINK);
}

/* ---- input ---- */

/* the green alpha letter on each key, without needing alpha; 0 is space */
static const struct { uint8_t group, mask; char c; } letters[] = {
    {2, kb_Math, 'a'}, {3, kb_Apps, 'b'}, {4, kb_Prgm, 'c'}, {2, kb_Recip, 'd'},
    {3, kb_Sin, 'e'}, {4, kb_Cos, 'f'}, {5, kb_Tan, 'g'}, {6, kb_Power, 'h'},
    {2, kb_Square, 'i'}, {3, kb_Comma, 'j'}, {4, kb_LParen, 'k'}, {5, kb_RParen, 'l'},
    {6, kb_Div, 'm'}, {2, kb_Log, 'n'}, {3, kb_7, 'o'}, {4, kb_8, 'p'},
    {5, kb_9, 'q'}, {6, kb_Mul, 'r'}, {2, kb_Ln, 's'}, {3, kb_4, 't'},
    {4, kb_5, 'u'}, {5, kb_6, 'v'}, {6, kb_Sub, 'w'}, {2, kb_Sto, 'x'},
    {3, kb_1, 'y'}, {4, kb_2, 'z'}, {3, kb_0, ' '},
};

static int readKey(void) {
    static int last = K_NONE, held;
    int k = K_NONE;

    kb_Scan();
    if (kb_Data[7] & kb_Up) k = K_UP;
    else if (kb_Data[7] & kb_Down) k = K_DOWN;
    else if (kb_Data[7] & kb_Left) k = K_LEFT;
    else if (kb_Data[7] & kb_Right) k = K_RIGHT;
    else if ((kb_Data[6] & kb_Enter) || (kb_Data[1] & kb_2nd)) k = K_OK;
    else if (kb_Data[6] & kb_Clear) k = K_BACK;
    else if (kb_Data[1] & kb_Del) k = K_DEL;
    else
        for (unsigned i = 0; i < sizeof letters / sizeof letters[0]; i++)
            if (kb_Data[letters[i].group] & letters[i].mask) k = letters[i].c;

    if (k != last) {
        last = k;
        held = 0;
        return k;
    }
    /* only scrolling and erasing repeat while held */
    if (k != K_UP && k != K_DOWN && k != K_DEL) return K_NONE;
    if (held < 10) {
        held++;
        return K_NONE;
    }
    return k;
}

static int waitKey(void) {
    int k;
    while (!(k = readKey())) delay(20);
    return k;
}

static void runSearch(int *hSel, int *hTop) {
    nHits = search(query, hits, MAX_HITS);
    *hSel = *hTop = 0;
}

static void remember(int mode, int sect, int top) {
    if (nHistory == (int)(sizeof history / sizeof history[0])) {   /* drop the oldest */
        memmove(history, history + 1, sizeof history - sizeof history[0]);
        nHistory--;
    }
    history[nHistory++] = (place_t){(uint8_t)mode, (uint8_t)sect, top};
}

/* opens sect scrolled to the '@'/'#' line, highlighted; returns top */
static int openAt(int sect, const char *line) {
    layout(sect);
    for (int i = 0; i < nRows; i++)
        if (rows[i].s == line + 1) { hlRow = i; break; }
    return hlRow < 0 ? 0 : hlRow < maxTop ? hlRow : maxTop;
}

int main(void) {
    int mode = M_HOME, sel = 0, sect = 0, top = 0, hSel = 0, hTop = 0, mSel = 0, mTop = 0;

    for (int i = 0; i < N_COMPARE; i++) {
        if (compareTopic[i]) menu[nMenu++] = (menu_t){compareTopic[i], -1};
        menu[nMenu++] = (menu_t){NULL, (int8_t)(groupStart[G_COMPARE] + i)};
    }
    while (menu[mSel].sect < 0) mSel++;

    for (int b = 0; b < 27; b++) {   /* by first letter; formulas, layouts, then definitions */
        bucketStart[b] = nItems;
        for (int s = 0; s < N_SECTIONS; s++)
            for (const char *const *p = sections[s].lines; *p; p++)
                if (**p == '@' && bucket((*p)[1]) == b && nItems < (int)(sizeof items / sizeof items[0]))
                    items[nItems++] = (item_t){*p, (uint8_t)s};
    }
    bucketStart[27] = nItems;

    gfx_Begin();
    gfx_SetDrawBuffer();
    setupGfx();
    do kb_Scan(); while (kb_AnyKey());   /* ignore the key that launched us */

    for (;;) {
        if (mode == M_HOME) drawHome(sel);
        else if (mode == M_SEARCH) drawSearch(hSel, hTop);
        else if (mode == M_COMPARE) drawCompare(mSel, mTop);
        else drawPage(sect, top);
        gfx_SwapDraw();

        const int k = waitKey();
        if (k >= ' ') {   /* typing anywhere searches */
            if (mode != M_SEARCH) qlen = 0;
            mode = M_SEARCH;
            if (qlen < (int)sizeof query - 1 && (qlen || k != ' ')) {
                query[qlen++] = k;
                query[qlen] = 0;
                runSearch(&hSel, &hTop);
            }
        } else if (mode == M_HOME) {
            int col = 0, row = 0;
            for (int r = 0; r < HOME_ROWS; r++)
                for (int c = 0; c < homeCols(r); c++)
                    if (cell(c, r) == sel) { col = c; row = r; }
            const int dr = k == K_UP ? -1 : k == K_DOWN ? 1 : 0;
            if (dr && row + dr >= 0 && row + dr < HOME_ROWS) {
                row += dr;
                if (col >= homeCols(row)) col = homeCols(row) - 1;
                sel = cell(col, row);
            } else if (k == K_LEFT && col > 0) sel = cell(col - 1, row);
            else if (k == K_RIGHT && col < homeCols(row) - 1) sel = cell(col + 1, row);
            else if (k == K_OK) {
                nHistory = 0;
                if (sel == H_COMPARE) { mode = M_COMPARE; continue; }
                remember(M_HOME, 0, 0);
                sect = sel;
                top = 0;
                layout(sect);
                mode = M_PAGE;
            } else if (k == K_BACK) break;
        } else if (mode == M_COMPARE) {
            const int d = k == K_UP || k == K_LEFT ? -1 : 1;
            if (k == K_UP || k == K_DOWN || k == K_LEFT || k == K_RIGHT) {
                int i = mSel + d;
                while (i > 0 && i < nMenu && menu[i].sect < 0) i += d;   /* skip topics */
                if (i >= 0 && i < nMenu && menu[i].sect >= 0) mSel = i;
                /* keep the topic above the first item of a block in view */
                if (mSel <= mTop) mTop = mSel && menu[mSel - 1].sect < 0 ? mSel - 1 : mSel;
                else if (mSel >= mTop + MENU_SHOWN) mTop = mSel - MENU_SHOWN + 1;
            } else if (k == K_BACK) mode = M_HOME;
            else if (k == K_OK) {
                nHistory = 0;
                remember(M_COMPARE, 0, 0);
                sect = menu[mSel].sect;
                top = 0;
                layout(sect);
                mode = M_PAGE;
            }
        } else if (mode == M_SEARCH) {
            if (k == K_UP && hSel > 0) { if (--hSel < hTop) hTop = hSel; }
            else if (k == K_DOWN && hSel < nHits - 1) { if (++hSel >= hTop + SHOWN) hTop++; }
            else if (k == K_DEL && qlen) { query[--qlen] = 0; runSearch(&hSel, &hTop); }
            else if (k == K_BACK) mode = M_HOME;
            else if (k == K_OK && nHits) {
                nHistory = 0;
                remember(M_SEARCH, 0, 0);
                sect = hits[hSel].sect;
                top = openAt(sect, hits[hSel].line);
                mode = M_PAGE;
            }
        } else if (curLink >= 0) {   /* picking a link */
            if ((k == K_UP || k == K_LEFT) && curLink > 0) curLink--;
            else if ((k == K_DOWN || k == K_RIGHT) && curLink < nLinks - 1) curLink++;
            else if (k == K_BACK || k == K_DEL) curLink = -1;
            else if (k == K_OK) {
                const link_t l = links[curLink];
                remember(M_PAGE, sect, top);
                sect = l.sect;
                top = openAt(sect, l.target);
            }
            if (curLink >= 0) top = reveal(rowOf(links[curLink].s), top);
        } else {
            if (k == K_UP && top > 0) top--;
            else if (k == K_DOWN && top < maxTop) top++;
            else if (k == K_LEFT || k == K_RIGHT) {
                sect = (sect + (k == K_LEFT ? N_SECTIONS - 1 : 1)) % N_SECTIONS;
                top = 0;
                layout(sect);
            } else if (k == K_OK && nLinks) {   /* start at the first link on screen */
                curLink = 0;
                while (curLink < nLinks - 1 && rowOf(links[curLink].s) < top) curLink++;
                top = reveal(rowOf(links[curLink].s), top);
            } else if (k == K_BACK || k == K_DEL) {
                const place_t p = nHistory ? history[--nHistory] : (place_t){M_HOME, 0, 0};
                sel = group(sect) == G_COMPARE ? H_COMPARE : sect;
                mode = p.mode;
                if (mode == M_PAGE) {
                    sect = p.sect;
                    layout(sect);
                    top = p.top;
                }
            }
        }
    }

    gfx_End();
    return 0;
}
