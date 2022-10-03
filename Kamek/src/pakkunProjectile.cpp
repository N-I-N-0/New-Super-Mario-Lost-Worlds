#include <common.h>
#include <game.h>
#include <g3dhax.h>
#include <sfx.h>
#include <stage.h>


const char* PParcNameList [] = {
	"pakkun",
	"customball",
	NULL	
};

const char* PRarcNameList [] = {
	"customball",
	NULL	
};

class dPakkunProjectile_c : public dEn_c {
	public:
		int onCreate();
		int onDelete();
		int onExecute();
		int onDraw();

		mHeapAllocator_c allocator;
		nw4r::g3d::ResFile resFile;
		m3d::mdl_c bodyModel;
		m3d::anmChr_c animationChr;

		
		int projectile;
		float Baseline;
		bool deleteForever;

		static dActor_c *build();

		void bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate);
		void setupBodyModel();
		void updateModelMatrices();

		void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther);
		void yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther);

		bool collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther);
		bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther);
		bool collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther);
		bool collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther);
		bool collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther);
		bool collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther);
		bool collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther);
		bool collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther);

		int getActorID(int setting);
		char* getModelName(int setting);
		float getOffset(int setting);
		bool doBounce(int setting);
		Vec2 collisionDistToEdge(int setting);
		void deathParticle();

		USING_STATES(dPakkunProjectile_c);
		DECLARE_STATE(Wait);
};


dActor_c *dPakkunProjectile_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dPakkunProjectile_c));
	dPakkunProjectile_c *c = new(buffer) dPakkunProjectile_c;

	return c;
}


Profile PakkunProjectileProfile(&dPakkunProjectile_c::build, ProfileId::PakkunProjectile, NULL, ProfileId::PakkunProjectile, ProfileId::PakkunProjectile, "Projectiles", PRarcNameList);



CREATE_STATE(dPakkunProjectile_c, Wait);

extern "C" void dAcPy_vf3F4(void* mario, void* other, int t);
extern "C" bool BigHanaFireball(dEn_c* t, ActivePhysics *apThis, ActivePhysics *apOther);
extern "C" void *dAcPy_c__ChangePowerupWithAnimation(void * Player, int powerup);
extern "C" int CheckExistingPowerup(void * Player);
extern "C" void *EN_LandbarrelPlayerCollision(dEn_c* t, ActivePhysics *apThis, ActivePhysics *apOther);
extern "C" char usedForDeterminingStatePress_or_playerCollision(dEn_c* t, ActivePhysics *apThis, ActivePhysics *apOther, int unk1);

void dPakkunProjectile_c::playerCollision(ActivePhysics *apThis, ActivePhysics *apOther) { 
	if (doBounce(this->projectile)) {
		char hitType;
		hitType = usedForDeterminingStatePress_or_playerCollision(this, apThis, apOther, 0);

		if (hitType == 1 || hitType == 2|| hitType == 3) {
			PlaySound(this, SE_EMY_DOWN);
			deathParticle();
			this->Delete(1);
		}
		else this->_vf220(apOther->owner);
	}
	else this->_vf220(apOther->owner);
}
void dPakkunProjectile_c::yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther) { this->playerCollision(apThis, apOther); }
bool dPakkunProjectile_c::collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther) {
	return true;
}
bool dPakkunProjectile_c::collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther) {
	if(this->settings == 2) {
		VEC2 shit;
		shit.x = this->speed.x;
		shit.y = this->speed.y;
		dEn_c::killWithSpecifiedState(apOther->owner, &shit, &dEn_c::StateID_DieFumi, 0);
	}
	else {
		this->_vf220(apOther->owner);
	}
	return true;
}
bool dPakkunProjectile_c::collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther) {
	if(this->settings == 2) {
		VEC2 shit;
		shit.x = this->speed.x;
		shit.y = this->speed.y;
		dEn_c::killWithSpecifiedState(apOther->owner, &shit, &dEn_c::StateID_DieFumi, 0);
	}
	else {
		this->_vf220(apOther->owner);
	}
	return true;
}
bool dPakkunProjectile_c::collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther) {
	return true;
}
bool dPakkunProjectile_c::collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther) {
	this->Delete(1);
	return true;
}

