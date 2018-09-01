#ifndef INPUT_H
#define INPUT_H

#include <3ds.h>

#define KEY_ANY 0xFFFFFFFF

// TODO: key bindings by passing a 'InputConfig' struct or manually
// adding/removing bindings

namespace Input {
	void update();

	bool isKeyDown(u32 key);
	bool isKeyHeld(u32 key);
	bool isKeyUp(u32 key);

	u16 getTouchX();
	u16 getTouchY();

	s16 getCirclePadX();
	s16 getCirclePadY();

	s16 getAccelX();
	s16 getAccelY();
	s16 getAccelZ();
	void setAccelState(bool state);

	s16 getGyroX();
	s16 getGyroY();
	s16 getGyroZ();
	void setGyroState(bool state);

	bool getString(char* out, const char* htext, const char* def, int maxlength, bool password = false);
	bool getInteger(int& out, const char* htext, int def = 0);
};

#endif /* INPUT_H */