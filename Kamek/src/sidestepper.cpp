#include <common.h>
#include <game.h>
#include <g3dhax.h>
#include <sfx.h>
#include <stage.h>
#include <profile.h>
#include "boss.h"
#include "liq.h"

extern "C" void daIce_c__breakReq(fBase_c* ice, int directionMaybe, int playerID);

#define NUMBER_OF_MINIS 3

const char* SidestepperArcNameList [] = 
{
	"sidestepper",
	"block_taru",
	"kanibo",
	NULL
};


class daMiniSidestepper_c : public dEn_c {
public:

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
	int startposx;
	bool left;
	bool right;
	bool fastwalkafterhit;
	bool everysecondtry;
	float rndmnum;
	int plusorminus; // 0-Plus, 1-Minus
	int plusorminusrock; // 0-Plus, 1-Minus
	int rndmactor;
	int buryprojectiletimer;
	bool everysecondtry2;
	int walkwaitwalk; //for wait 
	bool left2;
	bool right2;
	int timerock;
	int rndmtimerock;
	bool morelives;
	float point0;
	float point1;
	float point2;
	int howmanypoints; //Used for the BackUp State
	int whichpoint;
	float distbetweenpoints;
	bool bossFlag;
	
	int timer;

	int lives;


	Vec possand;
	Vec BackUpEffect;
	Vec posbarrel;
	Vec posenemy;
	Vec posrock;
	Vec barreleffect;
	Vec enemyeffect;


	static dActor_c* build();

	void bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate);

	void _vf148();
	void _vf14C();
	bool CreateIceActors();

	void updateModelMatrices();
	void playerCollision(ActivePhysics* apThis, ActivePhysics* apOther);

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
	//bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat13_Hammer(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat14_YoshiFire(ActivePhysics* apThis, ActivePhysics* apOther);

	USING_STATES(daMiniSidestepper_c);
	DECLARE_STATE(Wait);
	DECLARE_STATE(Walk);
	DECLARE_STATE(Bury);
	DECLARE_STATE(MoveSand);
	DECLARE_STATE(BackUp);
	DECLARE_STATE(Outro);
};

CREATE_STATE(daMiniSidestepper_c, Wait);
CREATE_STATE(daMiniSidestepper_c, Walk);
CREATE_STATE(daMiniSidestepper_c, Bury);
CREATE_STATE(daMiniSidestepper_c, MoveSand);
CREATE_STATE(daMiniSidestepper_c, BackUp);
CREATE_STATE(daMiniSidestepper_c, Outro);

class daSidestepper_c : public daBoss {
public:
	
	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	mHeapAllocator_c allocator;
	m3d::mdl_c bodyModel;
	nw4r::g3d::ResFile resFile;
	m3d::anmChr_c animationChr;

	u64 eventFlag;
	u32 delay;

	u32 effect;
	u8 type;

	S16Vec nullRot;
	Vec efScale;

	//
	m3d::anmTexSrt_c body;

	m3d::mdl_c fogModel;
	m3d::anmTexSrt_c fogSRT;
	//


	dAc_Py_c* target;

	float BaseLine;
	int randomnum;
	int isBuried; //0 when not, 1 when yes
	float startposx;
	float stagemiddle;
	bool left;
	bool right;
	bool fastwalkafterhit;
	bool everysecondtry;
	float rndmnum;
	int plusorminus; // 0-Plus, 1-Minus
	int plusorminusrock; // 0-Plus, 1-Minus
	int rndmactor;
	int buryprojectiletimer;
	bool everysecondtry2;
	int walkwaitwalk; //for wait 
	bool left2;
	bool right2;
	int timerock;
	int rndmtimerock;
	bool round0;
	bool round1;
	bool rockisleft;
	u32 barrelid;
	int barrelisthere;
	int rndmnumms;
	int rndmspeedr;


	dStageActor_c* barrel;
	daMiniSidestepper_c* enemy[NUMBER_OF_MINIS];
	dStageActor_c* rock;
	dStageActor_c* claw;

	nw4r::snd::SoundHandle* handle;

	int lives;


	Vec possand;
	Vec BackUpEffect;
	Vec posbarrel;
	Vec posenemy;
	Vec posrock;
	Vec barreleffect;
	Vec enemyeffect;


	static dActor_c *build();

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
	DECLARE_STATE(Hit);
	DECLARE_STATE(Wait);
	DECLARE_STATE(Walk);
	DECLARE_STATE(Projectiles);
	DECLARE_STATE(Bury);
	DECLARE_STATE(Actors);
	DECLARE_STATE(MoveSand);
	DECLARE_STATE(BackUp);
	DECLARE_STATE(Run);
	DECLARE_STATE(Claw);
	DECLARE_STATE(Outro);
};

