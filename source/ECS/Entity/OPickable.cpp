#include "OPickable.h"
#include "Level.h"

#define PILL_TIME (60 * 5)
#define POTION_CURE 8

OPickable::OPickable(Level& level, EntityType type): GameEntity(level, type) {}
OPickable::~OPickable() {}

void OPickable::onLoad() {
	if (hasComponent(COMPONENT_FLAG) && flg.id && flg.isMarked(level)) kill();
	if (hasComponent(COMPONENT_LOOT)) spr.frame = loot.type;
}

void OPickable::onKill() {
	if (hasComponent(COMPONENT_FLAG) && flg.id) {
		if (flg.isMarked(level)) return;
		flg.mark(level);
	}
	WorldData& wd = level.getWorldData();
	OPlayer& p = level.getPlayer();
	switch(type) {
		case OBJ_KEY:
			wd.inv.addKey(); break;
		case OBJ_FRIEND:
			wd.inv.addFriend(loot.type); popup("msg_thanks"); break;
		case OBJ_MAP:
			wd.map.hasmap = true; break;
		case OBJ_HPUP:
			wd.player.mhp++; break;
		case OBJ_POTION:
			wd.player.chp = std::min(uint8_t(wd.player.chp+POTION_CURE), wd.player.mhp); break;
		case OBJ_PILL:
			p.curstats.imm = PILL_TIME; p.spr.color.c.a = 0xAF; break;
		default: break;
	}
}
