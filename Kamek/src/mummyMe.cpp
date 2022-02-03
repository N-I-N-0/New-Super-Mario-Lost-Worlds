#include <common.h>
#include <game.h>
#include <g3dhax.h>
#include <sfx.h>
#include <profile.h>

extern void shyCollisionCallback(ActivePhysics *apThis, ActivePhysics *apOther);
const char* MummyMeFileList [] = {
	"mummyMe",
	NULL	
};




class daMummyMe_c : public dEn_c {
	int onCreate();
	int onDelete();
	int onExecute();
	int onDraw();

	mHeapAllocator_c allocator;
	nw4r::g3d::ResFile resFile;
	nw4r::g3d::ResFile anmFile;

	m3d::mdl_c bodyModel;
	m3d::anmChr_c chrAnimation;
	mEf::es2 effect;

	int timer;
	int type;
	float dying;
	float Baseline;
	char damage;
	char isDown;
	Vec initialPos;
	int distance;
	float XSpeed;
	u32 cmgr_returnValue;
	bool isBouncing;
	int directionStore;

	public: static dActor_c *build();

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
	bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat5_Mario(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat11_PipeCannon(ActivePhysics *apThis, ActivePhysics *apOther);

	void powBlockActivated(bool isNotMPGP);

	void _vf148();
	void _vf14C();
	bool CreateIceActors();
	void addScoreWhenHit(void *other);
	void bouncePlayerWhenJumpedOn(void *player);

	void spawnHitEffectAtPosition(Vec2 pos);
	void doSomethingWithHardHitAndSoftHitEffects(Vec pos);
	void playEnemyDownSound2();
	void playHpdpSound1(); // plays PLAYER_SE_EMY/GROUP_BOOT/SE_EMY_DOWN_HPDP_S or _H
	void playEnemyDownSound1();
	void playEnemyDownComboSound(void *player); // AcPy_c/daPlBase_c?
	void playHpdpSound2(); // plays PLAYER_SE_EMY/GROUP_BOOT/SE_EMY_DOWN_HPDP_S or _H
	void _vf260(void *other); // AcPy/PlBase? plays the SE_EMY_FUMU_%d sounds based on some value
	void _vf264(dStageActor_c *other); // if other is player or yoshi, do Wm_en_hit and a few other things
	void _vf268(void *other); // AcPy/PlBase? plays the SE_EMY_DOWN_SPIN_%d sounds based on some value
	void _vf278(void *other); // AcPy/PlBase? plays the SE_EMY_YOSHI_FUMU_%d sounds based on some value

	USING_STATES(daMummyMe_c);
	DECLARE_STATE(RealWalk);
	DECLARE_STATE(RealTurn);
	DECLARE_STATE(Die);
};

const SpriteData MummyMeSpriteData = {ProfileId::MummyMe, 0x5, -0x31, 0, 0x10, 0x10, 0x40, 0x40, 0x40, 0, 0, 0};
Profile MummyMeProfile(&daMummyMe_c::build, SpriteId::MummyMe, &MummyMeSpriteData, ProfileId::MummyMe, ProfileId::MummyMe, "Mummy-Me", MummyMeFileList);

dActor_c *daMummyMe_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daMummyMe_c));
	return new(buffer) daMummyMe_c;
}

///////////////////////
// Externs and States
///////////////////////
extern "C" bool SpawnEffect(const char*, int, Vec*, S16Vec*, Vec*);

//FIXME make this dEn_c->used...
extern "C" char usedForDeterminingStatePress_or_playerCollision(dEn_c* t, ActivePhysics *apThis, ActivePhysics *apOther, int unk1);
extern "C" int SomeStrangeModification(dStageActor_c* actor);
extern "C" void DoStuffAndMarkDead(dStageActor_c *actor, Vec vector, float unk);
extern "C" int SmoothRotation(short* rot, u16 amt, int unk2);

