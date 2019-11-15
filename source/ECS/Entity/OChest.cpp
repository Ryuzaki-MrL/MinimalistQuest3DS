#include "OChest.h"
#include "Level.h"
#include "Textbox.h"
#include "Message.h"

OChest::OChest(Level& level, EntityType type): GameEntity(level, type) {}
OChest::~OChest() {}

void OChest::onInteract(GameEntity& other) {
	if (flg.isMarked(level)) return;
	if (!loot.locked || level.getWorldData().inv.useKey()) {
		GameEntity* ent = level.instanceCreate(other.getX(), other.getY(), EntityType(loot.type));
		if (ent && ent->getType() == OBJ_FRIEND) ent->loot.type = 2;
		uint8_t msgitem = messageGetIndex("msg_item00") + loot.type;
		textboxCreateFormat(messageGet("msg_gotitem"), messageGet(msgitem));
		flg.mark(level); spr.frame = 1;
	} else {
		textboxCreate(messageGet("msg_locked"));
	}
}

void OChest::onLoad() {
	spr.frame = flg.isMarked(level);
}
