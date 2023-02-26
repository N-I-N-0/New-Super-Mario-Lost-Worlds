#include <common.h>
#include <game.h>
#include <g3dhax.h>
#include <sfx.h>

const char* MuncherArcNameList [] = {
	"pakkun_black",
	"ice",
	NULL
};

class dMuncher_c : public dEn_c {
	int onCreate();
	int onDelete();
	int onExecute();
	int onDraw();

	mHeapAllocator_c allocator;
	nw4r::g3d::ResFile resFile;
	m3d::mdl_c bodyModel;

	m3d::anmChr_c chrAnimation;

	mHeapAllocator_c iceAllocator;
	nw4r::g3d::ResFile iceResFile;
	m3d::mdl_c iceModel;
	Vec iceScale;
	float xIceScaleOffset;

	int timer;
	int maxTimer;
	float XSpeed;
	u32 cmgr_returnValue;
	bool isBouncing;

	int color, rotation, gravity;
	bool isFrozen, wideCollision, isFreezable, isBig;
	s16 colRot;

	Vec effectOffset;
	Vec effectScale;

	Physics::Info physicsInfo;
	Physics physics;

	static dMuncher_c *build();

	void bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate);
	void updateModelMatrices();
	bool calculateTileCollisions();

	USING_STATES(dMuncher_c);
	DECLARE_STATE(Wait);
	DECLARE_STATE(Frozen);
	DECLARE_STATE(Freeze);
	DECLARE_STATE(Unfreeze);

	public:
		bool normalActorCollision(dMuncher_c *saThis, dStageActor_c *saOther);
		bool isCurrentlyFrozen();
};

dMuncher_c *dMuncher_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dMuncher_c));
	return new(buffer) dMuncher_c;
}

///////////////////////
// Externs and States
///////////////////////
	// extern "C" void *EN_LandbarrelPlayerCollision(dEn_c* t, ActivePhysics *apThis, ActivePhysics *apOther);

	//FIXME make this dEn_c->used...
	extern "C" char usedForDeterminingStatePress_or_playerCollision(dEn_c* t, ActivePhysics *apThis, ActivePhysics *apOther, int unk1);
	// extern "C" int SmoothRotation(short* rot, u16 amt, int unk2);


	CREATE_STATE(dMuncher_c, Wait);
	CREATE_STATE(dMuncher_c, Frozen);
	CREATE_STATE(dMuncher_c, Freeze);
	CREATE_STATE(dMuncher_c, Unfreeze);


////////////////////////
// Collision Functions
////////////////////////


