#include <game.h>


const char* ScreamingPillarFileList[] = { "ScreamingPillar", NULL };

class daEnScreamingPillar_c : public dEn_c {
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

	USING_STATES(daEnScreamingPillar_c);
	DECLARE_STATE(Wait);
	DECLARE_STATE(Fall);
	DECLARE_STATE(Dead);
};

CREATE_STATE(daEnScreamingPillar_c, Wait);
CREATE_STATE(daEnScreamingPillar_c, Fall);
CREATE_STATE(daEnScreamingPillar_c, Dead);

void daEnScreamingPillar_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

dActor_c* daEnScreamingPillar_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daEnScreamingPillar_c));
	return new(buffer) daEnScreamingPillar_c;
}

const SpriteData ScreamingPillarSpriteData = { ProfileId::ScreamingPillar, 0, 0, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile ScreamingPillarProfile(&daEnScreamingPillar_c::build, SpriteId::ScreamingPillar, &ScreamingPillarSpriteData, ProfileId::ScreamingPillar, ProfileId::ScreamingPillar, "ScreamingPillar", ScreamingPillarFileList);


void daEnScreamingPillar_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate) {
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr(name);
	this->animationChr.bind(&this->bodyModel, anmChr, unk);
	this->bodyModel.bindAnim(&this->animationChr, unk2);
	this->animationChr.setUpdateRate(rate);
}



static void ScreamingPillarPhysCB1(daEnScreamingPillar_c *one, dStageActor_c *two) {
	OSReport("1\n");
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

static void ScreamingPillarPhysCB2(daEnScreamingPillar_c *one, dStageActor_c *two) {
	OSReport("2\n");
	return;
	
	if (two->stageActorType != 1)
		return;

	// if left/right
	//if (one->moveDirection <= 1)
	//	return;

	if (one->pos_delta.y < 0.0f)
		HurtMarioBecauseOfBeingSquashed(two, one, 2);
	else
		HurtMarioBecauseOfBeingSquashed(two, one, 10);
}

static void ScreamingPillarPhysCB3(daEnScreamingPillar_c *one, dStageActor_c *two, bool unkMaybeNotBool) {
	OSReport("3\n");
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

static bool ScreamingPillarPhysCB4(daEnScreamingPillar_c *one, dStageActor_c *two) {
	OSReport("4\n");
	return true;
}

static bool ScreamingPillarPhysCB5(daEnScreamingPillar_c *one, dStageActor_c *two) {
	OSReport("5\n");
	return true;
}

static bool ScreamingPillarPhysCB6(daEnScreamingPillar_c *one, dStageActor_c *two, bool unkMaybeNotBool) {
	OSReport("6\n");
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


void daEnScreamingPillar_c::playerCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	removeMyActivePhysics();
	doStateChange(&StateID_Fall);
}

void daEnScreamingPillar_c::yoshiCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->playerCollision(apThis, apOther);
}







int daEnScreamingPillar_c::onCreate() {
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	resFile.data = getResource("ScreamingPillar", "g3d/ScreamingPillar.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("DkScreamPillar");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_MapObj(&bodyModel, 0);

	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr("Wait");
	this->animationChr.setup(mdl, anmChr, &this->allocator, 0);

	allocator.unlink(); 

	this->scale.x = 0.2;
	this->scale.y = 0.2;
	this->scale.z = 0.2;

	ActivePhysics::Info HitMeBaby;
	HitMeBaby.xDistToCenter = 0.0;
	HitMeBaby.yDistToCenter = -72.0;
	HitMeBaby.xDistToEdge = 72.0;
	HitMeBaby.yDistToEdge = 280.0;
	HitMeBaby.category1 = 0x5;
	HitMeBaby.category2 = 0x0;
	HitMeBaby.bitfield1 = 0x4F;
	HitMeBaby.bitfield2 = 0xFFFFFFFF;
	HitMeBaby.unkShort1C = 0;
	HitMeBaby.callback = &dEn_c::collisionCallback;

	this->aPhysics.initWithStruct(this, &HitMeBaby);
	this->aPhysics.addToList();
	
	physicsInfo.x1 = -14;
	physicsInfo.y1 = 140;
	physicsInfo.x2 = -8;
	physicsInfo.y2 = 70;
	
	physicsInfo.otherCallback1 = &ScreamingPillarPhysCB1;
	physicsInfo.otherCallback2 = &ScreamingPillarPhysCB2;
	physicsInfo.otherCallback3 = &ScreamingPillarPhysCB3;

	physics.setup(this, &physicsInfo, 3, currentLayerID);
	physics.flagsMaybe = 0x260;
	//physics.setPtrToRotation(&rot1);
	physics.callback1 = &ScreamingPillarPhysCB4;
	physics.callback2 = &ScreamingPillarPhysCB5;
	physics.callback3 = &ScreamingPillarPhysCB6;
	physics.addToList();
	
	doStateChange(&StateID_Wait);
	
	return true;
}

int daEnScreamingPillar_c::onExecute() {
	updateModelMatrices();
	bodyModel._vf1C();

	acState.execute();

	/*this->rot1 += 0x200;
	this->rot2 += 0x200;
	this->rot3 += 0x200;
	
	physics1.setPtrToRotation(&rot1);
	physics2.setPtrToRotation(&rot2);
	physics3.setPtrToRotation(&rot3);
	*/

	return true;
}


void daEnScreamingPillar_c::beginState_Wait() {
	bindAnimChr_and_setUpdateRate("Wait", 1, 0.0, 1.0);
}
void daEnScreamingPillar_c::executeState_Wait() {
	if (this->animationChr.isAnimationDone()) {
		this->animationChr.setCurrentFrame(0.0);
	}
}
void daEnScreamingPillar_c::endState_Wait() {}


void daEnScreamingPillar_c::beginState_Fall() {
	bindAnimChr_and_setUpdateRate("tumble_front", 1, 0.0, 1.0);
}
void daEnScreamingPillar_c::executeState_Fall() {
	if (this->animationChr.isAnimationDone()) {
		doStateChange(&StateID_Dead);
	}
}
void daEnScreamingPillar_c::endState_Fall() {}

void daEnScreamingPillar_c::beginState_Dead() {
	bindAnimChr_and_setUpdateRate("tumble_wait", 1, 0.0, 1.0);
}
void daEnScreamingPillar_c::executeState_Dead() {
	if (this->animationChr.isAnimationDone()) {
		this->animationChr.setCurrentFrame(0.0);
	}
}
void daEnScreamingPillar_c::endState_Dead() {}



int daEnScreamingPillar_c::onDelete() {
	return true;
}

int daEnScreamingPillar_c::onDraw() {
	bodyModel.scheduleForDrawing();

	return true;
}