CREATE_STATE(daSidestepper_c, Grow);
CREATE_STATE(daSidestepper_c, Hit);
CREATE_STATE(daSidestepper_c, Wait);
CREATE_STATE(daSidestepper_c, Walk);
CREATE_STATE(daSidestepper_c, Projectiles);
CREATE_STATE(daSidestepper_c, Bury);
CREATE_STATE(daSidestepper_c, Actors);
CREATE_STATE(daSidestepper_c, MoveSand);
CREATE_STATE(daSidestepper_c, BackUp);
CREATE_STATE(daSidestepper_c, Run);
CREATE_STATE(daSidestepper_c, Claw);
CREATE_STATE(daSidestepper_c, Outro);

void daSidestepper_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate) {
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr(name);
	this->animationChr.bind(&this->bodyModel, anmChr, unk);
	this->bodyModel.bindAnim(&this->animationChr, unk2);
	this->animationChr.setUpdateRate(rate);
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

	if (blah->name == ICE_ACTOR)
	{
		daIce_c__breakReq(blah, 0, -1);
		return true;
	}

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
	if (acState.getCurrentState() == &StateID_Bury || acState.getCurrentState() == &StateID_BackUp || acState.getCurrentState() == &StateID_Hit)
	{
		if (apOther->owner->name != BLOCK_TARU) { return true; }
		destroyBarrel(blah);
		return true;
	}

	PlaySound(this, SE_EMY_DOWN);

	PlaySound(this, SE_EMY_BLOW_PAKKUN_DOWN);
	
	SpawnEffect("Wm_mr_kickhit", 0, &blah->pos, &nullRot, &oneVec);
	
	this->damage += 5;


	if (this->damage > this->lives) {	
		for (int i = 0; i < NUMBER_OF_MINIS; i++) {
			if(enemy[i] != NULL) {
				enemy[i]->doStateChange(&daMiniSidestepper_c::StateID_Outro);
			}
		}
		
		doStateChange(&StateID_Outro);
	} else {
		this->fastwalkafterhit = true;
		doStateChange(&StateID_Hit); 
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


dActor_c *daSidestepper_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daSidestepper_c));
	return new(buffer) daSidestepper_c;
}

