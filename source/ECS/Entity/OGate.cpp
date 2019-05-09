#include "OGate.h"
#include "Level.h"
#include "Textbox.h"
#include "Message.h"

OGate::OGate(Level& level, EntityType type): GameEntity(level, type) {}
OGate::~OGate() {}

void OGate::onKill() {
	flg.mark(level);
}

void OGate::onInteract(GameEntity&) {
	if (type != OBJ_LOCKEDGATE) return;
	if (level.getWorldData().inv.useKey()) {
		kill();
	} else {
		textboxCreate(messageGet("msg_locked"));
	}
}

void OGate::onLoad() {
	if (flg.isMarked(level)) kill();
}
