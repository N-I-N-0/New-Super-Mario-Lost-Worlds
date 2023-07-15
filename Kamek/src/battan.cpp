#include <common.h>
#include <game.h>
#include "profile.h"
#include <g3dhax.h>
#include <sfx.h>

const char* BattanArcNameList [] = {
	"battan",
	NULL	
};

class daBattan : public daEnBlockMain_c {
    ActivePhysics cPhysics;
	bool disableCPhysics;
	Physics::Info blockPhysicsInfo;

    static void *PhysicsCallback1;
	static void *PhysicsCallback2;
	static void *PhysicsCallback3;
	static void *OPhysicsCallback1;
	static void *OPhysicsCallback2;
	static void *OPhysicsCallback3;

    Physics bPhysics;

	int onCreate();
	int onDelete();
	int onExecute();
	int onDraw();

	mHeapAllocator_c allocator;
	nw4r::g3d::ResFile resFile;

	m3d::mdl_c bodyModel;

	m3d::anmChr_c chrAnimation;

	int timer;
	char damage;
	char isDown;
	float XSpeed;
	u32 cmgr_returnValue;
	bool isBouncing;

    int color;
    int distance;
    float currentDistance;
	bool startGoingRight;
    int objectBehind;
	bool bigBoi;

	bool isPlayerInZone(Vec2 pos, Vec2 size);

	void bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate);
	void updateModelMatrices();
	bool calculateTileCollisions();

	void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther);
	void yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther);

	bool collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat11_PipeCannon(ActivePhysics *apThis, ActivePhysics *apOther);

	void addScoreWhenHit(void *other);

	static void doDamage(ActivePhysics *apThis, ActivePhysics *apOther);

public:
	static dActor_c *build();

	USING_STATES(daBattan);
    DECLARE_STATE(Idle);
	DECLARE_STATE(Walk);
	DECLARE_STATE(Turn);
    DECLARE_STATE(FallWait);
    DECLARE_STATE(Fall);
    DECLARE_STATE(GroundWait);
    DECLARE_STATE(StandUp);
	DECLARE_STATE(Die);
};

dActor_c *daBattan::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daBattan));
	return new(buffer) daBattan;
}


const SpriteData BattanSpriteData = { ProfileId::Battan, 8, -8, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile BattanProfile(&daBattan::build, SpriteId::Battan, &BattanSpriteData, ProfileId::Battan, ProfileId::Battan, "Battan", BattanArcNameList, 0);


///////////////////////
// Externs and States
///////////////////////
	// extern "C" void *EN_LandbarrelPlayerCollision(dEn_c* t, ActivePhysics *apThis, ActivePhysics *apOther);

	//FIXME make this dEn_c->used...
	extern "C" char usedForDeterminingStatePress_or_playerCollision(dEn_c* t, ActivePhysics *apThis, ActivePhysics *apOther, int unk1);
	extern "C" int SmoothRotation(short* rot, u16 amt, int unk2);


    CREATE_STATE(daBattan, Idle);
	CREATE_STATE(daBattan, Walk);
	CREATE_STATE(daBattan, Turn);
    CREATE_STATE(daBattan, FallWait);
    CREATE_STATE(daBattan, Fall);
    CREATE_STATE(daBattan, GroundWait);
    CREATE_STATE(daBattan, StandUp);
	CREATE_STATE(daBattan, Die);


////////////////////////
// Collision Functions
////////////////////////

	// Collision callback to help shy guy not die at inappropriate times and ruin the dinner

	void daBattan::playerCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
		char hitType = 0;
		hitType = usedForDeterminingStatePress_or_playerCollision(this, apThis, apOther, 2);
	}

	void daBattan::yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
		this->playerCollision(apThis, apOther);
	}

	bool daBattan::collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther) {
		doStateChange(&StateID_Die);
		return true;
	}

	bool daBattan::collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther) {
		doStateChange(&StateID_Die);
		return true;
	}

	bool daBattan::collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther) {
		this->collisionCat7_GroundPound(apThis, apOther);
		return true;
	}

	bool daBattan::collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther) {
		return true;
	}

	bool daBattan::collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther){
		doStateChange(&StateID_Die);
		return true;
	}

	bool daBattan::collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther) {
		doStateChange(&StateID_Die);
		return true;
	}

	bool daBattan::collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther) {
		return true;
	}

	bool daBattan::collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther) {
		return true;
	}

	bool daBattan::collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther) {
		return true;
	}

	bool daBattan::collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther) {
		return false;
	}

	bool daBattan::collisionCat11_PipeCannon(ActivePhysics *apThis, ActivePhysics *apOther) {
		doStateChange(&StateID_Die);
		return true;
	}

	void daBattan::addScoreWhenHit(void *other) {}

	void daBattan::doDamage(ActivePhysics *apThis, ActivePhysics *apOther) {
		if (((daBattan*)apThis->owner)->disableCPhysics) return;

		for (int i = 0; i < 4; i++) {
			if (Player_Active[i] != 0 && Player_Lives[Player_ID[i]] > 0) {
				dStageActor_c *player = GetSpecificPlayerActor(i);
				if (((dStageActor_c*)apOther->owner) == player) {
					((daBattan*)apThis->owner)->_vf220(apOther->owner);
				}
			}
		}
	}


