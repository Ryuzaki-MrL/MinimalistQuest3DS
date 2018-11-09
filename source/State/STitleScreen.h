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
	void drawTop(Renderer& render);
	void drawBottom(Renderer& render);
};

#endif /* TITLESCREEN_H */