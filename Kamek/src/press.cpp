#include <game.h>


const char* PressFileList[] = { "Press", NULL };

class daEnPress_c : public dEn_c {
public:

	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	void updateModelMatrices();
	void bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate);

	static dActor_c* build();

	Physics::Info physicsInfo;
	Physics physics;
	
	mHeapAllocator_c allocator;
	m3d::mdl_c bodyModel;
	m3d::anmChr_c animationChr;
	nw4r::g3d::ResFile resFile;
	m3d::anmTexSrt_c body;
	
	u32 physicsWait;
	
	void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther);
	void yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther);

	/*bool collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther);

	bool collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther);*/

	USING_STATES(daEnPress_c);
	DECLARE_STATE(Wait);
	DECLARE_STATE(Fall);
	DECLARE_STATE(Dead);
};

CREATE_STATE(daEnPress_c, Wait);
CREATE_STATE(daEnPress_c, Fall);
CREATE_STATE(daEnPress_c, Dead);

void daEnPress_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

dActor_c* daEnPress_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daEnPress_c));
	return new(buffer) daEnPress_c;
}

const SpriteData PressSpriteData = { ProfileId::Press, 0, 0, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile PressProfile(&daEnPress_c::build, SpriteId::Press, &PressSpriteData, ProfileId::Press, ProfileId::Press, "Press", PressFileList);


void daEnPress_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate) {
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr(name);
	this->animationChr.bind(&this->bodyModel, anmChr, unk);
	this->bodyModel.bindAnim(&this->animationChr, unk2);
	this->animationChr.setUpdateRate(rate);
}



static void PressPhysCB1(daEnPress_c *one, dStageActor_c *two) {
	return;
	
	if (two->stageActorType != 1)
		return;

	// if left/right
	//if (one->moveDirection <= 1)
	//	return;

	if (one->pos_delta.y > 0.0f)
		HurtMarioBecauseOfBeingSquashed(two, one, 1);
	else
		HurtMarioBecauseOfBeingSquashed(two, one, 9);
}

static void PressPhysCB2(daEnPress_c *one, dStageActor_c *two) {
	if (two->stageActorType != 1)
		return;

	// if left/right
	//if (one->moveDirection <= 1)
	//	return;

	if (one->pos_delta.y < 1.0f)
		HurtMarioBecauseOfBeingSquashed(two, one, 2);
	else
		HurtMarioBecauseOfBeingSquashed(two, one, 10);
}

static void PressPhysCB3(daEnPress_c *one, dStageActor_c *two, bool unkMaybeNotBool) {
	return;

	if (two->stageActorType != 1)
		return;

	// if up/down
	//if (one->moveDirection > 1)
	//	return;

	if (unkMaybeNotBool) {
		if (one->pos_delta.x > 0.0f)
			HurtMarioBecauseOfBeingSquashed(two, one, 6);
		else
			HurtMarioBecauseOfBeingSquashed(two, one, 12);
	} else {
		if (one->pos_delta.x < 0.0f)
			HurtMarioBecauseOfBeingSquashed(two, one, 5);
		else
			HurtMarioBecauseOfBeingSquashed(two, one, 11);
	}
}

static bool PressPhysCB4(daEnPress_c *one, dStageActor_c *two) {
	return true;
}

static bool PressPhysCB5(daEnPress_c *one, dStageActor_c *two) {
	return true;
}

static bool PressPhysCB6(daEnPress_c *one, dStageActor_c *two, bool unkMaybeNotBool) {
	return true;

	if (unkMaybeNotBool) {
		if (one->pos_delta.x > 0.0f)
			return true;
	} else {
		if (one->pos_delta.x < 0.0f)
			return true;
	}
	return false;
}


void daEnPress_c::playerCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	removeMyActivePhysics();
	doStateChange(&StateID_Fall);
}

void daEnPress_c::yoshiCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->playerCollision(apThis, apOther);
}