bool daBattan::calculateTileCollisions() {
	// Returns true if sprite should turn, false if not.

	HandleXSpeed();
	HandleYSpeed();
	doSpriteMovement();

	cmgr_returnValue = collMgr.isOnTopOfTile();
	collMgr.calculateBelowCollisionWithSmokeEffect();

	if (isBouncing) {
		stuffRelatingToCollisions(0.1875f, 1.0f, 0.5f);
		if (speed.y != 0.0f)
			isBouncing = false;
	}

	float xDelta = pos.x - last_pos.x;
	if (xDelta >= 0.0f)
		direction = 0;
	else
		direction = 1;

	if (collMgr.isOnTopOfTile()) {
		// Walking into a tile branch

		if (cmgr_returnValue == 0) {
			isBouncing = true;
		}

		speed.y = 0.0f;

		max_speed.x = (direction == 1) ? -0.4f : 0.4f;
	} else {
		x_speed_inc = 0.0f;
	}

	// Bouncing checks
	if (_34A & 4) {
		Vec v = (Vec){0.0f, 1.0f, 0.0f};
		collMgr.pSpeed = &v;

		if (collMgr.calculateAboveCollision(collMgr.outputMaybe))
			speed.y = 0.0f;

		collMgr.pSpeed = &speed;

	} else {
		if (collMgr.calculateAboveCollision(collMgr.outputMaybe))
			speed.y = 0.0f;
	}

	collMgr.calculateAdjacentCollision(0);

	// Switch Direction
	if (collMgr.outputMaybe & (0x15 << direction)) {
		if (collMgr.isOnTopOfTile()) {
			isBouncing = true;
		}
		return true;
	}
	return false;
}

void daBattan::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate) {
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr(name);
	this->chrAnimation.bind(&this->bodyModel, anmChr, unk);
	this->bodyModel.bindAnim(&this->chrAnimation, unk2);
	this->chrAnimation.setUpdateRate(rate);
}

