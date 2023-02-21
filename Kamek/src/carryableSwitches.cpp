extern "C" bool switchOnCreate(dEn_c* self); //80a197b0
extern "C" void iceSetSpinLiftUpActor(dStageActor_c* self, dStageActor_c* other); //80121ef0

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
	static const lineSensor_s adjacent(3<<12, 6<<12, 6<<12);
	self->collMgr.init(self, &below, NULL, &adjacent);
	self->collMgr.calculateBelowCollisionWithSmokeEffect();
	self->collMgr.isOnTopOfTile();

	return ret;
}

void switchNewSetSpinLiftUpActor(dEn_c* self, dStageActor_c* other) {
	s8* playerNo;

	playerNo = other->getPlrNo();
	self->which_player = *playerNo;
	//self->physics.carryingActor = other;
	self->doStateChange(&daEnJumpdai_c::StateID_CarryUp);
}


void switchNewFinalUpdate(dEn_c* self) {
	if (self->acState.getCurrentState() == &daEnJumpdai_c::StateID_CarryUp) {
		dAcPy_c* player = dAcPy_c::findByID(self->which_player);

		dAcPy_c::getCarryPos(&self->pos, player);
	}



}

void noMoreSwitchPlayerCollision(dEn_c* self, ActivePhysics* apThis, ActivePhysics* apOther) {
	return;
}