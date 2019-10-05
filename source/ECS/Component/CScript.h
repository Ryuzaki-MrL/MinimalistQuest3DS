#ifndef CSCRIPT_H
#define CSCRIPT_H

#include <stdint.h>

class Level;
struct ScriptComponent {
  private:
	uint8_t interp;
	bool running = false;
	bool done = false;
  public:
	uint8_t scrid;

	void run();
	void setArg(int arg, int val);
	inline bool isDone() const { return done; }
	inline bool isRunning() const { return running; }
	inline void reset() { done = running = false; }
	void update(Level&);
};

#endif /* CSCRIPT_H */