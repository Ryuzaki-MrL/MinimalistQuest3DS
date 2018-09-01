#include "EntityData.h"
#include "rzdb.h"

static EntityData entities[256];
static BaseStatData basestats[256];

void entitiesLoad(const char* fname) {
	RZDB_File* rf = RZDB_OpenFile(fname);
	RZDB_ReadChunk(rf, (char*)entities, sizeof(EntityData));
	RZDB_ReadChunk(rf, (char*)basestats, sizeof(BaseStatData));
	RZDB_CloseFile(rf);
}

const EntityData& entityGetData(EntityType ent) {
	return entities[ent];
}

const BaseStatData& getBaseStatData(uint8_t idx) {
	return basestats[idx];
}