int daBattan::onCreate() {
    this->color = (this->settings >> 28) & 0xF; // Unused
    this->distance = ((this->settings >> 24) & 0xF) * 8;
    this->currentDistance = 0;
	this->startGoingRight = (this->settings >> 20) & 0xF;
    this->objectBehind = (this->settings >> 16) & 0xF; // Unused
	this->bigBoi = (this->settings >> 12) & 0xF;
	
	// Model creation	
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	this->resFile.data = getResource("battan", "g3d/t00.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("battan");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Enemy(&bodyModel, 0);


	// Animations start here
	bool ret;
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr("idle");
	ret = this->chrAnimation.setup(mdl, anmChr, &this->allocator, 0);

	allocator.unlink();

	// Stuff I do understand
	this->scale = (Vec){1.0, 1.0, 1.0};
	if (this->bigBoi) {
		this->scale.x = this->scale.y = this->scale.z = this->scale.x * 2.0;
	}

	this->rot.x = 0; // X is vertical axis
	if (this->distance == 0) this->rot.y = 0; // Y is horizontal axis
    else {
		if (this->startGoingRight) this->rot.y = 0x2800; // Y is horizontal axis
		else this->rot.y = 0xD800; // Y is horizontal axis
	}
	this->rot.z = 0; // Z is ... an axis >.>
	if (this->startGoingRight) this->direction = 0; // Heading right.
	else this->direction = 1; // Heading left.
	
	this->speed.x = 0.0;
	this->speed.y = 0.0;
	this->max_speed.x = 0.4;
	this->x_speed_inc = 0.0;
	this->XSpeed = 0.4;

	ActivePhysics::Info aPhysicsInfo;

	aPhysicsInfo.xDistToCenter = 0.0;
	aPhysicsInfo.yDistToCenter = 16.0 * this->scale.x;

	if (this->distance == 0) aPhysicsInfo.xDistToEdge = 12.0  * this->scale.x;
	else aPhysicsInfo.xDistToEdge = 8.0 * this->scale.x;
	aPhysicsInfo.yDistToEdge = 16.0 * this->scale.y;

	aPhysicsInfo.category1 = 0x3;
	aPhysicsInfo.category2 = 0x0;
	aPhysicsInfo.bitfield1 = 0x4F;
	aPhysicsInfo.bitfield2 = 0xffbafffe;
	aPhysicsInfo.unkShort1C = 0;
	aPhysicsInfo.callback = &dEn_c::collisionCallback;

	this->aPhysics.initWithStruct(this, &aPhysicsInfo);
	this->aPhysics.addToList();

	this->edible = 0;


    blockInit(pos.y);

	this->blockPhysicsInfo.x1 = -12 * this->scale.x;
	this->blockPhysicsInfo.y1 = 8 * this->scale.y;
	this->blockPhysicsInfo.x2 = 12 * this->scale.x;
	this->blockPhysicsInfo.y2 = 0 * this->scale.y;


	this->blockPhysicsInfo.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
	this->blockPhysicsInfo.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
	this->blockPhysicsInfo.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

	this->bPhysics.setup(this, &this->blockPhysicsInfo, 3, currentLayerID);
	this->bPhysics.flagsMaybe = 0x260;
	this->bPhysics.callback1 = &daEnBlockMain_c::PhysicsCallback1;
	this->bPhysics.callback2 = &daEnBlockMain_c::PhysicsCallback2;
	this->bPhysics.callback3 = &daEnBlockMain_c::PhysicsCallback3;


	ActivePhysics::Info cPhysicsInfo;

	cPhysicsInfo.xDistToCenter = 0.0;
	cPhysicsInfo.yDistToCenter = 3.875 * this->scale.y;

	if (this->distance == 0) cPhysicsInfo.xDistToEdge = 12.0 * this->scale.x;
	else cPhysicsInfo.xDistToEdge = 18.0 * this->scale.x;
	cPhysicsInfo.yDistToEdge = 3.875 * this->scale.y;

	cPhysicsInfo.category1 = 0x3;
	cPhysicsInfo.category2 = 0x0;
	cPhysicsInfo.bitfield1 = 0x4F;
	cPhysicsInfo.bitfield2 = 0x0;
	cPhysicsInfo.unkShort1C = 0;
	cPhysicsInfo.callback = daBattan::doDamage;

	this->cPhysics.initWithStruct(this, &cPhysicsInfo);
	this->disableCPhysics = true;
	this->cPhysics.addToList();


	// Tile collider

	// These fucking rects do something for the tile rect
	spriteSomeRectX = 28.0f;
	spriteSomeRectY = 32.0f;
	_320 = 0.0f;
	_324 = 16.0f;

	static const lineSensor_s below(12<<12, 4<<12, 0<<12);
	static const pointSensor_s above(0<<12, 12<<12);
	static const lineSensor_s adjacent(6<<12, 9<<12, 14<<12);

	collMgr.init(this, &below, &above, &adjacent);
	collMgr.calculateBelowCollisionWithSmokeEffect();

	cmgr_returnValue = collMgr.isOnTopOfTile();

	if (collMgr.isOnTopOfTile())
		isBouncing = false;
	else
		isBouncing = true;


	// State Changers
    if (this->distance == 0) doStateChange(&StateID_Idle);
    else doStateChange(&StateID_Walk);

	this->onExecute();
	return true;
}

int daBattan::onDelete() {
	return true;
}

int daBattan::onExecute() {
	acState.execute();
	updateModelMatrices();
	
	OSReport("current state: %s\n", acState.getCurrentState()->getName());
	
	float rect[] = {0.0, 0.0, 38.0, 38.0};
	int ret = this->outOfZone(this->pos, (float*)&rect, this->currentZoneID);
	if(ret) {
		this->Delete(1);
	}
	return true;
}

int daBattan::onDraw() {
	bodyModel.scheduleForDrawing();

	return true;
}

void daBattan::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}



