#include <common.h>
#include <game.h>
#include <profile.h>

const char* BananaFileList[] = {"I_banana", 0};

class daEnBanana_c : public dEn_c {
public:
	int onCreate();
	int onExecute();
	//int onDelete();
	int onDraw();

	void collect(bool withDamageEffect);

	mHeapAllocator_c allocator;
	nw4r::g3d::ResFile resFile;
	m3d::mdl_c bodyModel;
	m3d::anmClr_c clrAnimation;
	nw4r::g3d::ResAnmClr anmClr;
	m3d::anmChr_c chrAnimation;
	
	int counter;

	//int type;

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
	bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat9_RollingObject(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat13_Hammer(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat14_YoshiFire(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat3_StarPower(ActivePhysics* apThis, ActivePhysics* apOther);

	USING_STATES(daEnBanana_c);
	DECLARE_STATE(Appear);
	DECLARE_STATE(Wait);
	DECLARE_STATE(WaitForRespawn);
};

CREATE_STATE(daEnBanana_c, Appear);
CREATE_STATE(daEnBanana_c, Wait);
CREATE_STATE(daEnBanana_c, WaitForRespawn);

const SpriteData BananaSpriteData = { ProfileId::Banana, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile BananaProfile(&daEnBanana_c::build, SpriteId::Banana, &BananaSpriteData, ProfileId::Banana, ProfileId::Banana, "Banana", BananaFileList);


void daEnBanana_c::playerCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	collect(true);
	EN_LandbarrelPlayerCollision(this, apThis, apOther);
	DamagePlayer(this, apThis, apOther);

}

void daEnBanana_c::yoshiCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->playerCollision(apThis, apOther);
}

bool daEnBanana_c::collisionCat7_GroundPound(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->playerCollision(apThis, apOther);
	return true;
}

bool daEnBanana_c::collisionCat7_GroundPoundYoshi(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->playerCollision(apThis, apOther);
	return true;
}

bool daEnBanana_c::collisionCatD_Drill(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->playerCollision(apThis, apOther);
	return true;
}

bool daEnBanana_c::collisionCatA_PenguinMario(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->playerCollision(apThis, apOther);
	return true;
}

bool daEnBanana_c::collisionCat1_Fireball_E_Explosion(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}

bool daEnBanana_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}

bool daEnBanana_c::collisionCat9_RollingObject(ActivePhysics* apThis, ActivePhysics* apOther) {
	collect(true);
	return true;
}

bool daEnBanana_c::collisionCat13_Hammer(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}

bool daEnBanana_c::collisionCat14_YoshiFire(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}

bool daEnBanana_c::collisionCat3_StarPower(ActivePhysics* apThis, ActivePhysics* apOther) {
	collect(false);
	return true;
}


dActor_c* daEnBanana_c::build() {
	void* buffer = AllocFromGameHeap1(sizeof(daEnBanana_c));
	daEnBanana_c* c = new(buffer) daEnBanana_c;

	return c;
}


extern int getNybbleValue(u32 settings, int fromNybble, int toNybble);

void daEnBanana_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate) {
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr(name);
	this->chrAnimation.bind(&this->bodyModel, anmChr, unk);
	this->bodyModel.bindAnim(&this->chrAnimation, unk2);
	this->chrAnimation.setUpdateRate(rate);
}

