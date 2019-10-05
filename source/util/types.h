#ifndef TYPES_H
#define TYPES_H

#include <3ds/types.h>
#include "defs.h"

struct Point {
	int x, y;
	Point(int x, int y): x(x), y(y) {}
	~Point() {}
	inline void translate(int xx, int yy) { x += xx; y += yy; }
	inline void rotate(int angle) {
		float a = degtorad(angle);
		int nx = (x * cos(a) - y * sin(a));
		int ny = (y * cos(a) + x * sin(a));
		x = nx;
		y = ny;
	}
};

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
	Color() {}
	Color(uint32_t rgba): rgba(rgba) {}
	uint32_t rgba;
	struct c {
		uint8_t r,g,b,a;
	} c;
};

#endif /* TYPES_H */