#include <common.h>
#include <game.h>
#include <profile.h>

#include "daEnItem_c.h"

const char* SuperBubbleFileList[] = { "frogM", "PoisonMushroom", 0 };



class daSuperBubble_c : public dEn_c {
public:
	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	mHeapAllocator_c allocator;
	nw4r::g3d::ResFile resFile;
	nw4r::g3d::ResFile eggFile;
	m3d::mdl_c bodyModel;

	nw4r::g3d::ResAnmTexPat anmPat;

	m3d::anmChr_c chrAnimation;
	m3d::anmTexPat_c patAnimation;

	u8 yoshiColor;

	int alreadyOnTop;

	float XSpeed;
	bool canI;
	u32 cmgr_returnValue;
	int timer;
	int variation;
	bool burstFromCollision;

	m3d::anmChr_c animationChr;
	m3d::mdl_c coinModel;
	m3d::anmChr_c animationCoinChr;
	mMtx coinmatrix;
	VEC3 coinscale;
	S16Vec coinrot;
	bool isCoinSpawned;

	Actors actorID;
	dStageActor_c *content;

	static dActor_c* build();

	void bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate, bool isCoin);

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

	USING_STATES(daSuperBubble_c);
	DECLARE_STATE(Bubble_Thrown);
};

const SpriteData SuperBubbleSpriteData = { ProfileId::SuperBubble, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile SuperBubbleProfile(&daSuperBubble_c::build, SpriteId::SuperBubble, &SuperBubbleSpriteData, ProfileId::SuperBubble, ProfileId::SuperBubble, "SuperBubble", SuperBubbleFileList);


CREATE_STATE(daSuperBubble_c, Bubble_Thrown);


void daSuperBubble_c::playerCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	burstFromCollision = true;
	
	PlaySound(this, SE_OBJ_CMN_BALLOON_BREAK);
	if(actorID == EN_COIN) {
		int pid = getNybbleValue(apOther->owner->which_player, 6, 6);
		int coinsettings = 0 | (0x2 << 0) | (2 << 18) | (4 << 9) | (2 << 10) | ((pid + 8) << 16);
		VEC3 newPos = {this->pos.x, this->pos.y - 16, this->pos.z};
		dStageActor_c *coin = dStageActor_c::create(EN_ITEM, coinsettings, &newPos, 0, 0);
		PlaySound(this, SE_OBJ_GET_COIN);
	} else if(actorID == AC_YOSHI_EGG) {
		VEC3 newPos = {this->pos.x, this->pos.y - 16, this->pos.z};
		dStageActor_c *yoshiEgg = dStageActor_c::create(AC_YOSHI_EGG, (1 << 21 | yoshiColor), &newPos, 0, 0);
	}
	Vec efPos = {this->pos.x, this->pos.y, this->pos.z};
	S16Vec nullRot = {0,0,0};
	Vec oneVec = {1.0f, 1.0f, 1.0f};
	SpawnEffect("Wm_mr_balloonburst", 0, &efPos, &nullRot, &oneVec);
	this->Delete(1);
}