const SpriteData SidestepperSpriteData = { ProfileId::Sidestepper, 0, 0 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile SidestepperProfile(&daSidestepper_c::build, SpriteId::Sidestepper, &SidestepperSpriteData, ProfileId::Sidestepper, ProfileId::Sidestepper, "Sidestepper", SidestepperArcNameList);


int daSidestepper_c::onCreate() {
	//Variables
	this->nullRot = (S16Vec){0, 0, 0};
	this->efScale = (Vec){2.0f, 1.0f, 1.0f};

	target = GetSpecificPlayerActor(LiQ::NearestPlayer(this)); //target is mario


	this->randomnum = 0;
	this->isBuried = 0; //0 when not, 1 when yes
	this->fastwalkafterhit = false;
	this->everysecondtry = false;
	this->everysecondtry2 = true;
	this->round0 = false;
	this->round1 = false;
	this->rockisleft = false;

	this->barrelid = 418;
	
	
	// Do the following once when object is spawned:

	allocator.link(-1, GameHeaps[0], 0, 0x20);

	// Model settings. First string is arc name, second string is brres name inside arc, third string is mdl0 name inside brres.
	this->resFile.data = getResource("sidestepper", "g3d/sidestepper.brres");

	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("sidestepper");
	bodyModel.setup(mdl, &this->allocator, 0x224, 1, 0);
	SetupTextures_Boss(&bodyModel, 0);

	nw4r::g3d::ResAnmChr anmChr = resFile.GetResAnmChr("wait");
	this->animationChr.setup(mdl, anmChr, &this->allocator, 0);

	allocator.unlink();

	// Physics settings. Use hitbox debug mode to test hitbox, [Press "-" 16 times on star coin menu].
	ActivePhysics::Info Physics;
	Physics.xDistToCenter = 0.0;
	Physics.yDistToCenter = 20.0;
	Physics.xDistToEdge = 25.0;
	Physics.yDistToEdge = 25.0;
	Physics.category1 = 0x3;
	Physics.category2 = 0x0;
	Physics.bitfield1 = 0x4F;
	Physics.bitfield2 = 0xFFFFFFFF;
	Physics.unkShort1C = 0;
	Physics.callback = &dEn_c::collisionCallback;

	this->aPhysics.initWithStruct(this, &Physics);
	this->aPhysics.addToList();

	// Size settings

	this->scale.x = 0.15;
	this->scale.y = 0.15;
	this->scale.z = 0.15;

	this->pos.z = -1000.0;
	this->pos.y -= 1;

	this->startposx = pos.x;
	this->BaseLine = pos.y;

	//5 Lives function was disabled in Reggie due to incompatibility with other attacks. (Is enabled in the code)
	//DO NOT USE 5 LIVES!!!
	if (this->settings >> 31 & 0b1)
	{
		this->lives = 24;
	}
	else
	{
		this->lives = 14;
	}

	this->lives = 14;

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
	//
	fogModel.scheduleForDrawing();
	//fogModel._vf1C();
	this->fogSRT.process();
	//
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
	fogModel._vf1C();

	if (this->animationChr.isAnimationDone()) {
		this->animationChr.setCurrentFrame(0.0);
	}
	
	for (int i = 0; i < NUMBER_OF_MINIS; i++) {
    	if (enemy[i] != NULL && enemy[i]->acState.getCurrentState() == &daMiniSidestepper_c::StateID_Outro) {
        	enemy[i] = NULL;
		}
    }
	
	if ((dStageActor_c*)fBase_c::search(BLOCK_TARU))
	{
		if (acState.getCurrentState() != &StateID_Actors)
		{	
			if (barrel->pos.x < (this->pos.x + 30.0) && barrel->pos.x > (this->pos.x - 30.0))
			{
				if (barrel->pos.y < (this->pos.y + 30.0) && barrel->pos.y > (this->pos.y - 30.0))
					destroyBarrel(barrel);
			}
		}
	}

	return true;
}



/*//////*/
/*States*/
/*//////*/


void daSidestepper_c::beginState_Grow()
{
	this->timer = 0;

	bindAnimChr_and_setUpdateRate("wait", 1, 0.0, 1.0);

	SetupBoss();
}

void daSidestepper_c::executeState_Grow()
{
	this->timer += 1;

	bool ret;
	ret = GrowBossNoKameck(this, 0.15, 0.4, 0, this->timer);


	//Grow Sound Test

	if (ret) {
		PlaySound(this, SE_EMY_MECHAKOOPA_BOUND);
		doStateChange(&StateID_Bury);
	}
	

}

void daSidestepper_c::endState_Grow()
{
	CleanupBoss();
	this->BaseLine = this->pos.y;
}

void daSidestepper_c::beginState_Hit()
{
	timer = 0;

	bindAnimChr_and_setUpdateRate("hit", 1, 0.0, 1.0);
}

void daSidestepper_c::executeState_Hit()
{

	if (timer > 120)
	{
		doStateChange(&StateID_Bury);
	}

	timer += 1;
}

void daSidestepper_c::endState_Hit()
{ }


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
			if (everysecondtry2)
			{
				everysecondtry2 = false;
				doStateChange(&StateID_Actors);
			}
			else
			{
				//everysecondtry2 = true;
				doStateChange(&StateID_MoveSand);
			}
				
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

	target = GetSpecificPlayerActor(LiQ::NearestPlayer(this));

	if (target->pos.x > pos.x)
	{
		bindAnimChr_and_setUpdateRate("walk_r", 1, 0.0, 1.0);
		this->right = true;
		this->left = false;
	}
	else if (target->pos.x <= pos.x)
	{
		bindAnimChr_and_setUpdateRate("walk_l", 1, 0.0, 1.0);
		this->left = true;
		this->right = false;
	}
}

void daSidestepper_c::executeState_Walk()
{
	this->timer += 1;



	if (this->lives == 14)
	{
		if (damage > 9)
		{
			doStateChange(&StateID_Run);
		}
	}
	else if (this->lives == 24)
	{
		if (damage > 19)
		{
			doStateChange(&StateID_Run);
		}
	}



	if (this->left == true)
		pos.x -= 1;
	else if (this->right == true)
		pos.x += 1;
	if (this->randomnum > 2)
	{
		doStateChange(&StateID_Projectiles); //Before Bury
		this->randomnum = 0;
	}


	if (this->timer > 50) 
		doStateChange(&StateID_Wait); 
}

void daSidestepper_c::endState_Walk()
{

}

