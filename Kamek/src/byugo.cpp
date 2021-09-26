const char* ByugoNameList [] = { "Byugo", NULL };

class daByugo_c : public dEn_c {
public:

	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	void updateModelMatrices();
	void bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate);

	static dActor_c* build();

	mHeapAllocator_c allocator;
	m3d::mdl_c bodyModel;
	m3d::anmChr_c animationChr;
	nw4r::g3d::ResFile resFile;

	int loopCount;
	float radius;

	mEf::es2 effect;

	USING_STATES(daByugo_c);
	DECLARE_STATE(Wait);
	DECLARE_STATE(Sign);
	DECLARE_STATE(Start);
	DECLARE_STATE(Loop);
	DECLARE_STATE(End);
};

CREATE_STATE(daByugo_c, Wait);
CREATE_STATE(daByugo_c, Sign);
CREATE_STATE(daByugo_c, Start);
CREATE_STATE(daByugo_c, Loop);
CREATE_STATE(daByugo_c, End);

void daByugo_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y - 16, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}


void daByugo_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate) {
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr(name);
	this->animationChr.bind(&this->bodyModel, anmChr, unk);
	this->bodyModel.bindAnim(&this->animationChr, unk2);
	this->animationChr.setUpdateRate(rate);
}

dActor_c* daByugo_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daByugo_c));
	return new(buffer) daByugo_c;
}

const SpriteData ByugoSpriteData = { ProfileId::Byugo, 0, 0, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile ByugoProfile(&daByugo_c::build, SpriteId::Byugo, ByugoSpriteData, ProfileId::Byugo, ProfileId::Byugo, "Byugo", ByugoNameList);


int daByugo_c::onCreate() {
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	resFile.data = getResource("Byugo", "g3d/Byugo.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("Byugo");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Enemy(&bodyModel, 0);
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr("Wait");
	this->animationChr.setup(mdl, anmChr, &this->allocator, 0);

	allocator.unlink(); 

	this->scale = (Vec){0.7, 0.7, 0.7};

	doStateChange(&StateID_Wait);

	return true;
}

extern float layerZPositionForByugoHack;

int daByugo_c::onExecute() {
	acState.execute();

	updateModelMatrices();
	bodyModel._vf1C();

	for(int i = 0; i < 4; i++) {
		dAcPy_c *player;
		if(player = dAcPy_c::findByID(i)) {
			if(player->currentLayerID == 2) return true;
		}
	}
	layerZPositionForByugoHack = -1800.0f;	//reset value if no player hit by Ty-Foo --> doesn't work if e.g. a player uses a pipe at the same time, since that also makes the player switch to currentLayerID 2
	
	return true;
}

int daByugo_c::onDelete() {
	return true;
}

int daByugo_c::onDraw() {
	bodyModel.scheduleForDrawing();

	return true;
}

extern "C" u32 GenerateRandomNumber(int max);
void daByugo_c::beginState_Wait() {
	bindAnimChr_and_setUpdateRate("Wait", 1, 0.0, 1.0);
}
void daByugo_c::executeState_Wait() {
	if (this->animationChr.isAnimationDone()) {
		if(GenerateRandomNumber(5) == 0) {
			doStateChange(&StateID_Sign);
		} else {
			this->animationChr.setCurrentFrame(0.0);
		}
	}
}
void daByugo_c::endState_Wait() {}

void daByugo_c::beginState_Sign() {
	bindAnimChr_and_setUpdateRate("BlowSign", 1, 0.0, 1.0);
}
void daByugo_c::executeState_Sign() {
	if (this->animationChr.isAnimationDone()) {
		doStateChange(&StateID_Start);
	}
}
void daByugo_c::endState_Sign() {}

void daByugo_c::beginState_Start() {
	bindAnimChr_and_setUpdateRate("BlowStart", 1, 0.0, 1.0);
}
void daByugo_c::executeState_Start() {
	if (this->animationChr.isAnimationDone()) {
		doStateChange(&StateID_Loop);
	}
}
void daByugo_c::endState_Start() {}

void daByugo_c::beginState_Loop() {
	bindAnimChr_and_setUpdateRate("BlowLoop", 1, 0.0, 1.0);
	loopCount = 0;
	radius = 8.0f;
}
void daByugo_c::executeState_Loop() {
	for(int i = 0; i < 4; i++) {
		dAcPy_c *player;
		if(player = dAcPy_c::findByID(i)) {
			float dx = this->pos.x - player->pos.x;
			float dy = this->pos.y - player->pos.y;
			if(sqrtf(dx*dx + dy*dy) <= this->radius * 3) {
				if(player->states2.getCurrentState() != &daPlBase_c::StateID_Quake) {
					player->stunPlayer();
				}
			}
		}
	}


	S16Vec nullRot = {0,0,0};
	Vec oneVec = {0.75f*radius, 0.75f*radius, 0.75f*radius};
	effect.spawn("Wm_mr_slipsmoke", 0, &(Vec){pos.x, pos.y - 16, 0}, &nullRot, &oneVec);

	radius += 0.1f;

	if (this->animationChr.isAnimationDone()) {
		if(loopCount >= 9) {
			doStateChange(&StateID_End);
		} else {
			this->animationChr.setCurrentFrame(0.0);
			loopCount++;
		}
	}
}
void daByugo_c::endState_Loop() {
	for(int i = 0; i < 4; i++) {
		dAcPy_c *player;
		if(player = dAcPy_c::findByID(i)) {
			if(player->states2.getCurrentState() == &daPlBase_c::StateID_Quake) {
				player->unstunPlayer();
				player->currentLayerID = 2;
				layerZPositionForByugoHack = 5500.0f;
			}
		}
	}
}

void daByugo_c::beginState_End() {
	bindAnimChr_and_setUpdateRate("BlowEnd", 1, 0.0, 1.0);
}
void daByugo_c::executeState_End() {
	if (this->animationChr.isAnimationDone()) {
		doStateChange(&StateID_Wait);
	}
}
void daByugo_c::endState_End() {}



const char* SwitchLayerFileList [] = { NULL };

class daSwitchLayer_c : public dEn_c {
public:
	int onCreate();
	int onExecute();
	int onDelete();

	static dActor_c* build();

	void playerCollision(ActivePhysics* apThis, ActivePhysics* apOther);
};

dActor_c* daSwitchLayer_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daSwitchLayer_c));
	return new(buffer) daSwitchLayer_c;
}

