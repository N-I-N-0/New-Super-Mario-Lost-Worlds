#include <common.h>
#include <game.h>
#include <g3dhax.h>
#include <sfx.h>
#include "profile.h"


const char* GoombratArcNameList [] = {
	"kakibo",
	NULL
};

class daGoombrat_c : public dEn_c {
	int onCreate();
	int onDelete();
	int onExecute();
	int onDraw();

	mHeapAllocator_c allocator;
	nw4r::g3d::ResFile resFile;

	m3d::mdl_c model;

	m3d::anmChr_c animationChr;
	m3d::anmTexPat_c animationPat;
	nw4r::g3d::ResAnmTexPat anmPat;

	mEf::es2 effect;

	float XSpeed;
	u32 cmgr_returnValue;
	bool isBouncing;
	bool stillFalling;

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
	// bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther);

	bool collisionCat5_Mario(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat11_PipeCannon(ActivePhysics *apThis, ActivePhysics *apOther);

	void _vf148();
	void _vf14C();
	bool CreateIceActors();

	bool willWalkOntoSuitableGround();

	USING_STATES(daGoombrat_c);
	DECLARE_STATE(Walk);
	DECLARE_STATE(Turn);
	DECLARE_STATE(DieStomp);

	public: static dActor_c *build();
};

dActor_c *daGoombrat_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daGoombrat_c));
	return new(buffer) daGoombrat_c;
}

const SpriteData GoombratSpriteData = { ProfileId::Goombrat, 8, -8, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile GoombratProfile(&daGoombrat_c::build, SpriteId::Goombrat, &GoombratSpriteData, ProfileId::Goombrat, ProfileId::Goombrat, "Goombrat", GoombratArcNameList, 0);

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


CREATE_STATE(daGoombrat_c, Walk);
CREATE_STATE(daGoombrat_c, Turn);
CREATE_STATE(daGoombrat_c, DieStomp);

////////////////////////
// Collision Functions
////////////////////////
void daGoombrat_c::spriteCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
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

void daGoombrat_c::playerCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
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

void daGoombrat_c::yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
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
bool daGoombrat_c::collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther) {
	PlaySound(this, SE_EMY_DOWN);
	SpawnEffect("Wm_mr_hardhit", 0, &pos, &(S16Vec){0, 0, 0}, &(Vec){1.0, 1.0, 1.0});
	dEn_c::_vf148();
	return true;
}
bool daGoombrat_c::collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->collisionCatD_Drill(apThis, apOther);
}
bool daGoombrat_c::collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->collisionCatD_Drill(apThis, apOther);
}
bool daGoombrat_c::collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->collisionCatD_Drill(apThis, apOther);
}
bool daGoombrat_c::collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->collisionCatD_Drill(apThis, apOther);
}
bool daGoombrat_c::collisionCat5_Mario(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->collisionCatD_Drill(apThis, apOther);
}
bool daGoombrat_c::collisionCat11_PipeCannon(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->collisionCatD_Drill(apThis, apOther);
}
bool daGoombrat_c::collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther) {
	StageE4::instance->spawnCoinJump(pos, 0, 2, 0);
	return this->collisionCatD_Drill(apThis, apOther);
}

bool daGoombrat_c::collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther) {
	bool wut = dEn_c::collisionCat3_StarPower(apThis, apOther);
	return wut;
}

bool daGoombrat_c::collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->collisionCat1_Fireball_E_Explosion(apThis, apOther);
}

bool daGoombrat_c::collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther) {
	StageE4::instance->spawnCoinJump(pos, 0, 1, 0);
	return this->collisionCatD_Drill(apThis, apOther);
}

// void daGoombrat_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther) {
	
// 	dEn_C::collisionCat2_IceBall_15_YoshiIce(apThis, apOther);
// }

// These handle the ice crap
void daGoombrat_c::_vf148() {
	dEn_c::_vf148();
}
void daGoombrat_c::_vf14C() {
	dEn_c::_vf14C();
}

extern "C" void sub_80024C20(void);
extern "C" void __destroy_arr(void*, void(*)(void), int, int);
//extern "C" __destroy_arr(struct DoSomethingCool, void(*)(void), int cnt, int bar);

