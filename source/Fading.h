#ifndef FADING_H
#define FADING_H

#include <stdint.h>

#include "Graphics.h"

enum { FADE_RUNNING, FADE_HALFDONE, FADE_DONE };

void fadeStart(uint16_t steps, uint32_t color);
void fadeUpdate(Renderer&);
int fadeStatus();

#endif /* FADING_H */