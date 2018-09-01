#ifndef CTIMER_H
#define CTIMER_H

#include <stdint.h>

class GameEntity;
struct TimerComponent {
	uint16_t counter = 0;
	uint16_t limit = 0;
	void update(GameEntity& ent);
};

#endif /* CTIMER_H */