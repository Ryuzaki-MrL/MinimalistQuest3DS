#include <cstdio>

#include "STitleScreen.h"
#include "SMainGame.h"
#include "SGameOver.h"
#include "Input.h"
#include "SaveData.h"
#include "Textbox.h"
#include "Message.h"
#include "Sprite.h"

#define INV_XSEP 64

static uint8_t MSG_MENU_IDX, MSG_WEAPON, MSG_ITEM, MSG_PAUSE;

SMainGame::SMainGame(Game& game, bool newgame): GameState(game), screen(0), paused(false) {
	MSG_MENU_IDX = messageGetIndex("msg_tab00");
	MSG_WEAPON = messageGetIndex("msg_weapon");
	MSG_ITEM = messageGetIndex("msg_item");
	MSG_PAUSE = messageGetIndex("msg_pause");
	if (!newgame) loadCurrentSavedata(level.getWorldData());
	level.load(Level::makePath(0));
}

SMainGame::~SMainGame() {}

static inline const Rectangle getTouchArea() {
	int tx = Input::getTouchX();
	int ty = Input::getTouchY();
	return {tx, tx+1, ty, ty+1};
}

static void updateInventoryScreen(InventoryData& inv) {
	const Rectangle tbox = getTouchArea();
	for (int i = 0; i < WEAPON_COUNT; ++i) {
		if (tbox.collideWith(Rectangle(i*INV_XSEP+20, i*INV_XSEP+20+40, 120, 120+40))) {
			inv.curweapon = i; break;
		}
	}
	for (int i = 0; i < ITEM_COUNT; ++i) {
		if (tbox.collideWith(Rectangle(i*INV_XSEP+20, i*INV_XSEP+20+40, 192, 192+40))) {
			inv.curitem = i; break;
		}
	}
}

void SMainGame::update() {
	if (level.checkFlag(LFLAG_GAMEOVER)) {
		game.setStateWithFade(
			new SGameOver(game, level.checkFlag(LFLAG_ENDGAME), std::max(0, level.getWorldData().inv.countFriends()-1)),
			60 /* fade time */
		);
	}
	else if (!getTextbox().update()) {
		if (Input::isKeyDown(KEY_START)) {
			paused = !paused;
		}
		if (paused) {
			if (Input::isKeyDown(KEY_SELECT)) {
				game.setState(new STitleScreen(game));
			}
		}
		else {
			level.update();
		}
	}

	if (Input::isKeyDown(KEY_TOUCH)) {
		const Rectangle tbox = getTouchArea();
		for (uint8_t i = 0; i < 3; ++i) {
			if (tbox.collideWith(Rectangle(5+i*107, 5+96+i*107, 53, 53+32))) {
				screen = i; break;
			}
		}
		if (screen == 2) {
			updateInventoryScreen(level.getWorldData().inv);
		}
	}
}

void SMainGame::drawTop(Renderer& render) {
	level.draw(render);
	if (paused) {
		render.drawRectangle(0, 0, 400, 240, RGBA8(0,0,0,0xCF));
		render.drawText(FNT_ARIAL9, 200, 120, 1.0f, C_WHITE, ALIGN_CENTER, messageGet(MSG_PAUSE));
	}
}

static void drawMapScreen(const Level& level, Renderer& render) {
	const MapData& mp = level.getWorldData().map;
	if (!mp.hasmap) {
		render.drawRectangle(51, 89, 217, 144, C_BLACK);
		render.drawText(FNT_SQUARE, 160, 144, 1.0f, C_WHITE, ALIGN_CENTER, messageGet("msg_nomap"));
		return;
	}
	render.drawTexture(TEX_MAP, 51, 89);
	render.drawTextFormat(FNT_SQUARE, 52, 90, 1.0f, C_WHITE, ALIGN_LEFT, "%d%%", (mp.countVisited() * 100) / level.getSectionCount());
	for (size_t i = 0; i < SECTION_COUNT; ++i) {
		if (mp.isVisited(i)) {
			render.drawRectangle(51+24*(i%9), 124+18*(i/9), 24, 18, RGBA8(0xFF,0,0,0x6F));
		}
	}
	size_t sc = level.getSectionCurrent();
	render.drawSpriteExt(spriteGetTexture(SPR_PLAYER), 51+24*(sc%9)+8, 124+18*(sc/9)+6, 0, 0, 0.5f, 0.5f, 0, C_BLACK);
}