void daSidestepper_c::beginState_Projectiles()
{

	this->timer = 0;
	

	bindAnimChr_and_setUpdateRate("wait", 1, 0.0, 1.0);

	this->rndmtimerock = GenerateRandomNumber(6);

	switch (this->rndmtimerock) //Don't judge me for switches Mihi!
	{
	case 0:
		this->timerock = 0;
		break;
	case 1:
		this->timerock = 16;
		break;
	case 2:
		this->timerock = 32;
		break;
	case 3:
		this->timerock = 48;
		break;
	case 4:
		this->timerock = 64;
		break;
	case 5:
		this->timerock = 80;
		break;
	default:
		this->Delete(1);
		break;
	}


	plusorminusrock = GenerateRandomNumber(2);


	//Check lives
	

	if (damage > 4) //1 hit
	{
		round1 = true;
	}

	else { //0 hits
		round0 = true;
	}

	target = GetSpecificPlayerActor(LiQ::NearestPlayer(this));

	if (target->pos.x > pos.x)
	{
		bindAnimChr_and_setUpdateRate("projectileRotRight", 1, 0.0, 1.0);
		this->right = true;
		this->left = false;
		this->posrock = (Vec){ pos.x + 20.0 , pos.y + 20.0 , -1000 };
	}
	else if (target->pos.x < pos.x)
	{
		bindAnimChr_and_setUpdateRate("projectileRotLeft", 1, 0.0, 1.0);
		this->left = true;
		this->right = false;
		this->posrock = (Vec){ pos.x - 20.0 , pos.y + 20.0 , -1000 };
	}
}

