#include "OCheckpoint.h"
#include "Level.h"
#include "Savedata.h"

OCheckpoint::OCheckpoint(Level& level, EntityType type): GameEntity(level, type) {}
OCheckpoint::~OCheckpoint() {}

void OCheckpoint::onInteract(GameEntity&) {
	WorldData& wd = level.getWorldData();
	wd.player.x = getX() / 16;
	wd.player.y = getY() / 16;
	commitCurrentSavedata(wd);
	commitGlobalSavedata();
	popup("msg_saved");
}
