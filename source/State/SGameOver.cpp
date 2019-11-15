#include <cstdio>

#include "STitleScreen.h"
#include "SMainGame.h"
#include "SGameOver.h"
#include "Input.h"
#include "Message.h"

#define FONT_HEIGHT 10

static char s_saved[32];
static char s_endmsg[128];
static uint8_t s_lines;

SGameOver::SGameOver(Game& game, bool endgame, uint8_t friends): GameState(game) {
	if (endgame) {
		sprintf(s_saved, messageGet("msg_results"), friends);
		uint8_t frdmsg = messageGetIndex("msg_frd0");
		sprintf(
			s_endmsg, "%s\n\n%s\n\n%s\n\n%s\n",
			messageGet("msg_gameover"),
			s_saved,
			messageGet(frdmsg + friends),
			messageGet("msg_choice")
		);
	} else {
		sprintf(
			s_endmsg, "%s\n\n%s\n\n\n\n%s\n",
			messageGet("msg_gameover"),
			messageGet("msg_dead"),
			messageGet("msg_choice")
		);
	}
	s_lines = 1;
	for (int i = 0; s_endmsg[i]; ++i) {
		if (s_endmsg[i] == '\n') ++s_lines;
	}
}

SGameOver::~SGameOver() {}

void SGameOver::update() {
	if (Input::isKeyDown(KEY_START)) {
		game.setState(new SMainGame(game));
	} else if (Input::isKeyDown(KEY_SELECT)) {
		game.setState(new STitleScreen(game));
	}
}

void SGameOver::drawTop(Renderer& render) {
	render.screenClear(C_BLACK);
	render.drawText(FNT_SQUARE, 200, 120 - (s_lines*FONT_HEIGHT)/2, 1.0f, C_WHITE, ALIGN_CENTER, s_endmsg);
}

void SGameOver::drawBottom(Renderer& render) {
	render.screenClear(C_BLACK);
}
