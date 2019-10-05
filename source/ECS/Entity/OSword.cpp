#include "OSword.h"
#include "Level.h"

#define invdata	level.getWorldData().inv

OSword::OSword(Level& level, EntityType type): GameEntity(level, type) {
	mv.speed = curstats.spd;
	tmc.limit = (16 / mv.speed) + 1; // TODO: use sprite width here
}
OSword::~OSword() {}

void OSword::onKill() {
	signalAttackEnd();
}

void OSword::onTimeOut() {
	if (mv.speed > 0) {
		mv.speed *= -1.5;
	} else {
		kill();
	}
	tmc.limit = (16 / abs(mv.speed));
}

void OSword::onMove() {
	damage((type != OBJ_PICKAXE) ? OBJ_BRICKWALL : OBJ_NONE);
}
