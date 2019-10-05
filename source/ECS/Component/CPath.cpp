#include <stdlib.h>

#include "CPath.h"
#include "GameEntity.h"
#include "Level.h"
#include "Input.h"
#include "defs.h"

void PathComponent::update(GameEntity& ent, const Level& lv) {
	MovementComponent& mv = ent.mv;
	const StatComponent& st = ent.getStats();
	const Rectangle& lim = lv.getCamera().getLimit();
	switch(type) {
		case PATH_NONE: { // no path
			mv.speed = 0;
			break;
		}

		case PATH_FOLLOW: { // follow + random
			int px = lv.getPlayer().getX()+8, py = lv.getPlayer().getY()+8;
			int ex = ent.getX()+8, ey = ent.getY()+8;
			int minx = std::min(px, ex), miny = std::min(py, ey);
			int maxx = std::max(px, ex), maxy = std::max(py, ey);
			int pd = pointDistance(minx, miny, maxx, maxy);
			if (
				(pd <= st.rad*st.rad) && (pd > 32*32) &&
				//(!lv.checkSolid(Rectangle(minx, maxx, miny, maxy), ent.getUID())) &&
				(minx > lim.left+32 && maxx < lim.right-32 && miny > lim.top+32 && maxy < lim.bot+32)
			) {
				mv.speed = st.spd;
				mv.direction = pointDirection(lv.getPlayer().getX(), lv.getPlayer().getY(), ent.getX(), ent.getY());
				break;
			}
		}
		// fall through
		case PATH_WANDER: { // random 4-dir movement
			mv.speed = st.spd;
			mv.direction = ((mv.direction / 90) * 90) % 360;

			bool canmove = false;
			switch(mv.direction) {
				case   0: canmove = (lim.right - ent.getX()) >= 32; break;
				case  90: canmove = (ent.getY() - lim.top)   >= 32; break;
				case 180: canmove = (ent.getX() - lim.left)  >= 32; break;
				case 270: canmove = (lim.bot - ent.getY())   >= 32; break;
				default: break;
			}

			++helper;
			if (((helper >= 60) && ((rand() % 100) < 5)) || (!canmove)) {
				mv.direction += 90 * (1 + (rand()%2));
				helper = 0;
			}
			break;
		}

		case PATH_MIMIC: { // mimic input (reverse)
			if (Input::isKeyHeld(KEY_LEFT))  ent.move(st.spd,0); else
			if (Input::isKeyHeld(KEY_RIGHT)) ent.move(-st.spd,0);
			if (Input::isKeyHeld(KEY_UP)) 	  ent.move(0,st.spd); else
			if (Input::isKeyHeld(KEY_DOWN))  ent.move(0,-st.spd);
			break;
		}

		case PATH_HLINE: case PATH_VLINE: { // straight line
			mv.speed = st.spd;
			mv.direction = (type == PATH_HLINE) ? 0 : 90;
			break;
		}
	}
}
