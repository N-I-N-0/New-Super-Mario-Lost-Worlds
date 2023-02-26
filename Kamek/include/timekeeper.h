#ifndef __KAMEK_TIMEKEEPER_H
#define __KAMEK_TIMEKEEPER_H

class TimeKeeper {                       //The TimeKeeper class, handles things relited to the level's timer
	public:
		u32 *vtable;
		u32 timePlusFFFTimes40000;
		u16 time;
		u8 ambushMaybe;
		u8 timeLessThan100;
		u8 paused;
		
		static TimeKeeper* instance;
		void setTime(int time);
};

class Zone {                             //The Zone class, handles things related to a zone (duh)
    public:
        u16 x;
        u16 y;
        u16 w;
        u16 h;
        u16 objShad;
        u16 terShad;
        u8 id;
        u8 bnd_id;
        u8 scroll;
        u8 zoom;
        u8 field_10;
        u8 light;
        u8 fg_id;
        u8 bg_id;
        u8 mp_bias;
        u8 field_15;
        u8 music;
        u8 audio;
};


class Stage32C {                         //The Stage32C class, handles much random stuff
    public:
        u32 bulletData1[32];
        u32 bulletData2[32];
        u32 enemyCombos[4];
        u32 somethingAboutHatenaBalloons;
        u32 redCoinCount[4];
        u32 field_124;
        u32 field_128;
        u32 field_12C;
        u32 field_130;
        u32 greenCoinsCollected;
        u32 hasKilledEnemyThisTick_maybe;
        u16 booID;
        u8 _13E;
        u8 _13F;
        u32 bigBooID;
        u32 homingBanzaiID;
        u16 bulletBillCount;
        u8 _14A;
        u8 _14B;
        u32 bombCount;
        u32 goombaCount;
        u32 enemyKillCounter_maybe;
        u32 a_counter;
        u32 setToOneByBossDemoOrSomething;
        u32 aboutMortonBigPile;
        u32 somethingAboutPunchingChainlinkFence;
        u32 currentBigHanaMgr;
        u8 _16C;
        u8 _16D;
        u8 _16E;
        u8 _16F;
        u8 penguinCount;
        u8 _171;
        u8 _172;
        u8 _173;
        u32 pokeyTimer;
};

/*************************************************************************************************************************/
/*The MicroGoombaManager, dEffectExplosionMgr_c, dTimerMgr_c, dBlockMgr_c, StageF70, StageF74Sub and the StageF74 classes*/
/*************************************are here only to make the dScStage_c class work*************************************/
/*************************************************************************************************************************/

class MicroGoombaManager {
    public:
        u32 storedActorIDs[100];
        u8 storedGroupIDs[100];
        u8 generatedNumberFlag[4];
        u16 generatedNumbers[4];
};

class dEffectExplosionMgr_c {
    public:
        u16 field_0;
};

class dTimerMgr_c {
    public:
        u16 field_0;
};

class dBlockMgr_c {
    public:
        u32 areDonutLiftsDisabled;
};

class StageF70 {
    public:
        u16 field_0;
};

class StageF74Sub {
    public:
        u16 field_0;
        u8 _2;
        u8 _3;
        float field_4;
        float field_8;
        float field_C;
        float field_10;
        float field_14;
        float field_18;
        float field_1C;
        float field_20;
};

class StageF74 {
    public:
        StageF74Sub field_0[16];
};

class dScStage_c : public dScene_c {     //The dScStage_c class, handles things related to a stage
    public:
        u32 runningFrameCount_probably;
        FunctionChain chain;
        Stage80 field_80;
        StageC4 field_C4;
        StageE4 field_E4;
        Stage32C field_32C; //bleh
        MicroGoombaManager microGoombaManager; //bleh
        dWaterManager_c waterManager;
        dEffectExplosionMgr_c effectExplosionManager; //bleh
        dTimerMgr_c timerMgr; //bleh
        dBlockMgr_c blockMgr; //bleh
        StageF70 field_F70; //bleh
        StageF74 field_F74; //bleh
        u8 _11B4;
        u8 _11B5;
        u8 _11B6;
        u8 _11B7;
        u8 _11B8;
        u8 _11B9;
        u8 _11BA;
        u8 _11BB;
        u8 _11BC;
        u8 _11BD;
        u8 _11BE;
        u8 _11BF;
        u8 _11C0;
        u8 _11C1;
        u8 _11C2;
        u8 _11C3;
        u8 _11C4;
        u8 _11C5;
        u8 _11C6;
        u8 _11C7;
        u8 _11C8;
        u8 _11C9;
        u8 _11CA;
        u8 _11CB;
        u8 _11CC;
        u8 _11CD;
        u8 _11CE;
        u8 _11CF;
        u8 _11D0;
        u8 _11D1;
        u8 _11D2;
        u8 _11D3;
        u32 ptrToGameDisplay;
        u32 ptrToGoalManager;
        u32 ptrToSmallScoreManager;
        u32 ptrToFukidashiManager;
        u32 ptrToCourseTimeUp;
        u32 ptrToMiniGameCannon;
        u32 ptrToMiniGameWire;
        u32 ptrToModelPlayManager;
        u32 ptrToMessageWindow;
        u32 ptrToModelPlayGuide;
        u32 ptrToStaffCreditScore;
        u32 ptrToTheEnd;
        u32 ptrToYesNoWindow;
        u8 _1208;
        u8 _1209;
        u8 _120A;
        u8 _120B;
        u8 curWorld;
        u8 curLevel;
        u8 curArea;
        u8 curZone;
        u8 curLayer;
        u8 curEntrance;
        u8 field_1212;
        u8 field_1213;
        u32 uselessPTMFIndex;

        static u32 exeFrame;

        int onExecute_orig(); //80925A60
        int newOnExecute();
};







#endif