void daSidestepper_c::executeState_Projectiles()
{
	//+19 after shoot to create actor
	//+23 after Create Actor (then wait)


	this->timer += 1;

	if (this->right == true)
	{
		//If round0 is rn
		if (round0)
		{
			if (timer == 119)
			{
				bindAnimChr_and_setUpdateRate("projectileWaitRight", 1, 0.0, 1.0);
			}


			if (timer == 198)
			{

				bindAnimChr_and_setUpdateRate("projectileShootRight", 1, 0.0, 1.0);
			}

			if (timer == 217)
			{
				Vec interneffectpos = { pos.x + 12.0, pos.y + 2.0, 0 };
				S16Vec nullRot = { 0,0,0 };
				Vec oneVec = { 1.5f, 1.5f, 1.5f };
				SpawnEffect("Wm_mr_hardhit_glow", 0, &interneffectpos, &nullRot, &oneVec);
				this->rock = CreateActor(EN_KANITAMA, 0, this->posrock, 0, 0);
				PlaySound(this, SE_EMY_KANIBO_THROW);
			}

			if (timer == 240)
			{
				bindAnimChr_and_setUpdateRate("projectileShootRight", 1, 0.0, 1.0);
			}

			if (timer == 259)
			{
				Vec interneffectpos = { pos.x + 12.0, pos.y + 2.0, 0 };
				S16Vec nullRot = { 0,0,0 };
				Vec oneVec = { 1.5f, 1.5f, 1.5f };
				SpawnEffect("Wm_mr_hardhit_glow", 0, &interneffectpos, &nullRot, &oneVec);
				this->rock = CreateActor(EN_KANITAMA, 0, this->posrock, 0, 0);
				PlaySound(this, SE_EMY_KANIBO_THROW);
			}

			if (timer == 282)
			{
				bindAnimChr_and_setUpdateRate("projectileRotBackRight", 1, 0.0, 1.0);
			}

			if (timer > 216)
			{
				timerock--;
				if (timerock > 0)
				{
					rock->pos.y += 1;
				}
			}
		}
		
		//If round1 is rn
		else if (round1)
		{
			if (timer == 119)
			{
				bindAnimChr_and_setUpdateRate("projectileWaitRight", 1, 0.0, 1.0);
			}


			if (timer == 198)
			{

				bindAnimChr_and_setUpdateRate("projectileShootRight", 1, 0.0, 1.0);
			}

			if (timer == 217)
			{
				Vec interneffectpos = { pos.x + 12.0, pos.y + 2.0, 0 };
				S16Vec nullRot = { 0,0,0 };
				Vec oneVec = { 1.5f, 1.5f, 1.5f };
				SpawnEffect("Wm_mr_hardhit_glow", 0, &interneffectpos, &nullRot, &oneVec);
				this->rock = CreateActor(EN_KANITAMA, 0, this->posrock, 0, 0);
				PlaySound(this, SE_EMY_KANIBO_THROW);
			}

			if (timer == 240)
			{
				bindAnimChr_and_setUpdateRate("projectileShootRight", 1, 0.0, 1.0);
			}

			if (timer == 259)
			{
				Vec interneffectpos = { pos.x + 12.0, pos.y + 2.0, 0 };
				S16Vec nullRot = { 0,0,0 };
				Vec oneVec = { 1.5f, 1.5f, 1.5f };
				SpawnEffect("Wm_mr_hardhit_glow", 0, &interneffectpos, &nullRot, &oneVec);
				this->rock = CreateActor(EN_KANITAMA, 0, this->posrock, 0, 0);
				PlaySound(this, SE_EMY_KANIBO_THROW);
			}

			if (timer == 284)
			{
				bindAnimChr_and_setUpdateRate("projectileShootRight", 1, 0.0, 1.0);
			}

			if (timer == 303)
			{
				Vec interneffectpos = { pos.x + 12.0, pos.y + 2.0, 0 };
				S16Vec nullRot = { 0,0,0 };
				Vec oneVec = { 1.5f, 1.5f, 1.5f };
				SpawnEffect("Wm_mr_hardhit_glow", 0, &interneffectpos, &nullRot, &oneVec);
				this->rock = CreateActor(EN_KANITAMA, 0, this->posrock, 0, 0);
				PlaySound(this, SE_EMY_KANIBO_THROW);
			}

			if (timer == 328)
			{
				bindAnimChr_and_setUpdateRate("projectileShootRight", 1, 0.0, 1.0);
			}

			if (timer == 347)
			{
				Vec interneffectpos = { pos.x + 12.0, pos.y + 2.0, 0 };
				S16Vec nullRot = { 0,0,0 };
				Vec oneVec = { 1.5f, 1.5f, 1.5f };
				SpawnEffect("Wm_mr_hardhit_glow", 0, &interneffectpos, &nullRot, &oneVec);
				this->rock = CreateActor(EN_KANITAMA, 0, this->posrock, 0, 0);
				PlaySound(this, SE_EMY_KANIBO_THROW);
			}

			if (timer == 370)
			{
				bindAnimChr_and_setUpdateRate("projectileRotBackRight", 1, 0.0, 1.0);
			}

			if (timer > 216)
			{
				timerock--;
				if (timerock > 0)
				{
					rock->pos.y += 1;
				}
			}
		}
		
	}
	else if (this->left == true)
	{

		//If round0 is rn
		if (round0)
		{
			if (timer == 119)
			{
				bindAnimChr_and_setUpdateRate("projectileWaitLeft", 1, 0.0, 1.0);
			}


			if (timer == 198)
			{

				bindAnimChr_and_setUpdateRate("projectileShootLeft", 1, 0.0, 1.0);
			}

			if (timer == 217)
			{
				Vec interneffectpos = { pos.x - 12.0, pos.y + 2.0, 0 };
				S16Vec nullRot = { 0,0,0 };
				Vec oneVec = { 1.5f, 1.5f, 1.5f };
				SpawnEffect("Wm_mr_hardhit_glow", 0, &interneffectpos, &nullRot, &oneVec);
				this->rock = CreateActor(EN_KANITAMA, 1 << 0, this->posrock, 0, 0);
				PlaySound(this, SE_EMY_KANIBO_THROW);
			}

			if (timer == 240)
			{
				bindAnimChr_and_setUpdateRate("projectileShootLeft", 1, 0.0, 1.0);
			}

			if (timer == 259)
			{
				Vec interneffectpos = { pos.x - 12.0, pos.y + 2.0, 0 };
				S16Vec nullRot = { 0,0,0 };
				Vec oneVec = { 1.5f, 1.5f, 1.5f };
				SpawnEffect("Wm_mr_hardhit_glow", 0, &interneffectpos, &nullRot, &oneVec);
				this->rock = CreateActor(EN_KANITAMA, 1 << 0, this->posrock, 0, 0);
				PlaySound(this, SE_EMY_KANIBO_THROW);
			}

			if (timer == 282)
			{
				bindAnimChr_and_setUpdateRate("projectileRotBackLeft", 1, 0.0, 1.0);
			}

			if (timer > 216)
			{
				timerock--;
				if (timerock > 0)
				{
					rock->pos.y += 1;
				}
			}
		}

		//If round1 is rn
		else if (round1)
		{
			if (timer == 119)
			{
				bindAnimChr_and_setUpdateRate("projectileWaitLeft", 1, 0.0, 1.0);
			}


			if (timer == 198)
			{

				bindAnimChr_and_setUpdateRate("projectileShootLeft", 1, 0.0, 1.0);
			}

			if (timer == 217)
			{
				Vec interneffectpos = { pos.x - 12.0, pos.y + 2.0, 0 };
				S16Vec nullRot = { 0,0,0 };
				Vec oneVec = { 1.5f, 1.5f, 1.5f };
				SpawnEffect("Wm_mr_hardhit_glow", 0, &interneffectpos, &nullRot, &oneVec);
				this->rock = CreateActor(EN_KANITAMA, 1 << 0, this->posrock, 0, 0);
				PlaySound(this, SE_EMY_KANIBO_THROW);
			}

			if (timer == 240)
			{
				bindAnimChr_and_setUpdateRate("projectileShootLeft", 1, 0.0, 1.0);
			}

			if (timer == 259)
			{
				Vec interneffectpos = { pos.x - 12.0, pos.y + 2.0, 0 };
				S16Vec nullRot = { 0,0,0 };
				Vec oneVec = { 1.5f, 1.5f, 1.5f };
				SpawnEffect("Wm_mr_hardhit_glow", 0, &interneffectpos, &nullRot, &oneVec);
				this->rock = CreateActor(EN_KANITAMA, 1 << 0, this->posrock, 0, 0);
				PlaySound(this, SE_EMY_KANIBO_THROW);
			}

			if (timer == 284)
			{
				bindAnimChr_and_setUpdateRate("projectileShootLeft", 1, 0.0, 1.0);
			}

			if (timer == 303)
			{
				Vec interneffectpos = { pos.x - 12.0, pos.y + 2.0, 0 };
				S16Vec nullRot = { 0,0,0 };
				Vec oneVec = { 1.5f, 1.5f, 1.5f };
				SpawnEffect("Wm_mr_hardhit_glow", 0, &interneffectpos, &nullRot, &oneVec);
				this->rock = CreateActor(EN_KANITAMA, 1 << 0, this->posrock, 0, 0);
				PlaySound(this, SE_EMY_KANIBO_THROW);
			}

			if (timer == 328)
			{
				bindAnimChr_and_setUpdateRate("projectileShootLeft", 1, 0.0, 1.0);
			}

			if (timer == 347)
			{
				Vec interneffectpos = { pos.x - 12.0, pos.y + 2.0, 0 };
				S16Vec nullRot = { 0,0,0 };
				Vec oneVec = { 1.5f, 1.5f, 1.5f };
				SpawnEffect("Wm_mr_hardhit_glow", 0, &interneffectpos, &nullRot, &oneVec);
				this->rock = CreateActor(EN_KANITAMA, 1 << 0, this->posrock, 0, 0);
				PlaySound(this, SE_EMY_KANIBO_THROW);
			}

			if (timer == 370)
			{
				bindAnimChr_and_setUpdateRate("projectileRotBackLeft", 1, 0.0, 1.0);
			}

			if (timer > 216)
			{
				timerock--;
				if (timerock > 0)
				{
					rock->pos.y += 1;
				}
			}
		}
	}

	if (round0)
	{
		if (this->timer > 400)
		{
			this->round0 = false;
			this->round1 = false;

			everysecondtry2 = true;

			doStateChange(&StateID_Bury);
		}
	}
	if (round1)
	{
		if (this->timer > 488)
		{
			this->round0 = false;
			this->round1 = false;

			everysecondtry2 = true;

			doStateChange(&StateID_Bury);
		}
	}

}

