#include <stdio.h>
#include <string.h>

#include "Savedata.h"

#define SAVEDATA_PATH   "savedata.bin"
#define PROFILE_PATH	"profiles.bin"

static SaveData g_save;
static WorldData g_wdata[MAX_PROFILES];
static bool g_saveinit = false;
static uint8_t g_slot = 0;

void SaveProfile::setUserName(const char* name) {
	strncpy(username, name, sizeof(username) - 1);
	username[sizeof(username) - 1] = 0;
}

SaveData::SaveData(): profiles(), sys() {}
SaveData::~SaveData() {}

bool SaveData::load(const char* filename) {
	FILE* file = fopen(filename, "rb");
	if (!file) return false;
	fread(this, 1, sizeof(SaveData), file);
	fclose(file);
	for (int i = 0; i < MAX_PROFILES; ++i) {
		profiles[i].username[sizeof(profiles[i].username) - 1] = 0;
	}
	return true;
}

bool SaveData::commit(const char* filename) const {
	FILE* file = fopen(filename, "wb");
	if (!file) return false;
	fwrite(this, 1, sizeof(SaveData), file);
	fclose(file);
	return true;
}

void SaveData::clear() {
	memset(this, 0, sizeof(SaveData));
}

SaveData& getGlobalSavedata() {
	return g_save;
}

bool loadGlobalSavedata() {
	return g_save.load(PROFILE_PATH);
}

bool commitGlobalSavedata() {
	return g_save.commit(PROFILE_PATH);
}

const SaveProfile& currentSaveProfile() {
	return g_save.getProfile(g_slot);
}

void selectSaveProfile(uint8_t slot) {
	g_slot = slot % MAX_PROFILES;
}

static void loadGlobalWorldData(const char* filename) {
	FILE* file = fopen(filename, "rb");
	if (!file) return;
	fread(g_wdata, 1, sizeof(g_wdata), file);
	fclose(file);
}

static void saveGlobalWorldData(const char* filename) {
	FILE* file = fopen(filename, "wb");
	if (!file) return;
	fwrite(g_wdata, 1, sizeof(g_wdata), file);
	fclose(file);
}

void loadCurrentSavedata(WorldData& out) {
	if (!g_saveinit) {
		loadGlobalWorldData(SAVEDATA_PATH);
		g_saveinit = true;
	}
	out = g_wdata[g_slot];
}

void commitCurrentSavedata(const WorldData& world) {
	g_wdata[g_slot] = world;
	saveGlobalWorldData(SAVEDATA_PATH);
}
