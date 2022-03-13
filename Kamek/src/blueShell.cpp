#include <game.h>
#include "daEnShell_c.h"
#include <common.h>
#include <g3dhax.h>
#include <sfx.h>
#include <profile.h>

class daNakedNokonoko_c : public dEn_c {
public:
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
	
	int appearTimer;
	
	bool foundMyShell;
	u32 shellId;
	daEnShell_c* shell;

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

	USING_STATES(daNakedNokonoko_c);
	DECLARE_STATE(Appear);
	DECLARE_STATE(Appear2);
	DECLARE_STATE(Appear3);
	DECLARE_STATE(Appear4);
	DECLARE_STATE(Search);
	DECLARE_STATE(RealWalk);
	DECLARE_STATE(RealTurn);
	DECLARE_STATE(Die);
};


/*
- make sure no walking sounds can be heard
- only take damage when shell is destroyed
- add check that the shell pointers are valid
- fix rotate on wall collision
- prevent players from making damage to controlled shells?


*/


#define MIN_SHELL_SPEED 2.0f
#define MAX_SHELL_SPEED 4.5f
#define CHG_SHELL_SPEED 0.025f

CREATE_STATE(daPlBase_c, ShellConnect);

u32 blueShells[4];	//blue shell pointers
int blueShellTimers[4];		//blue shell timers

void daPlBase_c::beginState_ShellConnect() {
	this->setFlag(0xBB);	//makes invisible
	//this->setFlag(0x25);	//propeller indicator?
	//this->aPhysics.removeFromList();
	//ActivePhysics* ap1 = (ActivePhysics*)((int)this+4452);
	//ActivePhysics* ap1 = (ActivePhysics*)((int)this+4452);
	//this->setFlag(0x71);
	this->useDemoControl();
	blueShellTimers[this->settings % 4] = 0;
	daEnShell_c* shell = (daEnShell_c*)fBase_c::search(blueShells[this->settings % 4]);
	shell->speed.x = MIN_SHELL_SPEED + abs(this->speed.x / 2);
	if(shell->speed.x > MAX_SHELL_SPEED) {
		shell->speed.x = MAX_SHELL_SPEED;
	}
	if(this->rot.y < 0) {
		shell->speed.x *= -1;
	}
	shell->doStateChange(&daEnShell_c::StateID_Slide);
}

void daPlBase_c::executeState_ShellConnect(){
	daEnShell_c* shell = (daEnShell_c*)fBase_c::search(blueShells[this->settings % 4]);
	if (shell == 0) {
		((dAcPy_c*)this)->hurtPlayer_maybe(); //not sure whether this works - ghidra says this should get 2 more arguments
		this->states2.setState(&daPlBase_c::StateID_Jump);
		return;
	}
	Vec bindPos = shell->pos;
	bindPos.y -= 4;
	this->pos = bindPos;
	
	if(blueShellTimers[this->settings % 4] < 90) {
		blueShellTimers[this->settings % 4]++;
	} else {
		Remocon* con = GetRemoconMng()->controllers[this->settings % 4];
		if(con->isShaking != 0) {
			this->states2.setState(&daPlBase_c::StateID_Jump);
			shell->Delete(1);
		}
		
		if(con->nowPressed & WPAD_TWO && shell->collMgr.isOnTopOfTile()) { //add is on ground check here
			shell->speed.y = 6.0f;
		}
		
		if(con->heldButtons & WPAD_LEFT) {
			if(shell->speed.x > 0) {
				if(shell->speed.x > MIN_SHELL_SPEED) {
					shell->speed.x -= CHG_SHELL_SPEED;
				}
			} else {
				if(shell->speed.x > -MAX_SHELL_SPEED) {
					shell->speed.x -= CHG_SHELL_SPEED;
				}
			}
		}

		if(con->heldButtons & WPAD_RIGHT) {
			if(shell->speed.x > 0) {
				if(shell->speed.x < MAX_SHELL_SPEED) {
					shell->speed.x += CHG_SHELL_SPEED;
				}
			} else {
				if(shell->speed.x < -MIN_SHELL_SPEED) {
					shell->speed.x += CHG_SHELL_SPEED;
				}				
			}
		}

	}
}

