#ifndef OENEMY
#define OENEMY

#include "EntityDef.h"

NEWENTITY(OEnemy, OBJ_ENEMY, onMove(), onDamage(uint8_t, GameEntity&), onTimeOut(), onKill())

#endif /* OENEMY */