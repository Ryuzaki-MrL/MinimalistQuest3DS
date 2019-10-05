#ifndef CTIMER_H
#define CTIMER_H

#include <stdint.h>

#define TIMED_OUT 1

struct TimerComponent {
	uint16_t counter = 0;
	uint16_t limit = 0;
	bool update();
};

#endif /* CTIMER_H */