void daPlBase_c::endState_ShellConnect() {
	this->clearFlag(0xBB);
	//this->clearFlag(0x71);
	this->disableDemoControl();
}



extern "C" dStageActor_c *Actor_SearchByID(u32 actorID);
void setPlayerStateBlueShell(dAcPy_c* player) {			//setPlayerStateBlueShell__FP7dAcPy_c
	dStageActor_c* carrying = Actor_SearchByID(*(u32*)((u32)(player)+0x2A78)); //Actor_SearchByID(player->idWeAreCarrying)
	if(!carrying) return;
	//OSReport("ID: %d, Pointer: %p, Name: %d\n", player->idWeAreCarrying, carrying, carrying->name);
	if (carrying->name == 54 || carrying->name == 55) {	//if Koopa or Parakoopa Shell
		blueShells[player->settings % 4] = carrying->id;
		player->states2.setState(&daPlBase_c::StateID_ShellConnect);
		Vec spawnPos = player->pos;
		spawnPos.x += 10;
		daNakedNokonoko_c* nokonokoN = (daNakedNokonoko_c*)CreateActor(NakedNokonoko, 0, spawnPos, 0, 0);
		nokonokoN->speed.x = player->speed.x*2;
		nokonokoN->direction = player->direction;
		nokonokoN->shellId = carrying->id;
		if(player->direction == 0) {
			if(nokonokoN->speed.x < 2) {
				nokonokoN->speed.x = 2;
			}
		} else {
			if(nokonokoN->speed.x > -2) {
				nokonokoN->speed.x = -2;
			}
		}
		nokonokoN->speed.y = 3;
		
		dPlayerModelHandler_c *pmh = (dPlayerModelHandler_c*)(((u32)player) + 0x2A60);
        pmh->mdlClass->startAnimation(hipsr, 1.0f, 0.0f, 0.0f);
	}
}
















const char* NakedNokonokoFileList [] = {
	"nokonokoA",
	NULL	
};

const SpriteData NakedNokonokoSpriteData = {ProfileId::NakedNokonoko, 0x5, -0x31, 0, 0x10, 0x10, 0x40, 0x40, 0x40, 0, 0, 0};
Profile NakedNokonokoProfile(&daNakedNokonoko_c::build, SpriteId::NakedNokonoko, &NakedNokonokoSpriteData, ProfileId::NakedNokonoko, ProfileId::NakedNokonoko, "Naked Nokonoko", NakedNokonokoFileList);

dActor_c *daNakedNokonoko_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daNakedNokonoko_c));
	return new(buffer) daNakedNokonoko_c;
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

CREATE_STATE(daNakedNokonoko_c, Appear);
CREATE_STATE(daNakedNokonoko_c, Appear2);
CREATE_STATE(daNakedNokonoko_c, Appear3);
CREATE_STATE(daNakedNokonoko_c, Appear4);
CREATE_STATE(daNakedNokonoko_c, Search);
CREATE_STATE(daNakedNokonoko_c, RealWalk);
CREATE_STATE(daNakedNokonoko_c, RealTurn);
CREATE_STATE(daNakedNokonoko_c, Die);

////////////////////////
// Collision Functions
////////////////////////

void daNakedNokonoko_c::playerCollision(ActivePhysics *apThis, ActivePhysics *apOther) { 
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
void daNakedNokonoko_c::_vf278(void *other) {
	PlaySound(this, SE_EMY_HANACHAN_STOMP);
}

void daNakedNokonoko_c::bouncePlayerWhenJumpedOn(void *player) {
	bouncePlayer(player, 5.0f);
}

void daNakedNokonoko_c::yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	this->playerCollision(apThis, apOther);
}
bool daNakedNokonoko_c::collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther) {
	return BigHanaWeirdGP(this, apThis, apOther);
}
bool daNakedNokonoko_c::collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther) {
	return BigHanaGroundPound(this, apThis, apOther);
}
bool daNakedNokonoko_c::collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther) {
	return BigHanaGroundPound(this, apThis, apOther);
}

