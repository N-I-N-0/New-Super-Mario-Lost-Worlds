const char* DarkStarFileList [] = { "I_ztar", NULL };

class daDarkStar_c : public dEn_c {
public:

	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	void updateModelMatrices();
	void bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate);

	static dActor_c* build();

	bool spitOut(void *other);
	void addActivePhysics();

	//void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther);

	bool collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther); 
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
	
	u16 unk1;	//0xdd4
	u16 unk2;	//0xdd8
	int unk3;	//0xd88


	mEf::es2 effect;
	bool readdActivePhysicsNext;
	u8 timer;

	USING_STATES(daDarkStar_c);
	DECLARE_STATE(StarMove);
	DECLARE_STATE(DieStar);
	
	bool FUN_80a290c0();
	void FUN_80a288d0();
	void FUN_80a29280();
};

CREATE_STATE(daDarkStar_c, StarMove);
CREATE_STATE(daDarkStar_c, DieStar);

void daDarkStar_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

bool daDarkStar_c::collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther) {
	((dEn_c*)apThis->owner)->acState.setState(&daDarkStar_c::StateID_DieStar);
	((dAcPy_c*)apOther->owner)->endStar();
	SpawnEffect("Wm_ob_itemget", 0, &pos, &(S16Vec){0,0,0}, &(Vec){1.0, 1.0, 1.0});
}

void daDarkStar_c::beginState_DieStar() {
	this->aPhysics.removeFromList();
	this->timer = 0;
}
void daDarkStar_c::executeState_DieStar() {
/*
	if(timer >= 120) {
		this->Delete(1);
	} else if (timer >= 60) {
		float newScale = 1-((timer-60.0)/60.0);
		this->scale = (Vec){newScale, newScale, newScale};
	} else {
		this->pos.y += timer / 50;
	}
*/
	if(timer >= 20) {
		this->Delete(1);
	} else {
		float newScale = 1.1-(1.1*timer/20.0);
		this->scale = (Vec){newScale, newScale, newScale};
		this->rot.y += 0x1000 + timer*0x30;
		timer++;
	}
}
void daDarkStar_c::endState_DieStar() {}



bool daDarkStar_c::spitOut(void *other) {
	this->direction = ((dStageActor_c*)other)->direction;
	this->acState.setState(&StateID_StarMove);
	timer = 0;
	readdActivePhysicsNext = true;
	this->scale = (Vec){1.1, 1.1, 1.1};
	return true;
}


void daDarkStar_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate) {
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr(name);
	this->animationChr.bind(&this->bodyModel, anmChr, unk);
	this->bodyModel.bindAnim(&this->animationChr, unk2);
	this->animationChr.setUpdateRate(rate);
}

dActor_c* daDarkStar_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daDarkStar_c));
	return new(buffer) daDarkStar_c;
}

