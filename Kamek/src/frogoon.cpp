#include <common.h>
#include <game.h>
#include <g3dhax.h>
#include <sfx.h>
#include <profile.h>

extern void shyCollisionCallback(ActivePhysics *apThis, ActivePhysics *apOther);
const char* FrogoonFileList [] = {
	"frogoon",
	NULL	
};




class daFrogoon_c : public dEn_c {
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

	u8 maxWaitTimer;

	public: static dActor_c *build();

	void bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate);
	void updateModelMatrices();
	bool calculateTileCollisions();

	void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther);
	void yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther);
	void spriteCollision(ActivePhysics *apThis, ActivePhysics *apOther);

	/*bool collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther);
	//bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther);*/
	

	void _vf148();
	void _vf14C();
	bool CreateIceActors();


	USING_STATES(daFrogoon_c);
	DECLARE_STATE(RealWalk);
	DECLARE_STATE(RealTurn);
	DECLARE_STATE(Die);
	DECLARE_STATE(JumpDelay);
	DECLARE_STATE(JumpEnd);
};

const SpriteData FrogoonSpriteData = {ProfileId::Frogoon, 0x5, -0x31, 0, 0x10, 0x10, 0x40, 0x40, 0x40, 0, 0, 0};
Profile FrogoonProfile(&daFrogoon_c::build, SpriteId::Frogoon, &FrogoonSpriteData, ProfileId::Frogoon, ProfileId::Frogoon, "Frogoon", FrogoonFileList);

dActor_c *daFrogoon_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daFrogoon_c));
	return new(buffer) daFrogoon_c;
}

///////////////////////
// Externs and States
///////////////////////
extern "C" bool SpawnEffect(const char*, int, Vec*, S16Vec*, Vec*);

//FIXME make this dEn_c->used...
extern "C" char usedForDeterminingStatePress_or_playerCollision(dEn_c* t, ActivePhysics *apThis, ActivePhysics *apOther, int unk1);
extern "C" int SmoothRotation(short* rot, u16 amt, int unk2);



extern "C" void dAcPy_vf3F8(void* player, dEn_c* monster, int t);

CREATE_STATE(daFrogoon_c, RealWalk);
CREATE_STATE(daFrogoon_c, RealTurn);
CREATE_STATE(daFrogoon_c, Die);
CREATE_STATE(daFrogoon_c, JumpDelay);
CREATE_STATE(daFrogoon_c, JumpEnd);

////////////////////////
// Collision Functions
////////////////////////

void daFrogoon_c::playerCollision(ActivePhysics *apThis, ActivePhysics *apOther) { 
	char hitType;
	hitType = usedForDeterminingStatePress_or_playerCollision(this, apThis, apOther, 0);

	if(hitType == 1) {	// regular jump
		apOther->someFlagByte |= 2;
		doStateChange(&StateID_Die);
	}
	else if(hitType == 3) {	// spinning jump or whatever?
		apOther->someFlagByte |= 2;
		doStateChange(&StateID_Die);
	}
	else if(hitType == 0) {
		//EN_LandbarrelPlayerCollision(this, apThis, apOther);
		DamagePlayer(this, apThis, apOther);
	} else {
		DamagePlayer(this, apThis, apOther);
	}
}

void daFrogoon_c::yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	this->playerCollision(apThis, apOther);
}

