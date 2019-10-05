#include "OBounceBall.h"
#include "Level.h"

OBounceBall::OBounceBall(Level& level, EntityType type): GameEntity(level, type) {
	mv.fric = 204;
}
OBounceBall::~OBounceBall() {}

void OBounceBall::onMove() {
	spr.tr.angle += 2.25 * mv.speed;
	mv.speed -= 0.025;
	if (mv.speed < 0.025) mv.speed = 0;
}

void OBounceBall::onDamage(uint8_t, GameEntity& damager) {
	mv.speed = std::min(std::max(damager.getStats().atk + 1.0, 1.5), 16.0);
	mv.direction = damager.mv.direction;
}
