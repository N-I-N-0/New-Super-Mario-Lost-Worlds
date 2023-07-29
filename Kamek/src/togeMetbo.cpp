#include <common.h>
#include <game.h>
#include <g3dhax.h>
#include <sfx.h>
#include <profile.h>

#define SFX_TOGE_METBO_JUMP 2015
#define SFX_TOGE_METBO_BLOCK 2016

const char* TogeMetboArcNameList [] = {
	"toge_metbo",
	NULL
};

class daTogeMetbo_c : public dEn_c {
	int onCreate();
	int onDelete();
	int onExecute();
	int onDraw();

	mHeapAllocator_c allocator;
	nw4r::g3d::ResFile resFile;

	m3d::mdl_c model;

	m3d::anmChr_c animationChr;

	mEf::es2 effect;

	float XSpeed;
	u32 cmgr_returnValue;
	bool isBouncing;
	bool stillFalling;

	int timer, maxTimer, sfxTimer;
	int xCenter, distance;
	int minX, maxX;
	u16 xSeeDistance, ySeeDistance;

	nw4r::snd::SoundHandle handle; // Sound Handle

	dStateBase_c *previousState;

	dAcPy_c *targetPlayer;

	// StandOnTopCollider giantRider;

	void bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate);
	void updateModelMatrices();
	bool calculateTileCollisions();

	void spriteCollision(ActivePhysics *apThis, ActivePhysics *apOther);
	void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther);
	void yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther);

	bool collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther);

	bool collisionCat5_Mario(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat11_PipeCannon(ActivePhysics *apThis, ActivePhysics *apOther);

	bool willWalkOntoSuitableGround();
	dAcPy_c *detectPlayer();
	float distFromTargetPlayer();

	USING_STATES(daTogeMetbo_c);
	DECLARE_STATE(Walk);
	DECLARE_STATE(Wait);
	DECLARE_STATE(Turn);
	DECLARE_STATE(FoundPlayer);
	DECLARE_STATE(Follow);
	DECLARE_STATE(JumpSign);
	DECLARE_STATE(Jump);
	DECLARE_STATE(Down);
	DECLARE_STATE(RecoverSign);
	DECLARE_STATE(Recover);
	DECLARE_STATE(BlockProjectile);
	DECLARE_STATE(DieStomp);

	public: static dActor_c *build();
};

dActor_c *daTogeMetbo_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daTogeMetbo_c));
	return new(buffer) daTogeMetbo_c;
}

const SpriteData TogeMetboSpriteData = { ProfileId::TogeMetbo, 8, -12, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile TogeMetboProfile(&daTogeMetbo_c::build, SpriteId::TogeMetbo, &TogeMetboSpriteData, ProfileId::TogeMetbo, ProfileId::TogeMetbo, "TogeMetbo", TogeMetboArcNameList, 0);

///////////////////////
// Externs and States
///////////////////////
extern "C" bool SpawnEffect(const char*, int, Vec*, S16Vec*, Vec*);

//FIXME make this dEn_c->used...
extern "C" char usedForDeterminingStatePress_or_playerCollision(dEn_c* t, ActivePhysics *apThis, ActivePhysics *apOther, int unk1);
// extern "C" int SomeStrangeModification(dStageActor_c* actor);
// extern "C" void DoStuffAndMarkDead(dStageActor_c *actor, Vec vector, float unk);
extern "C" int SmoothRotation(short* rot, u16 amt, int unk2);
// extern "C" void addToList(StandOnTopCollider *self);

// extern "C" bool HandlesEdgeTurns(dEn_c* actor);


CREATE_STATE(daTogeMetbo_c, Walk);
CREATE_STATE(daTogeMetbo_c, Wait);
CREATE_STATE(daTogeMetbo_c, Turn);
CREATE_STATE(daTogeMetbo_c, FoundPlayer);
CREATE_STATE(daTogeMetbo_c, Follow);
CREATE_STATE(daTogeMetbo_c, JumpSign);
CREATE_STATE(daTogeMetbo_c, Jump);
CREATE_STATE(daTogeMetbo_c, Down);
CREATE_STATE(daTogeMetbo_c, RecoverSign);
CREATE_STATE(daTogeMetbo_c, Recover);
CREATE_STATE(daTogeMetbo_c, BlockProjectile);
CREATE_STATE(daTogeMetbo_c, DieStomp);

////////////////////////
// Collision Functions
////////////////////////
void daTogeMetbo_c::spriteCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	u16 name = ((dEn_c*)apOther->owner)->name;

	if (name == EN_COIN || name == EN_EATCOIN || name == AC_BLOCK_COIN || name == EN_COIN_JUGEM || name == EN_COIN_ANGLE
		|| name == EN_COIN_JUMP || name == EN_COIN_FLOOR || name == EN_COIN_VOLT || name == EN_COIN_WIND
		|| name == EN_BLUE_COIN || name == EN_COIN_WATER || name == EN_REDCOIN || name == EN_GREENCOIN
		|| name == EN_JUMPDAI || name == EN_ITEM)
		{ return; }

	if (acState.getCurrentState() == &StateID_Walk) {

		pos.x = ((pos.x - ((dEn_c*)apOther->owner)->pos.x) > 0) ? pos.x + 1.5 : pos.x - 1.5;
		// pos.x = direction ? pos.x + 1.5 : pos.x - 1.5;
		doStateChange(&StateID_Turn);
	}

	dEn_c::spriteCollision(apThis, apOther);
}

