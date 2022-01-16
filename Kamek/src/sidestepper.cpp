#include <common.h>
#include <game.h>
#include <g3dhax.h>
#include <sfx.h>
#include <stage.h>
#include "boss.h"

const char* arc [] = 
{
	
	"test_lift",
	NULL
};

extern "C" bool SpawnEffect(const char*, int, Vec*, S16Vec*, Vec*);
extern "C" void destroyBarrel(dStageActor_c*);


class daSidestepper_c : public daBoss {
	// Initial setup [Declare variables here]
	
	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	mHeapAllocator_c allocator;
	m3d::mdl_c bodyModel;
	nw4r::g3d::ResFile resFile;
	m3d::anmChr_c chrAnimation;

	u64 eventFlag;
	u32 delay;

	u32 effect;
	u8 type;

	S16Vec nullRot;
	Vec efScale;

	dAc_Py_c* target;

	float BaseLine;
	int randomnum;
	int isBuried; //0 when not, 1 when yes
	char isInvulnerable;
	int startposx;
	bool left;
	bool right;
	bool fastwalkafterhit;
	bool everysecondtry;
	float rndmnum;
	int plusorminus; // 1-Plus, 2-Minus
	int rndmactor;

	dStageActor_c* spawner1;
	dStageActor_c* spawner2;

	int lives;


	Vec possand;
	Vec BackUpEffect;
	Vec posbarrel;


	static daSidestepper_c *build();

	void bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate);

	void updateModelMatrices();
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
	
	USING_STATES(daSidestepper_c);
	DECLARE_STATE(Grow);
	DECLARE_STATE(Wait);
	DECLARE_STATE(Walk);
	DECLARE_STATE(Bury);
	DECLARE_STATE(Projectiles);
	DECLARE_STATE(MoveSand);
	DECLARE_STATE(BackUp);
	DECLARE_STATE(Run);
	DECLARE_STATE(Outro);
};

CREATE_STATE(daSidestepper_c, Grow);
CREATE_STATE(daSidestepper_c, Wait);
CREATE_STATE(daSidestepper_c, Walk);
CREATE_STATE(daSidestepper_c, Bury);
CREATE_STATE(daSidestepper_c, Projectiles);
CREATE_STATE(daSidestepper_c, MoveSand);
CREATE_STATE(daSidestepper_c, BackUp);
CREATE_STATE(daSidestepper_c, Run);
CREATE_STATE(daSidestepper_c, Outro);

void daSidestepper_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate) {
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr(name);
	this->chrAnimation.bind(&this->bodyModel, anmChr, unk);
	this->bodyModel.bindAnim(&this->chrAnimation, unk2);
	this->chrAnimation.setUpdateRate(rate);
}


// Extra collision conditions:

void daSidestepper_c::playerCollision(ActivePhysics* apThis, ActivePhysics* apOther)
{
	DamagePlayer(this, apThis, apOther);
}

bool daSidestepper_c::collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther) {
	// When player collides using star
	// Call the default collision function, to not repeat the same code
    return true;
}
bool daSidestepper_c::collisionCat5_Mario(ActivePhysics *apThis, ActivePhysics *apOther) {
	// When player collides using butt slide
	playerCollision(apThis, apOther);
    return true;
}
bool daSidestepper_c::collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther) {
	// When player drills with propeller
	playerCollision(apThis, apOther);
    return true;
}
bool daSidestepper_c::collisionCat8_FencePunch(ActivePhysics *apThis, ActivePhysics *apOther) {
	// When player punches fence behind
	playerCollision(apThis, apOther);
    return true;
}
bool daSidestepper_c::collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther) {
	// When player ground pounds
	playerCollision(apThis, apOther);
    return true;
}
bool daSidestepper_c::collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther) {
	// When player ground pounds with Yoshi
	playerCollision(apThis, apOther);
    return true;
}
bool daSidestepper_c::collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther) {
	// When player slides with penguin suit
	playerCollision(apThis, apOther);
    return true;
}
bool daSidestepper_c::collisionCat11_PipeCannon(ActivePhysics *apThis, ActivePhysics *apOther) {
	// When player shoots from pipe cannon
	playerCollision(apThis, apOther);
    return true;
}
bool daSidestepper_c::collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther) {
	// When player throws object at sprite
	
	dActor_c* block = apOther->owner;
	dEn_c* blah = (dEn_c*)block;

	if (blah->direction == 0) { blah->direction = 1; }
	else { blah->direction = 0; }
	
	blah->speed.x = -blah->speed.x;
	blah->pos.x += blah->speed.x;

	if (blah->speed.y < 0) {
		blah->speed.y = -blah->speed.y;
	}

	blah->doSpriteMovement();
	blah->doSpriteMovement();

	this->pos.x -= blah->speed.x;

	Vec oneVec = { 1.0f, 1.0f, 1.0f };
	if (acState.getCurrentState() == &StateID_Run)
	{
		PlaySound(this, SE_SYS_ROUTE_NG);
		blah->Delete(1);
		SpawnEffect("Wm_mr_kickhit", 0, &blah->pos, &nullRot, &oneVec);
		return true;
	}


	PlaySound(this, SE_EMY_DOWN);
	
	SpawnEffect("Wm_mr_kickhit", 0, &blah->pos, &nullRot, &oneVec);

	this->damage += 5;

	if (spawner1 != NULL)
		destroyBarrel(spawner1);
	if (spawner2 != NULL)
		destroyBarrel(spawner2);


	if (this->damage > this->lives)
	{
		doStateChange(&StateID_Outro);
	}

	else { 
		this->fastwalkafterhit = true;
		doStateChange(&StateID_Bury); 
	}

	return true;
}
bool daSidestepper_c::collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther) {
	// When collides with fireball
    return true;
}
bool daSidestepper_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther) {
	// When collides with ice ball
    return true;
}
bool daSidestepper_c::collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther) {
	// When collides with hammer projectile
    return true;
}
bool daSidestepper_c::collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther) {
	// When collides with Yoshi fire spit
    return true;
}


