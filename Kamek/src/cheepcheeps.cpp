#include <common.h>
#include <game.h>
#include <profile.h>
#include "path.h"

const char* CheepCheepModelFileList[] = { "pukupuku", 0 };

class daEnCheepCheepController_c;

class daEnCheepCheep_c : public dEn_c {
public:
	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	mHeapAllocator_c allocator;
	nw4r::g3d::ResFile resFile;
	//nw4r::g3d::ResFile resFileAnim;
	m3d::mdl_c bodyModel;

	nw4r::g3d::ResAnmTexPat anmPat;

	m3d::anmChr_c chrAnimation;
	m3d::anmTexPat_c patAnimation;

	int index;
	daEnCheepCheepController_c* parent;


	static dActor_c* build();

	void bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate);

	void setColor(u32 color);

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

	//setDeathInfo_IceBreak -> what should be done when in an iceblock and crashes a wall -> normally the sprite should die
	void _vf148();
	//setDeathInfo_IceVanish -> what should be done when collision with an iceblock thrown by Mario -> normally the sprite should die
	void _vf14C();
	//create an ice block when collided with Iceball
	bool CreateIceActors();

	USING_STATES(daEnCheepCheep_c);
	DECLARE_STATE(Wait);

	//void addScoreWhenHit(void* other);
};

CREATE_STATE(daEnCheepCheep_c, Wait);

void daEnCheepCheep_c::beginState_Wait() {}
void daEnCheepCheep_c::executeState_Wait() {}
void daEnCheepCheep_c::endState_Wait() {}

class daEnCheepCheepController_c : public dStageActor_c {
public:
	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();
	
	int startRadius;
	int laneDistance;
	int numberLanes;
	int numberCheepsPerLane;
	int secondsPerRound;
	u32 color;
	
	int frame;
	
	daEnCheepCheep_c* cheepcheeps[200];

	static dActor_c* build();
};

//const SpriteData CheepCheepModelSpriteData = { ProfileId::CheepCheepModel, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile CheepCheepModelProfile(&daEnCheepCheep_c::build, ProfileId::CheepCheepModel, NULL, ProfileId::CheepCheepModel, ProfileId::CheepCheepModel, "CheepCheepModel", CheepCheepModelFileList);



u8 hijackMusicWithSongName(const char* songName, int themeID, bool hasFast, int channelCount, int trackCount, int* wantRealStreamID);


void daEnCheepCheep_c::playerCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	DamagePlayer(this, apThis, apOther);
}
void daEnCheepCheep_c::yoshiCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	DamagePlayer(this, apThis, apOther);
}

bool daEnCheepCheep_c::collisionCat7_GroundPound(ActivePhysics* apThis, ActivePhysics* apOther) {
	bool ret = dEn_c::collisionCat7_GroundPound(apThis, apOther);
	if (ret) {
		this->parent->cheepcheeps[this->index] = NULL;
	}
	return ret;
}
bool daEnCheepCheep_c::collisionCat7_GroundPoundYoshi(ActivePhysics* apThis, ActivePhysics* apOther) {
	bool ret = dEn_c::collisionCat7_GroundPoundYoshi(apThis, apOther);
	if (ret) {
		this->parent->cheepcheeps[this->index] = NULL;
	}
	return ret;
}
bool daEnCheepCheep_c::collisionCatD_Drill(ActivePhysics* apThis, ActivePhysics* apOther) {
	bool ret = dEn_c::collisionCatD_Drill(apThis, apOther);
	if (ret) {
		this->parent->cheepcheeps[this->index] = NULL;
	}
	return ret;
}
bool daEnCheepCheep_c::collisionCatA_PenguinMario(ActivePhysics* apThis, ActivePhysics* apOther) {
	bool ret = dEn_c::collisionCatA_PenguinMario(apThis, apOther);
	if (ret) {
		this->parent->cheepcheeps[this->index] = NULL;
	}
	return ret;
}

bool daEnCheepCheep_c::collisionCat1_Fireball_E_Explosion(ActivePhysics* apThis, ActivePhysics* apOther) {
	bool ret = dEn_c::collisionCat1_Fireball_E_Explosion(apThis, apOther);
	if (ret) {
		this->parent->cheepcheeps[this->index] = NULL;
	}
	return ret;
}
bool daEnCheepCheep_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics* apThis, ActivePhysics* apOther) {
	bool ret = dEn_c::collisionCat2_IceBall_15_YoshiIce(apThis, apOther);
	if (ret) {
		this->parent->cheepcheeps[this->index] = NULL;
	}
	return ret;
}
bool daEnCheepCheep_c::collisionCat9_RollingObject(ActivePhysics* apThis, ActivePhysics* apOther) {
	bool ret = dEn_c::collisionCat9_RollingObject(apThis, apOther);
	if (ret) {
		this->parent->cheepcheeps[this->index] = NULL;
	}
	return ret;
}
bool daEnCheepCheep_c::collisionCat13_Hammer(ActivePhysics* apThis, ActivePhysics* apOther) {
	bool ret = dEn_c::collisionCat13_Hammer(apThis, apOther);
	if (ret) {
		this->parent->cheepcheeps[this->index] = NULL;
	}
	return ret;
}
bool daEnCheepCheep_c::collisionCat14_YoshiFire(ActivePhysics* apThis, ActivePhysics* apOther) {
	bool ret = dEn_c::collisionCat14_YoshiFire(apThis, apOther);
	if (ret) {
		this->parent->cheepcheeps[this->index] = NULL;
	}
	return ret;
}
bool daEnCheepCheep_c::collisionCat3_StarPower(ActivePhysics* apThis, ActivePhysics* apOther) {
	bool ret = dEn_c::collisionCat3_StarPower(apThis, apOther);
	if (ret) {
		this->parent->cheepcheeps[this->index] = NULL;
	}
	return ret;
}

