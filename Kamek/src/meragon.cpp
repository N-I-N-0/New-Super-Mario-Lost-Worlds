#include <common.h>
#include <game.h>
#include <g3dhax.h>
#include <sfx.h>
#include "profile.h"

#define SFX_MERAGON_FIRE_START 2000
#define SFX_MERAGON_FIRE 2001
#define SFX_MERAGON_WING 2002

const char* MeragonArcNameList [] = {
	"meragon",
	NULL
};

class daMeragon_c : public dEn_c {
	int onCreate();
	int onDelete();
	int onExecute();
	int onDraw();

	mHeapAllocator_c allocator;
	nw4r::g3d::ResFile resFile;

	m3d::mdl_c model;

	m3d::anmChr_c animationChr;

	mEf::es2 effect;

    float XSpeed, YSpeed;
    float turnRatio;
    u8 radius;

	u16 shootDelay;
	u8 shootCount;

    Vec2 center;

    int timer;

	int sfxTimer;
	nw4r::snd::SoundHandle handle; // Sound Handle

	void bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate);
	void updateModelMatrices();

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

    bool isOutsideOfEllipse(Vec2 center, Vec2 radius, Vec2 pos);

	void executeWaitPlaysound();

	USING_STATES(daMeragon_c);
	DECLARE_STATE(Wait);
    DECLARE_STATE(Follow);
    DECLARE_STATE(Turn);
	DECLARE_STATE(Shoot);
	DECLARE_STATE(AttackHit);
	DECLARE_STATE(DieStomp);

	public:
		dAcPy_c* targetPlayer;

		u8 fireballCount;
		dStageActor_c* fireballs[8];
		ActivePhysics::Callback hitCallbacks[8];

		dStateBase_c *getAttackHitState();

		static dActor_c *build();
		static void hitCallbackHandler(ActivePhysics *apThis, ActivePhysics *apOther);
};

dActor_c *daMeragon_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daMeragon_c));
	return new(buffer) daMeragon_c;
}

const SpriteData MeragonSpriteData = { ProfileId::Meragon, 8, -8, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile MeragonProfile(&daMeragon_c::build, SpriteId::Meragon, &MeragonSpriteData, ProfileId::Meragon, ProfileId::Meragon, "Meragon", MeragonArcNameList, 0);

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


CREATE_STATE(daMeragon_c, Wait);
CREATE_STATE(daMeragon_c, Follow);
CREATE_STATE(daMeragon_c, Turn);
CREATE_STATE(daMeragon_c, Shoot);
CREATE_STATE(daMeragon_c, AttackHit);
CREATE_STATE(daMeragon_c, DieStomp);

////////////////////////
// Collision Functions
////////////////////////
void daMeragon_c::playerCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
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

void daMeragon_c::yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
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
bool daMeragon_c::collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther) {
	PlaySound(this, SE_EMY_DOWN);
	SpawnEffect("Wm_mr_hardhit", 0, &pos, &(S16Vec){0, 0, 0}, &(Vec){1.0, 1.0, 1.0});
	dEn_c::_vf148();
	return true;
}
bool daMeragon_c::collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->collisionCatD_Drill(apThis, apOther);
}
bool daMeragon_c::collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->collisionCatD_Drill(apThis, apOther);
}
bool daMeragon_c::collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->collisionCatD_Drill(apThis, apOther);
}
bool daMeragon_c::collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->collisionCatD_Drill(apThis, apOther);
}
bool daMeragon_c::collisionCat5_Mario(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->collisionCatD_Drill(apThis, apOther);
}
bool daMeragon_c::collisionCat11_PipeCannon(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->collisionCatD_Drill(apThis, apOther);
}
bool daMeragon_c::collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther) {
	StageE4::instance->spawnCoinJump(pos, 0, 2, 0);
	return this->collisionCatD_Drill(apThis, apOther);
}

bool daMeragon_c::collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther) {
	bool wut = dEn_c::collisionCat3_StarPower(apThis, apOther);
	return wut;
}

bool daMeragon_c::collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther) {
	return this->collisionCat1_Fireball_E_Explosion(apThis, apOther);
}

bool daMeragon_c::collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther) {
	StageE4::instance->spawnCoinJump(pos, 0, 1, 0);
	return this->collisionCatD_Drill(apThis, apOther);
}

// void daMeragon_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther) {
	
// 	dEn_C::collisionCat2_IceBall_15_YoshiIce(apThis, apOther);
// }

// These handle the ice crap
void daMeragon_c::_vf148() {
	dEn_c::_vf148();
}
void daMeragon_c::_vf14C() {
	dEn_c::_vf14C();
}

extern "C" void sub_80024C20(void);
extern "C" void __destroy_arr(void*, void(*)(void), int, int);

