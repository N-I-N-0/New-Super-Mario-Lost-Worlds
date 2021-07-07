#include <common.h>
#include <game.h>
#include <g3dhax.h>
#include <sfx.h>


const char* PoihanaFileList [] = { "cataquack", NULL };

void shyCollisionCallback(ActivePhysics *apThis, ActivePhysics *apOther);
void ChuckleAndKnuckles(ActivePhysics *apThis, ActivePhysics *apOther);

class daPoihana_c : public dEn_c {
public:
	int onCreate();
	int onDelete();
	int onExecute();
	int onDraw();

	mHeapAllocator_c allocator;
	nw4r::g3d::ResFile resFile;

	m3d::mdl_c bodyModel;

	m3d::anmChr_c chrAnimation;

	mEf::es2 effect;

	int timer;
	int jumpCounter;
	float dying;
	float Baseline;
	char damage;
	char isDown;
	int distance;
	float XSpeed;
	u32 cmgr_returnValue;
	bool isBouncing;
	char backFire;
	int directionStore;
	bool stillFalling;

	StandOnTopCollider giantRider;
	ActivePhysics Chuckle;
	ActivePhysics Knuckles;

	static dActor_c *build();

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
	bool collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther);

	bool collisionCat5_Mario(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat11_PipeCannon(ActivePhysics *apThis, ActivePhysics *apOther);

	//void bouncePlayerWhenJumpedOn(void *player);
	
	void _vf148();
	void _vf14C();
	bool CreateIceActors();

	bool willWalkOntoSuitableGround();

	USING_STATES(daPoihana_c);
	DECLARE_STATE(RealWalk);
	DECLARE_STATE(RealTurn);

	DECLARE_STATE(GoDizzy);
	DECLARE_STATE(FireKnockBack);
	DECLARE_STATE(FlameHit);
	DECLARE_STATE(Recover);

	DECLARE_STATE(Die);

	int type;
};

dActor_c *daPoihana_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daPoihana_c));
	return new(buffer) daPoihana_c;
}

const SpriteData PoihanaSpriteData = { ProfileId::Poihana, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile PoihanaProfile(&daPoihana_c::build, SpriteId::Poihana, PoihanaSpriteData, ProfileId::Poihana, ProfileId::Poihana, "Poihana", PoihanaFileList);


///////////////////////
// Externs and States
///////////////////////
extern "C" bool SpawnEffect(const char*, int, Vec*, S16Vec*, Vec*);

//FIXME make this dEn_c->used...
extern "C" char usedForDeterminingStatePress_or_playerCollision(dEn_c* t, ActivePhysics *apThis, ActivePhysics *apOther, int unk1);
extern "C" int SomeStrangeModification(dStageActor_c* actor);
extern "C" void DoStuffAndMarkDead(dStageActor_c *actor, Vec vector, float unk);
extern "C" int SmoothRotation(short* rot, u16 amt, int unk2);

extern "C" bool HandlesEdgeTurns(dEn_c* actor);
extern "C" void dAcPy_vf3F8(void* player, dEn_c* monster, int t);

CREATE_STATE(daPoihana_c, RealWalk);
CREATE_STATE(daPoihana_c, RealTurn);

CREATE_STATE(daPoihana_c, GoDizzy);
CREATE_STATE(daPoihana_c, FireKnockBack);
CREATE_STATE(daPoihana_c, FlameHit);
CREATE_STATE(daPoihana_c, Recover);

CREATE_STATE(daPoihana_c, Die);

////////////////////////
// Collision Functions
////////////////////////

void ChuckleAndKnuckles(ActivePhysics *apThis, ActivePhysics *apOther) {
	if (apOther->owner->name != PLAYER) { return; }
	((dEn_c*)apThis->owner)->_vf220(apOther->owner);
}


void daPoihana_c::spriteCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	u16 name = ((dEn_c*)apOther->owner)->name;

	if (name == EN_COIN || name == EN_EATCOIN || name == AC_BLOCK_COIN || name == EN_COIN_JUGEM || name == EN_COIN_ANGLE
		|| name == EN_COIN_JUMP || name == EN_COIN_FLOOR || name == EN_COIN_VOLT || name == EN_COIN_WIND
		|| name == EN_BLUE_COIN || name == EN_COIN_WATER || name == EN_REDCOIN || name == EN_GREENCOIN
		|| name == EN_JUMPDAI || name == EN_ITEM)
		{ return; }

	if (acState.getCurrentState() == &StateID_RealWalk) {

		pos.x = ((pos.x - ((dEn_c*)apOther->owner)->pos.x) > 0) ? pos.x + 1.5 : pos.x - 1.5;
		doStateChange(&StateID_RealTurn); }

	if (acState.getCurrentState() == &StateID_FireKnockBack) {
		float distance = pos.x - ((dEn_c*)apOther->owner)->pos.x;
		pos.x = pos.x + (distance / 4.0);
	}

	dEn_c::spriteCollision(apThis, apOther);
}

