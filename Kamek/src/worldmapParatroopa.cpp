class dWMParatroopa_c : public dActor_c {
public:
	static dWMParatroopa_c *build();
	
	dWMParatroopa_c();
	
	int onCreate();
	int onDelete();
	int onExecute();
	void updateModelMatrices();
	void bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate, bool isWing);
	
	mHeapAllocator_c allocator;
	
	nw4r::g3d::ResFile resFile;
	m3d::mdl_c bodyModel;
	m3d::anmChr_c animationChr;
	
	nw4r::g3d::ResFile wingBrres;
	m3d::mdl_c wing;
	m3d::anmChr_c wingChr;
	
	bool moveUp;
	int frames;
	
	dStateWrapper_c<dWMParatroopa_c> state;
	
	USING_STATES(dWMParatroopa_c);
	DECLARE_STATE(Wait);
};

CREATE_STATE(dWMParatroopa_c, Wait);

dWMParatroopa_c *dWMParatroopa_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dWMParatroopa_c));
	dWMParatroopa_c *c = new(buffer) dWMParatroopa_c;

	return c;
}

dWMParatroopa_c::dWMParatroopa_c() : state(this, &StateID_Wait) {
}

int dWMParatroopa_c::onCreate() {
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	resFile.data = getResource("nokonokoA", "g3d/t00.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("nokonokoA");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Enemy(&bodyModel, 0);
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr("flyA");
	this->animationChr.setup(mdl, anmChr, &this->allocator, 0);

	wingBrres.data = getResource("wing", "g3d/wing.brres");
	nw4r::g3d::ResMdl mdl2 = this->wingBrres.GetResMdl("wing");
	wing.setup(mdl2, &allocator, 0x224, 1, 0);
	SetupTextures_Enemy(&wing, 0);
	nw4r::g3d::ResAnmChr anmChr2 = this->wingBrres.GetResAnmChr("wing_pata");
	this->wingChr.setup(mdl2, anmChr2, &this->allocator, 0);

	allocator.unlink();
	
	bindAnimChr_and_setUpdateRate("flyA", 1, 0.0, 1.0, false);
	bindAnimChr_and_setUpdateRate("wing_pata", 1, 0.0, 1.0, true);
	
	this->scale = (Vec){2.0, 2.0, 2.0};
	this->rot.y = -0x4000;
	this->pos.x += 300;
	this->pos.y += 60;
	
	return true;
}


int dWMParatroopa_c::onDelete() {
	return true;
}


int dWMParatroopa_c::onExecute() {
	state.execute();
	
	updateModelMatrices();
	bodyModel._vf1C();
	wing._vf1C();
	
	if (this->animationChr.isAnimationDone()) {
		this->animationChr.setCurrentFrame(0.0);
	}
	if (this->wingChr.isAnimationDone()) {
		this->wingChr.setCurrentFrame(0.0);
	}
	
	return true;
}

void dWMParatroopa_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
	bodyModel.scheduleForDrawing();
	
	matrix.translation(pos.x, pos.y + 10, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);
	
	wing.setDrawMatrix(matrix);
	wing.setScale(&scale);
	wing.calcWorld(false);
	wing.scheduleForDrawing();
}

void dWMParatroopa_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate, bool isWing) {
	if(isWing) {
		nw4r::g3d::ResAnmChr anmChr = this->wingBrres.GetResAnmChr(name);
		this->wingChr.bind(&this->wing, anmChr, unk);
		this->wing.bindAnim(&this->wingChr, unk2);
		this->wingChr.setUpdateRate(rate);
	} else {
		nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr(name);
		this->animationChr.bind(&this->bodyModel, anmChr, unk);
		this->bodyModel.bindAnim(&this->animationChr, unk2);
		this->animationChr.setUpdateRate(rate);
	}
}

void dWMParatroopa_c::beginState_Wait() {}
void dWMParatroopa_c::executeState_Wait() {
	if(moveUp) {
		pos.y += 1;
	} else {
		pos.y -= 1;
	}
	frames++;
	if(frames >= 120) {
		frames = 0;
		moveUp = !moveUp;
	}
}
void dWMParatroopa_c::endState_Wait() {}