void daSidestepper_c::endState_Projectiles()
{ }

void daSidestepper_c::beginState_Bury()
{
	bindAnimChr_and_setUpdateRate("groundDisappear", 1, 0.0, 1.0);

	this->timer = 0;

	PlaySound(this, 545);

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

		doStateChange(&StateID_Wait);
	}

	this->timer += 1;
}

void daSidestepper_c::endState_Bury()
{ }



void daSidestepper_c::beginState_Actors()
{
	this->timer = 0;

	if (this->barrel = (dStageActor_c*)fBase_c::search(BLOCK_TARU))
	{
		destroyBarrel(barrel);
	}
}

void daSidestepper_c::executeState_Actors()
{
	if (timer == 0)
	{
		//Barrel
		rndmnum = GenerateRandomNumber(170); //pos from actor

		plusorminus = GenerateRandomNumber(2); //plus or minus
		if (plusorminus == 0)
		{
			this->posbarrel = (Vec){ pos.x + rndmnum, pos.y, pos.z };
		}
		else if (plusorminus == 1)
		{
			this->posbarrel = (Vec){ pos.x - rndmnum, pos.y, pos.z };
		}
		else
			this->Delete(1);


		//Crab or Enemy
		rndmnum = GenerateRandomNumber(220); //pos from actor

		plusorminus = GenerateRandomNumber(2); //plus or minus
		if (plusorminus == 0)
		{
			this->posenemy = (Vec){ pos.x + rndmnum, this->BaseLine, pos.z - 100.0 };
		}
		else if (plusorminus == 1)
		{
			this->posenemy = (Vec){ pos.x - rndmnum, this->BaseLine, pos.z - 100.0 };
		}
		else
			this->Delete(1);
		
		
		//Create Effect at barrels pos
		this->barreleffect = (Vec){ this->posbarrel.x, this->BaseLine, 0 };
		SpawnEffect("Wm_mr_sanddive_out", 0, &this->barreleffect, &nullRot, &efScale);
		SpawnEffect("Wm_mr_sanddive_smk", 0, &this->barreleffect, &nullRot, &efScale);

		//Create Effect at crabs pos
		
		bool anyEnemyNull = false;
		for (int i = 0; i < NUMBER_OF_MINIS; i++) {
			if(enemy[i] == NULL) {
				anyEnemyNull = true;
				break;
			}
		}

		if (anyEnemyNull) {
			this->enemyeffect = (Vec){ this->posenemy.x, this->BaseLine, 0 };
			SpawnEffect("Wm_mr_sanddive_out", 0, &this->enemyeffect, &nullRot, &efScale);
			SpawnEffect("Wm_mr_sanddive_smk", 0, &this->enemyeffect, &nullRot, &efScale);
		}
	}
	
	if (timer > 50)
	{
		if (timer < 74)
		{
			if (timer == 51)
			{
				this->barrel = CreateActor(BLOCK_TARU, 0, this->posbarrel, 0, 0);
				this->barrel->pos.z = -1000.0;
				
				for (int i = 0; i < NUMBER_OF_MINIS; i++) {
					if(enemy[i] == NULL) {
						this->enemy[i] = (daMiniSidestepper_c*)CreateActor(MiniSidestepper, 0, this->posenemy, 0, 0);
						this->enemy[i]->bossFlag = true;
						break;
					}
				}

			}
			barrel->pos.y += 6.0;
		}
	}
	if (timer > 73)
		doStateChange(&StateID_MoveSand);

	this->timer += 1;
}

