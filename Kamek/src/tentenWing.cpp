#include <common.h>
#include <game.h>
#include "profile.h"
#include <sfx.h>
#include "path.h"

const char* TentenWingFileList[] = { "tenten_wing", 0 };


class daEnPataTenten_c : public dEnPath_c {
public:
	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	mHeapAllocator_c allocator;
	nw4r::g3d::ResFile resFile;
	m3d::mdl_c model;

	nw4r::g3d::ResAnmTexPat anmPat;

	m3d::anmChr_c animationChr;
	m3d::anmTexPat_c animationPat;

	bool linearMovement, upDown;

	bool facingRightUp;
	float wSpeed;
	u8 wDistance;
	int edgeMin, edgeMax;
    u16 amtRotate;

	int stepCount;

	bool ranOnce;

	int sfxTimer;
	nw4r::snd::SoundHandle handle; // Sound Handle

	static dActor_c* build();

	void bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate);

	void updateModelMatrices();
	void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther);
	void yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther);

	bool collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther);
	// bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther);

	bool collisionCat5_Mario(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat11_PipeCannon(ActivePhysics *apThis, ActivePhysics *apOther);

	void _vf148();
	void _vf14C();
	bool CreateIceActors();

	void playsound(int id);
	void executePlaysound();
    float getValueOfCurve(float ratio, float curveRatioVar, float minValue);

	USING_STATES(daEnPataTenten_c);
	DECLARE_STATE(Wait);
    DECLARE_STATE(Turn);
	DECLARE_STATE(FollowPath);
	DECLARE_STATE(DieStomp);
};

const SpriteData TentenWingSpriteData = { ProfileId::TentenWing, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0b10000 };
Profile TentenWingProfile(&daEnPataTenten_c::build, SpriteId::TentenWing, &TentenWingSpriteData, ProfileId::TentenWing, ProfileId::TentenWing, "TentenWing", TentenWingFileList);



u8 hijackMusicWithSongName(const char* songName, int themeID, bool hasFast, int channelCount, int trackCount, int* wantRealStreamID);
extern "C" char usedForDeterminingStatePress_or_playerCollision(dEn_c* t, ActivePhysics *apThis, ActivePhysics *apOther, int unk1);
extern "C" int SmoothRotation(short* rot, u16 amt, int unk2);

CREATE_STATE(daEnPataTenten_c, Wait);
CREATE_STATE(daEnPataTenten_c, Turn);
CREATE_STATE(daEnPataTenten_c, FollowPath);
CREATE_STATE(daEnPataTenten_c, DieStomp);

void daEnPataTenten_c::playerCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	char hitType = usedForDeterminingStatePress_or_playerCollision(this, apThis, apOther, 2);

	if (hitType == 1) {	// regular jump
		this->_vf260(apOther->owner);
		doStateChange(&StateID_DieStomp);
	}
	else if (hitType == 3) { // spin jump
		this->_vf268(apOther->owner);
		doStateChange(&StateID_DieSmoke);
	}
	else if (hitType == 0) { // sides
		this->dEn_c::playerCollision(apThis, apOther);
		this->_vf220(apOther->owner);
	}
	else if (hitType == 2) { // mini mario
		bouncePlayer(this, 5.0);
	}
}

void daEnPataTenten_c::yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	char hitType = usedForDeterminingStatePress_or_playerCollision(this, apThis, apOther, 2);

	if (hitType == 1 || hitType == 2) {	// regular jump or mini mario
		this->_vf278(apOther->owner);
		doStateChange(&StateID_DieSmoke);
	}
	else if (hitType == 3) { // spin jump
		this->_vf268(apOther->owner);
		doStateChange(&StateID_DieSmoke);
	}
	else if (hitType == 0) { // sides
		this->dEn_c::playerCollision(apThis, apOther);
		this->_vf220(apOther->owner);
	}
}
bool daEnPataTenten_c::collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther) {
	PlaySound(this, SE_EMY_DOWN);
	SpawnEffect("Wm_mr_hardhit", 0, &pos, &(S16Vec){0, 0, 0}, &(Vec){1.0, 1.0, 1.0});
	dEn_c::_vf148();
	return true;
}
bool daEnPataTenten_c::collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->collisionCatD_Drill(apThis, apOther);
}
bool daEnPataTenten_c::collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->collisionCatD_Drill(apThis, apOther);
}
bool daEnPataTenten_c::collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->collisionCatD_Drill(apThis, apOther);
}
bool daEnPataTenten_c::collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->collisionCatD_Drill(apThis, apOther);
}
bool daEnPataTenten_c::collisionCat5_Mario(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->collisionCatD_Drill(apThis, apOther);
}
bool daEnPataTenten_c::collisionCat11_PipeCannon(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->collisionCatD_Drill(apThis, apOther);
}
bool daEnPataTenten_c::collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther) {
	StageE4::instance->spawnCoinJump(pos, 0, 2, 0);
	return this->collisionCatD_Drill(apThis, apOther);
}

