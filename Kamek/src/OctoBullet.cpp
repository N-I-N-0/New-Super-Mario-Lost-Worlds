#include <common.h>
#include <game.h>
#include <g3dhax.h>
#include <sfx.h>
#include <profile.h>

const char* TBarcNameList[] = { "octo", NULL };

class daTakoboBu : dEn_c {
	int onCreate();
	int onExecute();
	int onDraw();
	int onDelete();

	mHeapAllocator_c allocator;
	nw4r::g3d::ResFile resFile;
	m3d::mdl_c bodyModel;

	void updateModelMatrix();

	void killMe();
	void addScoreWhenHit(void* other);

	void playerCollision(ActivePhysics* apThis, ActivePhysics* apOther);
	void yoshiCollision(ActivePhysics* apThis, ActivePhysics* apOther);

	bool collisionCat3_StarPower(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat5_Mario(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCatD_Drill(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat8_FencePunch(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat7_GroundPound(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat7_GroundPoundYoshi(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCatA_PenguinMario(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat11_PipeCannon(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat9_RollingObject(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat1_Fireball_E_Explosion(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat13_Hammer(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat14_YoshiFire(ActivePhysics* apThis, ActivePhysics* apOther);

public: static dActor_c* build();
};

Profile OctoBulletProfile(&daTakoboBu::build, ProfileId::OctoBullet, NULL, ProfileId::OctoBullet, ProfileId::OctoBullet, "OctoBullet", TBarcNameList);

dActor_c* daTakoboBu::build() {
	void* buffer = AllocFromGameHeap1(sizeof(daTakoboBu));
	return new(buffer) daTakoboBu;
}

void daTakoboBu::addScoreWhenHit(void* other) {
}

void daTakoboBu::playerCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	char hitType;
	hitType = usedForDeterminingStatePress_or_playerCollision(this, apThis, apOther, 0);

	if (hitType == 1 || hitType == 3)	//normal jump or spin jump
	{
		apOther->someFlagByte |= 2;
		killMe();
	}
	else if (hitType == 0) {
		this->_vf220(apOther->owner);
		killMe();
	}

	//weird stuff
	deathInfo.isDead = 0;
	this->flags_4FC |= (1 << (31 - 7));
	this->counter_504[apOther->owner->which_player] = 0;
}

void daTakoboBu::yoshiCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->playerCollision(apThis, apOther);
}

bool daTakoboBu::collisionCat3_StarPower(ActivePhysics* apThis, ActivePhysics* apOther) {
	killMe();
	return true;
}

bool daTakoboBu::collisionCat5_Mario(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->_vf220(apOther->owner);
	return this->collisionCat3_StarPower(apThis, apOther);
}

bool daTakoboBu::collisionCatD_Drill(ActivePhysics* apThis, ActivePhysics* apOther) {
	return this->collisionCat3_StarPower(apThis, apOther);
}

bool daTakoboBu::collisionCat8_FencePunch(ActivePhysics* apThis, ActivePhysics* apOther) {
	playerCollision(apThis, apOther);
	return true;
}

bool daTakoboBu::collisionCat7_GroundPound(ActivePhysics* apThis, ActivePhysics* apOther) {
	return this->collisionCat3_StarPower(apThis, apOther);
}

bool daTakoboBu::collisionCat7_GroundPoundYoshi(ActivePhysics* apThis, ActivePhysics* apOther) {
	return this->collisionCat3_StarPower(apThis, apOther);
}

bool daTakoboBu::collisionCatA_PenguinMario(ActivePhysics* apThis, ActivePhysics* apOther) {
	return this->collisionCat5_Mario(apThis, apOther);
}

bool daTakoboBu::collisionCat11_PipeCannon(ActivePhysics* apThis, ActivePhysics* apOther) {
	return this->collisionCat3_StarPower(apThis, apOther);
}

bool daTakoboBu::collisionCat9_RollingObject(ActivePhysics* apThis, ActivePhysics* apOther) {
	return this->collisionCat3_StarPower(apThis, apOther);
}

bool daTakoboBu::collisionCat1_Fireball_E_Explosion(ActivePhysics* apThis, ActivePhysics* apOther) {
	return true;
}

bool daTakoboBu::collisionCat2_IceBall_15_YoshiIce(ActivePhysics* apThis, ActivePhysics* apOther) {
	return true;
}

bool daTakoboBu::collisionCat13_Hammer(ActivePhysics* apThis, ActivePhysics* apOther) {
	killMe();
	return true;
}

bool daTakoboBu::collisionCat14_YoshiFire(ActivePhysics* apThis, ActivePhysics* apOther) {
	return true;
}

void daTakoboBu::updateModelMatrix() {
	matrix.translation(pos.x - 1.0, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

void daTakoboBu::killMe() {
	PlaySound(this, SE_OBJ_ROCK_S_CRASH);
	SpawnEffect("Wm_ob_cmnboxsmoke", 0, &pos, &(S16Vec){0, 0, 0}, & (Vec){0.3, 0.3, 0.3});
	SpawnEffect("Wm_ob_cmnboxgrain", 0, &pos, &(S16Vec){0, 0, 0}, & (Vec){0.3, 0.3, 0.3});
	SpawnEffect("Wm_ob_cmnrocktoomba", 0, &pos, &(S16Vec){0, 0, 0}, & (Vec){1.0, 1.0, 1.0});
	this->removeMyActivePhysics();
	this->Delete(1);
}

int daTakoboBu::onCreate() {
	this->speed.x = (settings == 1) ? -1.8f : 1.8f;
	this->speed.y = 0.0;

	allocator.link(-1, GameHeaps[0], 0, 0x20);

	this->resFile.data = getResource("octo", "g3d/rock.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("kanibo_rock");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Enemy(&bodyModel, 0);

	allocator.unlink();

	ActivePhysics::Info physicsInfo;
	physicsInfo.xDistToCenter = 0.0;
	physicsInfo.yDistToCenter = 0.0;
	physicsInfo.xDistToEdge = 6.0;
	physicsInfo.yDistToEdge = 6.0;
	physicsInfo.category1 = 0x3;
	physicsInfo.category2 = 0x0;
	physicsInfo.bitfield1 = 0x4F; 
	physicsInfo.bitfield2 = 0xFFBAFFFE;
	physicsInfo.unkShort1C = 0;
	physicsInfo.callback = &dEn_c::collisionCallback;
	this->aPhysics.initWithStruct(this, &physicsInfo);
	this->aPhysics.addToList();

	spriteSomeRectX = 28.0f;
	spriteSomeRectY = 32.0f;
	_320 = 0.0f;
	_324 = 16.0f;

	static const lineSensor_s below(-2 << 12, 2 << 12, -6 << 12);
	static const pointSensor_s above(0 << 12, 6 << 12);
	static const lineSensor_s adjacent(-6 << 12, 6 << 12, 5 << 12);
	collMgr.init(this, &below, &above, &adjacent);
	collMgr.calculateBelowCollisionWithSmokeEffect();

	this->scale.x = 1.0;
	this->scale.y = 1.0;
	this->scale.z = 1.0;

	this->onExecute();

	return true;
}

int daTakoboBu::onExecute() {
	rot.x -= 0x800;
	updateModelMatrix();
	doSpriteMovement();

	collMgr.calculateAdjacentCollision();
	if (collMgr.outputMaybe) {
		killMe();
	}

	return true;
}

int daTakoboBu::onDelete() {
	return true;
}

int daTakoboBu::onDraw() {
	bodyModel.scheduleForDrawing();
	return true;
}