bool daNakedNokonoko_c::collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther) {
	apOther->owner->kill();
	return true;
}
bool daNakedNokonoko_c::collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther){
	dAcPy_vf3F8(apOther->owner, this, 3);
	this->counter_504[apOther->owner->which_player] = 0xA;
	return true;
}
bool daNakedNokonoko_c::collisionCat5_Mario(ActivePhysics *apThis, ActivePhysics *apOther){
	return true;
}
bool daNakedNokonoko_c::collisionCat11_PipeCannon(ActivePhysics *apThis, ActivePhysics *apOther){
	return true;
}
bool daNakedNokonoko_c::collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther) {
	return true;
}
bool daNakedNokonoko_c::collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther){
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

bool daNakedNokonoko_c::collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther){
	return BigHanaFireball(this, apThis, apOther);
}
bool daNakedNokonoko_c::collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther) {
	return BigHanaFireball(this, apThis, apOther);
}
bool daNakedNokonoko_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther) {
	return BigHanaIceball(this, apThis, apOther);
}

// These handle the ice crap
void daNakedNokonoko_c::_vf148() {
	dEn_c::_vf148();
	doStateChange(&StateID_Die);
}
void daNakedNokonoko_c::_vf14C() {
	dEn_c::_vf14C();
	doStateChange(&StateID_Die);
}

extern "C" void sub_80024C20(void);
extern "C" void __destroy_arr(void*, void(*)(void), int, int);