bool daEnPataTenten_c::collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther) {
	bool wut = dEn_c::collisionCat3_StarPower(apThis, apOther);
	return wut;
}

bool daEnPataTenten_c::collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->collisionCat1_Fireball_E_Explosion(apThis, apOther);
}

bool daEnPataTenten_c::collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther) {
	StageE4::instance->spawnCoinJump(pos, 0, 1, 0);
	return this->collisionCatD_Drill(apThis, apOther);
}

// void daEnPataTenten_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther) {
	
// 	dEn_C::collisionCat2_IceBall_15_YoshiIce(apThis, apOther);
// }


// These handle the ice crap
void daEnPataTenten_c::_vf148() {
	dEn_c::_vf148();
}
void daEnPataTenten_c::_vf14C() {
	dEn_c::_vf14C();
}

extern "C" void sub_80024C20(void);
extern "C" void __destroy_arr(void*, void(*)(void), int, int);
//extern "C" __destroy_arr(struct DoSomethingCool, void(*)(void), int cnt, int bar);

bool daEnPataTenten_c::CreateIceActors() {
	struct DoSomethingCool my_struct = { 0, {this->pos.x, this->pos.y - 12.0, this->pos.z}, {1.7, 1.4, 2.0}, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
	this->frzMgr.Create_ICEACTORs( (void*)&my_struct, 1 );
	__destroy_arr( (void*)&my_struct, sub_80024C20, 0x3C, 1 );
	this->animationChr.setUpdateRate(0.0f);
	return true;
}


dActor_c* daEnPataTenten_c::build() {
	void* buffer = AllocFromGameHeap1(sizeof(daEnPataTenten_c));
	daEnPataTenten_c* c = new(buffer) daEnPataTenten_c;

	return c;
}


void daEnPataTenten_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate)
{
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr(name);
	this->animationChr.bind(&this->model, anmChr, unk);
	this->model.bindAnim(&this->animationChr, unk2);
	this->animationChr.setUpdateRate(rate);
}

