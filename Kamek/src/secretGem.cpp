#include <common.h>
#include <game.h>
#include <profile.h>

const char *SecretGemFileList[] = {"supergem", 0};

bool secretGemCollected = false;
bool secretGemAfterMidwayFlag = false;

class daEnSecretGem_c : public dEn_c {
public:
	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	mHeapAllocator_c allocator;
	nw4r::g3d::ResFile resFile;
	m3d::mdl_c bodyModel;
	m3d::anmChr_c anm;
	
	bool type8fastJump;
	int type8timer;
	int type8timer2;

	static dActor_c* build();

	void updateModelMatrices();
	void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther);
	void yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther);

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
	
	void bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate);
	
	USING_STATES(daEnSecretGem_c);
	
	DECLARE_STATE(Wait);
	DECLARE_STATE(Collect);
};

CREATE_STATE(daEnSecretGem_c, Wait);
CREATE_STATE(daEnSecretGem_c, Collect);

void daEnSecretGem_c::beginState_Wait() {}
void daEnSecretGem_c::executeState_Wait() {
	
	if (this->anm.isAnimationDone()) {
		this->anm.setCurrentFrame(0.0);
	}
	//doStateChange(&StateID_Fly);
}
void daEnSecretGem_c::endState_Wait() {}

void daEnSecretGem_c::beginState_Collect() {
	bindAnimChr_and_setUpdateRate("GotLow", 1, 0.0, 1.0);
}
void daEnSecretGem_c::executeState_Collect() {
	if (this->anm.isAnimationDone()) {
		secretGemCollected = true;
		secretGemAfterMidwayFlag = (GameMgrP->eight.checkpointEntranceID != 255);
		OSReport("secretGemAfterMidwayFlag: %s\n", secretGemAfterMidwayFlag ? "true" : "false");
		this->Delete(1);
	}
}
void daEnSecretGem_c::endState_Collect() {}


void daEnSecretGem_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate) {
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr(name);
	this->anm.bind(&this->bodyModel, anmChr, unk);
	this->bodyModel.bindAnim(&this->anm, unk2);
	this->anm.setUpdateRate(rate);
}



const SpriteData SecretGemSpriteData = { ProfileId::SecretGem, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile SecretGemProfile(&daEnSecretGem_c::build, SpriteId::SecretGem, &SecretGemSpriteData, ProfileId::SecretGem, ProfileId::SecretGem, "SecretGem", SecretGemFileList);



u8 hijackMusicWithSongName(const char *songName, int themeID, bool hasFast, int channelCount, int trackCount, int *wantRealStreamID);

void daEnSecretGem_c::playerCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	doStateChange(&StateID_Collect);
}
void daEnSecretGem_c::yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	this->type8fastJump = true;
}
bool daEnSecretGem_c::collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther) {
	this->type8fastJump = true;
	return false;
}
bool daEnSecretGem_c::collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther) {
	this->type8fastJump = true;
	return false;
}
bool daEnSecretGem_c::collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther) {
	return false;
}
bool daEnSecretGem_c::collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther) {
	return false;
}

bool daEnSecretGem_c::collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther) {
	return false;
}
bool daEnSecretGem_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther) {

	return false;
}
bool daEnSecretGem_c::collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther) {

	return false;
}
bool daEnSecretGem_c::collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther) {
	return false;
}
bool daEnSecretGem_c::collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther) {
	return false;
}

bool daEnSecretGem_c::collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther) {
	return false;
}


dActor_c *daEnSecretGem_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daEnSecretGem_c));
	daEnSecretGem_c *c = new(buffer) daEnSecretGem_c;

	return c;
}


extern int getNybbleValue(u32 settings, int fromNybble, int toNybble);

int daEnSecretGem_c::onCreate() {
	this->deleteForever = true;

	// Model creation	
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	this->resFile.data = getResource("supergem", "g3d/supergem.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("SuperGem");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Item(&bodyModel, 0);

	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr("Wait");
	this->anm.setup(mdl, anmChr, &this->allocator, 0);

	this->anm.bind(&this->bodyModel, anmChr, 1);
	this->bodyModel.bindAnim(&this->anm, 0.0);
	this->anm.setUpdateRate(1.0);
	

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

	// Stuff I do understand
	this->scale = (Vec){0.15f, 0.15f, 0.15f};

	this->rot.x = 0;
	this->rot.y = 0;
	this->rot.z = 0;

	//this->pos.z = 4000;

	SaveBlock* save = GetSaveFile()->GetBlock(-1);
	OSReport("Secret Gems: %d\n", save->secretGems);
	if(save->secretGems != this->settings & 0xFF) this->Delete(1);
	if(secretGemCollected) this->Delete(1);

	doStateChange(&StateID_Wait);

	this->onExecute();
	return true;
}


int daEnSecretGem_c::onDelete() {
	return true;
}

int daEnSecretGem_c::onDraw() {
	bodyModel.scheduleForDrawing();
	return true;
}


void daEnSecretGem_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y - 8, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

int daEnSecretGem_c::onExecute() {
	bodyModel._vf1C();
	updateModelMatrices();
	acState.execute();

	//this->rot.y += 100;

	/*if (this->anm.isAnimationDone()) {
		this->anm.setCurrentFrame(0.0);
	}*/

	//this->removeMyActivePhysics();
	//this->Delete(1);

	return true;
}