static void drawControlScreen(Renderer& render, const Level& level, const Game& game) {
	u32 memuse = 33554432 - linearSpaceFree();
	render.drawTextFormat(FNT_DEFAULT, 160, 88, 0.5f, RGBA8(0xFF,0,0,0xFF), ALIGN_CENTER,
		"MEM: %u (%.2f%%)\n"
		"FPS: %.2f\n"
		"Obj: %u / Active: %u\n",
		memuse, ((memuse / 33554432.0) * 100.0),
		game.getFPS(),
		level.instanceCount(), level.countActive()
	);
	render.drawText(FNT_DEFAULT, 160, 134, 0.6f, C_BLACK, ALIGN_CENTER, messageGet("msg_chelp"));
}

static void drawInventoryScreen(const InventoryData& inv, Renderer& render) {
	render.drawText(FNT_DEFAULT, 8, 96, 0.75f, C_BLACK, ALIGN_LEFT, messageGet(MSG_WEAPON));
	for (int i = 0; i < WEAPON_COUNT; ++i) {
		render.drawRectangle(i*INV_XSEP+20, 120, 40, 40, RGBA8(0xFF*(inv.curweapon==i),0,0,0xFF));
		render.drawRectangle(i*INV_XSEP+22, 122, 36, 36, C_WHITE);
		render.drawSprite(spriteGetTexture(SPR_WEAPONS, i), i*INV_XSEP+32, 132);
	}
	render.drawText(FNT_DEFAULT, 8, 168, 0.75f, C_BLACK, ALIGN_LEFT, messageGet(MSG_ITEM));
	for (int i = 0; i < ITEM_COUNT; ++i) {
		render.drawRectangle(i*INV_XSEP+20, 192, 40, 40, RGBA8(0xFF*(inv.curitem==i),0,0,0xFF));
		render.drawRectangle(i*INV_XSEP+22, 194, 36, 36, C_WHITE);
		render.drawSprite(spriteGetTexture(SPR_ITEMS, i), i*INV_XSEP+32, 204);
	}
}

void SMainGame::drawBottom(Renderer& render) {
	static char hpbuf[30];
	render.screenClear(C_WHITE);
	const WorldData& wd = level.getWorldData();
	for (uint8_t i = 0; i < 3; ++i) {
		render.drawRectangle(5+i*107, 53, 96, 32, RGBA8(0xFF*(screen==i),0,0,0xFF));
		render.drawText(FNT_SQUARE, 5+48+i*107, 62, 1.0f, C_WHITE, ALIGN_CENTER, messageGet(MSG_MENU_IDX + i));
	}
	switch(screen) {
		case 0: drawMapScreen(level, render); break;
		case 1: drawControlScreen(render, level, game); break;
		case 2: drawInventoryScreen(wd.inv, render); break;
	}
	if (getTextbox().draw(render)) return;
	render.drawRectangle(0, 48, 320, 1, C_BLACK);
	render.drawRectangle(144, 14, 18, 18, C_BLACK);
	render.drawRectangle(145, 15, 16, 16, C_WHITE);
	render.drawRectangle(240, 14, 18, 18, C_BLACK);
	render.drawRectangle(241, 15, 16, 16, C_WHITE);
	uint8_t chp = wd.player.chp;
	uint8_t mhp = wd.player.mhp;
	for (uint8_t i = 0; i < chp && i < mhp; ++i) hpbuf[i] = '|';
	for (uint8_t i = chp; i < mhp; ++i) hpbuf[i] = '\'';
	hpbuf[mhp] = 0;
	render.drawTextFormat(FNT_SQUARE, 0,  0, 1.0f, C_BLACK, ALIGN_LEFT, "%s\nHP: [%s]", currentSaveProfile().getUserName(), hpbuf);
	for (uint8_t i = 0; i < wd.inv.countKeys(); ++i) {
		render.drawSprite(spriteGetTexture(SPR_KEY), i<<4, 32);
	}
	for (uint8_t i = 0; i < FRIEND_COUNT; ++i) {
		if (wd.inv.hasFriend(i)) render.drawSprite(spriteGetTexture(SPR_PLAYER, i), 240 + (i<<4), 32);
	}
	render.drawText(FNT_SQUARE, 144,  0, 1.0f, C_BLACK, ALIGN_LEFT, messageGet(MSG_WEAPON));
	render.drawSprite(spriteGetTexture(SPR_WEAPONS, wd.inv.curweapon), 145, 15);
	render.drawTextFormat(FNT_SQUARE, 164, 16, 1.0f, C_BLACK, ALIGN_LEFT, "Lv.%d", wd.inv.getWeapon(wd.inv.curweapon));
	render.drawText(FNT_SQUARE, 240,  0, 1.0f, C_BLACK, ALIGN_LEFT, messageGet(MSG_ITEM));
	render.drawSprite(spriteGetTexture(SPR_ITEMS, wd.inv.curitem), 241, 15);
	render.drawTextFormat(FNT_SQUARE, 260, 16, 1.0f, C_BLACK, ALIGN_LEFT, "x%d", wd.inv.countItem(wd.inv.curitem));
}
