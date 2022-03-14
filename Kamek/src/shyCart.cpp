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

	dPlayer* passenger;
	bool done;
	int timerDone;

	int alreadyOnTop;

	bool isOnTopOfTile;
	u32 cmgr_returnValue;

	int driveSpeed;
	bool moveable;
	
	//int last_speed_y;
	//int last_speed_x;

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




void daEnShyCart_c::playerCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	/*//OSReport("Collided\n");
	//OSReport("Which_Player: %d\n", apOther->owner->which_player);
	if (this->passenger == (dPlayer*)apOther->owner || playerInWork[apOther->owner->which_player] || this->done)
	{
		return;
	}*/
	if (this->passenger == 0 && playerStatus[apOther->owner->which_player] == 0)
	{
		OSReport("Yes\n");
		this->passenger = (dPlayer*)apOther->owner;
		playerStatus[apOther->owner->which_player] = 2;
		//playerInWork[apOther->owner->which_player] = true;
		//playerRiding[apOther->owner->which_player] = true;
		return;
	}
}
void daEnShyCart_c::yoshiCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	//this->playerCollision(apThis, apOther);
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

	this->resFile.data = getResource("shyCart", "g3d/shyCart.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("ShyCart");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Player(&bodyModel, 0);

	allocator.unlink();

	ActivePhysics::Info HitMeBaby;

	HitMeBaby.xDistToCenter = 0.0;
	HitMeBaby.yDistToCenter = 0.0;
	HitMeBaby.xDistToEdge = 8.0;
	HitMeBaby.yDistToEdge = 10.0;

	HitMeBaby.category1 = 0x3;
	HitMeBaby.category2 = 0x0;
	HitMeBaby.bitfield1 = 0x4F;
	HitMeBaby.bitfield2 = 0x8028E;
	HitMeBaby.unkShort1C = 0;
	HitMeBaby.callback = &dEn_c::collisionCallback;

	this->aPhysics.initWithStruct(this, &HitMeBaby);
	this->aPhysics.addToList();

	// Stuff I do understand
	this->scale = (Vec){ 20.0, 20.0, 20.0 };

	this->rot.x = 0; // X is vertical axis
	this->rot.y = 0xD800; // Y is horizontal axis
	this->rot.z = 0; // Z is ... an axis >.>

	this->rot.y *= -1;


	this->driveSpeed = this->settings >> 25 & 0b11111111;
	this->moveable = true;


	this->max_speed.x = 10;
	this->speed.x = 0.0;             //set the current X speed depending of the spawning direction
	this->x_speed_inc = -0.6;                   //set the X speed increment depending of the spawning direction

	this->max_speed.y = -4;                                               //set the maximum Y speed
	this->speed.y = 0.0;                                                  //set the current Y speed
	this->y_speed_inc = -0.5;



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

	if (collMgr.isOnTopOfTile())                                       //just... read !
		isOnTopOfTile = false;
	else
		isOnTopOfTile = true;

	//this->last_speed_x = passenger->speed.x;
	//this->last_speed_y = passenger->speed.y;

	this->done = false;
	this->timerDone = 0;

	for (int i = 0; i < 4; i++)
	{
		if (playerCart[i] == 0)
		{
			playerCart[i] = this;
			break;
		}
	}

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
	bodyModel._vf1C();
	updateModelMatrices();

	if (this->passenger != 0)
	{
		if (!this->done)
		{

			this->passenger->pos = this->pos;
			this->passenger->last_pos = this->pos;


			//this->speed.x += this->x_speed_inc;
			//this->speed.y += this->y_speed_inc;


			/*if (this->last_speed_x < 0 && passenger->speed.x > 0)
			{
				this->speed.x = 5;
			}
			else if (this->last_speed_x > 0 && passenger->speed.x < 0)
			{
				this->speed.x = -5;
			}


			if (this->last_speed_y < 0 && passenger->speed.y > 0)
			{
				this->speed.y = 3;
			}
			else if (this->last_speed_y > 0 && passenger->speed.y < 0)
			{

			}*/


			/*this->speed.x = this->passenger->speed.x * 2;
			if (this->passenger->speed.y > 0)
			{
				this->speed.y = this->passenger->speed.y * 2;
			}
			else
			{
				this->speed.y = -4;
			}


			if (this->speed.x < 0)
			{
				this->max_speed.x = -10;
			}
			else
			{
				this->max_speed.x = 10;
			}

			if (this->speed.x < 0)
			{
				this->x_speed_inc = 0.6;
			}
			else {
				this->x_speed_inc = -0.6;
			}

			this->max_speed.y = -4;
			this->y_speed_inc = -0.5;*/

			/*if (this->speed.y >= -4)
			{
				this->speed.y -= 0.05;
			}*/


			Remocon* pIn = RemoconMng->controllers[this->passenger->which_player];

			//OSReport("Drivespeed: %d\n", this->driveSpeed);
			//OSReport("Held Buttons: %d\n", pIn->heldButtons);
			if (this->moveable)
			{
				if (pIn->heldButtons & WPAD_LEFT)
				{
					//OSReport("Left Pressed\n");
					this->speed.x = (this->driveSpeed / 2) * 1;
				}
				else
				{
					this->speed.x = this->driveSpeed;
				}
			}
			else
			{
				/*if (this->speed.x > 0)
				{
					this->speed.x -= 0.05;
				}
				else
				{
					this->speed.x = 0;
				}*/
				this->speed.x = 0;
			}

			if (Remocon_GetPressed(pIn) & WPAD_TWO && this->collMgr.isOnTopOfTile())
			{
				this->speed.y += 9;
			}

			//OSReport("Is Shaking: %d\n", pIn->isShaking);

			if (pIn->isShaking >= 1)
			{
				this->done = true;
			}

			//if (this->done) {
			//	i
			//}
			//else {
				passenger->speed.x = this->speed.x;

				this->passenger->pos = this->pos;
				this->passenger->last_pos = this->pos;
			//}

			bool ret = calculateTileCollisions();

			//this->last_speed_x = this->passenger->speed.x;
			//this->last_speed_y = this->passenger->speed.y;
		}
	}
	else
	{
		this->speed.x = 0;
		this->speed.y = -2;
		bool ret = calculateTileCollisions();
	}

	if (this->done && this->passenger != 0)
	{
		this->passenger->speed.x = this->speed.x * 3;
		this->passenger->speed.y += 9;
		playerStatus[this->passenger->which_player] = 0;
		this->passenger = 0;
	}

	if (this->done)
	{
		this->timerDone += 1;
		//OSReport("Timer: %d\n", timerDone);
		if (this->timerDone == 100)
		{
			this->timerDone = 0;
			this->done = false;
		}
		//OSReport("Timer: %d\n", timerDone);
	}

	return true;
}