#ifndef CONTENT_H
#define CONTENT_H

/* Custom glyphs, installed with gfx_SetCharData in main.c */
#define TIMES "\x01"
#define DIV   "\x02"
#define MINUS "\x03"
#define G_UP    "\x04"
#define G_DOWN  "\x05"
#define G_LEFT  "\x06"
#define G_RIGHT "\x07"

/* Line tags: '#' heading, '@' formula name / term, '=' expression,
 * '!' note, anything else is body text. Balance sheet lines are
 * "label\tvalue": '^' group, '>' item, '+' subtotal, '*' total.
 * Comparisons add '%' sub-heading, '~' point on the first side, '-' on the second.
 * Lists end with NULL. */
typedef struct {
    const char *title;
    const char *const *lines;
} section_t;

/* sections[] order: test unit, sketches & maxi problem, IB themes, comparisons */
#define N_FORMULA 6
#define N_LAYOUT 2
#define N_DEF 6
#define N_COMPARE 30
#define N_SECTIONS (N_FORMULA + N_LAYOUT + N_DEF + N_COMPARE)
extern const section_t sections[N_SECTIONS];

/* Compare menu grouping: the topic heading that starts at this comparison,
 * or NULL to carry on under the previous one. */
extern const char *const compareTopic[N_COMPARE];

#endif
