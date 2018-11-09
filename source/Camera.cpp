#include "Camera.h"
#include "defs.h"

Camera::Camera(): x(0), y(0), xorig(SUBSCREEN_WIDTH/2), yorig(SCREEN_HEIGHT/2), locked(false) {}
Camera::~Camera() {}

void Camera::update(int px, int py) {
	int limx = lim.right - SUBSCREEN_WIDTH;
	int limy = lim.bot - SCREEN_HEIGHT;
	if (xspd || yspd) {
		x += xspd; y += yspd;
		if (x >= lim.left && x <= limx && y >= lim.top && y <= limy) {
			xspd = yspd = 0;
		}
	} else if (!locked) {
		int newx = px - xorig;
		int newy = py - yorig;
		x = clamp(newx, lim.left, limx);
		y = clamp(newy, lim.top, limy);
	}
}
