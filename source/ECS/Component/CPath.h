#ifndef CPATH_H
#define CPATH_H

#include <stdint.h>

enum {
	PATH_NONE,
	PATH_WANDER,
	PATH_FOLLOW,
	PATH_MIMIC,
	PATH_HLINE,
	PATH_VLINE
};

class GameEntity;
class Level;
struct PathComponent {
	uint8_t type = 0;
	void update(GameEntity& ent, const Level& lv);

  private:
	uint8_t helper = 0;
};

#endif /* CPATH_H */