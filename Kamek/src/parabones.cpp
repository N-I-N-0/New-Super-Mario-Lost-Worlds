#include <common.h>
#include <game.h>
#include <profile.h>
#include "path.h"

const char* paraBonesFileList[] = { "karon", "wing", 0 };


class daEnParaBones_c : public dEnPath_c {
public:
	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	mHeapAllocator_c allocator;
	nw4r::g3d::ResFile resFile;
	nw4r::g3d::ResFile resFileAnim;
	m3d::mdl_c bodyModel;

	nw4r::g3d::ResAnmTexPat anmPat;

	m3d::anmChr_c chrAnimation;
	m3d::anmTexPat_c patAnimation;

	bool ranOnce;
	
	dEn_c* dryBone;
	u32 dryBoneId;

	static dActor_c* build();

	void bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate);

	void updateModelMatrices();
};

const SpriteData ParaBonesSpriteData = { ProfileId::ParaBones, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile ParaBonesProfile(&daEnParaBones_c::build, SpriteId::ParaBones, &ParaBonesSpriteData, ProfileId::ParaBones, ProfileId::ParaBones, "ParaBones", paraBonesFileList);

dActor_c* daEnParaBones_c::build() {
	void* buffer = AllocFromGameHeap1(sizeof(daEnParaBones_c));
	daEnParaBones_c* c = new(buffer) daEnParaBones_c;

	return c;
}

void daEnParaBones_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate){
	nw4r::g3d::ResAnmChr anmChr = this->resFileAnim.GetResAnmChr(name);
	this->chrAnimation.bind(&this->bodyModel, anmChr, unk);
	this->bodyModel.bindAnim(&this->chrAnimation, unk2);
	this->chrAnimation.setUpdateRate(rate);
}

int daEnParaBones_c::onCreate() {
	dryBone = (dEn_c*)CreateActor(EN_KARON, (this->settings & 0xFF000000) | (this->settings >> 23 & 1), &pos, 0, this->currentLayerID);
	dryBoneId = dryBone->id;
	
	// Model creation	
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	this->resFile.data = getResource("wing", "g3d/wing.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("wing");
	bodyModel.setup(mdl, &allocator, 0x227, 1, 0);
	SetupTextures_Player(&bodyModel, 0);

	this->resFileAnim.data = getResource("wing", "g3d/wing.brres");
	nw4r::g3d::ResAnmChr anmChr = this->resFileAnim.GetResAnmChr("wing_pata");
	this->chrAnimation.setup(mdl, anmChr, &this->allocator, 0);

	allocator.unlink();

	this->scale = (Vec){1, 1, 1};
	
	bindAnimChr_and_setUpdateRate("wing_pata", 1, 0.0, 1.0);

	beginState_Init();
	if (this->rotateNext) {
		if (this->stepVector.x > 0) {
			this->rot.y = 0x4000;
		} else {
			this->rot.y = -0x4000;
		}
	} else if (this->rotate0XNext) {
		this->rot.y = 0x0;
	}
	executeState_Init();
	doStateChange(&StateID_FollowPath);	
	
	return true;
}


int daEnParaBones_c::onDelete() {
	nw4r::snd::SoundHandle handle;
	PlaySoundWithFunctionB4(SoundRelatedClass, &handle, SE_EMY_DOWN, 1);
	StageE4::instance->spawnCoinJump(pos, 0, 1, 0);
	return true;
}

int daEnParaBones_c::onDraw() {
	bodyModel.scheduleForDrawing();
	return true;
}


void daEnParaBones_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y+8, pos.z);
	matrix.applyRotationYXZ(&dryBone->rot.x, &dryBone->rot.y, &dryBone->rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

extern dStateBase_c daEnKanon_c__StateID_Walk;
extern dStateBase_c daEnKanon_c__StateID_Turn;
extern dStateBase_c daEnKanon_c__StateID_Damage;

int daEnParaBones_c::onExecute() {
	dryBone = (dEn_c*)fBase_c::search(dryBoneId);
	if(!dryBone) {
		if(!isOutOfView()) {
			dryBone = (dEn_c*)CreateActor(EN_KARON, (this->settings & 0xFF000000) | (this->settings >> 23 & 1), &pos, 0, this->currentLayerID);
			dryBoneId = dryBone->id;
		} else {
			return true;
		}
	}
	
	acState.execute();
	
	bodyModel._vf1C();
	updateModelMatrices();

	if(!(dryBone->acState.getCurrentState() == &daEnKanon_c__StateID_Walk || dryBone->acState.getCurrentState() == &daEnKanon_c__StateID_Turn)) {
		this->Delete(1);
	}
	
	dryBone->pos = this->pos;
	
	if (this->chrAnimation.isAnimationDone()) {
		this->chrAnimation.setCurrentFrame(0.0);
	}

	return true;
}