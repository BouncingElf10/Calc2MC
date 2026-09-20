#ifndef SEARCH_H
#define SEARCH_H

#include <stdint.h>

typedef struct {
    const char *line;   /* the '@' or '#' line that matched */
    uint8_t sect;
    int score;
} hit_t;

/* Lenient search over names/headings (and the text under them).
 * Fills hits best-first, returns how many. */
int search(const char *query, hit_t *hits, int max);

/* If text starts with the item name (case, plurals, "&" for "and" and
 * bracketed parts like "(ROCE)" ignored), returns how many chars of text
 * it covers, else 0. text must be at the start of a word. */
int phraseAt(const char *text, const char *name);

/* letter or digit */
int isWordChar(char c);

#endif
