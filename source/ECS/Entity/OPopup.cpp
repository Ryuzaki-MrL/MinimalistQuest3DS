#include "OPopup.h"
#include "Level.h"
#include "Graphics.h"
#include "Message.h"

OPopup::OPopup(Level& level, EntityType type): GameEntity(level, type) {
	mv.speed = 2;
	mv.direction = 90;
}
OPopup::~OPopup() {}

void OPopup::onUpdate() {
	if (mv.speed <= 0) {
		spr.color.c.a -= 13;
		if (spr.color.c.a <= 8) kill();
	} else {
		mv.speed -= 0.1;
		if (mv.speed < 0) mv.speed = 0;
	}
}

void OPopup::onDraw(Renderer& render) {
	render.screenTranslate(-render.get3D(), 0);
	render.drawText(FNT_ARIAL9, pos.x, pos.y, 1.0f, spr.color.rgba, true, messageGet(loot.type));
	render.screenTranslate(render.get3D(), 0);
}