void daTogeMetbo_c::playerCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	if (acState.getCurrentState() != &StateID_Down && acState.getCurrentState() != &StateID_RecoverSign) return this->_vf220(apOther->owner);

	char hitType = usedForDeterminingStatePress_or_playerCollision(this, apThis, apOther, 2);

	if (hitType == 1) {	// regular jump
		this->_vf260(apOther->owner);
		doStateChange(&StateID_DieStomp);
	}
	else if (hitType == 3) { // spin jump
		this->_vf268(apOther->owner);
		doStateChange(&StateID_DieSmoke);
	}
	else if (hitType == 0) { // sides
		this->dEn_c::playerCollision(apThis, apOther);
		this->_vf220(apOther->owner);
	}
	else if (hitType == 2) { // mini mario
		bouncePlayer(this, 5.0);
	}
}

void daTogeMetbo_c::yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	if (acState.getCurrentState() != &StateID_Down && acState.getCurrentState() != &StateID_RecoverSign) return this->_vf220(apOther->owner);

	char hitType = usedForDeterminingStatePress_or_playerCollision(this, apThis, apOther, 2);

	if (hitType == 1 || hitType == 2) {	// regular jump or mini mario
		this->_vf278(apOther->owner);
		doStateChange(&StateID_DieSmoke);
	}
	else if (hitType == 3) { // spin jump
		this->_vf268(apOther->owner);
		doStateChange(&StateID_DieSmoke);
	}
	else if (hitType == 0) { // sides
		this->dEn_c::playerCollision(apThis, apOther);
		this->_vf220(apOther->owner);
	}
}
bool daTogeMetbo_c::collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther) {
	if (acState.getCurrentState() != &StateID_Down && acState.getCurrentState() != &StateID_RecoverSign) {
		this->_vf220(apOther->owner);
		return false;
	}

	PlaySound(this, SE_EMY_DOWN);
	SpawnEffect("Wm_mr_hardhit", 0, &pos, &(S16Vec){0, 0, 0}, &(Vec){1.0, 1.0, 1.0});
	dEn_c::_vf148();
	return true;
}
bool daTogeMetbo_c::collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->collisionCatD_Drill(apThis, apOther);
}
bool daTogeMetbo_c::collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->collisionCatD_Drill(apThis, apOther);
}
bool daTogeMetbo_c::collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->collisionCatD_Drill(apThis, apOther);
}
bool daTogeMetbo_c::collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->collisionCatD_Drill(apThis, apOther);
}
bool daTogeMetbo_c::collisionCat5_Mario(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->collisionCatD_Drill(apThis, apOther);
}
bool daTogeMetbo_c::collisionCat11_PipeCannon(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->collisionCatD_Drill(apThis, apOther);
}
bool daTogeMetbo_c::collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther) {
	StageE4::instance->spawnCoinJump(pos, 0, 2, 0);
	PlaySound(this, SE_EMY_DOWN);
	SpawnEffect("Wm_mr_hardhit", 0, &pos, &(S16Vec){0, 0, 0}, &(Vec){1.0, 1.0, 1.0});
	dEn_c::_vf148();
	return true;
}

