#include <new>

#include "ObjectFactory.h"
#include "OPickable.h"
#include "OWeapon.h"
#include "OItem.h"
#include "OGate.h"
#include "OWall.h"
#include "OEventBlock.h"
#include "OTrigger.h"
#include "OLever.h"
#include "OBrickWall.h"
#include "OBounceBall.h"
#include "OCheckpoint.h"
#include "OChest.h"
#include "OConveyor.h"
#include "OEnemy.h"
#include "OSword.h"
#include "OBoomerang.h"
#include "OPopup.h"

#define CASE(O,C) case O: new(out) C(level, O); break;

bool gameEntityFactory(GameEntity* out, EntityType id, Level& level) {
	switch(id) {
		CASE(OBJ_KEY, OPickable)
		CASE(OBJ_MAP, OPickable)
		CASE(OBJ_HPUP, OPickable)
		CASE(OBJ_FRIEND, OPickable)
		CASE(OBJ_WSWORD, OWeapon)
		CASE(OBJ_WBOOMERANG, OWeapon)
		CASE(OBJ_WPICKAXE, OWeapon)
		CASE(OBJ_IPOTION, OItem)
		CASE(OBJ_IPILL, OItem)
		CASE(OBJ_LOCKEDGATE, OGate)
		CASE(OBJ_GATE, OGate)
		CASE(OBJ_EVTRIGGER, OEventBlock)
		CASE(OBJ_SWITCH, OTrigger)
		CASE(OBJ_LEVER, OLever)
		CASE(OBJ_BRICKWALL, OBrickWall)
		CASE(OBJ_EVWALL, OWall)
		CASE(OBJ_BOUNCEBALL, OBounceBall)
		CASE(OBJ_CHECKPOINT, OCheckpoint)
		CASE(OBJ_CHEST, OChest)
		CASE(OBJ_CONVEYOR, OConveyor)
		CASE(OBJ_SKELETON, OEnemy)
		CASE(OBJ_SLIME, OEnemy)
		CASE(OBJ_MUSH, OEnemy)
		CASE(OBJ_SPIRIT, OEnemy)
		CASE(OBJ_SPIDER, OEnemy)
		CASE(OBJ_CHESTCRAB, OEnemy)
		CASE(OBJ_SWORD, OSword)
		CASE(OBJ_PICKAXE, OSword)
		CASE(OBJ_BOOMERANG, OBoomerang)
		CASE(OBJ_POPUP, OPopup)
		default: return false;
	}
	return true;
}
