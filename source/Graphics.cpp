#include <unordered_map>

#include "Graphics.h"
#include "rzft.h"
#include "defs.h"

static const char* fontpath[] = { "",
	"romfs:/fontmain.rzft",
	"romfs:/arial9.rzft"
};

static const char* fontsheetpath[] = { "",
	"romfs:/fontmain.t3x",
	"romfs:/arial9.t3x"
};

static C2D_TextBuf staticbuf;
static std::unordered_map<const char*, C2D_Text> txtready; // using a pointer as key is intended here
static C2D_ImageTint s_tint;
static float s_3dside;
static RZFT_Font* fonts[MAX_FONT] = { 0 };

static void cacheTextForRendering(const char* str) {
	if (!staticbuf) {
		staticbuf = C2D_TextBufNew(768);
		C2D_TextBufClear(staticbuf);
	}
	C2D_TextParse(&txtready[str], staticbuf, str);
	C2D_TextOptimize(&txtready[str]);
}

Renderer::Renderer(): tx(0), ty(0) {
	gfxInit(GSP_BGR8_OES, GSP_BGR8_OES, true);
	gfxSet3D(true);
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();

	top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	right = C2D_CreateScreenTarget(GFX_TOP, GFX_RIGHT);
	bot = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

	// TODO: separate textures from render engine
	texturesheet = C2D_SpriteSheetLoad("romfs:/textures.t3x");
	spritesheet = C2D_SpriteSheetLoad("romfs:/sprites.t3x");
	tileset = C2D_SpriteSheetLoad("romfs:/tileset.t3x");

	internalbuf = C2D_TextBufNew(256);

	for (int i = 1; i < MAX_FONT; ++i) {
		fonts[i] = RZFT_LoadFont(fontpath[i], fontsheetpath[i]);
	}
}

Renderer::~Renderer() {
	C2D_TextBufDelete(internalbuf);
	C2D_SpriteSheetFree(texturesheet);
	C2D_SpriteSheetFree(spritesheet);
	C2D_SpriteSheetFree(tileset);

	for (int i = 1; i < MAX_FONT; ++i) {
		RZFT_FreeFont(fonts[i]);
	}

	C2D_Fini();
	C3D_Fini();
	gfxExit();
}

void Renderer::frameStart() {
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TextBufClear(internalbuf);
}

void Renderer::setTargetScreen(gfxScreen_t screen, gfx3dSide_t side) {
	if (screen == GFX_TOP) {
		C2D_SceneBegin(currtarg = ((side==GFX_LEFT) ? top : right));
		s_3dside = (side==GFX_LEFT) ? -1.0 : 1.0;
	} else {
		C2D_SceneBegin(currtarg = bot);
	}
}

float Renderer::get3D() {
	return osGet3DSliderState() * s_3dside;
}

void Renderer::frameEnd() {
	C3D_FrameEnd(0);
}

void Renderer::screenClear(u32 color) {
	//C2D_TargetClear(currtarg, color);
	drawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, color);
}

void Renderer::screenBlend(u32 color) {
	C2D_Fade(color);
}

void Renderer::screenTranslate(float x, float y) {
	tx += x; ty += y;
}

void Renderer::drawRectangle(float x, float y, float w, float h, u32 color) {
	C2D_DrawRectSolid(x - tx, y - ty, 0.0f, w, h, color);
}

void Renderer::drawSprite(uint8_t id, float x, float y) {
	C2D_DrawImageAt(C2D_SpriteSheetGetImage(spritesheet, id), x - tx, y - ty, 0.0f);
}

void Renderer::drawSpriteExt(uint8_t id, float x, float y, float xorig, float yorig, float xscale, float yscale, float angle, u32 color) {
	C2D_PlainImageTint(&s_tint, color, 0.0f);
	C2D_Image img = C2D_SpriteSheetGetImage(spritesheet, id);
	C2D_DrawParams params = {
		{ x+xorig - tx, y+yorig - ty, xscale*img.subtex->width, yscale*img.subtex->height },
		{ xorig, yorig },
		0.0f, degtorad(-angle)
	};
	C2D_DrawImage(img, &params, &s_tint);
}

void Renderer::drawTile(uint8_t id, int x, int y) {
	C2D_DrawImageAt(C2D_SpriteSheetGetImage(tileset, id), x - tx, y - ty, 0.0f);
}

void Renderer::drawTexture(Texture id, int x, int y) {
	C2D_DrawImageAt(C2D_SpriteSheetGetImage(texturesheet, id), x, y, 0.0f);
}

void Renderer::drawTextureColor(Texture id, int x, int y, u32 color, float blend) {
	C2D_PlainImageTint(&s_tint, color, blend);
	C2D_DrawImageAt(C2D_SpriteSheetGetImage(texturesheet, id), x, y, 0.0f, &s_tint);
}

void Renderer::drawTextureFill(Texture id) {
	C2D_Image img = C2D_SpriteSheetGetImage(texturesheet, id);
	int txi = tx, tyi = ty;
	for (u16 x = 0; x <= SCREEN_WIDTH; x += img.subtex->width) {
		for (u16 y = 0; y <= SCREEN_HEIGHT; y += img.subtex->height) {
			C2D_DrawImageAt(img, x - (txi & (img.subtex->width - 1)), y - (tyi & (img.subtex->height - 1)), 0.0f);
		}
	}
}

void Renderer::drawText(Font font, float x, float y, float size, u32 color, bool center, const char* str) {
	if (!fonts[font]) {
		float w;
		if (!txtready.count(str)) cacheTextForRendering(str);
		C2D_TextGetDimensions(&txtready[str], size, size, &w, nullptr);
		C2D_DrawText(&txtready[str], C2D_WithColor, center ? (x - (w/2)) : x, y, 0.0f, size, size, color);
	} else {
		RZFT_DrawText(fonts[font], x - tx, y - ty, size, size, color, SUBSCREEN_WIDTH, center, str);
	}
}

void Renderer::drawTextFormat(Font font, float x, float y, float size, u32 color, bool center, const char* str, ...) {
	static char buffer[256] = "";
	va_list args;
	va_start(args, str);
	vsnprintf(buffer, sizeof(buffer) - 1, str, args);
	if (!fonts[font]) {
		float w;
		C2D_Text tmp;
		C2D_TextParse(&tmp, internalbuf, buffer);
		C2D_TextGetDimensions(&tmp, size, size, &w, nullptr);
		C2D_DrawText(&tmp, C2D_WithColor, center ? (x - (w/2)) : x, y, 0.0f, size, size, color);
	} else {
		RZFT_DrawText(fonts[font], x - tx, y - ty, size, size, color, SUBSCREEN_WIDTH, center, buffer);
	}
	va_end(args);
}
