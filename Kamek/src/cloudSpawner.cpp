const char* cloudSpawnerFileList[] = { "I_cloud", NULL };

class daCloudSpawner_c : public daEnBlockMain_c {
public:
	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	mHeapAllocator_c allocator;
	nw4r::g3d::ResFile resFile;
	m3d::mdl_c bodyModel;
	m3d::anmChr_c animationChr;

	s16 effectTimer;
	int type6wait;
	bool type6reappearing;

	static dActor_c *build();

	void bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate);
	void updateModelMatrices();
	void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther);
	void yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther);

	bool collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther);

	bool collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther);

	bool customCollision(ActivePhysics *apThis, ActivePhysics *apOther);
};

extern int getNybbleValue(u32 settings, int fromNybble, int toNybble);
extern "C" void *dAcPy_c__ChangePowerupWithAnimation(void * Player, int powerup);
extern "C" int CanExitThisLevel();
u8 hijackMusicWithSongName(const char *songName, int themeID, bool hasFast, int channelCount, int trackCount, int *wantRealStreamID);


void daCloudSpawner_c::playerCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	OSReport("spawning item\n");
	this->scale = (Vec){0.0, 0.0, 0.0};
	int itemsets = 0 | (3 << 0) | (1 << 7);//| (0x1 << 22);
	VEC3 itempos = {this->pos.x + 8, this->pos.y - 28, this->pos.z};
	dStageActor_c *item = dStageActor_c::create(EN_ITEM, itemsets, &apOther->owner->pos, 0, 0);
	item->addMyActivePhysics();
	item->speed.y = 0;
	item->scale = (Vec){0.0, 0.0, 0.0};
	int doRespawn = getNybbleValue(this->settings, 6, 6);
	if(doRespawn == 1) {
		this->removeMyActivePhysics();
		this->type6wait = 180;
	}
	else {
		this->Delete(1);
	}
}

void daCloudSpawner_c::yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	return playerCollision(apThis, apOther);
}

bool daCloudSpawner_c::collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther) {
	return false;
}
bool daCloudSpawner_c::collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther) {
	return collisionCat7_GroundPound(apThis, apOther);
}
bool daCloudSpawner_c::collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther) {
	return collisionCat9_RollingObject(apThis, apOther);
}
bool daCloudSpawner_c::collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther) {
	return collisionCat9_RollingObject(apThis, apOther);
}

bool daCloudSpawner_c::collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther) {
	return false;
}
bool daCloudSpawner_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther) {

	return false;
}
bool daCloudSpawner_c::collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther) {
	return false;
}
bool daCloudSpawner_c::collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther) {
	return collisionCat9_RollingObject(apThis, apOther);
}
bool daCloudSpawner_c::customCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	return false;
}
bool daCloudSpawner_c::collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther) {
	return collisionCat1_Fireball_E_Explosion(apThis, apOther);
}

bool daCloudSpawner_c::collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther) {
	return collisionCat9_RollingObject(apThis, apOther);
}


dActor_c *daCloudSpawner_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daCloudSpawner_c));
	return new(buffer) daCloudSpawner_c;
}


const SpriteData CloudSpawnerSpriteData = {ProfileId::CloudSpawner, 0x5, -0x31, 0, 0x10, 0x10, 0x40, 0x40, 0x40, 0, 0, 0};
Profile CloudSpawnerProfile(&daCloudSpawner_c::build, SpriteId::CloudSpawner, &CloudSpawnerSpriteData, ProfileId::CloudSpawner, ProfileId::CloudSpawner, "CloudSpawner", cloudSpawnerFileList);



void daCloudSpawner_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate) {
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr(name);
	this->animationChr.bind(&this->bodyModel, anmChr, unk);
	this->bodyModel.bindAnim(&this->animationChr, unk2);
	this->animationChr.setUpdateRate(rate);
}




int daCloudSpawner_c::onCreate() {
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	this->resFile.data = getResource("I_cloud", "g3d/I_fireflower.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("I_fireflower");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Item(&bodyModel, 0);

	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr("wait2");
	this->animationChr.setup(mdl, anmChr, &this->allocator, 0);

	allocator.unlink();


	// Stuff I do understand
	this->scale = (Vec){1.4, 1.4, 1.4};

	this->rot.x = -0x4000;
	this->rot.y = 0;
	this->rot.z = 0;


	ActivePhysics::Info HitMeBaby;
	HitMeBaby.xDistToCenter = 8.0;
	HitMeBaby.yDistToCenter = -15.0;
	HitMeBaby.xDistToEdge = 7.0;
	HitMeBaby.yDistToEdge = 7.0;
	HitMeBaby.category1 = 0x3;
	HitMeBaby.category2 = 0x0;
	HitMeBaby.bitfield1 = 0x6F;
	HitMeBaby.bitfield2 = 0xFFC00000;
	HitMeBaby.unkShort1C = 0;
	HitMeBaby.callback = &dEn_c::collisionCallback;

	this->aPhysics.initWithStruct(this, &HitMeBaby);
	this->aPhysics.addToList();

	this->pos.z = 3300.0f;
	this->pos.y -= 2;


	// bindAnimChr_and_setUpdateRate("wait2", 1, 0.0, 1.0);

	this->onExecute();
	return true;
}



int daCloudSpawner_c::onDelete() {
	return true;
}



int daCloudSpawner_c::onDraw() {
	bodyModel.scheduleForDrawing();
	return true;
}



void daCloudSpawner_c::updateModelMatrices() {
	float posytrans = -24.0f;
	float posxtrans = 8.0f;

	matrix.translation(pos.x + posxtrans, pos.y + posytrans, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}


int daCloudSpawner_c::onExecute() {
	updateModelMatrices();
	this->rot.y += 0x200;
	effectTimer++;

	if(this->type6wait != 0) {
		this->scale = (Vec){0.0, 0.0, 0.0};
		this->type6wait--;
	} else {
		this->type6reappearing = true;
		if(this->type6reappearing && this->scale.x < 1.4) {
			this->scale.x += 0.01;
			this->scale.y += 0.01;
			this->scale.z += 0.01;
		}
		if(this->scale.x > 1.4) {
			this->scale = (Vec){1.4, 1.4, 1.4};
		}
		if(this->scale.x > 1.0) {
			this->addMyActivePhysics();

			if(effectTimer % 40 == 0) {
				S16Vec nullRot = {0,0,0};
				Vec efScale = {0.5f, 0.5f, 0.5f};
				Vec efPos = {this->pos.x + 8, this->pos.y - 12, this->pos.z};
				SpawnEffect("Wm_ob_starcoinget_ring", 0, &efPos, &nullRot, &efScale);
			}
		}
	}
	
	if(this->animationChr.isAnimationDone()) {
		this->animationChr.setCurrentFrame(0.0);
	}


}