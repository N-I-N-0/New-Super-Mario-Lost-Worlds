// original code from https://github.com/Zenith-Team/Tsuru/blob/main/source/actors/scuttlebug.cpp
// see scuttlebug-LICENSE for the LICENSE of that file
// heavily modified to work with NSMBW

class Scuttlebug : public dEn_c {
public:
	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

    void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther);
    void yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther);
    bool collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther);
    bool collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther);
    bool _vf120(ActivePhysics *apThis, ActivePhysics *apOther);
    bool collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther);
    bool collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther);
    bool collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther);
    bool collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther);
    bool collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther);
    //bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther);
    bool collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther);
    bool collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther);
    bool customCollision(ActivePhysics *apThis, ActivePhysics *apOther);

	//setDeathInfo_IceBreak -> what should be done when in an iceblock and crashes a wall -> normally the sprite should die
	void _vf148();
	//setDeathInfo_IceVanish -> what should be done when collision with an iceblock thrown by Mario -> normally the sprite should die
	void _vf14C();
	//create an ice block when collided with Iceball
	bool CreateIceActors();

	void bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate);

	void updateModelMatrices();

	mHeapAllocator_c allocator;
	nw4r::g3d::ResFile resFile;
	nw4r::g3d::ResFile resFileAnim;
	m3d::mdl_c bodyModel;
	m3d::mdl_c bodyModel2;

	nw4r::g3d::ResAnmTexPat anmPat;

	m3d::anmChr_c chrAnimation;
	m3d::anmTexPat_c patAnimation;

	Vec originalPos;

	u8 eyeTimer;
	bool spawnCoin;

//    ModelWrapper* model;
//    ModelWrapper* string;

	static dActor_c* build();

    u32 timerIdleLowered;
    u32 timerIdleRaised;
    u32 timerLowering;
    u32 timerRising;
    u16 moveDistance;
    u16 idleTime;


	USING_STATES(Scuttlebug);
    DECLARE_STATE(Lowering);
    DECLARE_STATE(IdleLowered);
    DECLARE_STATE(Rising);
    DECLARE_STATE(IdleRaised);
    DECLARE_STATE(Die);
};

CREATE_STATE(Scuttlebug, Lowering);
CREATE_STATE(Scuttlebug, IdleLowered);
CREATE_STATE(Scuttlebug, Rising);
CREATE_STATE(Scuttlebug, IdleRaised);
CREATE_STATE(Scuttlebug, Die);


dActor_c* Scuttlebug::build() {
	void* buffer = AllocFromGameHeap1(sizeof(Scuttlebug));
	Scuttlebug* c = new(buffer) Scuttlebug;

	return c;
}