void daFrogoon_c::spriteCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	u16 name = ((dEn_c*)apOther->owner)->name;

	if (name == EN_COIN || name == EN_EATCOIN || name == AC_BLOCK_COIN || name == EN_COIN_JUGEM || name == EN_COIN_ANGLE
		|| name == EN_COIN_JUMP || name == EN_COIN_FLOOR || name == EN_COIN_VOLT || name == EN_COIN_WIND 
		|| name == EN_BLUE_COIN || name == EN_COIN_WATER || name == EN_REDCOIN || name == EN_GREENCOIN
		|| name == EN_JUMPDAI || name == EN_ITEM) {
			return;
	}

	/*if (acState.getCurrentState() == &StateID_RealWalk) {
		if(collMgr.isOnTopOfTile()) {
			pos.x = ((pos.x - ((dEn_c*)apOther->owner)->pos.x) > 0) ? pos.x + 1.5 : pos.x - 1.5;
			doStateChange(&StateID_RealTurn);
		} else {
			if (name == Frogoon) {
				daFrogoon_c* other = (daFrogoon_c*)apOther->owner;
				other->doStateChange(&StateID_RealWalk);
			}
		}
	}*/
	
	if (acState.getCurrentState() == &StateID_RealWalk) {
		if(collMgr.isOnTopOfTile()) {
			float distance = ((pos.x - ((dEn_c*)apOther->owner)->pos.x));
			float moveSideways = (abs(distance) < 22) ? 22-abs(distance) : 1.5;
			pos.x = (distance > 0) ? pos.x + moveSideways : pos.x - moveSideways;
			doStateChange(&StateID_RealTurn);
		}
	}
}

/*bool daFrogoon_c::collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther) {
	dEn_c::collisionCatD_Drill(apThis, apOther);
	doStateChange(&StateID_Die);
	return true; //BigHanaWeirdGP(this, apThis, apOther);
}
bool daFrogoon_c::collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther) {
	dEn_c::collisionCat7_GroundPound(apThis, apOther);
	doStateChange(&StateID_Die);
	return true;//BigHanaGroundPound(this, apThis, apOther);
}
bool daFrogoon_c::collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther) {
	dEn_c::collisionCat7_GroundPoundYoshi(apThis, apOther);
	doStateChange(&StateID_Die);
	return true;//BigHanaGroundPound(this, apThis, apOther);
}

bool daFrogoon_c::collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther) {
	dEn_c::collisionCat9_RollingObject(apThis, apOther);
	doStateChange(&StateID_Die);
	return true;
}
bool daFrogoon_c::collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther){
	dEn_c::collisionCatA_PenguinMario(apThis, apOther);
	dAcPy_vf3F8(apOther->owner, this, 3);
	this->counter_504[apOther->owner->which_player] = 0xA;
	return true;
}
bool daFrogoon_c::collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther) {
	dEn_c::collisionCat13_Hammer(apThis, apOther);
	doStateChange(&StateID_Die);
	return true;
}
bool daFrogoon_c::collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther){
	dEn_c::collisionCat3_StarPower(apThis, apOther);
	doStateChange(&StateID_Die);
	return true;
}

bool daFrogoon_c::collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther){
	dEn_c::collisionCat14_YoshiFire(apThis, apOther);
	doStateChange(&StateID_Die);
	return true;
}
bool daFrogoon_c::collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther) {
	dEn_c::collisionCat1_Fireball_E_Explosion(apThis, apOther);
	doStateChange(&StateID_Die);
	return true;
}
/*bool daFrogoon_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther) {
	return BigHanaIceball(this, apThis, apOther);
}*/

// These handle the ice crap
void daFrogoon_c::_vf148() {
	dEn_c::_vf148();
	doStateChange(&StateID_Die);
}
void daFrogoon_c::_vf14C() {
	dEn_c::_vf14C();
	doStateChange(&StateID_Die);
}

extern "C" void sub_80024C20(void);
extern "C" void __destroy_arr(void*, void(*)(void), int, int);

