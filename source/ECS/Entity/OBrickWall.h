#ifndef OBRICKWALL_H
#define OBRICKWALL_H

#include "EntityDef.h"

NEWENTITY(OBrickWall, OBJ_BRICKWALL, onKill(), onLoad(), onDamage(uint8_t, GameEntity&))

#endif /* OBRICKWALL_H */