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

/* every comparison: one name, a heading for each side, an exam tip,
 * at least three points per side, and a title that reads "A vs B" */
static void checkComparisons(void) {
    int topics = 0;
    for (int i = 0; i < N_COMPARE; i++) {
        const section_t *sec = &sections[N_SECTIONS - N_COMPARE + i];
        int names = 0, heads = 0, pros = 0, cons = 0, evals = 0;
        topics += compareTopic[i] != NULL;
        assert(strstr(sec->title, " vs "));
        for (const char *const *p = sec->lines; *p; p++)
            switch (**p) {
                case '@': names++; break;
                case '%': heads++; break;
                case '~': pros++; break;
                case '-': cons++; break;
                case '!': evals++; break;
                default: break;
            }
        assert(names == 1 && heads == 2 && evals == 1);
        assert(pros >= 3 && cons >= 3);
    }
    assert(topics == 3);
}

/* the calculator font is plain ASCII plus the glyphs in content.h; items[]
 * in main.c holds 400 names; rows[] holds 400 wrapped lines per page
 * (ponytail: ~34 chars per row is a rough guess at the proportional font) */
static void checkFits(void) {
    int names = 0;
    for (int s = 0; s < N_SECTIONS; s++) {
        int rows = 0;
        for (const char *const *p = sections[s].lines; *p; p++) {
            for (const char *c = *p; *c; c++)
                assert((*c >= ' ' && *c <= '~') || (*c >= 1 && *c <= 8) || *c == '\t');
            names += **p == '@';
            rows += (int)strlen(*p) / 34 + 1;
        }
        if (rows >= 400) fprintf(stderr,"%s: ~%d rows\n", sections[s].title, rows);
        assert(rows < 400);
    }
    assert(names <= 512);
}

int main(void) {
    assert(top("coulomb", "Coulomb's law", 2));
    assert(top("colomb law", "Coulomb's law", 3));
    assert(top("emf", "EMF", 2));
    assert(top("intrnl res", "Internal resistance", 3));
    assert(top("resistivty", "Resistivity", 1));
    assert(top("electric potential", "Electric potential", 2));
    assert(top("millikan", "Millikan", 3));
    assert(top("kirchoff", "Kirchhoff", 3));
    assert(top("ohms law", "Ohm's law", 2));
    assert(top("half life", "Half-life", 2));
    assert(top("terminal pd", "Terminal", 3));
    assert(top("thermistor", "Thermistor", 2));
    assert(top("ldr", "LDR", 3));
    assert(top("lenz", "Lenz's law", 2));
    assert(top("faraday", "Faraday's law", 3));
    assert(top("velocity selectr", "Velocity selector", 2));
    assert(top("flux linkage", "Magnetic flux linkage", 3));
    assert(top("ampere definition", "Definition of the ampere", 3));
    assert(top("bainbridge", "Bainbridge", 3));
    assert(top("fleming", "Fleming", 2));
    assert(top("helical", "Helical motion", 2));
    assert(top("charge to mass", "Charge-to-mass ratio", 3));
    assert(top("motional emf", "Motional EMF", 3));
    assert(top("rotating coil", "EMF of a rotating coil", 3));
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

    /* comparisons are searchable too */
    assert(top("series parallel", "Series vs parallel", 3));
    assert(top("ohmic non ohmic", "Ohmic vs non-ohmic", 3));
    assert(top("gravitational field", "Electric field vs gravitational field", 5));
    assert(top("faraday lenz", "Faraday's law vs Lenz's law", 3));
    checkComparisons();
    checkFits();

    assert(search("xyzzy", hits, 40) == 0);
    assert(search("", hits, 40) == 0);
    puts("search ok");
    return 0;
}
