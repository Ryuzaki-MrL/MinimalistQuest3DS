#include "OEventBlock.h"
#include "Level.h"

OEventBlock::OEventBlock(Level& level, EntityType type): GameEntity(level, type) {
	spr.color.c.a = 0;
}
OEventBlock::~OEventBlock() {}

void OEventBlock::onKill() {
	flg.mark(level);
}

void OEventBlock::onUpdate() {
	if (scr.isDone()) {
		kill();
	} else if (!scr.isRunning() && collideWith(level.getPlayer())) {
		scr.run();
	}
}

void OEventBlock::onLoad() {
	if (flg.isMarked(level)) kill();
}
