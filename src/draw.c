#include <fontlibc.h>
#include <graphx.h>
#include <math.h>
#include <string.h>
#include "draw.h"
#include "fonts/fonts.h"

bool penDraw;
uint8_t penFont, penColor;
int curX, curTop, curBot;
char rangeVar = 'x';

static const uint8_t asc[] = {11, 8}, hgt[] = {14, 10};
static const uint8_t raise[] = {6, 4};   /* exponent baseline above the base's */
static uint8_t curFont = 0xFF;
static int clipTop, clipBot = GFX_LCD_HEIGHT;

void clip(int top, int bottom) {
    clipTop = top;
    clipBot = bottom;
    gfx_SetClipRegion(0, top, GFX_LCD_WIDTH, bottom);
}

void pen(bool draw, uint8_t font, uint8_t color) {
    penDraw = draw;
    penFont = font;
    penColor = color;
}

void draw_init(void) {
    static const uint8_t pal[][3] = {
        {0x00, 0x00, 0x00},
        {0xFB, 0xFA, 0xF6},   /* paper */
        {0x1D, 0x24, 0x33},   /* ink */
        {0x0B, 0x6E, 0x64},   /* teal */
        {0xDC, 0xEF, 0xEB},   /* tint: answer box */
        {0xEE, 0xF6, 0xF4},   /* pale: selected step */
        {0x5F, 0x66, 0x76},   /* secondary text */
        {0xD9, 0xDC, 0xD6},   /* rules */
        {0xB0, 0x2E, 0x3E},   /* errors */
        {0xB9, 0xD9, 0xD3},   /* rule beside sub-steps */
    };
    for (uint8_t i = 0; i < sizeof pal / 3; i++)
        gfx_palette[i] = gfx_RGBTo1555(pal[i][0], pal[i][1], pal[i][2]);
    fontlib_SetWindowFullScreen();
    fontlib_SetTransparency(true);
    fontlib_SetNewlineOptions(0);
    fontlib_SetFirstPrintableCodePoint(1);   /* the math glyphs live below 0x20 */
}

static void use(uint8_t f) {
    if (f == curFont) return;
    fontlib_SetFont(f == F_BODY ? font_body : font_small, 0);
    fontlib_SetLineSpacing(0, 0);
    curFont = f;
}

static dim_t dims(int w) {
    return (dim_t){(int16_t)w, asc[penFont], (int16_t)(hgt[penFont] - asc[penFont])};
}

/* glyphs outside the screen are skipped, so wide lines can pan */
dim_t text(const char *s, int x, int y) {
    use(penFont);
    if (!penDraw) return dims(fontlib_GetStringWidth(s));
    const int x0 = x, top = y - asc[penFont];
    fontlib_SetForegroundColor(penColor);
    const bool rowIn = top >= clipTop && top + hgt[penFont] <= clipBot;
    for (; *s; s++) {
        const int w = fontlib_GetGlyphWidth(*s);
        if (rowIn && x >= 0 && x + w <= GFX_LCD_WIDTH) {
            fontlib_SetCursorPosition(x, top);
            fontlib_DrawGlyph(*s);
        }
        x += w;
    }
    return dims(x - x0);
}

static void hline(int x, int y, int w) {
    gfx_SetColor(penColor);
    gfx_HorizLine(x, y, w);
}

static void vline(int x, int y, int h) {
    gfx_SetColor(penColor);
    gfx_VertLine(x, y, h);
}

static void grow(dim_t *d, dim_t k) {
    d->w += k.w;
    if (k.a > d->a) d->a = k.a;
    if (k.d > d->d) d->d = k.d;
}

/* ---- shapes, shared by results and the editor ---- */

typedef dim_t (*layf)(const void *ctx, int x, int y);
typedef struct {
    layf fn;
    const void *ctx;
} kid_t;

static dim_t lay(kid_t k, int x, int y) {
    return k.fn(k.ctx, x, y);
}

static dim_t measure(kid_t k) {
    const bool d = penDraw;
    penDraw = false;
    const dim_t r = k.fn(k.ctx, 0, 0);
    penDraw = d;
    return r;
}

