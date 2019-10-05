#ifndef CSTAT_H
#define CSTAT_H

#include <stdint.h>

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

	void validate(); // adjust stats
	bool applyDamage(uint8_t dmg);
};

#endif /* CSTAT_H */