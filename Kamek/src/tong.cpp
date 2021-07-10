//#include <common.h>
//#include <game.h>
//#include <g3dhax.h>
//#include <sfx.h>
//#include <stage.h>
//#include <profile.h>
//#include "boss.h"

//#include "TikiTongBody.h"

const char* TikiTongList [] = { "Tong", NULL };

class daTikiTongBody_c;

class daTikiTongHand_c : public dEn_c {
public:

	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	static dActor_c* build();

	void updateModelMatrices();

	//void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther);

	//bool collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat5_Mario(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat8_FencePunch(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat11_PipeCannon(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther);

	daTikiTongBody_c* body;
	
	Vec2 moveGoal;
	Vec2 moveStep;
	S16Vec moveGoalRotation;
	S16Vec moveStepRotation;
	int moveStepCount;
	
	dStateBase_c* nextState;
	
	mHeapAllocator_c allocator;
	m3d::mdl_c bodyModel;
	m3d::anmChr_c chrAnimation;

	USING_STATES(daTikiTongHand_c);
	DECLARE_STATE(Wait);
	DECLARE_STATE(MoveTo);
};

CREATE_STATE(daTikiTongHand_c, Wait);
CREATE_STATE(daTikiTongHand_c, MoveTo);

dActor_c* daTikiTongHand_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daTikiTongHand_c));
	return new(buffer) daTikiTongHand_c;
}

const SpriteData TikiTongHandSpriteData = { ProfileId::TikiTongHand, 0, 0, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile TikiTongHandProfile(&daTikiTongHand_c::build, SpriteId::TikiTongHand, TikiTongHandSpriteData, ProfileId::TikiTongHand, ProfileId::TikiTongHand, "TikiTongHand", TikiTongList);



class daTikiTongCrown_c : public dEn_c {
public:

	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	static dActor_c* build();

	void updateModelMatrices();

	//void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther);

	//bool collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat5_Mario(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat8_FencePunch(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat11_PipeCannon(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther);

	daTikiTongBody_c* body;

	mHeapAllocator_c allocator;
	m3d::mdl_c bodyModel;
	m3d::anmChr_c chrAnimation;

	USING_STATES(daTikiTongCrown_c);
	DECLARE_STATE(Wait);
};

CREATE_STATE(daTikiTongCrown_c, Wait);

dActor_c* daTikiTongCrown_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daTikiTongCrown_c));
	return new(buffer) daTikiTongCrown_c;
}

const SpriteData TikiTongCrownSpriteData = { ProfileId::TikiTongCrown, 0, 0, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile TikiTongCrownProfile(&daTikiTongCrown_c::build, SpriteId::TikiTongCrown, TikiTongCrownSpriteData, ProfileId::TikiTongCrown, ProfileId::TikiTongCrown, "TikiTongCrown", TikiTongList);



class daTikiTongBody_c : public dEn_c {
public:

	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	static dActor_c* build();

	void updateModelMatrices();

	//void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther);

	//bool collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat5_Mario(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat8_FencePunch(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat11_PipeCannon(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther);

	daTikiTongHand_c* leftHand;
	daTikiTongHand_c* rightHand;
	daTikiTongCrown_c* crown;
	
	
	mHeapAllocator_c allocator;
	m3d::mdl_c bodyModel;
	m3d::anmChr_c chrAnimation;

	USING_STATES(daTikiTongBody_c);
	DECLARE_STATE(Wait);
};

CREATE_STATE(daTikiTongBody_c, Wait);




dActor_c* daTikiTongBody_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daTikiTongBody_c));
	return new(buffer) daTikiTongBody_c;
}

