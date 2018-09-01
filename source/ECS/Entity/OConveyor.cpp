#include "OConveyor.h"
#include "Level.h"
#include "defs.h"

OConveyor::OConveyor(Level& level, EntityType type): GameEntity(level, type) {}
OConveyor::~OConveyor() {}

void OConveyor::onUpdate() {
	level.collisionHandle(getBoundingBox(), *this, [](GameEntity& me, GameEntity& other) {
		if (other.hasProperty(PROPERTY_PICKABLE | PROPERTY_WEIGHT)) {
			// does not interfere with movement component
			float rad = degtorad(me.getSprite().tr.angle);
			float spd = me.getSprite().steps / 32.0;
			other.move(spd * cos(rad), spd * -sin(rad));
		}
	});
}
