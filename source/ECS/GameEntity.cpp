#include "GameEntity.h"
#include "Level.h"
#include "Graphics.h"
#include "Message.h"
#include "defs.h"

#define GROUP_COUNT 256

template<int N> struct _{ operator char() { return N + 256; } };

static uint16_t s_nextuid = 0;
static uint16_t s_entcount[NUM_ENTITIES] = { 0 };
static uint16_t s_entgp[GROUP_COUNT] = { 0 };

GameEntity::GameEntity(Level& level, EntityType type):
  level(level), data(entityGetData(type)), uid(++s_nextuid), type(type), group(0), dead(false) {
	char(_<sizeof(GameEntity)>()); // report size as a compiler warning
	++s_entcount[type]; ++s_entgp[group];
	spr = SpriteComponent(Sprite(data.sprite), data.anim);
	path.type = data.path;
	curstats.baseidx = data.baseidx;
	curstats.validate();
}
GameEntity::~GameEntity() {
	--s_entcount[type];
	--s_entgp[group];
}

uint16_t GameEntity::getCount(EntityType type) {
	return s_entcount[type];
}

uint16_t GameEntity::getCountGroup(uint8_t group) {
	return s_entgp[group];
}

void GameEntity::serialize(FILE* out) const {
	fwrite(&type, 1, sizeof(EntityType), out);
	fwrite(&pos.xs, 1, sizeof(pos.xs), out);
	fwrite(&pos.ys, 1, sizeof(pos.ys), out);
	fwrite(&group, 1, sizeof(group), out);
	if (hasProperty(PROPERTY_SPRTRSAVE)) fwrite(&spr.tr, 1, sizeof(spr.tr), out);
	if (hasComponent(COMPONENT_SPRITE)) fwrite(&spr.color, 1, sizeof(spr.color) + sizeof(spr.sprite), out);
	if (hasComponent(COMPONENT_STATS)) fwrite(&curstats.lvl, 1, sizeof(curstats.lvl), out);
	if (hasComponent(COMPONENT_FLAG)) fwrite(&flg, 1, sizeof(flg), out);
	if (hasComponent(COMPONENT_LOOT)) fwrite(&loot, 1, sizeof(loot), out);
	if (hasComponent(COMPONENT_PATH)) fwrite(&path.type, 1, sizeof(path.type), out);
	if (hasComponent(COMPONENT_SCRIPT)) fwrite(&scr.scrid, 1, sizeof(scr.scrid), out);
}

void GameEntity::deserialize(FILE* in) {
	fread(&pos.xs, 1, sizeof(pos.xs), in);
	fread(&pos.ys, 1, sizeof(pos.ys), in);
	pos.reset();
	if (hasProperty(PROPERTY_SPRTRSAVE))
		fread(&spr.tr, 1, sizeof(spr.tr), in);
	if (hasProperty(PROPERTY_SAVEANIM))
		fread(&spr.steps, 1, sizeof(spr.steps), in);
	if (hasComponent(COMPONENT_SPRITE)) {
		fread(&spr.color, 1, sizeof(uint32_t), in);
		fread(&spr.sprite, 1, sizeof(Sprite), in);
	}
	spr.calcBoundingBox();
	if (hasComponent(COMPONENT_FLAG))
		fread(&flg.id, 1, sizeof(flg.id), in);
	if (hasComponent(COMPONENT_LOOT)) {
		fread(&loot.type, 1, sizeof(loot.type), in);
		fread(&loot.locked, 1, sizeof(loot.locked), in);
	}
	if (hasComponent(COMPONENT_PATH))
		fread(&path.type, 1, sizeof(path.type), in);
	if (hasComponent(COMPONENT_SCRIPT))
		fread(&scr.scrid, 1, sizeof(scr.scrid), in);
	--s_entgp[group];
	fread(&group, 1, sizeof(group), in);
	++s_entgp[group];
	onLoad();
}

void GameEntity::setGroup(uint8_t gp) {
	--s_entgp[group];
	group = gp;
	++s_entgp[group];
}

bool GameEntity::move(float x, float y) {
	if (hasProperty(PROPERTY_GHOST) || !level.checkSolid(getBoundingBox(pos.x + x, pos.y + y), uid)) {
		pos.x += x;
		pos.y += y;
		onMove();
		return true;
	}
	return false;
}

void GameEntity::kill() {
	dead = true;
	onKill();
}

void GameEntity::interact() {
	level.collisionHandle(getBoundingBox() + Rectangle(-8, 8, -8, 8), uid, [this](GameEntity& other) {
		other.onInteract(*this);
	});
}

void GameEntity::damage(EntityType except) {
	level.collisionHandle(getBoundingBox(), uid, [this, except](GameEntity& other) {
		if ((other.hasProperty(PROPERTY_DAMAGEABLE)) && (other.getType() != except)) {
			other.applyDamage(*this);
		}
	});
}

void GameEntity::applyDamage(GameEntity& damager) {
	uint8_t dmg = clamp(clamp(damager.getStats().atk, 1, 255) - curstats.def, 0, 255);
	if (curstats.applyDamage(dmg)) {
		onDamage(dmg, damager);
	}
}

void GameEntity::popup(const char* msg) {
	GameEntity* pp = level.instanceCreate(getX(), getY() - 20, OBJ_POPUP);
	if (pp) pp->loot.type = messageGetIndex(msg);
}

void GameEntity::update() {
	spr.update();
	if (tmc.update() == TIMED_OUT) onTimeOut();
	if (hasComponent(COMPONENT_MOVEMENT)) mv.update(*this);
	if (hasComponent(COMPONENT_PATH)) path.update(*this, level);
	if (hasComponent(COMPONENT_SCRIPT)) scr.update(level);
	if (curstats.imm) --curstats.imm; else
	if (hasComponent(COMPONENT_STATS)) spr.color.c.a = 0xFF; // temporary hack
	onUpdate();
}

void GameEntity::draw(Renderer& render) {
	if (hasProperty(PROPERTY_SHOWDIR)) spr.tr.angle = mv.direction;
	spr.draw(pos.x, pos.y, render);
	if ((hasProperty(PROPERTY_DAMAGEABLE)) && (curstats.chp < curstats.mhp)) {
		int8_t sx = abs(spr.tr.xscale);
		render.drawRectangle(pos.x, pos.y-6, sx<<4, 6, C_BLACK);
		render.drawRectangle(pos.x+sx, pos.y-5, (curstats.chp*((sx<<4)-sx))/curstats.mhp, 4, RGBA8(0,0xBF,0,0xFF));
	}
	onDraw(render);
}
