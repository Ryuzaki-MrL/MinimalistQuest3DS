#ifndef OBJECTPOOL_H
#define OBJECTPOOL_H

#include <vector>

#include "ClassBuffer.h"
#include "GameEntity.h"

typedef bool(*ObjectPredFn)(const GameEntity&);

class ObjectPool {
  private:
	ClassBuffer<GameEntity>* obj;
	std::vector<size_t> activelist;
	size_t size, count;

  public:
	ObjectPool(size_t n);
	~ObjectPool();

	inline GameEntity* get(size_t pos) { return (pos < count) ? obj[pos].get() : nullptr; }
	inline const GameEntity* get(size_t pos) const { return (pos < count) ? (obj[pos].get()) : nullptr; }
	inline GameEntity* operator[](size_t pos) { return obj[pos]; }
	inline const GameEntity* operator[](size_t pos) const { return obj[pos]; }
	inline size_t getCount() const { return count; }
	inline size_t getSize() const { return size; }

	GameEntity* create(EntityType id, Level& level);
	void destroy(size_t pos);
	void garbageCollect();
	void clear();

	void buildActiveList(const Rectangle& region, bool inside = true);
	inline void clearActiveList() { activelist.clear(); }
	inline const std::vector<size_t>& getActiveList() const { return activelist; }

	GameEntity* find(ObjectPredFn pred);
};

#endif /* OBJECTPOOL_H */