const char* ScuttlebugFileList[] = { "gasagoso", NULL };
const SpriteData ScuttlebugSpriteData = { ProfileId::Scuttlebug, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile ScuttlebugProfile(&Scuttlebug::build, SpriteId::Scuttlebug, &ScuttlebugSpriteData, ProfileId::Scuttlebug, ProfileId::Scuttlebug, "Scuttlebug", ScuttlebugFileList);

/*const HitboxCollider::Info Scuttlebug::collisionInfo = {
    Vec2f(0.0f, -4.0f), Vec2f(16.0f, 16.0f), HitboxCollider::Shape::Rectangle, 5, 0, 0xFFFFFFFF, 0xFFFBFFFF, 0, &Enemy::collisionCallback
};*/


void Scuttlebug::_vf148() {
	dEn_c::_vf148();
	spawnCoin = true;
	this->Delete(1);
}

void Scuttlebug::_vf14C() {
	dEn_c::_vf14C();
	spawnCoin = true;
	this->Delete(1);
}

//this does weird shit, but it creates the iceblock around it
bool Scuttlebug::CreateIceActors() {
	struct DoSomethingCool my_struct = { 0, (Vec){pos.x, pos.y - 26, pos.z}, {2.0, 2.0, 4.0}, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
	this->frzMgr.Create_ICEACTORs((void*)&my_struct, 1);
	__destroy_arr((void*)&my_struct, sub_80024C20, 0x3C, 1);
	this->chrAnimation.setCurrentFrame(0.0);
	this->chrAnimation.setUpdateRate(0.0);
	//this->frozen = true;
	return true;
}


void Scuttlebug::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate)
{
	nw4r::g3d::ResAnmChr anmChr = this->resFileAnim.GetResAnmChr(name);
	this->chrAnimation.bind(&this->bodyModel, anmChr, unk);
	this->bodyModel.bindAnim(&this->chrAnimation, unk2);
	this->chrAnimation.setUpdateRate(rate);
}


int Scuttlebug::onCreate() {
    this->rot = (S16Vec){0x4000, 0, 0};
	this->scale = (Vec){2.5f, 2.5f, 2.5f};

	this->deleteForever = false; //makes the death state being used in preExecute

	// Model creation	
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	this->resFile.data = getResource("gasagoso", "g3d/gasagoso.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("gasagoso");
	bodyModel.setup(mdl, &allocator, 0x227, 1, 0);
	SetupTextures_Enemy(&bodyModel, 0);

	nw4r::g3d::ResMdl mdl2 = this->resFile.GetResMdl("string");
	bodyModel2.setup(mdl2, &allocator, 0x227, 1, 0);
	//SetupTextures_Player(&bodyModel2, 0);

	this->resFileAnim.data = getResource("gasagoso", "g3d/gasagoso.brres");
	nw4r::g3d::ResAnmChr anmChr = this->resFileAnim.GetResAnmChr("walk");
	this->chrAnimation.setup(mdl, anmChr, &this->allocator, 0);

	this->anmPat = this->resFile.GetResAnmTexPat("eyes");
	this->patAnimation.setup(mdl, anmPat, &this->allocator, 0, 1);
	this->patAnimation.bindEntry(&this->bodyModel, &anmPat, 0, 1);
	this->patAnimation.setFrameForEntry(0, 0);
	this->patAnimation.setUpdateRateForEntry(0.0f, 0);
	this->bodyModel.bindAnim(&this->patAnimation);

	allocator.unlink();

	ActivePhysics::Info HitMeBaby;

	HitMeBaby.xDistToCenter = 0.0;
	HitMeBaby.yDistToCenter = -4.0;

	HitMeBaby.xDistToEdge = 10.0;
	HitMeBaby.yDistToEdge = 12.0;

	HitMeBaby.category1 = 0x3;
	HitMeBaby.category2 = 0x0;
	HitMeBaby.bitfield1 = 0x6F;
	HitMeBaby.bitfield2 = 0xffbafffe;
	HitMeBaby.unkShort1C = 0;
	HitMeBaby.callback = &dEn_c::collisionCallback;

	this->aPhysics.initWithStruct(this, &HitMeBaby);
	this->aPhysics.addToList();

    // 16 = 1 tile?
    this->moveDistance = 16 * (this->settings >> 28 & 0xF); // nybble 5
    // 30 = 0.5 seconds
    this->idleTime = 30 * (this->settings >> 24 & 0xF); // nybble 6

    this->speed.y = 0.0f;
    this->y_speed_inc = 0.0f;				//gravity
    this->max_speed.y = 0.0f;

	bindAnimChr_and_setUpdateRate("walk", 1, 0.0, 1.0);

	originalPos = pos;
	doStateChange(&StateID_Lowering);

    return this->onExecute();
}


void Scuttlebug::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);

	bodyModel2.setDrawMatrix(matrix);
	bodyModel2.setScale(&scale);
	bodyModel2.calcWorld(false);
}

int Scuttlebug::onExecute() {
	bodyModel._vf1C();
	bodyModel2._vf1C();
	updateModelMatrices();

	acState.execute();
	
	
	if(eyeTimer < 180) {
		eyeTimer++;
	} else if (eyeTimer == 180) {
		this->patAnimation.setFrameForEntry(1, 0);
		eyeTimer++;
	} else if (eyeTimer < 195) {
		eyeTimer++;
	} else {
		eyeTimer = 0;
		this->patAnimation.setFrameForEntry(0, 0);
	}
	
	
	//dStateBase_c* currentState = this->acState.getCurrentState();
	//OSReport("Current State: %s\n", currentState->getName());
	//OSReport("pos: %f, %f, %f\n", pos.x, pos.y, pos.z);

	HandleXSpeed();                                      //consider the X speed
	HandleYSpeed();                                      //consider the Y speed
	doSpriteMovement();                                  //execute the speed movements

	if (this->chrAnimation.isAnimationDone()) {
		this->chrAnimation.setCurrentFrame(0.0);
	}

	return true;
}

int Scuttlebug::onDraw() {
    bodyModel.scheduleForDrawing();
	bodyModel2.scheduleForDrawing();

    return true;
}

int Scuttlebug::onDelete() {
	if(spawnCoin) {
		nw4r::snd::SoundHandle handle;
		PlaySoundWithFunctionB4(SoundRelatedClass, &handle, SE_EMY_DOWN, 1);
		StageE4::instance->spawnCoinJump(pos, 0, 1, 0);
	}
	return true;
}

void Scuttlebug::playerCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
    u32 hitType = usedForDeterminingStatePress_or_playerCollision(this, apThis, apOther, 0);

    if (hitType == 0) {
        DamagePlayer(this, apThis, apOther);
    } else if (hitType == 1 || hitType == 3) {
		SpawnEffect("Wm_en_burst_ss", 0, &pos, 0, &scale);
		VEC2 effectiveSpeed = (VEC2){0.0f, 0.0f};
        this->killWithSpecifiedState(apOther->owner, &effectiveSpeed, &Scuttlebug::StateID_Die);
    }
}

void Scuttlebug::yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
    this->playerCollision(apThis, apOther);
}

