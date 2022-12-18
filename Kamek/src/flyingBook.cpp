#include <common.h>
#include <game.h>
#include <profile.h>
#include "path.h"

const char* FlyingBookFileList[] = { "FlyingBook", 0 };


class daFlyingBook_c : public dEn_c {
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
	bool fallNext;
	int timer;


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

	bool shouldFall();

	USING_STATES(daFlyingBook_c);
	DECLARE_STATE(Wait);
	DECLARE_STATE(Attack);
	DECLARE_STATE(Fly);

	//void addScoreWhenHit(void* other);
};

CREATE_STATE(daFlyingBook_c, Wait);
CREATE_STATE(daFlyingBook_c, Attack);
CREATE_STATE(daFlyingBook_c, Fly);

const SpriteData FlyingBookSpriteData = { ProfileId::FlyingBook, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile FlyingBookProfile(&daFlyingBook_c::build, SpriteId::FlyingBook, &FlyingBookSpriteData, ProfileId::FlyingBook, ProfileId::FlyingBook, "FlyingBook", FlyingBookFileList);



u8 hijackMusicWithSongName(const char* songName, int themeID, bool hasFast, int channelCount, int trackCount, int* wantRealStreamID);

void daFlyingBook_c::playerCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
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
void daFlyingBook_c::yoshiCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->playerCollision(apThis, apOther);
}

/*bool daFlyingBook_c::collisionCat7_GroundPound(ActivePhysics* apThis, ActivePhysics* apOther) {
	bool ret = dEn_c::collisionCat7_GroundPound(apThis, apOther);
	if (ret)
		this->kill();
	return ret;
}
bool daFlyingBook_c::collisionCat7_GroundPoundYoshi(ActivePhysics* apThis, ActivePhysics* apOther) {
	return dEn_c::collisionCat7_GroundPoundYoshi(apThis, apOther);
}
bool daFlyingBook_c::collisionCatD_Drill(ActivePhysics* apThis, ActivePhysics* apOther) {
	return dEn_c::collisionCatD_Drill(apThis, apOther);
}
bool daFlyingBook_c::collisionCatA_PenguinMario(ActivePhysics* apThis, ActivePhysics* apOther) {
	return dEn_c::collisionCatA_PenguinMario(apThis, apOther);
}

bool daFlyingBook_c::collisionCat1_Fireball_E_Explosion(ActivePhysics* apThis, ActivePhysics* apOther) {
	CDPrintCurrentAddress();
	bool ret = dEn_c::collisionCat1_Fireball_E_Explosion(apThis, apOther);
	if (ret)
		this->kill();
	return ret;
}
//bool daFlyingBook_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics* apThis, ActivePhysics* apOther) {
//	return false;
//}
bool daFlyingBook_c::collisionCat9_RollingObject(ActivePhysics* apThis, ActivePhysics* apOther) {
	return dEn_c::collisionCat9_RollingObject(apThis, apOther);
}
bool daFlyingBook_c::collisionCat13_Hammer(ActivePhysics* apThis, ActivePhysics* apOther) {
	return dEn_c::collisionCat13_Hammer(apThis, apOther);
}
bool daFlyingBook_c::collisionCat14_YoshiFire(ActivePhysics* apThis, ActivePhysics* apOther) {
	return dEn_c::collisionCat14_YoshiFire(apThis, apOther);
}

bool daFlyingBook_c::collisionCat3_StarPower(ActivePhysics* apThis, ActivePhysics* apOther) {
	return dEn_c::collisionCat3_StarPower(apThis, apOther);
}*/


void daFlyingBook_c::_vf148() {
	dEn_c::_vf148();
	this->Delete(1);
}

void daFlyingBook_c::_vf14C() {
	dEn_c::_vf14C();
	this->Delete(1);
}

//this does weird shit, but it creates the iceblock around it
bool daFlyingBook_c::CreateIceActors() {
	struct DoSomethingCool my_struct = { 0, (Vec){pos.x, pos.y - 12, pos.z}, {1.7, 1.4, 2.0}, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
	this->frzMgr.Create_ICEACTORs((void*)&my_struct, 1);
	__destroy_arr((void*)&my_struct, sub_80024C20, 0x3C, 1);
	this->chrAnimation.setCurrentFrame(0.0);
	this->chrAnimation.setUpdateRate(0.0);
	//this->frozen = true;
	return true;
}


dActor_c* daFlyingBook_c::build() {
	void* buffer = AllocFromGameHeap1(sizeof(daFlyingBook_c));
	daFlyingBook_c* c = new(buffer) daFlyingBook_c;

	return c;
}


extern int getNybbleValue(u32 settings, int fromNybble, int toNybble);

void daFlyingBook_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate)
{
	nw4r::g3d::ResAnmChr anmChr = this->resFileAnim.GetResAnmChr(name);
	this->chrAnimation.bind(&this->bodyModel, anmChr, unk);
	this->bodyModel.bindAnim(&this->chrAnimation, unk2);
	this->chrAnimation.setUpdateRate(rate);
}

