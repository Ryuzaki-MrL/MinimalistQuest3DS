#include <3ds.h>

#include "Game.h"
#include "STitleScreen.h"

int main() {
	romfsInit();

	Game game;
	game.setState(new STitleScreen(game));

	while(aptMainLoop() && game.isRunning()) {
		game.update();
		game.draw();
	}

	romfsExit();
	return 0;
}
