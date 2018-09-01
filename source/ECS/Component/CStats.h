#ifndef CSTAT_H
#define CSTAT_H

#include <stdint.h>

class GameEntity;
struct StatComponent {
	uint16_t exp = 0;
	uint8_t lvl = 1;
	uint8_t atk;
	uint8_t def;
	uint8_t spd;
	uint8_t mhp = 0;
	uint8_t chp = 0;
	uint8_t rad;
	uint8_t imm = 0;
	uint8_t baseidx = 0;
	bool attacking = false;

	void addExp(uint16_t amt, GameEntity& me);
	void setLevel(uint8_t lvl);
	void validate(); // adjust stats
	void damage(GameEntity& me, GameEntity& other);
};

#endif /* CSTAT_H */