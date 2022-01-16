#include <common.h>
#include <game.h>
#include <g3dhax.h>
#include <sfx.h>
#include "boomeranghax.h"
#include "fileload.h"

int doWait = 0;
bool imDoneDoingVisibility;
u8 cloudSpawned[4];
int amIinCloud;
bool istherelightning = false;
int justspawnedit = 0;
bool isTargetSpawned = false;
int globalCoins;
int globalIceShoot;
int globalPropeller;

extern "C" float pow(float num, float power);
extern "C" int CheckExistingPowerup(void * Player);

int getNybbleValue(u32 settings, int fromNybble, int toNybble) {	 //Made that function because i wanted to, even if it's kinda useless. So shut up Meatball/CLF78
	int numberOfNybble = (toNybble  - fromNybble) + 1;               //gets how many nybbles are used for the process (example: nybbles 4-6 -> there's nybbles 4, 5 and 6 used -> numberOfNybble = 3) 
	int valueToUse = 48 - (4 * toNybble);                            //gets the value to use with the bitshift at the end 
	int fShit = pow(16, numberOfNybble) - 1;                         //gets the value to use with the "&" operator at the end 
	return ((settings >> valueToUse) & fShit);                       //uses everything to make the nybble value 
}


class daBoomerangHax_c : public dEn_c {
public:
	int onCreate();
	int onDelete();
	int onExecute();
	int onDraw();

	/********/
	/*Global*/
	/********/
	mHeapAllocator_c allocator;
	nw4r::g3d::ResFile resFile;

	m3d::mdl_c bodyModel;

	int alreadyOnTop;

	float XSpeed;
	bool canI;
	u32 cmgr_returnValue;
	bool isOnTopOfTile;
	int timer;
	int variation;

	/***********/
	/*Boomerang*/
	/***********/
	void playBoomerangSound();

	/******/
	/*Frog*/
	/******/
	m3d::anmChr_c animationChr;
	m3d::mdl_c coinModel;
	m3d::anmChr_c animationCoinChr;
	mMtx coinmatrix;
	VEC3 coinscale;
	S16Vec coinrot;
	S16Vec spikerot;
	bool doIhaveCoin;
	bool isCoinSpawned;

	/*******/
	/*Cloud*/
	/*******/
	StandOnTopCollider sotCollider;

	const char *brresName;
	daPlBase_c *player;
	pointSensor_s below;
	void dieFall_Begin();
	void dieFall_Execute();
	void dieFall_End();

	static daBoomerangHax_c *build();

	void updateModelMatrices();
	bool calculateTileCollisions();
	void bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate, bool isCoin);

	void spriteCollision(ActivePhysics *apThis, ActivePhysics *apOther);
	void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther);
	void yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther);

	bool collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther);

	USING_STATES(daBoomerangHax_c);
	DECLARE_STATE(Boomerang_Thrown);
	DECLARE_STATE(Boomerang_GotBack);
	DECLARE_STATE(Spike_Walk);
	DECLARE_STATE(Spike_Die);
	DECLARE_STATE(Frog_Thrown);
	DECLARE_STATE(Cloud_Thrown);
};

daBoomerangHax_c *daBoomerangHax_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daBoomerangHax_c));
	return new(buffer) daBoomerangHax_c;
}

void daBoomerangHax_c::dieFall_Begin() {
}
void daBoomerangHax_c::dieFall_Execute() {
}
void daBoomerangHax_c::dieFall_End() {
}

///////////////////////
// States
///////////////////////


	CREATE_STATE(daBoomerangHax_c, Boomerang_Thrown);
	CREATE_STATE(daBoomerangHax_c, Boomerang_GotBack);
	CREATE_STATE(daBoomerangHax_c, Spike_Walk);
	CREATE_STATE(daBoomerangHax_c, Spike_Die);
	CREATE_STATE(daBoomerangHax_c, Frog_Thrown);
	CREATE_STATE(daBoomerangHax_c, Cloud_Thrown);


