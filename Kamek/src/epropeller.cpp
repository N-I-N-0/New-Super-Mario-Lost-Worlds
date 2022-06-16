#include <game.h>


const char* EpropellerFileList[] = { "Epropeller", NULL };

class daEnEpropeller_c : public dEn_c {
public:

	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	void updateModelMatrices();
	

	static dActor_c* build();

	//void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther);
	//void spriteCollision(ActivePhysics *apThis, ActivePhysics *apOther);
	//void yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther);

	//bool collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat5_Mario(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat8_FencePunch(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat11_PipeCannon(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther); 
	
	s16 rot1;
	s16 rot2;
	s16 rot3;
	
	Physics::Info physicsInfo1;
	Physics physics1;
	Physics::Info physicsInfo2;
	Physics physics2;
	Physics::Info physicsInfo3;
	Physics physics3;
	
	
	mHeapAllocator_c allocator;
	m3d::mdl_c bodyModel;
	m3d::anmChr_c animationChr;
	nw4r::g3d::ResFile resFile;

};


void daEnEpropeller_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

dActor_c* daEnEpropeller_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daEnEpropeller_c));
	return new(buffer) daEnEpropeller_c;
}

const SpriteData EpropellerSpriteData = { ProfileId::Epropeller, 0, 0, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile EpropellerProfile(&daEnEpropeller_c::build, SpriteId::Epropeller, &EpropellerSpriteData, ProfileId::Epropeller, ProfileId::Epropeller, "Epropeller", EpropellerFileList);






static void EpropellerPhysCB1(daEnEpropeller_c *one, dStageActor_c *two) {
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

static void EpropellerPhysCB2(daEnEpropeller_c *one, dStageActor_c *two) {
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

static void EpropellerPhysCB3(daEnEpropeller_c *one, dStageActor_c *two, bool unkMaybeNotBool) {
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

static bool EpropellerPhysCB4(daEnEpropeller_c *one, dStageActor_c *two) {
	OSReport("4\n");
	return true;
}

static bool EpropellerPhysCB5(daEnEpropeller_c *one, dStageActor_c *two) {
	OSReport("5\n");
	return true;
}

static bool EpropellerPhysCB6(daEnEpropeller_c *one, dStageActor_c *two, bool unkMaybeNotBool) {
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










int daEnEpropeller_c::onCreate() {
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	resFile.data = getResource("Epropeller", "g3d/Epropeller.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("Epropeller");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Enemy(&bodyModel, 0);

	allocator.unlink(); 

	this->scale.x = 0.1;
	this->scale.y = 0.1;
	this->scale.z = 0.1;

	
	physicsInfo1.x1 = -14;
	physicsInfo1.y1 = 140;
	physicsInfo1.x2 = -8;
	physicsInfo1.y2 = 70;
	rot1 = 0;
	
	physicsInfo1.otherCallback1 = &EpropellerPhysCB1;
	physicsInfo1.otherCallback2 = &EpropellerPhysCB2;
	physicsInfo1.otherCallback3 = &EpropellerPhysCB3;

	physics1.setup(this, &physicsInfo1, 3, currentLayerID);
	physics1.flagsMaybe = 0x260;
	physics1.setPtrToRotation(&rot1);
	physics1.callback1 = &EpropellerPhysCB4;
	physics1.callback2 = &EpropellerPhysCB5;
	physics1.callback3 = &EpropellerPhysCB6;
	physics1.addToList();
	
	
	physicsInfo2.x1 = -14;
	physicsInfo2.y1 = 140;
	physicsInfo2.x2 = -8;
	physicsInfo2.y2 = 70;
	rot2 = -0x2AAB;
	
	physicsInfo2.otherCallback1 = &EpropellerPhysCB1;
	physicsInfo2.otherCallback2 = &EpropellerPhysCB2;
	physicsInfo2.otherCallback3 = &EpropellerPhysCB3;

	physics2.setup(this, &physicsInfo2, 3, currentLayerID);
	physics2.flagsMaybe = 0x260;
	physics2.setPtrToRotation(&rot2);
	physics2.callback1 = &EpropellerPhysCB4;
	physics2.callback2 = &EpropellerPhysCB5;
	physics2.callback3 = &EpropellerPhysCB6;
	physics2.addToList();
	
	
	physicsInfo3.x1 = -14;
	physicsInfo3.y1 = 140;
	physicsInfo3.x2 = -8;
	physicsInfo3.y2 = 70;
	rot3 = 0x2AAB;
	
	physicsInfo3.otherCallback1 = &EpropellerPhysCB1;
	physicsInfo3.otherCallback2 = &EpropellerPhysCB2;
	physicsInfo3.otherCallback3 = &EpropellerPhysCB3;

	physics3.setup(this, &physicsInfo3, 3, currentLayerID);
	physics3.flagsMaybe = 0x260;
	physics3.setPtrToRotation(&rot3);
	physics3.callback1 = &EpropellerPhysCB4;
	physics3.callback2 = &EpropellerPhysCB5;
	physics3.callback3 = &EpropellerPhysCB6;
	physics3.addToList();
	
	return true;
}

int daEnEpropeller_c::onExecute() {
	updateModelMatrices();
	bodyModel._vf1C();

	physics1._CC = 5; //5 coind, 7 blue coin?
	physics2._CC = 5;
	physics3._CC = 5;


	this->rot.z += 0x200;
	/*this->rot1 += 0x200;
	this->rot2 += 0x200;
	this->rot3 += 0x200;
	
	physics1.setPtrToRotation(&rot1);
	physics2.setPtrToRotation(&rot2);
	physics3.setPtrToRotation(&rot3);
	*/

	return true;
}

int daEnEpropeller_c::onDelete() {
	return true;
}

int daEnEpropeller_c::onDraw() {
	bodyModel.scheduleForDrawing();

	return true;
}