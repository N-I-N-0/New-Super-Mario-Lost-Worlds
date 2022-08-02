#include <common.h>
#include <game.h>
#include <profile.h>
#include "path.h"

const char* PokeyHeadModelFileList[] = { "sanboHead", 0 };

class daEnPokeyHead_c : public dEn_c {
public:
	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	mHeapAllocator_c allocator;
	nw4r::g3d::ResFile resFile;
	m3d::mdl_c bodyModel;

	static dActor_c* build();

	void updateModelMatrices();
	void playerCollision(ActivePhysics* apThis, ActivePhysics* apOther);
	void yoshiCollision(ActivePhysics* apThis, ActivePhysics* apOther);

	/*bool collisionCat7_GroundPound(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat7_GroundPoundYoshi(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCatD_Drill(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCatA_PenguinMario(ActivePhysics* apThis, ActivePhysics* apOther);

	bool collisionCat1_Fireball_E_Explosion(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat9_RollingObject(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat13_Hammer(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat14_YoshiFire(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat3_StarPower(ActivePhysics* apThis, ActivePhysics* apOther);*/

	//setDeathInfo_IceBreak -> what should be done when in an iceblock and crashes a wall -> normally the sprite should die
	//void _vf148();
	//setDeathInfo_IceVanish -> what should be done when collision with an iceblock thrown by Mario -> normally the sprite should die
	//void _vf14C();
	//create an ice block when collided with Iceball
	bool CreateIceActors();

	USING_STATES(daEnPokeyHead_c);
	DECLARE_STATE(Wait);

	//void addScoreWhenHit(void* other);
};

CREATE_STATE(daEnPokeyHead_c, Wait);

void daEnPokeyHead_c::beginState_Wait() {}
void daEnPokeyHead_c::executeState_Wait() {}
void daEnPokeyHead_c::endState_Wait() {}

const SpriteData PokeyHeadSpriteData = { ProfileId::PokeyHead, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile PokeyHeadProfile(&daEnPokeyHead_c::build, ProfileId::PokeyHead, &PokeyHeadSpriteData, ProfileId::PokeyHead, ProfileId::PokeyHead, "PokeyHead", PokeyHeadModelFileList);



void daEnPokeyHead_c::playerCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	DamagePlayer(this, apThis, apOther);
}
void daEnPokeyHead_c::yoshiCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	DamagePlayer(this, apThis, apOther);
}



//this does weird shit, but it creates the iceblock around it
bool daEnPokeyHead_c::CreateIceActors() {
	struct DoSomethingCool my_struct = { 0, (Vec){pos.x, pos.y - 20, pos.z}, {1.4, 1.4, 2.0}, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
	this->frzMgr.Create_ICEACTORs((void*)&my_struct, 1);
	__destroy_arr((void*)&my_struct, sub_80024C20, 0x3C, 1);
	return true;
}


dActor_c* daEnPokeyHead_c::build() {
	void* buffer = AllocFromGameHeap1(sizeof(daEnPokeyHead_c));
	daEnPokeyHead_c* c = new(buffer) daEnPokeyHead_c;

	return c;
}

int daEnPokeyHead_c::onCreate() {
	this->deleteForever = false; //makes the death state being used in preExecute

	// Model creation	
	allocator.link(-1, GameHeaps[0], 0, 0x20);
	this->resFile.data = getResource("sanboHead", "g3d/sanbo.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("sanbo_head");
	bodyModel.setup(mdl, &allocator, 0x227, 1, 0);
	SetupTextures_Enemy(&bodyModel, 0);

	allocator.unlink();

	ActivePhysics::Info HitMeBaby;

	HitMeBaby.xDistToCenter = 0.0;
	HitMeBaby.yDistToCenter = -7.5;

	HitMeBaby.xDistToEdge = 7.5;
	HitMeBaby.yDistToEdge = 7.5;

	HitMeBaby.category1 = 0x3;
	HitMeBaby.category2 = 0x0;
	HitMeBaby.bitfield1 = 0x6F;
	HitMeBaby.bitfield2 = 0xffbafffe;
	HitMeBaby.unkShort1C = 0;
	HitMeBaby.callback = &dEn_c::collisionCallback;

	this->aPhysics.initWithStruct(this, &HitMeBaby);
	this->aPhysics.addToList();

	//this->scale = (Vec){0.4, 0.4, 0.4};
	//this->pos.z = 4000;

	doStateChange(&StateID_Wait);

	return true;
}


int daEnPokeyHead_c::onDelete() {
	return true;
}

int daEnPokeyHead_c::onDraw() {
	bodyModel.scheduleForDrawing();
	return true;
}


void daEnPokeyHead_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y - 8, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

int daEnPokeyHead_c::onExecute() {
	bodyModel._vf1C();
	updateModelMatrices();

	acState.execute();

	return true;
}