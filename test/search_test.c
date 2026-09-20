/* Host check for the lenient search. Build and run from the project root:
 *   gcc -Isrc -o search_test test/search_test.c src/search.c src/content.c && ./search_test
 */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../src/content.h"
#include "../src/search.h"

static hit_t hits[40];

/* true if one of the top n hits contains want */
static int top(const char *q, const char *want, int n) {
    const int got = search(q, hits, 40);
    printf("%-18s ->", q);
    for (int i = 0; i < got && i < 3; i++)
        printf(" [%s | %s %d]", hits[i].line + 1, sections[hits[i].sect].title, hits[i].score);
    printf("\n");
    for (int i = 0; i < got && i < n; i++)
        if (strstr(hits[i].line, want)) return 1;
    return 0;
}

int main(void) {
    assert(top("break even", "Break-even", 2));
    assert(top("brek evn", "Break-even", 3));
    assert(top("acid", "Acid test", 1));
    assert(top("roce", "ROCE", 1));
    assert(top("markt shre", "Market share", 2));
    assert(top("gros proft margn", "Gross profit margin", 1));
    assert(top("delegaton", "Delegation", 1));
    assert(top("contrbution", "Contribution", 1));
    assert(top("payback", "Payback", 1));
    assert(top("deprecation", "depreciation", 3) || top("deprecation", "Depreciation", 3));
    assert(top("wrking capitl", "Working capital", 2));
    assert(top("360", "360-degree", 2));   /* reachable once alpha types digits */
    /* phrase matching used for links */
    assert(phraseAt("contribution per unit" TIMES " quantity", "Contribution per unit") == 21);
    assert(phraseAt("fixed costs + x", "Fixed cost") == 11);            /* plural */
    assert(phraseAt("total fixed costs", "Fixed cost") == 0);           /* must start there */
    assert(phraseAt("net profit before interest & tax\t120",
                    "Net profit before interest and tax") == 32);       /* & for and */
    assert(phraseAt("margin of safety" DIV " actual", "Margin of safety (%)") == 16);
    assert(phraseAt("current assets" MINUS, "Working capital (net current assets)") == 0);
    assert(phraseAt("break-even quantity", "Break-even quantity") == 19);
    assert(phraseAt("gross profitability", "Gross profit") == 0);       /* whole words only */

    assert(search("xyzzy", hits, 40) == 0);
    assert(search("", hits, 40) == 0);
    puts("search ok");
    return 0;
}