bool daMeragon_c::CreateIceActors() {
	struct DoSomethingCool my_struct = { 0, this->pos, {1.25, 1.25, 1.25}, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
	this->frzMgr.Create_ICEACTORs( (void*)&my_struct, 1 );
	__destroy_arr( (void*)&my_struct, sub_80024C20, 0x3C, 1 );
	this->animationChr.setUpdateRate(0.0f);
	return true;
}

void daMeragon_c::hitCallbackHandler(ActivePhysics *apThis, ActivePhysics *apOther) {
	daMeragon_c* meragon = (daMeragon_c*)apThis->owner->GetParent();
	dStageActor_c* other = apOther->owner;

	int index = -1;
	for (int i = 0; i < meragon->fireballCount; i++) {
		if (meragon->fireballs[i] == 0) continue;

		if (meragon->fireballs[i] == apThis->owner) {
			index = i;
			break;
		}
	}

	if (index == -1) return;

	meragon->hitCallbacks[index](apThis, apOther);

	if (other->stageActorType == other->PlayerType || other->stageActorType == other->YoshiType) {
		meragon->fireballs[index]->aPhysics.info.callback = meragon->hitCallbacks[index];
		meragon->fireballs[index] = 0;
		meragon->hitCallbacks[index] = 0;

		if (meragon->acState.getCurrentState() != meragon->getAttackHitState())
			meragon->doStateChange(meragon->getAttackHitState());
	}
}

dStateBase_c *daMeragon_c::getAttackHitState() {
	return &StateID_AttackHit;
}

void daMeragon_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate) {
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr(name);
	this->animationChr.bind(&this->model, anmChr, unk);
	this->model.bindAnim(&this->animationChr, unk2);
	this->animationChr.setUpdateRate(rate);
}

int daMeragon_c::onCreate() {
	int color = this->settings >> 24 & 0xF;

    this->radius = 64;
    this->turnRatio = 0.25;
    this->center = (Vec2){this->pos.x, this->pos.y};
    this->shootDelay = 132;
	this->shootCount = ((this->settings >> 20 & 0xF) % 8) + 1;
	this->sfxTimer = 0;
	this->fireballCount = 0;

	allocator.link(-1, GameHeaps[0], 0, 0x20);

	char resName[80];
	sprintf(resName, "g3d/t%02d.brres", color);
	this->resFile.data = getResource("meragon", resName);

	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("meragon");
	model.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Enemy(&model, 0);


	// Animations start here
	bool ret;
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr("fly_wait");
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
	this->max_speed.x = 0.0;
	this->x_speed_inc = 0.0;


	ActivePhysics::Info HitMeBaby;

	HitMeBaby.xDistToCenter = 0.0;
	HitMeBaby.yDistToCenter = 1.0;
	HitMeBaby.xDistToEdge = 12.0;
	HitMeBaby.yDistToEdge = 11.0;

	HitMeBaby.category1 = 0x3;
	HitMeBaby.category2 = 0x0;
	HitMeBaby.bitfield1 = 0x6F;
	HitMeBaby.bitfield2 = 0xffbafffe;
	HitMeBaby.unkShort1C = 0;
	HitMeBaby.callback = &dEn_c::collisionCallback;

	this->aPhysics.initWithStruct(this, &HitMeBaby);
	this->aPhysics.addToList();

    this->XSpeed = MakeRandomNumber(32) / 100.0;
    this->YSpeed = MakeRandomNumber(18) / 100.0;

	bindAnimChr_and_setUpdateRate("fly_wait", 1, 0.0, 0.5);
	doStateChange(&StateID_Wait);

	this->onExecute();
	return true;
}

int daMeragon_c::onDelete() {
	return true;
}

int daMeragon_c::onExecute() {
	acState.execute();
	updateModelMatrices();
	model._vf1C();

	return true;
}

int daMeragon_c::onDraw() {
	model.scheduleForDrawing();
	return true;
}

