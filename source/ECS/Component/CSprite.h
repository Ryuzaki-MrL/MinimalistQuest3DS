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
	Color color;
	SprTrData tr;
	int8_t steps;
	uint8_t frame;
	Sprite sprite;
  private:
	uint8_t counter = 0;

  public:
	SpriteComponent(Sprite spr = SPR_NONE, int8_t steps = 0): color(0xFF000000), steps(steps), frame(0), sprite(spr) {}

	void update();
	void draw(float x, float y, Renderer& render);

	// TODO: apply scale and rotation
	inline const Rectangle& getBoundingBox() const { return spriteGet(sprite)->bbox; }
};

#endif /* CSPRITE_H */