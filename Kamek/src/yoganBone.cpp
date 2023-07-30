#include <common.h>
#include <game.h>
#include "profile.h"
#include <dCourse.h>
#include <g3dhax.h>
#include <sfx.h>
#include <stage.h>
#include "boss.h"
#include "path.h"

const char* YoganBoneFileList[] = { "yogan_bone", NULL };

class daEnYoganBone_c : public dEnBlockPath_c {
public:
	enum YoganBoneType {
		HeadType,			// 0
		BodyLargeType,		// 1
		BodySmallType,		// 2
		BodyMediumType,		// 3
	};

	enum YoganBoneBehaviour {
		StartUpBehaviour,		// 0
		StartDownBehaviour,		// 1
		DontMoveBehaviour,		// 2
		UnknownBehaviour,		// 3
	};

	Physics::Info physicsInfo;
	StandOnTopCollider standOnTopCollider;

	int onCreate();
	int onDelete();
	int onExecute();
	int onDraw();

	void calledWhenUpMoveExecutes();
	void calledWhenDownMoveExecutes();

	void blockWasHit(bool isDown);
	void executeNotDone();

	bool playerOverlaps();

	mHeapAllocator_c allocator;
	nw4r::g3d::ResFile resFile;
	m3d::mdl_c model;
	m3d::anmChr_c animationChr;

	int timer;

	YoganBoneType type;
	YoganBoneBehaviour behaviour;

	bool eventActivated;
	bool wasHitAnim;
	int mouthOpen;
	bool ranOnce;

	u64 eventFlag;
	u8 lastEvState;
	u8 newEvState;
	u8 offState;

	nw4r::snd::SoundHandle handle; // Sound Handle

	static dActor_c *build();

	USING_STATES(daEnYoganBone_c);
	DECLARE_STATE(InitWait);
	DECLARE_STATE(FollowPath);
	DECLARE_STATE(Done);

	void bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate);
};


dActor_c *daEnYoganBone_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daEnYoganBone_c));
	daEnYoganBone_c *c = new(buffer) daEnYoganBone_c;

	return c;
}

