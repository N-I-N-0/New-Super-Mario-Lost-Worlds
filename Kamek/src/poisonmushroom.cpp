#include <common.h>
#include <game.h>
#include <g3dhax.h>
#include <sfx.h>

//Wrote by RedStoneMatt because CLF78 isn't able to do it :p


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
// Poison Mushroom

const char* PMarcNameList [] = {
	"PoisonMushroom",
	NULL	
};


class daPoisonShroom_c : public dEn_c {
public:
	int onCreate();
	int onDelete();
	int onExecute();
	int onDraw();

	mHeapAllocator_c allocator;
	nw4r::g3d::ResFile resFile;

	m3d::mdl_c bodyModel;

	m3d::anmChr_c chrAnimation;

	float XSpeed;
	u32 cmgr_returnValue;
	bool isOnTopOfTile;

	static dActor_c *build();

	void bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate);
	void updateModelMatrices();
	bool calculateTileCollisions();

	void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther);
	void yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther);

	bool collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther);
	// bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther);

	USING_STATES(daPoisonShroom_c);
	DECLARE_STATE(Out);
	DECLARE_STATE(Walk);
	DECLARE_STATE(Turn);
	DECLARE_STATE(Die);
};

dActor_c *daPoisonShroom_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daPoisonShroom_c));
	return new(buffer) daPoisonShroom_c;
}

const SpriteData PoisonShroomSpriteData = { ProfileId::PoisonShroom, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile PoisonShroomProfile(&daPoisonShroom_c::build, SpriteId::PoisonShroom, &PoisonShroomSpriteData, ProfileId::PoisonShroom, ProfileId::PoisonShroom, "PoisonShroom", PMarcNameList);

///////////////////////
// States
///////////////////////


	CREATE_STATE(daPoisonShroom_c, Out);
	CREATE_STATE(daPoisonShroom_c, Walk);
	CREATE_STATE(daPoisonShroom_c, Turn);
	CREATE_STATE(daPoisonShroom_c, Die);


////////////////////////
// Collision Functions
////////////////////////

	void daPoisonShroom_c::playerCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
		S16Vec nullRot = {0,0,0};
		Vec oneVec = {1.0f, 1.0f, 1.0f};
		SpawnEffect("Wm_en_obakedoor_sm", 0, &this->pos, &nullRot, &oneVec);
				
		PlaySound(this, SE_EMY_MECHAKOOPA_DAMAGE);
		this->kill();
		this->Delete(this->deleteForever);

		this->_vf220(apOther->owner);
		doStateChange(&StateID_Die);
	}

	void daPoisonShroom_c::yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
		return this->playerCollision(apThis, apOther);
	}

	bool daPoisonShroom_c::collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther) {
		S16Vec nullRot = {0,0,0};
		Vec oneVec = {1.0f, 1.0f, 1.0f};
		SpawnEffect("Wm_en_obakedoor_sm", 0, &this->pos, &nullRot, &oneVec);
				
		PlaySound(this, SE_EMY_MECHAKOOPA_DAMAGE);
		this->kill();
		this->Delete(this->deleteForever);

		doStateChange(&StateID_Die);
		return true;
	}

	bool daPoisonShroom_c::collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther) {
		S16Vec nullRot = {0,0,0};
		Vec oneVec = {1.0f, 1.0f, 1.0f};
		SpawnEffect("Wm_en_obakedoor_sm", 0, &this->pos, &nullRot, &oneVec);
				
		PlaySound(this, SE_EMY_MECHAKOOPA_DAMAGE);
		this->kill();
		this->Delete(this->deleteForever);
		
		this->_vf220(apOther->owner);
		doStateChange(&StateID_Die);
		return true;
	}

	bool daPoisonShroom_c::collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther) {
		return this->collisionCatD_Drill(apThis, apOther);
	}

	bool daPoisonShroom_c::collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther) {
		return this->collisionCatD_Drill(apThis, apOther);
	}

	bool daPoisonShroom_c::collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther){
		return this->collisionCatD_Drill(apThis, apOther);
	}

	bool daPoisonShroom_c::collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther) {
		return this->collisionCatD_Drill(apThis, apOther);
	}

	bool daPoisonShroom_c::collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther){
		return this->collisionCatD_Drill(apThis, apOther);
	}

	bool daPoisonShroom_c::collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther){
		return this->collisionCatD_Drill(apThis, apOther);
	}

	bool daPoisonShroom_c::collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther) {
		return this->collisionCatD_Drill(apThis, apOther);
	}


bool daPoisonShroom_c::calculateTileCollisions() {
	// Returns true if sprite should turn, false if not.

	HandleXSpeed();
	HandleYSpeed();
	doSpriteMovement();

	cmgr_returnValue = collMgr.isOnTopOfTile();
	collMgr.calculateBelowCollisionWithSmokeEffect();

	stuffRelatingToCollisions(0.1875f, 1.0f, 0.5f);

	float xDelta = pos.x - last_pos.x;
	if (xDelta >= 0.0f)
		direction = 0;
	else
		direction = 1;

	if (collMgr.isOnTopOfTile()) {
		// Walking into a tile branch

		if (cmgr_returnValue == 0)
			isOnTopOfTile = true;

		speed.y = 0.0f;
		max_speed.x = (direction == 1) ? -0.8f : 0.8f;
	}

	collMgr.calculateAdjacentCollision(0);

	// Switch Direction
	if (collMgr.outputMaybe & (0x15 << direction)) {
		if (collMgr.isOnTopOfTile()) {
			isOnTopOfTile = true;
		}
		return true;
	}
	return false;
}

