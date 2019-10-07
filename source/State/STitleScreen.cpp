#include "STitleScreen.h"
#include "SMainGame.h"
#include "Input.h"
#include "SaveData.h"
#include "Message.h"
#include "defs.h"

#define COPY_INFO	"©2018 Lazaro Vieira"
#define OPT_COUNT	3

static uint8_t MSG_MENU_IDX;

STitleScreen::STitleScreen(Game& game): GameState(game), selected(0) {
	MSG_MENU_IDX = messageGetIndex("msg_menu00");
	loadGlobalSavedata();
}
STitleScreen::~STitleScreen() {}

void STitleScreen::update() {
	if (Input::isKeyDown(KEY_DOWN)) {
		selected = (selected + 1) % OPT_COUNT;
	} else if (Input::isKeyDown(KEY_UP)) {
		selected = (selected - 1 + OPT_COUNT) % OPT_COUNT;
	}

	if (Input::isKeyDown(KEY_A)) {
		SaveProfile& sv = getGlobalSavedata().getProfile(0);
		switch(selected) {
			case 0: // continue
				if (sv.isValid()) game.setStateWithFade(new SMainGame(game, false), 40);
				break;

			case 1: // new game
				char name[sizeof(SaveProfile)];
				if (!Input::getString(name, messageGet("msg_chrname"), "", sizeof(name))) break;
				sv.setUserName(name);
				//commitCurrentSavedata(WorldData());
				//commitGlobalSavedata();
				game.setStateWithFade(new SMainGame(game, true), 60);
				break;

			case 2: // quit
				game.end();
				break;
		}
	}
}

void STitleScreen::drawTop(Renderer& render) {
	static uint8_t sblend = 0;
	static bool revblend = false;
	const uint8_t incb = 10 / (1 + (render.get3D() != 0.0));
	if (revblend) {
		sblend -= incb;
		if (sblend <= 50) revblend = false;
	} else {
		sblend += incb;
		if (sblend >= 250) revblend = true;
	}
	render.screenClear(C_WHITE);
	//render.screenTranslate(render.get3D(), 0);
	//render.drawTexture(TEX_LOGO, 64, 48); // TODO: draw logo
	render.screenTranslate(render.get3D(), 0);
	for (int i = 0; i < OPT_COUNT; ++i) {
		render.drawText(FNT_SQUARE, 200, 160+i*20, 1.0f, (selected==i) ? RGBA8(sblend,0,0,0xFF) : C_BLACK, ALIGN_CENTER, messageGet(MSG_MENU_IDX + i));
	}
	render.screenTranslate(-render.get3D(), 0);
	render.drawText(FNT_DEFAULT, 0, 240-16, 0.5f, C_BLACK, ALIGN_LEFT, COPY_INFO);
}
/*
static void drawContinueScreen(Renderer& render, const SaveData& sv) {
	if (!sv.isValid()) {
		render.drawText(FNT_DEFAULT, 0, 0, 0.6f, C_WHITE, false, MSG_CONTINUE_NOSAVE);
	} else {
		render.drawTextFormat(FNT_DEFAULT, 0, 0, 0.6f, C_WHITE, false, messageGet(MSG_CONTINUE_DESC), sv.getUserName());
	}
}

static void drawNewGameScreen(Renderer& render) {
	render.drawText(FNT_DEFAULT, 0, 0, 0.6f, C_WHITE, false, MSG_NEWGAME_DESC);
}

static void drawGameQuitScreen(Renderer& render) {
	render.drawText(FNT_DEFAULT, 0, 0, 0.6f, C_WHITE, false, MSG_QUITGAME_DESC);
}
*/
void STitleScreen::drawBottom(Renderer& render) {
	// TODO: bottom screen artwork for each option
	render.screenClear(C_WHITE);
	/*
	switch(selected) {
		case 0: drawContinueScreen(render); break;
		case 1: drawNewGameScreen(render); break;
		case 2: drawGameQuitScreen(render); break;
	}
	render.drawTextureColor(Texture(TEX_CONTINUE + selected), 32, 64, RGBA8(0,0,0,a[selected]), 0.0f);
	*/
}