const SpriteData SwitchLayerSpriteData = { ProfileId::SwitchLayer, 0, 0, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile SwitchLayerProfile(&daSwitchLayer_c::build, SpriteId::SwitchLayer, SwitchLayerSpriteData, ProfileId::SwitchLayer, ProfileId::SwitchLayer, "SwitchLayer", SwitchLayerFileList);

int daSwitchLayer_c::onCreate() {
	ActivePhysics::Info HitMeBaby; 
	HitMeBaby.xDistToCenter = 0.0; 
	HitMeBaby.yDistToCenter = 0.0; 
	HitMeBaby.xDistToEdge = this->settings; 
	HitMeBaby.yDistToEdge = 3.0; 
	HitMeBaby.category1 = 0x3; 
	HitMeBaby.category2 = 0x0; 
	HitMeBaby.bitfield1 = 0x4F; 
	HitMeBaby.bitfield2 = 0xFFFFFFFF; 
	HitMeBaby.unkShort1C = 0; 
	HitMeBaby.callback = &dEn_c::collisionCallback; 
	this->aPhysics.initWithStruct(this, &HitMeBaby); 
	this->aPhysics.addToList(); 

	return true;
}

int daSwitchLayer_c::onExecute() {
	return true;
}

int daSwitchLayer_c::onDelete() {
	return true;
}

void daSwitchLayer_c::playerCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	dAcPy_c* player = (dAcPy_c*)apOther->owner;
	if(player->speed.y > 0) {
		player->currentLayerID = 0;
	}
}