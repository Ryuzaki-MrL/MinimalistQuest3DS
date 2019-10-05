#include "CTimer.h"

bool TimerComponent::update() {
	if ((limit != 0) && (++counter >= limit)) {
		counter = limit = 0;
		return true;
	}
	return false;
}
