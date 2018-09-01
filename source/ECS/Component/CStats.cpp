#include "CStats.h"
#include "GameEntity.h"

// TODO: rewrite stats system (or scrap it for the time being)

static uint16_t expfloor(uint8_t lvl) {
	//return ((16 - lvl)*lvl + (lvl * lvl));
	return (4*lvl*lvl + 8*(lvl-1));
}

static uint8_t calcstat(uint8_t base, uint8_t lvl) {
	return (base * lvl) / 100;
}

void StatComponent::addExp(uint16_t amt, GameEntity& me) {
	exp += amt;
	while(exp >= expfloor(lvl+1)) {
		exp -= expfloor(++lvl);
		me.popup("msg_levelup");
	}
	validate();
}

void StatComponent::setLevel(uint8_t lvl) {	
	this->lvl = clamp(lvl, 1, 100);
	validate();
}

void StatComponent::validate() {
	const BaseStatData& base = getBaseStatData(baseidx);
	exp = clamp(exp, 0, expfloor(lvl+1)-1);
	atk = base.baseatk + calcstat(base.growatk, lvl);
	def = base.basedef + calcstat(base.growdef, lvl);
	spd = base.basespd + calcstat(base.growspd, lvl);
	uint8_t oldmhp = mhp;
	mhp = (base.basehp + calcstat(base.growhp, lvl));
	chp = clamp(chp + (mhp - oldmhp), 0, mhp);
	rad = base.baserad + calcstat(base.growrad, lvl);
}

void StatComponent::damage(GameEntity& me, GameEntity& other) {
	if (!imm) {
		uint8_t dmg = clamp(clamp(other.getStats().atk, 1, 255) - def, 0, 255);
		chp = (dmg > chp) ? 0 : (chp - dmg);
		imm = me.getBaseStats().baseimm;
		me.onDamage(dmg, other);
	}
}
