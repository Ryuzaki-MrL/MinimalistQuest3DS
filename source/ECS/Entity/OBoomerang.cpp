#include "OBoomerang.h"
#include "Level.h"
#include "defs.h"

#define invdata	level.getWorldData().inv

OBoomerang::OBoomerang(Level& level, EntityType type): GameEntity(level, type) {
	curstats.setLevel(invdata.weaponlvl[type - OBJ_WHIT]);
	curstats.addExp(invdata.weaponexp[type - OBJ_WHIT], *this);
	mv.speed = ((curstats.lvl + 1) / 2.0);
	if (mv.speed > 2) mv.speed = 2;
}
OBoomerang::~OBoomerang() {}

void OBoomerang::onKill() {
	invdata.weaponexp[type - OBJ_WHIT] = curstats.exp;
	invdata.weaponlvl[type - OBJ_WHIT] = curstats.lvl;
	signalAttackEnd();
}

void OBoomerang::onMove() {
	damage(OBJ_BRICKWALL);
	spr.tr.angle += mv.speed * 10;
	level.collisionHandle(getBoundingBox(), uid, [this](GameEntity& other) {
		if (other.hasProperty(PROPERTY_PICKABLE)) other.kill();
		if (other.getType() == OBJ_PLAYER && me.getStats().attacking) me.kill();
	});
	if (curstats.attacking) {
		mv.direction = pointDirection(level.getPlayer().getX(), level.getPlayer().getY(), pos.x, pos.y);
	} else if (
		(!level.checkTile(getBoundingBox(), TILE_PASSABLE) && level.checkSolid(getBoundingBox(), uid)) ||
		(pointDistance(pos.x, pos.y, pos.xs, pos.ys) > curstats.rad*curstats.rad)
	) {
		curstats.attacking = true; // using as a flag for "comeback"
		mv.speed = std::max(int(mv.speed), level.getPlayer().getStats().spd << 1);
	}
}
