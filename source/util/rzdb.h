#ifndef RZDB_H
#define RZDB_H

#include <stdio.h>

struct RZDB_File;
struct RZDB_Chunk;

RZDB_File* RZDB_OpenFile(const char* fname);
void RZDB_CloseFile(RZDB_File* file);

size_t RZDB_ReadChunk(RZDB_File* file, char* out, size_t chksize);
size_t RZDB_ReadString(RZDB_File* file, char* out);
size_t RZDB_ReadSize(RZDB_File* file);

RZDB_Chunk* RZDB_ReadChunkIndexed(RZDB_File* file, size_t chksize, size_t cnt);
const char* RZDB_ChunkGetDataAt(RZDB_Chunk* chunk, size_t idx);
size_t RZDB_ChunkGetCount(RZDB_Chunk* chunk);
void RZDB_FreeChunk(RZDB_Chunk* chunk);

#endif /* RZDB_H */