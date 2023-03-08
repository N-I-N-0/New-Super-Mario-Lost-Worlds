#include <common.h>
#include <game.h>
#include <profile.h>

class daEnFence_c : public dEn_c {
public:
	mHeapAllocator_c allocator;
	nw4r::g3d::ResFile resFile;
	m3d::mdl_c bodyModel;

	bool isVisible;

	void appear();
	void disappear();

	int onCreate();
	int onDelete();
	int onExecute();
	int onDraw();
	void updateModelMatrices();

	USING_STATES(daEnFence_c);

	static dActor_c *build();
};


const char *fenceFileList[] = {"fence", NULL};
const SpriteData fenceSpriteData = { ProfileId::fence, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile fenceProfile(&daEnFence_c::build, SpriteId::fence, &fenceSpriteData, ProfileId::fence, ProfileId::fence, "LM fence", fenceFileList);


int daEnFence_c::onCreate() {
	// Model creation
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	resFile.data = getResource("fence", "g3d/fence.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("fence");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	//SetupTextures_Enemy(&bodyModel, 0);

	allocator.unlink();

	this->scale = (Vec){0.5f, 0.5f, 0.5f};

	return true;
}


int daEnFence_c::onDelete() {
	return true;
}

void daEnFence_c::appear() {
	Vec efPos = {pos.x-8, pos.y+4, pos.z};
	SpawnEffect("Wm_ob_icepoison", 0, &efPos, &(S16Vec){0,0,0}, &(Vec){1.5, 2.5, 1.0});
	PlaySoundAsync(this, SE_EMY_TERESA);
}

void daEnFence_c::disappear() {
	Vec efPos = {pos.x-8, pos.y+4, pos.z};
	SpawnEffect("Wm_ob_icepoison", 0, &efPos, &(S16Vec){0,0,0}, &(Vec){1.5, 2.5, 1.0});
	PlaySoundAsync(this, SE_EMY_TERESA_DEAD);
}

int daEnFence_c::onExecute() {
	updateModelMatrices();

	if (dFlagMgr_c::instance->flags & spriteFlagMask) {
		if (!isVisible) {
			appear();
			isVisible = true;
		}
	} else {
		if(isVisible) {
			disappear();
			isVisible = false;
		}
	}

	return true;
}

int daEnFence_c::onDraw() {
	if(isVisible) {
		bodyModel.scheduleForDrawing();
	}
}

void daEnFence_c::updateModelMatrices() {
	matrix.translation(pos.x-8, pos.y-8, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

dActor_c *daEnFence_c::build() {

	void *buffer = AllocFromGameHeap1(sizeof(daEnFence_c));
	daEnFence_c *c = new(buffer) daEnFence_c;


	return c;
}
