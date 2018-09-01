#include <3ds.h>

void* operator new(size_t sz) {
	return linearAlloc(sz);
}

void operator delete(void* ptr) {
	linearFree(ptr);
}
