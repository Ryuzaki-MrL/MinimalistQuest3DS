#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "CMovement.h"
#include "CPosition.h"
#include "CReserved.h"
#include "CTimer.h"
#include "CPath.h"
#include "CSprite.h"
#include "CFlag.h"
#include "CLoot.h"
#include "CStats.h"

enum {
	COMPONENT_MOVEMENT	= (1 << 0),
	COMPONENT_POSITION	= (1 << 1),
	COMPONENT_TIMER		= (1 << 2),
	COMPONENT_SPRITE	= (1 << 3),
	COMPONENT_STATS		= (1 << 4),
	COMPONENT_FLAG		= (1 << 5),
	COMPONENT_LOOT		= (1 << 6),
	COMPONENT_PATH		= (1 << 7),
	COMPONENT_RESERVED	= (1 << 8),
	COMPONENT_SCRIPT	= (1 << 9)
};

#endif /* COMPONENTS_H */