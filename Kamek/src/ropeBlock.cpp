#include <common.h>
#include <game.h>
#include <g3dhax.h>

#include "boss.h"

const char* ropeBlockFileList [] = { "rope", NULL };

class daRopeBlock_c : public dEn_c {
public:
	int onCreate();
	int onDelete();
	int onExecute();
	int onDraw();

	mHeapAllocator_c allocator;
	nw4r::g3d::ResFile resFile;
	m3d::mdl_c bodyModel;
	m3d::anmChr_c chrAnimation;


	u64 eventFlag;

	u32 hookNodeID;

	dActor_c* coin;
	
	static dActor_c *build();

	void bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate);
	void updateModelMatrices();

	void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther);
	
	USING_STATES(daRopeBlock_c);
	DECLARE_STATE(Wait);
	DECLARE_STATE(Fall);
	DECLARE_STATE(JumpedAgainst);
	DECLARE_STATE(Fixated);
};

dActor_c *daRopeBlock_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daRopeBlock_c));
	return new(buffer) daRopeBlock_c;
}

const SpriteData RopeBlockSpriteData = { ProfileId::RopeBlock, 0, 0, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile RopeBlockProfile(&daRopeBlock_c::build, SpriteId::RopeBlock, &RopeBlockSpriteData, ProfileId::RopeBlock, ProfileId::RopeBlock, "Rope Block", ropeBlockFileList);




CREATE_STATE(daRopeBlock_c, Wait);
CREATE_STATE(daRopeBlock_c, Fall);
CREATE_STATE(daRopeBlock_c, JumpedAgainst);
CREATE_STATE(daRopeBlock_c, Fixated);






void daRopeBlock_c::playerCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	//DamagePlayer(this, apThis, apOther);

	//lets use this to make the rope jump again?
}







int daRopeBlock_c::onCreate() {
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	this->resFile.data = getResource("rope", "g3d/rope.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("rope");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	//SetupTextures_Map(&bodyModel, 0);
	
	nw4r::g3d::ResNode hook = mdl.GetResNode("Bone001");
	hookNodeID = hook.GetID();
	
	//nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr("test_short_12deg");
	//this->chrAnimation.setup(mdl, anmChr, &this->allocator, 0);

	allocator.unlink();
	
	//bindAnimChr_and_setUpdateRate("test_short_12deg", 1, 0.0, 1.0);

	this->scale = (Vec){10, 10, 10};


	char eventNum	= (this->settings >> 16) & 0xFF;

	this->eventFlag = (u64)1 << (eventNum - 1);


	
	doStateChange(&StateID_Wait);

	
	coin = (dActor_c*)CreateActor(EN_COIN, 0, this->pos, 0, 0);
	
	// this->onExecute();
	return true;
}

int daRopeBlock_c::onDelete() {
	return true;
}

int daRopeBlock_c::onExecute() {
	acState.execute();
	updateModelMatrices();
	bodyModel._vf1C();
	
	Vec ropePos;
	bodyModel.getNodeWorldMtxMultVecZero(hookNodeID, &ropePos);
	
	this->coin->pos = ropePos;

	
	/*if(this->chrAnimation.isAnimationDone()) {
		this->chrAnimation.setCurrentFrame(0.0);
	}*/
		
	return true;
}

int daRopeBlock_c::onDraw() {
	bodyModel.scheduleForDrawing();

	return true;
}


void daRopeBlock_c::updateModelMatrices() {
	// This won't work with wrap because I'm lazy.
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

void daRopeBlock_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate) {
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr(name);
	this->chrAnimation.bind(&this->bodyModel, anmChr, unk);
	this->bodyModel.bindAnim(&this->chrAnimation, unk2);
	this->chrAnimation.setUpdateRate(rate);
}




void daRopeBlock_c::beginState_Wait() { }
void daRopeBlock_c::executeState_Wait() { }
void daRopeBlock_c::endState_Wait() { }


void daRopeBlock_c::beginState_Fall() { }
void daRopeBlock_c::executeState_Fall() { }
void daRopeBlock_c::endState_Fall() { }


void daRopeBlock_c::beginState_JumpedAgainst() { }
void daRopeBlock_c::executeState_JumpedAgainst() { }
void daRopeBlock_c::endState_JumpedAgainst() { }


void daRopeBlock_c::beginState_Fixated() { }
void daRopeBlock_c::executeState_Fixated() { }
void daRopeBlock_c::endState_Fixated() { }


