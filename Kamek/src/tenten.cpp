#include <common.h>
#include <game.h>
#include <profile.h>
#include "path.h"

const char* WingTentenFileList[] = { "TentenWing", 0 };


class daEnParaTenten_c : public dEnPath_c {
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

	static dActor_c* build();

	void bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate);

	void updateModelMatrices();
	void playerCollision(ActivePhysics* apThis, ActivePhysics* apOther);
	void yoshiCollision(ActivePhysics* apThis, ActivePhysics* apOther);

	/*bool collisionCat7_GroundPound(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat7_GroundPoundYoshi(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCatD_Drill(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCatA_PenguinMario(ActivePhysics* apThis, ActivePhysics* apOther);

	bool collisionCat1_Fireball_E_Explosion(ActivePhysics* apThis, ActivePhysics* apOther);
	//bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat9_RollingObject(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat13_Hammer(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat14_YoshiFire(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat3_StarPower(ActivePhysics* apThis, ActivePhysics* apOther);*/

	//setDeathInfo_IceBreak -> what should be done when in an iceblock and crashes a wall -> normally the sprite should die
	void _vf148();
	//setDeathInfo_IceVanish -> what should be done when collision with an iceblock thrown by Mario -> normally the sprite should die
	void _vf14C();
	//create an ice block when collided with Iceball
	bool CreateIceActors();

	USING_STATES(daEnParaTenten_c);

	//void addScoreWhenHit(void* other);
};

const SpriteData WingTentenSpriteData = { ProfileId::WingTenten, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile WingTentenProfile(&daEnParaTenten_c::build, SpriteId::WingTenten, &WingTentenSpriteData, ProfileId::WingTenten, ProfileId::WingTenten, "WingTenten", WingTentenFileList);



u8 hijackMusicWithSongName(const char* songName, int themeID, bool hasFast, int channelCount, int trackCount, int* wantRealStreamID);

void daEnParaTenten_c::playerCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
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
void daEnParaTenten_c::yoshiCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->playerCollision(apThis, apOther);
}

/*bool daEnParaTenten_c::collisionCat7_GroundPound(ActivePhysics* apThis, ActivePhysics* apOther) {
	bool ret = dEn_c::collisionCat7_GroundPound(apThis, apOther);
	if (ret)
		this->kill();
	return ret;
}
bool daEnParaTenten_c::collisionCat7_GroundPoundYoshi(ActivePhysics* apThis, ActivePhysics* apOther) {
	return dEn_c::collisionCat7_GroundPoundYoshi(apThis, apOther);
}
bool daEnParaTenten_c::collisionCatD_Drill(ActivePhysics* apThis, ActivePhysics* apOther) {
	return dEn_c::collisionCatD_Drill(apThis, apOther);
}
bool daEnParaTenten_c::collisionCatA_PenguinMario(ActivePhysics* apThis, ActivePhysics* apOther) {
	return dEn_c::collisionCatA_PenguinMario(apThis, apOther);
}

bool daEnParaTenten_c::collisionCat1_Fireball_E_Explosion(ActivePhysics* apThis, ActivePhysics* apOther) {
	CDPrintCurrentAddress();
	bool ret = dEn_c::collisionCat1_Fireball_E_Explosion(apThis, apOther);
	if (ret)
		this->kill();
	return ret;
}
//bool daEnParaTenten_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics* apThis, ActivePhysics* apOther) {
//	return false;
//}
bool daEnParaTenten_c::collisionCat9_RollingObject(ActivePhysics* apThis, ActivePhysics* apOther) {
	return dEn_c::collisionCat9_RollingObject(apThis, apOther);
}
bool daEnParaTenten_c::collisionCat13_Hammer(ActivePhysics* apThis, ActivePhysics* apOther) {
	return dEn_c::collisionCat13_Hammer(apThis, apOther);
}
bool daEnParaTenten_c::collisionCat14_YoshiFire(ActivePhysics* apThis, ActivePhysics* apOther) {
	return dEn_c::collisionCat14_YoshiFire(apThis, apOther);
}

bool daEnParaTenten_c::collisionCat3_StarPower(ActivePhysics* apThis, ActivePhysics* apOther) {
	return dEn_c::collisionCat3_StarPower(apThis, apOther);
}*/


void daEnParaTenten_c::_vf148() {
	dEn_c::_vf148();
//	this->Delete(1);
}

void daEnParaTenten_c::_vf14C() {
	dEn_c::_vf14C();
//	this->Delete(1);
}

