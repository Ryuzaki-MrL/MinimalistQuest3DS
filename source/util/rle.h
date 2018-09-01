#ifndef RLE_H
#define RLE_H

#include <stdlib.h>

size_t rleCompress(char* out, const char* in, size_t sizein);
size_t rleDecompress(char* out, const char* in, size_t sizein);

#endif /* RLE_H */