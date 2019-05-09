#ifndef CMOVEMENT_H
#define CMOVEMENT_H

#include <stdint.h>

class GameEntity;
struct MovementComponent {
	float speed = 0;
	int16_t direction = 0;
	uint8_t fric = 255;

	void update(GameEntity& ent);
	inline void set(float spd, int16_t dir) {
		speed = spd;
		direction = dir;
	}
};

#endif /* CMOVEMENT_H */