const SpriteData DarkStarSpriteData = { ProfileId::DarkStar, 0, 0, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile DarkStarProfile(&daDarkStar_c::build, SpriteId::DarkStar, &DarkStarSpriteData, ProfileId::DarkStar, ProfileId::DarkStar, "Dark Star", DarkStarFileList);


int daDarkStar_c::onCreate() {
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	resFile.data = getResource("I_ztar", "g3d/I_ztar.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("I_ztar");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Enemy(&bodyModel, 0);
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr("wait");
	this->animationChr.setup(mdl, anmChr, &this->allocator, 0);

	allocator.unlink(); 

	addActivePhysics();


	static const lineSensor_s below(-5<<12, 5<<12, 0<<12);
	static const pointSensor_s above(0<<12, 12<<12);
	static const lineSensor_s adjacent(3<<12, 6<<12, 6<<12);

	collMgr.init(this, &below, &above, &adjacent);
	collMgr.calculateBelowCollisionWithSmokeEffect();

	this->scale = (Vec){1.1, 1.1, 1.1};
	
	//this->rot.y = -0x2000;
	
	//this->disableEatIn();

	//MakeMarioEnterDemoMode();

	this->eatenState = 0;
	this->edible = 1;

	doStateChange(&StateID_StarMove);

	return true;
}

void daDarkStar_c::addActivePhysics() {
	ActivePhysics::Info HitMeBaby;
	HitMeBaby.xDistToCenter = 0.0;
	HitMeBaby.yDistToCenter = 7.5;
	HitMeBaby.xDistToEdge = 7.5;
	HitMeBaby.yDistToEdge = 7.5;
	HitMeBaby.category1 = 0x3;
	HitMeBaby.category2 = 0x0;
	HitMeBaby.bitfield1 = 0x6F;
	HitMeBaby.bitfield2 = 0xFFC18000;
	HitMeBaby.unkShort1C = 0;
	HitMeBaby.callback = &dEn_c::collisionCallback;
	this->aPhysics.initWithStruct(this, &HitMeBaby);
	this->aPhysics.addToList();
}

int daDarkStar_c::onExecute() {
	acState.execute();

	updateModelMatrices();
	bodyModel._vf1C();

	/*if (this->animationChr.isAnimationDone()) {
		this->animationChr.setCurrentFrame(0.0);
	}*/

	if (readdActivePhysicsNext) {
		if(timer < 45) {
			timer++;
		} else {
			readdActivePhysicsNext = false;
			this->addActivePhysics();
		}
	}


	return true;
}

int daDarkStar_c::onDelete() {
	return true;
}

int daDarkStar_c::onDraw() {
	bodyModel.scheduleForDrawing();

	return true;
}




void daDarkStar_c::beginState_StarMove() {
  //if (*(char *)((int)this->acState.ac1.statemgr.vtable + 3) == 0) {
  if(true) {
	this->speed.y = 4.0f;
    this->speed.x = (this->direction ? -1.0f : 1.0f);
  } else {
    this->speed.x = 0.0f;
    this->speed.y = 0.0f;
  }
  checkLiquidImmersionAndKillIfTouchingLava(&this->pos, 1.0f);
  this->FUN_80a290c0();
  return;
}


void daDarkStar_c::FUN_80a288d0() {
	if (unk2 == 0) {
		return;
	}
	if (unk1 != 0) {
		unk1 -= 1;
	}
	if (unk1 != 0) {
		return;
	}
	unk2 = 0;
	return;
}


bool daDarkStar_c::FUN_80a290c0() {
  int iVar1 = this->EnWaterFlagCheck(&this->pos);
  if (iVar1) {
    this->isImmersedInLiquid_maybe = 1;
  }
  return iVar1 != 0;
}


void daDarkStar_c::FUN_80a29280(){
  if (unk3 == 3) {
    return;
  }
  //EnItem_BindAnimation(this,3);
  return;
}


extern "C" int SomeStrangeModification(dStageActor_c* actor);
extern "C" float EnItem_GetZPosToUse(dEn_c* item);
extern "C" void FUN_80a28e00(dActor_c* item);
extern "C" void FUN_80a28d10(dActor_c* item);
extern "C" void FUN_80a29110(dActor_c* item);
extern "C" void FUN_80a286c0(dActor_c* item);

void daDarkStar_c::executeState_StarMove(){
  //bool bVar1;
  uint uVar3;
  bool bVar5;
  int iVar4;
  float fVar6;

  effect.spawn("Wm_ob_startail", 0, &pos, &(S16Vec){0,0,0}, &(Vec){1.0, 1.0, 1.0});

  this->HandleYSpeed();
  this->doSpriteMovement();
  this->FUN_80a288d0();
  //FUN_80a28bb0(this);
  if (this->unk2 == 0) {
    uVar3 = SomeStrangeModification(this);
    FUN_80a28e00(this);
    if (uVar3 & 1) {
      if (this->isOnQuickSand == 0) {
        if (this->isImmersedInLiquid_maybe == 0) {
          this->speed.y = 4.0f;
        } else {
          this->speed.y = 2.0f;
        }
        FUN_80a28d10(this);
        this->FUN_80a29280();
        Vec2 soundPos;
        ConvertStagePositionToScreenPosition(&soundPos,&this->pos);
        SoundPlayingClass::instance2->PlaySoundAtPosition(SE_OBJ_STAR_BOUND,&soundPos,0);
      }
      /*if (*(char *)((int)&this[2]._.acState.ac1.statemgr.vtable + 3) == '\x01') {
        bVar1 = this->direction;
        *(undefined *)((int)&this[2]._.acState.ac1.statemgr.vtable + 3) = 2;
        this->speed.x = (this->direction ? -1.0f : 1.0f);
      }*/
    }
    if (uVar3 & 2) {
      this->speed.y = -2.0f;
    }
    if (uVar3 & 4) {
      this->direction ^= 1;
      this->speed.x = -this->speed.x;
    }
    FUN_80a29110(this);
    
    if (this->checksSomething()) {
      FUN_80a286c0(this);
      return;
    }
  }
  
  this->pos.z = 600.0f;//EnItem_GetZPosToUse(this);
/*
  iVar4 = FUN_80a282f0(this);
  if (iVar4 == 0) {
    FUN_80a28770(this);
  }
  FUN_80a28230(this);
*/
  return;
}


void daDarkStar_c::endState_StarMove() {}