void daEnCheepCheep_c::_vf148() {
	dEn_c::_vf148();
	this->parent->cheepcheeps[this->index] = NULL;
	this->Delete(1);
}

void daEnCheepCheep_c::_vf14C() {
	dEn_c::_vf14C();
	this->parent->cheepcheeps[this->index] = NULL;
	this->Delete(1);
}

//this does weird shit, but it creates the iceblock around it
bool daEnCheepCheep_c::CreateIceActors() {
	this->pos.z -= 500;
	struct DoSomethingCool my_struct = { 0, (Vec){pos.x, pos.y - 20, pos.z}, {1.4, 1.4, 2.0}, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
	this->frzMgr.Create_ICEACTORs((void*)&my_struct, 1);
	__destroy_arr((void*)&my_struct, sub_80024C20, 0x3C, 1);
	this->chrAnimation.setCurrentFrame(0.0);
	this->chrAnimation.setUpdateRate(0.0);
	//this->frozen = true;
	return true;
}


dActor_c* daEnCheepCheep_c::build() {
	void* buffer = AllocFromGameHeap1(sizeof(daEnCheepCheep_c));
	daEnCheepCheep_c* c = new(buffer) daEnCheepCheep_c;

	return c;
}


extern int getNybbleValue(u32 settings, int fromNybble, int toNybble);

void daEnCheepCheep_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate)
{
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr(name);
	this->chrAnimation.bind(&this->bodyModel, anmChr, unk);
	this->bodyModel.bindAnim(&this->chrAnimation, unk2);
	this->chrAnimation.setUpdateRate(rate);
}

