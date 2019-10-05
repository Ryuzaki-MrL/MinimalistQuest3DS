#include "OLever.h"
#include "Level.h"

OLever::OLever(Level& level, EntityType type): GameEntity(level, type) {}
OLever::~OLever() {}

void OLever::onInteract(GameEntity& other) {
	applyDamage(other);
}

void OLever::onDamage(uint8_t, GameEntity&) {
	scr.run();
	scr.setArg(1, loot.type);
	scr.setArg(2, group);
	loot.locked = !loot.locked;
	scr.setArg(0, loot.locked);
	uint8_t img = spriteGet(spr.sprite)->imgcount - 1;
	spr.frame = img * !loot.locked;
	spr.steps = (loot.locked - !loot.locked) * (20 / img);
	tmc.limit = 20;
}

void OLever::onTimeOut() {
	spr.steps = 0;
}
