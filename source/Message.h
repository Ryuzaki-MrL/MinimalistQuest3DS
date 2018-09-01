#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdint.h>
#include <string>

enum { LANG_EN, LANG_BR, MAX_LANG };

void messageInit(const char* filename);

void messageSetLang(uint8_t newlang);
uint8_t messageGetLang();

const char* messageGet(uint8_t id);
const char* messageGet(const std::string& id);
const char* messageGetLangStr();
uint8_t messageGetIndex(const std::string& id);

#endif /* MESSAGE_H */