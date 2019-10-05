/// Custom database format (RyuZaki DataBase)

#include <3ds.h>

#include "rzdb.h"

#pragma pack(1)

struct RZDB_File {
	FILE* handle;
};

struct RZDB_Chunk {
	char* data;
	u16 cnt;
	size_t chkidx[];
};

RZDB_File* RZDB_OpenFile(const char* fname) {
	FILE* handle = fopen(fname, "rb");
	if (!handle) return NULL;

	u32 magic = 0;
	fread(&magic, 1, sizeof(magic), handle);
	if (magic != 0x42445a52) {
		fclose(handle);
		return NULL;
	}
	RZDB_File* file = (RZDB_File*)linearAlloc(sizeof(RZDB_File));
	if (!file) return NULL;

	file->handle = handle;

	return file;
}

void RZDB_CloseFile(RZDB_File* file) {
	if (!file) return;
	fclose(file->handle);
	linearFree(file);
}

size_t RZDB_ReadChunk(RZDB_File* file, char* out, size_t chksize) {
	size_t sz = RZDB_ReadSize(file);
	return fread(out, chksize, sz, file->handle);
}

size_t RZDB_ReadString(RZDB_File* file, char* out) {
	return RZDB_ReadChunk(file, out, sizeof(char));
}

size_t RZDB_ReadSize(RZDB_File* file) {
	size_t sz = 0;
	size_t shift = 0;
	u8 tmp = 0;
	do {
		fread(&tmp, 1, sizeof(tmp), file->handle);
		sz |= (tmp & 0x7F) << shift;
		shift += 7;
	} while(tmp > 127);
	return sz;
}

RZDB_Chunk* RZDB_ReadChunkIndexed(RZDB_File* file, size_t chksize, size_t cnt) {
	int pos = ftell(file->handle);
	RZDB_Chunk* chk = (RZDB_Chunk*)linearAlloc(sizeof(RZDB_Chunk) + sizeof(size_t) * cnt);
	if (!chk) return NULL;
	chk->cnt = cnt;
	chk->chkidx[0] = 0;
	for (u16 i = 1; i <= cnt; ++i) {
		chk->chkidx[i] = RZDB_ReadSize(file) * chksize;
		fseek(file->handle, chk->chkidx[i], SEEK_CUR);
		chk->chkidx[i] += chk->chkidx[i-1] + 1;
	}
	fseek(file->handle, pos, SEEK_SET);
	chk->data = (char*)linearAlloc(chk->chkidx[cnt]);
	if (!chk->data) {
		linearFree(chk);
		return NULL;
	}
	char* out = chk->data;
	for (u16 i = 0; i < cnt; ++i) {
		out += RZDB_ReadChunk(file, out, chksize);
		*out = 0; ++out;
	}
	return chk;
}

const char* RZDB_ChunkGetDataAt(RZDB_Chunk* chunk, size_t idx) {
	return chunk->data + chunk->chkidx[idx];
}

size_t RZDB_ChunkGetCount(RZDB_Chunk* chunk) {
	return chunk->cnt;
}

void RZDB_FreeChunk(RZDB_Chunk* chunk) {
	if (!chunk) return;
	linearFree(chunk->data);
	linearFree(chunk);
}
