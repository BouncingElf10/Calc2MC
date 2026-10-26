/* host stand-in for the CE graphx library: draws into a buffer the tests save */
#ifndef GRAPHX_H
#define GRAPHX_H

#include <stdint.h>

#define GFX_LCD_WIDTH 320
#define GFX_LCD_HEIGHT 240

extern uint16_t gfx_palette[256];
#define gfx_RGBTo1555(r, g, b) ((uint16_t)((((r) >> 3) << 10) | (((g) >> 3) << 5) | ((b) >> 3)))
#define gfx_SetDrawBuffer()

void gfx_Begin(void);
void gfx_End(void);
void gfx_SwapDraw(void);
uint8_t gfx_SetColor(uint8_t c);
void gfx_HorizLine(int x, int y, int w);
void gfx_VertLine(int x, int y, int h);
void gfx_Line(int x0, int y0, int x1, int y1);
void gfx_Rectangle(int x, int y, int w, int h);
void gfx_FillRectangle(int x, int y, int w, int h);
void gfx_SetClipRegion(int xmin, int ymin, int xmax, int ymax);

#endif