void daMeragon_c::updateModelMatrices() {
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


bool daMeragon_c::isOutsideOfEllipse(Vec2 center, Vec2 radius, Vec2 pos) {
    float x = pos.x - center.x;
    float y = pos.y - center.y;
    float a = radius.x;
    float b = radius.y;

    return (x * x) / (a * a) + (y * y) / (b * b) > 1;
}


void daMeragon_c::executeWaitPlaysound() {
	this->sfxTimer++;

	switch (this->sfxTimer) {
		case 2:
			playSoundDistance(this->handle, this->pos, SFX_MERAGON_WING, 0.25);
			break;
	}

	if (this->sfxTimer > 6) this->sfxTimer = 0;
}


///////////////
// Wait State
///////////////
void daMeragon_c::beginState_Wait() {
    this->timer = 0;
    this->targetPlayer = 0;
}
void daMeragon_c::executeState_Wait() {
	if (this->animationChr.isAnimationDone()) {
		this->animationChr.setCurrentFrame(0.0);
	}

	this->executeWaitPlaysound();

    if (this->isOutsideOfEllipse(this->center, (Vec2){this->radius * 2.0, this->radius}, (Vec2){this->pos.x, this->pos.y})) {
        this->XSpeed = (this->center.x - this->pos.x) > 0 ? 0.25 : -0.25;
        this->YSpeed = (this->center.y - this->pos.y) > 0 ? 0.125 : -0.125;
    }

    timer++;
    if (timer >= 60) {
        timer = -MakeRandomNumber(120);
        if ((MakeRandomNumber(100) / 100) < this->turnRatio) {
            this->XSpeed = (MakeRandomNumber(64) - 32) / 100.0;
            this->YSpeed = (MakeRandomNumber(36) - 18) / 100.0;
        }

        if (this->isOutsideOfEllipse(this->center, (Vec2){this->radius, this->radius / 2.0}, (Vec2){this->pos.x, this->pos.y})) {
            if (MakeRandomNumber(100) < 50) this->XSpeed = (this->center.x - this->pos.x) > 0 ? 0.25 : -0.25;
            if (MakeRandomNumber(100) < 50) this->YSpeed = (this->center.y - this->pos.y) > 0 ? 0.125 : -0.125;
        }
    }

    this->pos.x += this->XSpeed;
    this->pos.y += this->YSpeed;

    if (this->XSpeed > 0 && this->direction == 1) this->doStateChange(&StateID_Turn);
    if (this->XSpeed < 0 && this->direction == 0) this->doStateChange(&StateID_Turn);

    Vec2 pos = (Vec2){this->pos.x, this->pos.y};
    Vec2 dist = (Vec2){this->radius, this->radius};
    dAcPy_c* p = this->doSearchNearPlayer(&dist, &pos);
    if (p != 0) {
        if (this->isOutsideOfEllipse((Vec2){this->pos.x, this->pos.y}, (Vec2){this->radius * 3.0, this->radius * 3.0}, (Vec2){p->pos.x, p->pos.y})) return;

        this->targetPlayer = p;
        this->doStateChange(&StateID_Follow);
    }
}
void daMeragon_c::endState_Wait() { }

///////////////
// Follow State
///////////////
void daMeragon_c::beginState_Follow() {
	this->timer = 0;
}
void daMeragon_c::executeState_Follow() {
    if (this->animationChr.isAnimationDone()) {
        this->animationChr.setCurrentFrame(0.0);
    }

	this->executeWaitPlaysound();

    if (this->targetPlayer == 0) return doStateChange(&StateID_Wait);

    if (this->isOutsideOfEllipse((Vec2){this->pos.x, this->pos.y}, (Vec2){this->radius * 3.0, this->radius * 3.0}, (Vec2){this->targetPlayer->pos.x, this->targetPlayer->pos.y})) return doStateChange(&StateID_Wait);

    if (this->targetPlayer->pos.x > this->pos.x && this->direction == 1) return this->doStateChange(&StateID_Turn);
    if (this->targetPlayer->pos.x < this->pos.x && this->direction == 0) return this->doStateChange(&StateID_Turn);


    int distance = sqrt(pow(this->pos.x - this->targetPlayer->pos.x, 2) + pow(this->pos.y - this->targetPlayer->pos.y, 2));
    float speed = distance / 500.0;
    float escapeSpeed = speed * 2;
    float above = radius * 0.75;

	timer++;
	if (timer >= this->shootDelay + (int)(distance / 10)) return this->doStateChange(&StateID_Shoot);

    if (distance > radius * 1.5) {
        if (this->pos.x < (this->targetPlayer->pos.x - 8)) this->pos.x += speed;
        else if (this->pos.x > (this->targetPlayer->pos.x + 8)) this->pos.x -= speed;

        if (this->pos.y < (this->targetPlayer->pos.y + above - 8)) this->pos.y += speed;
        else if (this->pos.y > (this->targetPlayer->pos.y + above + 8)) this->pos.y -= speed;
    }
    else if (distance < radius * 1.25) {
        if (this->pos.x < (this->targetPlayer->pos.x - 8)) this->pos.x -= escapeSpeed;
        else if (this->pos.x > (this->targetPlayer->pos.x + 8)) this->pos.x += escapeSpeed;

        if (this->pos.y < (this->targetPlayer->pos.y + above - 8)) this->pos.y += escapeSpeed;
        else if (this->pos.y > (this->targetPlayer->pos.y + above + 8)) this->pos.y -= escapeSpeed;
    }
}
void daMeragon_c::endState_Follow() { }

///////////////
// Turn State
///////////////
void daMeragon_c::beginState_Turn() {
	this->direction ^= 1;
}
void daMeragon_c::executeState_Turn() {

	if (this->animationChr.isAnimationDone()) {
		this->animationChr.setCurrentFrame(0.0);
	}

	this->executeWaitPlaysound();

    this->pos.x += this->XSpeed;
    this->pos.y += this->YSpeed;

	u16 amt = (this->direction == 0) ? 0x2800 : 0xD800;
	int done = SmoothRotation(&this->rot.y, amt, 0x800);

	if (done) {
        if (this->targetPlayer != 0) this->doStateChange(&StateID_Follow);
        else this->doStateChange(&StateID_Wait);
	}
}
void daMeragon_c::endState_Turn() { }

///////////////
// Shoot State
///////////////
void daMeragon_c::beginState_Shoot() {
    bindAnimChr_and_setUpdateRate("fire_start", 1, 0.0, 0.5);
	playSoundDistance(this->handle, this->pos, SFX_MERAGON_FIRE_START);
	this->timer = -1;
	this->sfxTimer = 0;
	for (int i = 0; i < this->fireballCount; i++) {
		if (this->fireballs[i] != 0) this->fireballs[i]->aPhysics.info.callback = this->hitCallbacks[i];

		this->fireballs[i] = 0;
		this->hitCallbacks[i] = 0;
	}
	this->fireballCount = 0;
}
void daMeragon_c::executeState_Shoot() {
    if (this->animationChr.isAnimationDone()) {
		this->timer++;
		if (this->timer >= this->shootCount) return this->doStateChange(&StateID_Follow);

		this->executeWaitPlaysound();

		bindAnimChr_and_setUpdateRate(this->timer == 0 ? "fire_end" : "fire", 1, 0.0, 1.0);
		playSoundDistance(this->handle, this->pos, SFX_MERAGON_FIRE);
        this->animationChr.setCurrentFrame(0.0);

		this->fireballs[this->fireballCount] = createChild(PAKKUN_FIREBALL, this, 0x0, &this->pos, 0, this->currentLayerID);

		Vec2 speedDir = (Vec2){
			this->targetPlayer->pos.x - this->pos.x,
			this->targetPlayer->pos.y - this->pos.y + 12
		};
		float speed = 1.5;

		float magnitude = sqrt(pow(speedDir.x, 2) + pow(speedDir.y, 2));
		speedDir.x /= magnitude;
		speedDir.y /= magnitude;

		this->fireballs[this->fireballCount]->speed.x = speedDir.x * speed;
		this->fireballs[this->fireballCount]->speed.y = speedDir.y * speed;
		this->fireballs[this->fireballCount]->x_speed_inc = 0.0f;
		this->fireballs[this->fireballCount]->y_speed_inc = 0.0f;

		this->hitCallbacks[this->fireballCount] = this->fireballs[this->fireballCount]->aPhysics.info.callback;
		this->fireballs[this->fireballCount]->aPhysics.info.callback = &daMeragon_c::hitCallbackHandler;

		this->fireballCount++;
    }
}
void daMeragon_c::endState_Shoot() {
	bindAnimChr_and_setUpdateRate("fly_wait", 1, 0.0, 0.5);
	this->sfxTimer = 0;
}

///////////////
// AttackHit State
///////////////
void daMeragon_c::beginState_AttackHit() {
	bindAnimChr_and_setUpdateRate("attack_hit", 1, 0.0, 1.0);
}
void daMeragon_c::executeState_AttackHit() {
	this->executeWaitPlaysound();
	if (this->animationChr.isAnimationDone()) return doStateChange(&StateID_Follow);
}
void daMeragon_c::endState_AttackHit() {
	bindAnimChr_and_setUpdateRate("fly_wait", 1, 0.0, 0.5);
	this->sfxTimer = 0;
}

///////////////
// Die by Stomp State
///////////////
void daMeragon_c::beginState_DieStomp() {
	this->removeMyActivePhysics();

	bindAnimChr_and_setUpdateRate("damage", 1, 0.0, 1.0);
	this->rot.y = 0;
	this->rot.x = 0;
}
void daMeragon_c::executeState_DieStomp() {
	if (this->animationChr.isAnimationDone()) {
		for (int i = 0; i < this->fireballCount; i++) {
			if (this->fireballs[i] != 0) this->fireballs[i]->aPhysics.info.callback = this->hitCallbacks[i];

			this->fireballs[i] = 0;
			this->hitCallbacks[i] = 0;
		}
		this->fireballCount = 0;

		this->Delete(true);
	}
}
void daMeragon_c::endState_DieStomp() { }
