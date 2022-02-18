#include <common.h>
#include <game.h>
#include <profile.h>
#include "colour.h"

class dWindColorMgr_c : public dStageActor_c {
public:
	int onCreate();
	int onExecute();
	int onDelete();

	static dActor_c *build();
};

const char *WindColorMgrFileList[] = {0};
const SpriteData WindColorMgrSpriteData = { ProfileId::WindColorMgr, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile WindColorMgrProfile(&dWindColorMgr_c::build, SpriteId::WindColorMgr, &WindColorMgrSpriteData, ProfileId::WindColorMgr, ProfileId::WindColorMgr, "WindColorMgr", WindColorMgrFileList);

dActor_c  *dWindColorMgr_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dWindColorMgr_c));
	dWindColorMgr_c *c = new(buffer) dWindColorMgr_c;

	return c;
}

colour windColor[] = {0xffdc78ff};

int dWindColorMgr_c::onCreate() {
	windColor[0].rgba = ((this->settings & 0xFFFFFF) << 8);
	
	return true;
}

int dWindColorMgr_c::onExecute() {
	return true;
}

int dWindColorMgr_c::onDelete() {
	windColor[0].rgba = 0xffdc78ff;

	return true;
}