int daEnBanana_c::onCreate() {
	//this->type = this->settings >> 28 & 0xF;

	this->deleteForever = false;


	this->counter = 0;


	// Model creation
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	this->resFile.data = getResource("I_banana", "g3d/I_banana.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("I_banana");
	bodyModel.setup(mdl, &allocator, 0x128, 1, 0);
	SetupTextures_Item(&bodyModel, 0);

	anmClr = this->resFile.GetResAnmClr("banana");
	this->clrAnimation.setup(mdl, anmClr, &this->allocator, 0, 1);
	this->clrAnimation.bind(&this->bodyModel, anmClr, 0, 0);
	this->bodyModel.bindAnim(&this->clrAnimation);
	this->clrAnimation.setFrameForEntry(79.0f, 0);
	this->clrAnimation.setUpdateRateForEntry(1.0f, 0);

	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr("wait");
	this->chrAnimation.setup(mdl, anmChr, &this->allocator, 0);

	allocator.unlink();

	ActivePhysics::Info HitMeBaby;

	HitMeBaby.xDistToCenter = 0.0;
	HitMeBaby.yDistToCenter = 0.0;

	HitMeBaby.xDistToEdge = 7.5;
	HitMeBaby.yDistToEdge = 7.5;

	HitMeBaby.category1 = 0x3;
	HitMeBaby.category2 = 0x0;
	HitMeBaby.bitfield1 = 0x4F;
	HitMeBaby.bitfield2 = 0x8028E;
	HitMeBaby.unkShort1C = 0;
	HitMeBaby.callback = &dEn_c::collisionCallback;

	this->aPhysics.initWithStruct(this, &HitMeBaby);
	this->aPhysics.addToList();

	this->disableEatIn();

	// Stuff I do understand
	this->scale = (Vec){2.0f, 2.0f, 2.0f};

	this->rot.x = 0;
	this->rot.y = 0;
	this->rot.z = 0;

	//this->pos.y -= 4;
	this->pos.z = 0;

	doStateChange(&StateID_Wait);

	bindAnimChr_and_setUpdateRate("wait", 1, 0.0, 0.6);

	this->onExecute();
	return true;
}


//int daEnBanana_c::onDelete() {
//	return true;
//}

void daEnBanana_c::collect(bool withDamageEffect) {
	doStateChange(&StateID_WaitForRespawn);

	if(withDamageEffect) {
		S16Vec nullRot = {0,0,0};
		Vec efScale = {1.0f, 1.0f, 1.0f};
		nw4r::snd::SoundHandle handle;


		SpawnEffect("Wm_mr_wirehit_hit", 0, &this->pos, &nullRot, &efScale);
		PlaySoundWithFunctionB4(SoundRelatedClass, &handle, SE_EMY_BIG_PAKKUN_DAMAGE_1, 1);
	}
}


void daEnBanana_c::beginState_Wait() {}
void daEnBanana_c::executeState_Wait() {}
void daEnBanana_c::endState_Wait() {}

void daEnBanana_c::beginState_WaitForRespawn() {
	this->counter = 0;
	this->clrAnimation.setFrameForEntry(0.0f, 0);
	this->aPhysics.removeFromList();
}
void daEnBanana_c::executeState_WaitForRespawn() {
	if(this->counter >= 600) {
		doStateChange(&StateID_Appear);
	} else {
		this->counter++;
	}
}
void daEnBanana_c::endState_WaitForRespawn() {}


void daEnBanana_c::beginState_Appear() {
	this->aPhysics.addToList();
}
void daEnBanana_c::executeState_Appear() {
	//OSReport("clr: %f\n", this->clrAnimation.getFrameForEntry(0));
	if (this->clrAnimation.getFrameForEntry(0) != 79.0f) {
		this->clrAnimation.process();
	} else {
		doStateChange(&StateID_Wait);
	}
}
void daEnBanana_c::endState_Appear() {}



int daEnBanana_c::onDraw() {
	bodyModel.scheduleForDrawing();
	return true;
}


void daEnBanana_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y - 1, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

int daEnBanana_c::onExecute() {
	bodyModel._vf1C();
	updateModelMatrices();

	if (this->chrAnimation.isAnimationDone()) {
		this->chrAnimation.setCurrentFrame(0.0);
	}

	this->acState.execute();

	//OSReport("Banana State: %s\n", this->acState.getCurrentState()->getName());
	//OSReport("counter: %d\n", counter);

	return true;
}