#include <common.h>
#include <game.h>
#include <profile.h>
#include "path.h"

extern "C" dCourse_c::rail_s *GetRail(int id);

const char* TikiAirshipFileList[] = { "tikiAirship", 0 };

class daTikiAirship_c : public dEnPath_c {
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
	StandOnTopCollider sotCollider;
	StandOnTopCollider sotCollider1;
	StandOnTopCollider sotCollider2;

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
	
	USING_STATES(daTikiAirship_c);
	
	DECLARE_STATE(Wait);
};

CREATE_STATE(daTikiAirship_c, Wait);


void daTikiAirship_c::beginState_Wait() {}
void daTikiAirship_c::executeState_Wait() {
	dStageActor_c* player = (dStageActor_c*)FindActorByType(PLAYER, 0);

	dx = player->pos.x - this->pos.x;
	dy = player->pos.y - this->pos.y;

	distance = sqrtf(pow(dx, 2) + pow(dy, 2));

	if (distance <= 64) {
		doStateChange(&StateID_FollowPath);
	}
}
void daTikiAirship_c::endState_Wait() {}






const SpriteData TikiAirshipSpriteData = { ProfileId::TikiAirship, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile TikiAirshipProfile(&daTikiAirship_c::build, SpriteId::TikiAirship, &TikiAirshipSpriteData, ProfileId::TikiAirship, ProfileId::TikiAirship, "TikiAirship", TikiAirshipFileList);


void daTikiAirship_c::playerCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
}
void daTikiAirship_c::yoshiCollision(ActivePhysics* apThis, ActivePhysics* apOther) {

}
bool daTikiAirship_c::collisionCat7_GroundPound(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daTikiAirship_c::collisionCat7_GroundPoundYoshi(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daTikiAirship_c::collisionCatD_Drill(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daTikiAirship_c::collisionCatA_PenguinMario(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}

bool daTikiAirship_c::collisionCat1_Fireball_E_Explosion(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daTikiAirship_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daTikiAirship_c::collisionCat9_RollingObject(ActivePhysics* apThis, ActivePhysics* apOther) {

	return false;
}
bool daTikiAirship_c::collisionCat13_Hammer(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daTikiAirship_c::collisionCat14_YoshiFire(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}

bool daTikiAirship_c::collisionCat3_StarPower(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}


dActor_c* daTikiAirship_c::build() {
	void* buffer = AllocFromGameHeap1(sizeof(daTikiAirship_c));
	daTikiAirship_c* c = new(buffer) daTikiAirship_c;

	return c;
}

int daTikiAirship_c::onCreate() {
	this->deleteForever = true;

	allocator.link(-1, GameHeaps[0], 0, 0x20);

	this->resFile.data = getResource("tikiAirship", "g3d/TikiAirship.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("TikiAirship");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	//SetupTextures_Player(&bodyModel, 0);

	allocator.unlink();


	//middle bottom
	physicsInfo1.x1 = -34;
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
	physicsInfo2.x1 = -60;//-34;
	physicsInfo2.y1 = 20;
	physicsInfo2.x2 = -30;
	physicsInfo2.y2 = 0;

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
	physicsInfo3.x2 = 58;//24;
	physicsInfo3.y2 = 0;

	physicsInfo3.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
	physicsInfo3.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
	physicsInfo3.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

	physics3.setup(this, &physicsInfo3, 3, currentLayerID);
	physics3.flagsMaybe = 0x260;
	physics3.callback1 = &daEnBlockMain_c::PhysicsCallback1;
	physics3.callback2 = &daEnBlockMain_c::PhysicsCallback2;
	physics3.callback3 = &daEnBlockMain_c::PhysicsCallback3;
	physics3.addToList();
	
	

	sotCollider.init(this, 0, 16, -8, 24, -34, 0, 1);
	sotCollider._47 = 0xA;
	sotCollider.flags = 0x80180 | 0xC00;
	sotCollider.addToList();

	sotCollider1.init(this, 0, 16, 4.1f, -30, -60, 0, 1);
	sotCollider1._47 = 0xA;
	sotCollider1.flags = 0x80180 | 0xC00;
	sotCollider1.addToList();
	
	sotCollider2.init(this, 0, 16, 4.1f, 58, 20, 0, 1);
	sotCollider2._47 = 0xA;
	sotCollider2.flags = 0x80180 | 0xC00;
	sotCollider2.addToList();
	
	this->disableEatIn();
	
	this->scale = (Vec){-0.75, 0.75, 0.75};

	this->rot.x = 0;
	this->rot.y = 0x4000;
	this->rot.z = 0;

	this->pos.z = -4000;

	beginState_Init();
	executeState_Init();
	doStateChange(&StateID_Wait);
	
	this->onExecute();
	return true;
}


int daTikiAirship_c::onDelete() {
	return true;
}

int daTikiAirship_c::onDraw() {
	bodyModel.scheduleForDrawing();
	return true;
}


void daTikiAirship_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y + 80, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

int daTikiAirship_c::onExecute() {
	bodyModel._vf1C();
	updateModelMatrices();
	acState.execute();

	
	sotCollider.update();
	sotCollider1.update();
	sotCollider2.update();
	physics1.update();
	physics2.update();
	physics3.update();
}