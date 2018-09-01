#ifndef GAME_H
#define GAME_H

#include "Graphics.h"

class GameState;
class Game {
  private:
	RenderEngine render;
	GameState* state;
	bool running;
	u64 framecnt, lasttime;
	float fps;

  public:
	Game();
	~Game();

	bool isRunning() const;
	float getFPS() const { return fps; }
	void setState(GameState* newstate);
	void update();
	void draw();
	void end();
};

#endif /* GAME_H */
