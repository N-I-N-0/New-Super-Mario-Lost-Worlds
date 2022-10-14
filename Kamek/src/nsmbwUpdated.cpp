#include <game.h>

const char* MantaMgr2FileList [] = {"manjirou", NULL};

class dMantaMgr2_c : public dStageActor_c {
public:
	static dActor_c *build(); // Method to allocate memory for the actor.

	int onCreate(); // Called once when the actor is created.
};


const SpriteData MantaMgr2SpriteData = { ProfileId::MANTA_MGR2_NEW, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile MantaMgr2Profile(&dMantaMgr2_c::build, SpriteId::MANTA_MGR2, &WaterliftSpawnerSpriteData, ProfileId::MANTA_MGR2_NEW, ProfileId::MANTA_MGR2_NEW, "MANTA_MGR2", MantaMgr2FileList);


dActor_c* dMantaMgr2_c::build() {
	void* buffer = AllocFromGameHeap1(sizeof(dMantaMgr2_c));
	return new(buffer) dMantaMgr2_c;
}


s32 dMantaMgr2_c::onCreate() {
	dStageActor_c* actor = (dStageActor_c*) dStageActor_c::create(MANTA_MGR2, settings, &(this->pos), 0, 0);
	return true;
}