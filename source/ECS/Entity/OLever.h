#ifndef OLEVER_H
#define OLEVER_H

#include "EntityDef.h"

NEWENTITY(OLever, OBJ_LEVER, onDamage(uint8_t, GameEntity&), onInteract(GameEntity&), onTimeOut())

#endif /* OLEVER_H */