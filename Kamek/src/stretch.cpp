#include <common.h>
#include <game.h>
#include <g3dhax.h>
#include <sfx.h>
#include <stage.h>
#include <profile.h>
#include <nsmlw.h>
#include "boss.h"

//Yeah... I know... This is a lazy copy of the sidestepper boss... But it works! -LiQ | 02/06/21 11:13am

extern "C" void destroyBarrel(dStageActor_c*);

const char* StretchArcNameList[] = { "stretch", NULL };

class daEnStretch_c : public dEn_c {
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

	Vec savedPos;

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
	int whichpoint;
	float distbetweenpoints;
	bool bossFlag;
	bool stillFalling;
	
	int timer;

	int warpNodeCount;
	dCourse_c::railNode_s** warpNodes;

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
	bool collisionCatD_Drill(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat7_GroundPound(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat7_GroundPoundYoshi(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat9_RollingObject(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat1_Fireball_E_Explosion(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat13_Hammer(ActivePhysics* apThis, ActivePhysics* apOther);


	bool willWalkOntoSuitableGround();

	USING_STATES(daEnStretch_c);
	DECLARE_STATE(Wait);
	DECLARE_STATE(Walk);
	DECLARE_STATE(Bury);
	DECLARE_STATE(MoveSand);
	DECLARE_STATE(BackUp);
	DECLARE_STATE(Outro);
};

CREATE_STATE(daEnStretch_c, Wait);
CREATE_STATE(daEnStretch_c, Walk);
CREATE_STATE(daEnStretch_c, Bury);
CREATE_STATE(daEnStretch_c, MoveSand);
CREATE_STATE(daEnStretch_c, BackUp);
CREATE_STATE(daEnStretch_c, Outro);


void daEnStretch_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate) {
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr(name);
	this->chrAnimation.bind(&this->bodyModel, anmChr, unk);
	this->bodyModel.bindAnim(&this->chrAnimation, unk2);
	this->chrAnimation.setUpdateRate(rate);
}


void daEnStretch_c::playerCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	dAcPy_c* player = (dAcPy_c *)apOther->owner;
	if (!player->isNoDamage()) {
		player->setDamage(this, 0);
		PlaySoundAsync(this, SE_EMY_CS_TERESA_BEAT_YOU);
	}
	return;
}

bool daEnStretch_c::collisionCat3_StarPower(ActivePhysics* apThis, ActivePhysics* apOther) {
	u32 playerId;
	int comboCount;
	dAcPy_c *player;

	SpawnEffect("Wm_en_teresavanish", 0, &pos, &(S16Vec){0,0,0}, &(Vec){1.0, 1.0, 1.0});

	player = (dAcPy_c*)(apOther->owner);
	playerId = *(player->getPlrNo());
	u32 starCount = player->daPlBase_c::getStarCount();
	player->playKameHitSound(starCount, 0);
	comboCount = player->getComboCount(); //8009f630
	dScoreMng_c::instance->dScoreMng_c::sub_800E2190(this, 0.0, 24.0, comboCount, playerId);
	if (playerId != -1) {
		dMultiMng_c::instance->dMultiMng_c::incEnemyDown(playerId);
	}

	PlaySoundAsync(this, SE_EMY_TERESA_DEAD);
	this->Delete(1);
	return true;
}



bool daEnStretch_c::collisionCatD_Drill(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->playerCollision(apThis, apOther);
	return false;
}

bool daEnStretch_c::collisionCat7_GroundPound(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->playerCollision(apThis, apOther);
	return false;
}
bool daEnStretch_c::collisionCat7_GroundPoundYoshi(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->playerCollision(apThis, apOther);
	return false;
}


bool daEnStretch_c::collisionCat9_RollingObject(ActivePhysics* apThis, ActivePhysics* apOther) {
	return dEn_c::collisionCat9_RollingObject(apThis, apOther);
}

bool daEnStretch_c::collisionCat1_Fireball_E_Explosion(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->dEn_c::fireballInvalid(apThis, apOther);
	return false;
}

bool daEnStretch_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->dEn_c::iceballInvalid(apThis, apOther);
	return false;
}

bool daEnStretch_c::collisionCat13_Hammer(ActivePhysics* apThis, ActivePhysics* apOther) {
	u32 playerId;
	int comboCount;
	dAcPy_c *player;
	dStageActor_c* hammer;

	SpawnEffect("Wm_en_teresavanish", 0, &pos, &(S16Vec){0,0,0}, &(Vec){1.0, 1.0, 1.0});

	hammer = (dStageActor_c*)(apOther->owner);
	playerId = *(hammer->getPlrNo());
	player = dAcPy_c::findByID(playerId);
	u32 starCount = player->daPlBase_c::getStarCount();
	player->playKameHitSound(starCount, 0);
	comboCount = player->getComboCount(); //8009f630
	dScoreMng_c::instance->dScoreMng_c::sub_800E2190(this, 0.0, 24.0, comboCount, playerId);
	if (playerId != -1) {
		dMultiMng_c::instance->dMultiMng_c::incEnemyDown(playerId);
	}

	PlaySoundAsync(this, SE_EMY_TERESA_DEAD);
	this->Delete(1);
	return true;
}

void daEnStretch_c::_vf148() {
	dEn_c::_vf148();
	this->Delete(1);
}

void daEnStretch_c::_vf14C() {
	dEn_c::_vf14C();
	this->Delete(1);
}

extern "C" void sub_80024C20(void);
extern "C" void __destroy_arr(void*, void(*)(void), int, int);
//extern "C" __destroy_arr(struct DoSomethingCool, void(*)(void), int cnt, int bar);

bool daEnStretch_c::CreateIceActors() {
	struct DoSomethingCool my_struct = { 0, this->pos, {1.8, 1.4, 1.2}, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
	this->frzMgr.Create_ICEACTORs((void*)&my_struct, 1);
	__destroy_arr((void*)&my_struct, sub_80024C20, 0x3C, 1);
	chrAnimation.setUpdateRate(0.0f);
	return true;
}





dActor_c* daEnStretch_c::build() {
	void* buffer = AllocFromGameHeap1(sizeof(daEnStretch_c));
	return new(buffer) daEnStretch_c;
}

const SpriteData StretchSpriteData = { ProfileId::Stretch, 0, 0 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 2 };
Profile StretchProfile(&daEnStretch_c::build, SpriteId::Stretch, &StretchSpriteData, ProfileId::Stretch, ProfileId::Stretch, "Stretch", StretchArcNameList);


int daEnStretch_c::onCreate() {
	//Variables
	this->nullRot = (S16Vec){ 0, 0, 0 };
	this->efScale = (Vec){ 1.0f, 0.5f, 1.0f };

	target = GetSpecificPlayerActor(LiQ::NearestPlayer(this)); //target is mario


	this->randomnum = 0;
	this->isBuried = 0; //0 when not, 1 when yes
	this->fastwalkafterhit = false;
	this->everysecondtry = false;
	this->everysecondtry2 = true;



	// Do the following once when object is spawned:

	allocator.link(-1, GameHeaps[0], 0, 0x20);

	// Model settings. First string is arc name, second string is brres name inside arc, third string is mdl0 name inside brres.
	this->resFile.data = getResource("stretch", "g3d/stretch.brres");

	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("stretch");
	bodyModel.setup(mdl, &this->allocator, 0x224, 1, 0);
	SetupTextures_Boss(&bodyModel, 0);

	nw4r::g3d::ResAnmChr anmChr = resFile.GetResAnmChr("walk");
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
	Physics.bitfield2 = 0xFFBAFFFE;
	Physics.unkShort1C = 0;
	Physics.callback = &dEn_c::collisionCallback;

	this->aPhysics.initWithStruct(this, &Physics);
	this->aPhysics.addToList();

	// These structs tell stupid collider what to collide with - these are from koopa troopa
	static const lineSensor_s below(-5<<12, 5<<12, 0<<12);
	static const pointSensor_s above(0<<12, 12<<12);
	static const lineSensor_s adjacent(8<<12, 9<<12, 8<<12);

	collMgr.init(this, &below, &above, &adjacent);
	collMgr.calculateBelowCollisionWithSmokeEffect();

	// Size settings

	this->scale.x = 1.0f;
	this->scale.y = 1.0f;
	this->scale.z = 1.0f;


	this->pos.z = -1000.0;

	this->startposx = pos.x;


	daSidestepper_c* test;



	this->BaseLine = pos.y;



	dCourse_c::rail_s *rail = GetRail(settings & 0xFF);
	dCourse_c *course = dCourseFull_c::instance->get(GetAreaNum());

	warpNodeCount = rail->nodeCount;

	warpNodes = new dCourse_c::railNode_s*[warpNodeCount];

	for (int i = 0; i < warpNodeCount; i++)
	{
		warpNodes[i] = &course->railNode[rail->startNode+i];
	}




	target = GetSpecificPlayerActor(LiQ::NearestPlayer(this));
	if (target->pos.x > pos.x) {
		this->rot.y = 0x1500;
	} else if (target->pos.x < pos.x) {
		this->rot.y = 0xE500;
	}
	doStateChange(&StateID_Wait);

	this->onExecute();
	return true;
}


int daEnStretch_c::onDelete() {
	// Do the following when deleted:
	delete[] warpNodes;

	return true;
}

int daEnStretch_c::onDraw() {
	bodyModel.scheduleForDrawing();
	return true;
}


void daEnStretch_c::updateModelMatrices() {
	if(this->acState.getCurrentState() == &StateID_BackUp || this->acState.getCurrentState() == &StateID_Bury) {
		matrix.translation(savedPos.x, savedPos.y, savedPos.z);
	} else {
		matrix.translation(pos.x, pos.y, pos.z);
	}
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

int daEnStretch_c::onExecute() {
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



void daEnStretch_c::beginState_Wait()
{
	this->timer = 0;
	bindAnimChr_and_setUpdateRate("walk", 1, 0.0, 1.0);
}

void daEnStretch_c::executeState_Wait()
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

void daEnStretch_c::endState_Wait()
{ }





bool daEnStretch_c::willWalkOntoSuitableGround() {
	static const float deltas[] = {2.5f, -2.5f};
	VEC3 checkWhere = {
			pos.x + deltas[direction],
			4.0f + pos.y,
			pos.z};

	u32 props = collMgr.getTileBehaviour2At(checkWhere.x, checkWhere.y, currentLayerID);

	//if (getSubType(props) == B_SUB_LEDGE)
	if (((props >> 16) & 0xFF) == 8)
		return false;

	float someFloat = 0.0f;
	if (collMgr.sub_800757B0(&checkWhere, &someFloat, currentLayerID, 1, -1)) {
		if (someFloat < checkWhere.y && someFloat > (pos.y - 5.0f))
			return true;
	}

	return false;
}




void daEnStretch_c::beginState_Walk()
{
	this->timer = 0;
	this->randomnum += 1;

	target = GetSpecificPlayerActor(LiQ::NearestPlayer(this));

	if (target->pos.x > pos.x)
	{
		//bindAnimChr_and_setUpdateRate("walk_r", 1, 0.0, 1.0);
		bindAnimChr_and_setUpdateRate("walk", 1, 0.0, 1.0);
		this->right = true;
		this->left = false;
		this->rot.y = 0x1500;
	}
	else if (target->pos.x < pos.x)
	{
		//bindAnimChr_and_setUpdateRate("walk_l", 1, 0.0, 1.0);
		bindAnimChr_and_setUpdateRate("walk", 1, 0.0, 1.0);
		this->left = true;
		this->right = false;
		this->rot.y = 0xE500;
	}
}

void daEnStretch_c::executeState_Walk()
{
	this->timer += 1;


	if (this->left == true)
		pos.x -= 1.2f;
	else if (this->right == true)
		pos.x += 1.2f;




	if (collMgr.isOnTopOfTile()) {
		stillFalling = false;

		if (!willWalkOntoSuitableGround()) {
			pos.x = this->left ? pos.x + 1.5 : pos.x - 1.5;
			bool temp = this->left;
			this->left = this->right;
			this->right = temp;

		}
	} else {
		if (!stillFalling) {
			stillFalling = true;
			pos.x = this->left ? pos.x + 1.5 : pos.x - 1.5;
			bool temp = this->left;
			this->left = this->right;
			this->right = temp;
		}
	}

	bool ret;
	collMgr.calculateAdjacentCollision(0);
	if (collMgr.outputMaybe & (0x15 << direction)) {
		ret = true;
	} else {
		ret = false;
	}
	if (ret) {
			bool temp = this->left;
			this->left = this->right;
			this->right = temp;
	}
	
	if(this->right) {
		this->rot.y = 0x1500;
	} else if(this->left) {
		this->rot.y = 0xE500;
	}




	if(this->timer > 160) {
		doStateChange(&StateID_Bury);
	}
}

void daEnStretch_c::endState_Walk()
{

}


void daEnStretch_c::beginState_Bury()
{
	bindAnimChr_and_setUpdateRate("in", 1, 0.0, 1.0);
	PlaySoundAsync(this, SE_EMY_CS_TERESA_BRING_IT);

	this->timer = 0;

	PlaySound(this, 0x221);

	this->savedPos = this->pos;

	//SpawnEffect("Wm_mr_sanddive_out", 0, &this->pos, &nullRot, &efScale);
	//SpawnEffect("Wm_mr_sanddive_smk", 0, &this->pos, &nullRot, &efScale);
}

void daEnStretch_c::executeState_Bury()
{
/*	if (this->timer < 60) {
		this->pos.y -= 1.0;  // Height is 54 pixels, move down
	}*/

	this->pos.y -= 0.5f;

	if (this->timer >= 37) {
		this->isBuried = 1;
		doStateChange(&StateID_Wait);
	}

	this->timer += 1;
}

void daEnStretch_c::endState_Bury() {
	this->scale = (Vec){0.0f, 0.0f, 0.0f};
}


void daEnStretch_c::beginState_MoveSand() {
	bindAnimChr_and_setUpdateRate("walk", 1, 0.0, 1.0);
	this->timer = 0;
}

void daEnStretch_c::executeState_MoveSand()
{
	this->timer += 1;

	if (this->timer == 60)
	{
		whichpoint = GenerateRandomNumber(warpNodeCount);
		//SpawnEffect("Wm_mr_sanddive_out", 0, &this->possand, &nullRot, &efScale);
		//SpawnEffect("Wm_mr_sanddive_smk", 0, &this->possand, &nullRot, &efScale);
		SpawnEffect("Wm_mr_sealandsmk_ss", 0, &(Vec){warpNodes[whichpoint]->xPos, -warpNodes[whichpoint]->yPos + 2, pos.z - 5500.0f}, &(S16Vec){0,0,0}, &(Vec){1.0, 1.0, 1.0});
	}

	if (this->timer == 120) {
		
		pos.x = warpNodes[whichpoint]->xPos;
		pos.y = -warpNodes[whichpoint]->yPos - 18.5f;
		doStateChange(&StateID_BackUp);
	}
}

void daEnStretch_c::endState_MoveSand() {}


void daEnStretch_c::beginState_BackUp() {
	this->scale = (Vec){1.0f, 1.0f, 1.0f};
	bindAnimChr_and_setUpdateRate("out", 1, 0.0, 1.0);
	PlaySoundAsync(this, SE_EMY_CS_TERESA_BRING_IT);
	this->timer = 0;

	BackUpEffect = (Vec){ pos.x, this->BaseLine, 0 };

	this->savedPos.x = this->pos.x;
	this->savedPos.y = this->pos.y + 18.5f;
	this->savedPos.z = this->pos.z;

	//SpawnEffect("Wm_mr_sanddive_out", 0, &this->BackUpEffect, &nullRot, &efScale);
	//SpawnEffect("Wm_mr_sanddive_smk", 0, &this->BackUpEffect, &nullRot, &efScale);
}

void daEnStretch_c::executeState_BackUp()
{
/*	if (this->timer < 60) {
		this->pos.y += 1.0;  // Height is 54 pixels, move down
	}*/


	this->pos.y += 0.5f;

	if (this->timer >= 37) {

		doStateChange(&StateID_Walk);
	}

	this->timer += 1;
}

void daEnStretch_c::endState_BackUp()
{ }


void daEnStretch_c::beginState_Outro()
{
	this->timer = 0;
	bindAnimChr_and_setUpdateRate("walk", 1, 0.0, 1.0);

	pos.z = 5000.0;

	this->removeMyActivePhysics();
}

void daEnStretch_c::executeState_Outro()
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

void daEnStretch_c::endState_Outro() { }