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
			case 0: if (sv.isValid()) game.setState(new SMainGame(game, false)); break; // continue
			case 1: char name[sizeof(SaveProfile)];
					if (!Input::getString(name, messageGet("msg_chrname"), "", sizeof(name))) break;
					sv.setUserName(name);
					commitCurrentSavedata(WorldData());
					commitGlobalSavedata();
					game.setState(new SMainGame(game, true)); break; // new game
			case 2: game.end(); break; // quit
		}
	}
}

void STitleScreen::drawTop(RenderEngine& render) {
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
	//render.drawTexture(TEX_LOGO, 64, 48); // TODO: draw logo with less depth
	render.screenTranslate(render.get3D(), 0);
	for (int i = 0; i < OPT_COUNT; ++i) {
		render.drawText(FNT_SQUARE, 200, 160+i*20, 1.0f, (selected==i) ? RGBA8(sblend,0,0,0xFF) : C_BLACK, true, messageGet(MSG_MENU_IDX + i));
	}
	render.screenTranslate(-render.get3D(), 0);
	render.drawText(FNT_DEFAULT, 0, 240-16, 0.5f, C_BLACK, false, COPY_INFO);
}
/*
static void drawContinueScreen(RenderEngine& render, const SaveData& sv) {
	if (!sv.isValid()) {
		render.drawText(FNT_DEFAULT, 0, 0, 0.6f, C_WHITE, false, MSG_CONTINUE_NOSAVE);
	} else {
		render.drawTextFormat(FNT_DEFAULT, 0, 0, 0.6f, C_WHITE, false, messageGet(MSG_CONTINUE_DESC), sv.getUserName());
	}
}

static void drawNewGameScreen(RenderEngine& render) {
	render.drawText(FNT_DEFAULT, 0, 0, 0.6f, C_WHITE, false, MSG_NEWGAME_DESC);
}

static void drawGameQuitScreen(RenderEngine& render) {
	render.drawText(FNT_DEFAULT, 0, 0, 0.6f, C_WHITE, false, MSG_QUITGAME_DESC);
}
*/
void STitleScreen::drawBottom(RenderEngine& render) {
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