////////////////////////
// Collision Functions
////////////////////////


	void daBoomerangHax_c::spriteCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
		if(this->variation == 0) {
			u16 name = ((dEn_c*)apOther->owner)->name;
	
			if (name == EN_COIN || name == EN_EATCOIN || name == AC_BLOCK_COIN || name == EN_COIN_JUGEM || name == EN_COIN_ANGLE
				|| name == EN_COIN_JUMP || name == EN_COIN_FLOOR || name == EN_COIN_VOLT || name == EN_COIN_WIND
				|| name == EN_BLUE_COIN || name == EN_COIN_WATER || name == EN_REDCOIN || name == EN_GREENCOIN
				|| name == EN_JUMPDAI || name == EN_ITEM || name == EN_STAR_COIN || name == AC_WATER_BUBBLE_CANNON || name == AC_BLOCK_GROUP) //this list will probably be updated in the future if I find other sprites that unfortunatly don't have to die
				{ return; }
	
			dAcPy_c *player = dAcPy_c::findByID((this->settings >> 4) & 0xF);
			this->addScoreWhenHit(player);
	
			if(name == EN_KANITAMA) {
				((dEn_c*)apOther->owner)->collisionCat7_GroundPound(apThis, apOther);
				return;
			}
	
			dEn_c::spriteCollision(apThis, apOther);
			((dEn_c*)apOther->owner)->collisionCat13_Hammer(apThis, apOther);
		}
		if(this->variation == 1) {
			u16 name = ((dEn_c*)apOther->owner)->name;                                                                           //get the name of the actor the spikeball collided with

			if (name == EN_COIN || name == EN_EATCOIN || name == AC_BLOCK_COIN || name == EN_COIN_JUGEM || name == EN_COIN_ANGLE //list of actors ignored by the spikeball
				|| name == EN_COIN_JUMP || name == EN_COIN_FLOOR || name == EN_COIN_VOLT || name == EN_COIN_WIND
				|| name == EN_BLUE_COIN || name == EN_COIN_WATER || name == EN_REDCOIN || name == EN_GREENCOIN
				|| name == EN_JUMPDAI || name == EN_ITEM || name == EN_STAR_COIN)
				{ return; }


			dEn_c::spriteCollision(apThis, apOther);                                                                             //do as if it hit a sprite
			((dEn_c*)apOther->owner)->collisionCat13_Hammer(apThis, apOther);                                                    //do as if it was touched by a hammer
		}
		if(this->variation == 3) {
			if(!this->isCoinSpawned) {
				u16 name = ((dEn_c*)apOther->owner)->name;
			
				if (   name == EN_COIN			|| name == EN_EATCOIN		|| name == AC_BLOCK_COIN	|| name == EN_COIN_JUGEM		|| name == EN_COIN_ANGLE
					|| name == EN_COIN_JUMP		|| name == EN_COIN_FLOOR	|| name == EN_COIN_VOLT		|| name == EN_COIN_WIND			|| name == EN_UNIRA
					|| name == EN_BLUE_COIN		|| name == EN_COIN_WATER	|| name == EN_REDCOIN		|| name == EN_GREENCOIN			|| name == AC_BLOCK_GROUP
					|| name == EN_JUMPDAI		|| name == EN_ITEM			|| name == EN_STAR_COIN		|| name == AC_WATER_BUBBLE_CANNON
					|| name == EN_BOSS_LARRY	|| name == EN_BOSS_WENDY	|| name == EN_BOSS_IGGY		|| name == EN_BOSS_LEMMY
					|| name == EN_BOSS_MORTON	|| name == EN_BOSS_ROY		|| name == EN_BOSS_LUDWIG	|| name == EN_BOSS_KOOPA
					|| name == EN_IGAPUKU		|| name == EN_BAKUBAKU		|| name == EN_IBARAMUSHI	|| name == EN_BIG_TOGETEKKYU
					|| name == WM_PUKU			|| name == EN_RAIL_POLY_ICE) //this list will probably be updated in the future if I find other sprites that unfortunatly don't have to die
					{ return; }
			
				this->doIhaveCoin = true;
				((dEn_c*)apOther->owner)->Delete(1);
			}
		}
		if(this->variation == 4) {
			u16 ownername = ((dEn_c*)apOther->owner)->name;
			u32 ownersets = ((dEn_c*)apOther->owner)->settings;
			u8 ownerdir = ((dEn_c*)apOther->owner)->direction;
			float ownerspeed = getNybbleValue(ownersets, 9, 10) * 0.1f;
			// OSReport("ownername = %d\n", ownername);
			// OSReport("ownersets = %d\n", ownersets);
			// OSReport("ownerdir = %d\n", ownerdir);
			// OSReport("ownerspeed1 = %d\n", ownerspeed1);
			// OSReport("ownerspeed = %f\n", ownerspeed);
			if(ownername == BIGHANA_MGR) { //SMG2 Object
				if(((ownersets >> 28) & 0xF) == 0) { //Windmill
					if(ownerdir == 0) { //Faces right
						this->pos.x += ownerspeed;
					}
					if(ownerdir == 1) { //Faces left
						this->pos.x -= ownerspeed;
					}
				}
			}

			//Added by LiQ for CloudPropeller
			else if (ownername == CloudPropeller) { return; }

			else
			{
				this->Delete(1);
			}
		}
	}

	void daBoomerangHax_c::playerCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
		if(this->variation == 0) {
			if(apOther->owner == dAcPy_c::findByID((this->settings >> 4) & 0xF)) {
				if(canI) {
					doStateChange(&StateID_Boomerang_GotBack);
				}
			}
		}
		if(this->variation == 3) {
			if(this->doIhaveCoin) {
				int pid = getNybbleValue(apOther->owner->which_player, 6, 6);
				int coinsettings = 0 | (0x2 << 0) | (2 << 18) | (4 << 9) | (2 << 10) | ((pid + 8) << 16);
				VEC3 newPos = {this->pos.x, this->pos.y - 16, this->pos.z};
				dStageActor_c *coin = dStageActor_c::create(EN_ITEM, coinsettings, &newPos, 0, 0);
				PlaySound(this, SE_OBJ_GET_COIN);
				PlaySound(this, SE_OBJ_CMN_BALLOON_BREAK);
				Vec efPos = {this->pos.x, this->pos.y, this->pos.z};
				S16Vec nullRot = {0,0,0};
				Vec oneVec = {1.0f, 1.0f, 1.0f};
				SpawnEffect("Wm_mr_balloonburst", 0, &efPos, &nullRot, &oneVec);
				this->Delete(1);
			}
		}
		if (this->variation == 4)
		{
			daPlBase_c* player = (daPlBase_c*)apOther->owner;

			if (CheckExistingPowerup(player) != 12 && CheckExistingPowerup(player) != 3)
			{
				this->Delete(1);
			}
		}
	}

	void daBoomerangHax_c::yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
		if(this->variation == 0) {
			if(apOther->owner == dAcPy_c::findByID((this->settings >> 4) & 0xF)) {
				if(canI) {
					doStateChange(&StateID_Boomerang_GotBack);
				}
			}
		}
		if(this->variation == 3) {
			this->playerCollision(apThis, apOther);
		}
		if (this->variation == 4)
		{
			daPlBase_c* player = (daPlBase_c*)apOther->owner;

			if (CheckExistingPowerup(player) != 12 && CheckExistingPowerup(player) != 3)
			{
				this->Delete(1);
			}
		}
	}

	bool daBoomerangHax_c::collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther) {
		if(this->variation == 0) {
			return false;
		}
		if(this->variation == 1) {
			doStateChange(&StateID_Spike_Die);
			return true;
		}
		if(this->variation == 3 || this->variation == 4) {
			return false;
		}
		return true;
	}

	bool daBoomerangHax_c::collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther) {
		if(this->variation == 0) {
			if(apOther->owner == dAcPy_c::findByID((this->settings >> 4) & 0xF)) {
				if(canI) {
					doStateChange(&StateID_Boomerang_GotBack);
				}
			}
			return false;
		}
		if(this->variation == 1 || this->variation == 3) {
			return false;
		}
		if(this->variation == 4) {
			this->Delete(1);
			return false;
		}
		return true;
	}

	bool daBoomerangHax_c::collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther) {
		if(this->variation == 0) {
			if(apOther->owner == dAcPy_c::findByID((this->settings >> 4) & 0xF)) {
				if(canI) {
					doStateChange(&StateID_Boomerang_GotBack);
				}
			}
			return false;
		}
		if(this->variation == 1 || this->variation == 3) {
			return false;
		}
		if(this->variation == 4) {
			this->Delete(1);
			return false;
		}
		return true;
	}

	bool daBoomerangHax_c::collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther) {
		if(this->variation == 0) {
			return false;
		}
		if(this->variation == 1) {
			doStateChange(&StateID_Spike_Die);
			return true;
		}
		if(this->variation == 3 || this->variation == 4) {
			return false;
		}
		return true;
	}

	bool daBoomerangHax_c::collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther){
		if(this->variation == 0) {
			if(apOther->owner == dAcPy_c::findByID((this->settings >> 4) & 0xF)) {
				if(canI) {
					doStateChange(&StateID_Boomerang_GotBack);
				}
			}
			return false;
		}
		if(this->variation == 1 || this->variation == 3 || this->variation == 4) {
			return false;
		}
		return true;
	}

	bool daBoomerangHax_c::collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther) {
		return false;
	}

	bool daBoomerangHax_c::collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther){
		if(this->variation == 0) {
			if(apOther->owner == dAcPy_c::findByID((this->settings >> 4) & 0xF)) {
				if(canI) {
					doStateChange(&StateID_Boomerang_GotBack);
				}
			}
			return false;
		}
		if(this->variation == 1 || this->variation == 3 || this->variation == 4) {
			return false;
		}
		return true;
	}

	bool daBoomerangHax_c::collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther){
		return false;
	}

	bool daBoomerangHax_c::collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther) {
		return false;
	}
	
	bool daBoomerangHax_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther) { 
		return false;
	}

