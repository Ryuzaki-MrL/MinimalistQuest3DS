#ifndef GAMEENTITY_H
#define GAMEENTITY_H

#include "types.h"
#include "Counter.h"
#include "ISerializable.h"
#include "EntityData.h"
#include "Components.h"
#include "Properties.h"

class Level;
class RenderEngine;
class GameEntity: public Counter<GameEntity>, public ISerializable {
  protected:
	SpriteComponent spr;
	StatComponent curstats;
	PositionComponent pos;
	MovementComponent mv;
	//ReservedComponent resv;
	TimerComponent tmc;
	LootComponent loot;
	FlagComponent flg;
	PathComponent path;

	Level& level;
	const EntityData& data;
	const uint16_t uid;
	const EntityType type;
	bool dead;

  private:
	virtual void onUpdate() {}
	virtual void onDraw(RenderEngine&) {}

	virtual void onKill() {}

	virtual void onMove() {}
	virtual void onLoad() {}
	virtual void onInteract(GameEntity& /* other */) {}

  public:
	GameEntity(Level& level, EntityType type);
	virtual ~GameEntity() {}

	inline EntityType getType() const { return type; }
	inline uint16_t getUID() const { return uid; }

	inline int getX() const { return pos.x; }
	inline int getY() const { return pos.y; }
	bool move(float x, float y);
	inline void setInitialPosition(int x, int y) { pos = PositionComponent(x, y); }

	inline const Rectangle getBoundingBox(int xx, int yy) const { return spr.getBoundingBox() + Rectangle(xx, xx, yy, yy); }
	inline const Rectangle getBoundingBox() const { return getBoundingBox(pos.x, pos.y); }

	inline SpriteComponent& getSprite() { return spr; }
	inline LootComponent& getLoot() { return loot; }
	inline MovementComponent& getMovement() { return mv; }
	inline StatComponent& getStats() { return curstats; }
	inline const StatComponent& getStats() const { return curstats; }
	inline const BaseStatData& getBaseStats() const { return getBaseStatData(curstats.baseidx); }

	// TODO: onEvent
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
	void draw(RenderEngine& render);

	void serialize(FILE* out) const;
	void deserialize(FILE* in);
};

#endif /* GAMEENTITY_H */