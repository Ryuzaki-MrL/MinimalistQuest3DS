#include "OConveyor.h"
#include "Level.h"
#include "defs.h"

OConveyor::OConveyor(Level& level, EntityType type): GameEntity(level, type) {}
OConveyor::~OConveyor() {}

void OConveyor::onUpdate() {
	level.collisionHandle(getBoundingBox(), uid, [this](GameEntity& other) {
		if (other.hasProperty(PROPERTY_PICKABLE | PROPERTY_WEIGHT)) {
			// does not interfere with movement component
			float rad = degtorad(spr.tr.angle);
			float spd = spr.steps / 32.0;
			other.move(spd * cos(rad), spd * -sin(rad));
		}
	});
}
