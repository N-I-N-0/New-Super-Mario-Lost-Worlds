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

	s16 flipRotation;

	void flipThisPanel();

	USING_STATES(daEnFlipPanel_c);
	//DECLARE_STATE(Wait);
	//DECLARE_STATE(Flipping);

	static dActor_c *build();
};

const SpriteData flipPanelSpriteData = { ProfileId::FlipPanel, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
 // Using WM_GRID as the execute order profile ID fixes bugs; original FlipPanel uses it as well
Profile flipPanelProfile(&daEnFlipPanel_c::build, SpriteId::FlipPanel, flipPanelSpriteData, ProfileId::WM_GRID, ProfileId::FlipPanel, "FlipPanel", FlipPanelFileList);


//CREATE_STATE(daEnFlipPanel_c, Wait);
//CREATE_STATE(daEnFlipPanel_c, Flipping);


void flipAllPanels() {
	daEnFlipPanel_c *panel = 0;
	while ((panel = (daEnFlipPanel_c*)fBase_c::search(FlipPanel, panel)) != 0) {
		panel->flipThisPanel();
	}
}

void daEnFlipPanel_c::flipThisPanel() {
	OSReport("This Panel should be rotated now!\n");
}

int daEnFlipPanel_c::onCreate() {
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	resFile.data = getResource("FlipSwapPanel", "g3d/FlipSwapPanel.brres");
	model.setup(resFile.GetResMdl("FlipSwapPanel"), &allocator, 0, 1, 0);
	//SetupTextures_MapObj(&model, 0);

	allocator.unlink();

	scale = (Vec){0.5, 0.5, 0.5};
	flipRotation = 0;

	physicsInfo.x1 = -100;
	physicsInfo.y1 = 6;
	physicsInfo.x2 = 6;
	physicsInfo.y2 = -6;

	physicsInfo.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
	physicsInfo.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
	physicsInfo.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

	physics.setup(this, &physicsInfo, 3, currentLayerID);
	physics.flagsMaybe = 0x260;
	physics.callback1 = &daEnBlockMain_c::PhysicsCallback1;
	physics.callback2 = &daEnBlockMain_c::PhysicsCallback2;
	physics.callback3 = &daEnBlockMain_c::PhysicsCallback3;
	physics.setPtrToRotation(&flipRotation);
	physics.addToList();

	//doStateChange(&daEnFlipPanel_c::StateID_Wait);

	return true;
}


int daEnFlipPanel_c::onDelete() {
	physics.removeFromList();

	return true;
}


int daEnFlipPanel_c::onExecute() {
	acState.execute();
	
	flipRotation += 0x800;
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

	return true;
}


dActor_c *daEnFlipPanel_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daEnFlipPanel_c));
	daEnFlipPanel_c *c = new(buffer) daEnFlipPanel_c;

	return c;
}