const SpriteData TikiTongBodySpriteData = { ProfileId::TikiTongBody, 0, 0, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile TikiTongBodyProfile(&daTikiTongBody_c::build, SpriteId::TikiTongBody, TikiTongBodySpriteData, ProfileId::TikiTongBody, ProfileId::TikiTongBody, "TikiTongBody", TikiTongList);


int daTikiTongBody_c::onCreate() {
	this->deleteForever = true;
	
	OSReport("Tiki 1\n");
	
	allocator.link(-1, GameHeaps[0], 0, 0x20);
	nw4r::g3d::ResFile rf(getResource("Tong", "g3d/Tong.brres"));
	bodyModel.setup(rf.GetResMdl("tong"), &allocator, 0x224, 1, 0);
	//this->chrAnimation.setup(mdl, anmChr, &this->allocator, 0);
	allocator.unlink();
	OSReport("Tiki 2\n");

	ActivePhysics::Info HitMeBaby;
	HitMeBaby.xDistToCenter = 0.0;
	HitMeBaby.yDistToCenter = 0.0;
	HitMeBaby.xDistToEdge = 12.0;
	HitMeBaby.xDistToEdge = 0.0;
	HitMeBaby.yDistToEdge = 15.0;
	HitMeBaby.category1 = 0x5;
	HitMeBaby.category2 = 0x0;
	HitMeBaby.bitfield1 = 0x4F;
	HitMeBaby.bitfield2 = 0xFFFFFFFF;
	HitMeBaby.unkShort1C = 0;
	HitMeBaby.callback = &dEn_c::collisionCallback;
	this->aPhysics.initWithStruct(this, &HitMeBaby);
	this->aPhysics.addToList();

	doStateChange(&StateID_Wait);

	this->scale = (Vec){0.5f, 0.5f, 0.5f};
	
	leftHand = (daTikiTongHand_c*)CreateActor(TikiTongHand, 0x00000000, this->pos, 0, 0);
	rightHand = (daTikiTongHand_c*)CreateActor(TikiTongHand, 0x80000000, this->pos, 0, 0);
	crown = (daTikiTongCrown_c*)CreateActor(TikiTongCrown, 0x00000000, this->pos, 0, 0);
	leftHand->body = this;
	rightHand->body = this;
	crown->body = this;

	Vec2 goal;
	goal.x = this->pos.x - 160;
	goal.y = this->pos.y - 40;
	leftHand->moveGoal = goal;
	leftHand->moveStepCount = 180;
	leftHand->moveGoalRotation = (S16Vec){-0x8000, 0, 0x8000};
	leftHand->nextState = &daTikiTongHand_c::StateID_Wait;
	leftHand->doStateChange(&daTikiTongHand_c::StateID_MoveTo);

	OSReport("Tiki 3\n");

	return true;
}

int daTikiTongBody_c::onExecute() {
	acState.execute();

	updateModelMatrices();
	bodyModel._vf1C();

	return true;
}

int daTikiTongBody_c::onDelete() {
	return true;
}

int daTikiTongBody_c::onDraw() {
	bodyModel.scheduleForDrawing();

	return true;
}

void daTikiTongBody_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}


void daTikiTongBody_c::beginState_Wait() {}
void daTikiTongBody_c::executeState_Wait() {
	this->pos.x += 2;
}
void daTikiTongBody_c::endState_Wait() {}


/*===========================================================================*/


int daTikiTongHand_c::onCreate() {
	this->deleteForever = true;
	
	OSReport("Tiki 1\n");
	
	allocator.link(-1, GameHeaps[0], 0, 0x20);
	nw4r::g3d::ResFile rf(getResource("Tong", this->settings >> 31 & 1 ? "g3d/LeftHand.brres" : "g3d/RightHand.brres"));
	bodyModel.setup(rf.GetResMdl("hand"), &allocator, 0x224, 1, 0);
	//this->chrAnimation.setup(mdl, anmChr, &this->allocator, 0);
	allocator.unlink();
	OSReport("Tiki 2\n");

	ActivePhysics::Info HitMeBaby;
	HitMeBaby.xDistToCenter = 0.0;
	HitMeBaby.yDistToCenter = 0.0;
	HitMeBaby.xDistToEdge = 12.0;
	HitMeBaby.xDistToEdge = 0.0;
	HitMeBaby.yDistToEdge = 15.0;
	HitMeBaby.category1 = 0x5;
	HitMeBaby.category2 = 0x0;
	HitMeBaby.bitfield1 = 0x4F;
	HitMeBaby.bitfield2 = 0xFFFFFFFF;
	HitMeBaby.unkShort1C = 0;
	HitMeBaby.callback = &dEn_c::collisionCallback;
	this->aPhysics.initWithStruct(this, &HitMeBaby);
	this->aPhysics.addToList();

	doStateChange(&StateID_Wait);

	this->scale = (Vec){0.5f, 0.5f, 0.5f};
	

	OSReport("Tiki 3\n");

	return true;
}

