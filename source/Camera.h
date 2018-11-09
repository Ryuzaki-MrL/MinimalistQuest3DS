#ifndef CAMERA_H
#define CAMERA_H

#include "types.h"

class Camera {
  private:
	Rectangle lim;
	int x, y, xspd, yspd;
	int xorig, yorig;
	bool locked;

  public:
	Camera();
	~Camera();

	inline void setOrigin(int x, int y) { this->xorig = x; this->yorig = y; }
	inline void setPosition(int x, int y) { this->x = x; this->y = y; }
	inline void setLimit(const Rectangle& lim) { this->lim = lim; }
	inline void setSpeed(int xspd, int yspd) { this->xspd = xspd; this->yspd = yspd; }
	inline void translate(int x, int y) { this->x += x; this->y += y; }
	inline const Rectangle& getLimit() const { return lim; }
	inline bool isScrolling() const { return xspd || yspd; }
	inline int getX() const { return x; }
	inline int getY() const { return y; }
	inline void setLock(bool lock) { locked = lock; }

	void update(int px, int py);
};

#endif /* CAMERA_H */