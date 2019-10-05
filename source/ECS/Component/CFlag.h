#ifndef CFLAG_H
#define CFLAG_H

#include <stdint.h>

class Level;
struct FlagComponent {
	uint16_t id = 0;
	void mark(Level&);
	bool isMarked(const Level&);
};

#endif /* CFLAG_H */