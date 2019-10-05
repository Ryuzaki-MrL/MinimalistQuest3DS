#include "OPickable.h"
#include "Level.h"

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
	switch(type) {
		case OBJ_KEY: wd.inv.addKey(); break;
		case OBJ_FRIEND: wd.inv.addFriend(loot.type); popup("msg_thanks"); break;
		case OBJ_MAP: wd.map.hasmap = true; break;
		case OBJ_HPUP: wd.player.mhp++; break;
		case OBJ_POTION: wd.player.chp = std::min(uint8_t(wd.player.chp+8), wd.player.mhp); break;
		default: break;
	}
}
