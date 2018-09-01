#include "OItem.h"
#include "Level.h"

OItem::OItem(Level& level, EntityType type): GameEntity(level, type) {
	spr.frame = type - OBJ_ITEM;
}
OItem::~OItem() {}

void OItem::onKill() {
	InventoryData& inv = level.getWorldData().inv;
	inv.addItem(type - OBJ_ITEM);
	inv.curitem = type - OBJ_ITEM;
}
