#ifndef RZFT_H
#define RZFT_H

#include <stdarg.h>
#include <stdint.h>

struct RZFT_Font;

RZFT_Font* RZFT_LoadFont(const char* fname, const char* sheetpath);
void RZFT_FreeFont(RZFT_Font* font);

float RZFT_GetTextWidth(const RZFT_Font* font, float scale, const char* text);

void RZFT_DrawText(const RZFT_Font* font, float x, float y, float scaleX, float scaleY, uint32_t color, int wrapX, uint8_t align, const char* text);
void RZFT_DrawTextFormat(const RZFT_Font* font, float x, float y, float scaleX, float scaleY, uint32_t color, int wrapX, uint8_t align, const char* text, ...);

#endif /* RZFT_H */