#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <functional>

#include "Camera.h"
#include "OPlayer.h"
#include "ObjectPool.h"
#include "WorldData.h"

//typedef std::function<void(GameEntity&)> CollisionFn;
typedef std::function<void(GameEntity&)> EntityFn;
typedef unsigned char Tile;

enum {
	LFLAG_CUTSCENE = 0b0001,
	LFLAG_GAMEOVER = 0b0010,
	LFLAG_ENDGAME  = 0b0110
};

#define TILE_SOLID		0x40
#define TILE_PASSABLE	0x20

struct LevelHeader {
	uint32_t magic;
	uint8_t width;
	uint8_t height;
	uint8_t playerx;
	uint8_t playery;
};

class Level {
  private:
	WorldData world;
	LevelHeader header;
	std::vector<Tile> tilemap;
	std::vector<Rectangle> sections;
	OPlayer player;
	ObjectPool pool;
	Camera camera;
	size_t occsecs, cursec;
	uint8_t flags;
	bool scrolling;

  public:
	Level();
	~Level();

	bool load(const char* filename);

	GameEntity* instanceCreate(int x, int y, EntityType id);
	void instanceDestroy(GameEntity*);
	inline size_t instanceCount() const { return pool.getCount(); }
	size_t countActive(EntityType type = OBJ_NONE) const;
	void refreshActive();

	inline void setFlag(uint8_t mask) { flags |= mask; }
	inline void clearFlag(uint8_t mask) { flags &= ~mask; }
	inline bool checkFlag(uint8_t mask) const { return ((flags & mask)==mask); }

	inline Tile getTile(uint8_t x, uint8_t y) const { return tilemap[y * header.width + x]; }
	inline void setTile(Tile t, uint8_t x, uint8_t y) { tilemap[y * header.width + x] = t; }
	inline uint32_t getWidth() const { return header.width << 4; }
	inline uint32_t getHeight() const { return header.height << 4; }
	void setSection(int x, int y, int xs = 0, int ys = 0);
	inline size_t getSectionCurrent() const { return cursec; }
	inline size_t getSectionCount() const { return occsecs; }

	inline const Camera& getCamera() const { return camera; }
	inline Camera& getCamera() { return camera; }
	inline const OPlayer& getPlayer() const { return player; }
	inline OPlayer& getPlayer() { return player; }
	inline const WorldData& getWorldData() const { return world; }
	inline WorldData& getWorldData() { return world; }

	bool checkTile(const Rectangle& bbox, uint8_t flags) const;
	bool checkSolid(const Rectangle& bbox, size_t except) const;
	const GameEntity* checkObject(const Rectangle& bbox, size_t except, uint16_t props = 0xFFFF) const;
	void collisionHandle(const Rectangle& bbox, size_t except, EntityFn colfn);
	void foreach(EntityFn, int group = -1);

	void update();
	void draw(Renderer&);

	static const char* makePath(uint8_t id);
};

#endif /* LEVEL_H */