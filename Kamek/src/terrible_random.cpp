const char* PRNGModifierFileList[] = { NULL };

class dRandom_c {
public:
    static void SetSeed(u32 seed);
    static void SetAltSeed(u32 alt_seed);
};

// The cycle length we want our PRNG to have.
// Choose 0 if you don't wish to change the
// PRNG seed at all.

u32 dRandom_c_SetRandomSeed(u32 x) {
    // These constants come from RoadrunnerWMC's research:
    // https://roadrunnerwmc.github.io/blog/2020/05/08/nsmb-rng.html
    /*#if CYCLE == 1
        u32 seed = 0x443b4723;
    #elif CYCLE == 2
        u32 seed = 0x99909c78;
    #elif CYCLE == 4
        u32 seed = 0x02fa05e2;
    #elif CYCLE == 8
        u32 seed = 0x000702ef;
    #else // CYCLE is not one of (1, 2, 4, 8)
        u32 seed = 0;
    #endif*/

	dRandom_c::SetSeed(0);
	dRandom_c::SetAltSeed(0);
    
    return x; // patched instruction
}



class dPRNGModifier_c : public dStageActor_c {
public:
	int onCreate();
	int onDelete();

	static dActor_c *build();
};

dActor_c  *dPRNGModifier_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dPRNGModifier_c));
	dPRNGModifier_c *c = new(buffer) dPRNGModifier_c;

	return c;
}

const SpriteData PRNGModifierSpriteData = { ProfileId::PRNGModifier, 0, 0, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile PRNGModifierProfile(&dPRNGModifier_c::build, SpriteId::PRNGModifier, &PRNGModifierSpriteData, ProfileId::PRNGModifier, ProfileId::PRNGModifier, "PRNGModifier", PRNGModifierFileList);

int dPRNGModifier_c::onCreate() {
	u32 seed = this->settings;
	if (this->currentLayerID & 1) {
		dRandom_c::SetSeed(seed);
	}
	if (this->currentLayerID & 2) {
		dRandom_c::SetAltSeed(seed);
	}
	
	
	return true;
}


int dPRNGModifier_c::onDelete() {
	if (this->currentLayerID & 1) {
		dRandom_c::SetSeed(0);
	}
	if (this->currentLayerID & 2) {
		dRandom_c::SetAltSeed(0);
	}
	return true;
}