/* in the small font */
static dim_t small(kid_t k, int x, int y) {
    const uint8_t f = penFont;
    penFont = F_SMALL;
    const dim_t r = k.fn(k.ctx, x, y);
    penFont = f;
    return r;
}

static dim_t smallMeasure(kid_t k) {
    const bool d = penDraw;
    penDraw = false;
    const dim_t r = small(k, 0, 0);
    penDraw = d;
    return r;
}

static int axis(void) {
    return penFont == F_SMALL ? 3 : 5;
}

static dim_t fraction(kid_t n, kid_t d, int x, int y) {
    const dim_t dn = measure(n), dd = measure(d);
    const int w = (dn.w > dd.w ? dn.w : dd.w) + 4, bar = y - axis();
    if (penDraw) {
        lay(n, x + (w - dn.w) / 2, bar - 2 - dn.d);
        lay(d, x + (w - dd.w) / 2, bar + 2 + dd.a);
        hline(x + 1, bar, w - 2);
    }
    return (dim_t){(int16_t)w, (int16_t)(axis() + 2 + dn.d + dn.a), (int16_t)(2 + dd.a + dd.d - axis())};
}

/* an exponent after something baseA tall */
static dim_t sup(kid_t e, int baseA, int x, int y) {
    int r = raise[penFont];
    if (baseA > asc[penFont] + 2) r += baseA - asc[penFont] - 2;
    const dim_t de = small(e, x + 1, y - r);
    dim_t d = {(int16_t)(de.w + 2), (int16_t)(r + de.a), 0};
    if (de.d - r > 0) d.d = de.d - r;
    return d;
}

/* nCr and nPr as in the formula booklet: n raised in front, r lowered after */
static dim_t counting(kid_t n, kid_t r, const char *c, int x, int y) {
    dim_t d = sup(n, asc[penFont], x - 1, y);
    grow(&d, text(c, x + d.w, y));
    const dim_t dr = small(r, x + d.w, y + 3);
    d.w += dr.w;
    if (dr.d + 3 > d.d) d.d = dr.d + 3;
    return d;
}

/* brackets, stretched around tall content */
static dim_t wrap(kid_t k, char open, int x, int y) {
    const dim_t dk = measure(k);
    const uint8_t f = penFont;
    if (dk.a <= asc[f] + 1 && dk.d <= hgt[f] - asc[f] + 1) {
        const char o[2] = {open, 0}, cl[2] = {open == '(' ? ')' : open == '[' ? ']' : '|', 0};
        dim_t d = text(o, x, y);
        grow(&d, lay(k, x + d.w, y));
        grow(&d, text(cl, x + d.w, y));
        return d;
    }
    const int top = y - dk.a - 1, h = dk.a + dk.d + 2, w = dk.w + 8;
    if (penDraw) {
        lay(k, x + 4, y);
        for (int side = 0; side < 2; side++) {
            const int lx = side ? x + w - 2 : x + 1, in = side ? -1 : 1;
            if (open == '|') {
                vline(lx, top, h);
            } else if (open == '[') {
                vline(lx, top, h);
                hline(side ? lx - 1 : lx, top, 2);
                hline(side ? lx - 1 : lx, top + h - 1, 2);
            } else {
                vline(lx, top + 2, h - 4);
                vline(lx + in, top, 2);
                vline(lx + in, top + h - 2, 2);
            }
        }
    }
    return (dim_t){(int16_t)w, (int16_t)(dk.a + 1), (int16_t)(dk.d + 1)};
}

