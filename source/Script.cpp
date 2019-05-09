#include "Script.h"
#include "rzdb.h"

static RZDB_Chunk* scrs = nullptr;

void scriptsLoad(const char* filename) {
	RZDB_File* rf = RZDB_OpenFile(filename);
	scrs = RZDB_ReadChunkIndexed(rf, sizeof(char), RZDB_ReadSize(rf));
	RZDB_CloseFile(rf);
}

const char* scriptGet(uint8_t id) {
	return RZDB_ChunkGetDataAt(scrs, id);
}