void daPoisonShroom_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate) {
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr(name);
	this->chrAnimation.bind(&this->bodyModel, anmChr, unk);
	this->bodyModel.bindAnim(&this->chrAnimation, unk2);
	this->chrAnimation.setUpdateRate(rate);
}

int daPoisonShroom_c::onCreate() {

	this->deleteForever = true;

	// Model creation	
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	this->resFile.data = getResource("PoisonMushroom", "g3d/PoisonMushroom.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("PoisonMushroom");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_MapObj(&bodyModel, 0);

	// Animations start here
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr((this->settings == 0) ? "outup" : "outdown");
	this->chrAnimation.setup(mdl, anmChr, &this->allocator, 0);


	allocator.unlink();

	// Stuff I do understand
	this->scale = (Vec){1.0, 1.0, 1.0};

	this->rot.x = 0;
	this->rot.y = 0;
	this->rot.z = 0;
	this->direction = 0; // Heading right.
	
	this->speed.x = 0.0;
	this->speed.y = 0.0;
	this->max_speed.x = -0.8;
	this->x_speed_inc = 0.0;
	this->XSpeed = 0.8;

	ActivePhysics::Info HitMeBaby;

	HitMeBaby.xDistToCenter = 0.0;
	HitMeBaby.yDistToCenter = 4.5;

	HitMeBaby.xDistToEdge = 7.5;
	HitMeBaby.yDistToEdge = 7.5;		

	HitMeBaby.category1 = 0x3;
	HitMeBaby.category2 = 0x0;
	HitMeBaby.bitfield1 = 0x6F;
	HitMeBaby.bitfield2 = 0xffbafffe;
	HitMeBaby.unkShort1C = 0;
	HitMeBaby.callback = &dEn_c::collisionCallback;

	this->aPhysics.initWithStruct(this, &HitMeBaby);
	this->aPhysics.addToList();


	// Tile collider

	// These fucking rects do something for the tile rect
	spriteSomeRectX = 28.0f;
	spriteSomeRectY = 32.0f;
	_320 = 0.0f;
	_324 = 16.0f;

	static const lineSensor_s below(-5<<12, 5<<12, 0<<12);
	static const pointSensor_s above(0<<12, 12<<12);
	static const lineSensor_s adjacent(3<<12, 6<<12, 6<<12);

	collMgr.init(this, &below, &above, &adjacent);
	collMgr.calculateBelowCollisionWithSmokeEffect();

	cmgr_returnValue = collMgr.isOnTopOfTile();

	if (collMgr.isOnTopOfTile())
		isOnTopOfTile = false;
	else
		isOnTopOfTile = true;

	this->pos.y += (this->settings == 0) ? 16 : -18;

	// State Changers
	bindAnimChr_and_setUpdateRate((this->settings == 0) ? "outup" : "outdown", 1, 0.0, 1.0);

	doStateChange(&StateID_Out); 

	this->onExecute();
	return true;
}

int daPoisonShroom_c::onDelete() {
	return true;
}

int daPoisonShroom_c::onExecute() {
	acState.execute();
	updateModelMatrices();

	bodyModel._vf1C();
	
	return true;
}

int daPoisonShroom_c::onDraw() {
	bodyModel.scheduleForDrawing();

	return true;
}

void daPoisonShroom_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y - 2.0, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}


///////////////
// Out State
///////////////
	void daPoisonShroom_c::beginState_Out() {
	}
	void daPoisonShroom_c::executeState_Out() {	
		this->speed.x = 0.0;
		this->speed.y = 0.0;
		this->max_speed.x = 0.0;
		this->max_speed.y = 0.0;

		if(this->chrAnimation.isAnimationDone()) {
			bindAnimChr_and_setUpdateRate("wait", 1, 0.0, 1.0);
			doStateChange(&StateID_Walk);
		}
	}
	void daPoisonShroom_c::endState_Out() { }


///////////////
// Walk State
///////////////
	void daPoisonShroom_c::beginState_Walk() {
		this->max_speed.x = (this->direction) ? -this->XSpeed : this->XSpeed;
		this->speed.x = (this->direction) ? -0.0 : 0.0;
		this->x_speed_inc = 0.1;

		this->max_speed.y = -2;
		this->speed.y = 0.0;
		this->y_speed_inc = -0.15;
	}
	void daPoisonShroom_c::executeState_Walk() {	
		bool ret = calculateTileCollisions();
		if (ret) {
			doStateChange(&StateID_Turn);
		}

		if(this->chrAnimation.isAnimationDone()) {
			this->chrAnimation.setCurrentFrame(0.0);
		}
	}
	void daPoisonShroom_c::endState_Walk() { }


///////////////
// Turn State
///////////////
	void daPoisonShroom_c::beginState_Turn() {
		this->direction ^= 1;
		this->speed.x *= -1;
	}
	void daPoisonShroom_c::executeState_Turn() {
		if(this->chrAnimation.isAnimationDone()) {
			this->chrAnimation.setCurrentFrame(0.0);
		}
		this->doStateChange(&StateID_Walk);
	}
	void daPoisonShroom_c::endState_Turn() { }


///////////////
// Die State
///////////////
	void daPoisonShroom_c::beginState_Die() {
		this->removeMyActivePhysics();
	}
	void daPoisonShroom_c::executeState_Die() {
		
	}
	void daPoisonShroom_c::endState_Die() { }