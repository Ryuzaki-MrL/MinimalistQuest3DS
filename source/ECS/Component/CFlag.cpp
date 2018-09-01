#include "CFlag.h"
#include "Level.h"

void FlagComponent::mark(Level& lv) {
	lv.getWorldData().flagSet(id);
}

bool FlagComponent::isMarked(const Level& lv) {
	return lv.getWorldData().flagCheck(id);
}
