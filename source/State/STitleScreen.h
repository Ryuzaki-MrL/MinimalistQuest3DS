#ifndef TITLESCREEN_H
#define TITLESCREEN_H

#include "GameState.h"

class STitleScreen: public GameState {
  private:
	uint8_t selected;

  public:
	STitleScreen(Game& game);
	~STitleScreen();

	void update();
	void drawTop(RenderEngine& render);
	void drawBottom(RenderEngine& render);
};

#endif /* TITLESCREEN_H */