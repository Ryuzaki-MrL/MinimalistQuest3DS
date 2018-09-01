#include "OSword.h"
#include "Level.h"

#define invdata	level.getWorldData().inv

// TODO: receive exp and level from caller so that any entity can use a sword

OSword::OSword(Level& level, EntityType type): GameEntity(level, type) {
	curstats.setLevel(invdata.weaponlvl[type - OBJ_WHIT]);
	curstats.addExp(invdata.weaponexp[type - OBJ_WHIT], *this);
	mv.speed = 1;
	//mv.speed = 0.8 + (curstats.lvl * 0.2);
	tmc.limit = (16 / mv.speed) + 1;
}
OSword::~OSword() {}

void OSword::onKill() {
	invdata.weaponexp[type - OBJ_WHIT] = curstats.exp;
	invdata.weaponlvl[type - OBJ_WHIT] = curstats.lvl;
	signalAttackEnd();
}

void OSword::onTimeOut() {
	tmc.limit = (12 / std::max(int(mv.speed), 1));
	if (mv.speed > 0) {
		mv.speed *= -1.3;
	} else {
		kill();
	}
}

void OSword::onMove() {
	damage((type != OBJ_PICKAXE) ? OBJ_BRICKWALL : OBJ_NONE);
}
