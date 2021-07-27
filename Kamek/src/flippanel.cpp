#include <common.h>
#include <game.h>
#include <profile.h>

const char *FlipPanelFileList[] = {"FlipSwapPanel", 0};

class daEnFlipPanel_c : public dEn_c {
public:
	int onCreate();
	int onDelete();
	int onExecute();
	int onDraw();

	Physics physics;
	Physics::Info physicsInfo;

	mHeapAllocator_c allocator;
	nw4r::g3d::ResFile resFile;
	m3d::mdl_c model;
	m3d::anmChr_c chrAnimation;

	s16 frame;
	bool flipClockWise;
	void bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate, int frame);
	void flipThisPanel();

	USING_STATES(daEnFlipPanel_c);
	DECLARE_STATE(Wait);
	DECLARE_STATE(Flipping);

	static dActor_c *build();
};

const SpriteData flipPanelSpriteData = { ProfileId::FlipPanel, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile flipPanelProfile(&daEnFlipPanel_c::build, SpriteId::FlipPanel, flipPanelSpriteData, ProfileId::FlipPanel, ProfileId::FlipPanel, "FlipPanel", FlipPanelFileList);


CREATE_STATE(daEnFlipPanel_c, Wait);
CREATE_STATE(daEnFlipPanel_c, Flipping);


void flipAllPanels() {
	daEnFlipPanel_c *panel = (daEnFlipPanel_c*)fBase_c::search(FlipPanel, 0);
	if(panel) {
		panel->flipThisPanel();
		while((panel = (daEnFlipPanel_c*)fBase_c::search(FlipPanel, panel)) != 0) {
			panel->flipThisPanel();
		}
		NewSFXPlayer(5, true);
	}
}

s16 flipPanelRotations[] = { 0x0, 0x13, 0x14, 0x39, 0xb6, 0x183, 0x280, 0x3b9, 0x53e, 0x71c, 0x93b, 0xb76, 0xdcd, 0x103c, 0x12c4, 0x1563, 0x1816, 0x1add, 0x1db6, 0x209f, 0x2398, 0x269e, 0x29b0, 0x2ccc, 0x2ff2, 0x331f, 0x3652, 0x3989, 0x3cc4, 0x4000, 0x433c, 0x4677, 0x49ae, 0x4ce1, 0x500e, 0x5334, 0x5650, 0x5962, 0x5c68, 0x5f61, 0x624a, 0x6523, 0x67ea, 0x6a9d, 0x6d3c, 0x6fc4, 0x7233, 0x748a, 0x76c5, 0x78e4, 0x7ac3, 0x7c49, 0x7d84, 0x7e80, 0x7f4a, 0x7fcc, 0x7ffc, 0x7ffd, 0x7ff3, 0x8000 };

void daEnFlipPanel_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate, int frame) {
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr(name);
	this->chrAnimation.bind(&this->model, anmChr, unk);
	chrAnimation.setCurrentFrame(frame);
	this->model.bindAnim(&this->chrAnimation, unk2);
	this->chrAnimation.setUpdateRate(rate);
}

void daEnFlipPanel_c::flipThisPanel() {
	if(acState.getCurrentState() == &StateID_Wait) {
		if(frame <= 0) {
			frame = 0;
			flipClockWise = false;
		}
		if(frame >= 59) {
			frame = 59;
			flipClockWise = true;
		}
		if(flipClockWise) {
			bindAnimChr_and_setUpdateRate("rotatePlateClockR-L", 1, 0.0, 1.0f, 0);
		} else {
			bindAnimChr_and_setUpdateRate("rotatePlateCounterL-R", 1, 0.0, 1.0f, 0);
		}
		doStateChange(&StateID_Flipping);
	} else {
		flipClockWise = !flipClockWise;
		if(flipClockWise) {
			bindAnimChr_and_setUpdateRate("rotatePlateClockR-L", 1, 0.0, 1.0f, 59-frame);
		} else {
			bindAnimChr_and_setUpdateRate("rotatePlateCounterL-R", 1, 0.0, 1.0f, frame);
		}
	}
}

void daEnFlipPanel_c::beginState_Wait() {}
void daEnFlipPanel_c::executeState_Wait() {}
void daEnFlipPanel_c::endState_Wait() {}

void daEnFlipPanel_c::beginState_Flipping() {}
void daEnFlipPanel_c::executeState_Flipping() {
	if(frame < 0 || frame > 59) {
		doStateChange(&StateID_Wait);
	} else {
		physics.setPtrToRotation(&flipPanelRotations[frame]);
		if(flipClockWise) {
			frame--;
		} else {
			frame++;
		}
	}
}
void daEnFlipPanel_c::endState_Flipping() {}

int daEnFlipPanel_c::onCreate() {
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	resFile.data = getResource("FlipSwapPanel", "g3d/FlipSwapPanel.brres");
	nw4r::g3d::ResMdl mdl = resFile.GetResMdl("FlipSwapPanel");
	model.setup(mdl, &allocator, 0, 1, 0);
	//SetupTextures_MapObj(&model, 0);

	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr("rotatePlateClockR-L");
	this->chrAnimation.setup(mdl, anmChr, &this->allocator, 0);

	allocator.unlink();

	scale = (Vec){0.25, 0.25, 0.25};
	
	physicsInfo.x1 = -50;
	physicsInfo.y1 = 6;
	physicsInfo.x2 = 3;
	physicsInfo.y2 = -6;

	//Callbacks from magicplatform.cpp because daEnBlockMain_c callbacks crash the game somehow
	physicsInfo.otherCallback1 = (void*)&PhysCB1;
	physicsInfo.otherCallback2 = (void*)&PhysCB2;
	physicsInfo.otherCallback3 = (void*)&PhysCB3;

	physics.setup(this, &physicsInfo, 3, currentLayerID);
	physics.flagsMaybe = 0x260;
	physics.callback1 = (void*)&PhysCB4;
	physics.callback2 = (void*)&PhysCB5;
	physics.callback3 = (void*)&PhysCB6;
	physics.addToList();

	if(this->settings & 1) {	//bit 48: dual box start left or start right
		OSReport("Start right\n");
		bindAnimChr_and_setUpdateRate("rotatePlateCounterL-R", 1, 0.0, 1.0f, 59);
		frame = 59;
		flipClockWise = true;
		physics.setPtrToRotation(&flipPanelRotations[frame]);
	} else {
		bindAnimChr_and_setUpdateRate("rotatePlateClockR-L", 1, 0.0, 1.0f, 59);
	}

	doStateChange(&daEnFlipPanel_c::StateID_Wait);

	return true;
}


int daEnFlipPanel_c::onDelete() {
	physics.removeFromList();

	return true;
}


int daEnFlipPanel_c::onExecute() {
	acState.execute();
	
	physics.update();

	// now check zone bounds based on state
	/*if (acState.getCurrentState()->isEqual(&StateID_Wait)) {
		checkZoneBoundaries(0);
	}*/

	return true;
}


int daEnFlipPanel_c::onDraw() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	model.setDrawMatrix(matrix);
	model.setScale(&scale);
	model.calcWorld(false);
	model.scheduleForDrawing();
	model._vf1C();

	return true;
}


dActor_c *daEnFlipPanel_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daEnFlipPanel_c));
	daEnFlipPanel_c *c = new(buffer) daEnFlipPanel_c;

	return c;
}