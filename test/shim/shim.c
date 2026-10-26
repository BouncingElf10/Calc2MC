/* Host stand-ins for graphx, fontlibc and keypadc, so the real main.c can
 * run on the PC. KEYS holds space-separated key names; "shot" saves the
 * screen as SHOTS/shot_N.ppm. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fontlibc.h"
#include "graphx.h"
#include "keypadc.h"
#include "../../src/fonts/fonts.h"

uint16_t gfx_palette[256];
static uint8_t buf[240][320], shown[240][320];
static uint8_t color;
static int cx0, cy0, cx1 = 320, cy1 = 240;

static void px(int x, int y) {
    if (x >= cx0 && x < cx1 && y >= cy0 && y < cy1 && x >= 0 && x < 320 && y >= 0 && y < 240) buf[y][x] = color;
}

/* ---- fonts ---- */

struct fontlib_font {
    int height;
    uint8_t width[256];
    uint16_t rows[256][16];   /* bit 15 is the leftmost pixel */
};

static struct fontlib_font fonts[2];
const fontlib_font_t *font_body = &fonts[0], *font_small = &fonts[1];
static const struct fontlib_font *font;
static int fx, fy;
static uint8_t fg;

static void load(struct fontlib_font *f, const char *path) {
    FILE *in = fopen(path, "r");
    if (!in) {
        fprintf(stderr, "no font %s\n", path);
        exit(1);
    }
    char line[256];
    int cp = -1, row = -1;
    while (fgets(line, sizeof line, in)) {
        line[strcspn(line, "\r\n")] = 0;
        if (!strncmp(line, "Height:", 7)) f->height = atoi(line + 7);
        else if (!strncmp(line, "Code point:", 11)) {
            const char *v = line + 11;
            while (*v == ' ') v++;
            cp = *v == '\'' ? (uint8_t)v[1] : (int)strtol(v, NULL, 0);
            row = -1;
        } else if (!strncmp(line, "Width:", 6) && cp >= 0) f->width[cp] = (uint8_t)atoi(line + 6);
        else if (!strncmp(line, "Data:", 5)) row = 0;
        else if (row >= 0 && row < f->height && cp >= 0) {
            for (int c = 0; c < f->width[cp]; c++)
                if (line[c * 2] == '#') f->rows[cp][row] |= (uint16_t)(0x8000 >> c);
            row++;
        }
    }
    fclose(in);
}

bool fontlib_SetFont(const fontlib_font_t *f, int flags) {
    (void)flags;
    font = f;
    return true;
}

void fontlib_SetLineSpacing(uint8_t a, uint8_t b) { (void)a, (void)b; }
void fontlib_SetForegroundColor(uint8_t c) { fg = c; }
void fontlib_SetWindowFullScreen(void) {}
void fontlib_SetTransparency(bool t) { (void)t; }
void fontlib_SetNewlineOptions(uint8_t o) { (void)o; }
void fontlib_SetFirstPrintableCodePoint(char c) { (void)c; }

uint8_t fontlib_GetGlyphWidth(char c) {
    return font->width[(uint8_t)c];
}

unsigned fontlib_GetStringWidth(const char *s) {
    unsigned w = 0;
    while (*s) w += fontlib_GetGlyphWidth(*s++);
    return w;
}

void fontlib_SetCursorPosition(unsigned x, uint8_t y) {
    fx = (int)x, fy = y;
}

void fontlib_DrawGlyph(char ch) {
    const uint8_t c = (uint8_t)ch, save = color;
    color = fg;
    for (int r = 0; r < font->height; r++)
        for (int k = 0; k < font->width[c]; k++)
            if (font->rows[c][r] & (0x8000 >> k)) px(fx + k, fy + r);
    color = save;
    fx += font->width[c];
}

/* ---- graphics ---- */

void gfx_Begin(void) {
    load(&fonts[0], "src/fonts/body.convfont");
    load(&fonts[1], "src/fonts/small.convfont");
}

void gfx_End(void) {}

static int shots;

static void save(void) {
    const char *dir = getenv("SHOTS");
    char path[256];
    snprintf(path, sizeof path, "%s/shot_%d.ppm", dir ? dir : ".", shots++);
    FILE *o = fopen(path, "wb");
    fprintf(o, "P6 320 240 255\n");
    for (int y = 0; y < 240; y++)
        for (int x = 0; x < 320; x++) {
            const uint16_t c = gfx_palette[shown[y][x]];
            const uint8_t rgb[3] = {(uint8_t)(((c >> 10) & 31) << 3), (uint8_t)(((c >> 5) & 31) << 3), (uint8_t)((c & 31) << 3)};
            fwrite(rgb, 1, 3, o);
        }
    fclose(o);
}

