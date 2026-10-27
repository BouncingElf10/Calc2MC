#include <string.h>
#include "edit.h"

const char *const tokenText[T_COUNT] = {
    "sin(", "cos(", "tan(", "asin(", "acos(", "atan(", "ln(", "log(", "pi", "inf",
};

/* slots per box type, the slot the cursor starts in, and which slots get an x */
static const uint8_t nslots[] = {2, 1, 1, 2, 1, 2, 2, 2, 4, 3, 2, 2, 2};
static const uint8_t first[] = {0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0};
static const uint8_t xslot[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 1, 3, 0, 0xFF, 0xFF, 0xFF};

void ed_clear(ed_t *e) {
    memset(e, 0, sizeof *e);
    e->rows[0].used = true;
    e->rows[0].box = 0xFF;
}

bool ed_empty(const ed_t *e) {
    return !e->rows[0].n;
}

static int newrow(ed_t *e, uint8_t box) {
    for (int i = 1; i < ED_ROWS; i++)
        if (!e->rows[i].used) {
            e->rows[i] = (row_t){.used = true, .box = box};
            return i;
        }
    return -1;
}

static void freebox(ed_t *e, int b);

static void freerow(ed_t *e, int r) {
    for (int i = 0; i < e->rows[r].n; i++)
        if (e->rows[r].it[i] >= BOX) freebox(e, e->rows[r].it[i] - BOX);
    e->rows[r].used = false;
}

static void freebox(ed_t *e, int b) {
    for (int i = 0; i < e->boxes[b].nslots; i++) freerow(e, e->boxes[b].slot[i]);
    e->boxes[b].used = false;
}

static bool put(ed_t *e, int r, int pos, uint8_t item) {
    row_t *row = &e->rows[r];
    if (row->n == ED_LEN) return false;
    memmove(row->it + pos + 1, row->it + pos, row->n - pos);
    row->it[pos] = item;
    row->n++;
    if (item >= BOX) e->boxes[item - BOX].row = r;
    return true;
}

bool ed_insert(ed_t *e, uint8_t item) {
    if (!put(e, e->row, e->pos, item)) return false;
    e->pos++;
    return true;
}

bool ed_text(ed_t *e, const char *s) {
    while (*s)
        if (!ed_insert(e, (uint8_t)*s++)) return false;
    return true;
}

/* how many boxes the row sits inside */
static int depth(const ed_t *e, int row) {
    int d = 0;
    for (uint8_t b; (b = e->rows[row].box) != 0xFF; row = e->boxes[b].row) d++;
    return d;
}

bool ed_box(ed_t *e, uint8_t type) {
    int b = 0;
    while (b < ED_BOXES && e->boxes[b].used) b++;
    /* deeper nesting would not fit the 4 KB stack when drawn */
    if (b == ED_BOXES || e->rows[e->row].n == ED_LEN || depth(e, e->row) == 6) return false;
    box_t *box = &e->boxes[b];
    *box = (box_t){.type = type, .nslots = nslots[type], .used = true};
    for (int i = 0; i < box->nslots; i++) {
        const int r = newrow(e, b);
        if (r < 0) {
            for (int j = 0; j < i; j++) e->rows[box->slot[j]].used = false;
            box->used = false;
            return false;
        }
        box->slot[i] = r;
    }
    if (xslot[type] != 0xFF) put(e, box->slot[xslot[type]], 0, 'x');
    put(e, e->row, e->pos, BOX + b);
    e->row = box->slot[first[type]];
    e->pos = 0;
    return true;
}

/* where the cursor's row sits: its box and which slot it is */
static box_t *parent(ed_t *e, int *slot) {
    const uint8_t b = e->rows[e->row].box;
    if (b == 0xFF) return NULL;
    box_t *box = &e->boxes[b];
    for (*slot = 0; box->slot[*slot] != e->row; (*slot)++) {}
    return box;
}

static int boxpos(ed_t *e, box_t *box) {
    const row_t *r = &e->rows[box->row];
    const uint8_t item = BOX + (uint8_t)(box - e->boxes);
    int i = 0;
    while (r->it[i] != item) i++;
    return i;
}

