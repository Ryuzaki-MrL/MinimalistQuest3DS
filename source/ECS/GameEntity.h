#ifndef GAMEENTITY_H
#define GAMEENTITY_H

#include "types.h"
#include "ISerializable.h"
#include "EntityData.h"
#include "Components.h"
#include "Properties.h"

class Level;
class Renderer;
class GameEntity: public ISerializable {
  protected:
	SpriteComponent spr;
	StatComponent curstats;
	PositionComponent pos;
	MovementComponent mv;
	TimerComponent tmc;
	LootComponent loot;
	FlagComponent flg;
	PathComponent path;
	ScriptComponent scr;

	Level& level;
	const EntityData& data;
	const uint16_t uid;
	const EntityType type;
	uint8_t group;
	bool dead;

  private:
	virtual void onUpdate() {}
	virtual void onDraw(Renderer&) {}

	virtual void onKill() {}

	virtual void onMove() {}
	virtual void onInteract(GameEntity& /* other */) {}

  public:
	GameEntity(Level& level, EntityType type);
	virtual ~GameEntity();

	inline EntityType getType() const { return type; }
	inline uint16_t getUID() const { return uid; }
	inline uint8_t getGroup() const { return group; }
	void setGroup(uint8_t);

	inline int getX() const { return pos.x; }
	inline int getY() const { return pos.y; }
	bool move(float x, float y);
	inline void setPosition(int x, int y) { pos = PositionComponent(x, y); }
	inline void setPath(uint8_t type) { path.type = type; }

	inline const Rectangle getBoundingBox(int xx, int yy) const { return spr.getBoundingBox() + Rectangle(xx, xx, yy, yy); }
	inline const Rectangle getBoundingBox() const { return getBoundingBox(pos.x, pos.y); }

	inline SpriteComponent& getSprite() { return spr; }
	inline LootComponent& getLoot() { return loot; }
	inline MovementComponent& getMovement() { return mv; }
	inline StatComponent& getStats() { return curstats; }
	inline const StatComponent& getStats() const { return curstats; }
	inline const BaseStatData& getBaseStats() const { return getBaseStatData(curstats.baseidx); }

	virtual void onLoad() {}
	virtual void onTimeOut() {}
	virtual void onDamage(uint8_t /* amount */, GameEntity& /* damager */) {}

	inline bool hasComponent(uint16_t comp) const { return data.comps & comp; }
	inline bool hasProperty(uint16_t prop) const { return data.props & prop; }

	void kill();
	inline bool isDead() const { return dead; }

	void interact();
	void damage(EntityType except = OBJ_NONE);
	void popup(const char* msg);
	inline bool collideWith(const Rectangle& r) const { return r.collideWith(getBoundingBox()); }
	inline bool collideWith(const GameEntity& obj) const { return obj.collideWith(getBoundingBox()); }

	void update();
	void draw(Renderer&);

	void serialize(FILE* out) const;
	void deserialize(FILE* in);

	static uint16_t getCount(EntityType);
	static uint16_t getCountGroup(uint8_t);
};

#endif /* GAMEENTITY_H */