#ifndef DEFS_H
#define DEFS_H

#include <math.h>

#define SCREEN_WIDTH	400
#define SUBSCREEN_WIDTH	320
#define SCREEN_HEIGHT	240

#define TILE_WIDTH		16
#define TILE_HEIGHT		16

#define PI 3.14159265

inline float degtorad(int deg) {
	return deg * PI / 180.0;
}

inline int pointDistance(int x1, int y1, int x2, int y2) {
	return ((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}

inline int pointDirection(int x1, int y1, int x2, int y2) {
	return -(atan2(y2-y1, x2-x1) * 180.0 / PI) + 180;
}

inline int signum(int val) {
	return (0 < val) - (val < 0);
}

inline int clamp(int val, int low, int hi) {
	return ((val < low) ? low : ((val > hi) ? hi : val));
}

#endif /* DEFS_H */