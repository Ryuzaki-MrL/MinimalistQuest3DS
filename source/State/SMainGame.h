#ifndef MAINGAME_H
#define MAINGAME_H

#include "GameState.h"
#include "Level.h"

class SMainGame: public GameState {
  private:
	Level level;
	uint8_t screen;
	bool paused;

  public:
	SMainGame(Game& game, bool newgame = false);
	~SMainGame();

	void update();
	void drawTop(RenderEngine& render);
	void drawBottom(RenderEngine& render);
};

#endif /* MAINGAME_H */