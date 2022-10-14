#include <common.h>
#include <game.h>
#include <g3dhax.h>
#include <sfx.h>
#include <profile.h>

const char* TarcNameList[] = { "octo", NULL };

void taktakCollisionCallback(ActivePhysics* apThis, ActivePhysics* apOther);

class daTakobo : dEn_c {
	int onCreate();			
	int onExecute();		
	int onDraw();			
	int onDelete();			

	mHeapAllocator_c allocator;
	nw4r::g3d::ResFile resFile;
	m3d::mdl_c bodyModel;
	m3d::anmChr_c chrAnimation;

	bool canWalk;
	int cooldown;
	int gonnaIgnoreRandomTimer;
	u32 cmgr_returnValue;
	bool isBouncing;
	u16 leftRot;
	u16 rightRot;
	float prepAttackMatrixMover;
	mEf::es2 effect;

	void setAnimation_and_updateRate(const char* animationName, int playsOnce, float unk2, float updateRate);
	void setupBodyModels();
	void updateModelMatrix();

	void setupTileCollider();
	bool calculateTileCollisions();
	void setupActivePhysics();
	void spriteCollision(ActivePhysics* apThis, ActivePhysics* apOther);
	void playerCollision(ActivePhysics* apThis, ActivePhysics* apOther);
	void yoshiCollision(ActivePhysics* apThis, ActivePhysics* apOther);

