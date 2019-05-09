#include "OPlayer.h"
#include "Level.h"
#include "Input.h"
#include "Textbox.h"

static EntityType s_pweapon = OBJ_NONE;

bool isPlayerAttacking() {
	return s_pweapon != OBJ_NONE;
}

void signalAttackEnd() {
	s_pweapon = OBJ_NONE;
}

OPlayer::OPlayer(Level& level, EntityType type): GameEntity(level, type) {
	const PlayerData& pd = level.getWorldData().player;
	curstats.setLevel(pd.lvl);
	curstats.addExp(pd.exp, *this);
	curstats.chp = pd.chp;
	s_pweapon = OBJ_NONE;
}
OPlayer::~OPlayer() {}

void OPlayer::onKill() {
	level.setFlag(LFLAG_GAMEOVER);
}

void OPlayer::onMove() {
	// change section when out of bounds
	const Rectangle& sec = level.getCamera().getLimit();
	if (pos.x+8 < 0) level.setFlag(LFLAG_ENDGAME);
	else if (pos.x+8 < sec.left)  level.setSection(pos.x - 32, pos.y, -10,  0);
	else if (pos.x+8 > sec.right) level.setSection(pos.x + 32, pos.y, +10,  0);
	else if (pos.y+8 < sec.top)   level.setSection(pos.x, pos.y - 32,   0, -8);
	else if (pos.y+8 > sec.bot)   level.setSection(pos.x, pos.y + 32,   0, +8);
}

void OPlayer::onTimeOut() {
	curstats.attacking = false;
}

void OPlayer::onDamage(uint8_t dmg, GameEntity&) {
	uint8_t& chp = level.getWorldData().player.chp;
	chp = (dmg > chp) ? 0 : (chp - dmg);
	spr.color.c.a = 0x9F;
	if (chp == 0) kill();
}

void OPlayer::onUpdate() {
	InventoryData& inv = level.getWorldData().inv;
	if ((!isPlayerAttacking()) || ((s_pweapon == OBJ_BOOMERANG) && (!curstats.attacking))) {
		if (Input::isKeyHeld(KEY_RIGHT)) {
			move(curstats.spd, 0); mv.direction = 0;
		} else if (Input::isKeyHeld(KEY_LEFT)) {
			move(-curstats.spd, 0); mv.direction = 180;
		}
		if (Input::isKeyHeld(KEY_DOWN)) {
			move(0, curstats.spd); mv.direction = ((mv.direction%180) == 0) ? ((270 + 360 - mv.direction) >> 1) : 270;
		} else if (Input::isKeyHeld(KEY_UP)) {
			move(0, -curstats.spd); mv.direction = ((mv.direction%180) == 0) ? ((90 + mv.direction) >> 1) : 90;
		}
		if (Input::isKeyDown(KEY_A)) {
			interact();
		} else if (Input::isKeyDown(KEY_B) && !isPlayerAttacking()) {
			if (inv.curweapon && inv.weaponlvl[inv.curweapon]) {
				GameEntity* wp = level.instanceCreate(getX(), getY(), EntityType(OBJ_SWORD + inv.curweapon - 1));
				if (wp) {
					wp->getMovement().direction = mv.direction;
					s_pweapon = wp->getType();
				}
				curstats.attacking = true;
				tmc.limit = 20;
			}
		}
	}

	// Change current item and weapon (TODO: move this to SMainGame)
	if (Input::isKeyDown(KEY_L | KEY_ZL)) {
		do {
			inv.curweapon = (inv.curweapon + (Input::isKeyDown(KEY_L) ? 1 : (WEAPON_COUNT-1))) % WEAPON_COUNT;
		} while(!inv.weaponlvl[inv.curweapon] && inv.curweapon != 0);
	}
	if (Input::isKeyDown(KEY_R | KEY_ZR)) {
		do {
			inv.curitem = (inv.curitem + (Input::isKeyDown(KEY_R) ? 1 : (ITEM_COUNT-1))) % ITEM_COUNT;
		} while(!inv.countItem(inv.curitem) && inv.curitem != 0);
	}

	if (Input::isKeyDown(KEY_X)) {
		int objtype; Input::getInteger(objtype, "Object ID");
		level.instanceCreate(pos.x+32, pos.y, EntityType(objtype)); // debug
	}
	if (Input::isKeyDown(KEY_Y)) {
		level.instanceCreate(pos.x+32, pos.y, EntityType(OBJ_SKELETON + (rand() % 5))); // debug
	}

	level.collisionHandle(getBoundingBox(), uid, [this](GameEntity& other) {
		if (other.hasProperty(PROPERTY_PICKABLE)) other.kill();
		if (other.hasProperty(PROPERTY_ENEMY)) me.getStats().damage(me, other);
	});

	// slip out of solids
	if (level.checkSolid(getBoundingBox(), uid)) {
		(move(16,0))||(move(-16,0))||(move(0,16))||(move(0,-16));
	}
}