bool daBattan::isPlayerInZone(Vec2 pos, Vec2 size) {
	for (int i = 0; i < 4; i++) {
		if (Player_Active[i] != 0 && Player_Lives[Player_ID[i]] > 0) {
			dStageActor_c *player = GetSpecificPlayerActor(i);
			Vec2 playerPos = {player->pos.x + player->aPhysics.info.xDistToCenter, player->pos.y + player->aPhysics.info.yDistToCenter};
			Vec2 playerSize = {player->aPhysics.info.xDistToEdge, player->aPhysics.info.yDistToEdge};
			if (playerPos.x + playerSize.x >= pos.x && playerPos.x <= pos.x + size.x && playerPos.y + playerSize.y >= pos.y && playerPos.y <= pos.y + size.y) {
				return true;
			}
		}
	}
	return false;
}



///////////////
// Idle State
///////////////
void daBattan::beginState_Idle() {
    bindAnimChr_and_setUpdateRate("idle", 1, 0.0, 1.0);
    this->speed.x = 0.0;
    this->max_speed.x = 0.0;

    this->max_speed.y = -4.0;
    this->speed.y = -4.0;
    this->y_speed_inc = -0.1875;
}
void daBattan::executeState_Idle() {
    calculateTileCollisions();
    bodyModel._vf1C();

    if(this->chrAnimation.isAnimationDone()) {
        this->chrAnimation.setCurrentFrame(0.0);
    }

	Vec2 triggerSize = {this->aPhysics.info.xDistToEdge * 2.4, this->aPhysics.info.yDistToEdge * 2.0};
	Vec2 triggerPos = {
		this->pos.x + this->aPhysics.info.xDistToCenter - (triggerSize.x / 2.0),
		this->pos.y + this->aPhysics.info.yDistToCenter - (triggerSize.y / 2.0)
	};

	if (this->isPlayerInZone(triggerPos, triggerSize)) {
		doStateChange(&StateID_FallWait);
	}
}
void daBattan::endState_Idle() { }


///////////////
// Walk State
///////////////
void daBattan::beginState_Walk() {
    this->max_speed.x = (this->direction) ? -this->XSpeed : this->XSpeed;
    this->speed.x = (direction) ? -0.4f : 0.4f;

    this->max_speed.y = -4.0;
    this->speed.y = -4.0;
    this->y_speed_inc = -0.1875;

    bindAnimChr_and_setUpdateRate("walk", 1, 0.0, 1.0);
}
void daBattan::executeState_Walk() {
    bool ret = calculateTileCollisions();
    bodyModel._vf1C();
    this->currentDistance += this->speed.x;

    if (ret || (this->currentDistance >= this->distance) || (this->currentDistance <= -this->distance)) {
        doStateChange(&StateID_Turn);
    }

    if(this->chrAnimation.isAnimationDone()) {
        this->chrAnimation.setCurrentFrame(0.0);
    }

	Vec2 triggerSize = {this->aPhysics.info.xDistToEdge * 3.6, this->aPhysics.info.yDistToEdge * 2.0};
	Vec2 triggerPos = {
		this->pos.x + this->aPhysics.info.xDistToCenter + (this->aPhysics.info.xDistToEdge / 2.0) + ((this->direction) ? -triggerSize.x : 0.0),
		this->pos.y + this->aPhysics.info.yDistToCenter - (triggerSize.y / 2.0)
	};

	if (this->isPlayerInZone(triggerPos, triggerSize) && !ret) {
		doStateChange(&StateID_FallWait);
	}
}
void daBattan::endState_Walk() { }


