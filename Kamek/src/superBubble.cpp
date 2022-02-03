#include <common.h>
#include <game.h>
#include <profile.h>

const char* SuperBubbleFileList[] = { "frogM", 0 };



class daSuperBubble_c : public dEn_c {
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

	int alreadyOnTop;

	float XSpeed;
	bool canI;
	u32 cmgr_returnValue;
	int timer;
	int variation;

	/******/
	/*Frog*/
	/******/
	m3d::anmChr_c animationChr;
	m3d::mdl_c coinModel;
	m3d::anmChr_c animationCoinChr;
	mMtx coinmatrix;
	VEC3 coinscale;
	S16Vec coinrot;
	bool doIhaveCoin;
	bool isCoinSpawned;

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
	int pid = getNybbleValue(apOther->owner->which_player, 6, 6);
	int coinsettings = 0 | (0x2 << 0) | (2 << 18) | (4 << 9) | (2 << 10) | ((pid + 8) << 16);
	VEC3 newPos = {this->pos.x, this->pos.y - 16, this->pos.z};
	dStageActor_c *coin = dStageActor_c::create(EN_ITEM, coinsettings, &newPos, 0, 0);
	PlaySound(this, SE_OBJ_GET_COIN);
	PlaySound(this, SE_OBJ_CMN_BALLOON_BREAK);
	Vec efPos = {this->pos.x, this->pos.y, this->pos.z};
	S16Vec nullRot = {0,0,0};
	Vec oneVec = {1.0f, 1.0f, 1.0f};
	SpawnEffect("Wm_mr_balloonburst", 0, &efPos, &nullRot, &oneVec);
	this->Delete(1);
}
void daSuperBubble_c::yoshiCollision(ActivePhysics* apThis, ActivePhysics* apOther) {

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
	}
	else {
		nw4r::g3d::ResAnmChr anmCoinChr = this->resFile.GetResAnmChr(name);
		this->animationCoinChr.bind(&this->coinModel, anmCoinChr, unk);
		this->coinModel.bindAnim(&this->animationCoinChr, unk2);
		this->animationCoinChr.setUpdateRate(rate);
	}
}

int daSuperBubble_c::onCreate() {
	this->deleteForever = true;

	OSReport("0\n");
	
	//OSReport("Nybble 1-2: %x\n", this->eventId2);
	//OSReport("Nybble 3-4: %x\n", this->eventId1);
	OSReport("spriteFlagMask: %x\n", (u32)(this->spriteFlagMask >> 32));
	OSReport("spriteFlagMask: %x\n", (u32)this->spriteFlagMask);

	// Model creation
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	this->resFile.data = getResource("frogM", "g3d/balloon.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("balloon");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Enemy(&bodyModel, 0);

	OSReport("1\n");

	nw4r::g3d::ResMdl coinmdl = this->resFile.GetResMdl("obj_coin");
	coinModel.setup(coinmdl, &allocator, 0x224, 1, 0);
	SetupTextures_MapObj(&coinModel, 0);

	OSReport("2\n");

	bool ret;
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr("float");
	ret = this->animationChr.setup(mdl, anmChr, &this->allocator, 0);

	OSReport("3\n");

	bool rett;
	nw4r::g3d::ResAnmChr anmCoinChr = this->resFile.GetResAnmChr("no");
	rett = this->animationCoinChr.setup(coinmdl, anmCoinChr, &this->allocator, 0);

	OSReport("4\n");

	allocator.unlink();

	Vec tempPos = (Vec){this->pos.x, this->pos.y, this->pos.z};
	content = CreateActor(EN_ITEM, 0xA, tempPos, 0, 0);

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
	bindAnimChr_and_setUpdateRate("no", 1, 0.0, 1.0, true);

	OSReport("6\n");

	doStateChange(&StateID_Bubble_Thrown);

	OSReport("7\n");

	this->onExecute();

	OSReport("8\n");

	return true;
}


int daSuperBubble_c::onDelete() {
	return true;
}

int daSuperBubble_c::onDraw() {
	bodyModel.scheduleForDrawing();
	return true;
}


void daSuperBubble_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);
	coinmatrix.translation(pos.x, pos.y, pos.z);
	coinmatrix.applyRotationYXZ(&coinrot.x, &coinrot.y, &coinrot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
	coinModel.setDrawMatrix(coinmatrix);
	coinModel.setScale(&coinscale);
	coinModel.calcWorld(false);
	
	content->pos = this->pos;
}

int daSuperBubble_c::onExecute() {
	
	//OSReport("Nybble 1-2: %x\n", this->eventId2);
	//OSReport("Nybble 3-4: %x\n", this->eventId1);
	OSReport("spriteFlagMask: %x\n", (u32)(this->spriteFlagMask >> 32));
	OSReport("spriteFlagMask: %x\n", (u32)this->spriteFlagMask);
	coinModel._vf1C();
	bodyModel._vf1C();
	updateModelMatrices();
	this->timer++;

	if(this->doIhaveCoin && !this->isCoinSpawned) {
		bindAnimChr_and_setUpdateRate("yes", 1, 0.0, 1.0, true);
		this->isCoinSpawned = true;
	}

	if(this->animationChr.isAnimationDone()) {
		this->animationChr.setCurrentFrame(0.0);
	}
	if(this->animationCoinChr.isAnimationDone()) {
		this->animationCoinChr.setCurrentFrame(0.0);
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
	this->coinrot.y += 0x400;



	for (int i = 0; i < 4; i++) {
		daPlBase_c *player = GetPlayerOrYoshi(i);
		if (player) {
			//player->setFlag(0x71);
			dPlayerModelHandler_c *pmh = (dPlayerModelHandler_c*)(((u32)player) + 0x2A60);
			pmh->mdlClass->startAnimation(132, 1.0f, 0.0f, 0.0f);
			pmh->mdlClass->enableStarColours();
			pmh->mdlClass->enableStarEffects();
		}
	}
}




void daSuperBubble_c::beginState_Bubble_Thrown() {

}

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

void daSuperBubble_c::endState_Bubble_Thrown() {

}