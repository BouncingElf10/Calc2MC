#include "fonts.h"

static const unsigned char body[] = {
    #include "body.inc"
};
static const unsigned char small[] = {
    #include "small.inc"
};

const fontlib_font_t *font_body = (const fontlib_font_t *)body;
const fontlib_font_t *font_small = (const fontlib_font_t *)small;
