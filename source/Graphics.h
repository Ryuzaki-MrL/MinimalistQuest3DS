#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <citro3d.h>
#include <citro2d.h>
#include <stdint.h>

#define RGBA8(r, g, b, a) C2D_Color32(r, g, b, a)
#define C_BLACK RGBA8(0,0,0,0xFF)
#define C_WHITE RGBA8(0xFF,0xFF,0xFF,0xFF)

enum Texture: uint8_t {
	TEX_BACKGROUND, TEX_MAP,
	MAX_TEXTURE
};

enum Font: uint8_t {
	FNT_DEFAULT, FNT_SQUARE, FNT_ARIAL9,
	MAX_FONT
};

class Renderer {
  private:
	float tx, ty;
	C3D_RenderTarget* top;
	C3D_RenderTarget* right;
	C3D_RenderTarget* bot;
	C3D_RenderTarget* currtarg;
	C2D_SpriteSheet texturesheet;
	C2D_SpriteSheet spritesheet;
	C2D_SpriteSheet tileset;
	C2D_TextBuf internalbuf;

  public:
	Renderer();
	~Renderer();

	void frameStart();
	void setTargetScreen(gfxScreen_t screen, gfx3dSide_t side = GFX_LEFT);
	float get3D();
	void frameEnd();

	void screenClear(u32 color);
	void screenBlend(u32 color);
	void screenTranslate(float x, float y);

	void drawRectangle(float x, float y, float w, float h, u32 color);
	void drawSprite(uint8_t id, float x, float y);
	void drawSpriteExt(uint8_t id, float x, float y, float xorig, float yorig, float xscale, float yscale, float angle, u32 color);
	void drawTile(uint8_t id, int x, int y);
	void drawTexture(Texture id, int x, int y);
	void drawTextureColor(Texture id, int x, int y, u32 color, float blend);
	void drawTextureFill(Texture id);

	void drawText(Font font, float x, float y, float size, u32 color, bool center, const char* str);
	void drawTextFormat(Font font, float x, float y, float size, u32 color, bool center, const char* str, ...);
};

#endif /* GRAPHICS_H */