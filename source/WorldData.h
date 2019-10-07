#ifndef WORLDDATA_H
#define WORLDDATA_H

#include <stdint.h>

#include "Bitset.h"

#define USERFLAG_COUNT	(128 * 8)
#define WEAPON_COUNT	4
#define ITEM_COUNT		4
#define FRIEND_COUNT	4
#define SECTION_COUNT	(9 * 6)
#define MAX_KEYS		8

struct PlayerData {
	uint8_t chp = 10;
	uint8_t mhp = 10;
	uint8_t x = 0;
	uint8_t y = 0;
};

struct InventoryData {
  private:
	uint8_t weaponlvl[WEAPON_COUNT];
	uint8_t inventory[ITEM_COUNT];
	uint8_t keys;
	uint8_t friends;
  public:
	uint8_t curweapon;
	uint8_t curitem;

	inline void addKey() { if (keys < MAX_KEYS) ++keys; }
	inline bool useKey() { return (keys > 0) ? (keys--) : (false); }
	inline uint8_t countKeys() const { return keys; }

	inline void addFriend(uint8_t id) { friends |= (1 << id); }
	inline bool hasFriend(uint8_t id) const { return (friends >> id) & 1; }
	inline uint8_t countFriends() const { return __builtin_popcount(friends); }

	inline void addItem(size_t id) { if (inventory[id] < 99) ++inventory[id]; }
	inline bool useItem(size_t id) { return (inventory[id] > 0) ? (inventory[id]--) : (false); }
	inline uint8_t countItem(size_t id) const { return inventory[id]; }

	inline uint8_t getWeapon(size_t id) const { return weaponlvl[id]; }
	inline void upgradeWeapon(size_t id) { if (weaponlvl[id] < 9) ++weaponlvl[id]; }
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

struct WorldData {
	Bitset<USERFLAG_COUNT> uflags;
	MapData map;
	InventoryData inv;
	PlayerData player;
};

#endif /* WORLDDATA_H */
