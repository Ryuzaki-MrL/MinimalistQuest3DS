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
	virtual void drawTop(Renderer&) {}
	virtual void drawBottom(Renderer&) {}
};

#endif /* GAMESTATE_H */