bool dPakkunProjectile_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther) {
	return false;
}
bool dPakkunProjectile_c::collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther) {
	return true;
}
bool dPakkunProjectile_c::collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther) {
	this->_vf220(apOther->owner);
	return true;
}




void dPakkunProjectile_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate) {
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr(name);
	this->animationChr.bind(&this->bodyModel, anmChr, unk);
	this->bodyModel.bindAnim(&this->animationChr, unk2);
	this->animationChr.setUpdateRate(rate);
}



int dPakkunProjectile_c::getActorID(int setting) {
	switch(setting) {
		case 0:
			return 106; // Fire Ball
		case 1:
			return 118; // Ice Ball
	}
	return 0;
}


char* dPakkunProjectile_c::getModelName(int setting) {
	switch(setting) {
		case 2:
			return "poison_ball";
		case 3:
			return "plant_ball";
		case 4:
			return "cannon_ball";
	}
	return "INT";
}


float dPakkunProjectile_c::getOffset(int setting) {
	switch(setting) {
		case 1:
			return 16.0;
	}
	return 0.0;
}

bool dPakkunProjectile_c::doBounce(int setting) {
	switch(setting) {
		case 4:
			return true;
	}
	return false;
}

Vec2 dPakkunProjectile_c::collisionDistToEdge(int setting) {
	switch(setting) {
		case 2:
			return (Vec2){4.0, 4.0};
		case 3:
			return (Vec2){8.0, 8.0};
		case 4:
			return (Vec2){8.0, 8.0};
	}
	return (Vec2){8.0, 8.0};
}


void dPakkunProjectile_c::deathParticle() {
	S16Vec nullRot = {0,0,0};
	Vec2 d = collisionDistToEdge(this->projectile);
	float size = max(d.x, d.y) / 16.0;
	Vec oneVec = {size, size, size};
	SpawnEffect("Wm_mr_hardhit", 0, &pos, &nullRot, &oneVec);
}


