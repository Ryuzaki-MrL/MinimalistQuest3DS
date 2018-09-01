#include "OGate.h"
#include "Level.h"

OGate::OGate(Level& level, EntityType type): GameEntity(level, type) {}
OGate::~OGate() {}

void OGate::onKill() {
	flg.mark(level);
}

void OGate::onInteract(GameEntity&) {
	if ((type == OBJ_LOCKEDGATE) && level.getWorldData().inv.useKey()) kill();
}

void OGate::onLoad() {
	if (flg.isMarked(level)) kill();
}
