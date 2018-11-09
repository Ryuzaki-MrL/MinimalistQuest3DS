#include "CSprite.h"
#include "Graphics.h"

void SpriteComponent::update() {
	if ((steps != 0) && (++counter == abs(steps))) {
		uint8_t ic = spriteGet(sprite)->imgcount;
		frame = ((steps > 0) ? (frame + 1) : (frame + ic - 1)) % (ic);
		counter = 0;
	}
}

void SpriteComponent::draw(float x, float y, Renderer& render) {
	const SpriteData* spr = spriteGet(sprite);
	render.drawSpriteExt(spriteGetTexture(sprite, frame), x, y, spr->xorig, spr->yorig, tr.xscale, tr.yscale, tr.angle, color.rgba);
}