void daBoomerangHax_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate, bool isCoin) {
	if(!isCoin) {
		nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr(name);
		this->animationChr.bind(&this->bodyModel, anmChr, unk);
		this->bodyModel.bindAnim(&this->animationChr, unk2);
		this->animationChr.setUpdateRate(rate);
	}
	else {
		nw4r::g3d::ResAnmChr anmCoinChr = this->resFile.GetResAnmChr(name);
		this->animationCoinChr.bind(&this->coinModel, anmCoinChr, unk);
		this->coinModel.bindAnim(&this->animationCoinChr, unk2);
		this->animationCoinChr.setUpdateRate(rate);
	}
}

int daBoomerangHax_c::onCreate() {
	this->alreadyOnTop = 0;
	this->canI = false;
	this->variation = (this->settings >> 12) & 0xF;
	OSReport("daBoomerangHax_c::onCreate()!!!\n");
	if(this->variation == 0) { //Boomerang
		this->deleteForever = true;

		// Model creation	
		allocator.link(-1, GameHeaps[0], 0, 0x20);

		this->resFile.data = getResource("boomerangM", "g3d/boomerang.brres");
		nw4r::g3d::ResMdl mdl;
		int pid = (this->settings >> 4) & 0xF;
		if(pid == 0) {
			mdl = this->resFile.GetResMdl("bros_boomerang_m");
		}
		if(pid == 1) {
			mdl = this->resFile.GetResMdl("bros_boomerang_l");
		}
		if(pid == 2) {
			mdl = this->resFile.GetResMdl("bros_boomerang_ky");
		}
		if(pid == 3) {
			mdl = this->resFile.GetResMdl("bros_boomerang_kb");
		}
		bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
		SetupTextures_Enemy(&bodyModel, 0);

		allocator.unlink();

		// Stuff I do understand
		this->scale = (Vec){0.85, 0.85, 0.85};

		this->rot.x = 0;
		this->rot.y = 0;
		this->rot.z = 0;
		this->direction = (this->settings >> 8) & 0xF; // Heading where the player look.

		ActivePhysics::Info HitMeBaby;

		HitMeBaby.xDistToCenter = 0.0;
		HitMeBaby.yDistToCenter = 0.0;

		HitMeBaby.xDistToEdge = 6.375;
		HitMeBaby.yDistToEdge = 6.375;		

		HitMeBaby.category1 = 0x3;
		HitMeBaby.category2 = 0x0;
		HitMeBaby.bitfield1 = 0x6F;
		HitMeBaby.bitfield2 = 0xffbafffe;
		HitMeBaby.unkShort1C = 0;
		HitMeBaby.callback = &dEn_c::collisionCallback;

		this->aPhysics.initWithStruct(this, &HitMeBaby);
		this->aPhysics.addToList();


		this->pos.y += 16;
		this->pos.z = 4000;


		doStateChange(&StateID_Boomerang_Thrown); 

		this->onExecute();
		return true;
	}
	if(this->variation == 1) { //Spikeball
		this->deleteForever = true;                                         //FOREVAAAAAAAAH

		/****************/
		/*Model Creation*/
		/****************/

		allocator.link(-1, GameHeaps[0], 0, 0x20);                          //allocator linking

		this->resFile.data = getResource("spikeM", "g3d/iron_ball.brres");  //get the brres in the arc file
		nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("iron_ball");       //get the mdl0
		bodyModel.setup(mdl, &allocator, 0x224, 1, 0);                      //setup it
		SetupTextures_MapObj(&bodyModel, 0);                                //setup shaders
		OSReport("IRON BALL!!!");


		allocator.unlink();                                                 //allocator unlinking

		/****************/
		/*Values Setting*/
		/****************/

		this->scale = (Vec){0.7, 0.7, 0.7};                                 //scale setting

		this->rot.x = 0;                                                    //X rotation setting
		this->rot.y = 0;                                                    //Y rotation setting
		this->rot.z = 0;                                                    //Z rotation setting
		this->direction = (this->settings >> 8) & 0xF;                      //heading where the player look.
		
		this->speed.x = 0.0;                                                //X speed setting
		this->speed.y = 3.0;                                                //Y speed setting
		this->max_speed.x = 0.0;                                            //X maximum speed setting
		this->x_speed_inc = 0.0;                                            //X speed incrementation setting
		this->XSpeed = 3.0;                                                 //XSpeed value (used later) setting

		/********************/
		/*Collision Creating*/
		/********************/

		ActivePhysics::Info HitMeBaby;                                      //ActivePhysics::Info creating

		HitMeBaby.xDistToCenter = 0.0;                                      //no X distance to center duh
		HitMeBaby.yDistToCenter = 0.0;                                      //no Y distance to center duh

		HitMeBaby.xDistToEdge = 7.5;                                        //1 block wide
		HitMeBaby.yDistToEdge = 7.5;                                        //1 block high		

		HitMeBaby.category1 = 0x3;                                          //idk what is this, ask treeki
		HitMeBaby.category2 = 0x0;                                          //^
		HitMeBaby.bitfield1 = 0x6F;                                         //^
		HitMeBaby.bitfield2 = 0xffbafffe;                                   //^
		HitMeBaby.unkShort1C = 0;                                           //^
		HitMeBaby.callback = &dEn_c::collisionCallback;                     //collision callback

		this->aPhysics.initWithStruct(this, &HitMeBaby);                    //initating the collision
		this->aPhysics.addToList();                                         //add to the collision list

		/********************/
		/*Colliders Creating*/
		/********************/

		// These fucking rects do something for the tile rect
		spriteSomeRectX = 28.0f;
		spriteSomeRectY = 32.0f;
		_320 = 0.0f;
		_324 = 16.0f;

		static const lineSensor_s below(-5<<12, 5<<12, -9<<12);            //Below collider:    unk, unk, y pos 
		static const pointSensor_s above(0<<12, 12<<12);                   //Above collider:    unk, unk, unk
		static const lineSensor_s adjacent(3<<12, 6<<12, 6<<12);           //Adjacent collider: unk, unk, unk

		collMgr.init(this, &below, &above, &adjacent);                     //initating colliders
		collMgr.calculateBelowCollisionWithSmokeEffect();                  //dude, the description of this line IS in the line, so what are you reading ?

		cmgr_returnValue = collMgr.isOnTopOfTile();                        //get if the sprite is in top of a tile (GOD DAMMIT IT IS WROTE)

		if (collMgr.isOnTopOfTile())                                       //just... read !
			isOnTopOfTile = false;
		else
			isOnTopOfTile = true;

		this->pos.y += 16;                                                 //because the game


		doStateChange(&StateID_Spike_Walk);                                //switch to the walk state (even if it's not really walking but shut up)

		this->onExecute();
		return true;
	}
	if(this->variation == 3) { //Bubble
		this->deleteForever = true;

		// Model creation	
		allocator.link(-1, GameHeaps[0], 0, 0x20);

		this->resFile.data = getResource("frogM", "g3d/balloon.brres");
		nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("balloon");
		bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
		SetupTextures_Enemy(&bodyModel, 0);

		nw4r::g3d::ResMdl coinmdl = this->resFile.GetResMdl("obj_coin");
		coinModel.setup(coinmdl, &allocator, 0x224, 1, 0);
		SetupTextures_MapObj(&coinModel, 0);

		bool ret;
		nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr("float");
		ret = this->animationChr.setup(mdl, anmChr, &this->allocator, 0);

		bool rett;
		nw4r::g3d::ResAnmChr anmCoinChr = this->resFile.GetResAnmChr("no");
		rett = this->animationCoinChr.setup(coinmdl, anmCoinChr, &this->allocator, 0);

		allocator.unlink();

		// Stuff I do understand
		this->scale = (Vec){0.6, 0.6, 0.6};
		this->coinscale = (Vec){1.0, 1.0, 1.0};

		this->rot.x = 0;
		this->rot.y = 0;
		this->rot.z = 0;
		this->direction = (this->settings >> 8) & 0xF; // Heading where the player look.
		this->speed.x = (this->direction == 1) ? -5 : 5;
		this->x_speed_inc = 0.1;

		ActivePhysics::Info HitMeBaby;

		HitMeBaby.xDistToCenter = 0.0;
		HitMeBaby.yDistToCenter = 0.0;

		HitMeBaby.xDistToEdge = 7.5;
		HitMeBaby.yDistToEdge = 7.5;		

		HitMeBaby.category1 = 0x3;
		HitMeBaby.category2 = 0x0;
		HitMeBaby.bitfield1 = 0x4F;
		HitMeBaby.bitfield2 = 0x8028E;
		HitMeBaby.unkShort1C = 0;
		HitMeBaby.callback = &dEn_c::collisionCallback;

		this->aPhysics.initWithStruct(this, &HitMeBaby);
		this->aPhysics.addToList();


		// this->pos.y += 16;
		// this->pos.z = 9000;

		bindAnimChr_and_setUpdateRate("float", 1, 0.0, 1.0, false);
		bindAnimChr_and_setUpdateRate("no", 1, 0.0, 1.0, true);

		doStateChange(&StateID_Frog_Thrown); 

		this->onExecute();
		return true;
	}
	if(this->variation == 4) { //Cloud
		this->deleteForever = true;

		// Model creation	
		allocator.link(-1, GameHeaps[0], 0, 0x20);

		this->resFile.data = getResource("cloudM", "g3d/power_cloud.brres");
		nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("spincloud");
		bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
		SetupTextures_MapObj(&bodyModel, 0);

		allocator.unlink();

		// Stuff I do understand
		// this->scale = (Vec){1.2, 1.2, 1.2};
		this->scale = (Vec){0.0, 0.0, 0.0};

		this->rot.x = 0;
		this->rot.y = 0;
		this->rot.z = 0;
		this->direction = (this->settings >> 8) & 0xF; // Heading where the player look.


		float fWidth = 3 * 16.0f;
		sotCollider.init(this,
				/*xOffset=*/-23.0f, /*yOffset=*/7.0f,
				/*topYOffset=*/0,
				/*rightSize=*/fWidth, /*leftSize=*/0,
				/*rotation=*/0, /*_45=*/1
				);

		// What is this for. I dunno
		sotCollider._47 = 0xA;
		sotCollider.flags = 0x80180 | 0xC00;

		sotCollider.addToList();


		ActivePhysics::Info HitMeBaby;

		HitMeBaby.xDistToCenter = 0.0;
		HitMeBaby.yDistToCenter = 8.0;

		HitMeBaby.xDistToEdge = 24;
		HitMeBaby.yDistToEdge = 1;

		HitMeBaby.category1 = 0x3;
		HitMeBaby.category2 = 0x0;
		HitMeBaby.bitfield1 = 0x6F;
		HitMeBaby.bitfield2 = 0xffbafffe;
		HitMeBaby.unkShort1C = 0;
		HitMeBaby.callback = &dEn_c::collisionCallback;

		this->aPhysics.initWithStruct(this, &HitMeBaby);
		this->aPhysics.addToList();

		this->pos.y += 16;
		this->pos.z = 4000;


		doStateChange(&StateID_Cloud_Thrown); 

		this->onExecute();
		return true;
	}
	return true;
}