int daEnPataTenten_c::onCreate() {
	if(!this->ranOnce) {
		this->ranOnce = true;
		return false;
	}
	this->sfxTimer = 0;

	int color = (this->eventId1 & 0xF0) >> 4;
    int groupId = (this->settings >> 24) & 0xFF;
	
	this->deleteForever = false; //makes the death state being used in preExecute

	// Model creation
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	char resName[80];
	sprintf(resName, "g3d/t%02d.brres", color);

	this->resFile.data = getResource("tenten_wing", resName);
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("tenten_wing");
	model.setup(mdl, &allocator, 0x227, 1, 0);
	SetupTextures_Enemy(&model, 0);

	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr("fly_wait");
	this->animationChr.setup(mdl, anmChr, &this->allocator, 0);

	this->anmPat = this->resFile.GetResAnmTexPat("tenten_wing");
	this->animationPat.setup(mdl, anmPat, &this->allocator, 0, 1);
	this->animationPat.bindEntry(&this->model, &anmPat, 0, 1);
	this->animationPat.setFrameForEntry(((this->eventId1 & 0xF) % 5), 0);
	this->animationPat.setUpdateRateForEntry(0.0f, 0);
	this->model.bindAnim(&this->animationPat);

	allocator.unlink();

	ActivePhysics::Info HitMeBaby;

	HitMeBaby.xDistToCenter = 0.0;
	HitMeBaby.yDistToCenter = 0.0;

	HitMeBaby.xDistToEdge = 10.0;
	HitMeBaby.yDistToEdge = 8.0;

	HitMeBaby.category1 = 0x3;
	HitMeBaby.category2 = 0x0;
	HitMeBaby.bitfield1 = 0x6F;
	HitMeBaby.bitfield2 = 0xffbafffe;
	HitMeBaby.unkShort1C = 0;
	HitMeBaby.callback = &dEn_c::collisionCallback;

	this->aPhysics.initWithStruct(this, &HitMeBaby);
	this->aPhysics.addToList();


	this->linearMovement = eventId2 >> 3 & 1;

	this->upDown = eventId2 >> 2 & 1;
	this->facingRightUp = eventId2 >> 1 & 1;
	this->wDistance = (settings >> 20 & 0xF) * 8;
	this->wSpeed = (settings >> 16 & 0xF) * 0.1;

	this->scale = (Vec){0.15, 0.15, 0.15};

	this->rot.x = 0;
	if (facingRightUp && !upDown) this->rot.y = 0x2800;
	else this->rot.y = 0xD800;
	this->rot.z = 0;

	if (this->upDown) {
		this->edgeMin = this->pos.y - this->wDistance + 2;
		this->edgeMax = this->pos.y + this->wDistance + 2;
	}
	else {
		this->edgeMin = this->pos.x - this->wDistance;
    	this->edgeMax = this->pos.x + this->wDistance;
	}

	bindAnimChr_and_setUpdateRate("fly_wait", 1, 0.0, 1.0);

	if (this->linearMovement) {
		if (this->wDistance == 0 || this->wSpeed == 0 || this->upDown) this->rot.y = 0;
		doStateChange(&StateID_Wait);
	}
    else {
		beginState_Init();
		if (this->rotateNext) {
			if (this->stepVector.x > 0) this->rot.y = 0x2800;
			else this->rot.y = 0xD800;
		}
		else if (this->rotate0XNext) this->rot.y = 0x0;
		executeState_Init();
		doStateChange(&StateID_FollowPath);
	}

	this->onExecute();
	return true;
}


int daEnPataTenten_c::onDelete() {
	return true;
}

int daEnPataTenten_c::onDraw() {
	model.scheduleForDrawing();
	return true;
}


void daEnPataTenten_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y - 8, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	model.setDrawMatrix(matrix);
	model.setScale(&scale);
	model.calcWorld(false);
}

int daEnPataTenten_c::onExecute() {
	acState.execute();
	model._vf1C();
	updateModelMatrices();

	return true;
}

void daEnPataTenten_c::executePlaysound() {
	this->sfxTimer++;

	switch (this->sfxTimer) {
		case 24:
			this->playsound(SFX_TENTEN_STEP_L);
			break;

		case 54:
			this->playsound(SFX_TENTEN_STEP_R);
			break;
	}

	if (this->sfxTimer > 60) this->sfxTimer = 0;
}

void daEnPataTenten_c::playsound(int id) {
	ClassWithCameraInfo *cwci = ClassWithCameraInfo::instance;
	Vec2 dist = {
		cwci->screenCentreX - this->pos.x,
		cwci->screenCentreY - this->pos.y
	};
	float volume = max<float>(0.0, (1.0 - (sqrtf(dist.x * dist.x + dist.y * dist.y) / 500.0)) * 1.0);
	if (volume <= 0.0) return;
	else if (volume > 1.0) volume = 1.0;

	PlaySoundWithFunctionB4(SoundRelatedClass, &this->handle, id, 1);
	handle.SetVolume(volume, 1);
}

float daEnPataTenten_c::getValueOfCurve(float ratio, float curveRatioVar, float minValue) {
    float curveRatio = 1.0 - curveRatioVar;

    if (ratio < curveRatioVar) {
        float r = ratio / curveRatioVar;
        return sin(r * (M_PI / 2.0)) / (1.0 / minValue) + minValue;
    }
    else if (ratio < curveRatio) {
        return 1.0;
    }
    else {
        float r = (ratio - curveRatio) / curveRatioVar;
        return (1 - sin(r * (M_PI / 2.0))) / (1.0 / minValue) + minValue;
    }
}

