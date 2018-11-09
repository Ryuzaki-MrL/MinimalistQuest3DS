#include <stdlib.h>

#include "CPath.h"
#include "GameEntity.h"
#include "Level.h"
#include "Input.h"
#include "defs.h"

void PathComponent::update(GameEntity& ent, const Level& lv) {
	MovementComponent& mv = ent.getMovement();
	const StatComponent& st = ent.getStats();
	switch(type) {
		case PATH_NONE: { // no path
			break;
		}
		case PATH_FOLLOW: { // follow + random
			int px = lv.getPlayer().getX(), py = lv.getPlayer().getY();
			int ex = ent.getX(), ey = ent.getY();
			int minx = std::min(px, ex), miny = std::min(py, ey);
			int maxx = std::max(px, ex), maxy = std::max(py, ey);
			int pd = pointDistance(minx, miny, maxx, maxy);
			const Rectangle& lim = lv.getCamera().getLimit();
			if (
				(pd <= st.rad*st.rad) && (pd > 16) &&
				//(!lv.checkSolid(Rectangle(minx, maxx, miny, maxy), ent.getUID())) &&
				(minx > lim.left+24 && maxx < lim.right-24 && miny > lim.top+24 && maxy < lim.bot+24)
			) {
				mv.speed = st.spd;
				mv.direction = pointDirection(lv.getPlayer().getX(), lv.getPlayer().getY(), ent.getX(), ent.getY());
				break;
			}
		}
		// fall through
		case PATH_WANDER: { // random 4-dir movement
			mv.speed = st.spd;
			mv.direction = (mv.direction / 90) * 90;
			++helper;
			if ((helper >= 60) && ((rand() % 100) < 5)) {
				mv.direction += 90 * (1 + (rand()&3));
				helper = 0;
			}
			break;
		}
		case PATH_MIMIC: { // mimic input (reverse)
			if (Input::isKeyHeld(KEY_LEFT)) ent.move(st.spd,0);
			else if (Input::isKeyHeld(KEY_RIGHT)) ent.move(-st.spd,0);
			if (Input::isKeyHeld(KEY_UP)) ent.move(0,st.spd);
			else if (Input::isKeyHeld(KEY_DOWN)) ent.move(0,-st.spd);
			break;
		}
		case PATH_HLINE: case PATH_VLINE: { // straight line
			if (!mv.speed) {
				mv.speed = st.spd;
				mv.direction = (type == PATH_HLINE) ? 0 : 90;
			}
			break;
		}
	}
}