bool Scuttlebug::collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther) {
    return this->collisionCat3_StarPower(apThis, apOther);
}

bool Scuttlebug::collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther) {
    /*Vec3f effectOrigin(this->position.x, this->position.y, 4500.0f);
    Vec3f effectPos(effectOrigin);
    Effect::spawn(RP_Jugemu_CloudDisapp, &effectPos, nullptr, &this->scale);*/

    //this->wasDestroyed = true;

	SpawnEffect("Wm_en_burst_ss", 0, &pos, 0, &scale);

	VEC2 effectiveSpeed = (VEC2){0.0f, 0.0f};
	this->killWithSpecifiedState(apOther->owner, &effectiveSpeed, &Scuttlebug::StateID_Die);

    return true;
}

bool Scuttlebug::_vf120(ActivePhysics *apThis, ActivePhysics *apOther) {
    return this->collisionCat3_StarPower(apThis, apOther);
}

bool Scuttlebug::collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther) {
    return this->collisionCat3_StarPower(apThis, apOther);
}

bool Scuttlebug::collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther) {
    return this->collisionCat3_StarPower(apThis, apOther);
}

bool Scuttlebug::collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther) {
    return this->collisionCat3_StarPower(apThis, apOther);
}

bool Scuttlebug::collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther) {
    return this->collisionCat3_StarPower(apThis, apOther);
}

bool Scuttlebug::collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther) {
    return this->collisionCat3_StarPower(apThis, apOther);
}

/*bool Scuttlebug::collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther) {
    return this->collisionCat3_StarPower(apThis, apOther);
}*/

bool Scuttlebug::collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther) {
    return this->collisionCat3_StarPower(apThis, apOther);
}

bool Scuttlebug::collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther) {
    return this->collisionCat3_StarPower(apThis, apOther);
}

bool Scuttlebug::customCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	if((apThis->info.bitfield2 & 0x80000) != 0) {								//if sprite can be hit by hammer -> sprites that need another behaviour get entirely different functions, just doing some basic filtering for enemies
		switch (((daBoomerangHax_c*)apOther->owner)->variation) {
			case 0:
			case 1:
				return this->collisionCat3_StarPower(apThis, apOther);
			case 3:
				return dEn_c::frogCollision(apThis, apOther);
			default:
				OSReport("customCollision failed due to wrong custom projectile settings!\n");
		}
	}
	return false;
}

/** STATE: Lowering */

void Scuttlebug::beginState_Lowering() {
    this->timerLowering = 0;
	this->y_speed_inc = -0.0875f;				//gravity
    this->max_speed.y = -1.0f;
}

void Scuttlebug::executeState_Lowering() {
	//if (speed.y < -1.0f) speed.y = -1.0f;
    if (this->pos.y <= this->originalPos.y) {
        this->pos.y = originalPos.y;
        this->speed.y = 0.0f;
        this->doStateChange(&Scuttlebug::StateID_IdleLowered);
    }
}

void Scuttlebug::endState_Lowering() { }

/** STATE: IdleLowered */

void Scuttlebug::beginState_IdleLowered() {
    this->timerIdleLowered = 0;
    this->y_speed_inc = 0.0f;				//gravity
    this->max_speed.y = 0.0f;
}

void Scuttlebug::executeState_IdleLowered() {
    if (this->timerIdleLowered >= this->idleTime)
        this->doStateChange(&Scuttlebug::StateID_Rising);
    else
        this->timerIdleLowered++;
}

void Scuttlebug::endState_IdleLowered() { }

/** STATE: Rising */

void Scuttlebug::beginState_Rising() {
    this->timerRising = 0;
    this->y_speed_inc = 0.0875f;				//gravity
    this->max_speed.y = 1.0f;
}

void Scuttlebug::executeState_Rising() {
	if (speed.y > 2.0f) speed.y = 2.0f;
    if (this->timerRising >= this->moveDistance / 2) {
        this->speed.y = 0.0f;
        this->doStateChange(&Scuttlebug::StateID_IdleRaised);
    }
    else this->timerRising++;
}

void Scuttlebug::endState_Rising() { }

/** STATE: IdleRaised */

void Scuttlebug::beginState_IdleRaised() {
    this->timerIdleRaised = 0;
    this->y_speed_inc = 0.0f;				//gravity
    this->max_speed.y = 0.0f;
}

void Scuttlebug::executeState_IdleRaised() {
    if (this->timerIdleRaised >= this->idleTime)
        this->doStateChange(&Scuttlebug::StateID_Lowering);
    else
        this->timerIdleRaised++;
}

void Scuttlebug::endState_IdleRaised() { }

/** STATE: Die */

void Scuttlebug::beginState_Die() {
    this->removeMyActivePhysics();
    //this->wasDestroyed = true;
	spawnCoin = true;
	this->Delete(1);
}

void Scuttlebug::executeState_Die() { }

void Scuttlebug::endState_Die() { }