int daFlyingBook_c::onCreate() {
	this->deleteForever = false; //makes the death state being used in preExecute

	// Model creation	
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	this->resFile.data = getResource("FlyingBook", "g3d/FlyingBook.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("FlyingBook");
	bodyModel.setup(mdl, &allocator, 0x227, 1, 0);
	SetupTextures_Player(&bodyModel, 0);

	this->resFileAnim.data = getResource("FlyingBook", "g3d/FlyingBook.brres");
	nw4r::g3d::ResAnmChr anmChr = this->resFileAnim.GetResAnmChr("fly");
	this->chrAnimation.setup(mdl, anmChr, &this->allocator, 0);

	/*this->anmPat = this->resFile.GetResAnmTexPat("TentenWing");
	this->patAnimation.setup(mdl, anmPat, &this->allocator, 0, 1);
	this->patAnimation.bindEntry(&this->bodyModel, &anmPat, 0, 1);
	this->patAnimation.setFrameForEntry(((this->eventId1 & 0b111) % 5), 0);
	this->patAnimation.setUpdateRateForEntry(0.0f, 0);
	this->bodyModel.bindAnim(&this->patAnimation);*/

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


	static const lineSensor_s below(12<<12, 4<<12, 0<<12);
	static const pointSensor_s above(0<<12, 12<<12);
	static const lineSensor_s adjacent(6<<12, 9<<12, 6<<12);

	collMgr.init(this, &below, &above, &adjacent);
	collMgr.calculateBelowCollisionWithSmokeEffect();


	this->scale = (Vec){0.5, 0.5, 0.5};

	this->rot.y = 0x5000;
	this->rot.z = 0x8000;
	

	this->pos.z = 4000;

	bindAnimChr_and_setUpdateRate("fly", 1, 0.0, 1.0);

	doStateChange(&StateID_Fly);


	this->onExecute();
	return true;
}


int daFlyingBook_c::onDelete() {
	nw4r::snd::SoundHandle handle;
	PlaySoundWithFunctionB4(SoundRelatedClass, &handle, SE_EMY_DOWN, 1);
	StageE4::instance->spawnCoinJump(pos, 0, 1, 0);
	return true;
}

int daFlyingBook_c::onDraw() {
	bodyModel.scheduleForDrawing();
	return true;
}


void daFlyingBook_c::updateModelMatrices() {
	matrix.translation(pos.x + 8, pos.y + 12, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

int daFlyingBook_c::onExecute() {
	bodyModel._vf1C();
	updateModelMatrices();
	acState.execute();

	return true;
}



bool daFlyingBook_c::shouldFall() {
	if(GenerateRandomNumber(60) == 0 || timer > 300) {
		float current = 1000000000000000000000000000000.0;

		for(u8 i = 0; i < 4; i++) {
			dStageActor_c* player = GetSpecificPlayerActor(i);
			if(!player) {
				continue;
			}
			float distance = abs(player->pos.x - pos.x);;
			if(distance < current) {
				current = distance;
			}
		}
		return current <= 32;

	} else {
		timer++;
		return false;
	}
}



void daFlyingBook_c::beginState_Attack() {}
void daFlyingBook_c::executeState_Attack() {
	collMgr.calculateBelowCollisionWithSmokeEffect();

	if (collMgr.isOnTopOfTile()) {
		this->aPhysics.removeFromList();
		doStateChange(&StateID_Wait);
	} else {
		this->pos.y -= 4;
	}
}
void daFlyingBook_c::endState_Attack() {}


void daFlyingBook_c::beginState_Fly() {
	this->speed.y = 0.0f;
}
void daFlyingBook_c::executeState_Fly() {
	if (this->chrAnimation.isAnimationDone()) {
		this->chrAnimation.setCurrentFrame(0.0);
	}







	this->direction = dSprite_c__getXDirectionOfFurthestPlayerRelativeToVEC3(this, this->pos);
	
	float speedDelta;
	speedDelta = 0.05;

	if (this->direction == 0) { // Going Left
		this->speed.x = this->speed.x + speedDelta; // 
		
		if (this->speed.x < 0) { this->speed.x += (speedDelta / 1.5); }
		if (this->speed.x < -6.0) { this->speed.x += (speedDelta * 2.0); }
	}
	else { // Going Right
		this->speed.x = this->speed.x - speedDelta;

		if (this->speed.x > 0) { this->speed.x -= (speedDelta / 1.5); }
		if (this->speed.x > 6.0) { this->speed.x -= (speedDelta * 2.0); }
	}

	this->HandleXSpeed();
	//this->HandleYSpeed();

	this->UpdateObjectPosBasedOnSpeedValuesReal();







	if (shouldFall()) {
		fallNext = true;
	}

	if ((((int)this->chrAnimation.getCurrentFrame()+15) %  30) == 0) {
		if (fallNext) {
			this->chrAnimation.setUpdateRate(0.0f);
			doStateChange(&StateID_Attack);
		}
	}
}
void daFlyingBook_c::endState_Fly() {}


void daFlyingBook_c::beginState_Wait() {}
void daFlyingBook_c::executeState_Wait() {}
void daFlyingBook_c::endState_Wait() {}