extern "C" bool switchOnCreate(dEn_c* self); //80a197b0
extern "C" void iceSetSpinLiftUpActor(dStageActor_c* self, dStageActor_c* other); //80121ef0

extern "C" void FUN_80a1a1e0(dEn_c*, daPlBase_c*);
extern "C" void FUN_80a1a320(dEn_c*, daPlBase_c*);
extern "C" void FUN_80080dc0(dEn_c*, daPlBase_c*);
extern "C" void FUN_80080de0(dEn_c*, daPlBase_c*);
extern "C" void FUN_80080e00(dEn_c*, daPlBase_c*);

extern "C" void FUN_80a1a650(dEn_c*);


bool switchNewOnCreate(dEn_c* self) {
	bool ret = switchOnCreate(self);

	ActivePhysics::Info HitMeBaby;
	HitMeBaby.xDistToCenter = 0.0;
	HitMeBaby.yDistToCenter = 9.0;
	HitMeBaby.xDistToEdge = 9.0;
	HitMeBaby.yDistToEdge = 9.0;
	HitMeBaby.category1 = 0x5;
	HitMeBaby.category2 = 0x0;
	HitMeBaby.bitfield1 = 0x4F;
	HitMeBaby.bitfield2 = 0x00040000;
	HitMeBaby.unkShort1C = 0;
	HitMeBaby.callback = &dEn_c::collisionCallback;

	self->aPhysics.initWithStruct(self, &HitMeBaby);
	self->aPhysics.addToList();

	static const lineSensor_s below(-5<<12, 5<<12, 0<<12);
	static const pointSensor_s above(0<<12, 12<<12);
	static const lineSensor_s adjacent(3<<12, 6<<12, 6<<12);
	self->collMgr.init(self, &below, &above, &adjacent);
	self->collMgr.calculateBelowCollisionWithSmokeEffect();
	self->collMgr.isOnTopOfTile();

	return ret;
}

void switchNewSetSpinLiftUpActor(dEn_c* self, dStageActor_c* other) {
	s8* playerNo;

	playerNo = other->getPlrNo();
	self->which_player = *playerNo;
	Physics* physics = (Physics*)((u32)self+0x754);
	physics->somePlayer = other;
	self->doStateChange(&daEnHnSwich_c::StateID_CarryUp);
}




void switchNewFinalUpdate(dEn_c* self) {
	if (self->acState.getCurrentState() == &daEnHnSwich_c::StateID_CarryUp) {
		dAcPy_c* player = dAcPy_c::findByID(self->which_player);
		dAcPy_c::getCarryPos(&self->pos, player);
	} else {
		Physics* physics = (Physics*)((u32)self+0x754);
		physics->somePlayer = NULL;
	}
	
	
	OSReport("state: %s\n", self->acState.getCurrentState()->getName());
}

void noMoreSwitchPlayerCollision(dEn_c* self, ActivePhysics* apThis, ActivePhysics* apOther) {
	return;
}














void daEnHnSwich_c::rotateIfCarryBool2Set(){
	bool angleReached;

	if (this->carryBool2) {
		angleReached = SmoothRotation(&this->rot.y, 0, 0x800);
		if (angleReached) {
			this->carryBool2 = false;
		}
	}
}

void daEnHnSwich_c::beginState_CarryUpThrow() {
	dAcPy_c *player;
	bool isOnGround;
	float newBaseSpeed;
	float speedX;

	player = dAcPy_c::findByID(this->which_player);
	speedX = player->speed.x;
	newBaseSpeed = (this->direction ? -2.5f : 2.5f);
	this->speed.y = -1.5;
	this->speed.z = 0.0;
	this->speed.x = newBaseSpeed + speedX * 0.75;

	isOnGround = this->collMgr.isOnTopOfTile();
	if ((!isOnGround) || (abs(player->speed.x) >= 0.5)) {
		this->carryBool = false;
	} else {
		this->carryBool = true;
	}

	this->carryBool2 = true;
	this->dEn_c::EnBgCheckFoot();
	this->collMgr.calculateAboveCollision(collMgr.outputMaybe);
	this->dEn_c::EnBgCheckWall();


/*	int i = 0;
	do {
		FUN_80a39630(this,i);
		i = i + 1;
	} while (i < 4);*/

	//this->pos.z = this->initialZPosition;
}

