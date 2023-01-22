#include "itemBowling.h"

const char* IBarcNameList [] = {
	"item_balloon",
	NULL
};

class dItemBalloon_c : public dEn_c {
	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	mHeapAllocator_c allocator;
	m3d::mdl_c bodyModel;

	m3d::anmChr_c chrAnimation;
	m3d::anmTexPat_c patAnimation;

	u8 type;

	static dItemBalloon_c *build();

	void updateModelMatrices();
	void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther);

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


void dItemBalloon_c::playerCollision(ActivePhysics *apThis, ActivePhysics *apOther) {

}

bool dItemBalloon_c::collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther) {
	return false;
}
bool dItemBalloon_c::collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther) {
	return false;
}
bool dItemBalloon_c::collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther) {
	return false;
}
bool dItemBalloon_c::collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther) {
	return false;
}

bool dItemBalloon_c::collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther) {
	return false;
}
bool dItemBalloon_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther) {
	return false;
}
bool dItemBalloon_c::collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther) {
	MapSoundPlayer(SoundRelatedClass, 0x245 + dItemBowlingMgr_c::instance->totalItemCount, 1);

	char effectName[20];
	snprintf(effectName, 20, "Wm_ob_itemballoon%02d", this->type);
	OSReport("effect: %s\n", effectName);
	Vec effectPos = (Vec){this->pos.x, this->pos.y - 16, this->pos.z};
	SpawnEffect(effectName, 0, &effectPos, &this->rot, &this->scale);

	dItemBowlingMgr_c::instance->wonItemsCount[this->type]++;
	dItemBowlingMgr_c::instance->totalItemCount++;
	this->removeMyActivePhysics();

	this->Delete(1);

	return false;
}
bool dItemBalloon_c::collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther) {
	return false;
}
bool dItemBalloon_c::collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther) {
	return false;
}

bool dItemBalloon_c::collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther) {
	return false;
}



dItemBalloon_c *dItemBalloon_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dItemBalloon_c));
	return new(buffer) dItemBalloon_c;
}


int dItemBalloon_c::onCreate() {
	this->type = this->settings & 0xF;

	allocator.link(-1, GameHeaps[0], 0, 0x20);

	nw4r::g3d::ResFile rf(getResource("item_balloon", "g3d/item_balloon.brres"));
	nw4r::g3d::ResMdl mdl = rf.GetResMdl("item_balloon");
	bodyModel.setup(mdl, &allocator, 0x227, 1, 0);
	SetupTextures_MapObj(&bodyModel, 0);


	nw4r::g3d::ResAnmChr anmChr = rf.GetResAnmChr("wait");
	this->chrAnimation.setup(mdl, anmChr, &this->allocator, 0);
	this->chrAnimation.bind(&this->bodyModel, anmChr, 1);
	this->bodyModel.bindAnim(&this->chrAnimation, 0.0);
	this->chrAnimation.setUpdateRate(1.0f);


	nw4r::g3d::ResAnmTexPat anmPat = rf.GetResAnmTexPat("wait");
	this->patAnimation.setup(mdl, anmPat, &this->allocator, 0, 1);
	this->patAnimation.bindEntry(&this->bodyModel, &anmPat, 0, 0);
	this->bodyModel.bindAnim(&this->patAnimation, 0.0);
	this->patAnimation.setCurrentFrame((float)this->type);

	allocator.unlink();

	ActivePhysics::Info HitMeBaby;
	HitMeBaby.xDistToCenter = 0.0;
	HitMeBaby.yDistToCenter = 0.0;
	HitMeBaby.xDistToEdge = 14.0;
	HitMeBaby.yDistToEdge = 14.0;
	HitMeBaby.category = 0x5;
	HitMeBaby.attack = 0x0;
	HitMeBaby.categoryBitfield = 0x4F;
	HitMeBaby.attackBitfield = 0xFFFFFFFF;
	HitMeBaby.miscFlags = 0;
	HitMeBaby.callback = &dEn_c::collisionCallback;

	this->aPhysics.initWithStruct(this, &HitMeBaby);
	this->aPhysics.addToList();

	this->scale = (Vec){1.5f, 1.5f, 1.5f};
	this->disableFlagMask = 0;

	this->onExecute();
	return true;
}


int dItemBalloon_c::onDelete() {
	return true;
}

int dItemBalloon_c::onDraw() {
	bodyModel.scheduleForDrawing();
	return true;
}


void dItemBalloon_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);

	bodyModel._vf1C();
}

int dItemBalloon_c::onExecute() {
	updateModelMatrices();

	if(this->chrAnimation.isAnimationDone()) 
		this->chrAnimation.setCurrentFrame(0.0);

	return true;
}

