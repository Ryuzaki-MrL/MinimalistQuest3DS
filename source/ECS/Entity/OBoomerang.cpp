#include "OBoomerang.h"
#include "Level.h"
#include "defs.h"

#define invdata	level.getWorldData().inv

OBoomerang::OBoomerang(Level& level, EntityType type): GameEntity(level, type) {
	mv.speed = level.getPlayer().getStats().spd * 1.25;
}
OBoomerang::~OBoomerang() {}

void OBoomerang::onKill() {
	signalAttackEnd();
}

void OBoomerang::onMove() {
	damage(OBJ_BRICKWALL);
	spr.tr.angle += mv.speed * 9;
	level.collisionHandle(getBoundingBox(), uid, [this](GameEntity& other) {
		if (other.hasProperty(PROPERTY_PICKABLE)) other.kill();
		if (other.getType() == OBJ_PLAYER && this->getStats().attacking) this->kill();
	});
	if (curstats.attacking) {
		mv.direction = pointDirection(level.getPlayer().getX(), level.getPlayer().getY(), pos.x, pos.y);
	} else if (
		(!level.checkTile(getBoundingBox(), TILE_PASSABLE) && level.checkSolid(getBoundingBox(), uid)) ||
		(pointDistance(pos.x, pos.y, pos.xs, pos.ys) > curstats.rad*curstats.rad)
	) {
		curstats.attacking = true; // using as a flag for "comeback"
		mv.speed = level.getPlayer().getStats().spd * 2;
	}
}
