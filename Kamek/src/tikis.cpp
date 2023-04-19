#include <common.h>
#include <game.h>
#include <profile.h>
#include "path.h"


const char* BanjoFileList[] = { "Banjo", 0 };
const char* CordianFileList[] = { "Cordian", 0 };
const char* GongFileList[] = { "Gong", 0 };
const char* GoonFileList[] = { "Goon", 0 };
const char* KalimbaFileList[] = { "Kalimba", 0 };
const char* WackyPipesFileList[] = { "WackyPipes", 0 };
const char* XyloboneFileList[] = { "Xylobone", 0 };


const char** TikisFileLists[] = {BanjoFileList, CordianFileList, GongFileList, GoonFileList, KalimbaFileList, WackyPipesFileList, XyloboneFileList};


class daEnTikis_c : public dEnPath_c {
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

	bool leftRight;

	bool facingRight;
	u8 wSpeed;
	u8 wDistance;

	int stepCount;

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


const SpriteData BanjoSpriteData = { ProfileId::Banjo, 8, -8, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
const SpriteData CordianSpriteData = { ProfileId::Cordian, 8, -8, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
const SpriteData GongSpriteData = { ProfileId::Gong, 8, -8, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
const SpriteData GoonSpriteData = { ProfileId::Goon, 8, -8, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
const SpriteData KalimbaSpriteData = { ProfileId::Kalimba, 8, -8, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
const SpriteData WackyPipesSpriteData = { ProfileId::WackyPipes, 8, -8, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
const SpriteData XyloboneSpriteData = { ProfileId::Xylobone, 8, -8, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile BanjoProfile(&daEnTikis_c::build, SpriteId::Banjo, &BanjoSpriteData, ProfileId::Banjo, ProfileId::Banjo, "Banjo", BanjoFileList);
Profile CordianProfile(&daEnTikis_c::build, SpriteId::Cordian, &CordianSpriteData, ProfileId::Cordian, ProfileId::Cordian, "Cordian", CordianFileList);
Profile GongProfile(&daEnTikis_c::build, SpriteId::Gong, &GongSpriteData, ProfileId::Gong, ProfileId::Gong, "Gong", GongFileList);
Profile GoonProfile(&daEnTikis_c::build, SpriteId::Goon, &GoonSpriteData, ProfileId::Goon, ProfileId::Goon, "Goon", GoonFileList);
Profile KalimbaProfile(&daEnTikis_c::build, SpriteId::Kalimba, &KalimbaSpriteData, ProfileId::Kalimba, ProfileId::Kalimba, "Kalimba", KalimbaFileList);
Profile WackyPipesProfile(&daEnTikis_c::build, SpriteId::WackyPipes, &WackyPipesSpriteData, ProfileId::WackyPipes, ProfileId::WackyPipes, "WackyPipes", WackyPipesFileList);
Profile XyloboneProfile(&daEnTikis_c::build, SpriteId::Xylobone, &XyloboneSpriteData, ProfileId::Xylobone, ProfileId::Xylobone, "Xylobone", XyloboneFileList);



u8 hijackMusicWithSongName(const char* songName, int themeID, bool hasFast, int channelCount, int trackCount, int* wantRealStreamID);

void daEnTikis_c::playerCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	DamagePlayer(this, apThis, apOther);
}
void daEnTikis_c::yoshiCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
}
bool daEnTikis_c::collisionCat7_GroundPound(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->Delete(1);
	return true;
}
bool daEnTikis_c::collisionCat7_GroundPoundYoshi(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->Delete(1);
	return true;
}
bool daEnTikis_c::collisionCatD_Drill(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->Delete(1);
	return true;
}
bool daEnTikis_c::collisionCatA_PenguinMario(ActivePhysics* apThis, ActivePhysics* apOther) {
	if ((this->facingRight && apOther->owner->pos.x < this->pos.x) || (!this->facingRight && apOther->owner->pos.x > this->pos.x))
	{
		this->Delete(1);
		return true;
	}
	return false;
}

bool daEnTikis_c::collisionCat1_Fireball_E_Explosion(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->Delete(1);
	return true;
}
/*bool daEnTikis_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics* apThis, ActivePhysics* apOther) {

	return false;
}*/
bool daEnTikis_c::collisionCat9_RollingObject(ActivePhysics* apThis, ActivePhysics* apOther) {

	return false;
}
bool daEnTikis_c::collisionCat13_Hammer(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->Delete(1);
	return true;
}
bool daEnTikis_c::collisionCat14_YoshiFire(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->Delete(1);
	return true;
}

bool daEnTikis_c::collisionCat3_StarPower(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->Delete(1);
	return true;
}


void daEnTikis_c::_vf148() {
	dEn_c::_vf148();
}

void daEnTikis_c::_vf14C() {
	dEn_c::_vf14C();
}

//this does weird shit, but it creates the iceblock around it
bool daEnTikis_c::CreateIceActors() {
	struct DoSomethingCool my_struct = { 0, this->pos, {1.5, 2.0, 1.8}, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
	this->frzMgr.Create_ICEACTORs((void*)&my_struct, 1);
	__destroy_arr((void*)&my_struct, sub_80024C20, 0x3C, 1);
	//this->chrAnimation.setCurrentFrame(0.0);
	this->chrAnimation.setUpdateRate(0.0);
	//this->frozen = true;
	return true;
}


dActor_c* daEnTikis_c::build() {
	void* buffer = AllocFromGameHeap1(sizeof(daEnTikis_c));
	daEnTikis_c* c = new(buffer) daEnTikis_c;

	return c;
}


extern int getNybbleValue(u32 settings, int fromNybble, int toNybble);

void daEnTikis_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate)
{
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr(name);
	this->chrAnimation.bind(&this->bodyModel, anmChr, unk);
	this->bodyModel.bindAnim(&this->chrAnimation, unk2);
	this->chrAnimation.setUpdateRate(rate);
}

int daEnTikis_c::onCreate() {
	this->deleteForever = true;

	// Model creation
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	int model = this->name - ProfileId::Banjo;

	char tikiBrres[50];
	sprintf(tikiBrres, "g3d/%s.brres", TikisFileLists[model][0]);

	this->resFile.data = getResource(TikisFileLists[model][0], tikiBrres);
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("tiki");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Player(&bodyModel, 0);

	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr("fly");
	this->chrAnimation.setup(mdl, anmChr, &this->allocator, 0);

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

	//OSReport("fr: %d, wD: %d, wS: %d\n", this->facingRight, this->wDistance, this->wSpeed);


	// Stuff I do understand
	this->scale = (Vec){ 0.1, 0.1, 0.1 };

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


	bindAnimChr_and_setUpdateRate("fly", 1, 0.0, 1.0);

	OSReport("LeftRight: %d\n", this->leftRight);
	if(this->leftRight) {
		doStateChange(&StateID_Wait);
	} else {
		beginState_Init();
		executeState_Init();
		
		if (this->stepVector.x > 0) {
			this->rot.y = 0x4000;
		}
		
		OSReport("currentNodeNum: %d\n", this->currentNodeNum);
		OSReport("pathID: %d\n", this->pathID);
		doStateChange(&StateID_FollowPath);
	}

	this->onExecute();
	return true;
}


int daEnTikis_c::onDelete() {
	return true;
}

int daEnTikis_c::onDraw() {
	bodyModel.scheduleForDrawing();
	return true;
}


void daEnTikis_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

int daEnTikis_c::onExecute() {
	bodyModel._vf1C();
	updateModelMatrices();

	//OSReport("fr: %d, wD: %d, wS: %d, stepCount: %d\n", this->facingRight, this->wDistance, this->wSpeed, this->stepCount);

	if(this->leftRight) {
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
	} else {
		acState.execute();
		if (this->stepsDone == this->stepCount) {
			if (this->rotateNext) {
				if (this->stepVector.x > 0) {
					this->rot.y = 0x4000;
				} else {
					this->rot.y = -0x4000;
				}
			}
		}
	}
	if (this->chrAnimation.isAnimationDone()) {
		this->chrAnimation.setCurrentFrame(0.0);
	}

	return true;
}