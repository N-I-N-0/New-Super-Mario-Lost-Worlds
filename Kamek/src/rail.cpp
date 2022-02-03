#include <common.h>
#include <game.h>
#include <profile.h>
#include "baddy.h"

const char* RailFileList[] = { "rail", 0 };


class daEnRail_c : public dEn_c {
public:
	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	/*mHeapAllocator_c allocator;
	nw4r::g3d::ResFile resFile;
	m3d::mdl_c bodyModel;*/

	static dActor_c* build();


	int breite;
	int hoehe;
	int type;


	//void updateModelMatrices();
	void playerCollision(ActivePhysics* apThis, ActivePhysics* apOther);
	void yoshiCollision(ActivePhysics* apThis, ActivePhysics* apOther);

	bool collisionCat7_GroundPound(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat7_GroundPoundYoshi(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCatD_Drill(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCatA_PenguinMario(ActivePhysics* apThis, ActivePhysics* apOther);

	bool collisionCat1_Fireball_E_Explosion(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat9_RollingObject(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat13_Hammer(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat14_YoshiFire(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat3_StarPower(ActivePhysics* apThis, ActivePhysics* apOther);
};

const SpriteData RailSpriteData = { ProfileId::Rail, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile RailProfile(&daEnRail_c::build, SpriteId::Rail, &RailSpriteData, ProfileId::Rail, ProfileId::Rail, "Rail", RailFileList);



u8 hijackMusicWithSongName(const char* songName, int themeID, bool hasFast, int channelCount, int trackCount, int* wantRealStreamID);

void daEnRail_c::playerCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	switch (type)
	{
	case 0:
		if (playerStatus[apOther->owner->which_player] == 2)
		{
			playerCart[apOther->owner->which_player]->done = true;
		}
		break;
	case 1:
		break;
	case 2:
		if (playerStatus[apOther->owner->which_player] == 2)
		{
			playerCart[apOther->owner->which_player]->moveable = false;
		}
		break;
	case 3:
		if (playerStatus[apOther->owner->which_player] == 2)
		{
			playerCart[apOther->owner->which_player]->moveable = true;
		}
		break;
	default:
		break;
	}
}
void daEnRail_c::yoshiCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->playerCollision(apThis, apOther);
}
bool daEnRail_c::collisionCat7_GroundPound(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnRail_c::collisionCat7_GroundPoundYoshi(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnRail_c::collisionCatD_Drill(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnRail_c::collisionCatA_PenguinMario(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}

bool daEnRail_c::collisionCat1_Fireball_E_Explosion(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnRail_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics* apThis, ActivePhysics* apOther) {

	return false;
}
bool daEnRail_c::collisionCat9_RollingObject(ActivePhysics* apThis, ActivePhysics* apOther) {

	return false;
}
bool daEnRail_c::collisionCat13_Hammer(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnRail_c::collisionCat14_YoshiFire(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}

bool daEnRail_c::collisionCat3_StarPower(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}


dActor_c* daEnRail_c::build() {
	void* buffer = AllocFromGameHeap1(sizeof(daEnRail_c));
	daEnRail_c* c = new(buffer) daEnRail_c;

	return c;
}


extern int getNybbleValue(u32 settings, int fromNybble, int toNybble);

int daEnRail_c::onCreate() {
	this->deleteForever = true;

	// Model creation	
	/*allocator.link(-1, GameHeaps[0], 0, 0x20);

	this->resFile.data = getResource("rail", "g3d/rail.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("Rail");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Player(&bodyModel, 0);

	allocator.unlink();*/


	this->breite = this->settings >> 23 & 0b11111111;					
	this->hoehe = this->settings >> 15 & 0b11111111;							
	this->type = this->settings >> 0 & 0b1111111111111111;

	OSReport("Type: %d\n", this->type);


	ActivePhysics::Info HitMeBaby;

	HitMeBaby.xDistToCenter = 0.0;
	HitMeBaby.yDistToCenter = 0.0;
	HitMeBaby.xDistToEdge = breite;
	HitMeBaby.yDistToEdge = hoehe;

	HitMeBaby.category1 = 0x3;
	HitMeBaby.category2 = 0x0;
	HitMeBaby.bitfield1 = 0x4F;
	HitMeBaby.bitfield2 = 0x8028E;
	HitMeBaby.unkShort1C = 0;
	HitMeBaby.callback = &dEn_c::collisionCallback;

	this->aPhysics.initWithStruct(this, &HitMeBaby);
	this->aPhysics.addToList();

	// Stuff I do understand
	this->scale = (Vec){ 20.0, 20.0, 20.0 };

	this->rot.x = 0; // X is vertical axis
	this->rot.y = 0; // Y is horizontal axis
	this->rot.z = 0; // Z is ... an axis >.>


	this->onExecute();
	return true;
}


int daEnRail_c::onDelete() {
	return true;
}

int daEnRail_c::onDraw() {
	//bodyModel.scheduleForDrawing();
	return true;
}


/*void daEnRail_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}*/

int daEnRail_c::onExecute() {
	//bodyModel._vf1C();
	//updateModelMatrices();

	return true;
}