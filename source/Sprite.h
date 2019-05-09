#ifndef SPRITE_H
#define SPRITE_H

#include "types.h"

enum Sprite: uint8_t {
	SPR_NONE,
	SPR_PLAYER, SPR_KEY, SPR_CHEST, SPR_BRICKWALL, SPR_WALL,
	SPR_BALL, SPR_SWITCH, SPR_LEVER, SPR_CONVEYOR, SPR_GATELOCK,
	SPR_GATE, SPR_WEAPONS, SPR_ITEMS, SPR_HPUP, SPR_MAP,
	SPR_CHECKPOINT, SPR_ENEMYMUSH, SPR_SKELETON, SPR_SLIME,
	SPR_SPIDER, SPR_SPIRIT, SPR_CHESTCRAB, SPR_EVENTMARK,
	SPR_SWORD, SPR_PICKAXE, SPR_BOOMERANG,
	MAX_SPRITE
};

struct SpriteData {
	uint8_t texture;
	uint8_t imgcount;
	float xorig;
	float yorig;
	Rectangle bbox;
};

//void spritesInit(const char* filename);

const SpriteData* spriteGet(Sprite id);
uint8_t spriteGetTexture(Sprite id, uint8_t frame = 0);

#endif /* SPRITE_H */
