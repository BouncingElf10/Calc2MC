/* host stand-in for fontlibc, reading the .convfont text fonts */
#ifndef FONTLIBC_H
#define FONTLIBC_H

#include <stdbool.h>
#include <stdint.h>

typedef struct fontlib_font fontlib_font_t;

bool fontlib_SetFont(const fontlib_font_t *font, int flags);
void fontlib_SetLineSpacing(uint8_t above, uint8_t below);
unsigned fontlib_GetStringWidth(const char *s);
uint8_t fontlib_GetGlyphWidth(char c);
void fontlib_SetCursorPosition(unsigned x, uint8_t y);
void fontlib_DrawGlyph(char c);
void fontlib_SetForegroundColor(uint8_t c);
void fontlib_SetWindowFullScreen(void);
void fontlib_SetTransparency(bool t);
void fontlib_SetNewlineOptions(uint8_t o);
void fontlib_SetFirstPrintableCodePoint(char c);

#endif