bool daNakedNokonoko_c::CreateIceActors() {
	struct DoSomethingCool my_struct = { 0, this->pos, {2.4, 3.0, 3.0}, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
	this->frzMgr.Create_ICEACTORs( (void*)&my_struct, 1 );
	__destroy_arr( (void*)&my_struct, sub_80024C20, 0x3C, 1 );
	return true;
}

void daNakedNokonoko_c::addScoreWhenHit(void *other) { }

void daNakedNokonoko_c::spawnHitEffectAtPosition(Vec2 pos) { }
void daNakedNokonoko_c::doSomethingWithHardHitAndSoftHitEffects(Vec pos) { }
void daNakedNokonoko_c::playEnemyDownSound2() { }
void daNakedNokonoko_c::playHpdpSound1() { } // plays PLAYER_SE_EMY/GROUP_BOOT/SE_EMY_DOWN_HPDP_S or _H
void daNakedNokonoko_c::playEnemyDownSound1() { }
void daNakedNokonoko_c::playEnemyDownComboSound(void *player) { } // AcPy_c/daPlBase_c?
void daNakedNokonoko_c::playHpdpSound2() { } // plays PLAYER_SE_EMY/GROUP_BOOT/SE_EMY_DOWN_HPDP_S or _H
void daNakedNokonoko_c::_vf260(void *other) { } // AcPy/PlBase? plays the SE_EMY_FUMU_%d sounds based on some value
void daNakedNokonoko_c::_vf264(dStageActor_c *other) { } // if other is player or yoshi, do Wm_en_hit and a few other things
void daNakedNokonoko_c::_vf268(void *other) { } // AcPy/PlBase? plays the SE_EMY_DOWN_SPIN_%d sounds based on some value

void daNakedNokonoko_c::powBlockActivated(bool isNotMPGP) {
}

bool daNakedNokonoko_c::calculateTileCollisions() {
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

void daNakedNokonoko_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate) {
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr(name);
	this->chrAnimation.bind(&this->bodyModel, anmChr, unk);
	this->bodyModel.bindAnim(&this->chrAnimation, unk2);
	this->chrAnimation.setUpdateRate(rate);
}

int daNakedNokonoko_c::onCreate() {
	OSReport("Spawning Naked Koopa\n");

	allocator.link(-1, GameHeaps[0], 0, 0x20);

	this->resFile.data = getResource("nokonokoA", "g3d/naked.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("nokonokoN");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Enemy(&bodyModel, 0);

	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr("Walk");
	this->chrAnimation.setup(mdl, anmChr, &this->allocator, 0);


	allocator.unlink();

	this->pos.y += 32.0;
	this->rot.x = 0; // X is vertical axis
	this->rot.y = 0xD800; // Y is horizontal axis
	this->rot.z = 0; // Z is ... an axis >.>
	this->direction = 1; // Heading left.
	
	//this->speed.x = 0.0;
	//this->speed.y = 0.0;
	this->Baseline = this->pos.y;

	ActivePhysics::Info HitMeBaby;
	float anmSpeed;

	this->scale = (Vec){0.4f, 0.4f, 0.4f};

	HitMeBaby.xDistToCenter = 0.0;
	HitMeBaby.yDistToCenter = 16.0;

	HitMeBaby.xDistToEdge = 10.0;
	HitMeBaby.yDistToEdge = 16.0;

	this->XSpeed = 1;
	anmSpeed = 1.25;

	static const lineSensor_s below(-5<<12, 5<<12, 0<<12);
	static const lineSensor_s adjacent(10<<12, 9<<12, 10<<12);
	collMgr.init(this, &below, 0, &adjacent);

	HitMeBaby.category1 = 0x3;
	HitMeBaby.category2 = 0x0;
	HitMeBaby.bitfield1 = 0x1;
	HitMeBaby.bitfield2 = 0x820A4;
	HitMeBaby.unkShort1C = 0x0;
	HitMeBaby.callback = &dEn_c::collisionCallback;

	this->aPhysics.initWithStruct(this, &HitMeBaby);
	this->aPhysics.addToList();

	collMgr.calculateBelowCollisionWithSmokeEffect();

	cmgr_returnValue = collMgr.isOnTopOfTile();

	if (collMgr.isOnTopOfTile())
		isBouncing = false;
	else
		isBouncing = true;

	bindAnimChr_and_setUpdateRate("Walk", 1, 0.0, anmSpeed); 
	doStateChange(&StateID_Appear);

	this->onExecute();
	return true;
}

int daNakedNokonoko_c::onDelete() {
	return true;
}

int daNakedNokonoko_c::onExecute() {
	this->shell = (daEnShell_c*)fBase_c::search(this->shellId);
	acState.execute();
	updateModelMatrices();
	bodyModel._vf1C();


	return true;
}

int daNakedNokonoko_c::onDraw() {
	bodyModel.scheduleForDrawing();
	return true;
}

void daNakedNokonoko_c::updateModelMatrices() {
	float yoff;

	yoff = 0;//-5.0;
	
	matrix.translation(pos.x, pos.y + yoff, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}


///////////////
// Real Walk State
///////////////
void daNakedNokonoko_c::beginState_RealWalk() {
	//inline this piece of code
	this->max_speed.x = (this->direction) ? -this->XSpeed : this->XSpeed;
	this->speed.x = (direction) ? -this->XSpeed : this->XSpeed;

	this->max_speed.y = -4.0;
	this->speed.y = -4.0;
	this->y_speed_inc = -0.1875;
}
void daNakedNokonoko_c::executeState_RealWalk() { 
	bool ret = calculateTileCollisions();
	if (ret) {
		doStateChange(&StateID_RealTurn);
	}

	if(this->chrAnimation.isAnimationDone()) {
		this->chrAnimation.setCurrentFrame(0.0);
	}
}
void daNakedNokonoko_c::endState_RealWalk() { }

///////////////
// Real Turn State
///////////////
void daNakedNokonoko_c::beginState_RealTurn() {

	this->direction ^= 1;
	this->speed.x = 0.0;
}
void daNakedNokonoko_c::executeState_RealTurn() { 
	if(this->chrAnimation.isAnimationDone()) {
		this->chrAnimation.setCurrentFrame(0.0);
	}

	u16 amt = (this->direction == 0) ? 0x2800 : 0xD800;
	int done = SmoothRotation(&this->rot.y, amt, 0x800);

	if(done) {
		this->doStateChange(&StateID_RealWalk);
	}
}
void daNakedNokonoko_c::endState_RealTurn() {
}


///////////////
// Appear State
///////////////
void daNakedNokonoko_c::beginState_Appear() {
	bindAnimChr_and_setUpdateRate("Fall", 1, 0.0, 1.0);
	appearTimer = 0;
}
void daNakedNokonoko_c::executeState_Appear() {
	if(collMgr.isOnTopOfTile()) {
		if(appearTimer > 40) {
			doStateChange(&StateID_Appear2);
			return;
		}
	}
	
	/*HandleXSpeed();
	HandleYSpeed();
	doSpriteMovement();*/
	calculateTileCollisions();

	if(appearTimer <= 40) {
		float appearScale = 0.01f*appearTimer;
		scale = (Vec){appearScale, appearScale, appearScale};
		appearTimer++;
	}
	
	if(this->chrAnimation.isAnimationDone()) {
		this->chrAnimation.setCurrentFrame(0.0);
	}
}
void daNakedNokonoko_c::endState_Appear() {
}

///////////////
// Appear2 State
///////////////
void daNakedNokonoko_c::beginState_Appear2() {
	bindAnimChr_and_setUpdateRate("Land", 1, 0.0, 1.0);
}
void daNakedNokonoko_c::executeState_Appear2() {
	if(this->chrAnimation.isAnimationDone()) {
		doStateChange(&StateID_Appear3);
	}
}
void daNakedNokonoko_c::endState_Appear2() {
}

///////////////
// Appear3 State
///////////////
void daNakedNokonoko_c::beginState_Appear3() {
	bindAnimChr_and_setUpdateRate("Recover", 1, 0.0, 1.0);
}
void daNakedNokonoko_c::executeState_Appear3() {
	if(this->chrAnimation.isAnimationDone()) {
		doStateChange(&StateID_Appear4);
	}
}
void daNakedNokonoko_c::endState_Appear3() {
}

///////////////
// Appear4 State
///////////////
void daNakedNokonoko_c::beginState_Appear4() {
	bindAnimChr_and_setUpdateRate("FindShell", 1, 0.0, 1.0);
}
void daNakedNokonoko_c::executeState_Appear4() {
	if(this->chrAnimation.isAnimationDone()) {
		doStateChange(&StateID_Search);
	}
}
void daNakedNokonoko_c::endState_Appear4() {
	//bindAnimChr_and_setUpdateRate("Run", 1, 0.0, 1.0);
}

///////////////
// Search State
///////////////
void daNakedNokonoko_c::beginState_Search() {
	bindAnimChr_and_setUpdateRate("FindShell", 1, 0.0, 1.0);
}
void daNakedNokonoko_c::executeState_Search() {
	
	/*if (direction == 0) {
		if (
	}*/

}
void daNakedNokonoko_c::endState_Search() {
	//bindAnimChr_and_setUpdateRate("Run", 1, 0.0, 1.0);
}

///////////////
// Die State
///////////////
void daNakedNokonoko_c::beginState_Die() {
	// dEn_c::dieFall_Begin();
	this->removeMyActivePhysics();

	bindAnimChr_and_setUpdateRate("Walk", 1, 0.0, 2.0); 
	this->timer = 0;
	this->dying = -10.0;
	this->Baseline = this->pos.y;
	this->rot.y = 0;
	this->rot.x = 0;
}
void daNakedNokonoko_c::executeState_Die() { 
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
void daNakedNokonoko_c::endState_Die() {
}








































