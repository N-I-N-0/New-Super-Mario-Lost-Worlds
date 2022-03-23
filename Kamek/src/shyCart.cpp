#include <common.h>
#include <game.h>
#include <profile.h>
#include "baddy.h"

const char* ShyCartFileList[] = { "shyCart", 0 };


class daEnShyCart_c : public dEn_c {
public:
	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	mHeapAllocator_c allocator;
	nw4r::g3d::ResFile resFile;
	m3d::mdl_c bodyModel;

	int alreadyOnTop;
	bool isOnTopOfTile;
	u32 cmgr_returnValue;

	u8 moveSpeed;

	u8 lastRailType;

	bool ready;
	bool readyTwo;

	dStageActor_c* passengers[4];

	bool jumping;
	bool slowing;
	bool speeding;

	float rotDestination;


	RideableActorCollider sotCollider;


	void rampCalc(u8 railType);

	static dActor_c* build();

	void updateModelMatrices();

	bool calculateTileCollisions();

	void playerCollision(ActivePhysics* apThis, ActivePhysics* apOther);
	void yoshiCollision(ActivePhysics* apThis, ActivePhysics* apOther);

	bool collisionCat7_GroundPound(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat7_GroundPoundYoshi(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCatD_Drill(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCatA_PenguinMario(ActivePhysics* apThis, ActivePhysics* apOther);

	bool collisionCat1_Fireball_E_Explosion(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat9_RollingObject(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat13_Hammer(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat14_YoshiFire(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat3_StarPower(ActivePhysics* apThis, ActivePhysics* apOther);
};

const SpriteData ShyCartSpriteData = { ProfileId::ShyCart, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile ShyCartProfile(&daEnShyCart_c::build, SpriteId::ShyCart, &ShyCartSpriteData, ProfileId::ShyCart, ProfileId::ShyCart, "ShyCart", ShyCartFileList);



u8 hijackMusicWithSongName(const char* songName, int themeID, bool hasFast, int channelCount, int trackCount, int* wantRealStreamID);


bool daEnShyCart_c::calculateTileCollisions() {

	/****************************/
	/***Tile collision detector**/
	/*Returns true if hit a wall*/
	/****************************/


	this->x_speed_inc = 0.0f;                        //no X speed incrementation


	HandleYSpeed();
	HandleXSpeed();                                      //consider the X speed
														 //consider the Y speed
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
		if (alreadyOnTop == 0) {                          //if it just landed
			PlaySound(this, SE_OBJ_ROCK_LAND);           //play SFX
			alreadyOnTop = 1;                            //now it's already on the to^p
		}

		speed.y = 0.0f;                                  //no Y speed anymore cuz it's on the ground
		max_speed.x = (direction == 1) ? -16.0f : 16.0f;   //maximum X speed re-setting
		this->x_speed_inc = 0.0f;                        //no X speed incrementation
	}
	else {                                               //if the sprite in in mid-air
		alreadyOnTop = 0;                                //it's no loner on the top of a tile
	}

	collMgr.calculateAdjacentCollision(0);               //literally calculate the adjacent collision

	if (collMgr.outputMaybe & (0x15 << direction)) {     //if hit a wall 
		isOnTopOfTile = collMgr.isOnTopOfTile();
		if(isOnTopOfTile)
			jumping = false;
		return true;                                     //returns true duh
	}

	isOnTopOfTile = collMgr.isOnTopOfTile();
	if (isOnTopOfTile)
		jumping = false;

	return false;                                        //if didn't hit a wall
}




void daEnShyCart_c::playerCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	if (!ready && !readyTwo)
	{
		for (int i = 0; i < GetActivePlayerCount(); i++)
		{
			passengers[i] = (dStageActor_c*)GetPlayerOrYoshi(i);
		}

		ready = true;
	}
}
void daEnShyCart_c::yoshiCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->playerCollision(apThis, apOther);
}
bool daEnShyCart_c::collisionCat7_GroundPound(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnShyCart_c::collisionCat7_GroundPoundYoshi(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnShyCart_c::collisionCatD_Drill(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnShyCart_c::collisionCatA_PenguinMario(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}

bool daEnShyCart_c::collisionCat1_Fireball_E_Explosion(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnShyCart_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics* apThis, ActivePhysics* apOther) {

	return false;
}
bool daEnShyCart_c::collisionCat9_RollingObject(ActivePhysics* apThis, ActivePhysics* apOther) {

	return false;
}
bool daEnShyCart_c::collisionCat13_Hammer(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnShyCart_c::collisionCat14_YoshiFire(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}

bool daEnShyCart_c::collisionCat3_StarPower(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}


dActor_c* daEnShyCart_c::build() {
	void* buffer = AllocFromGameHeap1(sizeof(daEnShyCart_c));
	daEnShyCart_c* c = new(buffer) daEnShyCart_c;

	return c;
}


extern int getNybbleValue(u32 settings, int fromNybble, int toNybble);

int daEnShyCart_c::onCreate() {
	this->deleteForever = true;

	// Model creation	
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	OSReport("Model\n");

	this->resFile.data = getResource("shyCart", "g3d/minecart.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("TruckWagon");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	//SetupTextures_Player(&bodyModel, 0);

	allocator.unlink();

	OSReport("Model Done\n");

	ActivePhysics::Info HitMeBaby;

	HitMeBaby.xDistToCenter = 0.0;
	HitMeBaby.yDistToCenter = 12.0;
	HitMeBaby.xDistToEdge = 32.0;
	HitMeBaby.yDistToEdge = 12.0;

	HitMeBaby.category1 = 0x3;
	HitMeBaby.category2 = 0x0;
	HitMeBaby.bitfield1 = 0x4F;
	HitMeBaby.bitfield2 = 0x8028E;
	HitMeBaby.unkShort1C = 0;
	HitMeBaby.callback = &dEn_c::collisionCallback;

	this->aPhysics.initWithStruct(this, &HitMeBaby);
	this->aPhysics.addToList();

	// Stuff I do understand
	this->scale = (Vec){ 0.75, 0.65, 0.2 };

	this->rot.x = 0; // X is vertical axis
	this->rot.y = 0x4000; // Y is horizontal axis
	this->rot.z = 0; // Z is ... an axis >.>


	this->moveSpeed = this->settings >> 24 & 0xFF;


	this->max_speed.x = 16;
	this->speed.x = 0;             //set the current X speed depending of the spawning direction
	this->x_speed_inc = 0;                   //set the X speed increment depending of the spawning direction

	this->max_speed.y = -4;                                               //set the maximum Y speed
	this->speed.y = 0.0;                                                  //set the current Y speed
	this->y_speed_inc = -0.2;

	sotCollider.init(this, -16.0f, -16.0f, 16.0f, 16.0f);

	// What is this for. I dunno
	//sotCollider._47 = 0xA;
	//sotCollider.flags = 0x80180 | 0xC00;

	sotCollider.addToList();

	// These fucking rects do something for the tile rect
	spriteSomeRectX = 28.0f;
	spriteSomeRectY = 32.0f;
	_320 = 0.0f;
	_324 = 16.0f;

	static const lineSensor_s below(-5 << 12, 5 << 12, -9 << 12);            //Below collider:    unk, unk, y pos 
	static const pointSensor_s above(0 << 12, 12 << 12);                   //Above collider:    unk, unk, unk
	static const lineSensor_s adjacent(3 << 12, 6 << 12, 6 << 12);           //Adjacent collider: unk, unk, unk

	collMgr.init(this, &below, &above, &adjacent);                     //initating colliders
	collMgr.calculateBelowCollisionWithSmokeEffect();                  //dude, the description of this line IS in the line, so what are you reading ?

	cmgr_returnValue = collMgr.isOnTopOfTile();                        //get if the sprite is in top of a tile (GOD DAMMIT IT IS WROTE)
	isOnTopOfTile = collMgr.isOnTopOfTile();

	this->onExecute();
	return true;
}


int daEnShyCart_c::onDelete() {
	return true;
}

int daEnShyCart_c::onDraw() {
	bodyModel.scheduleForDrawing();
	return true;
}


void daEnShyCart_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

int daEnShyCart_c::onExecute() {
	this->x_speed_inc = 0.0f;                        //no X speed incrementation

	bodyModel._vf1C();
	updateModelMatrices();
	bool ret = calculateTileCollisions();

	if (ready)
	{
		if (slowing)
		{
			OSReport("slow slow\n");
			this->speed.x *= 1.5;
		}
		else if (speeding)
		{
			OSReport("speed speed\n");
			this->speed.x /= 1.5;
		}

		u8 railType = 0;
		u32 tileBehave1;
		u32 tileBehave2;

		daPlBase_c* player = (daPlBase_c*)FindActorByType(PLAYER, 0);
		
		if(player->input.areWeShaking())
		{
			ready = false;
			readyTwo = true;
		}

		if (isOnTopOfTile)
		{
			for (int i = 0; i <= 10; i++)
			{
				tileBehave1 = collMgr.getTileBehaviour1At(this->pos.x, this->pos.y - i, 0);
				tileBehave2 = collMgr.getTileBehaviour2At(this->pos.x, this->pos.y - i, 0);

				railType = tileBehave2 >> 24 & 0xF;

				OSReport("i: %d, tileBehave1: %x, tileBehave2: %x, railType: %d, speed x: %f, speed y: %f, moveSpeed: %d\n", i, tileBehave1, tileBehave2, railType, this->speed.x, this->speed.y, this->moveSpeed);

				if (tileBehave1 != 0 || tileBehave2 != 0)
				{
					OSReport("i: %d\n", i);
					break;
				}
			}

			if (railType == 1) //straight rail
			{
				rotDestination = 0;

				OSReport("Is Rail\n");
				if (this->speed.x < moveSpeed)
				{
					OSReport("is accelerating\n");
					this->speed.x += ((float)moveSpeed) / 100;
				}
				else
				{
					OSReport("is slowing down\n");
					this->speed.x -= ((float)moveSpeed) / 200;
				}
			}
			else if (railType == 2)  //11,25° slope up
			{
				rotDestination = -(0x800);

				OSReport("Is Rail 11,25° slope up\n");
				if (this->speed.x < (((float)moveSpeed) / 1.25))
				{
					OSReport("is accelerating\n");
					this->speed.x += (((float)moveSpeed) / 1.25) / 100;
				}
				else
				{
					OSReport("is slowing down\n");
					this->speed.x -= (((float)moveSpeed) * 1.25) / 200;
				}
			}
			else if (railType == 3)  //22,5° slope up
			{
				rotDestination = -(0x1000);

				OSReport("Is Rail 22,5° slope up\n");
				if (this->speed.x < (((float)moveSpeed) / 1.5))
				{
					OSReport("is accelerating\n");
					this->speed.x += (((float)moveSpeed) / 1.5) / 100;
				}
				else
				{
					OSReport("is slowing down\n");
					this->speed.x -= (((float)moveSpeed) * 1.5) / 200;
				}
			}
			else if (railType == 4)  //45° slope up
			{
				rotDestination = -(0x2000);

				OSReport("Is Rail 45° slope up\n");
				if (this->speed.x < (((float)moveSpeed) / 2))
				{
					OSReport("is accelerating\n");
					this->speed.x += (((float)moveSpeed) / 2) / 100;
				}
				else
				{
					OSReport("is slowing down\n");
					this->speed.x -= (((float)moveSpeed) * 2) / 200;
				}
			}
			else if (railType == 5)  //11,25° slope down
			{
				rotDestination = 0x800;

				OSReport("Is Rail 11,25° slope down\n");
				if (this->speed.x < (((float)moveSpeed) * 1.25))
				{
					OSReport("is accelerating\n");
					this->speed.x += (((float)moveSpeed) * 1.25) / 100;
				}
				else
				{
					OSReport("is slowing down\n");
					this->speed.x -= (((float)moveSpeed) / 1.25) / 200;
				}
			}
			else if (railType == 6)  //22,5° slope down
			{
				rotDestination = 0x1000;

				OSReport("Is Rail 22,5° slope down\n");
				if (this->speed.x < (((float)moveSpeed) * 1.5))
				{
					OSReport("is accelerating\n");
					this->speed.x += (((float)moveSpeed) * 1.5) / 100;
				}
				else
				{
					OSReport("is slowing down\n");
					this->speed.x -= (((float)moveSpeed) / 1.5) / 200;
				}
			}
			else if (railType == 7)  //45° slope down
			{
				rotDestination = 0x2000;

				OSReport("Is Rail 45° slope down\n");
				if (this->speed.x < (((float)moveSpeed) * 2))
				{
					OSReport("is accelerating\n");
					this->speed.x += (((float)moveSpeed) * 2) / 100;
				}
				else
				{
					OSReport("is slowing down\n");
					this->speed.x -= (((float)moveSpeed) / 2) / 200;
				}
			}
			else
			{
				rotDestination = 0;

				OSReport("Is not Rail\n");
				if (this->speed.x > 0)
				{
					OSReport("Is not stoped yet\n");
					this->speed.x -= ((float)moveSpeed) / 150;
				}
			}

			OSReport("Is on top of tile: %d\n", isOnTopOfTile);

			for (int i = 0; i < GetActivePlayerCount(); i++)
			{
				Remocon* pIn = RemoconMng->controllers[i];

				if (!(pIn->nowPressed & WPAD_TWO))
				{
					break;
				}
				else if (i == GetActivePlayerCount() - 1)
				{
					this->speed.y += 4;
					jumping = true;
				}
			}
		}

		if (!jumping)
			this->rampCalc(railType);


		if (isOnTopOfTile)
		{
			for (int i = 0; i < GetActivePlayerCount(); i++)
			{
				Remocon* pIn = RemoconMng->controllers[i];

				OSReport("slow slow test i: %d\n", i);

				if (!(pIn->heldButtons & WPAD_LEFT))
				{
					slowing = false;
					OSReport("not slowing down\n");
					break;
				}
				else if (i == GetActivePlayerCount() - 1)
				{
					OSReport("slowing down\n");
					slowing = true;
				}
			}

			for (int i = 0; i < GetActivePlayerCount(); i++)
			{
				Remocon* pIn = RemoconMng->controllers[i];

				OSReport("slow slow test i: %d\n", i);

				if (!(pIn->heldButtons & WPAD_RIGHT))
				{
					speeding = false;
					OSReport("not speedingup down\n");
					break;
				}
				else if (i == GetActivePlayerCount() - 1)
				{
					OSReport("speedingup down\n");
					speeding = true;
				}
			}
		}

		if (slowing)
		{
			OSReport("speeeeeeeed\n");
			this->speed.x /= 1.5;
		}
		else if (speeding)
		{
			OSReport("speed speed\n");
			this->speed.x *= 1.5;
		}

		OSReport("speed x: %f, speed y: %f\n", this->speed.x, this->speed.y);


		if (this->rot.x != rotDestination)
		{
			if (this->rot.x < rotDestination)
			{
				rot.x += (0x80) * moveSpeed;
				if (this->rot.x > rotDestination)
				{
					this->rot.x = rotDestination;
				}
			}
			else
			{
				rot.x += (0x80) * -moveSpeed;
				if (this->rot.x > rotDestination)
				{
					this->rot.x = rotDestination;
				}
			}
		}

		lastRailType = railType;

		for (int i = 0; i < GetActivePlayerCount(); i++)
		{
			passengers[i]->pos.x = this->pos.x - ((((float)i) - 1.5) * 16);
			passengers[i]->pos.y = this->pos.y;
		}

		this->pos.z = passengers[0]->pos.z;
	}

	return true;
}

void daEnShyCart_c::rampCalc(u8 railType) 
{
	float lastXSpeed = this->speed.x;

	if (lastRailType == 0 || lastRailType == 1 || railType == 4)
	{
		return;
	}

	if (railType == 0) //Not
	{
		if (lastRailType == 2) //11,25 up
		{
			this->speed.x = ((this->speed.x / 8) * 7) * 1.4;
			this->speed.y = (this->speed.x / 8) * 1.8;
		}
		else if (lastRailType == 3) //22,5 up
		{
			this->speed.x = ((this->speed.x / 4) * 3) * 1.4;
			this->speed.y = (this->speed.x / 4) * 1.8;
		}
		else if (lastRailType == 4) //45 up
		{
			this->speed.x = (this->speed.x / 2) * 1.4;
			this->speed.y = (this->speed.x / 2) * 1.8;
		}
		else if (lastRailType == 5) //11,25 down
		{
			this->speed.x = ((this->speed.x / 8) * 7) * 1.4;
			this->speed.y = -((this->speed.x / 8) * 1.8);
		}
		else if (lastRailType == 6) //22,5 down
		{
			this->speed.x = ((this->speed.x / 4) * 3) * 1.4;
			this->speed.y = -((this->speed.x / 4) * 1.8);
		}
		else if (lastRailType == 7) //45 down
		{
			this->speed.x = (this->speed.x / 2) * 1.4;
			this->speed.y = -((this->speed.x / 2) * 1.8);
		}
	}
	else if (railType == 1) //Straight
	{
		if (lastRailType == 2) //11,25 up
		{
			this->speed.x = ((this->speed.x / 8) * 7) * 1.4;
			this->speed.y = (this->speed.x / 8) * 1.8;
		}
		else if (lastRailType == 3) //22,5 up
		{
			this->speed.x = ((this->speed.x / 4) * 3) * 1.4;
			this->speed.y = (this->speed.x / 4) * 1.8;
		}
		else if (lastRailType == 4) //45 up
		{
			this->speed.x = (this->speed.x / 2) * 1.4;
			this->speed.y = (this->speed.x / 2) * 1.8;
		}
	}
	else if (railType == 2) //11,25 up
	{
		if (lastRailType == 3) //22,5 up
		{
			this->speed.x = ((this->speed.x / 4) * 3) * 1.4;
			this->speed.y = (this->speed.x / 4) * 1.8;
		}
		else if (lastRailType == 4) //45 up
		{
			this->speed.x = (this->speed.x / 2) * 1.4;
			this->speed.y = (this->speed.x / 2) * 1.8;
		}
	}
	else if (railType == 3) //22,5 up
	{
		if (lastRailType == 4) //45 up
		{
			this->speed.x = (this->speed.x / 2) * 1.4;
			this->speed.y = (this->speed.x / 2) * 1.8;
		}
	}
	else if (railType == 4) //45 up
	{
		// Nothing cause it would bump anyways
	}
	else if (railType == 5) //11,25 down
	{
		if (lastRailType == 2) //11,25 up
		{
			this->speed.x = ((this->speed.x / 8) * 7) * 1.4;
			this->speed.y = (this->speed.x / 8) * 1.8;
		}
		else if (lastRailType == 3) //22,5 up
		{
			this->speed.x = ((this->speed.x / 4) * 3) * 1.4;
			this->speed.y = (this->speed.x / 4) * 1.8;
		}
		else if (lastRailType == 4) //45 up
		{
			this->speed.x = (this->speed.x / 2) * 1.4;
			this->speed.y = (this->speed.x / 2) * 1.8;
		}
	}
	else if (railType == 6) //22,5 down
	{
		if (lastRailType == 2) //11,25 up
		{
			this->speed.x = ((this->speed.x / 8) * 7) * 1.4;
			this->speed.y = (this->speed.x / 8) * 1.8;
		}
		else if (lastRailType == 3) //22,5 up
		{
			this->speed.x = ((this->speed.x / 4) * 3) * 1.4;
			this->speed.y = (this->speed.x / 4) * 1.8;
		}
		else if (lastRailType == 4) //45 up
		{
			this->speed.x = (this->speed.x / 2) * 1.4;
			this->speed.y = (this->speed.x / 2) * 1.8;
		}
		else if (lastRailType == 5) //11,25 down
		{
			this->speed.x = ((this->speed.x / 8) * 7) * 1.4;
			this->speed.y = -((this->speed.x / 8) * 1.8);
		}
	}
	else if (railType == 7) //45 down
	{
		if (lastRailType == 2) //11,25 up
		{
			this->speed.x = ((this->speed.x / 8) * 7) * 1.4;
			this->speed.y = (this->speed.x / 8) * 1.8;
		}
		else if (lastRailType == 3) //22,5 up
		{
			this->speed.x = ((this->speed.x / 4) * 3) * 1.4;
			this->speed.y = (this->speed.x / 4) * 1.8;
		}
		else if (lastRailType == 4) //45 up
		{
			this->speed.x = (this->speed.x / 2) * 1.4;
			this->speed.y = (this->speed.x / 2) * 1.8;
		}
		else if (lastRailType == 5) //11,25 down
		{
			this->speed.x = ((this->speed.x / 8) * 7) * 1.4;
			this->speed.y = -((this->speed.x / 8) * 1.8);
		}
		else if (lastRailType == 6) //22,5 down
		{
			this->speed.x = ((this->speed.x / 4) * 3) * 1.4;
			this->speed.y = -((this->speed.x / 4) * 1.8);
		}
	}

	/*if (lastXSpeed != speed.x)
	{
		OSReport("Changed\n");
		this->speed.x * 4;
		this->speed.y * 4;
	}*/
}