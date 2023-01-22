#ifndef __KAMEK_BOOMERANGHAX_H
#define __KAMEK_BOOMERANGHAX_H



class dGameDisplay_c : dBase_c {
    public:
        m2d::EmbedLayout_c layout;
        mEf::es2 effect;
        u32 _330;
        u32 _334;
        u32 _338;
        u32 _33C;
        u32 _340;
        u32 _344;
        u32 _348;
        u32 _34C;
        u32 _350;
        u32 _354;
        u32 _358;
        u32 _35C;
        u32 _360;
        u32 _364;
        u32 _368;
        u32 _36C;
        u32 _370;
        u32 _374;
        u32 _378;
        u32 _37C;
        u32 _380;
        u32 _384;
        u32 _388;
        u32 _38C;
        u32 _390;
        u32 _394;
        u32 _398;
        u32 _39C;
        u32 _3A0;
        u32 _3A4;
        dStateMgr_c state;
        u32 _3CC;
        u32 _3D0;
        u32 _3D4;
        u32 _3D8;
        u32 coins;
        u32 timer;
        u32 _3E4;
        u32 score;
        u32 _3EC;
        u32 _3F0;
        u32 _3F4;
        u32 _3F8;
        u32 _3FC;
        u32 _400;
        u32 _404;
        u32 _408;
        u32 _40C;
        u32 _410;
        u32 _414;
        u32 _418;
        u32 _41C;
        u32 _420;
        u32 _424;
        u32 _428;
        u32 _42C;
        u32 _430;
        u32 mustAtLeast2ForScoreToCount;
        u32 _438;
        u32 _43C;
        u32 _440;
        u32 _444;
        u8 _448;
        u8 _449;
        u8 _44A;
        u8 _44B;
        u32 _44C;
        u8 _450;
        u8 _451;
        u8 _452;
        u8 _453;
        u8 _454;
        u8 _455;
        u8 _456;
        u8 _457;
        u32 _458;
        u32 _45C;
        u32 _460;
        u32 _464;
        u32 _468;
        u32 _46C;
        u32 _470;
        u32 _474;
        u32 _478;
        u32 _47C;
        u32 _480;
        u32 _484;
        u32 _488;
        u32 _48C;
        u32 _490;
        nw4r::lyt::Picture* p_collectOff_00;
        nw4r::lyt::Picture* p_collection_00;
        nw4r::lyt::Picture* p_collectOff_01;
        nw4r::lyt::Picture* p_collection_01;
        nw4r::lyt::Picture* p_collectOff_02;
        nw4r::lyt::Picture* p_collection_02;
        nw4r::lyt::Picture* p_marioIcon_00;
        nw4r::lyt::Picture* p_luijiIcon_00;
        nw4r::lyt::Picture* p_kinoB_00;
        nw4r::lyt::Picture* p_kinoY_00;
        u32 _4BC;
        u32 _4C0;
        u32 _4C4;
        u32 _4C8;
        u32 _4CC;
        u32 _4D0;
        u32 _4D4;
        u32 _4D8;
        nw4r::lyt::TextBox* coinBox;
        nw4r::lyt::TextBox* timerBox;
        nw4r::lyt::TextBox* scoreBox;
        u32 _4E8;
        u32 _4EC;
        u32 _4F0;
        u32 _4F4;
        u8  _4F8;
        void* StarCoin1;
        void* StarCoin2;
        void* StarCoin3;
        u8  _505;
        u8  _506;
        u8  _507;
        u8 blob1[0x51C - 0x508];
        u32 _51C;
        u8 blob2[0x5A0 - 0x508];
        u32 _5A0;
        u32 purplecoins; //New
       
        static dGameDisplay_c* instance;
        int onCreate_orig();
        int onExecute_orig();
        int doWaitCheck();
        int cleanDisplay();
        void doGlobalCoin();
        void refreshPurpleCoins(); //New
		
		USING_STATES(dGameDisplay_c);
		REF_NINTENDO_STATE(ProcGoalEnd);
};


class daGabonRock_c : dEn_c { //The daGabonRock_c class, very important.
    public:
        int onCreate_orig();
        int getsettings();
};

class daEnBomhei_c : public dEn_c {
public:
    u8 field524_from_daEnCarry_c;
    u8 _525;
    u8 _522;
    u8 _527;
    mAllocator_c allocator;
    u32 resFile;
    m3d::mdl_c model;
    m3d::fanm_c anmChr;
    m3d::banm_c anmClr;
    u32 field_5E8;
    u32 isBurning;
    u32 field_5F0;
    u8 _5F4;
    u8 _5F5;
    u8 _5F6;
    u8 _5F7;
    u32 cannonShootDirection;
    u8 _5FC;
    u8 _5FD;
    u8 _5FE;
    u8 _5FF;
    u8 _600;
    u8 _601;
    u8 _602;
    u8 _603;
    VEC3 field_604;
    u8 _610;
    u8 _611;
    u8 _612;
    u8 _613;
    u8 _614;
    u8 _615;
    u8 _616;
    u8 _617;
    u8 _618;
    u8 _619;
    u8 _61A;
    u8 _61B;
    u8 _61C;
    u8 _61D;
    u8 _61E;
    u8 _61F;
    u32 hitsRemainingBeforeCoinJumpSpawns;
    u32 holdingPlayerID;
    u32 countdown;
    u8 _62C;
    u8 _62D;
    u8 _62E;
    u8 _62F;
    u8 _630;
    u8 _631;
    u8 _632;
    u8 _633;
    u8 _634;
    u8 _635;
    u8 _636;
    u8 _637;
    mEf::es2 effect;

    // States
    USING_STATES(daEnBomhei_c);
    REF_NINTENDO_STATE(Sleep);

    int onCreate_orig();
    int newOnCreate();
    void doThrowing();
    int amIthrown;

    void playerCollision_orig(ActivePhysics *apThis, ActivePhysics *apOther);
    void newPlayerCollision(ActivePhysics *apThis, ActivePhysics *apOther);
    void spriteCollision_orig(ActivePhysics *apThis, ActivePhysics *apOther);
    void newSpriteCollision(ActivePhysics *apThis, ActivePhysics *apOther);
};

#endif