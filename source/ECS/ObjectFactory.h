#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H

#include "GameEntity.h"

bool gameEntityFactory(GameEntity* out, EntityType id, Level& level);

#endif /* OBJECTFACTORY_H */