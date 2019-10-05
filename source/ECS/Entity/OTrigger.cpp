#include "OTrigger.h"
#include "Level.h"

OTrigger::OTrigger(Level& level, EntityType type): GameEntity(level, type) {}
OTrigger::~OTrigger() {}

void OTrigger::onUpdate() {
	if (scr.isDone()) {
		flg.mark(level);
		spr.frame = 1;
		scr.reset();
	} else if (!flg.isMarked(level)) {
		bool pressed = level.checkObject(getBoundingBox(), uid, PROPERTY_WEIGHT);
		spr.frame = pressed;
		if (scr.isRunning()) {
			scr.setArg(0, pressed);
		} else if (pressed) {
			scr.run();
			scr.setArg(1, loot.type);
			scr.setArg(2, group);
		}
	}
}

void OTrigger::onLoad() {
	spr.frame = flg.isMarked(level);
}