// Collision related
extern "C" void BigHanaPlayer(dEn_c* t, ActivePhysics *apThis, ActivePhysics *apOther);
extern "C" void BigHanaYoshi(dEn_c* t, ActivePhysics *apThis, ActivePhysics *apOther);
extern "C" bool BigHanaWeirdGP(dEn_c* t, ActivePhysics *apThis, ActivePhysics *apOther);
extern "C" bool BigHanaGroundPound(dEn_c* t, ActivePhysics *apThis, ActivePhysics *apOther);
extern "C" bool BigHanaFireball(dEn_c* t, ActivePhysics *apThis, ActivePhysics *apOther);
extern "C" bool BigHanaIceball(dEn_c* t, ActivePhysics *apThis, ActivePhysics *apOther);

extern "C" void dAcPy_vf3F8(void* player, dEn_c* monster, int t);

CREATE_STATE(daMummyMe_c, RealWalk);
CREATE_STATE(daMummyMe_c, RealTurn);
CREATE_STATE(daMummyMe_c, Die);

////////////////////////
// Collision Functions
////////////////////////

void daMummyMe_c::playerCollision(ActivePhysics *apThis, ActivePhysics *apOther) { 
	apOther->someFlagByte |= 2;

	dStageActor_c *player = apOther->owner;

	char hitType;
	hitType = usedForDeterminingStatePress_or_playerCollision(this, apThis, apOther, 0);
	if (hitType > 0) {
		PlaySound(this, SE_EMY_CMN_STEP);
		this->counter_504[apOther->owner->which_player] = 0xA;
	} else {
		this->dEn_c::playerCollision(apThis, apOther);
		this->_vf220(apOther->owner);
		if (Player_VF3D4(player)) {
			// WE'VE GOT A STAR, FOLKS
			if (apThis->_18 == 1 && !player->collMgr.isOnTopOfTile() && player->pos.y > apThis->bottom()) {
				bouncePlayer(player, 3.0f);
				PlaySound(this, SE_EMY_CMN_STEP);
			} else {
				dAcPy_vf3F8(player, this, 3);
			}
			this->counter_504[apOther->owner->which_player] = 0xA;

		}
	} 
}			
void daMummyMe_c::_vf278(void *other) {
	PlaySound(this, SE_EMY_HANACHAN_STOMP);
}

void daMummyMe_c::bouncePlayerWhenJumpedOn(void *player) {
	bouncePlayer(player, 5.0f);
}

void daMummyMe_c::yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	this->playerCollision(apThis, apOther);
}
bool daMummyMe_c::collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther) {
	return BigHanaWeirdGP(this, apThis, apOther);
}
bool daMummyMe_c::collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther) {
	return BigHanaGroundPound(this, apThis, apOther);
}
bool daMummyMe_c::collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther) {
	return BigHanaGroundPound(this, apThis, apOther);
}

bool daMummyMe_c::collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther) {
	apOther->owner->kill();
	return true;
}
bool daMummyMe_c::collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther){
	dAcPy_vf3F8(apOther->owner, this, 3);
	this->counter_504[apOther->owner->which_player] = 0xA;
	return true;
}
bool daMummyMe_c::collisionCat5_Mario(ActivePhysics *apThis, ActivePhysics *apOther){
	return true;
}
bool daMummyMe_c::collisionCat11_PipeCannon(ActivePhysics *apThis, ActivePhysics *apOther){
	return true;
}
bool daMummyMe_c::collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther) {
	return true;
}
bool daMummyMe_c::collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther){
	/*int hitType = usedForDeterminingStatePress_or_playerCollision(this, apThis, apOther, 0);
	if (hitType == 1 || hitType == 3) {
		PlaySound(this, SE_EMY_CMN_STEP);
		bouncePlayerWhenJumpedOn(apOther->owner);
	} else {
		dAcPy_vf3F8(apOther->owner, this, 3);
	}

	this->counter_504[apOther->owner->which_player] = 0xA;
	return true;*/
	return false;
}

bool daMummyMe_c::collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther){
	return BigHanaFireball(this, apThis, apOther);
}
bool daMummyMe_c::collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther) {
	return BigHanaFireball(this, apThis, apOther);
}
bool daMummyMe_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther) {
	return BigHanaIceball(this, apThis, apOther);
}

