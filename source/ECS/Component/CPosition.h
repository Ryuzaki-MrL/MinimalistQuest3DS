#ifndef CPOSITION_H
#define CPOSITION_H

struct PositionComponent {
	float x, y;
	uint16_t xs, ys;

	PositionComponent() {}
	PositionComponent(int x, int y): x(x), y(y), xs(x), ys(y) {}
	inline void reset() { x = xs; y = ys; }
};

#endif /* CPOSITION_H */