static dim_t radical(kid_t k, const kid_t *index, int x, int y) {
    int ix = 0;
    dim_t di = {0, 0, 0};
    if (index) {
        di = smallMeasure(*index);
        ix = di.w > 3 ? di.w - 3 : 0;
    }
    const dim_t dk = measure(k);
    const int top = y - dk.a - 2, bot = y + dk.d, sx = x + ix;
    if (penDraw) {
        gfx_SetColor(penColor);
        gfx_Line(sx, bot - 4, sx + 2, bot - 4);
        gfx_Line(sx + 2, bot - 4, sx + 4, bot);
        gfx_Line(sx + 4, bot, sx + 7, top);
        hline(sx + 7, top, dk.w + 3);
        lay(k, sx + 9, y);
        if (index) small(*index, x, bot - 6 - di.d);
    }
    dim_t d = {(int16_t)(ix + dk.w + 11), (int16_t)(dk.a + 3), dk.d};
    if (index && y - (bot - 6 - di.d - di.a) > d.a) d.a = y - (bot - 6 - di.d - di.a);
    return d;
}

static dim_t integral(kid_t fk, kid_t vk, const kid_t *lo, const kid_t *hi, int x, int y) {
    const dim_t df = measure(fk);
    int a = df.a + 2, d = df.d + 2;
    if (a < asc[penFont] + 3) a = asc[penFont] + 3;
    if (d < 5) d = 5;
    int bw = 0;
    dim_t dl = {0, 0, 0}, dh = {0, 0, 0};
    if (lo) {
        dl = smallMeasure(*lo);
        dh = smallMeasure(*hi);
        bw = dl.w > dh.w ? dl.w : dh.w;
    }
    if (penDraw) {
        gfx_SetColor(penColor);
        vline(x + 3, y - a + 2, a + d - 4);
        gfx_Line(x + 3, y - a + 2, x + 5, y - a);
        hline(x + 5, y - a, 2);
        gfx_Line(x + 3, y + d - 2, x + 1, y + d);
        hline(x, y + d, 2);
        if (lo) {
            small(*hi, x + 8, y - a + dh.a);
            small(*lo, x + 6, y + d - dl.d);
        }
    }
    int at = x + 9 + bw;
    if (penDraw) lay(fk, at, y);
    at += df.w + 2;
    at += text("d", at, y).w;
    at += lay(vk, at, y).w;
    return (dim_t){(int16_t)(at - x), (int16_t)a, (int16_t)d};
}

static dim_t textK(const void *s, int x, int y) {
    return text(s, x, y);
}

static kid_t T(const char *s) {
    return (kid_t){textK, s};
}

static dim_t limit(kid_t vk, kid_t tk, kid_t fk, int x, int y) {
    const dim_t dl = measure(T("lim"));
    const dim_t dv = smallMeasure(vk), dto = smallMeasure(T(G_TO)), dt = smallMeasure(tk);
    const int under = dv.w + dto.w + dt.w, w = under > dl.w ? under : dl.w;
    const int uy = y + (hgt[penFont] - asc[penFont]) + dv.a + 1;
    if (penDraw) {
        text("lim", x + (w - dl.w) / 2, y);
        const int ux = x + (w - under) / 2;
        small(vk, ux, uy);
        small(T(G_TO), ux + dv.w, uy);
        small(tk, ux + dv.w + dto.w, uy);
    }
    dim_t d = {(int16_t)(w + 3), dl.a, (int16_t)(uy - y + (dt.d > dv.d ? dt.d : dv.d))};
    grow(&d, lay(fk, x + w + 3, y));
    return d;
}

/* ---- results ---- */

static dim_t nodeK(const void *e, int x, int y) {
    return node(e, x, y);
}

static kid_t K(const node_t *e) {
    return (kid_t){nodeK, e};
}

static dim_t paren(const node_t *e, bool p, int x, int y) {
    return p ? wrap(K(e), '(', x, y) : node(e, x, y);
}

/* in brackets when it is a sum */
static dim_t sumK(const void *e, int x, int y) {
    const node_t *n = e;
    return paren(n, n->type == N_ADD, x, y);
}

static kid_t P(const node_t *e) {
    return (kid_t){sumK, e};
}

typedef struct {
    const node_t **f;
    int n;
} list_t;

static bool atomic(const node_t *e) {
    return e->type == N_VAR || e->type == N_CONST || e->type == N_FN
        || (e->type == N_NUM && e->v.q.den == 1 && e->v.q.num >= 0)
        || (e->type == N_FLT && e->v.f >= 0);
}

