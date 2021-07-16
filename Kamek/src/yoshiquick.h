#ifndef QUICKYOSHI_H
#define QUICKYOSHI_H 


class daYoshi2_c : public daPlBase_c {	//Yoshi's class, still incomplete because of my lazyness:
public:
	u32 modelHandler;
	u32 wut[13];						//-Things forgot by modelHandler or daPlBase_c, probably ?
	u32 newdata0[18];					//-Padding
	u32 eatenID;
	u32 newdata1[7];					//-Padding
	u8 someDir;
	u8 somePlayerID;
	u8 field_1542;
	u8 field_1543;
	u32 newdata2[5];					//-Padding
	u32 someActivePhysicsPointer;
	float field_155C;
	u32 newdata3[3];					//-Padding
	mEf::es2 effect;
	mEf::es2 effect2;
	u32 field_17BC;
	u32 _17C0;
	u32 _17C4;
	u32 _17C8;
	//Below was added by expanding the class
	bool isTimerEnabled;
	int swallowTimer;
	bool doOneTime;
	bool doneGP;
	u32 wingID;

	void doSwallow();

	int onExecute_orig();
	int newOnExecute();

	void endState_HipAttack_orig();
	void newEndState_HipAttack();
};

#endif