int daBoomerangHax_c::onDelete() {
	if(this->variation != 5) {
		this->Delete(1);
	}
	else {
		isTargetSpawned = false;
	}
	return true;
}

int daBoomerangHax_c::onExecute() {
	acState.execute();
	updateModelMatrices();

	bodyModel._vf1C();

	if(this->variation == 0) {
		this->playBoomerangSound();
		this->rot.z += (this->direction == 1) ? 2000 : -2000;
	}
	if(this->variation == 1) {
		this->spikerot.z += (this->direction == 1) ? 2000 : -2000; //Rotating it depending of its spawning direction
		//OSReport("Rotating spikeball: %d, %d / 0x%X\n", this->direction, this->rot.z, this->rot.z);
		PlaySound(this, SE_PLY_WALK_METAL);                  //Play SFX
	}
	if(this->variation == 3) {
		coinModel._vf1C();
		this->timer++;

		if(this->doIhaveCoin && !this->isCoinSpawned) {
			bindAnimChr_and_setUpdateRate("yes", 1, 0.0, 1.0, true);
			this->isCoinSpawned = true;
		}

		if(this->animationChr.isAnimationDone()) {
			this->animationChr.setCurrentFrame(0.0);
		}
		if(this->animationCoinChr.isAnimationDone()) {
			this->animationCoinChr.setCurrentFrame(0.0);
		}

		if(this->timer > 300) {
			PlaySound(this, SE_OBJ_CMN_BALLOON_BREAK);
			Vec efPos = {this->pos.x, this->pos.y, this->pos.z};
			S16Vec nullRot = {0,0,0};
			Vec oneVec = {1.0f, 1.0f, 1.0f};
			SpawnEffect("Wm_mr_balloonburst", 0, &efPos, &nullRot, &oneVec);
			this->Delete(1);
		}

		this->pos.y += 0.1;
		this->coinrot.y += 0x400;
	}
	if(this->variation == 4) {
		sotCollider.update();
	}

	return true;
}

int daBoomerangHax_c::onDraw() {
	bodyModel.scheduleForDrawing();
	if(this->variation == 3) {
		coinModel.scheduleForDrawing();
	}

	return true;
}

