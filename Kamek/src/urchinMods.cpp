#include <game.h>

void rotationMawRayLR(dEn_c* self) {
	if ((self->settings >> 24 & 0xF) == 1) {	//MawRay
		if ((self->settings >> 8 & 1) == 1) {	//Right first
			self->rot.x = 0x4000;
		} else {								//Left first
			self->rot.x = 0x4000;			
			self->rot.y = 0x8000;			
		}
	}
}


class daEnUnizoo_c : public dEn_c {
public:
	int onCreate(); //80ab9980
	int onDelete();
	int onExecute();
	
	void setCustomState();
	bool calculateTileCollisions();
	
	USING_STATES(daEnUnizoo_c);
	DECLARE_STATE(Roll);
	DECLARE_STATE(WaterFalling);
	REF_NINTENDO_STATE(Wait);
};

CREATE_STATE(daEnUnizoo_c, Roll);
CREATE_STATE(daEnUnizoo_c, WaterFalling);

void daEnUnizoo_c::beginState_Roll() {
	this->max_speed.x = 0;//(this->direction) ? -0.5 : 0.5; //set the maximum X speed depending of the spawning direction
	this->speed.x = (this->direction) ? -3.0 : 3.0;                       //set the current X speed depending of the spawning direction
	this->x_speed_inc = (this->direction) ? 0.6 : -0.6;                   //set the X speed increment depending of the spawning direction

	this->max_speed.y = -4;                                               //set the maximum Y speed
	this->speed.y = 6.0;                                                  //set the current Y speed
	this->y_speed_inc = -0.5;                                             //set the Y speed increment


	if(this->name == EN_UNIZOO) {
		static const lineSensor_s below(-5<<12, 5<<12, -12<<12);            //Below collider:    unk, unk, y pos 
		static const pointSensor_s above(0<<12, 12<<12);                   //Above collider:    unk, unk, unk
		static const lineSensor_s adjacent(3<<12, 12<<12, 12<<12);           //Adjacent collider: unk, unk, unk

		collMgr.init(this, &below, &above, &adjacent);                     //initating colliders
		collMgr.calculateBelowCollisionWithSmokeEffect();                  //dude, the description of this line IS in the line, so what are you reading ?
	} else { //EN_UNIRA
		static const lineSensor_s below(-11<<12, 11<<12, -24<<12);            //Below collider:    unk, unk, y pos 
		static const pointSensor_s above(0<<12, 24<<12);                   //Above collider:    unk, unk, unk
		static const lineSensor_s adjacent(3<<12, 24<<12, 24<<12);           //Adjacent collider: unk, unk, unk

		collMgr.init(this, &below, &above, &adjacent);                     //initating colliders
		collMgr.calculateBelowCollisionWithSmokeEffect();                  //dude, the description of this line IS in the line, so what are you reading ?
	}
}

extern "C" u8 checkWater(float, float, u8, float*);

void daEnUnizoo_c::executeState_Roll() {
	s16 rotationSpeed = 0x400 * this->speed.x / this->max_speed.x;
	this->rot.z += (this->direction == 1) ? rotationSpeed : -rotationSpeed;
	bool ret = this->calculateTileCollisions();                                 //calculate the tiles collision
	if (ret) {
		this->speed.x = 0.0f;
		//doStateChange(&StateID_Spike_Die);                                      //if hit a wall, die
	}
	if(this->EnWaterFlagCheck(&this->pos)) {
		this->doStateChange(&StateID_WaterFalling);
	}
}

bool daEnUnizoo_c::calculateTileCollisions() {
	/****************************/
	/***Tile collision detector**/
	/*Returns true if hit a wall*/
	/****************************/
	HandleXSpeed();                                      //consider the X speed
	HandleYSpeed();                                      //consider the Y speed
	doSpriteMovement();                                  //execute the speed movements

	u32 cmgr_returnValue = collMgr.isOnTopOfTile();          //check if the sprite is on top of a tile
	collMgr.calculateBelowCollisionWithSmokeEffect();    //duh

	stuffRelatingToCollisions(0.1875f, 1.0f, 0.5f);      //literally stuff related to collisions

	float xDelta = pos.x - last_pos.x;                   //just read dude

	if (xDelta >= 0.0f) {                                //change of direction if needed
		direction = 0;
	}
	else {
		direction = 1;
	}
	
	/*if(collMgr.getAngleOfSlopeWithXSpeed(this->speed.x) != 0) {
		if(collMgr.getAngleOfSlopeWithXSpeed(this->speed.x) < 0) {
			direction = 1;
		} else {
			direction = 0;
		}
	}*/

	OSReport("Slope: %x\n", collMgr.getAngleOfSlopeWithXSpeed(this->speed.x));

	if (collMgr.isOnTopOfTile()) {                       //if the sprite is on top of a tile
		/*if(alreadyOnTop == 0) {                          //if it just landed
			PlaySound(this, SE_OBJ_ROCK_LAND);           //play SFX
			alreadyOnTop = 1;                            //now it's already on the to^p
		}*/

		//if (cmgr_returnValue == 0)                       //just read ?
		//	isOnTopOfTile = true;

		speed.y = 0.0f;                                  //no Y speed anymore cuz it's on the ground
		max_speed.x = (direction == 1) ? -1.5f : 1.5f;   //maximum X speed re-setting
		this->x_speed_inc = 0.0f;                        //no X speed incrementation
		this->speed.x = collMgr.getAngleOfSlopeWithXSpeed(this->speed.x) * 0.05f - (direction == 1) ? -0.5f : 0.5f; //X speed re-setting
	}
	else {                                               //if the sprite in in mid-air
		//alreadyOnTop = 0;                                //it's no loner on the top of a tile
	}

	collMgr.calculateAdjacentCollision(0);               //literally calculate the adjacent collision

	if (collMgr.outputMaybe & (0x15 << direction)) {     //if hit a wall
		if (collMgr.isOnTopOfTile()) {                   //if on the top of a tile
			//isOnTopOfTile = true;
		}
		return true;                                     //returns true duh
	}
	return false;                                        //if didn't hit a wall
}

void daEnUnizoo_c::endState_Roll() {}

void daEnUnizoo_c::setCustomState() {
	OSReport("Setting Urchin state!\n");
	if(this->settings >> 31) {
		this->doStateChange(&StateID_Roll);
	}
}



void daEnUnizoo_c::beginState_WaterFalling() {
	speed.x = 0;
	speed.y *= 0.3;
}

void daEnUnizoo_c::executeState_WaterFalling() {
	s16 rotationSpeed = 0x400 * this->speed.y / this->max_speed.y;
	bool ret = this->calculateTileCollisions();                                 //calculate the tiles collision
}

void daEnUnizoo_c::endState_WaterFalling() {}