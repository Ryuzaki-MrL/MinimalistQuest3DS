#ifndef SAVEDATA_H
#define SAVEDATA_H

#include "WorldData.h"

#define MAX_PROFILES	1

struct SaveProfile {
  private:
	char username[11];

  public:
	inline const char* getUserName() const { return username; }
	void setUserName(const char* name); // safe
	inline bool isValid() const { return username[0]!=0; }

  friend class SaveData;
};

struct SaveSystem {
	uint8_t lang : 6;
	bool debug : 1;
};

class SaveData {
  private:
	SaveProfile profiles[MAX_PROFILES];
	SaveSystem sys;

  public:
	SaveData(const char* filename);
	~SaveData();

	inline SaveProfile& getProfile(uint8_t p) { return profiles[p]; }
	inline SaveSystem& getSystemData() { return sys; }

	bool commit(const char* filename) const;
	void clear();
};

SaveData& getGlobalSavedata();
bool commitGlobalSavedata();
const SaveProfile& currentSaveProfile();
void selectSaveProfile(uint8_t slot);
void loadCurrentSavedata(WorldData& out);
void commitCurrentSavedata(const WorldData& world);

#endif /* SAVEDATA_H */