const SpriteData YoganBoneSpriteData = { ProfileId::YoganBone, 64, -32, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile YoganBoneProfile(&daEnYoganBone_c::build, SpriteId::YoganBone, &YoganBoneSpriteData, ProfileId::YoganBone, ProfileId::YoganBone, "YoganBone", YoganBoneFileList, 0);


/*****************************************************************************/
// Glue Code
extern "C" void HurtMarioBecauseOfBeingSquashed(void *mario, dStageActor_c *squasher, int type);

static void YoganBonePhysCB1(daEnYoganBone_c *one, dStageActor_c *two) {
	void *callback = &daEnBlockMain_c::OPhysicsCallback1;
	((void (*)(daEnBlockMain_c *, dStageActor_c *)) callback)(one, two);

	if (two->stageActorType != 1)
		return;

	if (one->pos_delta.y > 0.0f)
		HurtMarioBecauseOfBeingSquashed(two, one, 1);
	else
		HurtMarioBecauseOfBeingSquashed(two, one, 9);
}

static void YoganBonePhysCB2(daEnYoganBone_c *one, dStageActor_c *two) {
	void *callback = &daEnBlockMain_c::OPhysicsCallback2;
	((void (*)(daEnBlockMain_c *, dStageActor_c *)) callback)(one, two);

	if (two->stageActorType != 1)
		return;

	if (one->pos_delta.y < 0.0f)
		HurtMarioBecauseOfBeingSquashed(two, one, 2);
	else
		HurtMarioBecauseOfBeingSquashed(two, one, 10);
}

static void YoganBonePhysCB3(daEnYoganBone_c *one, dStageActor_c *two, bool unkMaybeNotBool) {
	void *callback = &daEnBlockMain_c::OPhysicsCallback3;
	((void (*)(daEnBlockMain_c *, dStageActor_c *)) callback)(one, two);

	if (two->stageActorType != 1)
		return;

	if (unkMaybeNotBool) {
		if (one->pos_delta.x > 0.0f)
			HurtMarioBecauseOfBeingSquashed(two, one, 6);
		else
			HurtMarioBecauseOfBeingSquashed(two, one, 12);
	} else {
		if (one->pos_delta.x < 0.0f)
			HurtMarioBecauseOfBeingSquashed(two, one, 5);
		else
			HurtMarioBecauseOfBeingSquashed(two, one, 11);
	}
}

static bool YoganBonePhysCB4(daEnYoganBone_c *one, dStageActor_c *two) {
	void *callback = &daEnBlockMain_c::PhysicsCallback1;
	((void (*)(daEnBlockMain_c *, dStageActor_c *)) callback)(one, two);

	return (one->pos_delta.y > 0.0f);
}

static bool YoganBonePhysCB5(daEnYoganBone_c *one, dStageActor_c *two) {
	void *callback = &daEnBlockMain_c::PhysicsCallback2;
	((void (*)(daEnBlockMain_c *, dStageActor_c *)) callback)(one, two);

	return (one->pos_delta.y < 0.0f);
}

static bool YoganBonePhysCB6(daEnYoganBone_c *one, dStageActor_c *two, bool unkMaybeNotBool) {
	void *callback = &daEnBlockMain_c::PhysicsCallback3;
	((void (*)(daEnBlockMain_c *, dStageActor_c *)) callback)(one, two);

	if (unkMaybeNotBool) {
		if (one->pos_delta.x > 0.0f)
			return true;
	} else {
		if (one->pos_delta.x < 0.0f)
			return true;
	}
	return false;
}




#define ACTIVATE	1
#define DEACTIVATE	0


CREATE_STATE(daEnYoganBone_c, InitWait);
CREATE_STATE(daEnYoganBone_c, FollowPath);
CREATE_STATE(daEnYoganBone_c, Done);



void daEnYoganBone_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate) {
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr(name);
	this->animationChr.bind(&this->model, anmChr, unk);
	this->model.bindAnim(&this->animationChr, unk2);
	this->animationChr.setUpdateRate(rate);
}

