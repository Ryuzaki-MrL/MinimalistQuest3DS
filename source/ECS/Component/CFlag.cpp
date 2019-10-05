#include "CFlag.h"
#include "Level.h"

void FlagComponent::mark(Level& lv) {
	lv.getWorldData().uflags.set(id);
}

bool FlagComponent::isMarked(const Level& lv) {
	return lv.getWorldData().uflags.test(id);
}