// These handle the ice crap
void daMummyMe_c::_vf148() {
	dEn_c::_vf148();
	doStateChange(&StateID_Die);
}
void daMummyMe_c::_vf14C() {
	dEn_c::_vf14C();
	doStateChange(&StateID_Die);
}

extern "C" void sub_80024C20(void);
extern "C" void __destroy_arr(void*, void(*)(void), int, int);

bool daMummyMe_c::CreateIceActors() {
	struct DoSomethingCool my_struct = { 0, this->pos, {2.4, 3.0, 3.0}, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
	this->frzMgr.Create_ICEACTORs( (void*)&my_struct, 1 );
	__destroy_arr( (void*)&my_struct, sub_80024C20, 0x3C, 1 );
	return true;
}

void daMummyMe_c::addScoreWhenHit(void *other) { }

void daMummyMe_c::spawnHitEffectAtPosition(Vec2 pos) { }
void daMummyMe_c::doSomethingWithHardHitAndSoftHitEffects(Vec pos) { }
void daMummyMe_c::playEnemyDownSound2() { }
void daMummyMe_c::playHpdpSound1() { } // plays PLAYER_SE_EMY/GROUP_BOOT/SE_EMY_DOWN_HPDP_S or _H
void daMummyMe_c::playEnemyDownSound1() { }
void daMummyMe_c::playEnemyDownComboSound(void *player) { } // AcPy_c/daPlBase_c?
void daMummyMe_c::playHpdpSound2() { } // plays PLAYER_SE_EMY/GROUP_BOOT/SE_EMY_DOWN_HPDP_S or _H
void daMummyMe_c::_vf260(void *other) { } // AcPy/PlBase? plays the SE_EMY_FUMU_%d sounds based on some value
void daMummyMe_c::_vf264(dStageActor_c *other) { } // if other is player or yoshi, do Wm_en_hit and a few other things
void daMummyMe_c::_vf268(void *other) { } // AcPy/PlBase? plays the SE_EMY_DOWN_SPIN_%d sounds based on some value

void daMummyMe_c::powBlockActivated(bool isNotMPGP) {
}

bool daMummyMe_c::calculateTileCollisions() {
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

		// u32 blah = collMgr.s_80070760();
		// u8 one = (blah & 0xFF);
		// static const float incs[5] = {0.00390625f, 0.0078125f, 0.015625f, 0.0234375f, 0.03125f};
		// x_speed_inc = incs[one];
		max_speed.x = (direction == 1) ? -this->XSpeed : this->XSpeed;
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

void daMummyMe_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate) {
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr(name);
	this->chrAnimation.bind(&this->bodyModel, anmChr, unk);
	this->bodyModel.bindAnim(&this->chrAnimation, unk2);
	this->chrAnimation.setUpdateRate(rate);
}

int daMummyMe_c::onCreate() {
	OSReport("Spawning Mummy Me\n");

	allocator.link(-1, GameHeaps[0], 0, 0x20);

	this->resFile.data = getResource("mummyMe", "g3d/mummyMe.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("MummyMe");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);

	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr("walk");
	this->chrAnimation.setup(mdl, anmChr, &this->allocator, 0);


	allocator.unlink();

	this->pos.y += 32.0;
	this->rot.x = 0; // X is vertical axis
	this->rot.y = 0xD800; // Y is horizontal axis
	this->rot.z = 0; // Z is ... an axis >.>
	this->direction = 1; // Heading left.
	
	this->speed.x = 0.0;
	this->speed.y = 0.0;
	this->Baseline = this->pos.y;

	ActivePhysics::Info HitMeBaby;
	float anmSpeed;

	this->scale = (Vec){1.0f, 1.0f, 1.0f};

	HitMeBaby.xDistToCenter = 0.0;
	HitMeBaby.yDistToCenter = 16.0;

	HitMeBaby.xDistToEdge = 10.0;
	HitMeBaby.yDistToEdge = 16.0;

	this->XSpeed = 0.4;
	anmSpeed = 1.25;

	static const lineSensor_s below(-5<<12, 5<<12, 0<<12);
	static const lineSensor_s adjacent(10<<12, 9<<12, 10<<12);
	collMgr.init(this, &below, 0, &adjacent);

	HitMeBaby.category1 = 0x3;
	HitMeBaby.category2 = 0x0;
	HitMeBaby.bitfield1 = 0x1;
	HitMeBaby.bitfield2 = 0x820A4;
	HitMeBaby.unkShort1C = 0x0;
	HitMeBaby.callback = &shyCollisionCallback;

	this->aPhysics.initWithStruct(this, &HitMeBaby);
	this->aPhysics.addToList();

	collMgr.calculateBelowCollisionWithSmokeEffect();

	cmgr_returnValue = collMgr.isOnTopOfTile();

	if (collMgr.isOnTopOfTile())
		isBouncing = false;
	else
		isBouncing = true;

	bindAnimChr_and_setUpdateRate("walk", 1, 0.0, anmSpeed); 
	doStateChange(&StateID_RealWalk);

	this->onExecute();
	return true;
}

int daMummyMe_c::onDelete() {
	return true;
}

int daMummyMe_c::onExecute() {
	acState.execute();
	updateModelMatrices();

	return true;
}

int daMummyMe_c::onDraw() {
	bodyModel.scheduleForDrawing();
	return true;
}

void daMummyMe_c::updateModelMatrices() {
	float yoff;

	yoff = 1.25;//-5.0;
	
	matrix.translation(pos.x, pos.y + yoff, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}


///////////////
// Real Walk State
///////////////
void daMummyMe_c::beginState_RealWalk() {
	//inline this piece of code
	this->max_speed.x = (this->direction) ? -this->XSpeed : this->XSpeed;
	this->speed.x = (direction) ? -this->XSpeed : this->XSpeed;

	this->max_speed.y = -4.0;
	this->speed.y = -4.0;
	this->y_speed_inc = -0.1875;
}
void daMummyMe_c::executeState_RealWalk() { 
	bodyModel._vf1C();

	bool ret = calculateTileCollisions();
	if (ret) {
		doStateChange(&StateID_RealTurn);
	}

	if(this->chrAnimation.isAnimationDone()) {
		this->chrAnimation.setCurrentFrame(0.0);
	}
}
void daMummyMe_c::endState_RealWalk() { }

///////////////
// Real Turn State
///////////////
void daMummyMe_c::beginState_RealTurn() {

	this->direction ^= 1;
	this->speed.x = 0.0;
}
void daMummyMe_c::executeState_RealTurn() { 
	bodyModel._vf1C();

	if(this->chrAnimation.isAnimationDone()) {
		this->chrAnimation.setCurrentFrame(0.0);
	}

	u16 amt = (this->direction == 0) ? 0x2800 : 0xD800;
	int done = SmoothRotation(&this->rot.y, amt, 0x800);

	if(done) {
		this->doStateChange(&StateID_RealWalk);
	}
}
void daMummyMe_c::endState_RealTurn() {
}

///////////////
// Die State
///////////////
void daMummyMe_c::beginState_Die() {
	// dEn_c::dieFall_Begin();
	this->removeMyActivePhysics();

	bindAnimChr_and_setUpdateRate("walk", 1, 0.0, 2.0); 
	this->timer = 0;
	this->dying = -10.0;
	this->Baseline = this->pos.y;
	this->rot.y = 0;
	this->rot.x = 0;
}
void daMummyMe_c::executeState_Die() { 
	bodyModel._vf1C();

	if(this->chrAnimation.isAnimationDone()) {
		this->chrAnimation.setCurrentFrame(0.0);
	}

	this->timer += 1;
			
	// this->pos.x += 0.5; 
	this->pos.y = Baseline + (-0.2 * dying * dying) + 20.0;
	
	this->dying += 0.5;
		
	if (this->timer > 450) {
		this->kill();
		this->Delete(this->deleteForever);
	}

	// dEn_c::dieFall_Execute();

}
void daMummyMe_c::endState_Die() {
}

