#include <cstdlib>
#include <ctime>
#include <3ds.h>

#include "Game.h"
#include "STitleScreen.h"
#include "Input.h"
#include "Message.h"
#include "EntityData.h"

Game::Game(): state(nullptr), running(true), framecnt(0), lasttime(osGetTime()) {
	std::srand(std::time(0));
	messageInit("romfs:/messages.rzdb");
	entitiesLoad("romfs:/entities.rzdb");

	setState(new STitleScreen(*this));
}

Game::~Game() {
	delete state;
}

// TODO: apply delta time on update
void Game::update() {
	Input::update();
	state->update();
}

void Game::setState(GameState* newstate) {
	delete state;
	state = newstate;
}

void Game::draw() {
	render.frameStart();

	render.renderTargetScreen(GFX_TOP);
	state->drawTop(render);
	if (render.get3D() != 0.0) {
		render.renderTargetScreen(GFX_TOP, GFX_RIGHT);
		state->drawTop(render);
	}

	render.renderTargetScreen(GFX_BOTTOM);
	state->drawBottom(render);

	render.frameEnd();

	// calculate fps
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
