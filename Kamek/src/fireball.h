#ifndef __FIREBALL_H
#define __FIREBALL_H

#include <common.h>
#include <game.h>
#include <g3dhax.h>
#include <sfx.h>
#include <actors.h>


class daFireBall_Base_c : public dActorState_c {
public:
	mHeapAllocator_c allocator;
	StageActorLight light;

	int didSplash;
	int killCountdownToDestroy;
	int liquidStat;
	int field_414;
	float liquidHeight;
	Vec startVec;
	int field_428;
	mEf::es2 effect;

	int onCreate(); //0x80040960
	int onDelete(); //0x80040DD0 (it's only a li r3, 1)
	int beforeExecute(); //0x80040B00
	int onExecute(); //0x80040BD0
	int onDraw(); //0x80040D90
	void willBeDeleted(); //0x80040DC0 (it's only a blr)


	virtual void eatIn(); //0x80040F10
	virtual void disableEatIn(); //0x800413F0

	virtual void Move_Begin();
	virtual void Move_Execute();
	virtual void Move_End();
	virtual void Kill_Begin();
	virtual void Kill_Execute();
	virtual void Kill_End();
	virtual void eatIn_Begin();
	virtual void eatIn_Execute();
	virtual void eatIn_End();
	virtual void eatNow_Begin();
	virtual void eatNow_Execute();
	virtual void eatNow_End();

	void calledInOnCreate_b(); //0x80040AE0 (it's only a li r3, 1)
	void updateSpawnLimits(); //0x80040AD0 (it's only a li r3, 1)
	void calledInOnCreate_c(); //0x80040AF0 (it's only a blr)
	void setUpCollSensor(); //0x80040E70
	void setZOrder_maybe(); //0x80040D70 (it's only a blr)
	void playFireballEffect(); //0x80040D80
	void doFireSplash(); //0x80041000
	void doLavaSplash(); //0x800410F0
	void doPoisonSplash(); //0x800411D0
	void getSizeOfLight(); //0x80040FF0
	void vf138(); //0x800413E0
	void vf13C(); //0x800413D0

	void sub_80040BC0(); //0x80040BC0
	void killIfOutOfZone_maybe(); //0x80040DE0
	void playFireballHitEffect(); //0x80040EC0
	void updateLights(); //0x80040F70

	~daFireBall_Base_c();

	USING_STATES(daFireBall_Base_c);
	REF_NINTENDO_STATE(Move);
	REF_NINTENDO_STATE(Kill);
	REF_NINTENDO_STATE(EatIn);
	REF_NINTENDO_STATE(EatNow);
};


#endif

