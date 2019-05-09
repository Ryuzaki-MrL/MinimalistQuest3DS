#ifndef SCRIPT_H
#define SCRIPT_H

#include <stdint.h>

void scriptsLoad(const char* filename);

const char* scriptGet(uint8_t id); // returns pointer to first instruction

#endif /* SCRIPT_H */