///////////////
// Wait State
///////////////
void daEnPataTenten_c::beginState_Wait() {
    this->wSpeed = this->facingRightUp ? abs(this->wSpeed) : -abs(this->wSpeed);
	this->animationChr.setUpdateRate(1.0f);
}
void daEnPataTenten_c::executeState_Wait() {
	this->executePlaysound();
	if (this->animationChr.getUpdateRate() == 0.0f) this->animationChr.setUpdateRate(1.0f);
	else if (this->animationChr.isAnimationDone()) this->animationChr.setCurrentFrame(0.0);

	if (this->wDistance == 0 || this->wSpeed == 0) return;

    float dist = this->edgeMax - this->edgeMin;
    float distRatio;
    if (dist > 16.0) distRatio = 8.0 / dist;
    else distRatio = 0.5;

    float pos = (this->upDown ? this->pos.y : this->pos.x) - this->edgeMin;
    float ratio = pos / dist;
    float ratioCurve = this->getValueOfCurve(ratio, distRatio, 0.375);

    float newSpeed = this->wSpeed * ratioCurve;
    (this->upDown ? this->pos.y : this->pos.x) += newSpeed;

    if (this->facingRightUp) {
        if ((this->upDown ? this->pos.y : this->pos.x) > this->edgeMax) {
            this->facingRightUp = false;
            (this->upDown ? this->pos.y : this->pos.x) = this->edgeMax;
            doStateChange(&StateID_Turn);
        }
    }
    else {
        if ((this->upDown ? this->pos.y : this->pos.x) < this->edgeMin) {
            this->facingRightUp = true;
            (this->upDown ? this->pos.y : this->pos.x) = this->edgeMin;
            doStateChange(&StateID_Turn);
        }
    }

	dEnPath_c::executeState_Wait();
}
void daEnPataTenten_c::endState_Wait() { }

///////////////
// Turn State
///////////////
void daEnPataTenten_c::beginState_Turn() {
	this->animationChr.setUpdateRate(1.0f);
}
void daEnPataTenten_c::executeState_Turn() {
	this->executePlaysound();
    if (this->animationChr.getUpdateRate() == 0.0f) this->animationChr.setUpdateRate(1.0f);
	else if (this->animationChr.isAnimationDone()) this->animationChr.setCurrentFrame(0.0);

	if (this->upDown) {
		doStateChange(&StateID_Wait);
		return;
	}

    u16 amt = (this->facingRightUp == 1) ? 0x2800 : 0xD800;
    int done = SmoothRotation(&this->rot.y, amt, 0x800);

    if(done) {
        doStateChange(&StateID_Wait);
    }
}
void daEnPataTenten_c::endState_Turn() { }

///////////////
// Follow Path State
///////////////
void daEnPataTenten_c::beginState_FollowPath() {
    this->amtRotate = this->rot.y;
	this->animationChr.setUpdateRate(1.0f);
}
void daEnPataTenten_c::executeState_FollowPath() {
	this->executePlaysound();
	if (this->animationChr.getUpdateRate() == 0.0f) this->animationChr.setUpdateRate(1.0f);
	else if (this->animationChr.isAnimationDone()) this->animationChr.setCurrentFrame(0.0);

	if (this->stepsDone == this->stepCount) {
        if (this->rotateNext) {
            if (this->stepVector.x > 0) this->amtRotate = 0x2800;
            else this->amtRotate = 0xD800;
        }
        else if (this->rotate0XNext) this->amtRotate = 0x0;
    }

    SmoothRotation(&this->rot.y, this->amtRotate, 0x200);
	dEnPath_c::executeState_FollowPath();
}
void daEnPataTenten_c::endState_FollowPath() { }

///////////////
// Die by Stomp State
///////////////
void daEnPataTenten_c::beginState_DieStomp() {
	this->removeMyActivePhysics();

	bindAnimChr_and_setUpdateRate("damage", 1, 0.0, 1.0);
	this->rot.y = 0;
	this->rot.x = 0;
}
void daEnPataTenten_c::executeState_DieStomp() {
	if(this->animationChr.isAnimationDone()) {
		this->Delete(true);
	}
}
void daEnPataTenten_c::endState_DieStomp() { }
