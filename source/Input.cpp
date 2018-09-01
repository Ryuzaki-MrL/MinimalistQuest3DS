#include <stdio.h>
#include <stdlib.h>
#include <3ds.h>

#include "Input.h"

namespace Input {

static u32 kDown;
static u32 kHeld;
static u32 kUp;
static touchPosition touch;
static circlePosition circle;
static accelVector accel;
static angularRate gyro;

void update() {
	hidScanInput();

	kDown = hidKeysDown();
	kHeld = hidKeysHeld();
	kUp = hidKeysUp();

	hidCircleRead(&circle);
	hidTouchRead(&touch);
	hidAccelRead(&accel);
	hidGyroRead(&gyro);
}

bool isKeyDown(u32 key) {
	return (kDown & key);
}

bool isKeyHeld(u32 key) {
	return (kHeld & key);
}

bool isKeyUp(u32 key) {
	return (kUp & key);
}

u16 getTouchX() {
	return touch.px;
}

u16 getTouchY() {
	return touch.py;
}

s16 getCirclePadX() {
	return circle.dx;
}

s16 getCirclePadY() {
	return circle.dy;
}

s16 getAccelX() {
	return accel.x;
}

s16 getAccelY() {
	return accel.y;
}

s16 getAccelZ() {
	return accel.z;
}

void setAccelState(bool state) {
	if (state) {
		HIDUSER_EnableAccelerometer();
	} else {
		HIDUSER_DisableAccelerometer();
	}
}

s16 getGyroX() {
	return gyro.x;
}

s16 getGyroY() {
	return gyro.y;
}

s16 getGyroZ() {
	return gyro.z;
}

void setGyroState(bool state) {
	if (state) {
		HIDUSER_EnableGyroscope();
	} else {
		HIDUSER_DisableGyroscope();
	}
}

bool getString(char* out, const char* htext, const char* def, int maxlength, bool password) {
	SwkbdState kb;

	swkbdInit(&kb, SWKBD_TYPE_QWERTY, 2, maxlength);
	swkbdSetInitialText(&kb, def);
	swkbdSetHintText(&kb, htext);
	swkbdSetButton(&kb, SWKBD_BUTTON_LEFT, "Cancel", false);
	swkbdSetButton(&kb, SWKBD_BUTTON_RIGHT, "Confirm", true);
	swkbdSetValidation(&kb, SWKBD_NOTEMPTY_NOTBLANK, 0, 0);
	if (password) swkbdSetPasswordMode(&kb, SWKBD_PASSWORD_HIDE);

	swkbdInputText(&kb, out, maxlength);

	SwkbdResult result = swkbdGetResult(&kb);
	return (result==SWKBD_D1_CLICK1);
}

bool getInteger(int& out, const char* htext, int def) {
	static char buffer[11] = "";
	SwkbdState kb;

	swkbdInit(&kb, SWKBD_TYPE_NUMPAD, 2, sizeof(buffer) - 1);
	snprintf(buffer, sizeof(buffer)-1, "%d", def);
	swkbdSetInitialText(&kb, buffer);
	swkbdSetHintText(&kb, htext);
	swkbdSetButton(&kb, SWKBD_BUTTON_LEFT, "Cancel", false); // TODO: get button name based on language
	swkbdSetButton(&kb, SWKBD_BUTTON_RIGHT, "Confirm", true);
	swkbdSetFeatures(&kb, SWKBD_FIXED_WIDTH);
	swkbdSetValidation(&kb, SWKBD_NOTEMPTY_NOTBLANK, 0, 0);

	swkbdInputText(&kb, buffer, sizeof(buffer));

	SwkbdResult result = swkbdGetResult(&kb);
	out = atoi(buffer);
	return (result==SWKBD_D1_CLICK1);
}

}; // namespace Input