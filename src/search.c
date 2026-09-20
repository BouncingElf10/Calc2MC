#include <stdbool.h>
#include <string.h>
#include "content.h"
#include "search.h"

#define MAX_WORDS 4
#define MAX_LEN 20

static char lower(char c) {
    return c >= 'A' && c <= 'Z' ? c + 32 : c;
}

static bool isWord(char c) {
    c = lower(c);
    return (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9');
}

static bool same(const char *a, const char *b, int n) {
    while (n--)
        if (lower(*a++) != lower(*b++)) return false;
    return true;
}

static int min3(int a, int b, int c) {
    return a < b ? (a < c ? a : c) : (b < c ? b : c);
}

/* Edit distance (with swaps) between w and the closest prefix of c,
 * so half-typed words with typos still match. */
static int prefixDist(const char *w, int m, const char *c, int n) {
    int rows[3][MAX_LEN + 3] = {{0}};
    if (n > m + 2) n = m + 2;
    int *pp = rows[0], *p = rows[1], *cur = rows[2];

    for (int j = 0; j <= n; j++) p[j] = j;
    for (int i = 1; i <= m; i++) {
        cur[0] = i;
        for (int j = 1; j <= n; j++) {
            const bool eq = lower(w[i - 1]) == lower(c[j - 1]);
            cur[j] = min3(p[j] + 1, cur[j - 1] + 1, p[j - 1] + !eq);
            if (i > 1 && j > 1 && lower(w[i - 1]) == lower(c[j - 2]) &&
                lower(w[i - 2]) == lower(c[j - 1]) && pp[j - 2] + 1 < cur[j])
                cur[j] = pp[j - 2] + 1;
        }
        int *t = pp; pp = p; p = cur; cur = t;
    }
    int best = p[0];
    for (int j = 1; j <= n; j++)
        if (p[j] < best) best = p[j];
    return best;
}

static int wordScore(const char *w, int m, const char *c, int n, bool fuzzy) {
    if (n >= m && same(w, c, m)) return n == m ? 110 : 100;
    if (m < 3 || !fuzzy) return 0;
    for (int i = 1; i + m <= n; i++)
        if (same(w, c + i, m)) return 70;
    const int allowed = m <= 4 ? 1 : 2;
    /* cheap rejects keep this fast on the calculator: too short, or the
     * first letter is nowhere near the start (a swap is still fine) */
    if (n < m - allowed) return 0;
    if (lower(c[0]) != lower(w[0]) && (n < 2 || lower(c[1]) != lower(w[0]))) return 0;
    const int d = prefixDist(w, m, c, n);
    return d <= allowed ? 60 - 20 * d : 0;
}

/* best score of query word w against any word in s */
static int bestIn(const char *w, int m, const char *s, bool fuzzy) {
    int best = 0;
    while (*s) {
        while (*s && !isWord(*s)) s++;
        const char *start = s;
        while (isWord(*s)) s++;
        if (s > start) {
            const int sc = wordScore(w, m, start, s - start, fuzzy);
            if (sc > best) best = sc;
        }
    }
    return best;
}

int search(const char *query, hit_t *hits, int max) {
    const char *ws[MAX_WORDS];
    int wl[MAX_WORDS], nw = 0, nHits = 0;

    while (*query && nw < MAX_WORDS) {
        while (*query && !isWord(*query)) query++;
        ws[nw] = query;
        while (isWord(*query)) query++;
        wl[nw] = query - ws[nw];
        if (wl[nw] > MAX_LEN) wl[nw] = MAX_LEN;
        if (wl[nw]) nw++;
    }
    if (!nw) return 0;

    for (int s = 0; s < N_SECTIONS; s++) {
        for (const char *const *p = sections[s].lines; *p; p++) {
            if (**p != '@' && **p != '#') continue;
            int total = 0, found = 0;
            for (int k = 0; k < nw; k++) {
                /* name matches count double; the text under it only exact-ish */
                int sc = 2 * bestIn(ws[k], wl[k], *p + 1, true);
                for (const char *const *q = p + 1; *q && **q != '@' && **q != '#'; q++) {
                    const int b = bestIn(ws[k], wl[k], *q, false);
                    if (b > sc) sc = b;
                }
                total += sc;
                found += sc > 0;
            }
            if (!found) continue;
            /* partial matches rank lower; ties go to the shorter name */
            const int len = strlen(*p);
            total = total * found / nw * 16 - (len < 60 ? len : 60) / 4;

            int i = nHits < max ? nHits++ : max;
            for (; i > 0 && hits[i - 1].score < total; i--)
                if (i < max) hits[i] = hits[i - 1];
            if (i < max) hits[i] = (hit_t){*p, (uint8_t)s, total};
        }
    }
    return nHits;
}

int isWordChar(char c) {
    return isWord(c);
}

/* one word of a name vs one of the text: also cost/costs and and/& */
static bool tokenEq(const char *a, int al, const char *b, int bl) {
    if (al == bl) return same(a, b, al);
    if (al == bl + 1) return lower(a[bl]) == 's' && same(a, b, bl);
    if (bl == al + 1) return lower(b[al]) == 's' && same(a, b, al);
    return al == 3 && bl == 1 && *b == '&' && same(a, "and", 3);
}

int phraseAt(const char *text, const char *name) {
    const char *p = text;
    bool first = true;

    for (;;) {
        while (*name && !isWord(*name) && *name != '&') {
            if (*name == '(') {   /* skip "(ROCE)", "(%)" ... */
                while (*name && *name != ')') name++;
                if (*name) name++;
            } else name++;
        }
        if (!*name) return first ? 0 : p - text;

        const char *n0 = name;
        if (*name == '&') name++;
        else while (isWord(*name)) name++;

        if (!first) {   /* words in the text may be split by spaces or hyphens only */
            const char *q = p;
            while (*q == ' ' || *q == '-') q++;
            if (q == p) return 0;
            p = q;
        }
        const char *t0 = p;
        if (*p == '&') p++;
        else while (isWord(*p)) p++;
        if (p == t0 || !tokenEq(n0, name - n0, t0, p - t0)) return 0;
        first = false;
    }
}
