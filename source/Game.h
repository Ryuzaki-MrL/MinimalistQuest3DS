#ifndef GAME_H
#define GAME_H

#include "Graphics.h"

class GameState;
class Game {
  private:
	Renderer render;
	uint64_t framecnt, lasttime;
	float fps;
	GameState *state, *tmpstate;
	bool running, transition;

  public:
	Game();
	~Game();

	bool isRunning() const;
	float getFPS() const { return fps; }
	void setState(GameState*);
	void setStateWithFade(GameState*, uint16_t);
	void update();
	void draw();
	void end();
};

#endif /* GAME_H */