void ed_left(ed_t *e) {
    if (e->pos) {
        const uint8_t it = e->rows[e->row].it[e->pos - 1];
        if (it >= BOX) {   /* into the box, at the end of its last slot */
            box_t *box = &e->boxes[it - BOX];
            e->row = box->slot[box->nslots - 1];
            e->pos = e->rows[e->row].n;
        } else {
            e->pos--;
        }
        return;
    }
    int s;
    box_t *box = parent(e, &s);
    if (!box) return;
    if (s) {
        e->row = box->slot[s - 1];
        e->pos = e->rows[e->row].n;
    } else {
        e->row = box->row;
        e->pos = boxpos(e, box);
    }
}

void ed_right(ed_t *e) {
    const row_t *r = &e->rows[e->row];
    if (e->pos < r->n) {
        const uint8_t it = r->it[e->pos];
        if (it >= BOX) {
            e->row = e->boxes[it - BOX].slot[0];
            e->pos = 0;
        } else {
            e->pos++;
        }
        return;
    }
    int s;
    box_t *box = parent(e, &s);
    if (!box) return;
    if (s + 1 < box->nslots) {
        e->row = box->slot[s + 1];
        e->pos = 0;
    } else {
        e->row = box->row;
        e->pos = boxpos(e, box) + 1;
    }
}

/* the slot above/below the current one, or -1 */
static int vertical(box_t *box, int s, bool up) {
    switch (box->type) {
        case B_FRAC: case B_NCR: case B_NPR: return up ? (s == 1 ? 0 : -1) : (s == 0 ? 1 : -1);
        case B_DEFINT:
            if (s == 0) return up ? 1 : -1;
            if (s == 1) return up ? -1 : 0;
            return up ? 1 : 0;
    }
    return -1;
}

static void updown(ed_t *e, bool up) {
    int s;
    box_t *box = parent(e, &s);
    if (!box) return;
    const int to = vertical(box, s, up);
    if (to >= 0) {
        e->row = box->slot[to];
        if (e->pos > e->rows[e->row].n) e->pos = e->rows[e->row].n;
        return;
    }
    if (box->type == B_POW && !up) {   /* down out of an exponent */
        e->row = box->row;
        e->pos = boxpos(e, box) + 1;
        return;
    }
    /* try the box around this one */
    const uint8_t r = e->row, p = e->pos;
    e->row = box->row;
    e->pos = boxpos(e, box);
    if (e->rows[e->row].box == 0xFF) {
        e->row = r, e->pos = p;
        return;
    }
    int s2;
    box_t *outer = parent(e, &s2);
    if (vertical(outer, s2, up) < 0) e->row = r, e->pos = p;
    else updown(e, up);
}

void ed_up(ed_t *e) {
    updown(e, true);
}

void ed_down(ed_t *e) {
    updown(e, false);
}

void ed_del(ed_t *e) {
    row_t *r = &e->rows[e->row];
    if (e->pos) {
        const uint8_t it = r->it[e->pos - 1];
        if (it >= BOX) {
            ed_left(e);
            return;
        }
        memmove(r->it + e->pos - 1, r->it + e->pos, r->n - e->pos);
        r->n--;
        e->pos--;
        return;
    }
    int s;
    box_t *box = parent(e, &s);
    if (!box) return;
    if (r->n) {   /* at the start of a slot with something in it */
        ed_left(e);
        return;
    }
    /* empty slot: remove the box, keep what the other slots hold */
    const int at = boxpos(e, box);
    const int dest = box->row;
    row_t *d = &e->rows[dest];
    memmove(d->it + at, d->it + at + 1, d->n - at - 1);
    d->n--;
    int pos = at;
    for (int i = 0; i < box->nslots; i++) {
        row_t *sr = &e->rows[box->slot[i]];
        if (i == xslot[box->type]) continue;   /* the variable of d/dx and dx goes */
        for (int j = 0; j < sr->n; j++)
            if (put(e, dest, pos, sr->it[j])) {
                pos++;
                sr->it[j] = 0;   /* moved: freerow must not free its box */
            }
    }
    for (int i = 0; i < box->nslots; i++) {
        row_t *sr = &e->rows[box->slot[i]];
        for (int j = 0; j < sr->n; j++)
            if (sr->it[j] >= BOX) freebox(e, sr->it[j] - BOX);
        sr->used = false;
    }
    box->used = false;
    e->row = dest;
    e->pos = at;
}

