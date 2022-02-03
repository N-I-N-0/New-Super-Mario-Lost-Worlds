#include <game.h>

const char* WaterliftFileList [] = {"lift_han_wood", "lift_han_stone", "lift_kinoko_yoko", "lift_han_spin", "boss_koopa_lift", "boss_koopa_lift_down", NULL};

class dWaterliftSpawner : public dStageActor_c {
public:
	static dActor_c *build(); // Method to allocate memory for the actor.

	int onCreate(); // Called once when the actor is created.
};


const SpriteData WaterliftSpawnerSpriteData = { ProfileId::WATER_LIFT_NEW, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile WaterliftSpawnerProfile(&dWaterliftSpawner::build, SpriteId::WATER_LIFT, &WaterliftSpawnerSpriteData, ProfileId::WATER_LIFT_NEW, ProfileId::WATER_LIFT_NEW, "WATER_LIFT", WaterliftFileList);


dActor_c* dWaterliftSpawner::build() {
	void* buffer = AllocFromGameHeap1(sizeof(dWaterliftSpawner));
	return new(buffer) dWaterliftSpawner;
}


s32 dWaterliftSpawner::onCreate() {
	dStageActor_c* actor = (dStageActor_c*) dStageActor_c::create(WATER_LIFT, settings, &(this->pos), 0, 0);
	return true;
}