bool dMuncher_c::calculateTileCollisions() {
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

	if (collMgr.isOnTopOfTile()) {
		// Walking into a tile branch

		if (cmgr_returnValue == 0)
			isBouncing = true;

		if (speed.x != 0.0f) {
			//playWmEnIronEffect();
		}

		speed.y = 0.0f;
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


bool dMuncher_c::normalActorCollision(dMuncher_c *saThis, dStageActor_c *saOther) {
	if (saOther->stageActorType != 0) return false;

	switch (saOther->name) {
		case PL_FIREBALL:
		case BROS_FIREBALL:
		case PAKKUN_FIREBALL:
		case EN_FIREBAR:
		case EN_FIRESNAKE:
		case FIRE_BLITZ:
		case JR_FIRE:
		case KOOPA_FIRE:
			if (this->isFrozen) doStateChange(&StateID_Unfreeze);
			return true;

		case ICEBALL:
		case BROS_ICEBALL:
			if (this->isFreezable && !this->isFrozen) {
				doStateChange(&StateID_Freeze);
				saOther->Delete(1);
			}
			return true;
	}

	return false;
}

bool dMuncher_c::isCurrentlyFrozen() {
	return this->isFrozen;
}


/*****************************************************************************/
// Glue Code
extern "C" void HurtMarioBecauseOfBeingSquashed(void *mario, dStageActor_c *squasher, int type);

static void MuncherPhysCB1(dMuncher_c *one, dStageActor_c *two) { // Mario/Yoshi on top / Muncher on bottom
	if (one->normalActorCollision(one, two)) return;

	if (two->stageActorType != 1 && two->stageActorType != 2)
		return;

	if (one->pos_delta.y > 0.0f)
		HurtMarioBecauseOfBeingSquashed(two, one, 1);

	else if (one->pos_delta.y < 0.0f)
		HurtMarioBecauseOfBeingSquashed(two, one, 9);

	if (two->stageActorType == 1) { // Mario
		if (!one->isCurrentlyFrozen()) one->_vf220(two);
	}
}

static void MuncherPhysCB2(dMuncher_c *one, dStageActor_c *two) { // Mario/Yoshi on bottom / Muncher on top
	if (one->normalActorCollision(one, two)) return;

	if (two->stageActorType != 1 && two->stageActorType != 2)
		return;

	if (one->pos_delta.y < 0.0f)
		HurtMarioBecauseOfBeingSquashed(two, one, 2);

	else if (one->pos_delta.y > 0.0f)
		HurtMarioBecauseOfBeingSquashed(two, one, 10);

	if (!one->isCurrentlyFrozen()) one->_vf220(two);
}

static void MuncherPhysCB3(dMuncher_c *one, dStageActor_c *two, bool unkMaybeNotBool) { // Mario/Yoshi on side
	if (one->normalActorCollision(one, two)) return;

	if (two->stageActorType != 1 && two->stageActorType != 2)
		return;

	if (!one->isCurrentlyFrozen()) one->_vf220(two);
}

static bool MuncherPhysCB4(dMuncher_c *one, dStageActor_c *two) {
	return true;
}

static bool MuncherPhysCB5(dMuncher_c *one, dStageActor_c *two) {
	return true;
}

static bool MuncherPhysCB6(dMuncher_c *one, dStageActor_c *two, bool unkMaybeNotBool) {
	return true;
}


void dMuncher_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate) {
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr(name);
	this->chrAnimation.bind(&this->bodyModel, anmChr, unk);
	this->bodyModel.bindAnim(&this->chrAnimation, unk2);
	this->chrAnimation.setUpdateRate(rate);
}

int dMuncher_c::onCreate() {

	this->deleteForever = true;

	this->color = this->settings >> 28 & 0xF;

	int nybble5 = this->settings >> 24 & 0xF;
	this->isBig = nybble5 & 0b1000;
	this->wideCollision = nybble5 & 0b100;
	this->gravity = nybble5 & 0b11;

	int nybble8 = this->settings >> 16 & 0xF;
	bool moveSlightlyHorizontally = nybble8 & 0b1000;
	bool moveSlightlyUp = nybble8 & 0b100;
	this->isFreezable = nybble8 & 0b10;
	this->isFrozen = nybble8 & 0b1;

	this->rotation = this->settings & 0xFFFF;

	// Model creation
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	char resName[16];
	sprintf(resName, "g3d/t%02d.brres", this->color);
	resName[strlen(resName)] = 0;
	resFile.data = getResource("pakkun_black", resName);
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("pakkun_black");
	bodyModel.setup(mdl, &allocator, 0x23, 1, 0);
	SetupTextures_Enemy(&bodyModel, 0);


	// Animations start here
	static const nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr("attack");
	this->chrAnimation.setup(mdl, anmChr, &this->allocator, 0);

	allocator.unlink();


	// Ice Model creation
	iceAllocator.link(-1, GameHeaps[0], 0, 0x20);

	iceResFile.data = getResource("ice", "g3d/ice.brres");
	nw4r::g3d::ResMdl icemdl = this->iceResFile.GetResMdl("ice_A1");
	iceModel.setup(icemdl, &iceAllocator, 0x224, 1, 0);
	SetupTextures_MapObj(&iceModel, 0);

	iceAllocator.unlink();


	// Stuff I do understand
	this->scale = (Vec){1.0, 1.0, 1.0};
	if (this->isBig) this->scale = (Vec){2.0, 2.0, 2.0};
	this->iceScale = (Vec){.8 * this->scale.x, .8 * this->scale.y, .8 * this->scale.z};

	// rotation / 0xF * 0xFFFF == rotation * 0x1111
	// BUT 0x1111 doesn't work with some high values for some reason so I'm using 0xFFF
	this->rot.x = (float)(this->rotation); // X is vertical axis (0xFFF ~= 22.5 degrees)
	this->rot.y = 0x3FFF; // Y is horizontal axis
	this->rot.z = 0; // Z is ... an axis >.>
	this->direction = 1; // Heading left.
	
	this->speed.x = 0.0;
	this->speed.y = 0.0;
	this->max_speed.x = 0.8;
	this->x_speed_inc = 0.0;
	this->XSpeed = 0.8;

	Vec2 rotationBalance;
	float angle = ((this->rotation / (float)(0xFFFF)) * 360.0) - 90.0;
	if (angle < 0.0) angle += 360.0;
	rotationBalance.x = cos(angle * M_PI / 180.0);
	rotationBalance.y = sin(angle * M_PI / 180.0);

	float colBalance = 0.0;
	if (this->wideCollision) colBalance = (1.0 - (abs(rotationBalance.x) + abs(rotationBalance.y) - 1.0)) / 2.0; // Because yoshi falls between two munchers when there is an angle

	physicsInfo.x1 = (-8 * this->scale.x) - colBalance;
	physicsInfo.y1 = 16.0 * this->scale.y;
	physicsInfo.x2 = (8 * this->scale.x) + colBalance;
	physicsInfo.y2 = 0.0;

	this->xIceScaleOffset = colBalance / 2.0;
	this->iceScale.x += this->xIceScaleOffset;

	physicsInfo.otherCallback1 = &MuncherPhysCB1;
	physicsInfo.otherCallback2 = &MuncherPhysCB2;
	physicsInfo.otherCallback3 = &MuncherPhysCB3;

	physics.setup(this, &physicsInfo, 3, currentLayerID);
	physics.flagsMaybe = 0x260;
	physics.callback1 = &MuncherPhysCB4;
	physics.callback2 = &MuncherPhysCB5;
	physics.callback3 = &MuncherPhysCB6;
	this->colRot = -this->rot.x;
	physics.setPtrToRotation(&this->colRot);

	physics.addToList();

	if (moveSlightlyHorizontally) this->pos.x -= ((rotationBalance.x + 1.0) / 2.0) * 2.0;
	this->pos.y += 8.0;

	if (moveSlightlyUp) this->pos.y += 4.0;

	this->effectOffset.x = rotationBalance.x * 8.0;
	this->effectOffset.y = rotationBalance.y * -8.0;
	this->effectOffset.z = 0.0;

	this->effectScale = (Vec){.5 * this->scale.x, .5 * this->scale.x, .5 * this->scale.x};


	// Tile collider

	// These fucking rects do something for the tile rect
	spriteSomeRectX = 28.0f;
	spriteSomeRectY = 32.0f;
	_320 = 0.0f;
	_324 = 16.0f;

	static const pointSensor_s below(0<<12, 0<<12);
	static const pointSensor_s above(0<<12, 12<<12);
	static const lineSensor_s adjacent(6<<12, 9<<12, 8<<12);

	collMgr.init(this, &below, &above, &adjacent);
	collMgr.calculateBelowCollisionWithSmokeEffect();

	cmgr_returnValue = collMgr.isOnTopOfTile();

	if (collMgr.isOnTopOfTile())
		isBouncing = false;
	else
		isBouncing = true;


	// State Changers
	bindAnimChr_and_setUpdateRate("attack", 0, 0.0, 1.0);
	if (this->isFrozen) this->doStateChange(&StateID_Frozen);
	else this->doStateChange(&StateID_Wait);

	this->onExecute();
	return true;
}

int dMuncher_c::onDelete() {
	return true;
}

int dMuncher_c::onExecute() {
	acState.execute();
	physics.update();
	updateModelMatrices();
	
	// float rect[] = {0.0, 0.0, 38.0, 38.0};
	// int ret = this->outOfZone(this->pos, (float*)&rect, this->currentZoneID);
	// if(ret) {
	// 	this->Delete(1);
	// }
	return true;
}

int dMuncher_c::onDraw() {
	bodyModel.scheduleForDrawing();
	if (this->isFrozen) iceModel.scheduleForDrawing();

	return true;
}

void dMuncher_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);

	if (this->isFrozen) {
		iceModel.setDrawMatrix(matrix);
		iceModel.setScale(&iceScale);
		iceModel.calcWorld(false);
	}
}