bool daGoombrat_c::CreateIceActors() {
	struct DoSomethingCool my_struct = { 0, this->pos, {1.25, 1.25, 1.25}, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
	this->frzMgr.Create_ICEACTORs( (void*)&my_struct, 1 );
	__destroy_arr( (void*)&my_struct, sub_80024C20, 0x3C, 1 );
	this->animationChr.setUpdateRate(0.0f);
	this->animationPat.setUpdateRateForEntry(2.0f, 0);
	return true;
}

bool daGoombrat_c::calculateTileCollisions() {
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

void daGoombrat_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate) {
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr(name);
	this->animationChr.bind(&this->model, anmChr, unk);
	this->model.bindAnim(&this->animationChr, unk2);
	this->animationChr.setUpdateRate(rate);
}

int daGoombrat_c::onCreate() {
	this->stillFalling = 0;
	int color = this->settings >> 24 & 0xF;

	allocator.link(-1, GameHeaps[0], 0, 0x20);

	char resName[80];
	sprintf(resName, "g3d/t%02d.brres", color);
	resName[strlen(resName)] = 0;
	this->resFile.data = getResource("kakibo", resName);

	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("kakibo");
	model.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Enemy(&model, 0);


	// Animations start here
	bool ret;
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr("walk");
	ret = this->animationChr.setup(mdl, anmChr, &this->allocator, 0);

	this->anmPat = this->resFile.GetResAnmTexPat("walk");
	this->animationPat.setup(mdl, anmPat, &this->allocator, 0, 1);
	this->animationPat.bindEntry(&this->model, &anmPat, 0, 1);
	this->animationPat.setFrameForEntry(0, 0);
	this->animationPat.setUpdateRateForEntry(2.0f, 0);
	this->model.bindAnim(&this->animationPat);

	allocator.unlink();

	// Stuff I do understand

	this->scale = (Vec){1.0, 1.0, 1.0};

	this->rot.x = 0; // X is vertical axis
	this->rot.y = 0xD800; // Y is horizontal axis
	this->rot.z = 0; // Z is ... an axis >.>
	this->direction = 1; // Heading left.
	
	this->speed.x = 0.0;
	this->speed.y = 0.0;
	this->max_speed.x = 0.5;
	this->x_speed_inc = 0.15;
	this->XSpeed = 0.5;


	ActivePhysics::Info HitMeBaby;

	// Note: if this gets changed, also change the point where the default
	// values are assigned after de-ballooning
	HitMeBaby.xDistToCenter = 0.0;
	HitMeBaby.yDistToCenter = 8.0;
	HitMeBaby.xDistToEdge = 8.0;
	HitMeBaby.yDistToEdge = 8.0;

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
	static const lineSensor_s below(-5<<12, 5<<12, 0<<12);
	static const pointSensor_s above(0<<12, 12<<12);
	static const lineSensor_s adjacent(6<<12, 9<<12, 6<<12);

	collMgr.init(this, &below, &above, &adjacent);
	collMgr.calculateBelowCollisionWithSmokeEffect();

	cmgr_returnValue = collMgr.isOnTopOfTile();

	if (collMgr.isOnTopOfTile())
		isBouncing = false;
	else
		isBouncing = true;

	bindAnimChr_and_setUpdateRate("walk", 1, 0.0, 2.0);
	doStateChange(&StateID_Walk);

	this->onExecute();
	return true;
}

int daGoombrat_c::onDelete() {
	return true;
}

int daGoombrat_c::onExecute() {
	acState.execute();
	updateModelMatrices();
	model._vf1C();

	return true;
}

int daGoombrat_c::onDraw() {
	model.scheduleForDrawing();
	return true;
}

void daGoombrat_c::updateModelMatrices() {
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


bool daGoombrat_c::willWalkOntoSuitableGround() {
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

///////////////
// Walk State
///////////////
void daGoombrat_c::beginState_Walk() {
	this->max_speed.x = (this->direction) ? -this->XSpeed : this->XSpeed;
	this->speed.x = (direction) ? -0.5f : 0.5f;

	this->max_speed.y = -4.0;
	this->speed.y = -4.0;
	this->y_speed_inc = -0.1875;

	this->animationChr.setUpdateRate(2.0);
	this->animationPat.setUpdateRateForEntry(2.0f, 0);
}
void daGoombrat_c::executeState_Walk() {
	if (collMgr.isOnTopOfTile()) {
		stillFalling = false;

		if (!willWalkOntoSuitableGround()) {
			pos.x = direction ? pos.x + 1.5 : pos.x - 1.5;
			doStateChange(&StateID_Turn);
		}
	}
	else {
		if (!stillFalling) {
			stillFalling = true;
			pos.x = direction ? pos.x + 1.5 : pos.x - 1.5;
			doStateChange(&StateID_Turn);
		}
	}


	bool ret = calculateTileCollisions();
	if (ret) {
		doStateChange(&StateID_Turn);
	}

	if (this->animationChr.isAnimationDone()) {
		this->animationChr.setCurrentFrame(0.0);
	}
	if (this->animationPat.isEntryAnimationDone(0)) {
		this->animationPat.setFrameForEntry(0, 0);
		this->animationPat.setUpdateRateForEntry(2.0f, 0);
	}
}
void daGoombrat_c::endState_Walk() { }

///////////////
// Turn State
///////////////
void daGoombrat_c::beginState_Turn() {

	this->direction ^= 1;
	this->speed.x = 0.0;
}
void daGoombrat_c::executeState_Turn() {

	if (this->animationChr.isAnimationDone()) {
		this->animationChr.setCurrentFrame(0.0);
	}

	if (this->animationPat.isEntryAnimationDone(0)) {
		this->animationPat.setFrameForEntry(0, 0);
		this->animationPat.setUpdateRateForEntry(2.0f, 0);
	}

	u16 amt = (this->direction == 0) ? 0x2800 : 0xD800;
	int done = SmoothRotation(&this->rot.y, amt, 0x800);

	if(done) {
		this->doStateChange(&StateID_Walk);
	}
}
void daGoombrat_c::endState_Turn() { }

///////////////
// Die by Stomp State
///////////////
void daGoombrat_c::beginState_DieStomp() {
	this->removeMyActivePhysics();

	bindAnimChr_and_setUpdateRate("damage", 1, 0.0, 1.0);
	this->rot.y = 0;
	this->rot.x = 0;
}
void daGoombrat_c::executeState_DieStomp() {
	if(this->animationChr.isAnimationDone()) {
		this->Delete(true);
	}
}
void daGoombrat_c::endState_DieStomp() { }