void daBoomerangHax_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y - ((this->variation != 3) ? 2.0 : 0.0), pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);
	if(this->variation == 3) {
		coinmatrix.translation(pos.x, pos.y, pos.z);
		coinmatrix.applyRotationYXZ(&coinrot.x, &coinrot.y, &coinrot.z);
	}
	if(this->variation == 1) {
		matrix.applyRotationYXZ(&spikerot.x, &spikerot.y, &spikerot.z);
	}

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
	if(this->variation == 3) {
		coinModel.setDrawMatrix(coinmatrix);
		coinModel.setScale(&coinscale);
		coinModel.calcWorld(false);
	}
}

bool daBoomerangHax_c::calculateTileCollisions() {

	/****************************/
	/***Tile collision detector**/
	/*Returns true if hit a wall*/
	/****************************/


	HandleXSpeed();                                      //consider the X speed
	HandleYSpeed();                                      //consider the Y speed
	doSpriteMovement();                                  //execute the speed movements

	cmgr_returnValue = collMgr.isOnTopOfTile();          //check if the sprite is on top of a tile
	collMgr.calculateBelowCollisionWithSmokeEffect();    //duh

	stuffRelatingToCollisions(0.1875f, 1.0f, 0.5f);      //literally stuff related to collisions

	float xDelta = pos.x - last_pos.x;                   //just read dude

	if (xDelta >= 0.0f) {                                //change of direction if needed
		direction = 0;
	}
	else {
		direction = 1;
	}

	if (collMgr.isOnTopOfTile()) {                       //if the sprite is on top of a tile
		if(alreadyOnTop == 0) {                          //if it just landed
			PlaySound(this, SE_OBJ_ROCK_LAND);           //play SFX
			alreadyOnTop = 1;                            //now it's already on the to^p
		}

		if (cmgr_returnValue == 0)                       //just read ?
			isOnTopOfTile = true;

		speed.y = 0.0f;                                  //no Y speed anymore cuz it's on the ground
		max_speed.x = (direction == 1) ? -1.5f : 1.5f;   //maximum X speed re-setting
		this->x_speed_inc = 0.0f;                        //no X speed incrementation
		this->speed.x = (direction == 1) ? -1.5f : 1.5f; //X speed re-setting
	}
	else {                                               //if the sprite in in mid-air
		alreadyOnTop = 0;                                //it's no loner on the top of a tile
	}

	collMgr.calculateAdjacentCollision(0);               //literally calculate the adjacent collision

	if (collMgr.outputMaybe & (0x15 << direction)) {     //if hit a wall
		if (collMgr.isOnTopOfTile()) {                   //if on the top of a tile
			isOnTopOfTile = true;
		}
		return true;                                     //returns true duh
	}
	return false;                                        //if didn't hit a wall
}

///////////////
// Boomerang_Thrown State
///////////////
	void daBoomerangHax_c::beginState_Boomerang_Thrown() {

	}
	void daBoomerangHax_c::executeState_Boomerang_Thrown() { 
		HandleXSpeed();
		doSpriteMovement();
		float speedx = (this->direction == 1) ? -2.7 : 2.7;
		this->timer++;
		if(this-> timer > 0 && this->timer <= 20) {
			this->max_speed.x = speedx;
			this->x_speed_inc = 0;
			this->speed.x = speedx;
			this->pos.y += 1.5;
		}
		if(this->timer > 20 && this->timer <= 55) {
			this->speed.x = speedx;
		}
		if(this-> timer > 55 && this->timer <= 70) {
			canI = true;
			this->max_speed.x = -speedx;
			this->x_speed_inc = 0.2;
			this->pos.y -= 2;
		}
		if(this->timer > 70) {
			this->x_speed_inc = 0;
			this->max_speed.x = -speedx;
			this->speed.x = -speedx;
		}
		if(this->timer == 600) {
			this->direction = (this->direction == 1) ? 0 : 1;
			doStateChange(&StateID_Boomerang_GotBack);
		}
	}
	void daBoomerangHax_c::endState_Boomerang_Thrown() { 

	}


///////////////
// Boomerang_GotBack State
///////////////
	void daBoomerangHax_c::beginState_Boomerang_GotBack() {
		this->x_speed_inc = 0;
	}
	void daBoomerangHax_c::executeState_Boomerang_GotBack() { 
		HandleXSpeed();
		doSpriteMovement();
		float speedx = (this->direction == 1) ? -2.7 : 2.7;
		this->max_speed.x = -speedx;
		this->speed.x = -speedx;
		this->scale.x -= 0.1;
		this->scale.y -= 0.1;
		this->scale.z -= 0.1;
		if(this->scale.x <= 0) {
			this->Delete(1);
		}
	}
	void daBoomerangHax_c::endState_Boomerang_GotBack() { 

	}


///////////////
// Spike_Walk State
///////////////
	void daBoomerangHax_c::beginState_Spike_Walk() {
		this->max_speed.x = (this->direction) ? -this->XSpeed : this->XSpeed; //set the maximum X speed depending of the spawning direction
		this->speed.x = (this->direction) ? -3.0 : 3.0;                       //set the current X speed depending of the spawning direction
		this->x_speed_inc = (this->direction) ? 0.6 : -0.6;                   //set the X speed increment depending of the spawning direction

		this->max_speed.y = -4;                                               //set the maximum Y speed
		this->speed.y = 6.0;                                                  //set the current Y speed
		this->y_speed_inc = -0.5;                                             //set the Y speed increment
	}
	void daBoomerangHax_c::executeState_Spike_Walk() {
		bool ret = calculateTileCollisions();                                 //calculate the tiles collision
		if (ret) {
			doStateChange(&StateID_Spike_Die);                                      //if hit a wall, die
		}
	}
	void daBoomerangHax_c::endState_Spike_Walk() { }


///////////////
// Spike_Die State
///////////////
	void daBoomerangHax_c::beginState_Spike_Die() {
		this->removeMyActivePhysics();           //Remove the collision
	}
	void daBoomerangHax_c::executeState_Spike_Die() { 
		PlaySound(this, SE_OBJ_WOOD_BOX_BREAK);  //Play SFX
		Vec2 nyeh = {this->pos.x, this->pos.y};  //Get the current position as a Vec2
		this->spawnHitEffectAtPosition(nyeh);    //Spawn Hit Effect at this position (duh)
		doWait = 0;                              //doWait is now 0
		this->Delete(this->deleteForever);       //DELETE FOREVAAAAAAAH
	}
	void daBoomerangHax_c::endState_Spike_Die() { }


