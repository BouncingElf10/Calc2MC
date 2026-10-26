#ifndef DRAW_H
#define DRAW_H

#include <stdbool.h>
#include <stdint.h>
#include "cas.h"
#include "edit.h"

/* palette */
enum { C_PAPER = 1, C_INK, C_TEAL, C_TINT, C_PALE, C_SEC, C_RULE, C_ERR, C_SUBRULE, C_TRANSP = 255 };

/* fonts */
enum { F_BODY, F_SMALL };

/* glyphs in the DrSans code page */
#define G_PI "\x81"
#define G_LE "\x1c"
#define G_PM "\xb1"
#define G_INF "\x7f"
#define G_TO "\x13"
#define G_DOT "\xb7"
#define G_DIV "\xf7"
#define G_TIMES "\xd7"
#define G_MINUS "\x96"
#define G_APPROX "\x1d"
#define G_DOWN "\x0d"    /* triangles, for open and closed steps */
#define G_RIGHT "\x0f"
#define G_UPDOWN "\x10\x11"   /* the arrow keys in key hints */

typedef struct {
    int16_t w, a, d;   /* width, height above and below the baseline */
} dim_t;

/* The pen: whether layout draws or only measures, the font and the colour.
 * Every layout call puts its baseline at y and uses the pen. */
extern bool penDraw;
extern uint8_t penFont, penColor;
void pen(bool draw, uint8_t font, uint8_t color);

void draw_init(void);
void clip(int top, int bottom);   /* rows drawn, for lines and text */
dim_t text(const char *s, int x, int y);
dim_t node(const node_t *e, int x, int y);
dim_t caption(const char *cap, const node_t *arg, int x, int y);

/* an editor row; the cursor is drawn when show is set, its place kept in cur* */
dim_t edrow(const ed_t *ed, int row, int x, int y, bool show);
extern int curX, curTop, curBot;
extern char rangeVar;   /* the letter shown in an interval box */

#endif