daSidestepper_c *daSidestepper_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daSidestepper_c));
	return new(buffer) daSidestepper_c;
}


int daSidestepper_c::onCreate() {
	//Variables
	this->nullRot = (S16Vec){0, 0, 0};
	this->efScale = (Vec){2.0f, 1.0f, 1.0f};

	target = GetSpecificPlayerActor(0); //target is mario


	this->randomnum = 0;
	this->isBuried = 0; //0 when not, 1 when yes
	this->fastwalkafterhit = false;
	this->everysecondtry = false;
	
	
	OSReport("Test1");
	
	// Do the following once when object is spawned:

	allocator.link(-1, GameHeaps[0], 0, 0x20);

	// Model settings. First string is arc name, second string is brres name inside arc, third string is mdl0 name inside brres.
	this->resFile.data = getResource("test_lift", "g3d/test_lift.brres");

	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("test_lift");
	bodyModel.setup(mdl, &this->allocator, 0x224, 1, 0);
	SetupTextures_MapObj(&bodyModel, 0);

	nw4r::g3d::ResAnmChr anmChr = resFile.GetResAnmChr("wait");
	this->chrAnimation.setup(mdl, anmChr, &this->allocator, 0);


	allocator.unlink();

	OSReport("Test2");

	// Physics settings. Use hitbox debug mode to test hitbox, [Press "-" 16 times on star coin menu].
	ActivePhysics::Info Physics;
	Physics.xDistToCenter = 0.0;
	Physics.yDistToCenter = 0.0;
	Physics.xDistToEdge = 32.0;
	Physics.yDistToEdge = 53.0;
	Physics.category1 = 0x5;
	Physics.category2 = 0x0;
	Physics.bitfield1 = 0x4F;
	Physics.bitfield2 = 0xFFFFFFFF;
	Physics.unkShort1C = 0;
	Physics.callback = &dEn_c::collisionCallback;

	this->aPhysics.initWithStruct(this, &Physics);
	this->aPhysics.addToList();

	// Size settings

	this->scale.x = 0.1;
	this->scale.y = 0.1;
	this->scale.z = 0.1;

	//This make it Layer 0
	//this->pos.z = 3300.0; Cant use it because Bury state :(

	this->startposx = pos.x;
	this->BaseLine = pos.y;


	this->lives = 14;

	OSReport("Test3");
	doStateChange(&StateID_Grow);

	this->onExecute();
	return true;
}


int daSidestepper_c::onDelete() {
	// Do the following when deleted:
	
	return true;
}

int daSidestepper_c::onDraw() {
	bodyModel.scheduleForDrawing();
	return true;
}


void daSidestepper_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

int daSidestepper_c::onExecute() {
	// Do the following every frame:
	acState.execute();
	updateModelMatrices();
	bodyModel._vf1C();

	if (this->chrAnimation.isAnimationDone()) {
		this->chrAnimation.setCurrentFrame(0.0);
	}
	
	
	return true;
}

void daSidestepper_c::beginState_Grow()
{
	this->timer = 0;

	bindAnimChr_and_setUpdateRate("wait", 1, 0.0, 1.0);

	SetupKameck(this, Kameck);
}

void daSidestepper_c::executeState_Grow()
{
	this->timer += 1;

	bool ret;
	ret = GrowBoss(this, Kameck, 0.1, 0.4, 0, this->timer);

	//Grow Sound Test

	if (ret) {
		PlaySound(this, SE_EMY_MECHAKOOPA_BOUND);
		doStateChange(&StateID_Bury);
	}
	

}

