#include "boss.h"

const char* LevelCameraPreviewFileList[] = { NULL };

class dLevelCameraPreview_c : public dPath_c {
public:
	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	u32 timer;
	float roomLimit;
	float baseScreenWall;

	dAcPy_c *player[4];
	Vec freezeMarioPos;
	Vec lastPathPos;

	USING_STATES(dLevelCameraPreview_c);

	static dActor_c *build();
	static dLevelCameraPreview_c *instance;
};


const SpriteData LevelCameraPreviewSpriteData = { ProfileId::LevelCameraPreview, 8, -8, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile LevelCameraPreviewProfile(&dLevelCameraPreview_c::build, SpriteId::LevelCameraPreview, &LevelCameraPreviewSpriteData, ProfileId::LevelCameraPreview, ProfileId::LevelCameraPreview, "LevelCameraPreview", LevelCameraPreviewFileList);



dLevelCameraPreview_c *dLevelCameraPreview_c::instance = 0;

dActor_c* dLevelCameraPreview_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dLevelCameraPreview_c));
	dLevelCameraPreview_c *c = new(buffer) dLevelCameraPreview_c;

	instance = c;
	return c;
}


// extern "C" void* _8042A788;
extern "C" void doBGMStuff(void*, u32); // Lowers volume
extern "C" void doBGMStuff2(void*); // Puts volume back to how it was


int dLevelCameraPreview_c::onCreate() {

	MakeMarioEnterDemoMode();
	StageC4::instance->_1D = 1;
	//this->roomLimit = (this->pos.x + ((float)roomLength * 16.0f)) - ClassWithCameraInfo::instance->getEffectiveScreenLeft() - (ClassWithCameraInfo::instance->screenWidth / 2);
	//ClassWithCameraInfo::instance->xOffset = this->roomLimit;

	//mario->disableFlagMask |= 0x10;
	NextStageActorDisableFlags |= 0x10;

	beginState_Init();
	executeState_Init();

	this->lastPathPos = this->pos;

	OSReport("currentNodeNum: %d\n", this->currentNodeNum);
	OSReport("pathID: %d\n", this->pathID);
	acState.setState(&StateID_FollowPath);

	return true;
}


int dLevelCameraPreview_c::onDelete() {
	isInItemBowling = false;
	return true;
}

int dLevelCameraPreview_c::onDraw() {
	return true;
}

int dLevelCameraPreview_c::onExecute() {
	acState.execute();

	if(acState.getCurrentState() == &StateID_Done) {
		MakeMarioExitDemoMode();
		centerCameraOnObject(dAcPy_c::findByID(0)->pos, true, true);
		this->Delete(1);
	} else {
		//ClassWithCameraInfo::instance->screenCentreX -= this->lastPathPos.x - this->pos.x;
		//ClassWithCameraInfo::instance->screenCentreY -= this->lastPathPos.y - this->pos.y;
		centerCameraOnObject(this->pos, true, true);
	}
	
	this->lastPathPos = this->pos;

	return true;
}

extern "C" u8 NextStageActorDisableFlags;

// Init State
/*
void dLevelCameraPreview_c::beginState_Init() {
	MakeMarioEnterDemoMode();

	OSReport("Initializing Item Bowling.\n");
	StageC4::instance->_1D = 1;
	this->roomLimit = (this->pos.x + ((float)roomLength * 16.0f)) - ClassWithCameraInfo::instance->getEffectiveScreenLeft() - (ClassWithCameraInfo::instance->screenWidth / 2);
	ClassWithCameraInfo::instance->xOffset = this->roomLimit;

	mario->disableFlagMask |= 0x10;
	NextStageActorDisableFlags |= 0x10;

	this->timer = 0;
}

void dLevelCameraPreview_c::executeState_Init() {
	// OSReport("Executing Item Bowling.\n");
	if(this->timer > 60) {
		if(ClassWithCameraInfo::instance->xOffset > 0.0f) {
			ClassWithCameraInfo::instance->xOffset -= 4.0f;
		}
		else {
			// StageC4::instance->_1D = 0;
			ClassWithCameraInfo::instance->xOffset = 0.0f;

			acState.setState(&StateID_ShowTitle);
		}
	}

	this->timer++;
}

void dLevelCameraPreview_c::endState_Init() {
	
}

*/



