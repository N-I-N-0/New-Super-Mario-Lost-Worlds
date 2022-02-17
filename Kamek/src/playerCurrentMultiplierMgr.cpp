#include <common.h>
#include <game.h>
#include <profile.h>

class dPlCrMgr_c : public dStageActor_c {
public:
	int onCreate();
	int onExecute();

	static dActor_c *build();
};

const char *PlCrMgrFileList[] = {0};
const SpriteData PlCrMgrSpriteData = { ProfileId::PlCrMgr, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile PlCrMgrProfile(&dPlCrMgr_c::build, SpriteId::PlCrMgr, &PlCrMgrSpriteData, ProfileId::PlCrMgr, ProfileId::PlCrMgr, "PlCrMgr", PlCrMgrFileList);

dActor_c  *dPlCrMgr_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dPlCrMgr_c));
	dPlCrMgr_c *c = new(buffer) dPlCrMgr_c;

	return c;
}

float currentMultiplier;
float currentMultiplier2;
bool doesPlayerCurrentMultiplierMgrExist() {
	return fBase_c::search(PlCrMgr) != NULL;
}


int dPlCrMgr_c::onCreate() {
	currentMultiplier = 0.01f * (this->settings & 0xFFFFFFFF);
	currentMultiplier2 = 0.001f * (this->settings & 0xFFFFFFFF);

	return true;
}

int dPlCrMgr_c::onExecute() {
	return true;
}