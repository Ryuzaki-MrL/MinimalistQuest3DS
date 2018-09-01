#ifndef OPLAYER_H
#define OPLAYER_H

#include "EntityDef.h"

NEWENTITY(OPlayer, OBJ_PLAYER, onKill(), onUpdate(), onMove(), onTimeOut(), onDamage(uint8_t, GameEntity&))

bool isPlayerAttacking();
void signalAttackEnd();

#endif /* OPLAYER_H */