/* a binary operator with a little space each side */
static dim_t op(const char *s, int x, int y) {
    const int pad = penFont == F_SMALL ? 1 : 3;
    dim_t d = text(s, x + pad, y);
    d.w += 2 * pad;
    return d;
}

static dim_t factors(const void *ctx, int x, int y) {
    const list_t *l = ctx;
    dim_t d = {0, 0, 0};
    if (!l->n) return text("1", x, y);
    for (int i = 0; i < l->n; i++) {
        const node_t *e = l->f[i];
        const bool p = l->n > 1 && (e->type == N_ADD || e->type == N_PM || (i && negative(e)));
        if (i && (isnum(e) || (isnum(l->f[i - 1]) && p && negative(e))))
            grow(&d, text(G_DOT, x + d.w, y));
        else if (i && (e->type == N_FN || (e->type == N_POW && e->k[0]->type == N_FN)))
            d.w += 2;
        grow(&d, paren(e, p, x + d.w, y));
    }
    return d;
}

static const char *const fnName[] = {"sin", "cos", "tan", "sin", "cos", "tan", "ln", "log", "log", ""};

static char numbuf[8][24];
static uint8_t nb;

/* digits of n into p, at least min of them */
static char *digits(char *p, long n, int min) {
    char t[12];
    int k = 0;
    do t[k++] = (char)('0' + n % 10), n /= 10; while (n || k < min);
    while (k) *p++ = t[--k];
    return p;
}

/* a decimal with 6 significant digits, trailing zeros dropped */
static const char *fmt(double v) {
    char *s = numbuf[nb++ & 7], *p = s;
    int e = 0;
    if (v < 0) *p++ = '\x96', v = -v;   /* minus glyph */
    if (v != 0 && (v >= 1e9 || v < 1e-4)) {
        e = (int)floor(log10(v));
        v /= pow(10, e);
    }
    int dec = 5 - (v >= 1 ? (int)floor(log10(v)) : 0);
    if (dec < 0) dec = 0;
    long scale = 1;
    for (int i = 0; i < dec; i++) scale *= 10;
    long ip = (long)floor(v), fp = (long)floor((v - ip) * scale + 0.5);
    if (fp >= scale) ip++, fp -= scale;
    p = digits(p, ip, 1);
    if (fp) {
        *p++ = '.';
        p = digits(p, fp, dec);
        while (p[-1] == '0') p--;
    }
    if (e) {
        *p++ = 'E';
        if (e < 0) *p++ = '\x96', e = -e;
        p = digits(p, e, 1);
    }
    *p = 0;
    return s;
}

static const char *intText(int32_t n) {
    char *s = numbuf[nb++ & 7], *p = s;
    if (n < 0) *p++ = '\x96', n = -n;
    *digits(p, n, 1) = 0;
    return s;
}

/* Drawing recurses, so the factor and term lists are worked out in static
 * buffers and only copied to the 4 KB stack at their real size. */
static const node_t *spare[64], *spare2[32];

static dim_t product(const node_t *e, int x, int y) {
    int nt, nbt;
    dim_t d = {0, 0, 0};
    const int sign = fracparts(e, spare, &nt, spare2, &nbt);
    const node_t *top[nt + 1], *bot[nbt + 1];
    memcpy(top, spare, nt * sizeof *top);
    memcpy(bot, spare2, nbt * sizeof *bot);
    if (sign < 0) grow(&d, text(G_MINUS, x, y));
    const list_t lt = {top, nt}, lb = {bot, nbt};
    const kid_t kt = {factors, &lt}, kb = {factors, &lb};
    grow(&d, nbt ? fraction(kt, kb, x + d.w, y) : lay(kt, x + d.w, y));
    return d;
}

