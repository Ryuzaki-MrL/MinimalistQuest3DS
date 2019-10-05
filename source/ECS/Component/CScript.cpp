#include <queue>

#include "CScript.h"
#include "Script.h"
#include "Level.h"
#include "Message.h"
#include "Textbox.h"
#include "Fading.h"

#define NUM_VARS 16
#define NUM_ARGS 16
#define NUM_INTERPRETERS 16

// group always comes last
#define FOREACH(X) level.foreach([&](GameEntity& ent) { X }, next(level))

class Interpreter {
  private:
	int vars[NUM_VARS];
	int args[NUM_ARGS];
	const char* baseaddr;
	int pc = 0;
	int timer = 0;
	bool running = false;

	int next(Level&); // recursive

  public:
	void start(const char* base);
	void update(Level&);
	inline void setArg(int i, int val) { args[i] = val; }
	inline bool isRunning() const { return running; }
};

int Interpreter::next(Level& level) {
	if (timer) return --timer;
	int op = baseaddr[pc++];
	switch(op) {
		case 1: { // CREATE
			int x = next(level); int y = next(level);
			GameEntity* e = level.instanceCreate(x, y, EntityType(next(level)));
			if (e) e->setGroup(next(level));
			break;
		}
		case 2: { // DESTROY
			FOREACH( ent.kill(); ); break;
		}
		case 3: { // COUNT
			return GameEntity::getCountGroup(next(level));
		}
		case 4: { // COUNTALL
			return level.countActive(EntityType(next(level)));
		}
		case 5: { // MOVE
			int x = next(level); int y = next(level);
			FOREACH( ent.move(x, y); ); break;
		}
		case 6: { // MOVETO
			int x = next(level); int y = next(level);
			FOREACH( ent.setPosition(x, y); ); break;
		}
		case 7: { // SETLOOT
			int t = next(level);
			FOREACH( ent.loot.type = t; ); break;
		}
		case 8: { // SETSPR
			int s = next(level);
			FOREACH( ent.spr.setSprite(Sprite(s)); ); break;
		}
		case 9: { // SETANIM
			int a = next(level);
			FOREACH( ent.spr.steps = a; ); break;
		}
		case 10: { // SETPATH
			int p = next(level);
			FOREACH( ent.path.type = p; ); break;
		}
		case 11: { // SETFLAG
			int f = next(level);
			FOREACH( ent.flg.id = f; );
			level.refreshActive();
			break;
		}
		case 12: { // SETANGLE
			int a = next(level);
			FOREACH( ent.spr.setAngle(a); ); break;
		}
		case 13: { // SETSCALE
			int xs = next(level); int ys = next(level);
			FOREACH( ent.spr.scale(xs, ys); ); break;
		}
		case 14: { // SETCOLOR
			int r = next(level); int g = next(level); int b = next(level);
			int c = RGBA8(r,g,b,0xFF);
			FOREACH( ent.spr.color.rgba = c; ); break;
		}
		case 15: { // SETGROUP
			int g = next(level);
			FOREACH( ent.setGroup(g); ); break;
		}
		case 16: { // SETWFLAG
			level.getWorldData().uflags.set(next(level));
			level.refreshActive();
			break;
		}
		case 17: { // CHKWFLAG
			return level.getWorldData().uflags.test(next(level));
		}
		case 18: { // SETLFLAG
			level.setFlag(next(level)); break;
		}
		case 19: { // CLRLFLAG
			level.clearFlag(next(level)); break;
		}
		case 20: { // CHKLFLAG
			return level.checkFlag(next(level));
		}
		case 21: { // POPUP
			int x = next(level); int y = next(level); int t = next(level);
			GameEntity* pp = level.instanceCreate(x, y, OBJ_POPUP);
			if (pp) pp->loot.type = t;
			break;
		}
		case 22: { // TEXTBOX
			textboxCreate(messageGet(next(level))); break;
		}
		case 23: { // READARG
			return args[(uint8_t)baseaddr[pc++]];
		}
		case 24: { // SETCAM
			int x = next(level); int y = next(level);
			level.getCamera().setPosition(x, y);
			break;
		}
		case 25: { // MOVECAM
			int x = next(level); int y = next(level);
			level.getCamera().translate(x, y);
			break;
		}
		case 26: { // WAIT
			int prev = pc - 1, res = next(level);
			if (!res) pc = prev;
			return res;
		}
		case 27: { // SLEEP
			timer = next(level); break;
		}
		case 28: { // IF
			if (!next(level)) ++pc;
			break;
		}
		case 29:  { // NOT
			return !next(level);
		}
		case 30: { // GOTO
			pc = next(level); break;
		}
		case 31: { // STORE
			int val = next(level); int v = next(level);
			vars[v] = val;
			return vars[v];
		}
		case 32: { // ADD
			return next(level) + next(level);
		}
		case 33: { // ISLESS
			int op1 = next(level); int op2 = next(level);
			return op1 < op2;
		}
		case 34: { // ISGREATER
			int op1 = next(level); int op2 = next(level);
			return op1 > op2;
		}
		case 35: { // EQUALS
			int op1 = next(level); int op2 = next(level);
			return op1 == op2;
		}
		case 36: { // LITERAL
			return (int8_t)baseaddr[pc++];
		}
		case 37: { // VAR
			return vars[(uint8_t)baseaddr[pc++]];
		}
		case 38: { // FADE
			int steps = next(level); int color = next(level);
			fadeStart(steps, color);
			break;
		}
		case 39: { // PLAYERX
			return level.getPlayer().getX();
		}
		case 40: { // PLAYERY
			return level.getPlayer().getY();
		}
		case 41: { // CONTINUE
			return next(level), next(level);
		}
		case 42: { // MUL
			return next(level) * next(level);
		}
		case 43: { // CAMLIMX0
			return level.getCamera().getLimit().left;
		}
		case 44: { // CAMLIMY0
			return level.getCamera().getLimit().top;
		}
		case 45: { // CAMLIMX1
			return level.getCamera().getLimit().right;
		}
		case 46: { // CAMLIMY1
			return level.getCamera().getLimit().bot;
		}
		case 47: { // CLRWFLAG
			level.getWorldData().uflags.reset(next(level)); break;
		}
		default: { // END
			running = false;
		}
	}
	return 0;
}

void Interpreter::start(const char* base) {
	baseaddr = base;
	pc = 0;
	running = true;
}

void Interpreter::update(Level& level) {
	next(level);
}

static bool queueinit = false;
std::queue<uint8_t> freeinterps;
static Interpreter interps[NUM_INTERPRETERS];

static void initializeFreeQueue() {
	if (queueinit) return;
	queueinit = true;
	for (uint8_t i = 0; i < NUM_INTERPRETERS; ++i) freeinterps.push(i);
}

void ScriptComponent::run() {
	initializeFreeQueue();
	if (running) freeinterps.push(interp);
	if (freeinterps.empty()) return;
	interp = freeinterps.front(); freeinterps.pop();
	interps[interp].start(scriptGet(scrid));
	running = true;
	done = false;
}

void ScriptComponent::setArg(int arg, int val) {
	interps[interp].setArg(arg, val);
}

void ScriptComponent::update(Level& level) {
	if (running) {
		interps[interp].update(level);
		if (!interps[interp].isRunning()) {
			running = false;
			done = true;
			freeinterps.push(interp);
		}
	}
}