int daTikiTongHand_c::onExecute() {
	acState.execute();

	updateModelMatrices();
	bodyModel._vf1C();
	

	return true;
}

int daTikiTongHand_c::onDelete() {
	return true;
}

int daTikiTongHand_c::onDraw() {
	bodyModel.scheduleForDrawing();

	return true;
}

void daTikiTongHand_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}


void daTikiTongHand_c::beginState_Wait() {}
void daTikiTongHand_c::executeState_Wait() {
	this->pos = this->body->pos;
}
void daTikiTongHand_c::endState_Wait() {}


void daTikiTongHand_c::beginState_MoveTo() {				//moveGoal, moveStepCount and nextState have to be set before starting this state
	float xDiff = this->moveGoal.x - this->pos.x;
	float yDiff = this->moveGoal.y - this->pos.y;
	float length = sqrtf((xDiff*xDiff) + (yDiff*yDiff));
	float stepLength = length / this->moveStepCount;
	this->moveStep = (Vec2){(xDiff/length)*stepLength, (yDiff/length)*stepLength};
	
	s16 xRotDiff = this->moveGoalRotation.x - this->rot.x;
	s16 yRotDiff = this->moveGoalRotation.y - this->rot.y;
	s16 zRotDiff = this->moveGoalRotation.z - this->rot.z;
	
	this->moveStepRotation = (S16Vec){xRotDiff / moveStepCount, yRotDiff / moveStepCount, zRotDiff / moveStepCount};
}
void daTikiTongHand_c::executeState_MoveTo() {
	if(this->moveStepCount > 0) {
		this->pos.x += this->moveStep.x;
		this->pos.y += this->moveStep.y;
		
		this->rot.x += this->moveStepRotation.x;
		this->rot.y += this->moveStepRotation.y;
		this->rot.z += this->moveStepRotation.z;
		
		this->moveStepCount--;
	} else {
		doStateChange(this->nextState);
	}
}
void daTikiTongHand_c::endState_MoveTo() {
	this->rot = this->moveGoalRotation;		//to fix possible rounding errors
}


/*===========================================================================*/


int daTikiTongCrown_c::onCreate() {
	this->deleteForever = true;
	
	OSReport("Tiki 1\n");
	
	allocator.link(-1, GameHeaps[0], 0, 0x20);
	nw4r::g3d::ResFile rf(getResource("Tong", "g3d/Crown.brres"));
	bodyModel.setup(rf.GetResMdl("crown"), &allocator, 0x224, 1, 0);
	//this->chrAnimation.setup(mdl, anmChr, &this->allocator, 0);
	allocator.unlink();
	OSReport("Tiki 2\n");

	ActivePhysics::Info HitMeBaby;
	HitMeBaby.xDistToCenter = 0.0;
	HitMeBaby.yDistToCenter = 0.0;
	HitMeBaby.xDistToEdge = 12.0;
	HitMeBaby.xDistToEdge = 0.0;
	HitMeBaby.yDistToEdge = 15.0;
	HitMeBaby.category1 = 0x5;
	HitMeBaby.category2 = 0x0;
	HitMeBaby.bitfield1 = 0x4F;
	HitMeBaby.bitfield2 = 0xFFFFFFFF;
	HitMeBaby.unkShort1C = 0;
	HitMeBaby.callback = &dEn_c::collisionCallback;
	this->aPhysics.initWithStruct(this, &HitMeBaby);
	this->aPhysics.addToList();

	doStateChange(&StateID_Wait);

	this->scale = (Vec){0.5f, 0.5f, 0.5f};
	

	OSReport("Tiki 3\n");

	return true;
}

int daTikiTongCrown_c::onExecute() {
	acState.execute();

	updateModelMatrices();
	bodyModel._vf1C();
	

	return true;
}

int daTikiTongCrown_c::onDelete() {
	return true;
}

int daTikiTongCrown_c::onDraw() {
	bodyModel.scheduleForDrawing();

	return true;
}

void daTikiTongCrown_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}


void daTikiTongCrown_c::beginState_Wait() {}
void daTikiTongCrown_c::executeState_Wait() {
	this->pos = this->body->pos;
}
void daTikiTongCrown_c::endState_Wait() {}

