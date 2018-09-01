#include "rle.h"

#define MARKMASK 0b10000000

size_t rleCompress(char* out, const char* in, size_t sizein) {
	int cnt = 1;
	char cur = *in;
	char* start = out;
	for (size_t i = 1; i <= sizein; ++i) {
		if (i < sizein && in[i] == cur) {
			++cnt;
		} else {
			if (cnt == 1) {
				*out = cur & 0x7F; ++out;
			} else {
				while(cnt >= 0x7F) {
					*out = 0xFF; ++out;
					cnt -= 0x7F;
				}
				if (cnt > 1) {
					*out = (cnt & 0x7F) | MARKMASK; ++out;
				} else if (cnt == 1) {
					*out = cur & 0x7F; ++out;
				}
				*out = cur; ++out;
			}
			cur = in[i];
			cnt = 1;
		}
	}
	return (out - start);
}

size_t rleDecompress(char* out, const char* in, size_t sizein) {
	char* start = out;
	for (size_t i = 0; i < sizein; ++i) {
		bool mark = in[i] & MARKMASK;
		size_t cnt = !mark;
		while(mark) {
			cnt += in[i++] & ~(MARKMASK);
			if (i == sizein) return 0;
			mark = in[i] & MARKMASK;
		}
		for (size_t j = 0; j < cnt; ++j) {
			*out = in[i]; ++out;
		}
	}
	return (out - start);
}
