#include <common.h>
#include <game.h>
#include <g3dhax.h>
#include <sfx.h>
#include <stage.h>
#include <profile.h>
#include "boss.h"

//Yeah... I know... This is a lazy copy of the sidestepper boss... But it works! -LiQ | 02/06/21 11:13am

extern "C" void destroyBarrel(dStageActor_c*);

const char* MiniSidestepperArcNameList[] =
{
	"sidestepper",
	NULL
};

//class defined in sidestepper.cpp

void daMiniSidestepper_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate) {
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr(name);
	this->chrAnimation.bind(&this->bodyModel, anmChr, unk);
	this->bodyModel.bindAnim(&this->chrAnimation, unk2);
	this->chrAnimation.setUpdateRate(rate);
}


// Extra collision conditions:

void daMiniSidestepper_c::playerCollision(ActivePhysics* apThis, ActivePhysics* apOther)
{
	this->_vf220(apOther->owner);
}

bool daMiniSidestepper_c::collisionCat3_StarPower(ActivePhysics* apThis, ActivePhysics* apOther) {
	PlaySound(this, SE_EMY_DOWN);

	doStateChange(&StateID_Outro);

	return true;
}
bool daMiniSidestepper_c::collisionCat5_Mario(ActivePhysics* apThis, ActivePhysics* apOther) {
	// When player collides using butt slide
	playerCollision(apThis, apOther);
	return true;
}
bool daMiniSidestepper_c::collisionCatD_Drill(ActivePhysics* apThis, ActivePhysics* apOther) {
	// When player drills with propeller
	playerCollision(apThis, apOther);
	return true;
}
bool daMiniSidestepper_c::collisionCat8_FencePunch(ActivePhysics* apThis, ActivePhysics* apOther) {
	// When player punches fence behind
	playerCollision(apThis, apOther);
	return true;
}
bool daMiniSidestepper_c::collisionCat7_GroundPound(ActivePhysics* apThis, ActivePhysics* apOther) {
	// When player ground pounds
	playerCollision(apThis, apOther);
	return true;
}
bool daMiniSidestepper_c::collisionCat7_GroundPoundYoshi(ActivePhysics* apThis, ActivePhysics* apOther) {
	// When player ground pounds with Yoshi
	playerCollision(apThis, apOther);
	return true;
}
bool daMiniSidestepper_c::collisionCatA_PenguinMario(ActivePhysics* apThis, ActivePhysics* apOther) {
	// When player slides with penguin suit
	PlaySound(this, SE_EMY_DOWN);

	doStateChange(&StateID_Outro);

	return true;
}
bool daMiniSidestepper_c::collisionCat11_PipeCannon(ActivePhysics* apThis, ActivePhysics* apOther) {
	// When player shoots from pipe cannon
	PlaySound(this, SE_EMY_DOWN);

	doStateChange(&StateID_Outro);

	return true;
}
bool daMiniSidestepper_c::collisionCat9_RollingObject(ActivePhysics* apThis, ActivePhysics* apOther) {
	// When player throws object at sprite
	if (bossFlag)
	{
		dActor_c *block = apOther->owner;
		dEn_c* blah = (dEn_c*)block;

		if (block->name == BLOCK_TARU)
		{
			destroyBarrel(blah);
		}
	}

	PlaySound(this, SE_EMY_DOWN);

	doStateChange(&StateID_Outro);

	return true;
}
bool daMiniSidestepper_c::collisionCat1_Fireball_E_Explosion(ActivePhysics* apThis, ActivePhysics* apOther) {
	// When collides with fireball
	PlaySound(this, SE_EMY_DOWN);

	doStateChange(&StateID_Outro);

	return true;
}
bool daMiniSidestepper_c::collisionCat13_Hammer(ActivePhysics* apThis, ActivePhysics* apOther) {
	// When collides with hammer projectile
	PlaySound(this, SE_EMY_DOWN);

	doStateChange(&StateID_Outro);

	return true;
}
bool daMiniSidestepper_c::collisionCat14_YoshiFire(ActivePhysics* apThis, ActivePhysics* apOther) {
	// When collides with Yoshi fire spit
	PlaySound(this, SE_EMY_DOWN);

	doStateChange(&StateID_Outro);

	return true;
}



// These handle the ice crap
void daMiniSidestepper_c::_vf148() {
	dEn_c::_vf148();
	doStateChange(&StateID_Outro);
}
void daMiniSidestepper_c::_vf14C() {
	dEn_c::_vf14C();
	doStateChange(&StateID_Outro);
}

extern "C" void sub_80024C20(void);
extern "C" void __destroy_arr(void*, void(*)(void), int, int);
//extern "C" __destroy_arr(struct DoSomethingCool, void(*)(void), int cnt, int bar);