void daPoihana_c::playerCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	dStateBase_c *stateVar;
	dStateBase_c *deathState;
	dStageActor_c *player = apOther->owner;
	char hitType;
	stateVar = &StateID_GoDizzy;
	deathState = &StateID_Die;
	this->playEnemyDownSound1();
	doStateChange(stateVar); 
	bouncePlayer(player, 7.0f);
	OSReport("BOUNCING TO THE SKYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY");
	PlaySound(this, SE_EMY_CMN_STEP);
}

void daPoihana_c::yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	this->playerCollision(apThis, apOther);
}
bool daPoihana_c::collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther) {
	PlaySound(this, SE_EMY_DOWN);
	S16Vec nullRot = {0,0,0};
	Vec oneVec = {1.0f, 1.0f, 1.0f};
	SpawnEffect("Wm_mr_hardhit", 0, &pos, &nullRot, &oneVec);
	doStateChange(&StateID_Die);
	return true;
}
bool daPoihana_c::collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->collisionCatD_Drill(apThis, apOther);
}
bool daPoihana_c::collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->collisionCatD_Drill(apThis, apOther);
}
bool daPoihana_c::collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->collisionCatD_Drill(apThis, apOther);
}
bool daPoihana_c::collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther){
	return this->collisionCatD_Drill(apThis, apOther);
}
bool daPoihana_c::collisionCat5_Mario(ActivePhysics *apThis, ActivePhysics *apOther){
	return this->collisionCatD_Drill(apThis, apOther);
}
bool daPoihana_c::collisionCat11_PipeCannon(ActivePhysics *apThis, ActivePhysics *apOther){
	return this->collisionCatD_Drill(apThis, apOther);
}
bool daPoihana_c::collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther) {
	StageE4::instance->spawnCoinJump(pos, 0, 2, 0);
	return this->collisionCatD_Drill(apThis, apOther);
}

bool daPoihana_c::collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther){
	bool wut = dEn_c::collisionCat3_StarPower(apThis, apOther);
	doStateChange(&StateID_Die);
	return wut;
}

bool daPoihana_c::collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther){
	doStateChange(&StateID_DieSmoke);
	return true;
}
bool daPoihana_c::collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther) {
	this->damage += 1;
	dStateBase_c *stateVar;
	stateVar = &StateID_DieSmoke;
	if (this->type < 6) {
		backFire = apOther->owner->direction ^ 1;
		StageE4::instance->spawnCoinJump(pos, 0, 1, 0);
		doStateChange(&StateID_DieSmoke);
	}

	if (this->damage > 1) {
		nw4r::snd::SoundHandle handle;
		PlaySoundWithFunctionB4(SoundRelatedClass, &handle, SE_EMY_DOWN, 1);
		StageE4::instance->spawnCoinJump(pos, 0, 1, 0);
		doStateChange(&StateID_DieSmoke);
	}
	else {
		nw4r::snd::SoundHandle handle;
		PlaySoundWithFunctionB4(SoundRelatedClass, &handle, SE_EMY_KURIBO_L_DAMAGE_01, 1);
		doStateChange(stateVar);
	}
	return true;
}

