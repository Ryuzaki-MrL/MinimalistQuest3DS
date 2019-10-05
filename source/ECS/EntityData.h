#ifndef ENTITYDATA_H
#define ENTITYDATA_H

#include <stdint.h>

enum EntityType: uint8_t {
	OBJ_NONE, OBJ_PLAYER, OBJ_KEY, OBJ_WEAPON,
	OBJ_WSWORD, OBJ_WPICKAXE, OBJ_WBOOMERANG,
	OBJ_ITEM, OBJ_IPOTION, OBJ_IPILL,
	OBJ_HPUP, OBJ_MAP, OBJ_FRIEND, OBJ_CHEST,
	OBJ_LOCKEDGATE, OBJ_BRICKWALL, OBJ_CHECKPOINT,
	OBJ_SKELETON, OBJ_MUSH, OBJ_SLIME, OBJ_SPIRIT,
	OBJ_SPIDER, OBJ_CHESTCRAB, OBJ_ENEMY, OBJ_BOUNCEBALL,
	OBJ_CONVEYOR, OBJ_EVTRIGGER, OBJ_SWITCH,
	OBJ_LEVER, OBJ_WHIT, OBJ_SWORD, OBJ_PICKAXE,
	OBJ_BOOMERANG, OBJ_IUSE, OBJ_POTION, OBJ_PILL,
	OBJ_GATE, OBJ_EVWALL, OBJ_POPUP,
	NUM_ENTITIES
};

struct EntityData {
	uint16_t comps, props;
	uint8_t sprite;
	int8_t anim;
	uint8_t baseidx;
	uint8_t path;
};

struct BaseStatData {
	uint8_t baseexp, baseatk, basedef, basespd, basehp,	baserad;
	uint8_t baseimm, growatk, growdef, growspd, growhp, growrad;
};

void entitiesLoad(const char* fname);
const EntityData& entityGetData(EntityType ent);
const BaseStatData& getBaseStatData(uint8_t idx);

#endif /* ENTITYDATA_H */