#ifndef CSPRITE_H
#define CSPRITE_H

#include "Sprite.h"

struct SprTrData {
	int16_t angle = 0;
	int8_t xscale = 1;
	int8_t yscale = 1;
};

class Renderer;
struct SpriteComponent {
	Rectangle bbox;
	SprTrData tr;
	Color color;
	Sprite sprite;
	int8_t steps;
	uint8_t frame;
  private:
	uint8_t counter = 0;

  public:
	void calcBoundingBox();

	SpriteComponent(Sprite spr = SPR_NONE, int8_t steps = 0): color(0xFFFFFFFF), sprite(spr), steps(steps), frame(0) {
		calcBoundingBox();
	}

	void update();
	void draw(float x, float y, Renderer& render);

	inline const Rectangle& getBoundingBox() const { return bbox; }

	inline void setSprite(Sprite spr) { sprite = spr; calcBoundingBox(); }
	inline void setAngle(int16_t angle) { tr.angle = angle; calcBoundingBox(); }
	inline void rotate(int16_t angle) { tr.angle += angle; calcBoundingBox(); }
	inline void scale(int8_t xscale, int8_t yscale) { tr.xscale = xscale; tr.yscale = yscale; calcBoundingBox(); }
};

#endif /* CSPRITE_H */