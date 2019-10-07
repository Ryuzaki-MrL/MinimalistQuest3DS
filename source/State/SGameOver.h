#ifndef GAMEOVER_H
#define GAMEOVER_H

#include "GameState.h"
#include "Level.h"

class SGameOver: public GameState {
  public:
	SGameOver(Game&, bool endgame, uint8_t friends);
	~SGameOver();

	void update();
	void drawTop(Renderer&);
	void drawBottom(Renderer&);
};

#endif /* GAMEOVER_H */