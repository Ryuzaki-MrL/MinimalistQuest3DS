#ifndef ENTITIES_H
#define ENTITIES_H

#include "GameEntity.h"

// template for creating entities
#define NEWENTITY(NAME, TYPE, ...) \
class NAME final: public GameEntity { \
  private:\
	void __VA_ARGS__; \
  public:\
	NAME(Level&, EntityType type = TYPE); \
	~NAME(); \
};

#endif /* ENTITIES_H */