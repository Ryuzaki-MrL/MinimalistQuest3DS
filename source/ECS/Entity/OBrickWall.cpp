#include "OBrickWall.h"
#include "Level.h"

OBrickWall::OBrickWall(Level& level, EntityType type): GameEntity(level, type) {}
OBrickWall::~OBrickWall() {}

void OBrickWall::onKill() {
	flg.mark(level);
}

void OBrickWall::onDamage(uint8_t, GameEntity&) {
	if (curstats.chp == 0) kill();
}

void OBrickWall::onLoad() {
	if (flg.isMarked(level)) kill();
}