void daSuperBubble_c::yoshiCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->playerCollision(apThis, apOther);
}
bool daSuperBubble_c::collisionCat7_GroundPound(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daSuperBubble_c::collisionCat7_GroundPoundYoshi(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daSuperBubble_c::collisionCatD_Drill(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daSuperBubble_c::collisionCatA_PenguinMario(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}

bool daSuperBubble_c::collisionCat1_Fireball_E_Explosion(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daSuperBubble_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daSuperBubble_c::collisionCat9_RollingObject(ActivePhysics* apThis, ActivePhysics* apOther) {

	return false;
}
bool daSuperBubble_c::collisionCat13_Hammer(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daSuperBubble_c::collisionCat14_YoshiFire(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}

bool daSuperBubble_c::collisionCat3_StarPower(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}


dActor_c* daSuperBubble_c::build() {
	void* buffer = AllocFromGameHeap1(sizeof(daSuperBubble_c));
	daSuperBubble_c* c = new(buffer) daSuperBubble_c;

	return c;
}


extern int getNybbleValue(u32 settings, int fromNybble, int toNybble);

void daSuperBubble_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate, bool isCoin) {
	if(!isCoin) {
		nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr(name);
		this->animationChr.bind(&this->bodyModel, anmChr, unk);
		this->bodyModel.bindAnim(&this->animationChr, unk2);
		this->animationChr.setUpdateRate(rate);
	} else {
		nw4r::g3d::ResAnmChr anmCoinChr = this->resFile.GetResAnmChr(name);
		this->animationCoinChr.bind(&this->coinModel, anmCoinChr, unk);
		this->coinModel.bindAnim(&this->animationCoinChr, unk2);
		this->animationCoinChr.setUpdateRate(rate);
	}
}

int daSuperBubble_c::onCreate() {
	this->deleteForever = true;

	Vec tempPos = (Vec){this->pos.x, this->pos.y, this->pos.z};
	
	actorID = EN_ITEM;
	u32 set;
	Vec spawnPos = pos;
	switch(this->settings & 0xFF) {
		case 0:
			return true;
		case 1:
			break;
		case 2:
			set = 0x0B000000;
			break;
		case 3:
			set = 0x0B000009;
			break;
		case 4:
			set = 0x0B000015;
			break;
		case 5:
			set = 0x0B00000E;
			break;
		case 6:
			set = 0x0B000011;
			break;
		case 7:
			set = 0x0B000019;
			break;
		case 8:
			set = 0x0B000001;
			break;
		case 9:
			set = 0x0B000007;
			break;
		case 10:
			set = 0x0B000006;
			break;
		case 11:
			set = 0x0B000014;
			break;
		case 12:
			set = 0x0B000010;
			break;
		case 13:
			set = 0x0B00000F;
			break;
		case 14:
			set = 0x0B000013;
			break;
		case 15:
			set = 0x0B000016;
			break;
		case 16:
			set = 0x0B000012;
			break;
		case 17:
			actorID = AC_YOSHI_EGG;
			//set = 0x008003cc04060000;
			break;
		case 18:
			actorID = EN_COIN;
			//set = 0x008003cc04060000;
			break;
		case 19:
			spawnPos.y += 10;
			actorID = PoisonShroom;
			//set = 0x008003cc04060000;
			break;
		/*case 148:
			set = 0x008003cc04060000;
			break;*/
		default:
			break;
	}
	if (actorID != EN_COIN && actorID != AC_YOSHI_EGG) {
		content = CreateActor(actorID, set, tempPos, 0, 0);
	}

	if (actorID == EN_ITEM) {
		daEnItem_c* item = ((daEnItem_c*)content);
		item->doStateChange(&daEnItem_c::StateID_Wait);
		item->AddPhysics();
	}

	// Model creation
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	this->resFile.data = getResource("frogM", "g3d/balloon.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("balloon");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Enemy(&bodyModel, 0);
	
	
	if (actorID == AC_YOSHI_EGG) {
		yoshiColor = GenerateRandomNumber(10);
		u8 patColor = yoshiColor;
		if (yoshiColor < 4) {
			this->eggFile.data = getResource("I_yoshi_egg", "g3d/t00.brres");
		} else if (yoshiColor < 8) {
			this->eggFile.data = getResource("I_yoshi_egg", "g3d/t01.brres");
			patColor -= 4;
		} else {
			this->eggFile.data = getResource("I_yoshi_egg", "g3d/t02.brres");
			patColor -= 8;
		}
		nw4r::g3d::ResMdl mdl2 = this->eggFile.GetResMdl("I_yoshi_egg");
		coinModel.setup(mdl2, &allocator, 0x227, 1, 0);
		SetupTextures_Item(&coinModel, 0);
		
		this->anmPat = this->eggFile.GetResAnmTexPat("I_yoshi_egg");
		this->patAnimation.setup(mdl2, anmPat, &this->allocator, 0, 1);
		this->patAnimation.bindEntry(&this->coinModel, &anmPat, 0, 1);
		this->patAnimation.setFrameForEntry(patColor, 0);
		this->patAnimation.setUpdateRateForEntry(0.0f, 0);
		this->coinModel.bindAnim(&this->patAnimation);
		
	} else if (actorID == EN_COIN) {
		nw4r::g3d::ResMdl coinmdl = this->resFile.GetResMdl("obj_coin");
		coinModel.setup(coinmdl, &allocator, 0x224, 1, 0);
		SetupTextures_MapObj(&coinModel, 0);
		
		nw4r::g3d::ResAnmChr anmCoinChr = this->resFile.GetResAnmChr("no");
		this->animationCoinChr.setup(coinmdl, anmCoinChr, &this->allocator, 0);

	}


	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr("float");
	this->animationChr.setup(mdl, anmChr, &this->allocator, 0);


	allocator.unlink();

	

	// Stuff I do understand
	this->scale = (Vec){0.6, 0.6, 0.6};
	this->coinscale = (Vec){1.0, 1.0, 1.0};

	this->rot.x = 0;
	this->rot.y = 0;
	this->rot.z = 0;
	this->direction = (this->settings >> 8) & 0xF; // Heading where the player look.
	this->speed.x = (this->direction == 1) ? -5 : 5;
	this->x_speed_inc = 0.1;

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


	// this->pos.y += 16;
	// this->pos.z = 9000;

	OSReport("5\n");

	bindAnimChr_and_setUpdateRate("float", 1, 0.0, 1.0, false);
	if(actorID == EN_COIN) {
		bindAnimChr_and_setUpdateRate("no", 1, 0.0, 1.0, true);
	}

	OSReport("6\n");

	doStateChange(&StateID_Bubble_Thrown);

	OSReport("7\n");

	this->onExecute();

	OSReport("8\n");

	return true;
}


int daSuperBubble_c::onDelete() {
	if(!burstFromCollision) {
		if (actorID != EN_COIN && actorID != AC_YOSHI_EGG) {
			content->Delete(1);
		}
	}
	
	return true;
}

int daSuperBubble_c::onDraw() {
	bodyModel.scheduleForDrawing();
	if (actorID == EN_COIN || actorID == AC_YOSHI_EGG) {
		coinModel.scheduleForDrawing();
	}
	return true;
}


void daSuperBubble_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);
	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
	
	if (actorID == EN_COIN) {
		coinmatrix.translation(pos.x, pos.y, pos.z);
		coinmatrix.applyRotationYXZ(&coinrot.x, &coinrot.y, &coinrot.z);
		coinModel.setDrawMatrix(coinmatrix);
		coinModel.setScale(&coinscale);
		coinModel.calcWorld(false);
	} else if (actorID == AC_YOSHI_EGG) {
		coinmatrix.translation(pos.x, pos.y - 8, pos.z);
		coinmatrix.applyRotationYXZ(&coinrot.x, &coinrot.y, &coinrot.z);
		coinModel.setDrawMatrix(coinmatrix);
		coinModel.setScale(&coinscale);
		coinModel.calcWorld(false);
	} else {
		content->pos = this->pos;
	}
}

int daSuperBubble_c::onExecute() {
	if (actorID != EN_COIN && actorID != AC_YOSHI_EGG) {
		coinModel._vf1C();
	}
	bodyModel._vf1C();
	updateModelMatrices();
	this->timer++;

	if(this->animationChr.isAnimationDone()) {
		this->animationChr.setCurrentFrame(0.0);
	}

	if(actorID == EN_COIN) {
		if(!this->isCoinSpawned) {
			bindAnimChr_and_setUpdateRate("yes", 1, 0.0, 1.0, true);
			this->isCoinSpawned = true;
		}

		if(this->animationCoinChr.isAnimationDone()) {
			this->animationCoinChr.setCurrentFrame(0.0);
		}
		
		this->coinrot.y += 0x400;
	}

	if(this->timer > 300) {
		PlaySound(this, SE_OBJ_CMN_BALLOON_BREAK);
		Vec efPos = {this->pos.x, this->pos.y, this->pos.z};
		S16Vec nullRot = {0,0,0};
		Vec oneVec = {1.0f, 1.0f, 1.0f};
		SpawnEffect("Wm_mr_balloonburst", 0, &efPos, &nullRot, &oneVec);
		this->Delete(1);
	}

	this->pos.y += 0.1;
}




void daSuperBubble_c::beginState_Bubble_Thrown() {}

void daSuperBubble_c::executeState_Bubble_Thrown() {
	HandleXSpeed();
	doSpriteMovement();
	if(this->direction == 1) {
		if(this->speed.x > 0) {
			this->speed.x = 0;
			this->x_speed_inc = 0;
		}
	}
	if(this->direction == 0) {
		if(this->speed.x < 0) {
			this->speed.x = 0;
			this->x_speed_inc = 0;
		}
	}
}

void daSuperBubble_c::endState_Bubble_Thrown() {}