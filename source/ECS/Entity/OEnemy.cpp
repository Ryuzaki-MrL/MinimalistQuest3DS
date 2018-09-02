#include "OEnemy.h"
#include "Level.h"
#include "defs.h"

OEnemy::OEnemy(Level& level, EntityType type): GameEntity(level, type) {}
OEnemy::~OEnemy() {}

void OEnemy::onMove() {
	if (!hasProperty(PROPERTY_SHOWDIR)) {
		double c = cos(degtorad(mv.direction));
		spr.tr.xscale = (0 < c) - (c < 0) + (c == 0);
	}
}

void OEnemy::onKill() {
	if (type == OBJ_CHESTCRAB) {
		GameEntity* ch = level.instanceCreate(getX(), getY(), OBJ_CHEST);
		if (ch) ch->getLoot() = loot; // Passing whole state also allows for a locked chest crab
	} else {
		level.instanceCreate(getX(), getY(), EntityType(loot.type));
	}
}

void OEnemy::onTimeOut() {
	path.type -= 200; // restore path
	if (curstats.chp == 0) kill();
}

void OEnemy::onDamage(uint8_t amt, GameEntity& damager) {
	if (curstats.chp == 0) damager.getStats().addExp(getBaseStats().baseexp * curstats.lvl, damager);
	if (amt == 0) popup("msg_nodamage");
	spr.color.c.a = 0x9F;
	path.type += 200; // store path
	mv.speed = 0;
	tmc.limit = getBaseStats().baseimm >> 1;
}