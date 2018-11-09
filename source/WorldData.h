#ifndef WORLDDATA_H
#define WORLDDATA_H

#include <stdint.h>

#include "Bitset.h"

#define USERFLAG_COUNT	(128 * 8)
#define WEAPON_COUNT	4
#define ITEM_COUNT		4
#define SECTION_COUNT	(9 * 6) // hardcoding for this game only
#define MAX_KEYS		8

struct PlayerData {
	uint16_t exp = 0;
	uint8_t lvl = 1;
	uint8_t chp = 10;
	uint8_t mhp = 10;
	uint8_t x = 0;
	uint8_t y = 0;
};

struct InventoryData {
	uint16_t weaponexp[WEAPON_COUNT];
	uint8_t weaponlvl[WEAPON_COUNT];
  private:
	uint8_t inventory[ITEM_COUNT];
	uint8_t keys : 4;
	uint8_t friends : 4;
  public:
	uint8_t curweapon : 4;
	uint8_t curitem : 4;

	inline void addKey() { if (keys < MAX_KEYS) ++keys; }
	inline bool useKey() { return (keys > 0) ? (keys--) : (false); }
	inline uint8_t countKeys() const { return keys; }

	inline void addFriend(uint8_t id) { friends |= (1 << id); }
	inline bool hasFriend(uint8_t id) const { return (friends >> id) & 1; }

	inline void addItem(size_t id) { if (inventory[id] < 99) ++inventory[id]; }
	inline bool useItem(size_t id) { return (inventory[id] > 0) ? (inventory[id]--) : (false); }
	inline uint8_t countItem(size_t id) const { return inventory[id]; }
};

struct MapData {
  private:
	Bitset<SECTION_COUNT> visited;

  public:
	bool hasmap = false;
	inline void visit(size_t sec) { visited.set(sec); }
	inline bool isVisited(size_t sec) const { return visited.test(sec); }
	inline int countVisited() const { return visited.count(); }
};

class WorldData {
  private:
	Bitset<USERFLAG_COUNT> uflags;

  public:
	PlayerData player;
	InventoryData inv;
	MapData map;

	inline void flagSet(size_t id) { uflags.set(id); }
	inline bool flagCheck(size_t id) const { return uflags.test(id); }
};

#endif /* WORLDDATA_H */
