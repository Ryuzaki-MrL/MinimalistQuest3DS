#include <string.h>
#include <stdarg.h>

#include "Textbox.h"
#include "Graphics.h"
#include "Input.h"

static Textbox tb;

Textbox::Textbox(): buf(), buflen(0), str(""), size(1), current(0), skip(0), active(false) {}
Textbox::~Textbox() {}

void Textbox::setText(const char* str) {
	this->str = str;
	active = true;
	buf[0] = buflen = current = 0;
}

bool Textbox::update() {
	if (!active) return false;
	if (current < size && str[buflen]) {
		buf[buflen] = str[buflen];
		buflen += skip; skip ^= 1;
		buf[buflen] = 0;
		if (Input::isKeyDown(KEY_START)) {
			int i;
			for (i = 0; str[i]; ++i) buf[i] = str[i];
			buf[buflen = i] = 0;
		}
	} else {
		if (Input::isKeyDown(KEY_ANY)) {
			if (++current < size) {
				buf[buflen = 0] = 0;
			} else {
				active = false;
			}
		}
	}
	return true;
}

bool Textbox::draw(Renderer& render) {
	if (!active) return false;
	render.drawRectangle(0, 0, 320, 48, C_BLACK);
	render.drawText(FNT_SQUARE, 0, 0, 1.0f, C_WHITE, ALIGN_LEFT, buf);
	return true;
}

void textboxCreate(const char* str) {
	tb.setText(str);
}

void textboxCreateFormat(const char* str, ...) {
	static char buf[256] = "";
	va_list args;
	va_start(args, str);
	vsnprintf(buf, sizeof(buf) - 1, str, args);
	tb.setText(buf);
	va_end(args);
}

Textbox& getTextbox() {
	return tb;
}