///////////////
// Frog_Thrown State
///////////////
void daBoomerangHax_c::beginState_Frog_Thrown() {

}
void daBoomerangHax_c::executeState_Frog_Thrown() { 
	HandleXSpeed();
	doSpriteMovement();
	if(this->direction == 1) {
		if(this->speed.x > 0) {
			this->speed.x = 0;
			this->x_speed_inc = 0;
		}
	}
	if(this->direction == 0) {
		if(this->speed.x < 0) {
			this->speed.x = 0;
			this->x_speed_inc = 0;
		}
	}
}
void daBoomerangHax_c::endState_Frog_Thrown() { 

}


///////////////
// Cloud_Thrown State
///////////////
void daBoomerangHax_c::beginState_Cloud_Thrown() {
	this->timer = 0;
}
void daBoomerangHax_c::executeState_Cloud_Thrown() { 
	this->timer++;
	if(this->timer > 0 && this->timer < 13) {
		this->scale.x += 0.1;
		this->scale.y += 0.1;
		this->scale.z += 0.1;
		this->rot.y += 68264;
	}
	if(this->timer == 13) {
		this->scale.x = 1.2;
		this->scale.y = 1.2;
		this->scale.z = 1.2;
		this->rot.y = 0;
	}
	if(this->timer > 425) {
		this->scale.x -= 0.05;
		this->scale.y -= 0.05;
		this->scale.z -= 0.05;
	}
	if(this->timer == 438) {
		float fWidth = 1.5 * 16.0f;
		sotCollider.init(this,
				/*xOffset=*/-11.5f, /*yOffset=*/0.0f,
				/*topYOffset=*/0,
				/*rightSize=*/fWidth, /*leftSize=*/0,
				/*rotation=*/0, /*_45=*/1
				);

		// What is this for. I dunno
		sotCollider._47 = 0xA;
		sotCollider.flags = 0x80180 | 0xC00;

		sotCollider.addToList();
	}
	if(this->timer == 450) {
		this->Delete(1);
	}
}
void daBoomerangHax_c::endState_Cloud_Thrown() { 

}




/*****************************************************/
/**************Gabon Rock replacing shit**************/
/*Don't ask me how does it work, because i don't know*/
/*****************************************************/

int daGabonRock_c::getsettings() {									 //I know bleh bleh bleh that's not optimised, but lemme ask you something: Do I care ? The answer is no.
	int orig_val = this->onCreate_orig();
	if(getNybbleValue(this->settings, 12, 12) > 1) {
		int playerID = getNybbleValue(this->settings, 6, 6);
		dAcPy_c *player = dAcPy_c::findByID(playerID);
		int variation = getNybbleValue(this->settings, 11, 11);
		if(variation < 2) {
			if(variation == 1) { //Spike
				nw4r::snd::SoundHandle spikyHandle;
				PlaySoundWithFunctionB4(SoundRelatedClass, &spikyHandle, SE_EMY_GABON_ROCK_THROW, 1);
			}
			int settings = 0 | (playerID << 4) | (player->direction << 8) | (variation << 12);
			CreateActor(555, settings, player->pos, 0, 0);
			doWait = 60;
		}
		if(variation == 2) { //Wand

			doWait = 60;
		}
		if(variation == 3) { //Frog
			nw4r::snd::SoundHandle froggyHandle;
			PlaySoundWithFunctionB4(SoundRelatedClass, &froggyHandle, SE_EMY_KANIBO_THROW, 1);
			VEC3 actorpos = {player->pos.x + ((player->direction == 1) ? -10 : 10), player->pos.y + 16, player->pos.z};
			int bubbleSettings = 0 | (playerID << 4) | (player->direction << 8) | (variation << 12);
			CreateActor(555, bubbleSettings, actorpos, 0, 0);
			doWait = 30;
		}
		if(variation == 5) { //Gold

			doWait = 180;
		}
	}
	return orig_val;
}

void daEnBomhei_c::doThrowing() {
	this->isBurning = 180;
	this->speed.x = 3 * ((this->direction == 1) ? -1 : 1);
	this->speed.y = 3;
	this->amIthrown = 1;
	doStateChange(&daEnBomhei_c::StateID_Sleep);
}

void daEnBomhei_c::newPlayerCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	if(this->amIthrown != 1) {
		this->playerCollision_orig(apThis, apOther);
		OSReport("i'm not thrown, go ahead\n");
	}
	else {
		OSReport("i'm thrown, go away\n");
	}
}

void daEnBomhei_c::newSpriteCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	if(amIthrown == 1) {
		u16 name = ((dEn_c*)apOther->owner)->name;
		if (name == EN_COIN || name == EN_EATCOIN || name == AC_BLOCK_COIN || name == EN_COIN_JUGEM || name == EN_COIN_ANGLE
			|| name == EN_COIN_JUMP || name == EN_COIN_FLOOR || name == EN_COIN_VOLT || name == EN_COIN_WIND
			|| name == EN_BLUE_COIN || name == EN_COIN_WATER || name == EN_REDCOIN || name == EN_GREENCOIN
			|| name == EN_JUMPDAI || name == EN_ITEM || name == EN_STAR_COIN) { //this list will probably be updated in the future if I find other sprites that unfortunatly don't have to EXXXXPLOOOOODE
			this->spriteCollision_orig(apThis, apOther);
		}
		else {
			if(((daEnBomhei_c *)apOther->owner)->amIthrown == 1) {
				this->spriteCollision_orig(apThis, apOther);
			}
			else {
				this->isBurning = 0;
			}
		}
	}
	else {
		this->spriteCollision_orig(apThis, apOther);
	}
}


class placeholder {
public:
	void doCleanClouds(); //Okay so there's a very good reason of why i put that in a placeholder class. The thing is i need to use that function in poweruphax.S as an extern, but for some reason, _ZN13doCleanCloudsEv refuses to work, so i put it in that class so _ZN11placeholder13doCleanCloudsEv works !
	void increaseIceCount(); //Same as above.
	void increasePropellerCount(); //Same as above.
};

void placeholder::doCleanClouds() {
	amIinCloud = 0;
	imDoneDoingVisibility = false;
	cloudSpawned[0] = 0;
	cloudSpawned[1] = 0;
	cloudSpawned[2] = 0;
	cloudSpawned[3] = 0;
}

void placeholder::increaseIceCount() {
	globalIceShoot++;
}

void placeholder::increasePropellerCount() {
	globalPropeller++;
}

extern bool enableDebugMode;
bool doneWithGreenStars;
bool doneWithPurpleCoins;
extern u8 LastLevelPlayed[2];
extern "C" void *dAcPy_c__ChangePowerupWithAnimation(void * Player, int powerup);
int dGameDisplay_c::cleanClouds() {
	int orig_val = this->onCreate_orig();
	
	amIinCloud = 0;
	imDoneDoingVisibility = false;
	doneWithGreenStars = false;
	doneWithPurpleCoins = false;

	return orig_val;
}

