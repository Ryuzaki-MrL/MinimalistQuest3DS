#ifndef OBOUNCEBALL_H
#define OBOUNCEBALL_H

#include "EntityDef.h"

NEWENTITY(OBounceBall, OBJ_BOUNCEBALL, onMove(), onDamage(uint8_t, GameEntity&))

#endif /* OBOUNCEBALL_H */