// These handle the ice crap
void daPoihana_c::_vf148() {
	dEn_c::_vf148();
	doStateChange(&StateID_Die);
}
void daPoihana_c::_vf14C() {
	dEn_c::_vf14C();
	doStateChange(&StateID_Die);
}

extern "C" void sub_80024C20(void);
extern "C" void __destroy_arr(void*, void(*)(void), int, int);

bool daPoihana_c::CreateIceActors() {
	struct DoSomethingCool my_struct = { 0, this->pos, {1.9, 1.7, 2.5}, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
	this->frzMgr.Create_ICEACTORs( (void*)&my_struct, 1 );
	__destroy_arr( (void*)&my_struct, sub_80024C20, 0x3C, 1 );
	chrAnimation.setUpdateRate(0.0f);
	return true;
}

bool daPoihana_c::calculateTileCollisions() {
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

void daPoihana_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate) {
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr(name);
	this->chrAnimation.bind(&this->bodyModel, anmChr, unk);
	this->bodyModel.bindAnim(&this->chrAnimation, unk2);
	this->chrAnimation.setUpdateRate(rate);
}

int daPoihana_c::onCreate() {
	this->type = this->settings >> 28 & 0xF;
	this->distance = this->settings >> 12 & 0xF;

	stillFalling = 0;

	allocator.link(-1, GameHeaps[0], 0, 0x20);

	this->deleteForever = 1;
	this->resFile.data = getResource("cataquack", "g3d/cataquack.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("poihana");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);

	// Animations start here
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr("walk");
	this->chrAnimation.setup(mdl, anmChr, &this->allocator, 0);

	allocator.unlink();

	// Stuff I do understand
	this->scale = (Vec){0.1, 0.1, 0.1};

	this->pos.y += 36.0;
	this->rot.x = 0; // X is vertical axis
	this->rot.y = 0xD800; // Y is horizontal axis
	this->rot.z = 0; // Z is ... an axis >.>
	this->direction = 1; // Heading left.

	this->speed.x = 0.0;
	this->speed.y = 0.0;
	this->max_speed.x = 0.6;
	this->x_speed_inc = 0.15;
	this->Baseline = this->pos.y;
	this->XSpeed = 0.6;


	ActivePhysics::Info HitMeBaby;

	// Note: if this gets changed, also change the point where the default
	HitMeBaby.xDistToCenter = 0.0;
	HitMeBaby.yDistToCenter = 12.0;
	HitMeBaby.xDistToEdge = 8.0;
	HitMeBaby.yDistToEdge = 12.0;

	HitMeBaby.category1 = 0x3;
	HitMeBaby.category2 = 0x0;
	HitMeBaby.bitfield1 = 0x6F;
	HitMeBaby.bitfield2 = 0xffbafffe;
	HitMeBaby.unkShort1C = 0;
	HitMeBaby.callback = &shyCollisionCallback;

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


	// State Changers

	if (type == 0) {
		bindAnimChr_and_setUpdateRate("walk", 1, 0.0, 1.5);
		doStateChange(&StateID_RealWalk);
	}
	else if (type == 1) {
		bindAnimChr_and_setUpdateRate("walk", 1, 0.0, 1.5);
		doStateChange(&StateID_RealWalk);
	}

	return true;
}

int daPoihana_c::onDelete() {
	return true;
}

int daPoihana_c::onExecute() {
	acState.execute();
	updateModelMatrices();
	bodyModel._vf1C();

	return true;
}

int daPoihana_c::onDraw() {
	bodyModel.scheduleForDrawing();
	return true;
}

void daPoihana_c::updateModelMatrices() {
	// This won't work with wrap because I'm lazy.

/*	if (this->frzMgr._mstate == 1)
		matrix.translation(pos.x, pos.y, pos.z);
	else*/
	matrix.translation(pos.x, pos.y + 20, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

///////////////
// Real Walk State
///////////////
bool daPoihana_c::willWalkOntoSuitableGround() {
	static const float deltas[] = {2.5f, -2.5f};
	VEC3 checkWhere = {
			pos.x + deltas[direction],
			4.0f + pos.y,
			pos.z};

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


void daPoihana_c::beginState_RealWalk() {
	//inline this piece of code
	this->max_speed.x = (this->direction) ? -this->XSpeed : this->XSpeed;
	this->speed.x = (direction) ? -0.6f : 0.6f;

	this->max_speed.y = -4.0;
	this->speed.y = -4.0;
	this->y_speed_inc = -0.1875;
}
void daPoihana_c::executeState_RealWalk() {
	chrAnimation.setUpdateRate(1.5f);
	if (distance) {
		if (collMgr.isOnTopOfTile()) {
			stillFalling = false;

			if (!willWalkOntoSuitableGround()) {
				pos.x = direction ? pos.x + 1.5 : pos.x - 1.5;
				doStateChange(&StateID_RealTurn);
			}
		}
		else {
			if (!stillFalling) {
				stillFalling = true;
				pos.x = direction ? pos.x + 1.5 : pos.x - 1.5;
				doStateChange(&StateID_RealTurn);
			}
		}
	}

	bool ret = calculateTileCollisions();
	if (ret) {
		doStateChange(&StateID_RealTurn);
	}

	if(this->chrAnimation.isAnimationDone()) {
		this->chrAnimation.setCurrentFrame(0.0);
	}
}
void daPoihana_c::endState_RealWalk() { }

///////////////
// Real Turn State
///////////////
void daPoihana_c::beginState_RealTurn() {

	this->direction ^= 1;
	this->speed.x = 0.0;
}
void daPoihana_c::executeState_RealTurn() {

	if(this->chrAnimation.isAnimationDone()) {
		this->chrAnimation.setCurrentFrame(0.0);
	}

	u16 amt = (this->direction == 0) ? 0x2800 : 0xD800;
	int done = SmoothRotation(&this->rot.y, amt, 0x800);

	if(done) {
		this->doStateChange(&StateID_RealWalk);
	}
}
void daPoihana_c::endState_RealTurn() {
}

///////////////
// GoDizzy State
///////////////
void daPoihana_c::beginState_GoDizzy() {
	bindAnimChr_and_setUpdateRate("throw", 1, 0.0, 1.0);

	// SpawnEffect("Wm_en_spindamage", 0, &(Vec){this->pos.x, this->pos.y + 24.0, 0}, &(S16Vec){0,0,0}, &(Vec){1.0, 1.0, 1.0});

	this->max_speed.x = 0;
	this->speed.x = 0;
	this->x_speed_inc = 0;

	this->max_speed.y = -4.0;
	this->speed.y = -4.0;
	this->y_speed_inc = -0.1875;

	this->timer = 0;
	this->jumpCounter = 0;
	this->isDown = 1;
}
void daPoihana_c::executeState_GoDizzy() {
	calculateTileCollisions();

	Vec efPos = {this->pos.x, this->pos.y + 24.0f, 0};
	S16Vec nullRot = {0,0,0};
	Vec oneVec = {1.0f, 1.0f, 1.0f};
	//effect.spawn("Wm_en_spindamage", 0, &efPos, &nullRot, &oneVec);

	/*if (this->jumpCounter == 0) {
		if(this->chrAnimation.isAnimationDone()) {
			this->jumpCounter = 1;
			bindAnimChr_and_setUpdateRate("walk", 1, 0.0, 1.0);
		}
	}

	else {*/
		if(this->chrAnimation.isAnimationDone()) {
			this->chrAnimation.setCurrentFrame(0.0);
			doStateChange(&StateID_Recover);
			damage = 0;
		}

		/*if (this->timer > 600) {
			doStateChange(&StateID_Recover);
			damage = 0;
		}*/

		this->timer += 1;
	//}
}
void daPoihana_c::endState_GoDizzy() {}

///////////////
// FireKnockBack State
///////////////
void daPoihana_c::beginState_FireKnockBack() {
	bindAnimChr_and_setUpdateRate("overturn", 1, 0.0, 1.0);

	// Backfire 0 == Fireball to the right
	// Backfire 1 == Fireball to the left

	directionStore = this->direction;
	speed.x = (this->backFire) ? this->XSpeed : -this->XSpeed;
	speed.x *= 1.2f;
	max_speed.x = speed.x;
	x_speed_inc = 0.0f;
}
void daPoihana_c::executeState_FireKnockBack() {

	calculateTileCollisions();
	// move backwards here
	this->speed.x = this->speed.x / 1.02f;

	if(this->chrAnimation.isAnimationDone()) {
		if (aPhysics.result1 == 0 && aPhysics.result2 == 0 && aPhysics.result3 == 0) {
			bindAnimChr_and_setUpdateRate("walk", 1, 0.0, 1.5);
			doStateChange(&StateID_RealWalk);
		}
	}
}
void daPoihana_c::endState_FireKnockBack() {
	this->direction = directionStore;
}

///////////////
// FlameHit State
///////////////
void daPoihana_c::beginState_FlameHit() {
	bindAnimChr_and_setUpdateRate("overturn", 1, 0.0, 1.0);
}
void daPoihana_c::executeState_FlameHit() {

	if(this->chrAnimation.isAnimationDone()) {
	}
}
void daPoihana_c::endState_FlameHit() {}

///////////////
// Recover State
///////////////
void daPoihana_c::beginState_Recover() {
	//bindAnimChr_and_setUpdateRate("c18_L_DMG_F_4_R", 1, 0.0, 1.0);
}
void daPoihana_c::executeState_Recover() {

	calculateTileCollisions();

	//if(this->chrAnimation.isAnimationDone()) {
		bindAnimChr_and_setUpdateRate("walk", 1, 0.0, 1.5);
		doStateChange(&StateID_RealWalk);
	//}
}
void daPoihana_c::endState_Recover() {
	this->isDown = 0;
	this->rot.y = (direction) ? 0xD800 : 0x2800;
}

///////////////
// Die State
///////////////
void daPoihana_c::beginState_Die() {
	// dEn_c::dieFall_Begin();
	this->removeMyActivePhysics();

	bindAnimChr_and_setUpdateRate("overturn", 1, 0.0, 2.0);
	this->timer = 0;
	this->dying = -10.0;
	this->Baseline = this->pos.y;
	this->rot.y = 0;
	this->rot.x = 0;

	if (type > 5 && type < 9) {
		Vec efPos = {this->pos.x, this->pos.y - 32.0f, 0};
		S16Vec nullRot = {0,0,0};
		Vec oneVec = {1.0f, 1.0f, 1.0f};
		SpawnEffect("Wm_en_explosion_ln", 0, &efPos, &nullRot, &oneVec);
	}
}
void daPoihana_c::executeState_Die() {

	if(this->chrAnimation.isAnimationDone()) {
		this->chrAnimation.setCurrentFrame(0.0);
	}

	this->timer += 1;

	// this->pos.x += 0.5;
	this->pos.y = Baseline + (-0.2 * dying * dying) + 20.0;

	this->dying += 0.5;

	if (this->timer > 450) {
		OSReport("Killing");
		this->kill();
		this->Delete(this->deleteForever);
	}

	// dEn_c::dieFall_Execute();

}
void daPoihana_c::endState_Die() { }