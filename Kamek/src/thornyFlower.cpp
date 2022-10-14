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

};


void daThornyFlower_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
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
		this->scale.x = 2.0;
		this->scale.y = 2.0;
		this->scale.z = 2.0;
	} else {
		this->scale.x = 1.0;
		this->scale.y = 1.0;
		this->scale.z = 1.0;
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
	HitMeBaby.bitfield1 = 0x4F; 
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