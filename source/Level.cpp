#include <stdio.h>
#include <stdlib.h>
#include <new>

#include "Level.h"
#include "rle.h"
#include "defs.h"
#include "Graphics.h"

#define LEVELPATH "romfs:/map%u.lvl" // get workdir from platform.cpp
#define OBJECT_CAPACITY 1000

Level::Level(): header(), player(*this), pool(OBJECT_CAPACITY), flags(0) {}
Level::~Level() {}

const char* Level::makePath(uint8_t id) {
	static char lvpath[256];
	std::sprintf(lvpath, LEVELPATH, id);
	return lvpath;
}

bool Level::load(const char* filename) {
	FILE* lvl = fopen(filename, "rb");
	if (!lvl) return false;

	fread(&header, 1, sizeof(header), lvl);

	tilemap.resize(header.width * header.height);
	size_t sz = 0;
	fread(&sz, sizeof(sz), 1, lvl);
	char* buf = new char[sz];
	if (!buf) return false;
	fread(buf, 1, sz, lvl);
	rleDecompress((char*)tilemap.data(), buf, sz);
	delete buf;

	sections.resize((header.width / 20) * (header.height / 15));
	struct { uint8_t sx, sy, sw, sh; } ss;
	occsecs = 0;
	do {
		fread(&ss, sizeof(ss), 1, lvl);
		for (int j = ss.sy; j < ss.sy + ss.sh; ++j) {
			for (int i = ss.sx; i < ss.sx + ss.sw; ++i) {
				sections[j * (header.width / 20) + i] = Rectangle(
					ss.sx * SUBSCREEN_WIDTH, ss.sx * SUBSCREEN_WIDTH + ss.sw * SUBSCREEN_WIDTH,
					ss.sy * SCREEN_HEIGHT, ss.sy * SCREEN_HEIGHT + ss.sh * SCREEN_HEIGHT
				);
				++occsecs;
			}
		}
	} while(ss.sw); --occsecs;

	pool.clear();

	EntityType oid;
	fread(&sz, sizeof(sz), 1, lvl);
	for (size_t i = 0; i < sz; ++i) {
		fread(&oid, sizeof(EntityType), 1, lvl);
		GameEntity* ob = pool.create(oid, *this);
		if (ob) ob->deserialize(lvl);
	}

	new(&player) OPlayer(*this);
	player.setPosition(
		world.player.x ? (world.player.x * 16) : (header.playerx * 16),
		world.player.y ? (world.player.y * 16) : (header.playery * 16)
	);
	setSection(player.getX(), player.getY());
	camera.update(player.getX(), player.getY());

	fclose(lvl);
	return true;
}

GameEntity* Level::instanceCreate(int x, int y, EntityType id) {
	if (id == OBJ_PLAYER) {
		player.setPosition(x, y);
		setSection(player.getX(), player.getY());
		return nullptr;
	} else {
		GameEntity* obj = pool.create(id, *this);
		if (obj) obj->setPosition(x, y);
		return obj;
	}
}

void Level::instanceDestroy(GameEntity* obj) {
	if (obj) obj->kill();
	pool.garbageCollect();
}

size_t Level::countActive(EntityType type) const {
	if (type == OBJ_NONE) {
		return pool.getActiveList().size();
	}
	size_t cnt = 0;
	for (size_t i : pool.getActiveList()) {
		cnt += (pool[i]->getType() == type);
	}
	return cnt;
}

void Level::refreshActive() {
	pool.clearActiveList();
	pool.buildActiveList(camera.getLimit(), true);
}

void Level::setSection(int x, int y, int xs, int ys) {
	cursec = (header.width / 20) * (y / 240) + (x / 320);
	const Rectangle& sec = sections[cursec];
	camera.setPosition((camera.getX() / 320) * 320, (camera.getY() / 240) * 240);
	camera.setLimit(sec);
	camera.setSpeed(xs, ys);
	scrolling = true;
	pool.clearActiveList();
}

bool Level::checkTile(const Rectangle& bbox, uint8_t flags) const {
	return (
		(getTile(bbox.left  >> 4, bbox.top >> 4) & flags) ||
		(getTile(bbox.right >> 4, bbox.top >> 4) & flags) ||
		(getTile(bbox.right >> 4, bbox.bot >> 4) & flags) ||
		(getTile(bbox.left  >> 4, bbox.bot >> 4) & flags)
	);
}

bool Level::checkSolid(const Rectangle& bbox, size_t except) const {
	return (checkTile(bbox, TILE_SOLID)) || (checkObject(bbox, except, PROPERTY_SOLID) != nullptr);
}

const GameEntity* Level::checkObject(const Rectangle& bbox, size_t except, uint16_t props) const {
	if (player.getUID()!=except && player.collideWith(bbox) && player.hasProperty(props)) {
		return &player;
	}
	for (size_t i : pool.getActiveList()) {
		if (pool[i]->getUID()!=except && pool[i]->collideWith(bbox) && pool[i]->hasProperty(props)) {
			return pool[i];
		}
	}
	return nullptr;
}

void Level::collisionHandle(const Rectangle& bbox, size_t except, EntityFn colfn) {
	if (player.getUID()!=except && player.collideWith(bbox)) {
		colfn(player);
	}
	for (size_t i : pool.getActiveList()) {
		if (pool[i]->getUID()!=except && pool[i]->collideWith(bbox)) {
			colfn(*pool[i]);
		}
	}
}

void Level::foreach(EntityFn entfn, int group) {
	for (size_t i : pool.getActiveList()) {
		GameEntity& ent = *pool[i];
		if (group < 0 || ent.getGroup() == group) {
			entfn(ent);
		}
	}
}

void Level::update() {
	if (scrolling) {
		if (!camera.isScrolling()) {
			scrolling = false;
			pool.buildActiveList(camera.getLimit(), true);
		}
	} else {
		for (size_t i : pool.getActiveList()) {
			pool[i]->update();
		}
		player.update();
		cursec = (header.width / 20) * ((player.getY()+7) / 240) + ((player.getX()+7) / 320);
		world.map.visit(cursec);
		pool.garbageCollect();
	}
	camera.update(player.getX(), player.getY());
}

void Level::draw(Renderer& render) {
	render.screenClear(C_WHITE);
	// depth order: bg -> tilemap -> sprites
	float threedee = render.get3D() * 2.0;
	render.screenTranslate(camera.getX() - 40 + threedee, camera.getY());
	render.drawTextureFill(TEX_BACKGROUND);
	int tx = camera.getX() / TILE_WIDTH;
	int ty = camera.getY() / TILE_HEIGHT;
	for (int j = ty; j <= ty + SCREEN_HEIGHT / TILE_HEIGHT && j < header.height; ++j) {
		for (int i = tx; i <= tx + SUBSCREEN_WIDTH / TILE_WIDTH && i < header.width; ++i) {
			if (!getTile(i, j)) continue;
			render.drawTile(getTile(i, j) & 0x1F, i*TILE_WIDTH, j*TILE_HEIGHT);
		}
	}
	render.screenTranslate(-threedee/2.0, 0);
	for (size_t i : pool.getActiveList()) {
		pool[i]->draw(render);
	}
	player.draw(render);
	render.screenTranslate(-camera.getX() + 40 - (threedee/2.0), -camera.getY());
	threedee = fabs(threedee);
	render.drawRectangle(0, 0, 40 + threedee, 240, C_BLACK);
	render.drawRectangle(360 - threedee, 0, 41 + threedee, 240, C_BLACK);
}
