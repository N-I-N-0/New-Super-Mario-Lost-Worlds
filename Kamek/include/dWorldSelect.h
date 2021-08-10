#ifndef __WORLDSELECT
#define __WORLDSELECT

class daWMIsland_c : public dActor_c {
public:
    u8 _125, _126, _127;
    mAllocator_c allocator;    // 0x128
    m3d::mdl_c model;        // 0x144
    m3d::fanm_c anmChr;        // 0x184
    u8 unk1[132];            // 0x1BC
    u32 resFile;            // 0x240
    u8 unk2[56];            // 0x244
    u32 islandID;            // 0x27C
    u8 unk3[84];            // 0x280
    //Total Size: 0x2D4
};

class dWorldSelect_c : public dBase_c {
public:
    u32 field_70;                                // 0x70
    m2d::EmbedLayout_c layout;                    // 0x74
    dStateWrapper_c<dWorldSelect_c> acState;    // 0x20C
    daWMIsland_c *islands[9];                    // 0x248
    u8 unk1[0x10];                                // 0x26C
    u32 SelectedIsland;                            // 0x27C
    u8 unk2[0x1C];                                // 0x280
    int timeToWaitUntilWipe;                    // 0x29C
    u8 unk3[8];                                    // 0x2A0
    u8 world1Unlocked;                            // 0x2A8		//probably bool worldsUnlocked[9]; (?)
    u8 world2Unlocked;                            // 0x2A9
    u8 world3Unlocked;                            // 0x2AA
    u8 world4Unlocked;                            // 0x2AB
    u8 world5Unlocked;                            // 0x2AC
    u8 world6Unlocked;                            // 0x2AD
    u8 world7Unlocked;                            // 0x2AE
    u8 world8Unlocked;                            // 0x2AF
    u8 world9Unlocked;                            // 0x2B0
    u8 unk4[11];                                // 0x2B1
    
	//size at 0x80928818: 0x2BC
    //Total Size: 0x2BC
};

#endif