static dim_t fnCall(const node_t *e, const node_t *power, int x, int y) {
    dim_t d = text(fnName[e->op], x, y);
    if (e->op >= F_ASIN && e->op <= F_ATAN) grow(&d, sup(T(G_MINUS "1"), asc[penFont], x + d.w, y));
    if (power) grow(&d, sup(K(power), asc[penFont], x + d.w, y));
    if (e->op == F_LOGB) {
        const dim_t b = small(K(e->k[0]), x + d.w, y + 3);
        d.w += b.w;
        if (b.d + 3 > d.d) d.d = b.d + 3;
    }
    grow(&d, wrap(K(e->k[e->n - 1]), '(', x + d.w, y));
    return d;
}

dim_t node(const node_t *e, int x, int y) {
    cas_deeper();
    dim_t d = {0, 0, 0};
    switch (e->type) {
        case N_NUM:
            if (e->v.q.den == 1) return text(intText(e->v.q.num), x, y);
            if (e->v.q.num < 0) grow(&d, text(G_MINUS, x, y));
            grow(&d, fraction(T(intText(e->v.q.num < 0 ? -e->v.q.num : e->v.q.num)), T(intText(e->v.q.den)), x + d.w, y));
            return d;
        case N_FLT:
            return text(fmt(e->v.f), x, y);
        case N_VAR: {
            const char s[2] = {(char)e->op, 0};
            return text(s, x, y);
        }
        case N_CONST:
            return text((const char *[]){G_PI, "e", "i", G_INF}[e->op], x, y);
        case N_ADD: {
            const int n = sumterms(e, spare);
            const node_t *t[n];
            memcpy(t, spare, n * sizeof *t);
            for (int i = 0; i < n; i++) {
                const node_t *m = t[i];
                if (i) {
                    const bool minus = negative(m);
                    if (minus) m = negate(m);
                    grow(&d, op(minus ? G_MINUS : "+", x + d.w, y));
                }
                grow(&d, paren(m, m->type == N_PM || (i && m->type == N_ADD), x + d.w, y));
            }
            return d;
        }
        case N_MUL:
            if (e->n == 2 && isval(e->k[0], -1) && !isnum(e->k[1]) && e->k[1]->type != N_MUL) {
                grow(&d, text(G_MINUS, x, y));
                grow(&d, paren(e->k[1], e->k[1]->type == N_ADD || e->k[1]->type == N_PM, x + d.w, y));
                return d;
            }
            return product(e, x, y);
        case N_POW: {
            const node_t *b = e->k[0], *p = e->k[1];
            if (p->type == N_NUM && p->v.q.num == 1 && p->v.q.den > 1) {
                const kid_t ix = T(intText(p->v.q.den));
                return radical(K(b), p->v.q.den == 2 ? NULL : &ix, x, y);
            }
            if (negative(p)) return product(e, x, y);
            if (b->type == N_FN && b->op < F_ABS && isint(p, NULL)) return fnCall(b, p, x, y);
            d = paren(b, !atomic(b) || b->type == N_FN, x, y);
            grow(&d, sup(K(p), d.a, x + d.w, y));
            return d;
        }
        case N_FN:
            if (e->op == F_ABS) return wrap(K(e->k[0]), '|', x, y);
            if (e->op == F_FACT) {
                const node_t *a = e->k[0];
                d = paren(a, !atomic(a) || a->type == N_FN || negative(a), x, y);
                grow(&d, text("!", x + d.w, y));
                return d;
            }
            if (e->op != F_LOGB && e->n == 2) return counting(K(e->k[0]), K(e->k[1]), e->op == F_NCR ? "C" : "P", x, y);
            return fnCall(e, NULL, x, y);
        case N_EQ:
            d = node(e->k[0], x, y);
            grow(&d, op("=", x + d.w, y));
            grow(&d, node(e->k[1], x + d.w, y));
            return d;
        case N_OR: {
            if (!e->n) return text(e->op ? "Every value works" : "No real solutions", x, y);
            bool common = true;
            for (int i = 0; i < e->n; i++)
                common &= e->k[i]->type == N_EQ && same(e->k[i]->k[0], e->k[0]->k[0]);
            if (common) {
                d = node(e->k[0]->k[0], x, y);
                grow(&d, op("=", x + d.w, y));
            }
            for (int i = 0; i < e->n; i++) {
                if (i) {
                    const uint8_t c = penColor;
                    if (!common) penColor = C_SEC;   /* "or" is quieter than the maths */
                    grow(&d, text(common ? ",  " : "   or   ", x + d.w, y));
                    penColor = c;
                }
                grow(&d, node(common ? e->k[i]->k[1] : e->k[i], x + d.w, y));
            }
            return d;
        }
        case N_PM:
            if (e->n == 1) {
                d = text(G_PM, x, y);
            } else {
                d = node(e->k[0], x, y);
                grow(&d, op(G_PM, x + d.w, y));
            }
            grow(&d, paren(e->k[e->n - 1], e->k[e->n - 1]->type == N_ADD || negative(e->k[e->n - 1]), x + d.w, y));
            return d;
        case N_DERIV: {
            static char dx[3] = "dx";
            dx[1] = (char)e->k[1]->op;
            d = fraction(T("d"), T(dx), x, y);
            d.w++;
            grow(&d, wrap(K(e->k[0]), '(', x + d.w, y));
            return d;
        }
        case N_INTEG: {
            const kid_t lo = K(e->n == 4 ? e->k[2] : e), hi = K(e->n == 4 ? e->k[3] : e);
            return integral(P(e->k[0]), K(e->k[1]), e->n == 4 ? &lo : NULL, e->n == 4 ? &hi : NULL, x, y);
        }
        case N_LIMIT:
            return limit(K(e->k[1]), K(e->k[2]), P(e->k[0]), x, y);
        case N_EVAL: {
            d = wrap(K(e->k[0]), '[', x, y);
            const dim_t hb = small(K(e->k[2]), x + d.w + 1, y - d.a + asc[F_SMALL]);
            const dim_t lb = small(K(e->k[1]), x + d.w + 1, y + d.d);
            d.w += 1 + (hb.w > lb.w ? hb.w : lb.w);
            d.d += lb.d;
            return d;
        }
    }
    return d;
}