/* ---- serializing ---- */

typedef struct {
    char *p;
    uint16_t *map;
    int n, size;
} sout_t;

static void emit(sout_t *o, const char *s, uint16_t at) {
    while (*s && o->n < o->size - 1) {
        o->map[o->n] = at;
        o->p[o->n++] = *s++;
    }
    o->p[o->n] = 0;
}

static void serrow(const ed_t *e, int r, sout_t *o);

static void serslot(const ed_t *e, const box_t *b, int s, sout_t *o) {
    serrow(e, b->slot[s], o);
}

static void serbox(const ed_t *e, const box_t *b, sout_t *o, uint16_t at) {
    static const char *const open[] = {"frac(", "^(", "sqrt(", "root(", "abs(", "logb(", "der(", "int(", "int(", "lim(", "",
                                       "ncr(", "npr("};
    /* the order the slots are written in */
    static const uint8_t order[][4] = {{0, 1}, {0}, {0}, {0, 1}, {0}, {0, 1}, {1, 0}, {0, 1}, {2, 3, 0, 1}, {2, 0, 1}, {0, 1},
                                       {0, 1}, {0, 1}};
    emit(o, open[b->type], at);
    for (int i = 0; i < b->nslots; i++) {
        if (i) emit(o, ",", at);
        emit(o, "(", at);
        serslot(e, b, order[b->type][i], o);
        emit(o, ")", at);
    }
    if (b->type != B_RANGE) emit(o, ")", at);
}

static void serrow(const ed_t *e, int r, sout_t *o) {
    const row_t *row = &e->rows[r];
    for (int i = 0; i < row->n; i++) {
        const uint8_t it = row->it[i];
        const uint16_t at = (uint16_t)(r << 8 | i);
        if (it >= BOX) {
            serbox(e, &e->boxes[it - BOX], o, at);
        } else if (it >= TOKEN) {
            /* a letter right before a name would join it: x then sin -> x*sin */
            if (o->n && o->p[o->n - 1] >= 'a' && o->p[o->n - 1] <= 'z') emit(o, "*", at);
            emit(o, tokenText[it - TOKEN], at);
        } else {
            const char c[2] = {(char)it, 0};
            if (it >= 'a' && it <= 'z' && o->n && o->p[o->n - 1] >= 'a' && o->p[o->n - 1] <= 'z') emit(o, "*", at);
            emit(o, c, at);
        }
    }
}

int ed_serialize(const ed_t *e, char *out, uint16_t *map, int size) {
    sout_t o = {out, map, 0, size};
    out[0] = 0;
    serrow(e, 0, &o);
    return o.n;
}

void ed_seek(ed_t *e, uint16_t at) {
    e->row = at >> 8;
    e->pos = at & 0xFF;
    if (e->pos > e->rows[e->row].n) e->pos = e->rows[e->row].n;
}

int ed_vars(const char *s, char *out, int max) {
    static const char *const names[] = {"asin", "acos", "atan", "sin", "cos", "tan", "logb", "log", "ln", "abs",
                                        "ncr", "npr", "sqrt", "root", "frac", "der", "int", "lim", "pi", "inf"};
    int n = 0;
    bool hasx = false;
    while (*s) {
        bool name = false;
        for (unsigned i = 0; i < sizeof names / sizeof names[0]; i++) {
            const size_t len = strlen(names[i]);
            if (!strncmp(s, names[i], len)) {
                s += len;
                name = true;
                break;
            }
        }
        if (name) continue;
        const char c = *s++;
        if (c < 'a' || c > 'z' || c == 'e') continue;
        if (c == 'x') hasx = true;
        if (!memchr(out, c, n) && n < max) out[n++] = c;
    }
    if (hasx) {   /* x first */
        char *x = memchr(out, 'x', n);
        memmove(out + 1, out, x - out);
        out[0] = 'x';
    }
    return n;
}
