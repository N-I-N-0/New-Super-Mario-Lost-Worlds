#include <common.h>
#include <game.h>
#include <g3dhax.h>

#include "daEnItem_c.h"

const char* FishingBooFileList [] = { "FishingBoo", NULL };

class daFishinBoo_c : public dEn_c {
public:
	int onCreate();
	int onDelete();
	int onExecute();
	int onDraw();

	mHeapAllocator_c allocator;
	nw4r::g3d::ResFile resFile;
	m3d::mdl_c bodyModel;
	m3d::anmChr_c chrAnimation;

	u32 hookNodeID;
	dStageActor_c* obj;
	bool updatePos;
	Vec ropePos;
	u32 objID;
	float ySpeed;
	u32 timer;

	static dActor_c *build();

	void bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate);
	void updateModelMatrices();

	USING_STATES(daFishinBoo_c);
	DECLARE_STATE(Follow);
	DECLARE_STATE(FlyAway);
};

dActor_c *daFishinBoo_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daFishinBoo_c));
	return new(buffer) daFishinBoo_c;
}

const SpriteData FishingBooSpriteData = { ProfileId::FishingBoo, 8, -8, 0, 0, 0x10, 0x10, 0, 0, 0, 0, 0 };
Profile FishingBooProfile(&daFishinBoo_c::build, SpriteId::FishingBoo, &FishingBooSpriteData, ProfileId::FishingBoo, ProfileId::FishingBoo, "Fishin' Boo", FishingBooFileList);




CREATE_STATE(daFishinBoo_c, Follow);
CREATE_STATE(daFishinBoo_c, FlyAway);



