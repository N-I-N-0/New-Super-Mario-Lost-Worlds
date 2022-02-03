const char* ShroobUFOFileList [] = { "shroobUfo", NULL };
const char* ShroobUFOLaserBallFileList [] = { NULL };

class daShroobUFOLaserBall_c : public dStageActor_c {
public:
	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	static dActor_c* build();

	Vec moveVec;
	float speed;

	mEf::es2 effect;

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

};

class daShroobUFO_c : public dEn_c {
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

	int cooldown;

	mHeapAllocator_c allocator;
	m3d::mdl_c bodyModel;
	m3d::anmChr_c animationChr;
	m3d::anmChr_c animationChr2;
	m3d::anmChr_c animationChr3;
	nw4r::g3d::ResFile resFile;

	USING_STATES(daShroobUFO_c);
	DECLARE_STATE(Wait);
	DECLARE_STATE(Arm);
	DECLARE_STATE(Shoot);
	DECLARE_STATE(FlyAway);
};

CREATE_STATE(daShroobUFO_c, Wait);
CREATE_STATE(daShroobUFO_c, Arm);
CREATE_STATE(daShroobUFO_c, Shoot);
CREATE_STATE(daShroobUFO_c, FlyAway);

void daShroobUFO_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}


void daShroobUFO_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate) {
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr(name);
	this->animationChr.bind(&this->bodyModel, anmChr, unk);
	this->bodyModel.bindAnim(&this->animationChr, unk2);
	this->animationChr.setUpdateRate(rate);
}

dActor_c* daShroobUFO_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daShroobUFO_c));
	return new(buffer) daShroobUFO_c;
}

const SpriteData ShroobUFOSpriteData = { ProfileId::ShroobUfo, 0, 0, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile ShroobUFOProfile(&daShroobUFO_c::build, SpriteId::ShroobUfo, &ShroobUFOSpriteData, ProfileId::ShroobUfo, ProfileId::ShroobUfo, "Shroob UFO", ShroobUFOFileList);


int daShroobUFO_c::onCreate() {
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	resFile.data = getResource("shroobUfo", "g3d/shroobUfo.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("shroobUfo");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Enemy(&bodyModel, 0);
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr("FlyNoTurret");
	this->animationChr.setup(mdl, anmChr, &this->allocator, 0);

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

	this->scale = (Vec){0.1, 0.1, 0.1};
	
	this->rot.y = -0x2000;
	
	//this->disableEatIn();

	//MakeMarioEnterDemoMode();

	doStateChange(&StateID_Wait);

	return true;
}

int daShroobUFO_c::onExecute() {
	acState.execute();

	updateModelMatrices();
	bodyModel._vf1C();

	/*if (this->animationChr.isAnimationDone()) {
		this->animationChr.setCurrentFrame(0.0);
	}*/

	return true;
}

int daShroobUFO_c::onDelete() {
	return true;
}

int daShroobUFO_c::onDraw() {
	bodyModel.scheduleForDrawing();

	return true;
}

void daShroobUFO_c::beginState_Wait() {
	bindAnimChr_and_setUpdateRate("FlyNoTurret", 1, 0.0, 1.0);
	cooldown = 120;											//I'm reusing cooldown here since I don't want to add new integers if I don't need them ¯\_(ツ)_/¯
}
void daShroobUFO_c::executeState_Wait() {
	if(this->animationChr.isAnimationDone()) {
		this->animationChr.setCurrentFrame(0.0);
	}
	
	if(/*GenerateRandomNumber(33) == 0 ||*/ cooldown < 1) {
		doStateChange(&StateID_Arm);
	} else {
		cooldown--;
		this->pos.x -= 2;
		if(this->animationChr.isAnimationDone()) {
			this->animationChr.setCurrentFrame(0.0);
		}
	}
}
void daShroobUFO_c::endState_Wait() {
	bindAnimChr_and_setUpdateRate("ArmOut", 1, 0.0, 1.0);
}

void daShroobUFO_c::beginState_Arm() {}
void daShroobUFO_c::executeState_Arm() {
	if(this->animationChr.isAnimationDone()) {
		if(GenerateRandomNumber(10) == 0) {
			doStateChange(&StateID_FlyAway);
		} else {
			doStateChange(&StateID_Shoot);
		}	
	}
}
void daShroobUFO_c::endState_Arm() {}

void daShroobUFO_c::beginState_Shoot() {
	bindAnimChr_and_setUpdateRate("GunOutFire", 1, 0.0, 1.0);
}
void daShroobUFO_c::executeState_Shoot() {
	if(this->animationChr.isAnimationDone()) {
		daShroobUFOLaserBall_c* laserBall = (daShroobUFOLaserBall_c*)dStageActor_c::create(ShroobUFOLaserBall, 0, &(Vec){pos.x-10.5, pos.y-10, 6000}, 0, this->currentLayerID);
		laserBall->moveVec.x = -0.5;
		laserBall->moveVec.y = -0.5;
		doStateChange(&StateID_Arm);
	}
}
void daShroobUFO_c::endState_Shoot() {}

void daShroobUFO_c::beginState_FlyAway() {
	bindAnimChr_and_setUpdateRate("ArmIn", 1, 0.0, 1.0);
}
void daShroobUFO_c::executeState_FlyAway() {
	if(this->animationChr.isAnimationDone()) {
		cooldown = 0;
		doStateChange(&StateID_Wait);
	}
}
void daShroobUFO_c::endState_FlyAway() {}





dActor_c* daShroobUFOLaserBall_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daShroobUFOLaserBall_c));
	return new(buffer) daShroobUFOLaserBall_c;
}

const SpriteData ShroobUFOLaserBallSpriteData = { ProfileId::ShroobUFOLaserBall, 0, 0, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile ShroobUFOLaserBallProfile(&daShroobUFOLaserBall_c::build, SpriteId::ShroobUFOLaserBall, &ShroobUFOLaserBallSpriteData, ProfileId::ShroobUFOLaserBall, ProfileId::ShroobUFOLaserBall, "ShroobUFOLaserBall", ShroobUFOLaserBallFileList);

int daShroobUFOLaserBall_c::onCreate() {
	speed = (this->settings >> 8 & 0xF) * 0.25;
	moveVec.x = cos(0.024543692606 * (this->settings & 0xF)) * speed;
	moveVec.y = sin(0.024543692606 * (this->settings & 0xF)) * speed;
	
	return true;
}

int daShroobUFOLaserBall_c::onExecute() {
	this->pos.x += moveVec.x;
	this->pos.y += moveVec.y;
	
	S16Vec nullRot = {0,0,0};
	Vec oneVec = {1.0f, 1.0f, 1.0f};
	effect.spawn("Ufo_Laser", 0, &pos, &nullRot, &oneVec);
	
	return true;
}

int daShroobUFOLaserBall_c::onDelete() {
	return true;
}

int daShroobUFOLaserBall_c::onDraw() {
	return true;
}