int daEnYoganBone_c::onCreate() {
	if(!this->ranOnce) {
		this->ranOnce = true;
		return false;
	}

	allocator.link(-1, GameHeaps[0], 0, 0x20);

	this->eventActivated = false;
	this->wasHitAnim = false;
	this->mouthOpen = 0;

	this->lastEvState = 0xF;
	char eventNum	= this->eventId2 & 0xF;
	this->eventFlag = (u64)1 << (eventNum - 1);

	this->type = (YoganBoneType)((this->eventId1 & 0xC) >> 2);
	this->behaviour = (YoganBoneBehaviour)(this->eventId1 & 0x3);
	int color = (this->eventId1 & 0xF0) >> 4;
	int groupId = (this->settings >> 24) & 0xFF;
	bool facingLeft = (this->settings >> 20) & 0xF;
	this->ignoreLastNodesCount = (this->eventId2 & 0xF0) >> 4;

	char resName[24];
	sprintf(resName, "g3d/t%02d_%d.brres", color, this->type);
	resName[strlen(resName)] = 0;
	resFile.data = getResource("yogan_bone", resName);

	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("yogan_bone");
	model.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Enemy(&model, 0);

	bool ret;
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr("do_nothing");
	ret = this->animationChr.setup(mdl, anmChr, &this->allocator, 0);

	allocator.unlink();


	blockInit(pos.y);
	this->pos.z = 150.0;

	if (this->type == HeadType) {
		physicsInfo.x1 = -32;
		physicsInfo.y1 = 30;
		physicsInfo.x2 = 40;
		physicsInfo.y2 = -48;
	}
	else if (this->type == BodyLargeType) {
		physicsInfo.x1 = -56;
		physicsInfo.y1 = 32;
		physicsInfo.x2 = 56;
		physicsInfo.y2 = -48;
	}
	else if (this->type == BodySmallType) {
		physicsInfo.x1 = -32;
		physicsInfo.y1 = 32;
		physicsInfo.x2 = 32;
		physicsInfo.y2 = -48;
	}
	else if (this->type == BodyMediumType) {
		physicsInfo.x1 = -40;
		physicsInfo.y1 = 32;
		physicsInfo.x2 = 40;
		physicsInfo.y2 = -48;
	}

	physicsInfo.otherCallback1 = (void*)&YoganBonePhysCB1;
	physicsInfo.otherCallback2 = (void*)&YoganBonePhysCB2;
	physicsInfo.otherCallback3 = (void*)&YoganBonePhysCB3;

	physics.setup(this, &physicsInfo, 3, currentLayerID, 0);

	physics.flagsMaybe = 0x260;
	physics.callback1 = (void*)&YoganBonePhysCB4;
	physics.callback2 = (void*)&YoganBonePhysCB5;
	physics.callback3 = (void*)&YoganBonePhysCB6;

	physics.addToList();

	standOnTopCollider.init(this, 0, 0, physicsInfo.y1 + 0.0001, physicsInfo.x2, physicsInfo.x1, 0, 1);
	standOnTopCollider._47 = 0xA;
	standOnTopCollider.flags = 0x80180 | 0xC00;
	standOnTopCollider.addToList();

	this->spriteSomeRectX = 99999.0f;
	this->spriteSomeRectY = 99999.0f;

	if (this->behaviour == StartUpBehaviour) this->timer = 0;
	else if (this->behaviour == StartDownBehaviour) this->timer = 148;
	else this->timer = 0;

	bindAnimChr_and_setUpdateRate("do_nothing", 1, 0.0, 1.0);

	dEnBlockPath_c::beginState_Init();

	if (this->changeToDone) {
		changeToDone = false;

		switch (this->type) { // Reposition the sprite based on type
			case HeadType:
				pos.x += -24;
				pos.y += 16;
				break;

			case BodySmallType:
				pos.x += -24;
				break;

			case BodyMediumType:
				pos.x += -16;
				break;

			default:
				break;
		}

		doStateChange(&daEnYoganBone_c::StateID_Done);
	}
	else {
		switch (this->type) {
			case HeadType:
				offset.x = 32;
				offset.y = -8;
				break;

			case BodyLargeType:
				offset.x = 56;
				offset.y = -32;
				break;

			case BodySmallType:
				offset.x = 32;
				offset.y = -32;
				break;

			case BodyMediumType:
				offset.x = 40;
				offset.y = -32;
				break;

			default:
				break;
		}

		this->pos.x = this->currentNode->xPos + this->offset.x;
		this->pos.y = (-this->currentNode->yPos) + this->offset.y;

		doStateChange(&daEnYoganBone_c::StateID_InitWait);
	}

	return true;
}


int daEnYoganBone_c::onDelete() {
	physics.removeFromList();
	standOnTopCollider.removeFromList();

	return true;
}


int daEnYoganBone_c::onExecute() {
	acState.execute();
	physics.update();
	standOnTopCollider.update();

	blockUpdate();

	// now check zone bounds based on state
	// if (acState.getCurrentState()->isEqual(&StateID_Wait)) {
	// 	checkZoneBoundaries(0);
	// }

	this->timer++;

	if (this->behaviour <= StartDownBehaviour) {
		float updown = cos(((this->timer * 2.0 * M_PI) / 296.0)) * 0.25;

		if (this->timer > 296) {
			this->timer = 0;
		}

		this->pos.y += updown;
		this->offset.y += updown;
	}

	return true;
}


int daEnYoganBone_c::onDraw() {
	model.scheduleForDrawing();
	model._vf1C();

	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	model.setDrawMatrix(matrix);
	model.setScale(&scale);
	model.calcWorld(false);
	model.scheduleForDrawing();

	return true;
}


void daEnYoganBone_c::blockWasHit(bool isDown) {
	pos.y = initialY;
}



