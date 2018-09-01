#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Game.h"

class GameState {
  protected:
	Game& game;

  public:
	GameState(Game& game): game(game) {}
	virtual ~GameState() {}

	virtual void update() = 0;
	virtual void drawTop(RenderEngine&) {}
	virtual void drawBottom(RenderEngine&) {}
};

#endif /* GAMESTATE_H */