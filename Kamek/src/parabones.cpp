#include <common.h>
#include <game.h>
#include <profile.h>
#include "path.h"

const char* paraBonesFileList[] = { "karon", "wing", 0 };


class daEnParaBones_c : public dEnPath_c {
public:
	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	mHeapAllocator_c allocator;
	nw4r::g3d::ResFile resFile;
	nw4r::g3d::ResFile resFileAnim;
	m3d::mdl_c bodyModel;

	nw4r::g3d::ResAnmTexPat anmPat;

	m3d::anmChr_c chrAnimation;
	m3d::anmTexPat_c patAnimation;

	bool leftRight;

	bool facingRight;
	u8 wSpeed;
	u8 wDistance;

	int stepCount;

	bool ranOnce;
	
	dEn_c* dryBone;

	static dActor_c* build();

	void bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate);

	void updateModelMatrices();
	void playerCollision(ActivePhysics* apThis, ActivePhysics* apOther);
	void yoshiCollision(ActivePhysics* apThis, ActivePhysics* apOther);

	bool collisionCat7_GroundPound(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat7_GroundPoundYoshi(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCatD_Drill(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCatA_PenguinMario(ActivePhysics* apThis, ActivePhysics* apOther);

	bool collisionCat1_Fireball_E_Explosion(ActivePhysics* apThis, ActivePhysics* apOther);
	//bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat9_RollingObject(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat13_Hammer(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat14_YoshiFire(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat3_StarPower(ActivePhysics* apThis, ActivePhysics* apOther);

	//setDeathInfo_IceBreak -> what should be done when in an iceblock and crashes a wall -> normally the sprite should die
	void _vf148();
	//setDeathInfo_IceVanish -> what should be done when collision with an iceblock thrown by Mario -> normally the sprite should die
	void _vf14C();
	//create an ice block when collided with Iceball
	bool CreateIceActors();

	//void addScoreWhenHit(void* other);
};

const SpriteData ParaBonesSpriteData = { ProfileId::ParaBones, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile ParaBonesProfile(&daEnParaBones_c::build, SpriteId::ParaBones, ParaBonesSpriteData, ProfileId::ParaBones, ProfileId::ParaBones, "ParaBones", paraBonesFileList);



u8 hijackMusicWithSongName(const char* songName, int themeID, bool hasFast, int channelCount, int trackCount, int* wantRealStreamID);

void daEnParaBones_c::playerCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	char hitType;
	hitType = usedForDeterminingStatePress_or_playerCollision(this, apThis, apOther, 0);

	if(hitType == 1) {	// regular jump
		apOther->someFlagByte |= 2;
		this->Delete(1);
	}
	else if(hitType == 3) {	// spinning jump or whatever?
		apOther->someFlagByte |= 2;
		this->Delete(1);
	}
	else if(hitType == 0) {
		EN_LandbarrelPlayerCollision(this, apThis, apOther);
		DamagePlayer(this, apThis, apOther);
	} else {
		DamagePlayer(this, apThis, apOther);
	}
}
void daEnParaBones_c::yoshiCollision(ActivePhysics* apThis, ActivePhysics* apOther) {}
bool daEnParaBones_c::collisionCat7_GroundPound(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->Delete(1);
	return true;
}
bool daEnParaBones_c::collisionCat7_GroundPoundYoshi(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->Delete(1);
	return true;
}
bool daEnParaBones_c::collisionCatD_Drill(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->Delete(1);
	return true;
}
bool daEnParaBones_c::collisionCatA_PenguinMario(ActivePhysics* apThis, ActivePhysics* apOther) {
	if ((this->facingRight && apOther->owner->pos.x < this->pos.x) || (!this->facingRight && apOther->owner->pos.x > this->pos.x))
	{
		this->Delete(1);
		return true;
	}
	return false;
}

bool daEnParaBones_c::collisionCat1_Fireball_E_Explosion(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->Delete(1);
	return true;
}
/*bool daEnParaBones_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics* apThis, ActivePhysics* apOther) {

	return false;
}*/
bool daEnParaBones_c::collisionCat9_RollingObject(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnParaBones_c::collisionCat13_Hammer(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->Delete(1);
	return true;
}
bool daEnParaBones_c::collisionCat14_YoshiFire(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->Delete(1);
	return true;
}

bool daEnParaBones_c::collisionCat3_StarPower(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->Delete(1);
	return true;
}


void daEnParaBones_c::_vf148() {
	dEn_c::_vf148();
}

void daEnParaBones_c::_vf14C() {
	dEn_c::_vf14C();
}

//this does weird shit, but it creates the iceblock around it
bool daEnParaBones_c::CreateIceActors() {
	struct DoSomethingCool my_struct = { 0, this->pos, {1.5, 2.0, 1.8}, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
	this->frzMgr.Create_ICEACTORs((void*)&my_struct, 1);
	__destroy_arr((void*)&my_struct, sub_80024C20, 0x3C, 1);
	//this->chrAnimation.setCurrentFrame(0.0);
	this->chrAnimation.setUpdateRate(0.0);
	//this->frozen = true;
	return true;
}


dActor_c* daEnParaBones_c::build() {
	void* buffer = AllocFromGameHeap1(sizeof(daEnParaBones_c));
	daEnParaBones_c* c = new(buffer) daEnParaBones_c;

	return c;
}


extern int getNybbleValue(u32 settings, int fromNybble, int toNybble);

void daEnParaBones_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate)
{
	nw4r::g3d::ResAnmChr anmChr = this->resFileAnim.GetResAnmChr(name);
	this->chrAnimation.bind(&this->bodyModel, anmChr, unk);
	this->bodyModel.bindAnim(&this->chrAnimation, unk2);
	this->chrAnimation.setUpdateRate(rate);
}

int daEnParaBones_c::onCreate() {
	if(!this->ranOnce) {
		this->ranOnce = true;
		return false;
	}
	
	this->deleteForever = false;

	dryBone = (dEn_c*)CreateActor(EN_KARON, 0, &pos, 0, this->currentLayerID);


	// Model creation	
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	this->resFile.data = getResource("wing", "g3d/wing.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("wing");
	bodyModel.setup(mdl, &allocator, 0x227, 1, 0);
	SetupTextures_Player(&bodyModel, 0);

	this->resFileAnim.data = getResource("wing", "g3d/wing.brres");
	nw4r::g3d::ResAnmChr anmChr = this->resFileAnim.GetResAnmChr("wing_pata");
	this->chrAnimation.setup(mdl, anmChr, &this->allocator, 0);

	allocator.unlink();

	this->leftRight = settings >> 31 & 1;

	this->facingRight = settings >> 30 & 1;
	this->wDistance = settings >> 20 & 0b11111111;
	this->wSpeed = settings >> 16 & 0b1111;

	this->scale = (Vec){1, 1, 1};
	
	bindAnimChr_and_setUpdateRate("wing_pata", 1, 0.0, 1.0);

	if(this->leftRight) {
		doStateChange(&StateID_Wait);
	} else {
		beginState_Init();
		if (this->rotateNext) {
			if (this->stepVector.x > 0) {
				this->rot.y = 0x4000;
			} else {
				this->rot.y = -0x4000;
			}
		} else if (this->rotate0XNext) {
			this->rot.y = 0x0;
		}
		executeState_Init();
		doStateChange(&StateID_FollowPath);
	}
	
	
	this->onExecute();
	return true;
}


int daEnParaBones_c::onDelete() {
	nw4r::snd::SoundHandle handle;
	PlaySoundWithFunctionB4(SoundRelatedClass, &handle, SE_EMY_DOWN, 1);
	StageE4::instance->spawnCoinJump(pos, 0, 1, 0);
	return true;
}

int daEnParaBones_c::onDraw() {
	bodyModel.scheduleForDrawing();
	return true;
}


void daEnParaBones_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y+8, pos.z);
	matrix.applyRotationYXZ(&dryBone->rot.x, &dryBone->rot.y, &dryBone->rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

extern dStateBase_c daEnKanon_c__StateID_Walk;

int daEnParaBones_c::onExecute() {
	bodyModel._vf1C();
	updateModelMatrices();

	//OSReport("fr: %d, wD: %d, wS: %d, stepCount: %d\n", this->facingRight, this->wDistance, this->wSpeed, this->stepCount);


	acState.execute();
	
	if(dryBone->acState.getCurrentState() != &daEnKanon_c__StateID_Walk) {
		this->Delete(1);
	}
	
	/*if (this->stepsDone == this->stepCount) {
		if (this->rotateNext) {
			if (this->stepVector.x > 0) {
				dryBone->rot.y = 0x4000;
			} else {
				dryBone->rot.y = -0x4000;
			}
		} else if (this->rotate0XNext) {
			dryBone->rot.y = 0x0;
		}
	}*/
	
	dryBone->pos = this->pos;
	
	if (this->chrAnimation.isAnimationDone()) {
		this->chrAnimation.setCurrentFrame(0.0);
	}

	return true;
}