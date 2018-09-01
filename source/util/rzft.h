#ifndef RZFT_H
#define RZFT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <citro2d.h>
#include <stdarg.h>

typedef struct s_rzftfont RZFT_Font;

RZFT_Font* RZFT_LoadFont(const char* fname, const char* sheetpath);
void RZFT_FreeFont(RZFT_Font* font);

float RZFT_GetTextWidth(const RZFT_Font* font, float scale, const char* text);

void RZFT_DrawText(const RZFT_Font* font, float x, float y, float scaleX, float scaleY, u32 color, int wrapX, int center, const char* text);
void RZFT_DrawTextFormat(const RZFT_Font* font, float x, float y, float scaleX, float scaleY, u32 color, int wrapX, int center, const char* text, ...);

#ifdef __cplusplus
}
#endif

#endif /* RZFT_H */