void daEnHnSwich_c::executeState_CarryUpThrow() {
	this->dActor_c::HandleYSpeed();
	this->dEn_c::doSpriteMovement();
	this->daEnHnSwich_c::rotateIfCarryBool2Set();
	this->dEn_c::EnBgCheckFoot();

	this->collMgr.calculateAboveCollision(collMgr.outputMaybe);

	this->dEn_c::EnBgCheckWall();

	if (this->collMgr.isOnTopOfTile()) {
		if (!this->carryBool) {
			this->speed.y = 2.0;
			this->speed.x = this->speed.x * 0.5;
			this->carryBool = true;
		} else {
			this->speed.y = 0.0;
			this->speed.x = 0.0;
			this->which_player = -1;
			doStateChange(&daEnHnSwich_c::StateID_Wait);
		}
	} else if (this->collMgr.outputMaybe & 0x3c000000) {
		this->speed.y = -0.8125;
	}

	if (this->collMgr.outputMaybe & (0x15 << this->direction)) {
		this->direction ^= 1;
		this->speed.x = -this->speed.x;
		this->max_speed.x = -this->max_speed.x;;
	}

	this->dEn_c::checkLiquidImmersionAndKillIfTouchingLava(&this->pos, 1.0);

	this->dStageActor_c::checkZoneBoundaries(0);
}

void daEnHnSwich_c::endState_CarryUpThrow() {
	//this->aPhysics.info.callback = JumpDai_CollisionCallback;
}








CREATE_STATE(daEnHnSwich_c, CarryUp);
CREATE_STATE(daEnHnSwich_c, CarryUpThrow);

void daEnHnSwich_c::beginState_CarryUp() {}

void daEnHnSwich_c::executeState_CarryUp() {
	dAcPy_c* player;
	s16 spinAngle;
	dStateBase_c* currentState;
	bool isCarryUpThrow;

	player = dAcPy_c::findByID(this->which_player);
	isCarryUpThrow = player->daPlBase_c::testFlag(4);
	if ((this->carryingFlags & 2 || this->carryingFlags & 1) || isCarryUpThrow) {
		this->direction = this->directionForCarry;
		for(;;)
		this->y_speed_inc = -0.1875;
		this->doStateChange(&daEnHnSwich_c::StateID_CarryUpThrow);
	} else {
		spinAngle = player->dAcPy_c::getMissSpinAngle();
		this->rot.y = player->rot.y + spinAngle;
	}

	//this->field_0x5ec = 0.0;

	isCarryUpThrow = player->daPlBase_c::testFlag(0x46);
	if (!isCarryUpThrow) {
		if (this->collMgr.calculateAboveCollision(0)) {
			currentState = this->acState.getCurrentState();
			isCarryUpThrow = (currentState == &daEnHnSwich_c::StateID_CarryUpThrow);
			if (!isCarryUpThrow) {
				this->doStateChange(&daEnHnSwich_c::StateID_CarryUpThrow);
				this->speed.x = 0.0;
				this->speed.y = -0.5;
				this->speed.z = 0.0;
			}
		}
	}

	//FUN_80a39e20(this);
	return;
}

void daEnHnSwich_c::endState_CarryUp() {
	dAcPy_c *player;

	player = dAcPy_c::findByID(this->which_player);
	player->dAcPy_c::dropObject(this);
	this->carryingFlags &= 0xfffffffc;
	this->rCollDetector.setRide(NULL);
	this->collMgr.outputMaybe = 0;
	return;
}