// #include <common.h>
// #include <game.h>
// #include <g3dhax.h>
// #include <daYoshi_c.h>
// #include "yoshiFly.h"

// extern int getNybbleValue(u32 settings, int fromNybble, int toNybble);

// dYoshiWingsRenderer_c *dYoshiWingsRenderer_c::build() {
// 	return new dYoshiWingsRenderer_c;
// }

// dYoshiWingsRenderer_c::dYoshiWingsRenderer_c() { } dYoshiWingsRenderer_c::~dYoshiWingsRenderer_c() {
// }

// void dYoshiWingsRenderer_c::setup(dPlayerModelHandler_c *handler) {
// 	setup(handler, 0);
// }

// void dYoshiWingsRenderer_c::setup(dPlayerModelHandler_c *handler, int sceneID) {
// 	OSReport("Starting setup\n");
// 	yoshi = (dPlayerModel_c*)handler->mdlClass;

// 	allocator.link(-1, GameHeaps[0], 0, 0x20);

// 	nw4r::g3d::ResFile rf(getResource("Y_TexGreen", "g3d/wing.brres"));

// 	nw4r::g3d::ResMdl rm = rf.GetResMdl("wing");
// 	wings.setup(rm, &allocator, 0, 1, 0);
// 	SetupTextures_Enemy(&wings, sceneID);

// 	allocator.unlink();


// 	yoshiModel = &yoshi->models[0].body;
// 	nw4r::g3d::ResMdl *yoshiResMdl =
// 		(nw4r::g3d::ResMdl*)(((u32)yoshiModel->scnObj) + 0xE8);


// 	nw4r::g3d::ResNode spin = yoshiResMdl->GetResNode("spin");
// 	spinNodeID = spin.GetID();
// 	OSReport("Ending setup\n");
// }

// void dYoshiWingsRenderer_c::draw() {
// 	OSReport("Starting draw\n");
// 	daYoshi_c *CuteYoshi = dAcPy_c::findByID(yoshi->player_id_1)->getYoshi();
// 	if (!(getNybbleValue(CuteYoshi->settings, 12, 12) == 4 && getNybbleValue(CuteYoshi->settings, 5, 5) == 1))
// 		return;

// 	Mtx rootMtx;
// 	yoshiModel->getMatrixForNode(spinNodeID, &rootMtx);

// 	wings.setDrawMatrix(&rootMtx);
// 	wings.setScale(1.0f, 1.0f, 1.0f);
// 	wings.calcWorld(false);

// 	wings.scheduleForDrawing();
// 	OSReport("Ending draw\n");
// }



#include <common.h>
#include <game.h>
#include <profile.h>
#include <daYoshi_c.h>

const char *YoshiWingsFileList[] = {"Y_TexGreen", 0};


class daEnYoshiWings_c : public dEn_c {
public:
	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	mHeapAllocator_c allocator;
	nw4r::g3d::ResFile resFile;
	m3d::mdl_c bodyModel;
	m3d::anmChr_c animationChr;

	daYoshi_c *CuteYoshi;

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
};

const SpriteData YoshiWingsSpriteData = { ProfileId::EN_YOSHIWINGS, 0, 0, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile YoshiWingsProfile(&daEnYoshiWings_c::build, SpriteId::EN_YOSHIWINGS, &YoshiWingsSpriteData, ProfileId::EN_YOSHIWINGS, ProfileId::EN_YOSHIWINGS, "daEnYoshiWings_c", YoshiWingsFileList);



u8 hijackMusicWithSongName(const char *songName, int themeID, bool hasFast, int channelCount, int trackCount, int *wantRealStreamID);

void daEnYoshiWings_c::playerCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
}
void daEnYoshiWings_c::yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
}
bool daEnYoshiWings_c::collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther) {
	return false;
}
bool daEnYoshiWings_c::collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther) {
	return false;
}
bool daEnYoshiWings_c::collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther) {
	return false;
}
bool daEnYoshiWings_c::collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther) {
	return false;
}