void daEnYoganBone_c::calledWhenUpMoveExecutes() {
	if (initialY >= pos.y)
		blockWasHit(false);
}

void daEnYoganBone_c::calledWhenDownMoveExecutes() {
	if (initialY <= pos.y)
		blockWasHit(true);
}



void daEnYoganBone_c::executeNotDone() {
	int result = blockResult();

	if (result == 1) {
		anotherFlag = 2;
		isGroundPound = false;
	}
	else if (result != 0) {
		anotherFlag = 1;
		isGroundPound = true;

		if (this->type == HeadType) bindAnimChr_and_setUpdateRate("shake", 1, 0.0, 1.0);
		else bindAnimChr_and_setUpdateRate("shake_body", 1, 0.0, 1.0);

		playSoundDistance(this->handle, this->pos, SE_EMY_LIFT_LINE_HPDP_BODY);
		this->wasHitAnim = true;
	}

	if (this->wasHitAnim && this->animationChr.isAnimationDone()) {
		this->wasHitAnim = false;
		bindAnimChr_and_setUpdateRate("do_nothing", 1, 0.0, 1.0);
	}
}



///////////////
// InitWait State
///////////////
void daEnYoganBone_c::beginState_InitWait() { }
void daEnYoganBone_c::executeState_InitWait() {
	this->executeNotDone();

	if (this->eventActivated) {
		playSoundDistance(this->handle, this->pos, SE_EMY_LIFT_LINE_ACTIVE);
		return doStateChange(&daEnYoganBone_c::StateID_FollowPath);
	}

	this->newEvState = 0;
	if (dFlagMgr_c::instance->flags & this->eventFlag)
		this->newEvState = 1;
	
	if (this->newEvState == this->lastEvState)
		return;

	if (this->newEvState == ACTIVATE) {
		this->offState = (this->newEvState == 1) ? 1 : 0;
		this->eventActivated = true;
	}
	
	else {
		this->offState = (this->newEvState == 1) ? 0 : 1;
	}

	this->lastEvState = this->newEvState;
}
void daEnYoganBone_c::endState_InitWait() { }

///////////////
// FollowPath State
///////////////
void daEnYoganBone_c::beginState_FollowPath() {
	dEnBlockPath_c::beginState_FollowPath();
}
void daEnYoganBone_c::executeState_FollowPath() {
	this->executeNotDone();
	dEnBlockPath_c::executeState_FollowPath();
}
void daEnYoganBone_c::endState_FollowPath() {
	dEnBlockPath_c::endState_FollowPath();
	doStateChange(&daEnYoganBone_c::StateID_Done);
}