void gfx_SwapDraw(void) {
    memcpy(shown, buf, sizeof buf);
}

uint8_t gfx_SetColor(uint8_t c) {
    const uint8_t old = color;
    color = c;
    return old;
}

void gfx_HorizLine(int x, int y, int w) {
    for (int i = 0; i < w; i++) px(x + i, y);
}

void gfx_VertLine(int x, int y, int h) {
    for (int i = 0; i < h; i++) px(x, y + i);
}

void gfx_Line(int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1, dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1, e = dx + dy;
    for (;;) {
        px(x0, y0);
        if (x0 == x1 && y0 == y1) break;
        const int e2 = 2 * e;
        if (e2 >= dy) e += dy, x0 += sx;
        if (e2 <= dx) e += dx, y0 += sy;
    }
}

void gfx_Rectangle(int x, int y, int w, int h) {
    gfx_HorizLine(x, y, w);
    gfx_HorizLine(x, y + h - 1, w);
    gfx_VertLine(x, y, h);
    gfx_VertLine(x + w - 1, y, h);
}

void gfx_FillRectangle(int x, int y, int w, int h) {
    for (int i = 0; i < h; i++) gfx_HorizLine(x, y + i, w);
}

void gfx_SetClipRegion(int xmin, int ymin, int xmax, int ymax) {
    cx0 = xmin, cy0 = ymin, cx1 = xmax, cy1 = ymax;
}

/* ---- keys ---- */

uint8_t kb_Data[8];
static const char *script;
static bool released = true;

static const struct { const char *name; uint8_t group, mask; } names[] = {
    {"up", 7, kb_Up}, {"down", 7, kb_Down}, {"left", 7, kb_Left}, {"right", 7, kb_Right},
    {"enter", 6, kb_Enter}, {"clear", 6, kb_Clear}, {"+", 6, kb_Add}, {"-", 6, kb_Sub}, {"*", 6, kb_Mul},
    {"/", 6, kb_Div}, {"^", 6, kb_Power}, {"y=", 1, kb_Yequ}, {"2nd", 1, kb_2nd}, {"mode", 1, kb_Mode},
    {"del", 1, kb_Del}, {"sto", 2, kb_Sto}, {"ln", 2, kb_Ln}, {"log", 2, kb_Log}, {"sq", 2, kb_Square},
    {"recip", 2, kb_Recip}, {"math", 2, kb_Math}, {"alpha", 2, kb_Alpha}, {"0", 3, kb_0}, {"1", 3, kb_1},
    {"4", 3, kb_4}, {"7", 3, kb_7}, {",", 3, kb_Comma}, {"sin", 3, kb_Sin}, {"apps", 3, kb_Apps},
    {"x", 3, kb_GraphVar}, {".", 4, kb_DecPnt}, {"2", 4, kb_2}, {"5", 4, kb_5}, {"8", 4, kb_8},
    {"(", 4, kb_LParen}, {"cos", 4, kb_Cos}, {"prgm", 4, kb_Prgm}, {"neg", 5, kb_Chs}, {"3", 5, kb_3},
    {"6", 5, kb_6}, {"9", 5, kb_9}, {")", 5, kb_RParen}, {"tan", 5, kb_Tan},
};

void kb_Scan(void) {
    memset(kb_Data, 0, sizeof kb_Data);
    if (!script) script = getenv("KEYS") ? getenv("KEYS") : "";
    if (!released) {   /* every press is followed by a release */
        released = true;
        return;
    }
    while (*script == ' ') script++;
    char word[16];
    int n = 0;
    while (*script && *script != ' ' && n < 15) word[n++] = *script++;
    word[n] = 0;
    if (!n) strcpy(word, "clear");   /* end of the script: quit */
    if (!strcmp(word, "wait")) return;
    if (!strcmp(word, "shot")) {
        save();
        return;
    }
    for (unsigned i = 0; i < sizeof names / sizeof names[0]; i++)
        if (!strcmp(word, names[i].name)) {
            kb_Data[names[i].group] = names[i].mask;
            released = false;
            return;
        }
    fprintf(stderr, "unknown key %s\n", word);
}

int kb_AnyKey(void) {
    for (int i = 1; i < 8; i++)
        if (kb_Data[i]) return 1;
    return 0;
}