bool daEnYoshiWings_c::collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther) {
	return false;
}
bool daEnYoshiWings_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther) {

	return false;
}
bool daEnYoshiWings_c::collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther) {

	return false;
}
bool daEnYoshiWings_c::collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther) {
	return false;
}
bool daEnYoshiWings_c::collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther) {
	return false;
}

bool daEnYoshiWings_c::collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther) {
	return false;
}


dActor_c *daEnYoshiWings_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daEnYoshiWings_c));
	daEnYoshiWings_c *c = new(buffer) daEnYoshiWings_c;

	return c;
}


extern int getNybbleValue(u32 settings, int fromNybble, int toNybble);

int daEnYoshiWings_c::onCreate() {
	this->deleteForever = true;

	// Model creation	
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	this->resFile.data = getResource("Y_TexGreen", "g3d/wing.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("wing");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Enemy(&bodyModel, 0);

	bool ret;
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr("wing_pata");
	ret = this->animationChr.setup(mdl, anmChr, &this->allocator, 0);
	this->animationChr.bind(&this->bodyModel, anmChr, 1);
	this->bodyModel.bindAnim(&this->animationChr, 0);
	this->animationChr.setUpdateRate(1.0);

	allocator.unlink();

	// ActivePhysics::Info HitMeBaby;

	// HitMeBaby.xDistToCenter = 0.0;
	// HitMeBaby.yDistToCenter = 0.0;

	// HitMeBaby.xDistToEdge = 7.5;
	// HitMeBaby.yDistToEdge = 7.5;

	// HitMeBaby.category1 = 0x3;
	// HitMeBaby.category2 = 0x0;
	// HitMeBaby.bitfield1 = 0x6F;
	// HitMeBaby.bitfield2 = 0xffbafffe;
	// HitMeBaby.unkShort1C = 0;
	// HitMeBaby.callback = &dEn_c::collisionCallback;

	// this->aPhysics.initWithStruct(this, &HitMeBaby);
	// this->aPhysics.addToList();

	// Stuff I do understand
	CuteYoshi = (daYoshi_c *)Actor_SearchByID(this->settings);

	this->scale = (Vec){1.0, 1.0, 1.0};

	this->rot.x = 0;
	this->rot.y = 0;
	this->rot.z = 0;

	this->pos.z = 4000;


	this->onExecute();
	return true;
}


int daEnYoshiWings_c::onDelete() {
	return true;
}

int daEnYoshiWings_c::onDraw() {
	bodyModel.scheduleForDrawing();
	return true;
}

// extern nw4r::g3d::ResMdl *yoshiResMDL;
void daEnYoshiWings_c::updateModelMatrices() {
	// dPlayerModel_c *yoshi = (dPlayerModel_c*)(CuteYoshi->modelhandler->mdlClass);
	// m3d::mdl_c *yoshiModel = &yoshi->models[0].body;
	// // nw4r::g3d::ResMdl *yoshiResMdl =
	// // 	(nw4r::g3d::ResMdl*)(((u32)yoshiModel->scnObj) + 0xE8);


	// nw4r::g3d::ResNode spin = yoshiResMDL->GetResNode("skl_root");
	// u32 spinNodeID = spin.GetID();

	// Mtx rootMtx;
	// yoshiModel->getMatrixForNode(spinNodeID, &rootMtx);

	matrix.translation(CuteYoshi->pos.x + ((CuteYoshi->direction) ? -4 : 4), CuteYoshi->pos.y + 11, CuteYoshi->pos.z + 5);
	matrix.applyRotationYXZ(&CuteYoshi->rot.x, &CuteYoshi->rot.y, &CuteYoshi->rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);

}

int daEnYoshiWings_c::onExecute() {
	bodyModel._vf1C();
	updateModelMatrices();
	if(CuteYoshi->input.getHeldTwo()) {
		this->animationChr.setUpdateRate(1.0);
	}
	else {
		this->animationChr.setUpdateRate(0.0);
	}

	if(this->animationChr.isAnimationDone()) {
		this->animationChr.setCurrentFrame(0.0);
	}

	return true;
}