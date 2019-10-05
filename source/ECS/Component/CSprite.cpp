#include <algorithm>

#include "CSprite.h"
#include "Graphics.h"

void SpriteComponent::calcBoundingBox() {
	const SpriteData* spr = spriteGet(sprite);
	int left  = (((spr->bbox.left  - spr->xorig)     * abs(tr.xscale)));
	int right = (((spr->bbox.right - spr->xorig + 1) * abs(tr.xscale)));
	int top   = (((spr->bbox.top   - spr->yorig)     * abs(tr.yscale)));
	int bot   = (((spr->bbox.bot   - spr->yorig + 1) * abs(tr.yscale)));
	Point lt(left, top);  lt.rotate(-tr.angle); lt.translate(spr->xorig + 0, spr->yorig + 0);
	Point rt(right, top); rt.rotate(-tr.angle); rt.translate(spr->xorig - 1, spr->yorig + 0);
	Point lb(left, bot);  lb.rotate(-tr.angle); lb.translate(spr->xorig + 0, spr->yorig - 1);
	Point rb(right, bot); rb.rotate(-tr.angle); rb.translate(spr->xorig - 1, spr->yorig - 1);
	left  = std::min(lt.x, std::min(rt.x, std::min(lb.x, rb.x)));
	right = std::max(lt.x, std::max(rt.x, std::max(lb.x, rb.x)));
	top   = std::min(lt.y, std::min(rt.y, std::min(lb.y, rb.y)));
	bot   = std::max(lt.y, std::max(rt.y, std::max(lb.y, rb.y)));
	bbox = Rectangle(left, right, top, bot);
}

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