int daFishinBoo_c::onCreate() {
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	this->resFile.data = getResource("FishingBoo", "g3d/FishingBoo.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("FishingBoo");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Enemy(&bodyModel, 0);
	
	nw4r::g3d::ResNode hook = mdl.GetResNode("PowerUp");
	hookNodeID = hook.GetID();
	
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr("test_short_12deg");
	this->chrAnimation.setup(mdl, anmChr, &this->allocator, 0);

	allocator.unlink();
	
	bindAnimChr_and_setUpdateRate("test_short_12deg", 1, 0.0, 1.0);

	this->scale = (Vec){3, 3, 3};

	this->pos.z = -4000.0f;

	this->rot.y = -0x800;

	this->timer = 30*(this->settings >> 16 & 0xFF);

	Actors content = EN_ITEM;
	u32 set = 0;
	u32 selectedContent = this->settings & 0xFF;
	OSReport("selectedContent: %d\n", selectedContent);
	if(selectedContent == 1) {
		selectedContent = GenerateRandomNumber(19);
	}
	switch(selectedContent) {
		case 0:
			return true;
		case 1:
			break;
		case 2:
			set = 0x0B000000;
			break;
		case 3:
			set = 0x0B000009;
			break;
		case 4:
			set = 0x0B000015;
			break;
		case 5:
			set = 0x0B00000E;
			break;
		case 6:
			set = 0x0B000011;
			break;
		case 7:
			set = 0x0B000019;
			break;
		case 8:
			set = 0x0B000001;
			break;
		case 9:
			set = 0x0B000007;
			break;
		case 10:
			set = 0x0B000006;
			break;
		case 11:
			set = 0x0B000014;
			break;
		case 12:
			set = 0x0B000010;
			break;
		case 13:
			set = 0x0B00000F;
			break;
		case 14:
			set = 0x0B000013;
			break;
		case 15:
			set = 0x0B000016;
			break;
		case 16:
			set = 0x0B000012;
			break;
		case 17:
			content = AC_YOSHI_EGG;
			u8 color = this->settings >> 8 & 0xF;	//0 = light blue; 1 = pink; 2 = yellow; 3 = green; 4 = blue; 5 = red; 6 = orange; 7 = brown; 8 = white; 9 = purple.
			set = (color > 9 ? 0 : (u32)(1 << 21 | color));
			break;
		case 18:
			content = EN_COIN;
			break;
		case 19:
			//spawnPos.y += 10;
			content = PoisonShroom;
			//set = 0x008003cc04060000;
			break;
		case 20:
			content = EN_STAR_COIN;
			set = this->settings & 0x00000300;
			break;
		default:
			break;
	}

	OSReport("selectedContent: %d\n", selectedContent);
	OSReport("content: %d\n", content);

	Vec tempPos = (Vec){this->pos.x, this->pos.y, this->pos.z};
	//bodyModel.getNodeWorldMtxMultVecZero(hookNodeID, &ropePos);
	this->obj = CreateActor(content, set, tempPos, 0, 0);
	//this->obj = (dActor_c*)dStageActor_c::create(content, set, &ropePos, 0, 0);

	this->objID = this->obj->id;

	if (content == EN_ITEM) {
		daEnItem_c* item = ((daEnItem_c*)this->obj);
		item->doStateChange(&daEnItem_c::StateID_Wait);
		item->AddPhysics();
	}

	doStateChange(&StateID_Follow);

	this->onExecute();
	return true;
}

int daFishinBoo_c::onDelete() {
	return true;
}

int daFishinBoo_c::onExecute() {
	acState.execute();
	updateModelMatrices();
	bodyModel._vf1C();

	this->obj = (dStageActor_c*)fBase_c::search(this->objID);
	if(this->obj) {

		//OSReport("State: %s\n", ((dEn_c*)this->obj)->acState.getCurrentState()->getName());
		bodyModel.getNodeWorldMtxMultVecZero(hookNodeID, /*&this->obj->pos*/&ropePos);
		//OSReport("rope pos: %f, %f, %f, %d\n", ropePos.x, ropePos.y, ropePos.z, a);
		//OSReport("boo  pos: %f, %f, %f\n", pos.x, pos.y, pos.z);
		this->obj->pos.x = ropePos.x;
		this->obj->pos.y = ropePos.y;
		this->obj->pos.z = ropePos.z;

	} else {
		if (this->acState.getCurrentState() != &StateID_FlyAway) {
			doStateChange(&StateID_FlyAway);
		}
	}

	if(this->chrAnimation.isAnimationDone()) {
		this->chrAnimation.setCurrentFrame(0.0);
	}

	if(this->acState.getCurrentState() == &StateID_FlyAway) {
		float rect[] = {this->_320, this->_324, this->spriteSomeRectX, this->spriteSomeRectY};
		int ret = this->outOfZone(this->pos, (float*)&rect, this->currentZoneID);
		if(ret) {
			if(this->obj) {this->obj->Delete(1);}
			this->Delete(1);
		}
	} else {
		checkZoneBoundaries(0);
	}

	return true;
}

int daFishinBoo_c::onDraw() {
	bodyModel.scheduleForDrawing();

	return true;
}


void daFishinBoo_c::updateModelMatrices() {
	// This won't work with wrap because I'm lazy.
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

void daFishinBoo_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate) {
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr(name);
	this->chrAnimation.bind(&this->bodyModel, anmChr, unk);
	this->bodyModel.bindAnim(&this->chrAnimation, unk2);
	this->chrAnimation.setUpdateRate(rate);
}


// Follow State

void daFishinBoo_c::beginState_Follow() {}
void daFishinBoo_c::executeState_Follow() { 
	float speedDelta = 0.15;

	this->speed.x = this->speed.x + speedDelta;

	if (this->speed.x > 0) { this->speed.x -= (speedDelta / 2); }
	if (this->speed.x > 2.0) { this->speed.x -= (speedDelta); }

	this->HandleXSpeed();

	this->speed.y = 0.0f;

	this->HandleYSpeed();

	this->UpdateObjectPosBasedOnSpeedValuesReal();

	if(this->timer == 0) {
		doStateChange(&StateID_FlyAway);
	} else {
		this->timer--;
	}
}
void daFishinBoo_c::endState_Follow() {}



















// FlyAway State
void daFishinBoo_c::beginState_FlyAway() {
	this->ySpeed = 1.0f;
}
void daFishinBoo_c::executeState_FlyAway() {
	float speedDelta = 0.15;

	this->speed.x = this->speed.x + speedDelta;

	if (this->speed.x > 0) { this->speed.x -= (speedDelta / 2); }
	if (this->speed.x > 2.0) { this->speed.x -= (speedDelta); }

	this->HandleXSpeed();


	ySpeed += speedDelta;
	this->speed.y = ySpeed;

	this->HandleYSpeed();

	this->UpdateObjectPosBasedOnSpeedValuesReal();

}
void daFishinBoo_c::endState_FlyAway() {}


