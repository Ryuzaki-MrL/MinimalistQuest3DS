#include "CStats.h"
#include "EntityData.h"
#include "defs.h"

void StatComponent::validate() {
	const BaseStatData& base = getBaseStatData(baseidx);
	//exp = clamp(exp, 0, expfloor(lvl+1)-1);
	atk = base.baseatk; //+ calcstat(base.growatk, lvl);
	def = base.basedef; //+ calcstat(base.growdef, lvl);
	spd = base.basespd; //+ calcstat(base.growspd, lvl);
	uint8_t oldmhp = mhp;
	mhp = base.basehp; //(base.basehp + calcstat(base.growhp, lvl));
	chp = clamp(chp + (mhp - oldmhp), 0, mhp);
	rad = base.baserad; //+ calcstat(base.growrad, lvl);
}

bool StatComponent::applyDamage(uint8_t dmg) {
	if (this->imm) return false;
	chp = (dmg > chp) ? 0 : (chp - dmg);
	this->imm = getBaseStatData(baseidx).baseimm;
	return true;
}
