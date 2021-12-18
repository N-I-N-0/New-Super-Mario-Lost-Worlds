#include <common.h>
#include <game.h>
#include <profile.h>
#include "path.h"

extern "C" dCourse_c::rail_s *GetRail(int id);

const char* BallonFileList[] = { "ballon", 0 };

class daBallon_c : public dEnPath_c {
public:
	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	mHeapAllocator_c allocator;
	nw4r::g3d::ResFile resFile;
	m3d::mdl_c bodyModel;

	u8 moveDirection;
	Physics physics0;
	Physics::Info physicsInfo1;
	Physics physics1;
	Physics::Info physicsInfo2;
	Physics physics2;
	Physics::Info physicsInfo3;
	Physics physics3;
	Physics::Info physicsInfo4;
	Physics physics4;
	Physics::Info physicsInfo5;
	Physics physics5;
	Physics::Info physicsInfo6;
	Physics physics6;
	float aTestFloat;
	s16 leftRotPtr;
	s16 rightRotPtr;
	StandOnTopCollider sotCollider;

	dCourse_c *course;
	dCourse_c::rail_s *rail;
	dCourse_c::railNode_s *currentNode;
	dCourse_c::railNode_s *nextNode;
	int currentNodeNum;
	float moveDeltaX, moveDeltaY;
	int steps;
	int speed;

	static dActor_c *build();

	void updateModelMatrices();
	void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther);
	void yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther);

	bool collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther);

	bool collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther);
	
	USING_STATES(daBallon_c);
	
	DECLARE_STATE(Wait);
};

CREATE_STATE(daBallon_c, Wait);


void daBallon_c::beginState_Wait() {}
void daBallon_c::executeState_Wait() {
	dStageActor_c* player = (dStageActor_c*)FindActorByType(PLAYER, 0);

	dx = player->pos.x - this->pos.x;
	dy = player->pos.y - this->pos.y;

	distance = sqrtf(pow(dx, 2) + pow(dy, 2));

	if (distance <= 64) {
		doStateChange(&StateID_FollowPath);
	}
}
void daBallon_c::endState_Wait() {}