//this does weird shit, but it creates the iceblock around it
bool daEnParaTenten_c::CreateIceActors() {
	struct DoSomethingCool my_struct = { 0, (Vec){pos.x, pos.y - 12, pos.z}, {1.7, 1.4, 2.0}, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
	this->frzMgr.Create_ICEACTORs((void*)&my_struct, 1);
	__destroy_arr((void*)&my_struct, sub_80024C20, 0x3C, 1);
	this->chrAnimation.setCurrentFrame(0.0);
	this->chrAnimation.setUpdateRate(0.0);
	//this->frozen = true;
	return true;
}


dActor_c* daEnParaTenten_c::build() {
	void* buffer = AllocFromGameHeap1(sizeof(daEnParaTenten_c));
	daEnParaTenten_c* c = new(buffer) daEnParaTenten_c;

	return c;
}


extern int getNybbleValue(u32 settings, int fromNybble, int toNybble);

void daEnParaTenten_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate)
{
	nw4r::g3d::ResAnmChr anmChr = this->resFileAnim.GetResAnmChr(name);
	this->chrAnimation.bind(&this->bodyModel, anmChr, unk);
	this->bodyModel.bindAnim(&this->chrAnimation, unk2);
	this->chrAnimation.setUpdateRate(rate);
}

int daEnParaTenten_c::onCreate() {
	if(!this->ranOnce) {
		this->ranOnce = true;
		return false;
	}
	
	this->deleteForever = false; //makes the death state being used in preExecute

	// Model creation	
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	this->resFile.data = getResource("TentenWing", "g3d/TentenWing.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("TentenWing");
	bodyModel.setup(mdl, &allocator, 0x227, 1, 0);
	SetupTextures_Player(&bodyModel, 0);

	this->resFileAnim.data = getResource("TentenWing", "g3d/TentenWing.brres");
	nw4r::g3d::ResAnmChr anmChr = this->resFileAnim.GetResAnmChr("FlyWait");
	this->chrAnimation.setup(mdl, anmChr, &this->allocator, 0);

	this->anmPat = this->resFile.GetResAnmTexPat("TentenWing");
	this->patAnimation.setup(mdl, anmPat, &this->allocator, 0, 1);
	this->patAnimation.bindEntry(&this->bodyModel, &anmPat, 0, 1);
	this->patAnimation.setFrameForEntry(((this->eventId1 & 0b111) % 5), 0);
	this->patAnimation.setUpdateRateForEntry(0.0f, 0);
	this->bodyModel.bindAnim(&this->patAnimation);

	allocator.unlink();

	ActivePhysics::Info HitMeBaby;

	HitMeBaby.xDistToCenter = 0.0;
	HitMeBaby.yDistToCenter = 0.0;

	HitMeBaby.xDistToEdge = 7.5;
	HitMeBaby.yDistToEdge = 7.5;

	HitMeBaby.category1 = 0x3;
	HitMeBaby.category2 = 0x0;
	HitMeBaby.bitfield1 = 0x6F;
	HitMeBaby.bitfield2 = 0xffbafffe;
	HitMeBaby.unkShort1C = 0;
	HitMeBaby.callback = &dEn_c::collisionCallback;

	this->aPhysics.initWithStruct(this, &HitMeBaby);
	this->aPhysics.addToList();


	this->leftRight = settings >> 31 & 1;

	this->facingRight = settings >> 30 & 1;
	this->wDistance = settings >> 20 & 0b11111111;
	this->wSpeed = settings >> 16 & 0b1111;

	this->scale = (Vec){0.4, 0.4, 0.4};

	this->rot.x = 0;
	if (facingRight)
	{
		this->rot.y = 0x4000;
	}
	else
	{
		this->rot.y = 0x4000;
		this->rot.y *= -1;
	}
	this->rot.z = 0;

	this->pos.z = 4000;

	bindAnimChr_and_setUpdateRate("FlyWait", 1, 0.0, 1.0);

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


int daEnParaTenten_c::onDelete() {
	nw4r::snd::SoundHandle handle;
	PlaySoundWithFunctionB4(SoundRelatedClass, &handle, SE_EMY_DOWN, 1);
	StageE4::instance->spawnCoinJump(pos, 0, 1, 0);
	return true;
}

int daEnParaTenten_c::onDraw() {
	bodyModel.scheduleForDrawing();
	return true;
}


void daEnParaTenten_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y - 8, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

int daEnParaTenten_c::onExecute() {
	bodyModel._vf1C();
	updateModelMatrices();

	acState.execute();

	if(this->leftRight) {
		if(acState.getCurrentState() == &StateID_Wait) {
			if (this->facingRight) {
				this->rot.y = 0x4000;

				this->pos.x += this->wSpeed;
				this->stepCount += 1;

				if ((stepCount * wSpeed) > wDistance) {
					this->facingRight = false;
					this->stepCount = 0;
				}
			} else {
				this->rot.y = 0x4000;
				this->rot.y *= -1;

				this->pos.x -= this->wSpeed;
				this->stepCount += 1;

				if ((stepCount * wSpeed) > wDistance) {
					this->facingRight = true;
					this->stepCount = 0;
				}
			}
		}
	} else {
		
		if (this->stepsDone == this->stepCount) {
			if (this->rotateNext) {
				if (this->stepVector.x > 0) {
					this->rot.y = 0x4000;
				} else {
					this->rot.y = -0x4000;
				}
			} else if (this->rotate0XNext) {
				this->rot.y = 0x0;
			}
		}
	}
	if (this->chrAnimation.isAnimationDone()) {
		this->chrAnimation.setCurrentFrame(0.0);
	}

	return true;
}