#ifndef TYPES_H
#define TYPES_H

#include <3ds/types.h>

// TODO: keep platform-specific code away from headers
// and create "platform.cpp" for filesystem and language
// detection functions

struct Rectangle {
	int left, right, top, bot;
	Rectangle() {}
	Rectangle(int l, int r, int t, int b): left(l), right(r), top(t), bot(b) {}
	~Rectangle() {}
	inline bool collideWith(const Rectangle& other) const {
		return !((other.left > right) || (other.right < left) || (other.top > bot) || (other.bot < top));
	}
	inline const Rectangle operator+(const Rectangle other) const {
		return Rectangle((other.left + left), (other.right + right), (other.top + top), (other.bot + bot));
	};
};

union Color {
	Color(uint32_t rgba): rgba(rgba) {}
	uint32_t rgba;
	struct c {
		uint8_t r,g,b,a;
	} c;
};

inline int signum(int val) {
	return (0 < val) - (val < 0);
}

inline int clamp(int val, int low, int hi) {
	return ((val < low) ? low : ((val > hi) ? hi : val));
}

#endif /* TYPES_H */