void daSidestepper_c::endState_Actors()
{ }

void daSidestepper_c::beginState_MoveSand()
{


	bindAnimChr_and_setUpdateRate("wait", 1, 0.0, 1.0);
	this->timer = 0;

	this->rndmnumms = GenerateRandomNumber(4) + 1;

}

void daSidestepper_c::executeState_MoveSand()
{
	
	target = GetSpecificPlayerActor(LiQ::NearestPlayer(this));

	this->timer += 1;

	if (this->timer == 60)
	{

		this->possand = (Vec){ target->pos.x, this->BaseLine, 0 };

		SpawnEffect("Wm_mr_sanddive_out", 0, &this->possand, &nullRot, &efScale);
		SpawnEffect("Wm_mr_sanddive_smk", 0, &this->possand, &nullRot, &efScale);
	}

	if (this->timer == 120)
	{

		this->possand = (Vec){ target->pos.x, this->BaseLine, 0 };

		SpawnEffect("Wm_mr_sanddive_out", 0, &this->possand, &nullRot, &efScale);
		SpawnEffect("Wm_mr_sanddive_smk", 0, &this->possand, &nullRot, &efScale);
	}

	if (this->timer == 180)
	{

		this->possand = (Vec){ target->pos.x, this->BaseLine, 0 };

		SpawnEffect("Wm_mr_sanddive_out", 0, &this->possand, &nullRot, &efScale);
		SpawnEffect("Wm_mr_sanddive_smk", 0, &this->possand, &nullRot, &efScale);
	}

	if (this->timer == (this->rndmnumms * 60))
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
	bindAnimChr_and_setUpdateRate("groundAppear", 1, 0.0, 1.0);
	this->timer = 0;

	BackUpEffect = (Vec){ pos.x, this->BaseLine, 0 };

	SpawnEffect("Wm_mr_sanddive_out", 0, &this->BackUpEffect, &nullRot, &efScale);
	SpawnEffect("Wm_mr_sanddive_smk", 0, &this->BackUpEffect, &nullRot, &efScale);
	PlaySound(this, 775);
}