/* a caption with its argument drawn where the '@' is */
dim_t caption(const char *cap, const node_t *arg, int x, int y) {
    dim_t d = {0, asc[penFont], (int16_t)(hgt[penFont] - asc[penFont])};
    if (!cap) return d;
    const char *at = strchr(cap, '@');
    char buf[64];
    size_t n = at ? (size_t)(at - cap) : strlen(cap);
    if (n > 63) n = 63;
    memcpy(buf, cap, n);
    buf[n] = 0;
    grow(&d, text(buf, x, y));
    if (at && arg) {
        grow(&d, node(arg, x + d.w, y));
        grow(&d, text(at + 1, x + d.w, y));
    }
    return d;
}

/* ---- the editor ---- */

typedef struct {
    const ed_t *ed;
    uint8_t row;
    bool show;
} rowctx_t;

static dim_t rowK(const void *ctx, int x, int y) {
    const rowctx_t *r = ctx;
    return edrow(r->ed, r->row, x, y, r->show);
}

static dim_t box(const ed_t *ed, const box_t *b, int prevA, int x, int y, bool show) {
    rowctx_t ctx[4];
    kid_t k[4];
    for (int i = 0; i < b->nslots; i++) {
        ctx[i] = (rowctx_t){ed, b->slot[i], show};
        k[i] = (kid_t){rowK, &ctx[i]};
    }
    dim_t d;
    switch (b->type) {
        case B_FRAC: return fraction(k[0], k[1], x, y);
        case B_POW: return sup(k[0], prevA, x, y);
        case B_SQRT: return radical(k[0], NULL, x, y);
        case B_ROOT: return radical(k[1], &k[0], x, y);
        case B_ABS: return wrap(k[0], '|', x, y);
        case B_LOGB: {
            d = text("log", x, y);
            const dim_t db = small(k[0], x + d.w, y + 3);
            d.w += db.w;
            if (db.d + 3 > d.d) d.d = db.d + 3;
            grow(&d, wrap(k[1], '(', x + d.w, y));
            return d;
        }
        case B_DERIV: {
            /* d over d and the variable */
            const dim_t dv = measure(k[0]), dw = measure(T("d"));
            const int w = dv.w + dw.w + 4, bar = y - axis();
            if (penDraw) {
                text("d", x + (w - dw.w) / 2, bar - 2 - dw.d);
                text("d", x + 2, bar + 2 + dv.a);
                lay(k[0], x + 2 + dw.w, bar + 2 + dv.a);
                hline(x + 1, bar, w - 2);
            }
            d = (dim_t){(int16_t)(w + 1), (int16_t)(axis() + 2 + dw.d + dw.a), (int16_t)(2 + dv.a + dv.d - axis())};
            grow(&d, wrap(k[1], '(', x + d.w, y));
            return d;
        }
        case B_INT: return integral(k[0], k[1], NULL, NULL, x, y);
        case B_DEFINT: return integral(k[2], k[3], &k[0], &k[1], x, y);
        case B_LIM: return limit(k[0], k[1], k[2], x, y);
        case B_NCR: return counting(k[0], k[1], "C", x, y);
        case B_NPR: return counting(k[0], k[1], "P", x, y);
        default: {   /* B_RANGE: lo <= x <= hi */
            const char v[2] = {rangeVar, 0};
            d = lay(k[0], x, y);
            grow(&d, op(G_LE, x + d.w, y));
            grow(&d, text(v, x + d.w, y));
            grow(&d, op(G_LE, x + d.w, y));
            grow(&d, lay(k[1], x + d.w, y));
            return d;
        }
    }
}