	bool collisionCat1_Fireball_E_Explosion(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat7_GroundPound(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat7_GroundPoundYoshi(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCatD_Drill(ActivePhysics* apThis, ActivePhysics* apOther);

	void addScoreWhenHit(void* other);

	void playOnStompSound();
	void playDeathSound();
	void playShootSound();

	//setDeathInfo_IceBreak -> what should be done when in an iceblock and crashes a wall -> normally the sprite should die
	void _vf148();
	//setDeathInfo_IceVanish -> what should be done when collision with an iceblock thrown by Mario -> normally the sprite should die
	void _vf14C();
	bool CreateIceActors();

	void move();
	bool willWalkOntoSuitableGround();
	int nearestPlayer();
	void attackPlayerMaybe();

	USING_STATES(daTakobo);
	DECLARE_STATE(IdleWalk);
	DECLARE_STATE(Turn);
	DECLARE_STATE(PrepareAttack);
	DECLARE_STATE(Attack);
	DECLARE_STATE(Dizzy);
	DECLARE_STATE(DiePress);

	public: static dActor_c* build();
};

// #								-ID- ----  -X Offs- -Y Offs-  -RectX1- -RectY1- -RectX2- -RectY2-  -1C- -1E- -20- -22-  Flag ----
const SpriteData OctoSpriteData = { ProfileId::Octo, 8, -0x10, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile OctoProfile(&daTakobo::build, SpriteId::Octo, &OctoSpriteData, ProfileId::Octo, ProfileId::Octo, "Octo", TarcNameList);

dActor_c* daTakobo::build() {
	void* buffer = AllocFromGameHeap1(sizeof(daTakobo));
	return new(buffer) daTakobo;
}

////////////////////////////////
// Extern functions and States 
////////////////////////////////
extern "C" char usedForDeterminingStatePress_or_playerCollision(dEn_c * t, ActivePhysics * apThis, ActivePhysics * apOther, int unk1);
extern "C" void sub_80024C20(void);
extern "C" void __destroy_arr(void*, void(*)(void), int, int);

CREATE_STATE(daTakobo, IdleWalk);
CREATE_STATE(daTakobo, Turn);
CREATE_STATE(daTakobo, PrepareAttack);
CREATE_STATE(daTakobo, Attack);
CREATE_STATE(daTakobo, Dizzy);
CREATE_STATE(daTakobo, DiePress);

///////////////////////
// Collision functions
///////////////////////

void taktakCollisionCallback(ActivePhysics* apThis, ActivePhysics* apOther) {
	if (apOther->owner->name == ProfileId::OctoBullet) {
		return;
	}
	dEn_c::collisionCallback(apThis, apOther);
}

void daTakobo::addScoreWhenHit(void* other) { }

void daTakobo::spriteCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	u16 name = ((dEn_c*)apOther->owner)->name;
	if (name == EN_COIN || name == EN_EATCOIN || name == AC_BLOCK_COIN || name == EN_COIN_JUGEM || name == EN_COIN_ANGLE
		|| name == EN_COIN_JUMP || name == EN_COIN_FLOOR || name == EN_COIN_VOLT || name == EN_COIN_WIND
		|| name == EN_BLUE_COIN || name == EN_COIN_WATER || name == EN_REDCOIN || name == EN_GREENCOIN
		|| name == EN_JUMPDAI || name == EN_ITEM)
	{
		return;
	}

	if (acState.getCurrentState() == &StateID_IdleWalk && canWalk != 0) {
		pos.x = ((pos.x - ((dEn_c*)apOther->owner)->pos.x) > 0) ? pos.x + 1.5 : pos.x - 1.5;
		doStateChange(&StateID_Turn);
	}

	dEn_c::spriteCollision(apThis, apOther);
}

void daTakobo::playerCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	char hitType;
	hitType = usedForDeterminingStatePress_or_playerCollision(this, apThis, apOther, 0);

	if (hitType == 1 || hitType == 3)	//normal jump or spin jump
	{
		apOther->someFlagByte |= 2;
		playOnStompSound();
		doStateChange(&StateID_Dizzy);
	}
	else if (hitType == 0) {
		dEn_c::playerCollision(apThis, apOther);
		this->_vf220(apOther->owner);
	}

	//weird stuff
	deathInfo.isDead = 0;
	this->flags_4FC |= (1 << (31 - 7));
	this->counter_504[apOther->owner->which_player] = 0;
}

void daTakobo::yoshiCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->playerCollision(apThis, apOther);
}

bool daTakobo::collisionCat1_Fireball_E_Explosion(ActivePhysics* apThis, ActivePhysics* apOther) {
	playOnStompSound();
	doStateChange(&StateID_Dizzy);
	return true;
}

bool daTakobo::collisionCat7_GroundPound(ActivePhysics* apThis, ActivePhysics* apOther) {
	return this->collisionCatD_Drill(apThis, apOther);
}

bool daTakobo::collisionCat7_GroundPoundYoshi(ActivePhysics* apThis, ActivePhysics* apOther) {
	return this->collisionCatD_Drill(apThis, apOther);
}

bool daTakobo::collisionCatD_Drill(ActivePhysics* apThis, ActivePhysics* apOther) {
	playDeathSound();
	SpawnEffect("Wm_mr_hardhit", 0, &pos, &(S16Vec){0, 0, 0}, & (Vec){1.0, 1.0, 1.0});
	doStateChange(&StateID_DiePress);
	return true;
}

void daTakobo::setupTileCollider() {
	spriteSomeRectX = 28.0f;
	spriteSomeRectY = 32.0f;
	_320 = 0.0f;
	_324 = 16.0f;

	static const lineSensor_s below(-5 << 12, 5 << 12, 0 << 12);
	static const pointSensor_s above(0 << 12, 20 << 12);
	static const lineSensor_s adjacent(6 << 12, 14 << 12, 11 << 12);
	collMgr.init(this, &below, &above, &adjacent);
	collMgr.calculateBelowCollisionWithSmokeEffect();
	cmgr_returnValue = collMgr.isOnTopOfTile();
	if (collMgr.isOnTopOfTile()) { this->isBouncing = false; }
	else { this->isBouncing = true; }
}

bool daTakobo::calculateTileCollisions() {

	cmgr_returnValue = collMgr.isOnTopOfTile();
	collMgr.calculateBelowCollisionWithSmokeEffect();

	if (isBouncing) {
		stuffRelatingToCollisions(0.1875f, 1.0f, 0.5f);
		if (speed.y != 0.0f)
			isBouncing = false;
	}

	float xDelta = pos.x - last_pos.x;
	//NOT >=, just >
	if (xDelta > 0.0f)
		direction = 0;
	else if (xDelta < 0.0f)
		direction = 1;

	if (collMgr.isOnTopOfTile()) {
		// Walking into a tile branch

		if (cmgr_returnValue == 0)
			isBouncing = true;

		speed.y = 0.0f;

		max_speed.x = (direction == 1) ? -0.6f : 0.6f;
	}
	else {
		x_speed_inc = 0.0f;
	}

	// Bouncing checks
	if (_34A & 4) {
		Vec v = (Vec){ 0.0f, 1.0f, 0.0f };
		collMgr.pSpeed = &v;

		if (collMgr.calculateAboveCollision(collMgr.outputMaybe))
			speed.y = 0.0f;

		collMgr.pSpeed = &speed;

	}
	else {
		if (collMgr.calculateAboveCollision(collMgr.outputMaybe))
			speed.y = 0.0f;
	}

	collMgr.calculateAdjacentCollision(0);

	// Switch Direction
	if (collMgr.outputMaybe & (0x15 << direction)) {
		if (collMgr.isOnTopOfTile()) {
			isBouncing = true;
		}
		return true;
	}
	return false;
}

void daTakobo::setupActivePhysics() {
	ActivePhysics::Info physicsInfo;
	physicsInfo.xDistToCenter = 0.0;
	physicsInfo.yDistToCenter = 12.0;
	physicsInfo.xDistToEdge = 9.0;
	physicsInfo.yDistToEdge = 12.0f;
	physicsInfo.category1 = 0x3;
	physicsInfo.category2 = 0x0;
	physicsInfo.bitfield1 = 0x4F;
	physicsInfo.bitfield2 = 0xFFBAFFFE;
	physicsInfo.unkShort1C = 0;
	physicsInfo.callback = &taktakCollisionCallback;

	this->aPhysics.initWithStruct(this, &physicsInfo);
	this->aPhysics.addToList();
}

bool daTakobo::willWalkOntoSuitableGround() {
	static const float deltas[] = { 3.0f, -3.0f };
	VEC3 checkWhere = {
			pos.x + deltas[direction],
			4.0f + pos.y,
			pos.z };

	u32 props = collMgr.getTileBehaviour2At(checkWhere.x, checkWhere.y, currentLayerID);

	if (((props >> 16) & 0xFF) == 8)
		return false;

	float someFloat = 0.0f;
	if (collMgr.sub_800757B0(&checkWhere, &someFloat, currentLayerID, 1, -1)) {
		if (someFloat < checkWhere.y && someFloat >(pos.y - 5.0f))
			return true;
	}

	return false;
}

///////////////////////
// Ice Stuff
///////////////////////
void daTakobo::_vf148() {
	dEn_c::_vf148();
	doStateChange(&StateID_DieFall);
}

void daTakobo::_vf14C() {
	dEn_c::_vf14C();
	doStateChange(&StateID_DieFall);
}

//this does weird shit, but it creates the iceblock around it
bool daTakobo::CreateIceActors() {
	struct DoSomethingCool my_struct = { 0, this->pos, {1.5, 2.0, 1.8}, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
	this->frzMgr.Create_ICEACTORs((void*)&my_struct, 1);
	__destroy_arr((void*)&my_struct, sub_80024C20, 0x3C, 1);
	this->chrAnimation.setUpdateRate(0.0);
	return true;
}

///////////////////////
// Sound-effects
///////////////////////
void daTakobo::playOnStompSound() {
	PlaySound(this, SE_EMY_HANACHAN_STOMP);
}

void daTakobo::playDeathSound() {
	PlaySound(this, SE_EMY_DOWN);
}

void daTakobo::playShootSound() {
	PlaySound(this, SE_VOC_YS_ATTACK);
}

///////////////////////
// Other functions
///////////////////////
int daTakobo::nearestPlayer() {
	int validPlayers[4];
	int playerCount = 0; 
	int smallestSquareDist = 200000.0;
	int nearestPlayer = -1;
	for (int i = 0; i < 4; i++) {
		if (Player_Active[i] != 0 && Player_Lives[Player_ID[i]] > 0) {
			validPlayers[playerCount] = i;
			playerCount++;
		}
	}
	for (int i = 0; i < playerCount; i++) {
		dStageActor_c* currentOne = GetSpecificPlayerActor(validPlayers[i]);
		f32 dx = currentOne->pos.x - this->pos.x;
		f32 dy = currentOne->pos.y - this->pos.y;
		f32 squareDist = dx * dx + dy * dy;
		if (squareDist <= smallestSquareDist) {
			smallestSquareDist = squareDist;
			nearestPlayer = validPlayers[i];
		}
	}
	return nearestPlayer;
}

void daTakobo::attackPlayerMaybe() {
	int rand;
	rand = GenerateRandomNumber(17);
	if (rand == 1 || gonnaIgnoreRandomTimer >= 80) {
		doStateChange(&StateID_PrepareAttack);
		gonnaIgnoreRandomTimer = 0;
	} else {
		gonnaIgnoreRandomTimer++;
	}
}

void daTakobo::move() {
	HandleXSpeed();
	HandleYSpeed();
	doSpriteMovement();
}

///////////////////////
// Models and Animating
///////////////////////
void daTakobo::setAnimation_and_updateRate(const char* animationName, int playsOnce, float unk2, float updateRate) {
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr(animationName);
	this->chrAnimation.bind(&this->bodyModel, anmChr, playsOnce);
	this->bodyModel.bindAnim(&this->chrAnimation, unk2);
	this->chrAnimation.setUpdateRate(updateRate);
}

void daTakobo::setupBodyModels() {
	this->allocator.link(-1, GameHeaps[0], 0, 0x20);

	this->resFile.data = getResource("octo", "g3d/t00.brres");
	nw4r::g3d::ResMdl resourceModel = this->resFile.GetResMdl("Takobo");
	this->bodyModel.setup(resourceModel, &this->allocator, 0x224, 1, 0);
	SetupTextures_Enemy(&bodyModel, 0);

	nw4r::g3d::ResAnmChr resourceAnimation = this->resFile.GetResAnmChr("Wait");
	this->chrAnimation.setup(resourceModel, resourceAnimation, &this->allocator, 0);

	allocator.unlink();
}

void daTakobo::updateModelMatrix() {
	if (this->frzMgr._mstate == 1) {
		matrix.translation(pos.x, pos.y + 4.0, pos.z);
	}
	else {
		if (acState.getCurrentState() == &StateID_IdleWalk || acState.getCurrentState() == &StateID_Turn) {
			matrix.translation(pos.x, pos.y + 4.5, pos.z);
		}
		else if (acState.getCurrentState() == &StateID_PrepareAttack) { 
			matrix.translation(pos.x, pos.y + this->prepAttackMatrixMover, pos.z);
		}
		else {
			matrix.translation(pos.x, pos.y, pos.z);
		}
	}
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

////////////////////////////
// Main functions
////////////////////////////

int daTakobo::onCreate() {
	this->canWalk = this->settings >> 28 & 0xF;
	this->cooldown = 0;
	this->scale = (Vec){ 0.18f, 0.18f, 0.18f };
	this->leftRot = 0xEA00;
	this->rightRot = 0x1800;
	this->rot.x = 0;
	this->rot.y = leftRot;
	this->rot.z = 0;
	this->isBouncing = 0;
	this->pos.z = 1000.0;
	this->direction = 1;
	this->prepAttackMatrixMover = 0.0f;

	setupBodyModels();
	setupActivePhysics();
	setupTileCollider();

	setAnimation_and_updateRate("Wait", 0, 0.0, 1.0f);
	doStateChange(&StateID_IdleWalk);

	this->onExecute();
	return true;
}

int daTakobo::onDelete() {
	return true;
}

int daTakobo::onExecute() {
	acState.execute();
	updateModelMatrix();
	bodyModel._vf1C();
	return true;
}

int daTakobo::onDraw() {
	bodyModel.scheduleForDrawing();
	return true;
}

////////////////////////////
// State functions
////////////////////////////

void daTakobo::beginState_IdleWalk() {
	if (this->canWalk) {
		this->max_speed.x = (direction) ? -0.6f : 0.6f;
		this->speed.x = (direction) ? -0.6f : 0.6f;
	}
	else {
		this->max_speed.x = 0.0f;
		this->speed.x = 0.0f;
	}
	
	this->max_speed.y = -4.0;
	this->speed.y = -4.0;
	this->y_speed_inc = -0.1875;
}

void daTakobo::executeState_IdleWalk() {
	chrAnimation.setUpdateRate(1.0f);
	this->cooldown += 1;
	if (canWalk != 0) {
		if (collMgr.isOnTopOfTile()) {
			if (!willWalkOntoSuitableGround()) {
				pos.x = direction ? pos.x + 1.5 : pos.x - 1.5;
				doStateChange(&StateID_Turn);
			}
		}

		move();
		if (direction == 0) {
			SmoothRotation(&this->rot.y, rightRot, 0x500);
		}
		else {
			SmoothRotation(&this->rot.y, leftRot, 0x500);
		}

		bool ret = calculateTileCollisions();
		if (ret) {
			doStateChange(&StateID_Turn);
		}

		if (cooldown > 120) {
			attackPlayerMaybe();
		}
	}
	else {
		int playerNum = this->nearestPlayer();
		if (playerNum != -1) {
			dStageActor_c* player = GetSpecificPlayerActor(playerNum);
			if (player->pos.x > this->pos.x) {
				this->direction = 0;
				SmoothRotation(&this->rot.y, rightRot, 0x500);
			}
			else {
				this->direction = 1;
				SmoothRotation(&this->rot.y, leftRot, 0x500);
			}

			if (cooldown > 100 && ((player->pos.x - this->pos.x) * (player->pos.x - this->pos.x) < 32000.0)) {
				attackPlayerMaybe();
			}
		}
		move();
		calculateTileCollisions();
	}
}

void daTakobo::endState_IdleWalk() {}

void daTakobo::beginState_Turn() {
	this->direction ^= 1;
}

void daTakobo::executeState_Turn() {
	chrAnimation.setUpdateRate(1.0f);

	u16 amt = (this->direction == 0) ? rightRot : leftRot;
	int done = SmoothRotation(&this->rot.y, amt, 0x500);

	if (done) {
		doStateChange(&StateID_IdleWalk);
	}
}

void daTakobo::endState_Turn() {}

void daTakobo::beginState_PrepareAttack() {
	this->max_speed.x = 0.0;
	this->speed.x = 0.0;
	this->x_speed_inc = 0.0;
	this->cooldown = 0;
	setAnimation_and_updateRate("AttackSign", 1, 0.0f, 1.0f);
}

void daTakobo::executeState_PrepareAttack() {
	this->prepAttackMatrixMover += 2.0f / 59.0f;
	calculateTileCollisions();
	chrAnimation.setUpdateRate(1.0f);
	if (this->chrAnimation.isAnimationDone()) {
		doStateChange(&StateID_Attack);
	}
}

void daTakobo::endState_PrepareAttack() {
	this->prepAttackMatrixMover = 0.0f;
}

void daTakobo::beginState_Attack() {
	setAnimation_and_updateRate("Attack", 1, 0.0f, 1.0f);
	
	playShootSound();
	f32 newX = (direction == 1) ? this->pos.x - 6.0 : this->pos.x + 6.0;
	Vec newPos = (Vec){ newX, this->pos.y + 12.0, 2000.0 };
	int newSettings = (direction == 1) ? 1 : 0;
	dStageActor_c* spawned = CreateActor(ProfileId::OctoBullet, newSettings, newPos, 0, 0);
	effect.spawn("Wm_en_landsmoke_s", 0, &newPos, &(S16Vec){0, 0, 0}, &(Vec){1.0, 1.0, 1.0});
}

void daTakobo::executeState_Attack() {
	calculateTileCollisions();
	chrAnimation.setUpdateRate(1.0f);

	if (this->chrAnimation.isAnimationDone()) {
		setAnimation_and_updateRate("Wait", 0, 0.0f, 1.0f);
		doStateChange(&StateID_IdleWalk);
	}
}

void daTakobo::endState_Attack() {}

void daTakobo::beginState_Dizzy() {
	this->max_speed.x = 0.0;
	this->speed.x = 0.0;
	this->x_speed_inc = 0.0;
	this->cooldown = 0;
	setAnimation_and_updateRate("Reaction", 1, 0.0f, 1.0f);
}

void daTakobo::executeState_Dizzy() {
	calculateTileCollisions();
	chrAnimation.setUpdateRate(1.0f);
	effect.spawn("Wm_en_spindamage", 0, &(Vec){this->pos.x, this->pos.y + 24.0, 0}, & (S16Vec){0, 0, 0}, & (Vec){1.0, 1.0, 1.0});
	if (this->chrAnimation.isAnimationDone()) {
		setAnimation_and_updateRate("Wait", 0, 0.0f, 1.0f);
		doStateChange(&StateID_IdleWalk);
	}
}

void daTakobo::endState_Dizzy() {}

void daTakobo::beginState_DiePress() {
	this->removeMyActivePhysics();
	this->setAnimation_and_updateRate("PressDown", 1, 0.0, 1.0f);
}

void daTakobo::executeState_DiePress() {
	if (chrAnimation.isAnimationDone())
	{
		setAnimation_and_updateRate("PressDown", 1, 0.0, 0.0f);
		this->Delete(1);
	}
}

void daTakobo::endState_DiePress() {}