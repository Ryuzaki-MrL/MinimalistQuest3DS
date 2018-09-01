#include "CMovement.h"
#include "GameEntity.h"
#include "defs.h"

void MovementComponent::update(GameEntity& ent) {
	float rad = degtorad(direction);
	if (!ent.move(speed * cos(rad), speed * -sin(rad))) {
		rad = degtorad(direction + 90);
		if ((direction % 90) == 0) {
			direction += 180;
		} else if (ent.move(speed * cos(rad), speed * -sin(rad))) {
			direction += 90;
		} else {
			direction -= 90;
		}
		speed *= (fric / 255.0);
	}
}