int dGameDisplay_c::doWaitCheck() {
	int orig_val = this->onExecute_orig();
	if(doWait > 0) {
		doWait--;
	}
	// OSReport("globalIceShoot = %d\n", globalIceShoot);
	// OSReport("globalPropeller = %d\n", globalPropeller);

	dAcPy_c *player = dAcPy_c::findByID(0);
	int p = CheckExistingPowerup(player);
	if(player->input.nowPressed & WPAD_B) {
		enableDebugMode = !enableDebugMode;
	}
	
	/*******/
	/*Cloud*/
	/*******/
	if(p == 12 && doWait == 0) {
		if (player->input.areWeShaking() && cloudSpawned[0] < 3) {
			int isPlayerMoving = 1;
			if(!(player->input.heldButtons & WPAD_LEFT) && !(player->input.heldButtons & WPAD_RIGHT) && (player->collMgr.isOnTopOfTile())) {
				isPlayerMoving = 0;
			}
			int settings = 0 | (0 << 4) | (player->direction << 8) | (4 << 12);
			VEC3 actualPos = {player->pos.x + ((isPlayerMoving == 1) ? ((player->direction == 1) ? -24 : 24) : 0), player->pos.y - ((isPlayerMoving == 1) ? 25 : 5), player->pos.z};
			// VEC3 actualPos = {player->pos.x, player->pos.y - 20, player->pos.z};
			dStageActor_c *cloud = CreateActor(555, settings, actualPos, 0, 0);
			cloudSpawned[0]++;
			player->speed.x = 0;
			player->speed.y = 0;
			imDoneDoingVisibility = false;
			doWait = 30;
		}
	}
	
	for(int i=1; i<4; i++) {
		dAcPy_c *player = dAcPy_c::findByID(i);
		if (CheckExistingPowerup(player) == 12 && player->input.areWeShaking() && cloudSpawned[i] < 3) {
			int isPlayerMoving = 1;
			if(!(player->input.heldButtons & WPAD_LEFT) && !(player->input.heldButtons & WPAD_RIGHT) && (player->collMgr.isOnTopOfTile())) {
				isPlayerMoving = 0;
			}
			int settings = 0 | (0 << 4) | (player->direction << 8) | (4 << 12);
			VEC3 actualPos = {player->pos.x + ((isPlayerMoving == 1) ? ((player->direction == 1) ? -24 : 24) : 0), player->pos.y - ((isPlayerMoving == 1) ? 25 : 5), player->pos.z};
			// VEC3 actualPos = {player->pos.x, player->pos.y - 20, player->pos.z};
			dStageActor_c *cloud = CreateActor(555, settings, actualPos, 0, 0);
			cloudSpawned[i]++;
			player->speed.x = 0;
			player->speed.y = 0;
			imDoneDoingVisibility = false;
			doWait = 30;
		}
	}
	
	if(p == 12) {
		if(amIinCloud == 0) {
			amIinCloud = 1;
			imDoneDoingVisibility = false;
		}
	}
	if(p != 12) {
		if(amIinCloud == 1) {
			amIinCloud = 0;
			imDoneDoingVisibility = false;
		}
	}
	if(!imDoneDoingVisibility) {
		nw4r::lyt::Picture *P_cloud_00;
		nw4r::lyt::Picture *P_cloud_01;
		nw4r::lyt::Picture *P_cloud_02;
		nw4r::lyt::Picture *P_cloudOff_00;
		nw4r::lyt::Picture *P_cloudOff_01;
		nw4r::lyt::Picture *P_cloudOff_02;
		P_cloud_00 = layout.findPictureByName("P_cloud_00");
		P_cloud_01 = layout.findPictureByName("P_cloud_01");
		P_cloud_02 = layout.findPictureByName("P_cloud_02");
		P_cloudOff_00 = layout.findPictureByName("P_cloudOff_00");
		P_cloudOff_01 = layout.findPictureByName("P_cloudOff_01");
		P_cloudOff_02 = layout.findPictureByName("P_cloudOff_02");
		if(p == 12) {
			if(cloudSpawned[0] == 0) {
				P_cloud_00->SetVisible(true);
				P_cloud_01->SetVisible(true);
				P_cloud_02->SetVisible(true);
				P_cloudOff_00->SetVisible(false);
				P_cloudOff_01->SetVisible(false);
				P_cloudOff_02->SetVisible(false);
			}
			if(cloudSpawned[0] == 1) {
				P_cloud_00->SetVisible(false);
				P_cloud_01->SetVisible(true);
				P_cloud_02->SetVisible(true);
				P_cloudOff_00->SetVisible(true);
				P_cloudOff_01->SetVisible(false);
				P_cloudOff_02->SetVisible(false);
			}
			if(cloudSpawned[0] == 2) {
				P_cloud_00->SetVisible(false);
				P_cloud_01->SetVisible(false);
				P_cloud_02->SetVisible(true);
				P_cloudOff_00->SetVisible(true);
				P_cloudOff_01->SetVisible(true);
				P_cloudOff_02->SetVisible(false);
			}
			if(cloudSpawned[0] == 3) {
				P_cloud_00->SetVisible(false);
				P_cloud_01->SetVisible(false);
				P_cloud_02->SetVisible(false);
				P_cloudOff_00->SetVisible(true);
				P_cloudOff_01->SetVisible(true);
				P_cloudOff_02->SetVisible(true);
			}
		}
		else {
			P_cloud_00->SetVisible(false);
			P_cloud_01->SetVisible(false);
			P_cloud_02->SetVisible(false);
			P_cloudOff_00->SetVisible(false);
			P_cloudOff_01->SetVisible(false);
			P_cloudOff_02->SetVisible(false);
		}
		
		P_cloud_00 = layout.findPictureByName("P_cloud_03");
		P_cloud_01 = layout.findPictureByName("P_cloud_05");
		P_cloud_02 = layout.findPictureByName("P_cloud_04");
		P_cloudOff_00 = layout.findPictureByName("P_cloudOff_05");
		P_cloudOff_01 = layout.findPictureByName("P_cloudOff_04");
		P_cloudOff_02 = layout.findPictureByName("P_cloudOff_03");
		if(CheckExistingPowerup(dAcPy_c::findByID(1))) {
			if(cloudSpawned[1] == 0) {
				P_cloud_00->SetVisible(true);
				P_cloud_01->SetVisible(true);
				P_cloud_02->SetVisible(true);
				P_cloudOff_00->SetVisible(false);
				P_cloudOff_01->SetVisible(false);
				P_cloudOff_02->SetVisible(false);
			}
			if(cloudSpawned[1] == 1) {
				P_cloud_00->SetVisible(false);
				P_cloud_01->SetVisible(true);
				P_cloud_02->SetVisible(true);
				P_cloudOff_00->SetVisible(true);
				P_cloudOff_01->SetVisible(false);
				P_cloudOff_02->SetVisible(false);
			}
			if(cloudSpawned[1] == 2) {
				P_cloud_00->SetVisible(false);
				P_cloud_01->SetVisible(false);
				P_cloud_02->SetVisible(true);
				P_cloudOff_00->SetVisible(true);
				P_cloudOff_01->SetVisible(true);
				P_cloudOff_02->SetVisible(false);
			}
			if(cloudSpawned[1] == 3) {
				P_cloud_00->SetVisible(false);
				P_cloud_01->SetVisible(false);
				P_cloud_02->SetVisible(false);
				P_cloudOff_00->SetVisible(true);
				P_cloudOff_01->SetVisible(true);
				P_cloudOff_02->SetVisible(true);
			} 
		} else {
			P_cloud_00->SetVisible(false);
			P_cloud_01->SetVisible(false);
			P_cloud_02->SetVisible(false);
			P_cloudOff_00->SetVisible(false);
			P_cloudOff_01->SetVisible(false);
			P_cloudOff_02->SetVisible(false);
		}
		
		
		P_cloud_00 = layout.findPictureByName("P_cloud_06");
		P_cloud_01 = layout.findPictureByName("P_cloud_08");
		P_cloud_02 = layout.findPictureByName("P_cloud_07");
		P_cloudOff_00 = layout.findPictureByName("P_cloudOff_08");
		P_cloudOff_01 = layout.findPictureByName("P_cloudOff_07");
		P_cloudOff_02 = layout.findPictureByName("P_cloudOff_06");
		if(CheckExistingPowerup(dAcPy_c::findByID(2))) {
			if(cloudSpawned[2] == 0) {
				P_cloud_00->SetVisible(true);
				P_cloud_01->SetVisible(true);
				P_cloud_02->SetVisible(true);
				P_cloudOff_00->SetVisible(false);
				P_cloudOff_01->SetVisible(false);
				P_cloudOff_02->SetVisible(false);
			}
			if(cloudSpawned[2] == 1) {
				P_cloud_00->SetVisible(false);
				P_cloud_01->SetVisible(true);
				P_cloud_02->SetVisible(true);
				P_cloudOff_00->SetVisible(true);
				P_cloudOff_01->SetVisible(false);
				P_cloudOff_02->SetVisible(false);
			}
			if(cloudSpawned[2] == 2) {
				P_cloud_00->SetVisible(false);
				P_cloud_01->SetVisible(false);
				P_cloud_02->SetVisible(true);
				P_cloudOff_00->SetVisible(true);
				P_cloudOff_01->SetVisible(true);
				P_cloudOff_02->SetVisible(false);
			}
			if(cloudSpawned[2] == 3) {
				P_cloud_00->SetVisible(false);
				P_cloud_01->SetVisible(false);
				P_cloud_02->SetVisible(false);
				P_cloudOff_00->SetVisible(true);
				P_cloudOff_01->SetVisible(true);
				P_cloudOff_02->SetVisible(true);
			} 
		} else {
			P_cloud_00->SetVisible(false);
			P_cloud_01->SetVisible(false);
			P_cloud_02->SetVisible(false);
			P_cloudOff_00->SetVisible(false);
			P_cloudOff_01->SetVisible(false);
			P_cloudOff_02->SetVisible(false);
		}
		
		
		P_cloud_00 = layout.findPictureByName("P_cloud_09");
		P_cloud_01 = layout.findPictureByName("P_cloud_11");
		P_cloud_02 = layout.findPictureByName("P_cloud_10");
		P_cloudOff_00 = layout.findPictureByName("P_cloudOff_11");
		P_cloudOff_01 = layout.findPictureByName("P_cloudOff_10");
		P_cloudOff_02 = layout.findPictureByName("P_cloudOff_09");
		if(CheckExistingPowerup(dAcPy_c::findByID(3))) {
			if(cloudSpawned[3] == 0) {
				P_cloud_00->SetVisible(true);
				P_cloud_01->SetVisible(true);
				P_cloud_02->SetVisible(true);
				P_cloudOff_00->SetVisible(false);
				P_cloudOff_01->SetVisible(false);
				P_cloudOff_02->SetVisible(false);
			}
			if(cloudSpawned[3] == 1) {
				P_cloud_00->SetVisible(false);
				P_cloud_01->SetVisible(true);
				P_cloud_02->SetVisible(true);
				P_cloudOff_00->SetVisible(true);
				P_cloudOff_01->SetVisible(false);
				P_cloudOff_02->SetVisible(false);
			}
			if(cloudSpawned[3] == 2) {
				P_cloud_00->SetVisible(false);
				P_cloud_01->SetVisible(false);
				P_cloud_02->SetVisible(true);
				P_cloudOff_00->SetVisible(true);
				P_cloudOff_01->SetVisible(true);
				P_cloudOff_02->SetVisible(false);
			}
			if(cloudSpawned[3] == 3) {
				P_cloud_00->SetVisible(false);
				P_cloud_01->SetVisible(false);
				P_cloud_02->SetVisible(false);
				P_cloudOff_00->SetVisible(true);
				P_cloudOff_01->SetVisible(true);
				P_cloudOff_02->SetVisible(true);
			} 
		} else {
			P_cloud_00->SetVisible(false);
			P_cloud_01->SetVisible(false);
			P_cloud_02->SetVisible(false);
			P_cloudOff_00->SetVisible(false);
			P_cloudOff_01->SetVisible(false);
			P_cloudOff_02->SetVisible(false);
		}
		
		
		imDoneDoingVisibility = true;
	}

	return orig_val;
}