bool daTogeMetbo_c::collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther) {
	bool wut = dEn_c::collisionCat3_StarPower(apThis, apOther);
	return wut;
}

bool daTogeMetbo_c::collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->collisionCat1_Fireball_E_Explosion(apThis, apOther);
}

bool daTogeMetbo_c::collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther) {
	if (acState.getCurrentState() == &StateID_Wait || acState.getCurrentState() == &StateID_Walk || acState.getCurrentState() == &StateID_Follow)
		doStateChange(&StateID_BlockProjectile);
	playSoundDistance(this->handle, this->pos, SFX_TOGE_METBO_BLOCK, 0.75);
	return false;
}

bool daTogeMetbo_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther) {
	if (acState.getCurrentState() == &StateID_Wait || acState.getCurrentState() == &StateID_Walk || acState.getCurrentState() == &StateID_Follow)
		doStateChange(&StateID_BlockProjectile);
	playSoundDistance(this->handle, this->pos, SFX_TOGE_METBO_BLOCK, 0.75);
	return false;
}

bool daTogeMetbo_c::calculateTileCollisions() {
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
	if (xDelta > 0.0f)
		direction = 0;
	else if (xDelta < 0.0f)
		direction = 1;

	if (collMgr.isOnTopOfTile()) {
		// Walking into a tile branch

		if (cmgr_returnValue == 0)
			isBouncing = true;

		if (speed.x != 0.0f) {
			//playWmEnIronEffect();
		}

		speed.y = 0.0f;

		max_speed.x = (direction == 1) ? -XSpeed : XSpeed;
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

void daTogeMetbo_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate) {
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr(name);
	this->animationChr.bind(&this->model, anmChr, unk);
	this->model.bindAnim(&this->animationChr, unk2);
	this->animationChr.setUpdateRate(rate);
}

int daTogeMetbo_c::onCreate() {
	this->stillFalling = 0;

	this->xCenter = this->pos.x;
	this->distance = 16 * 4;

	this->minX = this->xCenter - this->distance;
	this->maxX = this->xCenter + this->distance;

	this->xSeeDistance = 16 * 6;
	this->ySeeDistance = 16 * 3;

	int color = this->settings >> 24 & 0xF;

	allocator.link(-1, GameHeaps[0], 0, 0x20);

	char resName[16];
	getSpriteTexResName(resName, color);
	this->resFile.data = getResource("toge_metbo", resName);
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("toge_metbo");
	model.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Enemy(&model, 0);


	// Animations start here
	bool ret;
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr("wait");
	ret = this->animationChr.setup(mdl, anmChr, &this->allocator, 0);

	allocator.unlink();

	// Stuff I do understand

	this->scale = (Vec){0.2, 0.2, 0.2};

	this->rot.x = 0; // X is vertical axis
	this->rot.y = 0xD800; // Y is horizontal axis
	this->rot.z = 0; // Z is ... an axis >.>
	this->direction = 1; // Heading left.
	
	this->speed.x = 0.0;
	this->speed.y = 0.0;
	this->max_speed.x = 0.5;
	this->x_speed_inc = 0.15;
	this->XSpeed = 0.5;

	this->pos.z = -100.0;


	ActivePhysics::Info HitMeBaby;

	// Note: if this gets changed, also change the point where the default
	// values are assigned after de-ballooning
	HitMeBaby.xDistToCenter = 0.0;
	HitMeBaby.yDistToCenter = 18.0;
	HitMeBaby.xDistToEdge = 15.0;
	HitMeBaby.yDistToEdge = 18.0;

	HitMeBaby.category1 = 0x3;
	HitMeBaby.category2 = 0x0;
	HitMeBaby.bitfield1 = 0x6F;
	HitMeBaby.bitfield2 = 0xffbafffe;
	HitMeBaby.unkShort1C = 0;
	HitMeBaby.callback = &dEn_c::collisionCallback;

	this->aPhysics.initWithStruct(this, &HitMeBaby);
	this->aPhysics.addToList();


	// Tile collider

	// These fucking rects do something for the tile rect
	spriteSomeRectX = 28.0f;
	spriteSomeRectY = 32.0f;
	_320 = 0.0f;
	_324 = 16.0f;

	// These structs tell stupid collider what to collide with - these are from koopa troopa
	static const lineSensor_s below(-14<<12, 14<<12, 0<<12);
	static const pointSensor_s above(0<<12, 36<<12);
	static const lineSensor_s adjacent(1<<12, 36<<12, 15<<12);

	collMgr.init(this, &below, &above, &adjacent);
	collMgr.calculateBelowCollisionWithSmokeEffect();

	cmgr_returnValue = collMgr.isOnTopOfTile();

	if (collMgr.isOnTopOfTile())
		isBouncing = false;
	else
		isBouncing = true;

	bindAnimChr_and_setUpdateRate("wait", 1, 0.0, 1.0);
	doStateChange(&StateID_Wait);

	this->onExecute();
	return true;
}

int daTogeMetbo_c::onDelete() {
	return true;
}

int daTogeMetbo_c::onExecute() {
	// OSReport("%s\n", this->acState.getCurrentState()->getName());
	acState.execute();
	updateModelMatrices();
	model._vf1C();

	return true;
}

int daTogeMetbo_c::onDraw() {
	model.scheduleForDrawing();
	return true;
}

void daTogeMetbo_c::updateModelMatrices() {
	// This won't work with wrap because I'm lazy.

	if (this->frzMgr._mstate == 1)
		matrix.translation(pos.x, pos.y + 2.0, pos.z);
	else
		matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	model.setDrawMatrix(matrix);
	model.setScale(&scale);
	model.calcWorld(false);
}


bool daTogeMetbo_c::willWalkOntoSuitableGround() {
	static const float deltas[] = {1.5f, -1.5f};
	VEC3 checkWhere = {
			pos.x + deltas[direction],
			4.0f + pos.y,
			pos.z
		};

	u32 props = collMgr.getTileBehaviour2At(checkWhere.x, checkWhere.y, currentLayerID);

	//if (getSubType(props) == B_SUB_LEDGE)
	if (((props >> 16) & 0xFF) == 8)
		return false;

	float someFloat = 0.0f;
	if (collMgr.sub_800757B0(&checkWhere, &someFloat, currentLayerID, 1, -1)) {
		if (someFloat < checkWhere.y && someFloat > (pos.y - 5.0f))
			return true;
	}

	return false;
}


dAcPy_c *daTogeMetbo_c::detectPlayer() {
	Vec2 dist = (Vec2){this->xSeeDistance, this->ySeeDistance};
	Vec2 pos = (Vec2){this->pos.x, this->pos.y};
	dAcPy_c *player = this->doSearchNearPlayer(&dist, &pos);

	if (player == 0) return 0;

	if (RectanglesOverlap(
		&(Vec){
			player->pos.x - player->aPhysics.info.xDistToEdge + player->aPhysics.info.xDistToCenter,
			player->pos.y - player->aPhysics.info.yDistToEdge + player->aPhysics.info.yDistToCenter,
		},
		&(Vec){
			player->pos.x + player->aPhysics.info.xDistToEdge + player->aPhysics.info.xDistToCenter,
			player->pos.y + player->aPhysics.info.yDistToEdge + player->aPhysics.info.yDistToCenter,
		},
		&(Vec){
			this->pos.x - (this->direction == 1 ? this->xSeeDistance : this->xSeeDistance / 2),
			this->pos.y - this->ySeeDistance,
		},
		&(Vec){
			this->pos.x + (this->direction == 1 ? this->xSeeDistance / 2 : this->xSeeDistance),
			this->pos.y + this->ySeeDistance,
		}
	)) { return player; }
}

float daTogeMetbo_c::distFromTargetPlayer() {
	if (this->targetPlayer == 0) return 0.0;

	float xDist = this->targetPlayer->pos.x - this->pos.x;
	float yDist = this->targetPlayer->pos.y - this->pos.y;

	return sqrtf(xDist * xDist + yDist * yDist);
}

///////////////
// Wait State
///////////////
void daTogeMetbo_c::beginState_Wait() {
	this->max_speed.x = 0.0;
	this->speed.x = 0.0;
	this->x_speed_inc = 0.0;

	this->max_speed.y = -4.0;
	this->speed.y = -4.0;
	this->y_speed_inc = -0.1875;

	this->timer = 0;
	this->maxTimer = MakeRandomNumber(120) + 120;

	bindAnimChr_and_setUpdateRate("wait", 1, 0.0, 1.0);

	this->targetPlayer = 0;
}
void daTogeMetbo_c::executeState_Wait() {
	if (this->animationChr.isAnimationDone()) {
		this->animationChr.setCurrentFrame(0.0);
	}

	if (this->timer > this->maxTimer) {
		return this->doStateChange(&StateID_Walk);
	}
	this->timer++;

	calculateTileCollisions();

	dAcPy_c *player = detectPlayer();
	if (player != 0) {
		this->targetPlayer = player;
		return this->doStateChange(&StateID_FoundPlayer);
	}
}
void daTogeMetbo_c::endState_Wait() {
	this->previousState = &StateID_Wait;
}

///////////////
// Walk State
///////////////
void daTogeMetbo_c::beginState_Walk() {
	this->timer = 0;
	this->maxTimer = MakeRandomNumber(180) + 60;

	bindAnimChr_and_setUpdateRate("walk", 1, 0.0, 1.0);

	this->max_speed.x = (this->direction) ? -this->XSpeed : this->XSpeed;
	this->speed.x = (this->direction) ? -0.5 : 0.5;
	this->x_speed_inc = 0.15;

	this->max_speed.y = -4.0;
	this->speed.y = -4.0;
	this->y_speed_inc = -0.1875;

	this->targetPlayer = 0;
	this->sfxTimer = 0;
}
void daTogeMetbo_c::executeState_Walk() {
	if (collMgr.isOnTopOfTile()) {
		stillFalling = false;

		if (!willWalkOntoSuitableGround()) {
			pos.x = direction ? pos.x + 1.5 : pos.x - 1.5;
			return doStateChange(&StateID_Turn);
		}
	}
	else {
		if (!stillFalling) {
			stillFalling = true;
			pos.x = direction ? pos.x + 1.5 : pos.x - 1.5;
			return doStateChange(&StateID_Turn);
		}
	}

	if (this->pos.x > this->maxX && this->direction == 0) {
		return doStateChange(&StateID_Turn);
	}
	if (this->pos.x < this->minX && this->direction == 1) {
		return doStateChange(&StateID_Turn);
	}

	if (this->timer > this->maxTimer && collMgr.isOnTopOfTile()) {
		return this->doStateChange(&StateID_Walk);
	}
	this->timer++;


	bool ret = calculateTileCollisions();
	if (ret) {
		return doStateChange(&StateID_Turn);
	}

	if (this->animationChr.isAnimationDone()) {
		this->animationChr.setCurrentFrame(0.0);
	}

	dAcPy_c *player = detectPlayer();
	if (player != 0) {
		this->targetPlayer = player;
		return this->doStateChange(&StateID_FoundPlayer);
	}
}
void daTogeMetbo_c::endState_Walk() {
	this->previousState = &StateID_Walk;
}

///////////////
// Turn State
///////////////
void daTogeMetbo_c::beginState_Turn() {
	bindAnimChr_and_setUpdateRate(this->targetPlayer == 0 ? "walk" : "run", 1, 0.0, 1.0);

	this->direction ^= 1;
	this->speed.x = 0.0;
	this->x_speed_inc = 0.0;
}
void daTogeMetbo_c::executeState_Turn() {
	if (this->animationChr.isAnimationDone()) {
		this->animationChr.setCurrentFrame(0.0);
	}

	u16 amt = (this->direction == 0) ? 0x2800 : 0xD800;
	int done = SmoothRotation(&this->rot.y, amt, 0x800);

	if (done) {
		this->doStateChange(this->previousState);
	}
}
void daTogeMetbo_c::endState_Turn() { }

///////////////
// Found Player State
///////////////
void daTogeMetbo_c::beginState_FoundPlayer() {
	if (this->targetPlayer == 0) return this->doStateChange(&StateID_Walk);

	bindAnimChr_and_setUpdateRate("search", 1, 0.0, 0.5);
	playSoundDistance(this->handle, this->pos, SFX_TOGE_METBO_JUMP);
	this->direction = (this->targetPlayer->pos.x > this->pos.x) ? 0 : 1;
	this->speed.y = 2.5;
	this->speed.x = 0.0;
	this->x_speed_inc = 0.0;
	this->max_speed.x = 0.0;
}
void daTogeMetbo_c::executeState_FoundPlayer() {
	calculateTileCollisions();

	u16 amt = (this->direction == 0) ? 0x2800 : 0xD800;
	int done = SmoothRotation(&this->rot.y, amt, 0xB00);

	if (this->animationChr.isAnimationDone() && done) return this->doStateChange(&StateID_Follow);
}
void daTogeMetbo_c::endState_FoundPlayer() { }

///////////////
// Follow State
///////////////
void daTogeMetbo_c::beginState_Follow() {
	bindAnimChr_and_setUpdateRate("run", 1, 0.0, 0.5);

	this->max_speed.x = ((this->direction) ? -this->XSpeed : this->XSpeed) * 2.0;
	this->speed.x = ((this->direction) ? -0.5 : 0.5) * 2.0;
	this->x_speed_inc = 0.15;

	this->max_speed.y = -4.0;
	this->speed.y = -4.0;
	this->y_speed_inc = -0.1875;

	this->sfxTimer = 0;
}
void daTogeMetbo_c::executeState_Follow() {
	if (this->targetPlayer == 0) return this->doStateChange(&StateID_Walk);

	if (this->animationChr.isAnimationDone()) {
		this->animationChr.setCurrentFrame(0.0);
	}

	bool ret = calculateTileCollisions();

	if (direction == 0 && this->pos.x > this->targetPlayer->pos.x) {
		return this->doStateChange(&StateID_Turn);
	}
	else if (direction == 1 && this->pos.x < this->targetPlayer->pos.x) {
		return this->doStateChange(&StateID_Turn);
	}

	float xDist = this->targetPlayer->pos.x - this->pos.x;
	float newXSeeDistance = this->xSeeDistance * 2.0;
	float yDist = this->targetPlayer->pos.y - this->pos.y;
	float newYSeeDistance = this->ySeeDistance * 2.0;

	if (xDist > newXSeeDistance || xDist < -newXSeeDistance) {
		return this->doStateChange(&StateID_Walk);
	}

	if (yDist > newYSeeDistance || yDist < -newYSeeDistance) {
		return this->doStateChange(&StateID_Walk);
	}

	if ((this->distFromTargetPlayer() < 64) || ret) {
		return this->doStateChange(&StateID_Jump);
	}
}
void daTogeMetbo_c::endState_Follow() {
	this->previousState = &StateID_Follow;
}

///////////////
// Jump Sign State
///////////////
void daTogeMetbo_c::beginState_JumpSign() {
	this->speed.x = 0.0;
	this->x_speed_inc = 0.0;
	this->max_speed.x = 0.0;

	bindAnimChr_and_setUpdateRate("attack_sign", 1, 0.0, 0.5);
}
void daTogeMetbo_c::executeState_JumpSign() {
	calculateTileCollisions();
	if (this->animationChr.isAnimationDone()) return this->doStateChange(&StateID_Jump);
}
void daTogeMetbo_c::endState_JumpSign() { }

///////////////
// Jump State
///////////////
void daTogeMetbo_c::beginState_Jump() {
	bindAnimChr_and_setUpdateRate("attack_jump", 1, 0.0, 1.0);
	playSoundDistance(this->handle, this->pos, SFX_TOGE_METBO_JUMP);

	this->speed.x = (this->direction) ? -1.5 : 1.5;
	this->x_speed_inc = 0.0;
	this->max_speed.x = (this->direction) ? -1.5 : 1.5;

	this->speed.y = 4.0;
	this->y_speed_inc = -0.1875;
	this->max_speed.y = -4.0;
}
void daTogeMetbo_c::executeState_Jump() {
	calculateTileCollisions();

	if (this->animationChr.isAnimationDone()) {
		this->animationChr.setUpdateRate(0.0);
		this->animationChr.setCurrentFrame(this->animationChr.numFrames - 1.0);
	}

	if (collMgr.isOnTopOfTile()) return doStateChange(&StateID_Down);
}
void daTogeMetbo_c::endState_Jump() { }

///////////////
// Down State
///////////////
void daTogeMetbo_c::beginState_Down() {
	bindAnimChr_and_setUpdateRate("swoon", 1, 0.0, 0.5);

	this->speed.x = 0.0;
	this->x_speed_inc = 0.0;
	this->max_speed.x = 0.0;

	this->timer = 0;
}
void daTogeMetbo_c::executeState_Down() {
	calculateTileCollisions();
	if (collMgr.isOnTopOfTile()) timer++;

	if (this->animationChr.isAnimationDone()) {
		this->animationChr.setCurrentFrame(0.0);
	}

	if (this->timer > 180 && collMgr.isOnTopOfTile()) {
		return this->doStateChange(&StateID_RecoverSign);
	}
}
void daTogeMetbo_c::endState_Down() { }

///////////////
// Recover Sign State
///////////////
void daTogeMetbo_c::beginState_RecoverSign() {
	bindAnimChr_and_setUpdateRate("recover_sign", 1, 0.0, 0.5);
}
void daTogeMetbo_c::executeState_RecoverSign() {
	calculateTileCollisions();
	if (this->animationChr.isAnimationDone()) return this->doStateChange(&StateID_Recover);
}
void daTogeMetbo_c::endState_RecoverSign() { }

///////////////
// Recover State
///////////////
void daTogeMetbo_c::beginState_Recover() {
	bindAnimChr_and_setUpdateRate("recover", 1, 0.0, 0.5);
}
void daTogeMetbo_c::executeState_Recover() {
	calculateTileCollisions();
	if (this->animationChr.isAnimationDone()) return this->doStateChange(&StateID_Wait);
}
void daTogeMetbo_c::endState_Recover() { }

///////////////
// Block Projectile State
///////////////
void daTogeMetbo_c::beginState_BlockProjectile() {
	bindAnimChr_and_setUpdateRate("reaction_fireball", 1, 0.0, 0.5);

	this->speed.x = 0.0;
	this->x_speed_inc = 0.0;
	this->max_speed.x = 0.0;
}
void daTogeMetbo_c::executeState_BlockProjectile() {
	calculateTileCollisions();
	if (this->animationChr.isAnimationDone() && collMgr.isOnTopOfTile()) return this->doStateChange(this->previousState);
}
void daTogeMetbo_c::endState_BlockProjectile() { }

///////////////
// Die by Stomp State
///////////////
void daTogeMetbo_c::beginState_DieStomp() {
	this->removeMyActivePhysics();

	bindAnimChr_and_setUpdateRate("damage", 1, 0.0, 1.0);
	this->rot.y = 0;
	this->rot.x = 0;
}
void daTogeMetbo_c::executeState_DieStomp() {
	if (this->animationChr.isAnimationDone()) {
		this->Delete(true);
	}
}
void daTogeMetbo_c::endState_DieStomp() { }
