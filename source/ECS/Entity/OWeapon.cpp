#include "OWeapon.h"
#include "Level.h"

OWeapon::OWeapon(Level& level, EntityType type): GameEntity(level, type) {
	spr.frame = type - OBJ_WEAPON;
}
OWeapon::~OWeapon() {}

void OWeapon::onKill() {
	InventoryData& inv = level.getWorldData().inv;
	inv.upgradeWeapon(type - OBJ_WEAPON);
	inv.curweapon = type - OBJ_WEAPON;
}