/***********************/
/**Global coin counter**/
/***********************/

void dGameDisplay_c::doGlobalCoin() {
	if(globalCoins != dGameDisplay_c::instance->coins) {
		globalCoins++;
	}
	nw4r::lyt::TextBox *stupidcoin = dGameDisplay_c::instance->layout.findTextBoxByName("T_coin_00");					//get the coin textbox
	char str[2];																										//make a char that will contain the text to display
	sprintf(str, "%02d", dGameDisplay_c::instance->coins);																//insert the number in the char win three characters
	wchar_t nyeh[2];																									//make a wchar_t
	nyeh[0] = str[0];																									//put the char's content in the wchar_t
	nyeh[1] = str[1];
	stupidcoin->SetString(nyeh, 0, 2);																					//write it to the textbox
}

void dGameDisplay_c::refreshPurpleCoins() {
	nw4r::lyt::TextBox *stupidpurplecoin = dGameDisplay_c::instance->layout.findTextBoxByName("T_purpleCount_00");		//get the purple coin textbox
	char str[3];																										//make a char that will contain the text to display
	sprintf(str, "%03d", dGameDisplay_c::instance->purplecoins);														//insert the number in the char win three characters
	wchar_t nyeh[3];																									//make a wchar_t
	nyeh[0] = str[0];																									//put the char's content in the wchar_t
	nyeh[1] = str[1];
	nyeh[2] = str[2];
	stupidpurplecoin->SetString(nyeh, 0, 3);																			//write it to the textbox
}