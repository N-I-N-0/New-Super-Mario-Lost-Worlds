class dWMAirships_c : public dActor_c {
public:
	static dActor_c *build();
	
	dWMAirships_c();
	
	int onCreate();
	int onDelete();
	int onExecute();
	void updateModelMatrices();
	void bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate);
	bool checkCollision();
	char* getModelName();
	
	mHeapAllocator_c allocator;
	
	nw4r::g3d::ResFile resFile;
	m3d::mdl_c bodyModel;
	m3d::anmChr_c animationChr;
	
	int frames;
	Vec startPos;
	
	int airship;
	
	dStateWrapper_c<dWMAirships_c> state;
	
	USING_STATES(dWMAirships_c);
	DECLARE_STATE(Wait);
};

const char* EmptyAirshipsFileList[] = {NULL};
Profile WMAirshipsProfile(&dWMAirships_c::build, ProfileId::WMAirships, NULL, ProfileId::WMAirships, ProfileId::WMAirships, "Map Airships", EmptyAirshipsFileList);


CREATE_STATE(dWMAirships_c, Wait);

dActor_c *dWMAirships_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dWMAirships_c));
	dWMAirships_c *c = new(buffer) dWMAirships_c;

	return c;
}

dWMAirships_c::dWMAirships_c() : state(this, &StateID_Wait) {
}

char* dWMAirships_c::getModelName() {
	switch(this->airship) {
		case 0:
			return "Wendy";
		case 1:
			return "Larry";
		case 2:
			return "Iggy";
		case 3:
			return "Lemmy";
		case 4:
			return "Ludwig";
		case 5:
			return "Roy";
		case 6:
			return "Morton";
	}
}

int dWMAirships_c::onCreate() {
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	this->airship = (this->settings & 0xF) % 7;

	resFile.data = getResource("cobKoopaShips", "g3d/Airships.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl(this->getModelName());
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Enemy(&bodyModel, 0);
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr(this->getModelName());
	this->animationChr.setup(mdl, anmChr, &this->allocator, 0);

	allocator.unlink();
	
	this->scale = (Vec){2.0, 2.0, 2.0};
	this->rot.y = -0x4000;
	this->pos.x += 300;
	
	this->startPos = this->pos;
	
	return true;
}


int dWMAirships_c::onDelete() {
	return true;
}

extern int PtrToWM_CS_SEQ_MNG;
extern "C" bool FUN_801017c0(int, int, int, int, int);
extern "C" int dCsSeqMng_c__GetCutName(int);

int dWMAirships_c::onExecute() {
	updateModelMatrices();
	bodyModel._vf1C();
	
	if(dCourseSelectManager_c::instance->state.getCurrentState() != &dCourseSelectManager_c::StateID_KeyWait) {
		return true;
	}
	
	state.execute();
	
	int currentCommand = dCsSeqMng_c__GetCutName(PtrToWM_CS_SEQ_MNG);
	
	if(currentCommand != -1) {
		// OSReport("GetCutName: %d\n", currentCommand);
	}
	switch(currentCommand) {
		case 164:
			//if(state.getCurrentState() != &StateID_PlayerLost) state.setState(&StateID_PlayerLost);
			break;
		case 161:
			//if(state.getCurrentState() != &StateID_PlayerWon) state.setState(&StateID_PlayerWon);
			break;
	}
	
	return true;
}

void dWMAirships_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
	bodyModel.scheduleForDrawing();
}


void dWMAirships_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate) {
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr(name);
	this->animationChr.bind(&this->bodyModel, anmChr, unk);
	this->bodyModel.bindAnim(&this->animationChr, unk2);
	this->animationChr.setUpdateRate(rate);
}

void dWMAirships_c::beginState_Wait() {
	bindAnimChr_and_setUpdateRate(this->getModelName(), 1, 0.0, 1.0);
}
void dWMAirships_c::executeState_Wait() {
	if (this->animationChr.isAnimationDone()) {
		this->animationChr.setCurrentFrame(0.0);
	}
}
void dWMAirships_c::endState_Wait() {}