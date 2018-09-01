#include "GameEntity.h"
#include "Level.h"
#include "Graphics.h"
#include "Message.h"

template<int N> struct _{ operator char() { return N + 256; } };

static uint16_t s_nextuid = 0;

GameEntity::GameEntity(Level& level, EntityType type):
  level(level), data(entityGetData(type)), uid(++s_nextuid), type(type), dead(false) {
	char(_<sizeof(GameEntity)>()); // report size as a compiler warning
	spr = SpriteComponent(Sprite(data.sprite), data.anim);
	path.type = data.path;
	curstats.baseidx = data.baseidx;
	curstats.setLevel(1);
}

void GameEntity::serialize(FILE* out) const {
	fwrite(&pos.xs, 1, sizeof(pos.xs), out);
	fwrite(&pos.ys, 1, sizeof(pos.ys), out);
	if (hasComponent(COMPONENT_SPRITE)) fwrite(&spr, 1, sizeof(spr), out); else // >>
	if (hasProperty(PROPERTY_SPRTRSAVE)) fwrite(&spr.tr, 1, sizeof(spr.tr), out);
	if (hasComponent(COMPONENT_STATS)) fwrite(&curstats.lvl, 1, sizeof(curstats.lvl), out);
	if (hasComponent(COMPONENT_FLAG)) fwrite(&flg, 1, sizeof(flg), out);
	if (hasComponent(COMPONENT_LOOT)) fwrite(&loot, 1, sizeof(loot), out);
	if (hasComponent(COMPONENT_PATH)) fwrite(&path.type, 1, sizeof(path.type), out);
}

void GameEntity::deserialize(FILE* in) {
	fread(&pos.xs, 1, sizeof(pos.xs), in);
	fread(&pos.ys, 1, sizeof(pos.ys), in);
	pos.reset();
	if (hasComponent(COMPONENT_SPRITE)) fread(&spr, 1, sizeof(spr), in); else // >>
	if (hasProperty(PROPERTY_SPRTRSAVE)) fread(&spr.tr, 1, sizeof(spr.tr), in);
	if (hasComponent(COMPONENT_STATS)) {
		fread(&curstats.lvl, 1, sizeof(curstats.lvl), in);
		curstats.setLevel(curstats.lvl);
	}
	if (hasComponent(COMPONENT_FLAG)) fread(&flg, 1, sizeof(flg), in);
	if (hasComponent(COMPONENT_LOOT)) fread(&loot, 1, sizeof(loot), in);
	if (hasComponent(COMPONENT_PATH)) fread(&path.type, 1, sizeof(path.type), in);
	onLoad();
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
	level.collisionHandle(getBoundingBox() + Rectangle(-8, 8, -8, 8), *this, [](GameEntity& me, GameEntity& other) {
		other.onInteract(me);
	});
}

void GameEntity::damage(EntityType except) {
	level.collisionHandle(getBoundingBox(), *this, [except](GameEntity& me, GameEntity& other) {
		if ((other.hasProperty(PROPERTY_DAMAGEABLE)) && (other.getType() != except)) {
			other.getStats().damage(other, me /* damager */);
		}
	});
}

void GameEntity::popup(const char* msg) {
	GameEntity* pp = level.instanceCreate(getX(), getY() - 20, OBJ_POPUP);
	if (pp) pp->getLoot().type = messageGetIndex(msg);
}

void GameEntity::update() {
	spr.update();
	tmc.update(*this);
	if (hasComponent(COMPONENT_MOVEMENT)) mv.update(*this);
	if (hasComponent(COMPONENT_PATH)) path.update(*this, level);
	if (curstats.imm) --curstats.imm; // temporary hack
	else if (hasComponent(COMPONENT_STATS)) spr.color.c.a = 0xFF;
	onUpdate();
}

void GameEntity::draw(RenderEngine& render) {
	if (hasProperty(PROPERTY_SHOWDIR)) spr.tr.angle = mv.direction; // temporary hack
	spr.draw(pos.x, pos.y, render);
	if ((hasProperty(PROPERTY_DAMAGEABLE)) && (curstats.chp < curstats.mhp)) {
		int8_t sx = abs(spr.tr.xscale);
		render.drawRectangle(pos.x, pos.y-6, sx<<4, 6, C_BLACK);
		render.drawRectangle(pos.x+sx, pos.y-5, (curstats.chp*((sx<<4)-sx))/curstats.mhp, 4, RGBA8(0,0x7F,0,0xFF));
	}
	onDraw(render);
}
