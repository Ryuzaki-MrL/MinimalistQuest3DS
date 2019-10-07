#include "Fading.h"
#include "types.h"

struct FadeCtx {
	Color color;
	uint16_t steps, counter;
	bool halfdone = false;
	bool done = true;
};
static FadeCtx ctx;

void fadeStart(uint16_t steps, uint32_t color) {
	ctx.color.rgba = color;
	ctx.steps = steps;
	ctx.counter = 0;
	ctx.done = ctx.halfdone = false;
}

void fadeUpdate(Renderer& render) {
	if (ctx.done) return;
	if (ctx.halfdone) {
		--ctx.counter;
		if (ctx.counter == 0) ctx.done = true;
	} else {
		++ctx.counter;
		if (ctx.counter >= ctx.steps) ctx.halfdone = true;
	}
	ctx.color.c.a = (ctx.counter * 255) / ctx.steps;
	render.screenBlend(ctx.color.rgba);
}

int fadeStatus() {
	if (ctx.done) return FADE_DONE;
	if (ctx.halfdone) return FADE_HALFDONE;
	//if (ctx.counter >= ctx.steps) return FADE_HALFDONE;
	return FADE_RUNNING;
}
