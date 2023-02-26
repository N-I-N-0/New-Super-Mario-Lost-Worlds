#include <common.h>
#include <game.h>
#include <profile.h>
#include "path.h"

extern "C" dCourse_c::rail_s *GetRail(int id);

const char* GreatGoalPoleFileList[] = { "goal_set", "wing", 0 };

class daGreatGoalPole_c : public dEnPath_c {
public:
	int onCreate();
	int onExecute();
	int afterExecute(int param);
	int onDelete();
	int onDraw();

	Physics physics;

	mHeapAllocator_c allocator;
	nw4r::g3d::ResFile goalBrres;
	m3d::mdl_c baseA;
	m3d::mdl_c goal_flag;
	nw4r::g3d::ResFile wingBrres;
	m3d::mdl_c wing;
	m3d::anmChr_c anmFlag;
	m3d::anmTexSrt_c flagSrt;
	m3d::anmChr_c anmWing;

	S16Vec baseArot;
	S16Vec wingRot;
	Vec wingScale;

	ActivePhysics a1UPPhysics;
	Physics::Info physicsInfo;
	StandOnTopCollider sotCollider;
	
	bool done;
	
	s32 timer;
	u8 exiting;
	bool touched;

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
	
	void bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate);
	
	USING_STATES(daGreatGoalPole_c);
	
	DECLARE_STATE(Wait);
	DECLARE_STATE(Anim);
	DECLARE_STATE(Touched);
};

CREATE_STATE(daGreatGoalPole_c, Wait);
CREATE_STATE(daGreatGoalPole_c, Anim);
CREATE_STATE(daGreatGoalPole_c, Touched);


void daGreatGoalPole_c::beginState_Wait() {}
void daGreatGoalPole_c::executeState_Wait() {
	if (!done) {
		dStageActor_c* player = (dStageActor_c*)FindActorByType(PLAYER, 0);

		if (player->pos.x >= this->pos.x - 64) {
			doStateChange(&StateID_FollowPath);
		}
	}
}
void daGreatGoalPole_c::endState_Wait() {}



void daGreatGoalPole_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate) {
	nw4r::g3d::ResAnmChr anmChr = this->goalBrres.GetResAnmChr(name);
	this->anmFlag.bind(&this->goal_flag, anmChr, unk);
	this->goal_flag.bindAnim(&this->anmFlag, unk2);
	this->anmFlag.setUpdateRate(rate);
}


