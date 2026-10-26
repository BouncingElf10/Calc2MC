#ifndef EDIT_H
#define EDIT_H

#include <stdbool.h>
#include <stdint.h>

/* The template editor. A row is a list of items: an ASCII character, a
 * token (sin(, pi...) or a box (fraction, power, root...) whose slots are
 * rows of their own. Row 0 is the top row. */

enum {   /* tokens, stored as 0x80 + T_* */
    T_SIN, T_COS, T_TAN, T_ASIN, T_ACOS, T_ATAN, T_LN, T_LOG, T_PI, T_INF, T_COUNT
};
#define TOKEN 0x80
#define BOX 0xC0   /* items >= BOX are boxes: BOX + box index */

enum { B_FRAC, B_POW, B_SQRT, B_ROOT, B_ABS, B_LOGB, B_DERIV, B_INT, B_DEFINT, B_LIM, B_RANGE };

#define ED_ROWS 40
#define ED_LEN 40
#define ED_BOXES 24

typedef struct {
    uint8_t n;
    uint8_t it[ED_LEN];
    uint8_t box;     /* the box this row is a slot of, 0xFF for row 0 */
    bool used;
} row_t;

typedef struct {
    uint8_t type, nslots;
    uint8_t slot[4];   /* rows, in left-to-right order */
    uint8_t row;       /* the row the box sits in */
    bool used;
} box_t;

typedef struct {
    row_t rows[ED_ROWS];
    box_t boxes[ED_BOXES];
    uint8_t row, pos;   /* cursor: before item pos of row */
} ed_t;

void ed_clear(ed_t *e);
bool ed_insert(ed_t *e, uint8_t item);
bool ed_text(ed_t *e, const char *s);       /* inserts each character */
bool ed_box(ed_t *e, uint8_t type);         /* cursor goes to the first slot to fill */
void ed_left(ed_t *e);
void ed_right(ed_t *e);
void ed_up(ed_t *e);
void ed_down(ed_t *e);
void ed_del(ed_t *e);
bool ed_empty(const ed_t *e);

/* Linear text for the parser. map[i] holds row << 8 | pos for each output
 * character, so a parse error can put the cursor back where it happened. */
int ed_serialize(const ed_t *e, char *out, uint16_t *map, int size);
void ed_seek(ed_t *e, uint16_t at);

/* the letters used as variables, in order of first use, x first if present */
int ed_vars(const char *text, char *out, int max);

extern const char *const tokenText[T_COUNT];

#endif
