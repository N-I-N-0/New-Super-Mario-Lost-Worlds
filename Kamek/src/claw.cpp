#include <profile.h>
#include <game.h>
#include <sfx.h>

const char* ClawANL [] = { "test_lift", NULL };

class daClaw : public dEn_c {
public:

	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	void updateModelMatrices();
	void bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate);

	static dActor_c* build();

	void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther);

	bool collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat5_Mario(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat8_FencePunch(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat11_PipeCannon(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther);


	int timer;
	bool leftorright;
	dStageActor_c* sidestepper;
	dAc_Py_c* target;

	mHeapAllocator_c allocator;
	m3d::mdl_c bodyModel;
	m3d::anmChr_c animationChr;
	nw4r::g3d::ResFile resFile;

	USING_STATES(daClaw);
	DECLARE_STATE(Appear);
	DECLARE_STATE(Attack);
	DECLARE_STATE(Disappear);
};

CREATE_STATE(daClaw, Appear);
CREATE_STATE(daClaw, Attack);
CREATE_STATE(daClaw, Disappear);

void daClaw::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}


void daClaw::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate) {
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr(name);
	this->animationChr.bind(&this->bodyModel, anmChr, unk);
	this->bodyModel.bindAnim(&this->animationChr, unk2);
	this->animationChr.setUpdateRate(rate);
}

dActor_c* daClaw::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daClaw));
	return new(buffer) daClaw;
}

const SpriteData ClawSpriteData = { ProfileId::Claw, 0, 0, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };

Profile ClawProfile(&daClaw::build, SpriteId::Claw, &ClawSpriteData, ProfileId::Claw, ProfileId::Claw, "Claw", ClawANL);



void daClaw::playerCollision(ActivePhysics* apThis, ActivePhysics* apOther)
{
	this->_vf220(apOther->owner);
}

bool daClaw::collisionCat3_StarPower(ActivePhysics* apThis, ActivePhysics* apOther)
{
	return true;
}
bool daClaw::collisionCat5_Mario(ActivePhysics* apThis, ActivePhysics* apOther)
{
	return true;
}
bool daClaw::collisionCatD_Drill(ActivePhysics* apThis, ActivePhysics* apOther)
{
	return true;
}
bool daClaw::collisionCat8_FencePunch(ActivePhysics* apThis, ActivePhysics* apOther)
{
	return true;
}
bool daClaw::collisionCat7_GroundPound(ActivePhysics* apThis, ActivePhysics* apOther)
{
	return true;
}
bool daClaw::collisionCat7_GroundPoundYoshi(ActivePhysics* apThis, ActivePhysics* apOther)
{
	return true;
}
bool daClaw::collisionCatA_PenguinMario(ActivePhysics* apThis, ActivePhysics* apOther)
{
	return true;
}
bool daClaw::collisionCat11_PipeCannon(ActivePhysics* apThis, ActivePhysics* apOther)
{
	return true;
}
bool daClaw::collisionCat9_RollingObject(ActivePhysics* apThis, ActivePhysics* apOther)
{
	return true;
}
bool daClaw::collisionCat1_Fireball_E_Explosion(ActivePhysics* apThis, ActivePhysics* apOther)
{
	return true;
}
bool daClaw::collisionCat2_IceBall_15_YoshiIce(ActivePhysics* apThis, ActivePhysics* apOther)
{
	return true;
}
bool daClaw::collisionCat13_Hammer(ActivePhysics* apThis, ActivePhysics* apOther)
{
	return true;
}
bool daClaw::collisionCat14_YoshiFire(ActivePhysics* apThis, ActivePhysics* apOther)
{
	return true;
}




int daClaw::onCreate() {


	target = GetSpecificPlayerActor(0); //target is mario



	allocator.link(-1, GameHeaps[0], 0, 0x20);

/*
	this->resFile.data = getResource("test_lift", "g3d/test_lift.brres");

	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("test_lift");
	bodyModel.setup(mdl, &this->allocator, 0x224, 1, 0);
	SetupTextures_MapObj(&bodyModel, 0);

	nw4r::g3d::ResAnmChr anmChr = resFile.GetResAnmChr("wait");
	this->chrAnimation.setup(mdl, anmChr, &this->allocator, 0);
*/
	resFile.data = getResource("test_lift", "g3d/test_lift.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("test_lift");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Boss(&bodyModel, 0);
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr("clawAppear");
	this->animationChr.setup(mdl, anmChr, &this->allocator, 0);

	allocator.unlink(); 

	ActivePhysics::Info HitMeBaby; 
	HitMeBaby.xDistToCenter = 0.0; 
	HitMeBaby.yDistToCenter = 0.0; 
	HitMeBaby.xDistToEdge = 2.0; 
	HitMeBaby.yDistToEdge = 2.0; 
	HitMeBaby.category1 = 0x3; 
	HitMeBaby.category2 = 0x0; 
	HitMeBaby.bitfield1 = 0x4F; 
	HitMeBaby.bitfield2 = 0xFFFFFFFF; 
	HitMeBaby.unkShort1C = 0; 
	HitMeBaby.callback = &dEn_c::collisionCallback; 
	this->aPhysics.initWithStruct(this, &HitMeBaby); 
	this->aPhysics.addToList(); 

	this->scale.x = 0.4; 
	this->scale.y = 0.4; 
	this->scale.z = 0.4; 


	//sidestepper = (dStageActor_c*)fBase_c::search(779);

	//this->pos = sidestepper->pos;
	this->pos.z = -1000;

	bindAnimChr_and_setUpdateRate("clawAppear", 1, 0.0, 1.0);

	doStateChange(&StateID_Appear);



	this->onExecute();

	return true;
}

int daClaw::onExecute() {
	acState.execute();

	updateModelMatrices();
	bodyModel._vf1C();

	if (this->animationChr.isAnimationDone()) {
		this->animationChr.setCurrentFrame(0.0);
	}

	return true;
}

int daClaw::onDelete() {
	return true;
}

int daClaw::onDraw() {
	bodyModel.scheduleForDrawing();

	return true;
}

void daClaw::beginState_Appear() {

	timer = 0;

	bindAnimChr_and_setUpdateRate("clawAppear", 1, 0.0, 1.0);

	//this->pos.x = sidestepper->pos.x;
	this->pos.y = pos.y + 100.0;
}
void daClaw::executeState_Appear() {
	if (timer == 80) {
		doStateChange(&StateID_Attack);
	}

	timer++;
}
void daClaw::endState_Appear() { }

void daClaw::beginState_Attack() {
	timer = 0;

	bindAnimChr_and_setUpdateRate("clawAttack", 1, 0.0, 1.0);

	if (target->pos.x < this->pos.x) //player is left
	{
		this->leftorright = false; //false = left
	}
	else //player is right
	{
		this->leftorright = true; //true = right
	}
}
void daClaw::executeState_Attack() {
	if (timer < 180)
	{
		if (leftorright)
		{
			pos.x += 2.0;
		}
		else
		{
			pos.x -= 2.0;
		}
	}
	else
	{
		doStateChange(&StateID_Disappear);
	}

	timer++;
}
void daClaw::endState_Attack() { }

void daClaw::beginState_Disappear() {
	timer = 0;

	bindAnimChr_and_setUpdateRate("clawDisappear", 1, 0.0, 1.0);
}
void daClaw::executeState_Disappear() {
	if (timer == 80)
	{
		this->Delete(1);
	}

	timer++;
}
void daClaw::endState_Disappear() { }