int daEnCheepCheep_c::onCreate() {
	this->deleteForever = false; //makes the death state being used in preExecute

	// Model creation	
	allocator.link(-1, GameHeaps[0], 0, 0x20);
	this->resFile.data = getResource("pukupuku", "g3d/pukupuku.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("pukupuku");
	bodyModel.setup(mdl, &allocator, 0x227, 1, 0);
	SetupTextures_Enemy(&bodyModel, 0);
	
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr("swim");
	this->chrAnimation.setup(mdl, anmChr, &this->allocator, 0);

	this->anmPat = this->resFile.GetResAnmTexPat("pukupuku");
	this->patAnimation.setup(mdl, anmPat, &this->allocator, 0, 1);
	this->patAnimation.bindEntry(&this->bodyModel, &anmPat, 0, 1);
	this->patAnimation.setFrameForEntry(0, 0);//((this->eventId1 & 0b111) % 5), 0);
	this->patAnimation.setUpdateRateForEntry(0.0f, 0);
	this->bodyModel.bindAnim(&this->patAnimation);

	allocator.unlink();

	ActivePhysics::Info HitMeBaby;

	HitMeBaby.xDistToCenter = 0.0;
	HitMeBaby.yDistToCenter = -7.5;

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

	//this->scale = (Vec){0.4, 0.4, 0.4};
	//this->pos.z = 4000;

	/* Copied from NSMASRv2
	Unk1 = 1, // Something with shells sliding on expanding blocks, couldn't find a difference
	AltKillEffect = 2, // Spawns a different effect when spin-jumped on
	ChainlinkIce = 4, // Sets ice's chainlinkMode based on the appearsOnBackfence variable
	CoinBattleExtraSFX = 0x10, // Plays an extra sound effect if killed in coin battle
	KillWhenFrozenUnderwater = 0x10000,
	DestroyIceIfFrozenMidair = 0x20000, // Spawns a coin when destroyed
	NoPlayerDamage = 0x1000000,
	*/

	this->flags_4FC |= 0x10000; //IceBitfield: KillWhenFrozenUnderwater

	bindAnimChr_and_setUpdateRate("swim", 1, 0.0, 1.0);

	doStateChange(&StateID_Wait);

	return true;
}


int daEnCheepCheep_c::onDelete() {
	this->parent->cheepcheeps[this->index] = NULL;
	/*nw4r::snd::SoundHandle handle;
	PlaySoundWithFunctionB4(SoundRelatedClass, &handle, SE_EMY_DOWN, 1);
	StageE4::instance->spawnCoinJump(pos, 0, 1, 0);*/
	return true;
}

int daEnCheepCheep_c::onDraw() {
	bodyModel.scheduleForDrawing();
	return true;
}


void daEnCheepCheep_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y - 8, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

int daEnCheepCheep_c::onExecute() {
	bodyModel._vf1C();
	updateModelMatrices();

	acState.execute();
	
	
	
	if (this->chrAnimation.isAnimationDone()) {
		this->chrAnimation.setCurrentFrame(0.0);
	}

	return true;
}


void daEnCheepCheep_c::setColor(u32 color) {
	this->patAnimation.setFrameForEntry(color, 0);
}




















const SpriteData CheepCheepControlerSpriteData = { ProfileId::CheepCheepController, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile CheepCheepControllerProfile(&daEnCheepCheepController_c::build, SpriteId::CheepCheepController, &CheepCheepControlerSpriteData, ProfileId::CheepCheepController, ProfileId::CheepCheepController, "CheepCheepController", CheepCheepModelFileList);


dActor_c* daEnCheepCheepController_c::build() {
	void* buffer = AllocFromGameHeap1(sizeof(daEnCheepCheepController_c));
	daEnCheepCheepController_c* c = new(buffer) daEnCheepCheepController_c;

	return c;
}

int daEnCheepCheepController_c::onCreate() {
	
	this->startRadius = (this->settings & 0xF) *24;
	this->laneDistance = (this->settings >> 4 & 0xF) * 20;
	this->numberLanes = this->settings >> 8 & 0xF;
	this->numberCheepsPerLane = this->settings >> 12 & 0xF;
	this->secondsPerRound = (this->settings >> 16 & 0xF) * 30; //converted into frames with factor 2 from sin/cos calculated into it so we can reduce additional calculations in onExecute
	this->direction = this->settings >> 20 & 1;
	this->color = this->settings >> 24 & 3;
	
	/*OSReport("startRadius: %d\n", this->startRadius);
	OSReport("laneDistance: %d\n", this->laneDistance);
	OSReport("numberLanes: %d\n", this->numberLanes);
	OSReport("numberCheepsPerLane: %d\n", this->numberCheepsPerLane);
	OSReport("secondsPerRound: %d\n", this->secondsPerRound);
	OSReport("direction: %d\n", this->direction);
	OSReport("color: %d\n", this->color);*/

	this->frame = 0;
	
	if (this->direction) {
		this->secondsPerRound = -this->secondsPerRound;
	}
	
	int j = 0;
	int end = this->numberLanes*this->numberCheepsPerLane;
	for (int i = 0; i < end; i++) {
		Vec cheepPos = this->pos;
		cheepPos.x += (this->startRadius+j*this->laneDistance)*cos(i*2*M_PI/this->numberLanes);
		cheepPos.y += (this->startRadius+j*this->laneDistance)*sin(i*2*M_PI/this->numberLanes);

		cheepcheeps[i] = (daEnCheepCheep_c*)dActor_c::create(CheepCheepModel, 0, &cheepPos, 0);
		cheepcheeps[i]->index = i;
		cheepcheeps[i]->parent = this;
		if(this->color < 3) {
			cheepcheeps[i]->setColor(this->color);
		} else {
			cheepcheeps[i]->setColor(i % 3);
		}

		if ((i+1) % this->numberLanes == 0) {
			j++;
		}
	}

	return true;
}


int daEnCheepCheepController_c::onDelete() {
	return true;
}

int daEnCheepCheepController_c::onDraw() {
	return true;
}




int daEnCheepCheepController_c::onExecute() {
	this->frame++;
	int j = 0;
	int end = this->numberLanes*this->numberCheepsPerLane;
	for (int i = 0; i < end; i++) {
		if(cheepcheeps[i]) {
			float the_sin = sin(i*2*M_PI/this->numberLanes + this->frame*M_PI/secondsPerRound);
			float the_cos = cos(i*2*M_PI/this->numberLanes + this->frame*M_PI/secondsPerRound);
			cheepcheeps[i]->pos.x = this->pos.x + (this->startRadius+j*this->laneDistance)*the_cos; //*2 / 720 => 1 round per 6 seconds
			cheepcheeps[i]->pos.y = this->pos.y + (this->startRadius+j*this->laneDistance)*the_sin;

			s16 cheepRotX = int(the_cos / M_PI * 0x8000);
			s16 cheepRotY = int(the_sin / M_PI * 0x8000);

			if(!this->direction) {
				cheepRotX *= -1;
				cheepRotY *= -1;
			}

			cheepcheeps[i]->rot.x = cheepRotX;
			cheepcheeps[i]->rot.y = cheepRotY;
		}

		if ((i+1) % this->numberLanes == 0) {
			j++;
		}
	}
		
	return true;
}




bool daEnJumpPuku_cCreateIceActors(dEn_c* self) {
	self->aPhysics.removeFromList();

	struct DoSomethingCool my_struct = { 0, (Vec){self->pos.x, self->pos.y - 13.0, self->pos.z}, {1.6, 1.4, 1.5}, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
	self->frzMgr.Create_ICEACTORs( (void*)&my_struct, 1 );
	__destroy_arr( (void*)&my_struct, sub_80024C20, 0x3C, 1 );
	return true;
}