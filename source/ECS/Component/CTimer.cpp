#include "CTimer.h"
#include "GameEntity.h"

void TimerComponent::update(GameEntity& ent) {
	if ((limit != 0) && (++counter >= limit)) {
		counter = limit = 0;
		ent.onTimeOut();
	}
}