///////////////
// Done State
///////////////
void daEnYoganBone_c::beginState_Done() {
	dEnBlockPath_c::beginState_Done();
}
void daEnYoganBone_c::executeState_Done() {
	dEnBlockPath_c::executeState_Done();

	int result = blockResult();

	if (result == 1) {
		anotherFlag = 2;
		isGroundPound = false;
	}
	else if (result != 0) {
		anotherFlag = 1;
		isGroundPound = true;

		if (this->type == HeadType) {
			if (this->mouthOpen == 0 || this->mouthOpen == 4) {
				if (this->acState.getCurrentState() == &StateID_Done && this->mouthOpen == 0) {
					bindAnimChr_and_setUpdateRate("mouth_open", 1, 0.0, 1.0);
					this->mouthOpen = 1;
				} else {
					bindAnimChr_and_setUpdateRate("shake", 1, 0.0, 1.0);
					playSoundDistance(this->handle, this->pos, SE_EMY_LIFT_LINE_HPDP_BODY);
					this->wasHitAnim = true;
				}
			}
		}
		else {
			bindAnimChr_and_setUpdateRate("shake_body", 1, 0.0, 1.0);
			playSoundDistance(this->handle, this->pos, SE_EMY_LIFT_LINE_HPDP_BODY);
			this->wasHitAnim = true;
		}
	}

	if (this->wasHitAnim && this->animationChr.isAnimationDone()) {
		this->wasHitAnim = false;
		bindAnimChr_and_setUpdateRate("do_nothing", 1, 0.0, 1.0);
	}

	switch (this->mouthOpen) {
		case 1:
			if (this->animationChr.currentFrame >= 21 && this->animationChr.currentFrame < 128) {
				float s = sin(((this->animationChr.currentFrame - 21.0) * M_PI) / 107.0);
				physicsInfo.y1 += s * (50.0 / 107.0);
				physics.setup(this, &physicsInfo, 3, currentLayerID);
				standOnTopCollider.init(this, 0, 0, physicsInfo.y1 + 0.1, physicsInfo.x2, physicsInfo.x1, 0, 1);
			}

			if (this->animationChr.isAnimationDone()) {
				bindAnimChr_and_setUpdateRate("coin_exhale", 1, 0.0, 1.0);
				this->mouthOpen = 2;
			}
			break;

		case 2:
			float s = sin((((int)(this->animationChr.currentFrame) % 10) * 2.0 * M_PI) / 10.0);
			physicsInfo.y1 += s * 0.9;
			physics.setup(this, &physicsInfo, 3, currentLayerID);
			standOnTopCollider.init(this, 0, 0, physicsInfo.y1 + 0.1, physicsInfo.x2, physicsInfo.x1, 0, 1);

			if (this->timer % 8 == 0) {
				dStageActor_c* coin = dStageActor_c::create(EN_COIN_JUGEM, 0, &((Vec){this->pos.x + 32, this->pos.y - 12, this->pos.z}), 0, 0);
				coin->speed.x = 2.0 + (MakeRandomNumber(20) / 10.0);
				coin->speed.y = 20.0 + (MakeRandomNumber(40) / 10.0);
			}

			if (this->animationChr.isAnimationDone()) {
				bindAnimChr_and_setUpdateRate("mouth_close", 1, 0.0, 1.0);
				this->mouthOpen = 3;
			}
			break;

		case 3:
			if (this->animationChr.currentFrame >= 9 && this->animationChr.currentFrame < 88) {
				float s = sin(((this->animationChr.currentFrame - 9.0) * M_PI) / 79.0);
				physicsInfo.y1 -= s * (50.0 / 79.0);
				physics.setup(this, &physicsInfo, 3, currentLayerID);
				standOnTopCollider.init(this, 0, 0, physicsInfo.y1 + 0.1, physicsInfo.x2, physicsInfo.x1, 0, 1);
			}

			if (this->animationChr.isAnimationDone()) {
				bindAnimChr_and_setUpdateRate("do_nothing", 1, 0.0, 1.0);
				this->mouthOpen = 4;
			}
			break;
	}
}
void daEnYoganBone_c::endState_Done() {
	dEnBlockPath_c::endState_Done();
}



bool daEnYoganBone_c::playerOverlaps() {
	dStageActor_c *player = 0;

	Vec myBL = {pos.x - 32.0f, pos.y - 48.0f, 0.0f};
	Vec myTR = {pos.x + 40.0f, pos.y + 32.0f, 0.0f};

	while ((player = (dStageActor_c*)fBase_c::search(PLAYER, player)) != 0) {
		float centerX = player->pos.x + player->aPhysics.info.xDistToCenter;
		float centerY = player->pos.y + player->aPhysics.info.yDistToCenter;

		float left = centerX - player->aPhysics.info.xDistToEdge;
		float right = centerX + player->aPhysics.info.xDistToEdge;

		float top = centerY + player->aPhysics.info.yDistToEdge;
		float bottom = centerY - player->aPhysics.info.yDistToEdge;

		Vec playerBL = {left, bottom + 0.1f, 0.0f};
		Vec playerTR = {right, top - 0.1f, 0.0f};

		if (RectanglesOverlap(&playerBL, &playerTR, &myBL, &myTR))
			return true;
	}

	return false;
}

