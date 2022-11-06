const char* ThornyFlowerFileList[] = { "thornyFlower", NULL };

class daThornyFlower_c : public dEn_c {
public:

	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	void updateModelMatrices();
	
	static dActor_c* build();

	//void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther);
	//void spriteCollision(ActivePhysics *apThis, ActivePhysics *apOther);
	//void yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther);

	bool collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat5_Mario(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat8_FencePunch(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat11_PipeCannon(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther); 
	bool collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther);
	bool customCollision(ActivePhysics* apThis, ActivePhysics* apOther);
	bool spikeballCollision(ActivePhysics* apThis, ActivePhysics* apOther);
	bool frogCollision(ActivePhysics* apThis, ActivePhysics* apOther);
	
	//setDeathInfo_IceBreak -> what should be done when in an iceblock and crashes a wall -> normally the sprite should die
	void _vf148();
	//setDeathInfo_IceVanish -> what should be done when collision with an iceblock thrown by Mario -> normally the sprite should die
	void _vf14C();
	
	mHeapAllocator_c allocator;
	m3d::mdl_c bodyModel;
	m3d::anmChr_c animationChr;
	nw4r::g3d::ResFile resFile;

	USING_STATES(daThornyFlower_c);
	void dieFall_Begin();
	void dieFall_Execute();
	void dieFall_End();
};


void daThornyFlower_c::updateModelMatrices() {
	if (this->settings & 2) {
		matrix.translation(pos.x, pos.y-20, pos.z);
	} else {
		matrix.translation(pos.x, pos.y-10, pos.z);
	}
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

dActor_c* daThornyFlower_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daThornyFlower_c));
	return new(buffer) daThornyFlower_c;
}

const SpriteData ThornyFlowerSpriteData = { ProfileId::ThornyFlower, 8, -8, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile ThornyFlowerProfile(&daThornyFlower_c::build, SpriteId::ThornyFlower, &ThornyFlowerSpriteData, ProfileId::ThornyFlower, ProfileId::ThornyFlower, "ThornyFlower", ThornyFlowerFileList);


int daThornyFlower_c::onCreate() {
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	resFile.data = getResource("thornyFlower", "g3d/thornyFlower.brres");
	nw4r::g3d::ResMdl mdl;
	if (this->settings & 1) {
		mdl = this->resFile.GetResMdl("NeedlePlantFly");
	} else {
		mdl = this->resFile.GetResMdl("NeedlePlant");
	}
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Player(&bodyModel, 0);

	allocator.unlink(); 

	if (this->settings & 2) {
		this->scale.x = 0.25;
		this->scale.y = 0.25;
		this->scale.z = 0.25;
	} else {
		this->scale.x = 0.125;
		this->scale.y = 0.125;
		this->scale.z = 0.125;
	}

	ActivePhysics::Info HitMeBaby; 
	HitMeBaby.xDistToCenter = 0.0; 
	HitMeBaby.yDistToCenter = 0.0; 
	if (this->settings & 2) {
		HitMeBaby.xDistToEdge = 24.0; 
		HitMeBaby.yDistToEdge = 24.0; 
	} else {
		HitMeBaby.xDistToEdge = 12.0;
		HitMeBaby.yDistToEdge = 12.0;
	}
	HitMeBaby.category1 = 0x3; 
	HitMeBaby.category2 = 0x0; 
	HitMeBaby.bitfield1 = 0x6F; 
	HitMeBaby.bitfield2 = 0xffbafffe; 
	HitMeBaby.unkShort1C = 0; 
	HitMeBaby.callback = &dEn_c::collisionCallback; 
	this->aPhysics.initWithStruct(this, &HitMeBaby); 
	this->aPhysics.addToList();

	return true;
}

int daThornyFlower_c::onExecute() {
	updateModelMatrices();
	bodyModel._vf1C();
	acState.execute();

	this->rot.y += 0x400;

	return true;
}

int daThornyFlower_c::onDelete() {
	return true;
}

int daThornyFlower_c::onDraw() {
	bodyModel.scheduleForDrawing();

	return true;
}



void daThornyFlower_c::_vf148() {
	dEn_c::_vf148();
	this->Delete(1);
}

void daThornyFlower_c::_vf14C() {
	dEn_c::_vf14C();
	this->Delete(1);
}






void daThornyFlower_c::dieFall_Begin() {}
void daThornyFlower_c::dieFall_Execute() {
	this->removeMyActivePhysics();
	if (this->scale.x <= 0.01) {
		this->Delete(1);
	}else {
		scale.x -= 0.005;
		scale.y -= 0.005;
		scale.z -= 0.005;
	}
}
void daThornyFlower_c::dieFall_End() {}








bool daThornyFlower_c::collisionCat3_StarPower(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daThornyFlower_c::collisionCat7_GroundPound(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daThornyFlower_c::collisionCat7_GroundPoundYoshi(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daThornyFlower_c::collisionCatD_Drill(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daThornyFlower_c::collisionCatA_PenguinMario(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}

bool daThornyFlower_c::collisionCat1_Fireball_E_Explosion(ActivePhysics* apThis, ActivePhysics* apOther) {
	fireballInvalid(apThis, apOther);
	return false;
}
bool daThornyFlower_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics* apThis, ActivePhysics* apOther) {
	iceballInvalid(apThis, apOther);
	return false;
}
bool daThornyFlower_c::collisionCat9_RollingObject(ActivePhysics* apThis, ActivePhysics* apOther) {
	return dEn_c::collisionCat9_RollingObject(apThis, apOther);
}
bool daThornyFlower_c::collisionCat13_Hammer(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daThornyFlower_c::collisionCat14_YoshiFire(ActivePhysics* apThis, ActivePhysics* apOther) {
	fireballInvalid(apThis, apOther);
	return false;
}

/*bool daThornyFlower_c::collisionCat3_StarPower(ActivePhysics* apThis, ActivePhysics* apOther) {
	return dEn_c::collisionCat3_StarPower(apThis, apOther);
}*/






bool daThornyFlower_c::customCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	if((apThis->info.bitfield2 & 0x80000) != 0) {								//if sprite can be hit by hammer -> sprites that need another behaviour get entirely different functions, just doing some basic filtering for enemies
		switch (((daBoomerangHax_c*)apOther->owner)->variation) {
			case 0:
				return false;
			case 1:
				return this->spikeballCollision(apThis, apOther);
			case 3:
				return this->frogCollision(apThis, apOther);;
			default:
				return false;
		}
	}
	return false;
}

bool daThornyFlower_c::spikeballCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	return dEn_c::collisionCat13_Hammer(apThis, apOther);
}

bool daThornyFlower_c::frogCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	((daBoomerangHax_c*)apOther->owner)->destroyBubble();
	return false;
}