void daSidestepper_c::executeState_BackUp()
{
	if (this->timer < 90 && this->timer > 29) {
		this->pos.y += 2.0;  // Height is 54 pixels, move down
	}

	if (this->timer == 90) { bindAnimChr_and_setUpdateRate("wait", 1, 0.0, 1.0); }

	if (this->timer > 120) {
		if (pos.y != this->BaseLine)
		{
			pos.y = this->BaseLine;
		}
		
		if (this->fastwalkafterhit)
		{
			this->fastwalkafterhit = false;
			doStateChange(&StateID_Run);
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

	this->rndmspeedr = GenerateRandomNumber(3) + 2;

	if (this->damage > 5)
	{
		this->rndmspeedr = 4;
	}

	target = GetSpecificPlayerActor(LiQ::NearestPlayer(this));

	if (target->pos.x > pos.x)
	{
		bindAnimChr_and_setUpdateRate("attack_l_down", 1, 0.0, 1.0);
		this->right = true;
		this->left = false;
	}
	else if (target->pos.x <= pos.x)
	{
		bindAnimChr_and_setUpdateRate("attack_r_down", 1, 0.0, 1.0);
		this->left = true;
		this->right = false;
	}
}

void daSidestepper_c::executeState_Run()
{
	this->timer += 1;

	if (this->right == true)
	{
		if (this->timer < 60)
		{
			SpawnEffect("Wm_mr_sanddive_smk", 0, &this->pos, &nullRot, &efScale);
		}
		if (this->timer >= 60)
		{
			if (timer == 60) { bindAnimChr_and_setUpdateRate("attack_l_loop", 1, 0.0, 1.0); }
			PlaySound(this, SE_OBJ_TEKKYU_G_CRASH);
			pos.x += this->rndmspeedr;

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
		if (this->timer < 60)
		{
			SpawnEffect("Wm_mr_sanddive_smk", 0, &this->pos, &nullRot, &efScale);
		}
		if (this->timer >= 60)
		{
			if (timer == 60) { bindAnimChr_and_setUpdateRate("attack_r_loop", 1, 0.0, 1.0); }
			PlaySound(this, SE_OBJ_TEKKYU_G_CRASH);
			pos.x -= this->rndmspeedr;

			if (everysecondtry == true) {
				SpawnEffect("Wm_mr_sanddive_smk", 0, &this->pos, &nullRot, &efScale);
				everysecondtry = false;
			}
			if (everysecondtry == false) {
				everysecondtry = true;
			}
		}
	}


	if (this->damage != 10) 
	{
		if (this->timer > 230)
		{
			doStateChange(&StateID_Bury);
		}
	}
	else 
	{
		if (this->timer > 230)
		{
			if (this->right == true)
			{
				if (this->timer < 291)
				{
					SpawnEffect("Wm_mr_sanddive_smk", 0, &this->pos, &nullRot, &efScale);
				}
				if (this->timer >= 291)
				{
					if (timer == 291) { bindAnimChr_and_setUpdateRate("attack_l_loop", 1, 0.0, 1.0); }
					PlaySound(this, SE_OBJ_TEKKYU_G_CRASH);
					pos.x += this->rndmspeedr;

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
				if (this->timer < 291)
				{
					SpawnEffect("Wm_mr_sanddive_smk", 0, &this->pos, &nullRot, &efScale);
				}
				if (this->timer >= 291)
				{
					if (timer == 291) { bindAnimChr_and_setUpdateRate("attack_r_loop", 1, 0.0, 1.0); }
					PlaySound(this, SE_OBJ_TEKKYU_G_CRASH);
					pos.x -= this->rndmspeedr;

					if (everysecondtry == true) {
						SpawnEffect("Wm_mr_sanddive_smk", 0, &this->pos, &nullRot, &efScale);
						everysecondtry = false;
					}
					if (everysecondtry == false) {
						everysecondtry = true;
					}
				}
			}
		}
		
		if (this->timer > 291)
		{
			doStateChange(&StateID_Bury);
		}
	}
}

void daSidestepper_c::endState_Run()
{ }




void daSidestepper_c::beginState_Claw()
{
	timer = 0;
	this->claw = CreateActor(801, 0, this->pos, 0, 0);
}
void daSidestepper_c::executeState_Claw()
{
	if (timer == 350)
	{
		doStateChange(&StateID_BackUp);
	}
	
	timer++;
}
void daSidestepper_c::endState_Claw() 
{ }




void daSidestepper_c::beginState_Outro()
{
	this->timer = 0;
	bindAnimChr_and_setUpdateRate("hit", 1, 0.0, 1.0);
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
	ret = ShrinkBoss(this, &this->pos, 0.4, this->timer);

	if (ret == true) {
		this->pos.z = 3000.0; 
		BossExplode(this, &this->pos);
		this->dying = 1;
		this->timer = 0;
	}

	this->timer += 1;
}

void daSidestepper_c::endState_Outro() { }