bool daMiniSidestepper_c::CreateIceActors() {
	struct DoSomethingCool my_struct = { 0, this->pos, {1.8, 1.4, 1.2}, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
	this->frzMgr.Create_ICEACTORs((void*)&my_struct, 1);
	__destroy_arr((void*)&my_struct, sub_80024C20, 0x3C, 1);
	chrAnimation.setUpdateRate(0.0f);
	return true;
}





dActor_c* daMiniSidestepper_c::build() {
	void* buffer = AllocFromGameHeap1(sizeof(daMiniSidestepper_c));
	return new(buffer) daMiniSidestepper_c;
}

const SpriteData MiniSidestepperSpriteData =
{ ProfileId::MiniSidestepper, 0, 0 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 2 };

Profile MiniSidestepperProfile(&daMiniSidestepper_c::build, SpriteId::MiniSidestepper, &MiniSidestepperSpriteData, ProfileId::MiniSidestepper, ProfileId::MiniSidestepper, "MiniSidestepper", MiniSidestepperArcNameList);


int daMiniSidestepper_c::onCreate() {
	//Variables
	this->nullRot = (S16Vec){ 0, 0, 0 };
	this->efScale = (Vec){ 1.0f, 0.5f, 1.0f };

	target = GetSpecificPlayerActor(0); //target is mario


	this->randomnum = 0;
	this->isBuried = 0; //0 when not, 1 when yes
	this->fastwalkafterhit = false;
	this->everysecondtry = false;
	this->everysecondtry2 = true;



	// Do the following once when object is spawned:

	allocator.link(-1, GameHeaps[0], 0, 0x20);

	// Model settings. First string is arc name, second string is brres name inside arc, third string is mdl0 name inside brres.
	this->resFile.data = getResource("sidestepper", "g3d/sidestepper.brres");

	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("sidestepper");
	bodyModel.setup(mdl, &this->allocator, 0x224, 1, 0);
	SetupTextures_Boss(&bodyModel, 0);

	nw4r::g3d::ResAnmChr anmChr = resFile.GetResAnmChr("wait");
	this->chrAnimation.setup(mdl, anmChr, &this->allocator, 0);


	//
	/*

	mdl = this->resFile.GetResMdl("bubble_fog_CS");
	fogModel.setup(mdl, &allocator, 0x124, 1, 0);

	nw4r::g3d::ResAnmTexSrt anmRes = this->resFile.GetResAnmTexSrt("wait_proj");
	this->fogSRT.setup(mdl, anmRes, &this->allocator, 0, 1);
	this->fogSRT.bindEntry(&this->fogModel, anmRes, 0, 0);
	this->fogModel.bindAnim(&this->fogSRT, 0.0);

	*/
	//



	allocator.unlink();

	// Physics settings. Use hitbox debug mode to test hitbox, [Press "-" 16 times on star coin menu].
	ActivePhysics::Info Physics;
	Physics.xDistToCenter = 0.0;
	Physics.yDistToCenter = 2.5;
	Physics.xDistToEdge = 10.0;
	Physics.yDistToEdge = 12.0;
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

	this->startposx = pos.x;


	daSidestepper_c* test;



	this->BaseLine = pos.y;


	//Set dist
	if (this->settings >> 3 & 0b1)
	{
		distbetweenpoints = 16.0;
	}
	else if (this->settings >> 4 & 0b1)
	{
		distbetweenpoints = 32.0;
	}
	else if (this->settings >> 5 & 0b1)
	{
		distbetweenpoints = 48.0;
	}
	else if (this->settings >> 6 & 0b1)
	{
		distbetweenpoints = 64.0;
	}
	else if (this->settings >> 7 & 0b1)
	{
		distbetweenpoints = 80.0;
	}

	else if (bossFlag)
	{
		distbetweenpoints = 80.0;
	}

	else
	{
		distbetweenpoints = 16.0;
	}



	//Check SpawnPoints
	if (this->settings & 0b1) //1 point
	{
		howmanypoints = 1;
		this->point0 = this->startposx;
	}
	else if (this->settings >> 1 & 0b1) //2
	{
		howmanypoints = 2;
		this->point0 = this->startposx;
		this->point1 = this->startposx + distbetweenpoints;
	}
	else if (this->settings >> 2 & 0b1) //3
	{
		howmanypoints = 3;
		this->point0 = this->startposx;
		this->point1 = this->startposx + distbetweenpoints;
		this->point2 = this->startposx - distbetweenpoints;
	}

	else if (bossFlag) //when sidestepper boss
	{
		howmanypoints = 3;
		this->point0 = this->startposx;
		this->point1 = this->startposx + distbetweenpoints;
		this->point2 = this->startposx - distbetweenpoints;
	}
	else //1 point
	{
		howmanypoints = 1;
		this->point0 = this->startposx;
	}


	doStateChange(&StateID_Wait);

	this->onExecute();
	return true;
}


int daMiniSidestepper_c::onDelete() {
	// Do the following when deleted:

	return true;
}

int daMiniSidestepper_c::onDraw() {
	bodyModel.scheduleForDrawing();
	return true;
}


void daMiniSidestepper_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

int daMiniSidestepper_c::onExecute() {
	// Do the following every frame:
	acState.execute();
	updateModelMatrices();
	bodyModel._vf1C();

	if (this->chrAnimation.isAnimationDone()) {
		this->chrAnimation.setCurrentFrame(0.0);
	}


	return true;
}



/*//////*/
/*States*/
/*//////*/



void daMiniSidestepper_c::beginState_Wait()
{
	this->timer = 0;
	bindAnimChr_and_setUpdateRate("wait", 1, 0.0, 1.0);
}

void daMiniSidestepper_c::executeState_Wait()
{
	this->timer += 1;

	if (timer > 50)
	{
		if (this->isBuried == 1)
		{
			this->isBuried = 0;
			
			everysecondtry2 = true;
			doStateChange(&StateID_MoveSand);
			

		}
		else
			doStateChange(&StateID_Walk);
	}
}

void daMiniSidestepper_c::endState_Wait()
{ }

void daMiniSidestepper_c::beginState_Walk()
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

void daMiniSidestepper_c::executeState_Walk()
{
	this->timer += 1;


	if (this->left == true)
		pos.x -= 0.4;
	else if (this->right == true)
		pos.x += 0.4;
	if (this->randomnum == 4)
	{
		doStateChange(&StateID_Bury);
		this->randomnum = 0;
	}


	if (this->timer > 50)
		doStateChange(&StateID_Wait);
}

void daMiniSidestepper_c::endState_Walk()
{

}


void daMiniSidestepper_c::beginState_Bury()
{
	bindAnimChr_and_setUpdateRate("wait", 1, 0.0, 1.0);

	this->timer = 0;

	PlaySound(this, 0x221);

	SpawnEffect("Wm_mr_sanddive_out", 0, &this->pos, &nullRot, &efScale);
	SpawnEffect("Wm_mr_sanddive_smk", 0, &this->pos, &nullRot, &efScale);
}

void daMiniSidestepper_c::executeState_Bury()
{
	if (this->timer < 60) {
		this->pos.y -= 1.0;  // Height is 54 pixels, move down
	}

	if (this->timer > 90) {
		this->isBuried = 1;
		doStateChange(&StateID_Wait);
	}

	this->timer += 1;
}

void daMiniSidestepper_c::endState_Bury()
{ }


void daMiniSidestepper_c::beginState_MoveSand()
{


	bindAnimChr_and_setUpdateRate("wait", 1, 0.0, 1.0);
	this->timer = 0;

}

void daMiniSidestepper_c::executeState_MoveSand()
{
	this->timer += 1;

	if (this->timer == 60)
	{

		this->possand = (Vec){ pos.x, this->BaseLine, 0 };

		SpawnEffect("Wm_mr_sanddive_out", 0, &this->possand, &nullRot, &efScale);
		SpawnEffect("Wm_mr_sanddive_smk", 0, &this->possand, &nullRot, &efScale);
	}

	if (this->timer == 120)
	{
		whichpoint = GenerateRandomNumber(howmanypoints);
		switch (whichpoint)
		{
		case 0:
			pos.x = point0;
			break;
		case 1:
			pos.x = point1;
			break;
		case 2:
			pos.x = point2;
			break;
		}

		doStateChange(&StateID_BackUp);
	}
}

void daMiniSidestepper_c::endState_MoveSand()
{

}


void daMiniSidestepper_c::beginState_BackUp()
{
	bindAnimChr_and_setUpdateRate("wait", 1, 0.0, 1.0);
	this->timer = 0;

	BackUpEffect = (Vec){ pos.x, this->BaseLine, 0 };


	SpawnEffect("Wm_mr_sanddive_out", 0, &this->BackUpEffect, &nullRot, &efScale);
	SpawnEffect("Wm_mr_sanddive_smk", 0, &this->BackUpEffect, &nullRot, &efScale);
}

void daMiniSidestepper_c::executeState_BackUp()
{
	if (this->timer < 60) {
		this->pos.y += 1.0;  // Height is 54 pixels, move down
	}

	if (this->timer > 90) {

		doStateChange(&StateID_Walk);
	}

	this->timer += 1;
}

void daMiniSidestepper_c::endState_BackUp()
{ }


void daMiniSidestepper_c::beginState_Outro()
{
	this->timer = 0;
	bindAnimChr_and_setUpdateRate("wait", 1, 0.0, 1.0);

	pos.z = 5000.0;

	this->removeMyActivePhysics();
}

void daMiniSidestepper_c::executeState_Outro()
{
	if (timer == 300)
	{
		this->Delete(1);
	}
	
	//this->rot.x += 0x500;
	//this->rot.y += 0x500;
	//this->rot.z += 0x500;
	//pos.y -= 1.5;
	
	dEn_c::dieFall_Execute();

	this->timer += 1;
}

void daMiniSidestepper_c::endState_Outro() { }