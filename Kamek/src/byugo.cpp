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

	//void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther);

	//bool collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat5_Mario(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat8_FencePunch(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat11_PipeCannon(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther);


	mHeapAllocator_c allocator;
	m3d::mdl_c bodyModel;
	m3d::anmChr_c animationChr;
	nw4r::g3d::ResFile resFile;

	int loopCount;

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
	matrix.translation(pos.x, pos.y, pos.z);
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

	OSReport("Byugo 1\n");

	resFile.data = getResource("Byugo", "g3d/Byugo.brres");
	OSReport("Byugo 2\n");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("Byugo");
	OSReport("Byugo 3\n");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	OSReport("Byugo 4\n");
	SetupTextures_Enemy(&bodyModel, 0);
	OSReport("Byugo 5\n");
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr("Wait");
	OSReport("Byugo 6\n");
	this->animationChr.setup(mdl, anmChr, &this->allocator, 0);
	OSReport("Byugo 7\n");

	allocator.unlink(); 

	ActivePhysics::Info HitMeBaby; 
	HitMeBaby.xDistToCenter = 0.0; 
	HitMeBaby.yDistToCenter = 0.0; 
	HitMeBaby.xDistToEdge = 0.0; 
	HitMeBaby.yDistToEdge = 0.0; 
	HitMeBaby.category1 = 0x3; 
	HitMeBaby.category2 = 0x0; 
	HitMeBaby.bitfield1 = 0x4F; 
	HitMeBaby.bitfield2 = 0xFFFFFFFF; 
	HitMeBaby.unkShort1C = 0; 
	HitMeBaby.callback = &dEn_c::collisionCallback; 
	this->aPhysics.initWithStruct(this, &HitMeBaby); 
	this->aPhysics.addToList(); 

	OSReport("Byugo 8\n");

	OSReport("Byugo 9\n");

	this->scale = (Vec){0.7, 0.7, 0.7};

	doStateChange(&StateID_Wait);

	return true;
}

int daByugo_c::onExecute() {
	acState.execute();

	updateModelMatrices();
	bodyModel._vf1C();

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
}
void daByugo_c::executeState_Loop() {
	
	if (this->animationChr.isAnimationDone()) {
		if(loopCount >= 9) {
			doStateChange(&StateID_End);
		} else {
			this->animationChr.setCurrentFrame(0.0);
			loopCount++;
		}
	}
}
void daByugo_c::endState_Loop() {}

void daByugo_c::beginState_End() {
	bindAnimChr_and_setUpdateRate("BlowEnd", 1, 0.0, 1.0);
}
void daByugo_c::executeState_End() {
	if (this->animationChr.isAnimationDone()) {
		doStateChange(&StateID_Wait);
	}
}
void daByugo_c::endState_End() {}

