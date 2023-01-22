#ifndef __CHIMPNPC_H
#define __CHIMPNPC_H

#include <common.h>
#include <game.h>
#include <g3dhax.h>
#include <sfx.h>

class dChimpTBMgr_c; // Forward declaration

class dChimpNPC_c : public dEn_c {
public:
	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	mHeapAllocator_c allocator;
	nw4r::g3d::ResFile resFile;
	m3d::mdl_c bodyModel;
	m3d::anmChr_c chrAnimation;

	dChimpTBMgr_c* textBoxMgr;

	bool isSpinning;
	bool isSurprised;
	bool isUntalking;
	bool doneReacting;
	bool doneRotating;

	u32 spinTimer;

	bool waitForTopOfTile[4];
	dAcPy_c *players[4];

	static dChimpNPC_c *build();

	void bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate);
	void updateModelMatrices();
	void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther);

	// bool collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther);
	// bool collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther);
	// bool collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther);
	// bool collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther);

	// bool collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther);
	// bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther);
	// bool collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther);
	// bool collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther);
	// bool collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther);
	// bool collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther);

};

class dChimpTBMgr_c : public dStageActor_c {
public:
	dChimpTBMgr_c() : acState(this, &StateID_Init) { }

	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	m2d::EmbedLayout_c layout;
	dStateWrapper_c<dChimpTBMgr_c> acState;

	bool layoutLoaded, isVisible;

	dChimpNPC_c *parent;
	u32 timer;
	u32 textAnim;

	nw4r::lyt::TextBox *T_text[2];

	enum Animation {
		SHOW_ALL = 0,
		TEXT_NEXT1 = 1,
		TEXT_NEXT2 = 2,
		TEXT_BACK1 = 3,
		TEXT_BACK2 = 4,
		HIDE_ALL = 5,
		BUTTON_APPEAR = 6,
		BUTTON_IDLE = 7,
		BUTTON_DISAPPEAR = 8,
	};

	enum Settings {
		FIRST_TIME = 0,
		REGULAR_ITEMBOWLING = 1,
		REGULAR_PENGUINSLIDE = 2,
		REGULAR_SANDSLIDE = 3,
		REGULAR_GEARMO = 4,
		REGULAR_BULLETBILLPKR = 5,
		FIRST_ITEMBOWLING = 6,
		FIRST_PENGUINSLIDE = 7,
		FIRST_SANDSLIDE = 8,
		FIRST_GEARMO = 9,
		FIRST_BULLETBILLPKR = 10,
		SETTINGS_COUNT,
	};


	static dChimpTBMgr_c *build();
	static dChimpTBMgr_c *instance;

	USING_STATES(dChimpTBMgr_c);
	DECLARE_STATE(Init);
	DECLARE_STATE(WaitForButton);
	DECLARE_STATE(WaitForInput);
	DECLARE_STATE(WaitForText1);
	DECLARE_STATE(WaitForText2);
	DECLARE_STATE(Show);
	DECLARE_STATE(End);
};


#endif