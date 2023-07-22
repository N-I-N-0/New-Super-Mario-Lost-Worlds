#include <common.h>
#include <game.h>

const char *RocketBarrelFileList[] = {"rocket_barrel", 0};

#define SFX_barrelFail    2010
#define SFX_barrelFly     2011
#define SFX_barrelIn      2012
#define SFX_barrelStartUp 2013

class dRocketBarrel_c : public dEn_c {
public:
	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	mHeapAllocator_c allocator;
	m3d::mdl_c bodyModel;
	nw4r::g3d::ResFile resFile;

	m3d::anmChr_c chrAnimation;

	mEf::es2 smoke;
	mEf::es2 flames;
	int smTimer;
	int sfxTimer;

	u8 type;

	dStageActor_c *HScrollLimiterB;
	u32 currentPress;
	u32 pressTimer;

	bool isFlying;
	float maxHeight;
	float minHeight;

	bool playersIn;

	daPlBase_c *players[4];

	bool isRoaring;

	nw4r::snd::StrmSoundHandle barrelSoundHandle;
	nw4r::snd::StrmSoundHandle roarSoundHandle;

	static dActor_c *build();

	void bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate);
	void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther);
	void spriteCollision(ActivePhysics *apThis, ActivePhysics *apOther);

	bool collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther);

	bool collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther);

	USING_STATES(dRocketBarrel_c);
	DECLARE_STATE(Idle);
	DECLARE_STATE(WaitForPress);
	DECLARE_STATE(StartFly);
};

CREATE_STATE(dRocketBarrel_c, Idle);
CREATE_STATE(dRocketBarrel_c, WaitForPress);
CREATE_STATE(dRocketBarrel_c, StartFly);

extern "C" void *MakeMarioEnterDemoMode();
extern "C" void *MakeMarioExitDemoMode();

void dRocketBarrel_c::playerCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	if(!this->playersIn) {
		MakeMarioEnterDemoMode();

		for(int i = 0; i < 4; i++) {
			if(Player_Active[i] && Player_Lives[i] > 0) {
				this->players[i] = GetPlayerOrYoshi(i);
			}
		}

		PlaySoundWithFunctionB4(SoundRelatedClass, &barrelSoundHandle, SFX_barrelIn, 1);
		barrelSoundHandle.SetVolume(1.5f, 1);
		barrelSoundHandle.SetPitch(1.0f);

		doStateChange(&StateID_WaitForPress);
		this->playersIn = true;
	}
}

void dRocketBarrel_c::spriteCollision(ActivePhysics *apThis, ActivePhysics *apOther) {

}

bool dRocketBarrel_c::collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther) {
	return false;
}
bool dRocketBarrel_c::collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther) {
	return false;
}
bool dRocketBarrel_c::collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther) {
	return false;
}
bool dRocketBarrel_c::collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther) {
	return false;
}

bool dRocketBarrel_c::collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther) {
	return false;
}
bool dRocketBarrel_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther) {
	return false;
}
bool dRocketBarrel_c::collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther) {
	return false;
}
bool dRocketBarrel_c::collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther) {
	return false;
}
bool dRocketBarrel_c::collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther) {
	return false;
}

bool dRocketBarrel_c::collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther) {
	return false;
}


dActor_c *dRocketBarrel_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dRocketBarrel_c));
	dRocketBarrel_c *c = new(buffer) dRocketBarrel_c;

	return c;
}

#include <profile.h>
const SpriteData rocketBarrelData = {ProfileId::RocketBarrel, 8, -8, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0};
Profile RocketBarrelProfile = Profile(&dRocketBarrel_c::build, SpriteId::RocketBarrel, &rocketBarrelData, ProfileId::RocketBarrel, ProfileId::RocketBarrel, "RocketBarrel", RocketBarrelFileList);


void dRocketBarrel_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate) {
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr(name);
	this->chrAnimation.bind(&this->bodyModel, anmChr, unk);
	this->bodyModel.bindAnim(&this->chrAnimation, unk2);
	this->chrAnimation.setUpdateRate(rate);
}