static const char *itemText(uint8_t it) {
    static char s[2];
    if (it >= TOKEN) {
        if (it == TOKEN + T_PI) return G_PI;
        if (it == TOKEN + T_INF) return G_INF;
        return tokenText[it - TOKEN];
    }
    switch (it) {
        case '*': return G_TIMES;
        case '/': return G_DIV;
        case '-': return G_MINUS;
    }
    s[0] = (char)it;
    s[1] = 0;
    return s;
}

dim_t edrow(const ed_t *ed, int row, int x, int y, bool show) {
    cas_deeper();
    const row_t *r = &ed->rows[row];
    const bool here = show && ed->row == row;
    const uint8_t f = penFont;
    if (penDraw && here) {   /* the cursor needs the row's full height */
        penDraw = false;
        const dim_t full = edrow(ed, row, 0, y, show);
        penDraw = true;
        curTop = y - full.a;
        curBot = y + full.d;
    }
    dim_t d = {0, asc[f], (int16_t)(hgt[f] - asc[f])};
    if (!r->n) {   /* an empty slot */
        if (row && penDraw) {
            gfx_SetColor(here ? C_TEAL : C_SEC);
            gfx_Rectangle(x + 1, y - asc[f] + 3, f == F_SMALL ? 5 : 7, asc[f] - 3);
        }
        if (here) curX = x + 1;
        d.w = row ? (f == F_SMALL ? 7 : 9) : 2;
        return d;
    }
    int prevA = asc[f];
    for (int i = 0; i < r->n; i++) {
        if (here && ed->pos == i) curX = x + d.w;
        const uint8_t it = r->it[i];
        dim_t k;
        if (it >= BOX) {
            k = box(ed, &ed->boxes[it - BOX], prevA, x + d.w, y, show);
        } else if (it >= TOKEN + T_ASIN && it <= TOKEN + T_ATAN) {
            /* sin^-1( */
            k = text(fnName[it - TOKEN], x + d.w, y);
            grow(&k, sup(T(G_MINUS "1"), asc[f], x + d.w + k.w, y));
            grow(&k, text("(", x + d.w + k.w, y));
        } else if (it == '+' || it == '-' || it == '=') {
            k = op(itemText(it), x + d.w, y);
        } else {
            k = text(itemText(it), x + d.w, y);
        }
        prevA = k.a;
        grow(&d, k);
    }
    if (here && ed->pos == r->n) curX = x + d.w;
    return d;
}
