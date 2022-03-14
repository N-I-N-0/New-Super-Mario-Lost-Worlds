#include <common.h>
#include <game.h>
#include <profile.h>
#include <stage.h>
#include "baddy.h"

const char* PullStarFileList[] = { "launchStar", 0 };

class daEnPullStar_c : public dEn_c {
public:
	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	mHeapAllocator_c allocator;
	nw4r::g3d::ResFile resFile;
	nw4r::g3d::ResFile resFileAnim;
	m3d::mdl_c bodyModel;

	m3d::anmChr_c chrAnimation;

	u8 pullScale;
	u8 pullSpeed;
	
	bool active;
	bool isActivatedByEvent;

	int id;

	bool afterCheckpoint;

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
};

const SpriteData PullStarSpriteData = { ProfileId::PullStar, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile PullStarProfile(&daEnPullStar_c::build, SpriteId::PullStar, &PullStarSpriteData, ProfileId::PullStar, ProfileId::PullStar, "PullStar", PullStarFileList);

u8 hijackMusicWithSongName(const char* songName, int themeID, bool hasFast, int channelCount, int trackCount, int* wantRealStreamID);

void daEnPullStar_c::playerCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	
}
void daEnPullStar_c::yoshiCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	//this->playerCollision(apThis, apOther);
}
bool daEnPullStar_c::collisionCat7_GroundPound(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnPullStar_c::collisionCat7_GroundPoundYoshi(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnPullStar_c::collisionCatD_Drill(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnPullStar_c::collisionCatA_PenguinMario(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnPullStar_c::collisionCat1_Fireball_E_Explosion(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnPullStar_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnPullStar_c::collisionCat9_RollingObject(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnPullStar_c::collisionCat13_Hammer(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnPullStar_c::collisionCat14_YoshiFire(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnPullStar_c::collisionCat3_StarPower(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}


dActor_c* daEnPullStar_c::build() 
{
	void* buffer = AllocFromGameHeap1(sizeof(daEnPullStar_c));
	daEnPullStar_c* c = new(buffer) daEnPullStar_c;

	return c;
}


extern int getNybbleValue(u32 settings, int fromNybble, int toNybble);

void daEnPullStar_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate) 
{
	nw4r::g3d::ResAnmChr anmChr = this->resFileAnim.GetResAnmChr(name);
	this->chrAnimation.bind(&this->bodyModel, anmChr, unk);
	this->bodyModel.bindAnim(&this->chrAnimation, unk2);
	this->chrAnimation.setUpdateRate(rate);
}

int daEnPullStar_c::onCreate() 
{
	OSReport("Starting daEnPullStar_c::onCreate()\n");
	
	// Model creation	
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	nw4r::g3d::ResMdl mdl;

	this->resFile.data = getResource("launchStar", "g3d/pullPull.brres");
	mdl = this->resFile.GetResMdl("pullPull");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Player(&bodyModel, 0);

	//TODO annimation of Pullstar not Launchstar

	this->resFileAnim.data = getResource("launchStar", "g3d/launchStarAnim.brres");
	nw4r::g3d::ResAnmChr anmChr = this->resFileAnim.GetResAnmChr("idle");
	this->chrAnimation.setup(mdl, anmChr, &this->allocator, 0);
	
	allocator.unlink();

	ActivePhysics::Info HitMeBaby;

	HitMeBaby.xDistToCenter = 0.0;
	HitMeBaby.yDistToCenter = 0.0;

	HitMeBaby.xDistToEdge = 15.0;
	HitMeBaby.yDistToEdge = 15.0;

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
	this->scale = (Vec){ 0.13, 0.13, 0.13 };

	this->rot.x = 0;
	this->rot.y = 0;
	this->rot.z = 0;

	this->pos.z = -4000;

	this->active = this->settings >> 30 & 1;								
	this->isActivatedByEvent = this->settings >> 29 & 1;					

	this->id = this->settings >> 24 & 0b11111;								

	this->pullScale = this->settings >> 18 & 0b111111;							
	this->pullSpeed = this->settings >> 12 & 0b111111;

	this->scale = (Vec){ 0.13f,0.13f,0.13f };

	this->afterCheckpoint = this->settings >> 3 & 1;

	OSReport("ID: %d\n", this->id);

	OSReport("Eight Launch One: %d\n", GameMgrP->eight.checkpointEntranceID);

	checkStarChipReset(afterCheckpoint);

	this->onExecute();
	return true;
}

int daEnPullStar_c::onDelete() 
{
	return true;
}

int daEnPullStar_c::onDraw() 
{
	bodyModel.scheduleForDrawing();
	return true;
}

void daEnPullStar_c::updateModelMatrices() 
{
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

int daEnPullStar_c::onExecute()
{
	bodyModel._vf1C();
	updateModelMatrices();
	chrAnimation.setUpdateRate(1.0f);
	if (this->chrAnimation.isAnimationDone())
	{
		this->chrAnimation.setCurrentFrame(0.0);
	}

	OSReport("CheckpointActivated: %d\n", chekpointActivated);

	if (this->active)
	{
		this->scale = (Vec){ 0.13f,0.13f,0.13f };
		return true;
	}
	else
	{
		this->scale = (Vec){ 0, 0, 0 };

		OSReport("-------------------------------------------------------------------------------\n");
		OSReport("Eight Pull two: %d\n", GameMgrP->eight.checkpointEntranceID);
		OSReport("Collected After 1: %d\n", launchStarChipCollectedAfterFlag[this->id][0]);
		OSReport("Collected After 2: %d\n", launchStarChipCollectedAfterFlag[this->id][1]);
		OSReport("Collected After 3: %d\n", launchStarChipCollectedAfterFlag[this->id][2]);
		OSReport("Collected After 4: %d\n", launchStarChipCollectedAfterFlag[this->id][3]);
		OSReport("Collected After 5: %d\n", launchStarChipCollectedAfterFlag[this->id][4]);
		OSReport(" \n");
		OSReport("Collected Before 1: %d\n", launchStarChipCollectedBeforeFlag[this->id][0]);
		OSReport("Collected Before 2: %d\n", launchStarChipCollectedBeforeFlag[this->id][1]);
		OSReport("Collected Before 3: %d\n", launchStarChipCollectedBeforeFlag[this->id][2]);
		OSReport("Collected Before 4: %d\n", launchStarChipCollectedBeforeFlag[this->id][3]);
		OSReport("Collected Before 5: %d\n", launchStarChipCollectedBeforeFlag[this->id][4]);
		OSReport("-------------------------------------------------------------------------------\n");

		for (int i = 0; i < 5; i++)
		{
			if (launchStarChipCollectedAfterFlag[this->id][i] != true && launchStarChipCollectedBeforeFlag[this->id][i] != true)
			{
				return true;
			}
		}

		this->active = true;
		return true;
	}
}