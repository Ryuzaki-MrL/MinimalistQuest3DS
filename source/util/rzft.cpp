/// Custom font format for use with Citro2D (RyuZaki FonT)

#include <citro2d.h>

#include "rzft.h"

#pragma pack(1)

struct rzftHeader {
	char magic[4]; // RZFT
	C2D_SpriteSheet sheet;
	uint16_t lineHeight;
	uint16_t baseline;
	uint16_t npages;
	uint16_t spacing;
};

struct rzftChar {
	uint16_t glyph;
	int16_t xoffs;
	int16_t yoffs;
	int16_t advance;
};

struct rzftPage {
	rzftChar chr[0x100];
};

struct RZFT_Font {
	rzftHeader hdr;
	rzftPage pages[];
};

RZFT_Font* RZFT_LoadFont(const char* fname, const char* sheetpath) {
	FILE* file = fopen(fname, "rb");
	if (!file) return NULL;

	fseek(file, 0, SEEK_END);
	size_t size = ftell(file);
	rewind(file);

	RZFT_Font* font = (RZFT_Font*)linearAlloc(size);
	if (!font) return NULL;
	fread(font, 1, size, file);
	fclose(file);

	font->hdr.sheet = C2D_SpriteSheetLoad(sheetpath);

	return font;
}

void RZFT_FreeFont(RZFT_Font* font) {
	if (!font) return;
	C2D_SpriteSheetFree(font->hdr.sheet);
	linearFree(font);
}

float RZFT_GetTextWidth(const RZFT_Font* font, float scale, const char* text) {
	float w = 0, mw = 0;
	while(*text) {
		u32 c;
		int units = decode_utf8(&c, (const u8*)text);
		if (units == -1) break;
		text += units;
		if (c == '\n') {
			if (w > mw) mw = w;
			w = 0;
		} else {
			w += font->hdr.spacing + font->pages[c >> 8].chr[c & 0xFF].advance * scale;
		}
	}
	return ((w > mw) ? w : mw);
}

static void RZFT_WordWrap(const RZFT_Font* font, float scale, int wrap, char* text) {
	if (wrap <= 0) return;
	int len = 0, lastlen = 0;
	char* wpos = text;
	while(*text) {
		u32 c;
		int units = decode_utf8(&c, (const u8*)text);
		if (units == -1) break;
		if (c == ' ') {
			wpos = text;
			lastlen = len;
		} else if (c == '\n') {
			len = 0;
			text += units;
			continue;
		}
		text += units;
		rzftChar chr = font->pages[c >> 8].chr[c & 0xFF];
		len += font->hdr.spacing + chr.advance * scale;
		if (len > wrap) {
			*wpos = '\n';
			len -= lastlen;
		}
	}
}

static void RZFT_DrawTextInternal(const RZFT_Font* font, float x, float y, float scaleX, float scaleY, u32 color, int center, const char* text) {
	if (center) {
		x -= ((int)RZFT_GetTextWidth(font, scaleX, text) >> 1);
	}
	int xx = x, yy = y;
	C2D_ImageTint tint;
	C2D_PlainImageTint(&tint, color, 1.0f);
	while(*text) {
		u32 c;
		int units = decode_utf8(&c, (const u8*)text);
		if (units == -1) break;
		text += units;
		if (c == '\n') {
			yy += font->hdr.lineHeight * scaleY;
			xx = x;
			continue;
		}
		rzftChar chr = font->pages[c >> 8].chr[c & 0xFF];
		C2D_DrawImageAt(C2D_SpriteSheetGetImage(font->hdr.sheet, chr.glyph), xx+chr.xoffs, yy+chr.yoffs, 0.0f, &tint, scaleX, scaleY);
		xx += font->hdr.spacing + chr.advance * scaleX;
	}
}

void RZFT_DrawText(const RZFT_Font* font, float x, float y, float scaleX, float scaleY, u32 color, int wrapX, int center, const char* text) {
	static char buffer[256] = "";
	if (wrapX >= 0) {
		strncpy(buffer, text, sizeof(buffer) - 1);
		RZFT_WordWrap(font, scaleX, wrapX, buffer);
		RZFT_DrawTextInternal(font, x, y, scaleX, scaleY, color, center, buffer);
	} else {
		RZFT_DrawTextInternal(font, x, y, scaleX, scaleY, color, center, text);
	}
}

void RZFT_DrawTextFormat(const RZFT_Font* font, float x, float y, float scaleX, float scaleY, u32 color, int wrapX, int center, const char* text, ...) {
	static char buffer[256] = "";
	va_list args;
	va_start(args, text);
	vsnprintf(buffer, sizeof(buffer) - 1, text, args);
	RZFT_WordWrap(font, scaleX, wrapX, buffer);
	RZFT_DrawTextInternal(font, x, y, scaleX, scaleY, color, center, buffer);
	va_end(args);
}