bool daFrogoon_c::CreateIceActors() {
	struct DoSomethingCool my_struct = { 0, this->pos, {2.4, 3.0, 3.0}, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
	this->frzMgr.Create_ICEACTORs( (void*)&my_struct, 1 );
	__destroy_arr( (void*)&my_struct, sub_80024C20, 0x3C, 1 );
	return true;
}


bool daFrogoon_c::calculateTileCollisions() {
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

void daFrogoon_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate) {
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr(name);
	this->chrAnimation.bind(&this->bodyModel, anmChr, unk);
	this->bodyModel.bindAnim(&this->chrAnimation, unk2);
	this->chrAnimation.setUpdateRate(rate);
}

int daFrogoon_c::onCreate() {
	OSReport("Spawning Frogoon\n");

	allocator.link(-1, GameHeaps[0], 0, 0x20);

	this->resFile.data = getResource("frogoon", "g3d/frogoon.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("frogoon");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Enemy(&bodyModel, 0);

	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr("Wait");
	this->chrAnimation.setup(mdl, anmChr, &this->allocator, 0);


	allocator.unlink();

	this->pos.y += 32.0;
	this->rot.x = 0; // X is vertical axis
	this->rot.y = 0xC000; // Y is horizontal axis
	this->rot.z = 0; // Z is ... an axis >.>
	this->direction = 1; // Heading left.
	
	this->speed.x = 0.0;
	this->speed.y = 0.0;
	this->Baseline = this->pos.y;

	ActivePhysics::Info HitMeBaby;
	float anmSpeed;

	this->scale = (Vec){1.0f, 1.0f, 1.0f};

	HitMeBaby.xDistToCenter = 0.0;
	HitMeBaby.yDistToCenter = 7.0;

	HitMeBaby.xDistToEdge = 10.0;
	HitMeBaby.yDistToEdge = 7.0;

	this->XSpeed = 1.0;
	anmSpeed = 1.0;

	static const lineSensor_s below(-4<<12, 4<<12, 0<<12);
	static const lineSensor_s adjacent(10<<12, 9<<12, 10<<12);
	collMgr.init(this, &below, 0, &adjacent);

	HitMeBaby.category1 = 0x3;
	HitMeBaby.category2 = 0x0;
	HitMeBaby.bitfield1 = 0x6f;
	HitMeBaby.bitfield2 = 0xffbafffe;
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

	maxWaitTimer = 0;

	bindAnimChr_and_setUpdateRate("Wait", 1, 0.0, anmSpeed); 
	doStateChange(&StateID_RealWalk);

	this->onExecute();
	return true;
}

int daFrogoon_c::onDelete() {
	return true;
}

int daFrogoon_c::onExecute() {
	//OSReport("State: %s\n", acState.getCurrentState()->getName());

	acState.execute();
	updateModelMatrices();

	if(!collMgr.isOnTopOfTile()) {
		if (acState.getCurrentState() != &StateID_RealWalk) {
			doStateChange(&StateID_RealWalk);
		}
	}

	return true;
}

int daFrogoon_c::onDraw() {
	bodyModel.scheduleForDrawing();
	return true;
}

void daFrogoon_c::updateModelMatrices() {
	float yoff;

	yoff = 0;//1.15;//-5.0;
	
	u32 behaivour1 = collisionMgr_c::getTileBehaviour1At(pos.x, pos.y - 2, this->currentLayerID);
	u32 behaivour2 = collisionMgr_c::getTileBehaviour2At(pos.x, pos.y - 2, this->currentLayerID);
	
	if ((behaivour1 & 0xFF) == 0x20) {	//we are on a slope
		static s16 tileSlopeRotations[] = {0x2000, -0x2000, 0x1000, 0x1000, -0x1000, -0x1000, 0x3000, 0x3000, -0x3000, -0x3000, 0x4000, 0x500, 0x500, 0x500, 0x500, -0x500, -0x500, -0x500, -0x500};
		this->rot.x = tileSlopeRotations[behaivour2 & 0xFF];
		if (this->direction == 0) {
			rot.x *= -1;
		}
	} else {							//we are on ground (probably)
		if(collMgr.isOnTopOfTile()) {
			this->rot.x = 0;
		}
	}
	
	matrix.translation(pos.x, pos.y + yoff, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}


///////////////
// Real Walk State
///////////////
void daFrogoon_c::beginState_RealWalk() {
	//inline this piece of code
	this->max_speed.x = (this->direction) ? -this->XSpeed : this->XSpeed;
	this->speed.x = (direction) ? -this->XSpeed : this->XSpeed;

	this->max_speed.y = -4.0;
	this->speed.y = -4.0;
	this->y_speed_inc = -0.1875;

	bindAnimChr_and_setUpdateRate("Jump", 1, 0.0, 1.0); 
}
void daFrogoon_c::executeState_RealWalk() { 
	bodyModel._vf1C();

	bool ret = calculateTileCollisions();
	if (ret) {
		doStateChange(&StateID_RealTurn);
		return;
	}

	if(this->chrAnimation.isAnimationDone() && collMgr.isOnTopOfTile()) {
		doStateChange(&StateID_JumpEnd);
	}
}
void daFrogoon_c::endState_RealWalk() {}


///////////////
// Jump End State
///////////////
void daFrogoon_c::beginState_JumpEnd() {
	bindAnimChr_and_setUpdateRate("Land", 1, 0.0, 1.0);
	this->chrAnimation.setCurrentFrame(0.0);
}
void daFrogoon_c::executeState_JumpEnd() {
	bodyModel._vf1C();
	//calculateTileCollisions();
	
	if(this->chrAnimation.isAnimationDone()) {
		doStateChange(&StateID_JumpDelay);
	}
}
void daFrogoon_c::endState_JumpEnd() {}


///////////////
// Jump Delay State
///////////////
void daFrogoon_c::beginState_JumpDelay() {
	bindAnimChr_and_setUpdateRate("Wait", 1, 0.0, 1.0); 
	this->chrAnimation.setCurrentFrame(0.0);
	maxWaitTimer = 0;
}
void daFrogoon_c::executeState_JumpDelay() {
	bodyModel._vf1C();
	//calculateTileCollisions();
	if(this->chrAnimation.isAnimationDone()) {
		if(GenerateRandomNumber(5) == 0 || maxWaitTimer >= 3) {
			doStateChange(&StateID_RealWalk);
		} else {
			maxWaitTimer++;
			this->chrAnimation.setCurrentFrame(0.0);
		}
	}
}
void daFrogoon_c::endState_JumpDelay() {}




///////////////
// Real Turn State
///////////////
void daFrogoon_c::beginState_RealTurn() {

	this->direction ^= 1;
	this->speed.x = 0.0;
}
void daFrogoon_c::executeState_RealTurn() { 
	bodyModel._vf1C();

	if(this->chrAnimation.isAnimationDone()) {
		this->chrAnimation.setCurrentFrame(0.0);
	}

	u16 amt = (this->direction == 0) ? 0x4000 : 0xC000;
	int done = SmoothRotation(&this->rot.y, amt, 0x800);

	if(done) {
		this->doStateChange(&StateID_JumpDelay);
	}
}
void daFrogoon_c::endState_RealTurn() {}

///////////////
// Die State
///////////////
void daFrogoon_c::beginState_Die() {
	// dEn_c::dieFall_Begin();
	this->removeMyActivePhysics();

	bindAnimChr_and_setUpdateRate("Wait", 1, 0.0, 1.0); 
	this->timer = 0;
	this->dying = -10.0;
	this->Baseline = this->pos.y;
	this->rot.y = 0;
	this->rot.x = 0;
	
	nw4r::snd::SoundHandle handle;
	PlaySoundWithFunctionB4(SoundRelatedClass, &handle, SE_EMY_DOWN, 1);
	StageE4::instance->spawnCoinJump(pos, 0, 1, 0);
}
void daFrogoon_c::executeState_Die() { 
	bodyModel._vf1C();

	if(this->chrAnimation.isAnimationDone()) {
		this->chrAnimation.setCurrentFrame(0.0);
	}

	this->timer += 1;
			
	// this->pos.x += 0.5; 
	this->pos.y = Baseline + (-0.2 * dying * dying) + 20.0;
	
	this->scale.y = 1.0 - (timer / 450.0);
	
	this->dying += 0.5;
		
	if (this->timer > 450) {
		this->kill();
		this->Delete(this->deleteForever);
	}

	// dEn_c::dieFall_Execute();

}
void daFrogoon_c::endState_Die() {}

