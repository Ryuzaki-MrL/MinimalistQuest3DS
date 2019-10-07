#include <cstdlib>
#include <ctime>
#include <3ds.h>

#include "GameState.h"
#include "Input.h"
#include "Message.h"
#include "EntityData.h"
#include "Script.h"
#include "Fading.h"

Game::Game(): framecnt(0), lasttime(osGetTime()), state(nullptr), running(true), transition(false) {
	std::srand(std::time(0));

	messageInit("romfs:/messages.rzdb");
	entitiesLoad("romfs:/entities.rzdb");
	scriptsLoad("romfs:/scripts.rzdb");
}

Game::~Game() {
	delete state;
}

void Game::update() {
	if (transition) {
		switch(fadeStatus()) {
			case FADE_HALFDONE:
				if (state != tmpstate) setState(tmpstate);
				break;
			case FADE_DONE:
				transition = false;
				break;
		}
	} else {
		Input::update();
		state->update();
	}
}

void Game::setState(GameState* st) {
	delete state;
	state = st;
}

void Game::setStateWithFade(GameState* st, uint16_t steps) {
	transition = true;
	tmpstate = st;
	fadeStart(steps, C_BLACK);
}

void Game::draw() {
	render.frameStart();

	fadeUpdate(render);
	render.setTargetScreen(GFX_TOP);
	state->drawTop(render);
	if (render.get3D() != 0.0) {
		render.setTargetScreen(GFX_TOP, GFX_RIGHT);
		state->drawTop(render);
	}

	render.setTargetScreen(GFX_BOTTOM);
	state->drawBottom(render);

	render.frameEnd();

	// calculate fps (debug)
	++framecnt;
	u64 delta = osGetTime() - lasttime;
	if (delta > 1000) {
		fps = framecnt / (delta / 1000.0f);
		framecnt = 0;
		lasttime = osGetTime();
	}
}

bool Game::isRunning() const {
	return (running && state);
}

void Game::end() {
	running = false;
}