///////////////
// Turn State
///////////////
void daBattan::beginState_Turn() {
    this->direction ^= 1;
    this->speed.x = 0.0;
}
void daBattan::executeState_Turn() { 
    if(this->chrAnimation.isAnimationDone()) {
        this->chrAnimation.setCurrentFrame(0.0);
    }

    u16 amt = (this->direction == 0) ? 0x2800 : 0xD800;
    int done = SmoothRotation(&this->rot.y, amt, 0x800);

    if(done) {
        doStateChange(&StateID_Walk);
    }
}
void daBattan::endState_Turn() {
	this->rot.y = (this->direction == 0) ? 0x2800 : 0xD800;
}


///////////////
// FallWait State
///////////////
void daBattan::beginState_FallWait() {
	this->max_speed.x = 0.0;
    this->speed.x = 0.0;
    bindAnimChr_and_setUpdateRate("press_sign", 1, 0.0, 1.0);
}
void daBattan::executeState_FallWait() {
	this->chrAnimation.process();
    if(this->chrAnimation.isAnimationDone()) doStateChange(&StateID_Fall);
}
void daBattan::endState_FallWait() { }


///////////////
// Fall State
///////////////
void daBattan::beginState_Fall() {
    bindAnimChr_and_setUpdateRate("press", 1, 0.0, 1.0);

	// if (this->distance != 0) this->cPhysics.info.xDistToCenter = this->cPhysics.info.xDistToEdge * (this->direction == 1 ? -1.0 : 1.0);
	// this->cPhysics.addToList();
	// this->aPhysics.removeFromList();
}
void daBattan::executeState_Fall() {
	this->chrAnimation.process();
	int frame = this->chrAnimation.currentFrame;
	int maxFrame = this->chrAnimation.numFrames;
	int capFrame = maxFrame / 1.55; // 1.75 before
    if(this->chrAnimation.isAnimationDone()) doStateChange(&StateID_GroundWait);
	else if (frame == capFrame) {
		if (this->distance != 0) this->cPhysics.info.xDistToCenter = this->cPhysics.info.xDistToEdge * (this->direction == 1 ? -1.0 : 1.0);
		this->disableCPhysics = false;
		this->aPhysics.removeFromList();
	}
}
void daBattan::endState_Fall() {
	this->disableCPhysics = true;
}


///////////////
// GroundWait State
///////////////
void daBattan::beginState_GroundWait() {
	if (this->distance != 0) {
		this->blockPhysicsInfo.x1 = (-19 * this->scale.x) + this->cPhysics.info.xDistToCenter;
		this->blockPhysicsInfo.x2 = (19 * this->scale.x) + this->cPhysics.info.xDistToCenter;
	}
	this->blockPhysicsInfo.y1 = 8 * this->scale.y;
	this->blockPhysicsInfo.y2 = 0;
	this->bPhysics.setup(this, &this->blockPhysicsInfo, 3, currentLayerID);

    this->bPhysics.addToList();
    bindAnimChr_and_setUpdateRate("ground_wait", 1, 0.0, 1.0);
}
void daBattan::executeState_GroundWait() {
	this->chrAnimation.process();
    if(this->chrAnimation.isAnimationDone()) doStateChange(&StateID_StandUp);

	int result = blockResult();
	if (result > 1) {
		doStateChange(&StateID_Die);
	}
}
void daBattan::endState_GroundWait() { }


///////////////
// StandUp State
///////////////
void daBattan::beginState_StandUp() {
    this->bPhysics.removeFromList();
    bindAnimChr_and_setUpdateRate("stand_up", 1, 0.0, 1.0);
}
void daBattan::executeState_StandUp() {
	this->chrAnimation.process();
    if(this->chrAnimation.isAnimationDone()) {
        if (this->distance == 0) doStateChange(&StateID_Idle);
        else doStateChange(&StateID_Walk);
    }
}
void daBattan::endState_StandUp() {
    this->aPhysics.addToList();
}


///////////////
// Die State
///////////////
void daBattan::beginState_Die() {
    dEn_c::dieFall_Begin();

    PlaySound(this, SE_EMY_DOWN);
    S16Vec nullRot = {0,0,0};
    Vec oneVec = {1.0f * this->scale.x, 1.0f * this->scale.y, 1.0f * this->scale.z};
    SpawnEffect("Wm_mr_hardhit", 0, &pos, &nullRot, &oneVec);
}
void daBattan::executeState_Die() { 

    bodyModel._vf1C();

    this->kill();
    this->Delete(1);
}
void daBattan::endState_Die() { }

