#include <unordered_map>

#include "Message.h"
#include "rzdb.h"

typedef std::unordered_map<std::string, uint8_t> MsgDict;

static uint8_t lang = 0;
static RZDB_Chunk* langs = nullptr;
static RZDB_Chunk* msgentries = nullptr;
static RZDB_Chunk* messages = nullptr;
static MsgDict* msgmap = nullptr;

void messageInit(const char* filename) {
	RZDB_File* rf = RZDB_OpenFile(filename);
	uint8_t langcnt = RZDB_ReadSize(rf);
	langs = RZDB_ReadChunkIndexed(rf, sizeof(char), langcnt);
	uint8_t entrycnt = RZDB_ReadSize(rf);
	msgentries = RZDB_ReadChunkIndexed(rf, sizeof(char), entrycnt);
	msgmap = new MsgDict[langcnt];
	messages = RZDB_ReadChunkIndexed(rf, sizeof(char), entrycnt * langcnt);
	for (uint8_t i = 0; i < langcnt; ++i) {
		for (uint8_t j = 0; j < entrycnt; ++j) {
			msgmap[i][std::string(RZDB_ChunkGetDataAt(msgentries, j))] = i*entrycnt + j;
		}
	}
	RZDB_CloseFile(rf);
}

void messageSetLang(uint8_t newlang) {
	lang = newlang % RZDB_ChunkGetCount(langs);
}

uint8_t messageGetLang() {
	return lang;
}

const char* messageGet(uint8_t id) {
	return RZDB_ChunkGetDataAt(messages, lang*RZDB_ChunkGetCount(msgentries) + id);
}

const char* messageGet(const std::string& id) {
	const char* msg = RZDB_ChunkGetDataAt(messages, msgmap[lang][id]);
	return msg ? msg : "NULL";
}

const char* messageGetLangStr() {
	return RZDB_ChunkGetDataAt(langs, lang);
}

uint8_t messageGetIndex(const std::string& id) {
	return msgmap[0][id];
}
