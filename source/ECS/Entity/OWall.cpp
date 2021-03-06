#include "OWall.h"
#include "Level.h"
#include "Graphics.h"

OWall::OWall(Level& level, EntityType type): GameEntity(level, type) {}
OWall::~OWall() {}

void OWall::onKill() {
	flg.mark(level);
}

void OWall::onLoad() {
	if (flg.isMarked(level)) {
		kill();
	} else if (group >= 128 && group <= 144) {
		spr.setSprite((group & 1) ? SPR_NONE : SPR_WALL);
	}
}

void OWall::onDraw(Renderer& render) {
	if (group > 144) {
		render.drawRectangle(pos.x, pos.y, 16 * spr.tr.xscale, 16 * spr.tr.yscale, C_BLACK);
	}
}