///////////////
// Wait State
///////////////
void dMuncher_c::beginState_Wait() {
	this->max_speed.x = 0;
	this->speed.x = 0;

	switch (this->gravity) {
		case 1:
			this->max_speed.y = 0.0;
			this->speed.y = 0.0;
			this->y_speed_inc = 0.0;
			break;

		default:
			this->max_speed.y = -4.0;
			this->speed.y = -4.0;
			this->y_speed_inc = -0.1875;
			break;
	}

	bindAnimChr_and_setUpdateRate("attack", 1, 1.0, 1.0);
	float frame = dScStage_c::exeFrame % int(this->chrAnimation.numFrames);;
	this->chrAnimation.setCurrentFrame(frame);
}
void dMuncher_c::executeState_Wait() {
	if (this->gravity != 1) bool ret = calculateTileCollisions();
	// if (ret) {
	// 	doStateChange(&StateID_Turn);
	// }

	bodyModel._vf1C();

	if(this->chrAnimation.isAnimationDone()) {
		this->chrAnimation.setCurrentFrame(0.0);
	}
}
void dMuncher_c::endState_Wait() { }


///////////////
// Frozen State
///////////////
void dMuncher_c::beginState_Frozen() {
	this->max_speed.x = 0;
	this->speed.x = 0;

	this->max_speed.y = 0.0;
	this->speed.y = 0.0;
	this->y_speed_inc = 0.0;

	bindAnimChr_and_setUpdateRate("attack", 1, 1.0, 0.0);

	this->isFrozen = true;
	this->physics.flagsMaybe = 0x268;

	this->iceScale = (Vec){.8 * this->scale.x + this->xIceScaleOffset, .8 * this->scale.y, .8 * this->scale.z};
}
void dMuncher_c::executeState_Frozen() {
	bodyModel._vf1C();
}
void dMuncher_c::endState_Frozen() {
	this->isFrozen = false;
	this->physics.flagsMaybe = 0x260;
}