void daSidestepper_c::endState_Grow()
{
	CleanupKameck(this, Kameck);
	this->BaseLine = this->pos.y;
}

void daSidestepper_c::beginState_Wait()
{
	this->timer = 0;
	bindAnimChr_and_setUpdateRate("wait", 1, 0.0, 1.0);
}

void daSidestepper_c::executeState_Wait()
{
	this->timer += 1;

	if (timer > 50)
	{
		if (this->isBuried == 1)
		{
			this->isBuried = 0;
			doStateChange(&StateID_Projectiles);
		}
		else
			doStateChange(&StateID_Walk);
	}
}

void daSidestepper_c::endState_Wait()
{ }

void daSidestepper_c::beginState_Walk()
{
	this->timer = 0;
	this->randomnum += 1;

	if (target->pos.x > pos.x)
	{
		bindAnimChr_and_setUpdateRate("walk_r", 1, 0.0, 1.0);
		this->right = true;
		this->left = false;
	}
	else if (target->pos.x < pos.x)
	{
		bindAnimChr_and_setUpdateRate("walk_l", 1, 0.0, 1.0);
		this->left = true;
		this->right = false;
	}
}

void daSidestepper_c::executeState_Walk()
{
	this->timer += 1;

	if (this->left == true)
		pos.x -= 1;
	else if (this->right == true)
		pos.x += 1;
	if (this->randomnum == 3)
	{
		doStateChange(&StateID_Bury);
		this->randomnum = 0;
	}
	if (this->timer > 50) 
		doStateChange(&StateID_Wait); 
}

void daSidestepper_c::endState_Walk()
{

}


void daSidestepper_c::beginState_Bury()
{
	bindAnimChr_and_setUpdateRate("wait", 1, 0.0, 1.0);

	this->timer = 0;

	PlaySound(this, 0x221);

	SpawnEffect("Wm_mr_sanddive_out", 0, &this->pos, &nullRot, &efScale);
	SpawnEffect("Wm_mr_sanddive_smk", 0, &this->pos, &nullRot, &efScale);
}

void daSidestepper_c::executeState_Bury()
{
	if (this->timer < 60) {
		this->pos.y -= 2.0;  // Height is 54 pixels, move down
	}

	if (this->timer > 90) {
		this->isBuried = 1;
		doStateChange(&StateID_Wait); //before StateID_Projectile
	}

	this->timer += 1;
}

void daSidestepper_c::endState_Bury()
{ }


//THIS IS SOOOOO HARD
void daSidestepper_c::beginState_Projectiles()
{
	this->timer = 0;

	//pos projectile
	rndmnum = GenerateRandomNumber(50); //pos from actor
	plusorminus = GenerateRandomNumber(2); //pos-plus or pos-minus

	switch (plusorminus)
	{
	case 0:
		this->posbarrel = (Vec){ pos.x + rndmnum, pos.y, 0 };
		break;
	case 1:
		this->posbarrel = (Vec){ pos.x - rndmnum, pos.y, 0 };
		break;
	default:
		OSReport("GenerateRandomnum(2) didn't work correctly! Deleting...");
		this->Delete(1);
		break;
	}

	rndmactor = GenerateRandomNumber(5);

	spawner1 = CreateActor(418, 0, this->posbarrel, 0, 0);



	//pos barrel
	rndmnum = GenerateRandomNumber(20);
	plusorminus = GenerateRandomNumber(2); //pos-plus or pos-minus

	switch (plusorminus)
	{
	case 0:
		this->posbarrel = (Vec){ pos.x + rndmnum, pos.y, 0 };
		break;
	case 1:
		this->posbarrel = (Vec){ pos.x - rndmnum, pos.y, 0 };
		break;
	default:
		OSReport("GenerateRandomnum(2) didn't work correctly! Deleting...");
		this->Delete(1);
		break;
	}

	spawner2 = CreateActor(418, 0, this->posbarrel, 0, 0);
}

void daSidestepper_c::executeState_Projectiles()
{

	if (this->timer < 60) {
		spawner1->pos.y += 2.0;  // Height is 54 pixels, move down
		spawner2->pos.y += 2.0;  // Height is 54 pixels, move down
	}

	if (this->timer > 90) {
		if (spawner1->pos.y != this->BaseLine || spawner2->pos.y != this->BaseLine)
		{
			spawner1->pos.y = this->BaseLine;
			spawner2->pos.y = this->BaseLine;
		}
		doStateChange(&StateID_MoveSand);
	}
	this->timer += 1;
}

void daSidestepper_c::endState_Projectiles()
{ }