int dRocketBarrel_c::onCreate() {
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	this->resFile.data = getResource("rocket_barrel", "g3d/model.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("RocketBarrel");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Enemy(&bodyModel, 0);

	bool ret;
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr("Idle");
	ret = this->chrAnimation.setup(mdl, anmChr, &this->allocator, 0);

	allocator.unlink();

	ActivePhysics::Info HitMeBaby;
	HitMeBaby.xDistToCenter = 0.0;
	HitMeBaby.yDistToCenter = 0.0;
	HitMeBaby.xDistToEdge = 12.0;
	HitMeBaby.yDistToEdge = 12.0;
	HitMeBaby.category = 0x5;
	HitMeBaby.attack = 0x0;
	HitMeBaby.categoryBitfield = 0x4F;
	HitMeBaby.attackBitfield = 0xFFFFFFFF;
	HitMeBaby.miscFlags = 0;
	HitMeBaby.callback = &dEn_c::collisionCallback;

	this->aPhysics.initWithStruct(this, &HitMeBaby);
	this->aPhysics.addToList();

	this->scale = (Vec){0.175f, 0.175f, 0.175f};
	this->disableFlagMask = 0;


	bindAnimChr_and_setUpdateRate("Idle", 1, 0.0, 1.0);

	doStateChange(&StateID_Idle);

	this->onExecute();
	return true;
}


int dRocketBarrel_c::onDelete() {
	return true;
}

int dRocketBarrel_c::onDraw() {
	bodyModel.scheduleForDrawing();
	return true;
}


int dRocketBarrel_c::onExecute() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);

	bodyModel._vf1C();

	acState.execute();


	if(this->isFlying) {
		if(this->chrAnimation.isAnimationDone()) 
			this->chrAnimation.setCurrentFrame(0.0);

		maxHeight = ClassWithCameraInfo::instance->screenTop - 16.0f;
		minHeight = ClassWithCameraInfo::instance->screenTop - ClassWithCameraInfo::instance->screenHeight - 16.0f;

		if(this->pos.y > maxHeight) {
			this->pos.y = maxHeight;
			this->speed.y = -1.5f;
		}
		if(this->pos.y < minHeight) { 
			// this->pos.y = minHeight;

			MakeMarioExitDemoMode();
			if(barrelSoundHandle.Exists()) barrelSoundHandle.Stop(0);
			if(roarSoundHandle.Exists()) roarSoundHandle.Stop(0);

			this->pos.y -= 20.0f; // Make sure you die
		}
	}



	if(this->playersIn) {
		for(int i = 0; i < 4; i++) {
			if(this->players[i]) {
				this->players[i]->pos = (Vec){this->pos.x, this->pos.y - 8, this->pos.z};
				this->players[i]->scale = (Vec){0, 0, 0};
				this->players[i]->removeMyActivePhysics();
			}
		}
	}

	return true;
}


void dRocketBarrel_c::beginState_Idle() {
	// Vec limitPos = (Vec){this->pos.x + 192.0f, this->pos.y, this->pos.z};
	// HScrollLimiterB = dStageActor_c::create(EN_LR_STOP_DW, 0, &limitPos, 0, 0);
}

void dRocketBarrel_c::executeState_Idle() {
	StopBGMMusic();
}

void dRocketBarrel_c::endState_Idle() {

}


void dRocketBarrel_c::beginState_WaitForPress() {
	currentPress = 0;
	pressTimer = 0;
}



void dRocketBarrel_c::executeState_WaitForPress() {
	if(currentPress) {
		pressTimer--;

		if(pressTimer == 0) {
			currentPress = 0;
			pressTimer = 120;
			PlaySoundWithFunctionB4(SoundRelatedClass, &barrelSoundHandle, SFX_barrelFail, 1);
			barrelSoundHandle.SetVolume(1.0f, 1);
			barrelSoundHandle.SetPitch(1.0f);
		}
	}

	int nowPressed = Remocon_GetPressed(GetActiveRemocon());
	//OSReport("currentPress: %d\n", currentPress);
	if(nowPressed & WPAD_TWO) {
		currentPress++;
		pressTimer = 120;

		PlaySoundWithFunctionB4(SoundRelatedClass, &barrelSoundHandle, SFX_barrelStartUp, 1);
		float volume = 0.4 + 0.125f * currentPress;
		float pitch = 1.0f + 0.1f * currentPress;
		barrelSoundHandle.SetVolume(volume, 1);
		barrelSoundHandle.SetPitch(pitch);
	}

	if(currentPress >= 5) {
		doStateChange(&StateID_StartFly);
	}

}