const SpriteData BallonSpriteData = { ProfileId::Ballon, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile BallonProfile(&daBallon_c::build, SpriteId::Ballon, BallonSpriteData, ProfileId::Ballon, ProfileId::Ballon, "Ballon", BallonFileList);


void daBallon_c::playerCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
}
void daBallon_c::yoshiCollision(ActivePhysics* apThis, ActivePhysics* apOther) {

}
bool daBallon_c::collisionCat7_GroundPound(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daBallon_c::collisionCat7_GroundPoundYoshi(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daBallon_c::collisionCatD_Drill(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daBallon_c::collisionCatA_PenguinMario(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}

bool daBallon_c::collisionCat1_Fireball_E_Explosion(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daBallon_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daBallon_c::collisionCat9_RollingObject(ActivePhysics* apThis, ActivePhysics* apOther) {

	return false;
}
bool daBallon_c::collisionCat13_Hammer(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daBallon_c::collisionCat14_YoshiFire(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}

bool daBallon_c::collisionCat3_StarPower(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}


dActor_c* daBallon_c::build() {
	void* buffer = AllocFromGameHeap1(sizeof(daBallon_c));
	daBallon_c* c = new(buffer) daBallon_c;

	return c;
}

static void BallonPhysCB1(daBallon_c *one, dStageActor_c *two) {
	if (two->stageActorType != 1)
		return;

	// if left/right
	if (one->moveDirection <= 1)
		return;

	if (one->pos_delta.y > 0.0f)
		HurtMarioBecauseOfBeingSquashed(two, one, 1);
	else
		HurtMarioBecauseOfBeingSquashed(two, one, 9);
}

static void BallonPhysCB2(daBallon_c *one, dStageActor_c *two) {
	if (two->stageActorType != 1)
		return;

	// if left/right
	if (one->moveDirection <= 1)
		return;

	if (one->pos_delta.y < 0.0f)
		HurtMarioBecauseOfBeingSquashed(two, one, 2);
	else
		HurtMarioBecauseOfBeingSquashed(two, one, 10);
}

static void BallonPhysCB3(daBallon_c *one, dStageActor_c *two, bool unkMaybeNotBool) {
	if (two->stageActorType != 1)
		return;

	// if up/down
	if (one->moveDirection > 1)
		return;

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

static bool BallonPhysCB4(daBallon_c *one, dStageActor_c *two) {
	return (one->pos_delta.y > 0.0f);
}

static bool BallonPhysCB5(daBallon_c *one, dStageActor_c *two) {
	return (one->pos_delta.y < 0.0f);
}

static bool BallonPhysCB6(daBallon_c *one, dStageActor_c *two, bool unkMaybeNotBool) {
	if (unkMaybeNotBool) {
		if (one->pos_delta.x > 0.0f)
			return true;
	} else {
		if (one->pos_delta.x < 0.0f)
			return true;
	}
	return false;
}

int daBallon_c::onCreate() {
	this->deleteForever = true;

	// Model creation	
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	this->resFile.data = getResource("ballon", "g3d/ballon_green.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("ballon_green");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Player(&bodyModel, 0);

	allocator.unlink();


	//round
	physics0.setupRound(this, 0.0f, 175.0f, 80.0f, (void*)&BallonPhysCB1, (void*)&BallonPhysCB2, (void*)&BallonPhysCB3, 1, 0, 0);
	physics0.callback1 = (void*)&BallonPhysCB4;
	physics0.callback2 = (void*)&BallonPhysCB5;
	physics0.callback3 = (void*)&BallonPhysCB6;
	physics0.addToList();
	

	//middle bottom
	physicsInfo1.x1 = -24;
	physicsInfo1.y1 = 4;
	physicsInfo1.x2 = 24;
	physicsInfo1.y2 = 0;

	physicsInfo1.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
	physicsInfo1.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
	physicsInfo1.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

	physics1.setup(this, &physicsInfo1, 3, currentLayerID);
	physics1.flagsMaybe = 0x260;
	physics1.callback1 = &daEnBlockMain_c::PhysicsCallback1;
	physics1.callback2 = &daEnBlockMain_c::PhysicsCallback2;
	physics1.callback3 = &daEnBlockMain_c::PhysicsCallback3;
	physics1.addToList();
	
	
	//left bottom
	physicsInfo2.x1 = -24;
	physicsInfo2.y1 = 20;
	physicsInfo2.x2 = -20;
	physicsInfo2.y2 = 4;

	physicsInfo2.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
	physicsInfo2.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
	physicsInfo2.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

	physics2.setup(this, &physicsInfo2, 3, currentLayerID);
	physics2.flagsMaybe = 0x260;
	physics2.callback1 = &daEnBlockMain_c::PhysicsCallback1;
	physics2.callback2 = &daEnBlockMain_c::PhysicsCallback2;
	physics2.callback3 = &daEnBlockMain_c::PhysicsCallback3;
	physics2.addToList();


	//right bottom
	physicsInfo3.x1 = 20;
	physicsInfo3.y1 = 20;
	physicsInfo3.x2 = 24;
	physicsInfo3.y2 = 4;

	physicsInfo3.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
	physicsInfo3.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
	physicsInfo3.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

	physics3.setup(this, &physicsInfo3, 3, currentLayerID);
	physics3.flagsMaybe = 0x260;
	physics3.callback1 = &daEnBlockMain_c::PhysicsCallback1;
	physics3.callback2 = &daEnBlockMain_c::PhysicsCallback2;
	physics3.callback3 = &daEnBlockMain_c::PhysicsCallback3;
	physics3.addToList();
	
	
	//right top
	physicsInfo4.x1 = -14;
	physicsInfo4.y1 = 140;
	physicsInfo4.x2 = -8;
	physicsInfo4.y2 = 70;
	rightRotPtr = -5500;
	
	physicsInfo4.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
	physicsInfo4.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
	physicsInfo4.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

	physics4.setup(this, &physicsInfo4, 3, currentLayerID);
	physics4.flagsMaybe = 0x260;
	physics4.setPtrToRotation(&rightRotPtr);
	physics4.callback1 = &daEnBlockMain_c::PhysicsCallback1;
	physics4.callback2 = &daEnBlockMain_c::PhysicsCallback2;
	physics4.callback3 = &daEnBlockMain_c::PhysicsCallback3;
	physics4.addToList();
	
	
	//left top
	physicsInfo5.x1 = 8;
	physicsInfo5.y1 = 140;
	physicsInfo5.x2 = 14;
	physicsInfo5.y2 = 70;
	leftRotPtr = 5500;

	physicsInfo5.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
	physicsInfo5.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
	physicsInfo5.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

	physics5.setup(this, &physicsInfo5, 3, currentLayerID);
	physics5.flagsMaybe = 0x260;
	physics5.setPtrToRotation(&leftRotPtr);
	physics5.callback1 = &daEnBlockMain_c::PhysicsCallback1;
	physics5.callback2 = &daEnBlockMain_c::PhysicsCallback2;
	physics5.callback3 = &daEnBlockMain_c::PhysicsCallback3;
	physics5.addToList();
	
	
	//middle top
	physicsInfo6.x1 = -32;
	physicsInfo6.y1 = 72;
	physicsInfo6.x2 = 32;
	physicsInfo6.y2 = 62;

	physicsInfo6.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
	physicsInfo6.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
	physicsInfo6.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

	physics6.setup(this, &physicsInfo6, 3, currentLayerID);
	physics6.flagsMaybe = 0x260;
	physics6.callback1 = &daEnBlockMain_c::PhysicsCallback1;
	physics6.callback2 = &daEnBlockMain_c::PhysicsCallback2;
	physics6.callback3 = &daEnBlockMain_c::PhysicsCallback3;
	physics6.addToList();
	
	
	

	sotCollider.init(this, 0, 16, -8, 24, -24, 0, 1);
	sotCollider._47 = 0xA;
	sotCollider.flags = 0x80180 | 0xC00;
	sotCollider.addToList();
	
	
	
	
	
	
	this->disableEatIn();


	this->aTestFloat = 0.25f;


	/*speed = ((this->settings >> 12 & 0b1111) + 1 ) * 8;						//Bit 29-32
	currentNodeNum = this->settings >> 8 & 0b11111111;						//Bit 33-40
	int pathID = this->settings & 0b11111111;								//Bit 41-48
	if(pathID) {
		rail = GetRail(pathID);
		course = dCourseFull_c::instance->get(GetAreaNum());
		currentNode = &course->railNode[rail->startNode + currentNodeNum];
		nextNode = &course->railNode[rail->startNode + 1 + currentNodeNum];
		OSReport("pathID: %d\n", pathID);
		OSReport("startNode: %d\n", currentNodeNum);
		OSReport("N1.x: %d, N1.y: %d, N2.x: %d, N2.y: %d\n", currentNode->xPos, currentNode->yPos, nextNode->xPos, nextNode->yPos);
		//OSReport("Ballon: %f, %f\n", this->pos.x, this->pos.y);
		this->pos.x = currentNode->xPos;
		this->pos.y = -currentNode->yPos;
		//OSReport("Ballon: %f, %f\n", this->pos.x, this->pos.y);
	} else {
		
	}*/
	

	// Stuff I do understand
	this->scale = (Vec){1, 1, 1};

	this->rot.x = 0;
	this->rot.y = 0;
	this->rot.z = 0;

	this->pos.z = 4000;

	beginState_Init();
	executeState_Init();
	doStateChange(&StateID_Wait);
	
	this->onExecute();
	return true;
}


int daBallon_c::onDelete() {
	return true;
}

int daBallon_c::onDraw() {
	bodyModel.scheduleForDrawing();
	return true;
}


void daBallon_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

int daBallon_c::onExecute() {
	bodyModel._vf1C();
	updateModelMatrices();
	acState.execute();
	/*physics1.removeFromList();
	physics1.setupRound(this, 0.0f, 200.0f - this->aTestFloat, 80.0f, (void*)&BallonPhysCB1, (void*)&BallonPhysCB2, (void*)&BallonPhysCB3, 1, 0, 0);
	physics1.addToList();
	OSReport("Ballon aTestFloat: %f\n", this->aTestFloat);
	this->aTestFloat += 0.25;*/
	//this->pos.x += aTestFloat;
	
	//this->rot.y = -5500;
	//OSReport("Rot.y: %d\n", this->rot.y);
	
		//round
	physics0.removeFromList();
	physics0.setupRound(this, 0.0f, 175.0f, 80.0f, (void*)&BallonPhysCB1, (void*)&BallonPhysCB2, (void*)&BallonPhysCB3, 1, 0, 0);
	physics0.callback1 = (void*)&BallonPhysCB4;
	physics0.callback2 = (void*)&BallonPhysCB5;
	physics0.callback3 = (void*)&BallonPhysCB6;
	physics0.addToList();
	
	sotCollider.update();
	physics0.update();
	physics1.update();
	physics2.update();
	physics3.update();
	physics4.update();
	physics5.update();
	physics6.update();
}