void daSidestepper_c::beginState_MoveSand()
{


	bindAnimChr_and_setUpdateRate("wait", 1, 0.0, 1.0);
	this->timer = 0;

	this->possand = (Vec){ target->pos.x, this->BaseLine, 0 };

	PlaySound(this, 0x221);

	SpawnEffect("Wm_mr_sanddive_out", 0, &possand, &nullRot, &efScale);
	SpawnEffect("Wm_mr_sanddive_smk", 0, &possand, &nullRot, &efScale);

}

void daSidestepper_c::executeState_MoveSand()
{
	this->timer += 1;

	if (this->timer > 60)
	{
		pos.x = target->pos.x;
		doStateChange(&StateID_BackUp);
	}
}

void daSidestepper_c::endState_MoveSand()
{

}


void daSidestepper_c::beginState_BackUp()
{
	bindAnimChr_and_setUpdateRate("wait", 1, 0.0, 1.0);
	this->timer = 0;

	BackUpEffect = (Vec){ pos.x, this->BaseLine, 0 };

	PlaySound(this, 0x221);

	SpawnEffect("Wm_mr_sanddive_out", 0, &this->BackUpEffect, &nullRot, &efScale);
	SpawnEffect("Wm_mr_sanddive_smk", 0, &this->BackUpEffect, &nullRot, &efScale);
}

void daSidestepper_c::executeState_BackUp()
{
	if (this->timer < 60) {
		this->pos.y += 2.0;  // Height is 54 pixels, move down
	}

	if (this->timer > 90) {
		if (pos.y != this->BaseLine)
		{
			pos.y = this->BaseLine;
		}
		
		if (this->fastwalkafterhit)
		{
			if (lives == 14)
			{
				if (damage > 9)
					doStateChange(&StateID_Run);
				else
					this->fastwalkafterhit = false;
					doStateChange(&StateID_Run);
			}
			else if (lives == 24)
			{
				if (damage > 19)
					doStateChange(&StateID_Run);
				else
					this->fastwalkafterhit = false;
					doStateChange(&StateID_Run);
			}
		}
		else
			doStateChange(&StateID_Walk);
	}

	this->timer += 1;
}

void daSidestepper_c::endState_BackUp()
{ }



void daSidestepper_c::beginState_Run()
{


	this->timer = 0;

	//Kamek testing

	
	//here

	if (target->pos.x > pos.x)
	{
		bindAnimChr_and_setUpdateRate("walk_r", 1, 0.0, 1.0);
		this->right = true;
		this->left = false;
	}
	else if (target->pos.x < pos.x)
	{
		bindAnimChr_and_setUpdateRate("walk_l", 1, 0.0, 1.0);
		this->left = true;
		this->right = false;
	}
}

void daSidestepper_c::executeState_Run()
{
	
	this->timer += 1;

	if (this->right == true)
	{
		if (this->timer < 30)
			SpawnEffect("Wm_mr_sanddive_smk", 0, &this->pos, &nullRot, &efScale);

		if (this->timer >= 30)
		{
			PlaySound(this, SE_OBJ_TEKKYU_G_CRASH);
			pos.x += 3;

			if (everysecondtry == true) {
				SpawnEffect("Wm_mr_sanddive_smk", 0, &this->pos, &nullRot, &efScale);
				everysecondtry = false;
			}
			if (everysecondtry == false) {
				everysecondtry = true;
			}
		}
		
	}
	else if (this->left == true)
	{
		if (this->timer < 30)
			SpawnEffect("Wm_mr_sanddive_smk", 0, &this->pos, &nullRot, &efScale);

		if (this->timer >= 30)
		{
			PlaySound(this, SE_OBJ_TEKKYU_G_CRASH);
			pos.x += 3;

			if (everysecondtry == true) {
				SpawnEffect("Wm_mr_sanddive_smk", 0, &this->pos, &nullRot, &efScale);
				everysecondtry = false;
			}
			if (everysecondtry == false) {
				everysecondtry = true;
			}
		}
	}

	if (this->timer > 200)
	{
		doStateChange(&StateID_Bury);
	}
}

void daSidestepper_c::endState_Run()
{ }

void daSidestepper_c::beginState_Outro()
{
	this->timer = 0;
	bindAnimChr_and_setUpdateRate("wait", 1, 0.0, 1.0);
	OutroSetup(this);
}

void daSidestepper_c::executeState_Outro()
{
	if (this->dying == 1) {
		if (this->timer > 180) {
			ExitStage(WORLD_MAP, 0, BEAT_LEVEL, MARIO_WIPE);
		}
		if (this->timer == 60) { PlayerVictoryCries(this); }

		this->timer += 1;
		return;
	}

	bool ret;
	ret = ShrinkBoss(this, &this->pos, 0.6, this->timer);

	if (ret == true) {
		BossExplode(this, &this->pos);
		this->dying = 1;
		this->timer = 0;
	}

	this->timer += 1;
}

void daSidestepper_c::endState_Outro() { }