void dPakkunProjectile_c::setupBodyModel() {
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	this->resFile.data = getResource("customball", "g3d/customball.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl(getModelName(this->projectile));
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Enemy(&bodyModel, 0);

	bool ret;
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr("customball");
	ret = this->animationChr.setup(mdl, anmChr, &this->allocator, 0);

	allocator.unlink();
}


int dPakkunProjectile_c::onCreate() {
	float speed = 1.0f;

	this->projectile = (this->settings >> 24 & 0xF);

	int players = GetActivePlayerCount();
	Vec target;
	float dist;

	for (int i = 0; i < players; i++) {
		dAcPy_c *player = (dAcPy_c*)GetSpecificPlayerActor(i);
		if (i == 0) {
			target = player->pos;
			dist = sqrtf((this->pos.x - player->pos.x) * (this->pos.x - player->pos.x) + (this->pos.y - player->pos.y) * (this->pos.y - player->pos.y));
		}
		else {
			float distance = sqrtf((player->pos.x - this->pos.x) * (player->pos.x - this->pos.x) + (player->pos.y - this->pos.y) * (player->pos.y - this->pos.y));
			if (distance < dist) {
				target = player->pos;
				dist = distance;
			}
		}
	}

	Vec speed_dir = (Vec){target.x - this->pos.x, target.y - this->pos.y, 0.0f};
	float magnitude = sqrtf(speed_dir.x * speed_dir.x + speed_dir.y * speed_dir.y);

	speed_dir.x /= magnitude;
	speed_dir.y /= magnitude;

	speed_dir.x *= speed;
	speed_dir.y *= speed;

	float offset = getOffset(this->projectile) * (this->pos.x < target.x ? 1 : -1);

	char* s = getModelName(this->projectile);

	if (s != "INT") {
		setupBodyModel();
		this->deleteForever = false;
	}
	else {
		dStageActor_c *spawner = CreateActor(getActorID(this->projectile), 0, this->pos, 0, 0);
		spawner->speed.x = speed_dir.x;
		spawner->speed.y = speed_dir.y;
		spawner->pos.x += offset;
		spawner->x_speed_inc = 0.0f;
		spawner->y_speed_inc = 0.0f;

		this->Delete(1);
		this->deleteForever = true;
	}

	if (this->deleteForever) return false;


	this->scale = (Vec){1.0, 1.0, 1.0};
	this->rot.x = 0; // X is vertical axis
	this->rot.y = 0xD800; // Y is horizontal axis
	this->rot.z = 0; // Z is ... an axis >.>
	this->direction = 1; // Heading left.

	this->speed.x = speed_dir.x;
	this->speed.y = speed_dir.y;
	this->x_speed_inc = 0.0f;
	this->y_speed_inc = 0.0f;
	this->Baseline = this->pos.y;

	this->pos.x += offset;

	ActivePhysics::Info HitMeBaby;
	
	HitMeBaby.xDistToCenter = 0.0;
	HitMeBaby.yDistToCenter = 0.0;

	Vec2 collSize = collisionDistToEdge(this->projectile);

	HitMeBaby.xDistToEdge = collSize.x;
	HitMeBaby.yDistToEdge = collSize.y;

	HitMeBaby.category1 = 0x3;
	HitMeBaby.category2 = 0x0;
	HitMeBaby.bitfield1 = 0x4F;
	HitMeBaby.bitfield2 = 0x8028E;
	HitMeBaby.unkShort1C = 0;
	HitMeBaby.callback = &dEn_c::collisionCallback;

	this->aPhysics.initWithStruct(this, &HitMeBaby);
	this->aPhysics.addToList();

	u32 flags = SENSOR_BREAK_BRICK | SENSOR_BREAK_BLOCK | SENSOR_80000000;
	static const lineSensor_s below(flags, 12<<12, 4<<12, 0<<12);
	static const pointSensor_s above(flags, 0<<12, 12<<12);
	static const lineSensor_s adjacent(flags, 6<<12, 9<<12, 6<<12);
	collMgr.init(this, &below, &above, &adjacent);


	bindAnimChr_and_setUpdateRate("customball", 1, 0.0, 1.0);

	doStateChange(&StateID_Wait);

	this->onExecute();
	return true;
}

int dPakkunProjectile_c::onDelete() {
	return true;
}

int dPakkunProjectile_c::onExecute() {
	acState.execute();
	updateModelMatrices();

	HandleXSpeed();
	HandleYSpeed();
	doSpriteMovement();

	collMgr.calculateBelowCollisionWithSmokeEffect();
	collMgr.calculateAboveCollision(0);
	collMgr.calculateAdjacentCollision();
	if (collMgr.outputMaybe) {
		deathParticle();
		this->Delete(1);
	}

	if(this->animationChr.isAnimationDone()) {
		this->animationChr.setCurrentFrame(0.0);
	}

	return true;
}


int dPakkunProjectile_c::onDraw() {
	bodyModel.scheduleForDrawing();
	bodyModel._vf1C();
	return true;
}


void dPakkunProjectile_c::updateModelMatrices() {
	// This won't work with wrap because I'm lazy.
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}


// Wait State

void dPakkunProjectile_c::beginState_Wait() {
	
}

void dPakkunProjectile_c::executeState_Wait() {
	if(this->animationChr.isAnimationDone()) {
		this->animationChr.setCurrentFrame(0.0);
	}
}
void dPakkunProjectile_c::endState_Wait() {

}