void dRocketBarrel_c::endState_WaitForPress() {
	//HScrollLimiterB->Delete(1);
}


void dRocketBarrel_c::beginState_StartFly() {
	//this->rot.z = -0x4000;
	bindAnimChr_and_setUpdateRate("Deploy", 1, 0.0, 1.0);
	StartBGMMusic();

	this->speed.y = 3.0f;
	this->y_speed_inc = -0.1f;
	this->max_speed.y = 3.6f; // max speed = -(min speed) * 1.2
	this->max_speed.x = -3.0f; // min speed

	this->smTimer = 0;
}



void dRocketBarrel_c::executeState_StartFly() {
	if(!this->isFlying && this->chrAnimation.isAnimationDone())  {
		bindAnimChr_and_setUpdateRate("Fly", 1, 0.0, 1.0);

		this->isFlying = true;
	}


	Vec efPos1 = (Vec){this->pos.x, this->pos.y - 6, this->pos.z};
	S16Vec nullRot = (S16Vec){0,0,(s16)(this->rot.z - 0x8000)};
	Vec efScale = {1.0f, 1.0f, 1.0f};
	smoke.spawn("Wm_en_rocketsmoke", 0, &efPos1, &nullRot, &efScale);
	flames.spawn("Wm_en_rocketflames", 0, &efPos1, &nullRot, &efScale);

	if(this->sfxTimer % 30 == 0) {
		PlaySoundWithFunctionB4(SoundRelatedClass, &barrelSoundHandle, SFX_barrelFly, 1);
		barrelSoundHandle.SetVolume(0.75f, 1);
		barrelSoundHandle.SetPitch(1.0f);
	}
	this->sfxTimer++;

	if(this->smTimer < 20) this->smTimer++;


	int nowHeld = Remocon_GetButtons(GetActiveRemocon());
	if(nowHeld & WPAD_TWO) {
		if(this->pos.y < maxHeight) {
			this->speed.y += 0.25f;
		}

		if(!this->isRoaring) {
			//PlaySoundWithFunctionB4(SoundRelatedClass, &roarSoundHandle, 2005, 1);
			this->isRoaring = true;
		}
	}
	else if(this->isRoaring) {
		if(roarSoundHandle.Exists()) roarSoundHandle.Stop(20);
		this->isRoaring = false;
	}

	//this->y_speed_inc = -(this->speed.y - 3.0f) / 3.0f;

	// max -> 64° = 0x2D83
	// 0 -> 16° = 0xB61
	// min -> -24° = -0x1111
	// from min to max, min = 0, max = 0x3E94, then total - 0x1111
	// so max-min * ? = 0x3E94 <=> ? = 0x3E94 / max-min
	// so rot = ((speed.y + 3.0f) * ?) - 0x1111 padded by 90° if needed
	this->rot.z = (s16)(((this->speed.y + 3.0f) * (0x3E94 / (this->max_speed.y - this->max_speed.x))) - 0x1111) - 0x4000;
	this->speed.y += this->y_speed_inc;
	if(this->speed.y > this->max_speed.y) this->speed.y = this->max_speed.y;
	// if(this->speed.y < this->max_speed.x) this->speed.y = this->max_speed.x;
	this->pos.y += this->speed.y;


	this->pos.x += 3.5f;

	// OSReport("Position: %f\n", this->pos.x);

	if(this->pos.x > 13208.0f) {
		doStateChange(&StateID_Idle);
	}
}

void dRocketBarrel_c::endState_StartFly() {
	this->isFlying = false;
	this->playersIn = false;
	for(int i = 0; i < 4; i++) {
		if(this->players[i]) {
			this->players[i]->scale = (Vec){1, 1, 1};
			this->players[i]->addMyActivePhysics();
		}
	}
	MakeMarioExitDemoMode();
	if(barrelSoundHandle.Exists()) barrelSoundHandle.Stop(0);
	if(roarSoundHandle.Exists()) roarSoundHandle.Stop(0);

	StopBGMMusic();
	this->Delete(1);
}