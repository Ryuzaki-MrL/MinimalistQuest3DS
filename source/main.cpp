#include <3ds.h>

#include "Game.h"

int main() {
	romfsInit();
	Game game;

	while(aptMainLoop() && game.isRunning()) {
		game.update();
		game.draw();
	}

	romfsExit();
	return 0;
}
