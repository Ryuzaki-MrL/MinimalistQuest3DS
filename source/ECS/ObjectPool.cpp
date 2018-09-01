#include <algorithm>

#include "ObjectPool.h"
#include "ObjectFactory.h"

ObjectPool::ObjectPool(size_t n): obj(new ClassBuffer<GameEntity>[n]), size(n), count(0) {
	activelist.reserve(n);
}
ObjectPool::~ObjectPool() {
	for (size_t i = 0; i < count; ++i) {
		obj[i].get()->~GameEntity();
	}
	delete obj;
}

GameEntity* ObjectPool::create(EntityType id, Level& level) {
	if (count < size && gameEntityFactory(obj[count], id, level)) {
		activelist.push_back(count);
		return obj[count++];
	}
	return nullptr;
}

void ObjectPool::destroy(size_t pos) {
	obj[pos].get()->~GameEntity();
	std::swap(obj[pos], obj[--count]);
}

void ObjectPool::garbageCollect() {
	for (size_t i = 0; i < activelist.size();) {
		if (obj[activelist[i]].get()->isDead()) {
			std::swap(activelist[i], activelist.back());
			activelist.pop_back();
		} else {
			++i;
		}
	}
}

void ObjectPool::clear() {
	count = 0;
	clearActiveList();
}

// TODO: add flexibility to build list based on any criteria
void ObjectPool::buildActiveList(const Rectangle& region, bool inside) {
	clearActiveList();
	for (size_t i = 0; i < count;) {
		const GameEntity& inst = *obj[i];
		if (inst.isDead()) {
			destroy(i);
		} else {
			if ((inst.hasProperty(PROPERTY_PERSISTENT)) || (inst.collideWith(region) ^ !inside)) {
				activelist.push_back(i);
			}
			++i;
		}
	}
}

GameEntity* ObjectPool::find(ObjectPredFn pred) {
	for (size_t i = 0; i < count; ++i) {
		GameEntity* inst = obj[i];
		if (pred(*inst)) return inst;
	}
	return nullptr;
}