int daEnPress_c::onCreate() {
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	resFile.data = getResource("Press", "g3d/Press.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("Press");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_MapObj(&bodyModel, 0);

	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr("Press");
	this->animationChr.setup(mdl, anmChr, &this->allocator, 0);

	nw4r::g3d::ResAnmTexSrt anmRes = this->resFile.GetResAnmTexSrt("Lava");
	this->body.setup(mdl, anmRes, &this->allocator, 0, 1);
	this->body.bindEntry(&this->bodyModel, anmRes, 0, 0);
	this->bodyModel.bindAnim(&this->body, 0.0);

	allocator.unlink(); 

	this->scale.x = 0.05;
	this->scale.y = 0.05;
	this->scale.z = 0.05;

	ActivePhysics::Info HitMeBaby;
	HitMeBaby.xDistToCenter = 0.0;
	HitMeBaby.yDistToCenter = -72.0;
	HitMeBaby.xDistToEdge = 90.0;
	HitMeBaby.yDistToEdge = 280.0;
	HitMeBaby.category1 = 0x5;
	HitMeBaby.category2 = 0x0;
	HitMeBaby.bitfield1 = 0x4F;
	HitMeBaby.bitfield2 = 0xFFC00000;
	HitMeBaby.unkShort1C = 0;
	HitMeBaby.callback = &dEn_c::collisionCallback;

	this->aPhysics.initWithStruct(this, &HitMeBaby);
	this->aPhysics.addToList();
	
	physicsInfo.x1 = -14;
	physicsInfo.y1 = 120;
	physicsInfo.x2 = 14;
	physicsInfo.y2 = 118;
	
	physicsInfo.otherCallback1 = &PressPhysCB1;
	physicsInfo.otherCallback2 = &PressPhysCB2;
	physicsInfo.otherCallback3 = &PressPhysCB3;

	physics.setup(this, &physicsInfo, 3, currentLayerID);
	physics.flagsMaybe = 0x260;
	//physics.setPtrToRotation(&rot1);
	physics.callback1 = &PressPhysCB4;
	physics.callback2 = &PressPhysCB5;
	physics.callback3 = &PressPhysCB6;
	//physics.addToList();
	
	doStateChange(&StateID_Wait);
	
	return true;
}

int daEnPress_c::onExecute() {
	updateModelMatrices();
	bodyModel._vf1C();

	acState.execute();

	this->body.process();

	/*this->rot1 += 0x200;
	this->rot2 += 0x200;
	this->rot3 += 0x200;
	
	physics1.setPtrToRotation(&rot1);
	physics2.setPtrToRotation(&rot2);
	physics3.setPtrToRotation(&rot3);
	*/

	return true;
}


void daEnPress_c::beginState_Wait() {
	bindAnimChr_and_setUpdateRate("Press", 1, 0.0, 0.5);
}
void daEnPress_c::executeState_Wait() {
	if (this->animationChr.isAnimationDone()) {
		this->animationChr.setCurrentFrame(0.0);
	}
}
void daEnPress_c::endState_Wait() {}


void daEnPress_c::beginState_Fall() {
	bindAnimChr_and_setUpdateRate("Press", 1, 0.0, 0.5);
}
void daEnPress_c::executeState_Fall() {
	if(physicsWait < 60) {
		physicsWait++;
	} else if (physicsWait == 60) {
		physics.addToList();
		physicsWait++;
	} else if (physicsWait < 73) {
		this->physicsInfo.y1-=7.5;
		this->physicsInfo.y2-=10;
		physics.setup(this, &physicsInfo, 3, currentLayerID);		
		physicsWait++;
	} else if(physicsWait == 73) {
		ShakeScreen(StageScreen, 0, 1, 0, 0);
		SpawnEffect("Wm_mr_sndlandsmk", 0, &(Vec){this->pos.x, this->pos.y + 4.0, this->pos.z + 5500.0}, &(S16Vec){0,0,0}, &(Vec){1.0, 1.0, 1.0});
		PlaySound(this, SE_BOSS_MORTON_GROUND_SHAKE);
		physicsWait++;
	}

	if (this->animationChr.isAnimationDone()) {
		doStateChange(&StateID_Dead);
	}
}
void daEnPress_c::endState_Fall() {}

void daEnPress_c::beginState_Dead() {
	bindAnimChr_and_setUpdateRate("Press", 1, 0.0, 0.5);
}
void daEnPress_c::executeState_Dead() {
	if (this->animationChr.isAnimationDone()) {
		this->animationChr.setCurrentFrame(0.0);
	}
}
void daEnPress_c::endState_Dead() {}



int daEnPress_c::onDelete() {
	return true;
}

int daEnPress_c::onDraw() {
	bodyModel.scheduleForDrawing();

	return true;
}