///////////////
// Freeze State
///////////////
void dMuncher_c::beginState_Freeze() {
	this->max_speed.x = 0;
	this->speed.x = 0;

	this->max_speed.y = 0.0;
	this->speed.y = 0.0;
	this->y_speed_inc = 0.0;

	bindAnimChr_and_setUpdateRate("attack", 1, 1.0, 0.0);

	this->isFrozen = true;
	this->physics.flagsMaybe = 0x268;

	this->iceScale = (Vec){0, 0, 0};
	this->timer = 0;
	this->maxTimer = 6;

	S16Vec nullRot = {0,0,0};
	Vec oneVec = {1.0f, 1.0f, 1.0f};
	Vec effPos = {this->pos.x + this->effectOffset.x, this->pos.y + this->effectOffset.y, this->pos.z + this->effectOffset.z};
	SpawnEffect("Wm_ob_iceattack", 0, &effPos, &nullRot, &oneVec);
	SpawnEffect("Wm_ob_iceattackkira", 0, &effPos, &nullRot, &oneVec);
	SpawnEffect("Wm_ob_iceattackline", 0, &effPos, &nullRot, &oneVec);
	SpawnEffect("Wm_ob_iceattacksmk", 0, &effPos, &nullRot, &oneVec);

	PlaySoundAsync(this, SE_OBJ_PNGN_ICE_FREEZE);
}
void dMuncher_c::executeState_Freeze() {
	this->timer++;

	float scale = .8 * ((float)this->timer / (float)this->maxTimer) * this->scale.x;
	this->iceScale = (Vec){scale + this->xIceScaleOffset, scale, scale};

	if (this->timer >= this->maxTimer) this->doStateChange(&StateID_Frozen);

	bodyModel._vf1C();
}
void dMuncher_c::endState_Freeze() {}


///////////////
// Unfreeze State
///////////////
void dMuncher_c::beginState_Unfreeze() {
	this->max_speed.x = 0;
	this->speed.x = 0;

	this->max_speed.y = 0.0;
	this->speed.y = 0.0;
	this->y_speed_inc = 0.0;

	//bindAnimChr_and_setUpdateRate("attack", 1, 1.0, 0.0);

	this->isFrozen = true;
	this->physics.flagsMaybe = 0x268;

	this->iceScale = (Vec){.8 * this->scale.x + this->xIceScaleOffset, .8 * this->scale.y, .8 * this->scale.z};
	this->timer = 30;
	this->maxTimer = this->timer;

	S16Vec nullRot = {0,0,0};
	Vec oneVec = {1.0f, 1.0f, 1.0f};
	Vec effPos = {this->pos.x + this->effectOffset.x, this->pos.y + this->effectOffset.y, this->pos.z + this->effectOffset.z};
	SpawnEffect("Wm_ob_iceevaporate", 0, &effPos, &nullRot, &oneVec);
	PlaySoundAsync(this, SE_EMY_FIRE_SNAKE_EXTINCT);
}
void dMuncher_c::executeState_Unfreeze() {
	this->timer--;

	float scale = .8 * ((float)this->timer / (float)this->maxTimer) * this->scale.x;
	this->iceScale = (Vec){scale + this->xIceScaleOffset, scale, scale};

	if (this->timer <= 0) this->doStateChange(&StateID_Wait);

	bodyModel._vf1C();
}
void dMuncher_c::endState_Unfreeze() {
	this->isFrozen = false;
	this->physics.flagsMaybe = 0x260;
}