const SpriteData GreatGoalPoleSpriteData = { ProfileId::GreatGoalPole, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile GreatGoalPoleProfile(&daGreatGoalPole_c::build, SpriteId::GreatGoalPole, &GreatGoalPoleSpriteData, ProfileId::GreatGoalPole, ProfileId::GreatGoalPole, "TheGreatGoalPole", GreatGoalPoleFileList);


void daGreatGoalPole_c::playerCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	if(!this->touched) {
		doStateChange(&StateID_Touched);
		this->touched = true;
	}
}
void daGreatGoalPole_c::yoshiCollision(ActivePhysics* apThis, ActivePhysics* apOther) {

}
bool daGreatGoalPole_c::collisionCat7_GroundPound(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daGreatGoalPole_c::collisionCat7_GroundPoundYoshi(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daGreatGoalPole_c::collisionCatD_Drill(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daGreatGoalPole_c::collisionCatA_PenguinMario(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}

bool daGreatGoalPole_c::collisionCat1_Fireball_E_Explosion(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daGreatGoalPole_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daGreatGoalPole_c::collisionCat9_RollingObject(ActivePhysics* apThis, ActivePhysics* apOther) {

	return false;
}
bool daGreatGoalPole_c::collisionCat13_Hammer(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daGreatGoalPole_c::collisionCat14_YoshiFire(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}

bool daGreatGoalPole_c::collisionCat3_StarPower(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}


dActor_c* daGreatGoalPole_c::build() {
	void* buffer = AllocFromGameHeap1(sizeof(daGreatGoalPole_c));
	daGreatGoalPole_c* c = new(buffer) daGreatGoalPole_c;

	return c;
}


int daGreatGoalPole_c::onCreate() {
	this->deleteForever = true;

	// Model creation	
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	this->goalBrres.data = getResource("goal_set", "g3d/t00.brres");
	nw4r::g3d::ResMdl baseAMdl = this->goalBrres.GetResMdl("baseA");
	nw4r::g3d::ResMdl goal_flagMdl = this->goalBrres.GetResMdl("goal_flag");
	

	
	this->wingBrres.data = getResource("wing", "g3d/wing.brres");
	nw4r::g3d::ResMdl wingMdl = this->wingBrres.GetResMdl("wing");
	

	
	this->baseA.setup(baseAMdl, &allocator, 0x224, 1, 0);
	this->goal_flag.setup(goal_flagMdl, &allocator, 0x224, 1, 0);
	this->wing.setup(wingMdl, &allocator, 0x224, 1, 0);
	
	SetupTextures_MapObj(&this->baseA, 0);
	SetupTextures_MapObj(&this->goal_flag, 0);
	SetupTextures_Enemy(&this->wing, 0);


	
	nw4r::g3d::ResAnmChr anmChr = this->goalBrres.GetResAnmChr("wait");
	this->anmFlag.setup(goal_flagMdl, anmChr, &this->allocator, 0);
	
	
	nw4r::g3d::ResAnmTexSrt anmSrt = this->goalBrres.GetResAnmTexSrt("wait");
	this->flagSrt.setup(goal_flagMdl, anmSrt, &this->allocator, 0, 1);
	this->flagSrt.setEntryByte34(0, 0);


	nw4r::g3d::ResAnmChr anmChr2 = this->wingBrres.GetResAnmChr("wing_kuri");
	this->anmWing.setup(wingMdl, anmChr2, &this->allocator, 0);
	
	
	
	//bindAnimChr_and_setUpdateRate("wait", 1, 0.0, 1.0);
	this->anmFlag.bind(&this->goal_flag, anmChr, 1);
	this->goal_flag.bindAnim(&this->anmFlag, 0.0);
	this->anmFlag.setUpdateRate(1.0);

	this->flagSrt.bindEntry(&this->goal_flag, anmSrt, 0, 1);
	this->goal_flag.bindAnim(&this->flagSrt, 1.0);
	this->flagSrt.setFrameForEntry(1.0, 0);
	this->flagSrt.setUpdateRateForEntry(1.0, 0);

	this->anmWing.bind(&this->wing, anmChr2, 1);
	this->wing.bindAnim(&this->anmWing, 0.0);
	this->anmWing.setUpdateRate(1.0);
	

	allocator.unlink();

	//goal_flag
	ActivePhysics::Info aPhysicsInfo;
	aPhysicsInfo.xDistToCenter = 0.0;
	aPhysicsInfo.yDistToCenter = 72.0;
	aPhysicsInfo.category1 = 0x5;
	aPhysicsInfo.category2 = 0x0;
	aPhysicsInfo.bitfield1 = 0x4F;

	aPhysicsInfo.bitfield2 = 0x200;
	aPhysicsInfo.xDistToEdge = 3;
	aPhysicsInfo.yDistToEdge = 76;

	aPhysicsInfo.unkShort1C = 0;
	aPhysicsInfo.callback = &dEn_c::collisionCallback;

	this->aPhysics.initWithStruct(this, &aPhysicsInfo);
	this->aPhysics.addToList();

	//goal_flag 1UP
	ActivePhysics::Info a1UPPhysicsInfo;
	a1UPPhysicsInfo.xDistToCenter = 0.0;
	a1UPPhysicsInfo.yDistToCenter = 151.0;
	a1UPPhysicsInfo.category1 = 0x5;
	a1UPPhysicsInfo.category2 = 0x0;
	a1UPPhysicsInfo.bitfield1 = 0x4F;

	a1UPPhysicsInfo.bitfield2 = 0x200;
	a1UPPhysicsInfo.xDistToEdge = 3;
	a1UPPhysicsInfo.yDistToEdge = 3;

	a1UPPhysicsInfo.unkShort1C = 0;
	a1UPPhysicsInfo.callback = &dEn_c::collisionCallback;

	this->a1UPPhysics.initWithStruct(this, &a1UPPhysicsInfo);
	this->a1UPPhysics.addToList();

	//baseA
	physicsInfo.x1 = -16;
	physicsInfo.y1 = 15.5;
	physicsInfo.x2 = 16;
	physicsInfo.y2 = 0;

	physics.setup(this, &physicsInfo, 1, 0, 0);
	physics.flagsMaybe = 0x260;
	physics.callback1 = (void*)&PhysCB4;
	physics.callback2 = (void*)&PhysCB5;
	physics.callback3 = (void*)&PhysCB6;
	physics.addToList();
	
	sotCollider.init(this, 0, 16, 0, 16, -16, 0, 1);
	sotCollider._47 = 0xA;
	sotCollider.flags = 0x80180 | 0xC00;
	sotCollider.addToList();
	
	done = false;
	
	this->disableEatIn();

	baseArot = (S16Vec){0, 0, 0};
	wingRot = (S16Vec){0, 0x4000, 0};
	
	this->scale = (Vec){1, 1, 1};
	this->wingScale = (Vec){2, 2, 2};

	this->rot.x = 0;
	this->rot.y = 0xC000; //0x8000;
	this->rot.z = 0;

	this->pos.z = 4000;

	beginState_Init();
	executeState_Init();
	doStateChange(&StateID_Wait);
	
	return true;
}


int daGreatGoalPole_c::onDelete() {
	physics.removeFromList();
	return true;
}

int daGreatGoalPole_c::onDraw() {
	baseA.scheduleForDrawing();
	goal_flag.scheduleForDrawing();
	wing.scheduleForDrawing();
	return true;
}


void daGreatGoalPole_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&baseArot.x, &baseArot.y, &baseArot.z);
	baseA.setDrawMatrix(matrix);
	baseA.setScale(&scale);
	baseA.calcWorld(false);

	matrix.translation(pos.x, pos.y, pos.z + 5);
	matrix.applyRotationYXZ(&wingRot.x, &wingRot.y, &wingRot.z);
	wing.setDrawMatrix(matrix);
	wing.setScale(&wingScale);
	wing.calcWorld(false);

	matrix.translation(pos.x, pos.y+85, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);
	goal_flag.setDrawMatrix(matrix);
	goal_flag.setScale(&scale);
	goal_flag.calcWorld(false);
}

int daGreatGoalPole_c::onExecute() {
	baseA._vf1C();
	goal_flag._vf1C();
	wing._vf1C();
	updateModelMatrices();
	acState.execute();

	if (this->anmFlag.isAnimationDone()) {
		this->anmFlag.setCurrentFrame(0.0);
	}
	
	if(acState.getCurrentState()->isEqual(&StateID_FollowPath)){
		if (this->anmWing.isAnimationDone()) {
			this->anmWing.setCurrentFrame(0.0);
		}
	} else {
		this->anmWing.setCurrentFrame(anmWing.numFrames);
	}
	
	this->flagSrt.process();
	if(this->flagSrt.isEntryAnimationDone(0))
		this->flagSrt.setFrameForEntry(1.0, 0);
	
	sotCollider.update();
	physics.update();
	return true;
}

int daGreatGoalPole_c::afterExecute(int param) {
	dEn_c::afterExecute(param);
	a1UPPhysics.clear();
	return true;
}
















static nw4r::snd::StrmSoundHandle greatGoalHandle;







void daGreatGoalPole_c::beginState_Touched() {
	this->timer = 0;
	//this->isGlowing = false;
	//cosmicKillFlag = 1;

	// WLClass::instance->flags |= 4;
	WLClass::instance->_4 = 5;
	WLClass::instance->_8 = 0;
	dStage32C_c::instance->freezeMarioBossFlag = 1;

	// MakeMarioEnterDemoMode();
	StopBGMMusic();

	//PlaySoundWithFunctionB4(SoundRelatedClass, &starSoundHandle, 767, 1); // SE_OBJ_BROOM_KEY_GET

	S16Vec nullRot = {0,0,0};
	Vec twoVec = {2.0f, 2.0f, 2.0f};
	SpawnEffect("Wm_ob_keyget01", 0, &this->pos, &nullRot, &twoVec);
}

void daGreatGoalPole_c::executeState_Touched() {
	bool isOnGround = true;
	for(int i = 0; i < 4; i++) {
		if(Player_Active[i] && Player_Lives[i] > 0) {
			if(!(GetPlayerOrYoshi(i)->collMgr.isOnTopOfTile())) {
				// OSReport("Player %d isn't on ground\n", i);
				isOnGround = false;
			}
		}
	}

	// OSReport("isOnGround: %d\n", isOnGround);

	if(isOnGround) {
		this->timer++;
	}

	if(this->timer == 30) {
		doStateChange(&StateID_Anim);
	}
}

void daGreatGoalPole_c::endState_Touched() {}







void daGreatGoalPole_c::beginState_Anim() {
	this->timer = 0;
	WLClass::instance->_4 = 5;
	WLClass::instance->_8 = 0;
	dStage32C_c::instance->freezeMarioBossFlag = 1;
	// WLClass::instance->flags |= 0xf;

	//if(!this->isCastleStar) 
	//	PlaySoundWithFunctionB4(SoundRelatedClass, &greatGoalHandle, STRM_BGM_COURSE_CLEAR, 1);
	//else
		PlaySoundWithFunctionB4(SoundRelatedClass, &greatGoalHandle, STRM_BGM_SHIRO_BOSS_CLEAR, 1);

	BossGoalForAllPlayers();
}

void daGreatGoalPole_c::executeState_Anim() {
	this->timer++;
	// OSReport("_4 & _8: %d %d\n", WLClass::instance->_4, WLClass::instance->_8);
	// OSReport("flags: %d %d\n", WLClass::instance->flags, *((u32*)(*((u32*)(WLClass::instance->flags)) + 0x10)));

	if(this->timer == 20) {
		WLClass::instance->_4 = 5;
		WLClass::instance->_8 = 0;
	}
	else if(this->timer == 50) {
		UpdateGameMgr();
		*((u8*)((u32)(dCourseClear_c::instance) + 0x375)) = 1;
	}
	else if(this->timer == 230) {
		dGameDisplay_c::instance->_44C = 1;
		*((u8*)((u32)(dCourseClear_c::instance) + 0x376)) = 1;
	}

	if(this->exiting) {
		this->exiting++;
	}
	//else if(!strcmp(dGameDisplay_c::instance->state.getCurrentState()->getName(), "dGameDisplay_c::StateID_ProcGoalEnd")) { // For some reason, strcmp returns true if the string ISN'T what it was compared to; weird. EDIT: that's actually normal
	else if(dGameDisplay_c::instance->state.getCurrentState() == &dGameDisplay_c::StateID_ProcGoalEnd) {
		this->exiting = 1;
	}

	if(this->exiting == 60) {
		ExitStage(WORLD_MAP, 0, BEAT_LEVEL, MARIO_WIPE);
	}
}

void daGreatGoalPole_c::endState_Anim() {}





