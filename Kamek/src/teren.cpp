#include <common.h>
#include <game.h>
#include <profile.h>
#include "path.h"

const char* TerenFileList[] = { "Teren", 0 };


class daEnTeren_c : public dEnPath_c {
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

	u8 wSpeed;
	u8 wDistance;

	int stepCount;

	bool ranOnce;

	static dActor_c* build();

	void bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate);

	void updateModelMatrices();
	void playerCollision(ActivePhysics* apThis, ActivePhysics* apOther);
//	void yoshiCollision(ActivePhysics* apThis, ActivePhysics* apOther);

	bool collisionCat3_StarPower(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCatD_Drill(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat7_GroundPound(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat7_GroundPoundYoshi(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat9_RollingObject(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat1_Fireball_E_Explosion(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat13_Hammer(ActivePhysics* apThis, ActivePhysics* apOther);

	//setDeathInfo_IceBreak -> what should be done when in an iceblock and crashes a wall -> normally the sprite should die
	void _vf148();
	//setDeathInfo_IceVanish -> what should be done when collision with an iceblock thrown by Mario -> normally the sprite should die
	void _vf14C();
	//create an ice block when collided with Iceball
	bool CreateIceActors();

	USING_STATES(daEnTeren_c);

	//void addScoreWhenHit(void* other);
};

const SpriteData TerenSpriteData = { ProfileId::Teren, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile TerenProfile(&daEnTeren_c::build, SpriteId::Teren, &TerenSpriteData, ProfileId::Teren, ProfileId::Teren, "Teren", TerenFileList);



u8 hijackMusicWithSongName(const char* songName, int themeID, bool hasFast, int channelCount, int trackCount, int* wantRealStreamID);


void daEnTeren_c::playerCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	dAcPy_c* player = (dAcPy_c *)apOther->owner;
	if (!player->isNoDamage()) {
		player->setDamage(this, 0);
	}
	return;
}

#include <dMultiMng_c.h>
bool daEnTeren_c::collisionCat3_StarPower(ActivePhysics* apThis, ActivePhysics* apOther) {
	u32 playerId;
	int comboCount;
	dAcPy_c *player;

	SpawnEffect("Wm_en_teresavanish", 0, &pos, &(S16Vec){0,0,0}, &(Vec){1.0, 1.0, 1.0});

	player = (dAcPy_c*)(apOther->owner);
	playerId = *(player->getPlrNo());
	u32 starCount = player->daPlBase_c::getStarCount();
	player->playKameHitSound(starCount, 0);
	comboCount = player->getComboCount(); //8009f630
	dScoreMng_c::instance->dScoreMng_c::sub_800E2190(this, 0.0, 24.0, comboCount, playerId);
	if (playerId != -1) {
		dMultiMng_c::instance->dMultiMng_c::incEnemyDown(playerId);
	}

	PlaySoundAsync(this, SE_EMY_TERESA_DEAD);
	this->Delete(1);
	return true;
}



bool daEnTeren_c::collisionCatD_Drill(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->playerCollision(apThis, apOther);
	return false;
}

bool daEnTeren_c::collisionCat7_GroundPound(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->playerCollision(apThis, apOther);
	return false;
}
bool daEnTeren_c::collisionCat7_GroundPoundYoshi(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->playerCollision(apThis, apOther);
	return false;
}


bool daEnTeren_c::collisionCat9_RollingObject(ActivePhysics* apThis, ActivePhysics* apOther) {
	return dEn_c::collisionCat9_RollingObject(apThis, apOther);
}

bool daEnTeren_c::collisionCat1_Fireball_E_Explosion(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->dEn_c::fireballInvalid(apThis, apOther);
	return false;
}

bool daEnTeren_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->dEn_c::iceballInvalid(apThis, apOther);
	return false;
}

bool daEnTeren_c::collisionCat13_Hammer(ActivePhysics* apThis, ActivePhysics* apOther) {
	u32 playerId;
	int comboCount;
	dAcPy_c *player;
	dStageActor_c* hammer;

	SpawnEffect("Wm_en_teresavanish", 0, &pos, &(S16Vec){0,0,0}, &(Vec){1.0, 1.0, 1.0});

	hammer = (dStageActor_c*)(apOther->owner);
	playerId = *(hammer->getPlrNo());
	player = dAcPy_c::findByID(playerId);
	u32 starCount = player->daPlBase_c::getStarCount();
	player->playKameHitSound(starCount, 0);
	comboCount = player->getComboCount(); //8009f630
	dScoreMng_c::instance->dScoreMng_c::sub_800E2190(this, 0.0, 24.0, comboCount, playerId);
	if (playerId != -1) {
		dMultiMng_c::instance->dMultiMng_c::incEnemyDown(playerId);
	}

	PlaySoundAsync(this, SE_EMY_TERESA_DEAD);
	this->Delete(1);
	return true;
}

void daEnTeren_c::_vf148() {
	dEn_c::_vf148();
	this->Delete(1);
}

void daEnTeren_c::_vf14C() {
	dEn_c::_vf14C();
	this->Delete(1);
}

//this does weird shit, but it creates the iceblock around it
bool daEnTeren_c::CreateIceActors() {
	struct DoSomethingCool my_struct = { 0, (Vec){pos.x, pos.y - 12, pos.z}, {1.7, 1.4, 2.0}, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
	this->frzMgr.Create_ICEACTORs((void*)&my_struct, 1);
	__destroy_arr((void*)&my_struct, sub_80024C20, 0x3C, 1);
	this->chrAnimation.setCurrentFrame(0.0);
	this->chrAnimation.setUpdateRate(0.0);
	//this->frozen = true;
	return true;
}


dActor_c* daEnTeren_c::build() {
	void* buffer = AllocFromGameHeap1(sizeof(daEnTeren_c));
	daEnTeren_c* c = new(buffer) daEnTeren_c;

	return c;
}


extern int getNybbleValue(u32 settings, int fromNybble, int toNybble);

void daEnTeren_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate)
{
	nw4r::g3d::ResAnmChr anmChr = this->resFileAnim.GetResAnmChr(name);
	this->chrAnimation.bind(&this->bodyModel, anmChr, unk);
	this->bodyModel.bindAnim(&this->chrAnimation, unk2);
	this->chrAnimation.setUpdateRate(rate);
}

int daEnTeren_c::onCreate() {
	if(!this->ranOnce) {
		this->ranOnce = true;
		return false;
	}
	
	this->deleteForever = false; //makes the death state being used in preExecute

	// Model creation	
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	this->resFile.data = getResource("Teren", "g3d/Teren.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("Teren");
	bodyModel.setup(mdl, &allocator, 0x227, 1, 0);
	SetupTextures_Enemy(&bodyModel, 0);

	this->resFileAnim.data = getResource("Teren", "g3d/Teren.brres");
	nw4r::g3d::ResAnmChr anmChr = this->resFileAnim.GetResAnmChr("Wait");
	this->chrAnimation.setup(mdl, anmChr, &this->allocator, 0);

	allocator.unlink();

	ActivePhysics::Info HitMeBaby;

	HitMeBaby.xDistToCenter = 0.0;
	HitMeBaby.yDistToCenter = 0.0;

	HitMeBaby.xDistToEdge = 7.5;
	HitMeBaby.yDistToEdge = 7.5;

	HitMeBaby.category1 = 0x3;
	HitMeBaby.category2 = 0x0;
	HitMeBaby.bitfield1 = 0x47;
	HitMeBaby.bitfield2 = 0x0008A80E;
	HitMeBaby.unkShort1C = 0x2002;
	HitMeBaby.callback = &dEn_c::collisionCallback;

	this->aPhysics.initWithStruct(this, &HitMeBaby);
	this->aPhysics.addToList();


	this->wDistance = settings >> 20 & 0b11111111;
	this->wSpeed = settings >> 16 & 0b1111;

	this->scale = (Vec){1.0, 1.0, 1.0};

	this->rot.x = 0;
	this->rot.y = 0;
	this->rot.z = 0;

	this->pos.z = 4000;

	bindAnimChr_and_setUpdateRate("Wait", 1, 0.0, 1.0);

	beginState_Init();
	executeState_Init();
	doStateChange(&StateID_FollowPath);

	this->onExecute();
	return true;
}


int daEnTeren_c::onDelete() {
	nw4r::snd::SoundHandle handle;
	PlaySoundWithFunctionB4(SoundRelatedClass, &handle, SE_EMY_DOWN, 1);
	StageE4::instance->spawnCoinJump(pos, 0, 1, 0);
	return true;
}

int daEnTeren_c::onDraw() {
	bodyModel.scheduleForDrawing();
	return true;
}


void daEnTeren_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y - 8, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

int daEnTeren_c::onExecute() {
	bodyModel._vf1C();
	updateModelMatrices();


	acState.execute();

	if (this->chrAnimation.isAnimationDone()) {
		this->chrAnimation.setCurrentFrame(0.0);
	}

	return true;
}