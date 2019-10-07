#ifndef TITLESCREEN_H
#define TITLESCREEN_H

#include "GameState.h"

class STitleScreen: public GameState {
  private:
	uint8_t selected;

  public:
	STitleScreen(Game&);
	~STitleScreen();

	void update();
	void drawTop(Renderer&);
	void drawBottom(Renderer&);
};

#endif /* TITLESCREEN_H */