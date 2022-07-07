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
	bool calculateTileCollisions(bool water);
	void animation();
	
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
	bool ret = this->calculateTileCollisions(false);                                 //calculate the tiles collision
	if (ret) {
		this->speed.x = 0.0f;
		//doStateChange(&StateID_Spike_Die);                                      //if hit a wall, die
	}
	if(this->EnWaterFlagCheck(&this->pos)) {
		this->doStateChange(&StateID_WaterFalling);
	}
	
	//this->animation();
}

bool daEnUnizoo_c::calculateTileCollisions(bool water) {
	/****************************/
	/***Tile collision detector**/
	/*Returns true if hit a wall*/
	/****************************/
	
	OSReport("xSpeed: %f\n", this->speed.x);
	OSReport("ySpeed: %f\n", this->speed.y);
	OSReport("direction: %d\n", this->direction);
	
	if (!water && collMgr.isOnTopOfTile()) {                       //if the sprite is on top of a tile
		speed.y = 0.0f;                                  //no Y speed anymore cuz it's on the ground
		max_speed.x = (direction == 1) ? -1.5f : 1.5f;   //maximum X speed re-setting
		this->x_speed_inc = 0.0f;                        //no X speed incrementation
		this->speed.x = (direction == 1) ? -1.5f : 1.5f; //X speed re-setting
	}
	
	HandleXSpeed();                                      //consider the X speed
	HandleYSpeed();                                      //consider the Y speed
	doSpriteMovement();                                  //execute the speed movements

	OSReport("xSpeed: %f\n", this->speed.x);
	OSReport("ySpeed: %f\n", this->speed.y);
	OSReport("direction: %d\n", this->direction);

	OSReport("----\n");

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
	
	OSReport("Slope: %x\n", collMgr.getAngleOfSlopeWithXSpeed(this->speed.x));

	

	collMgr.calculateAdjacentCollision(0);               //literally calculate the adjacent collision

	if (collMgr.outputMaybe & (0x15 << direction)) {     //if hit a wall
		this->speed.x = 0;
		this->x_speed_inc = 0;
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



inline void daEnUnizoo_c::animation() {
	m3d::anmChr_c* chrAnimation = (m3d::anmChr_c*)(((u32)this)+0x58C);
	if(chrAnimation->isAnimationDone()) {
		chrAnimation->setCurrentFrame(0.0);
	}
}



void daEnUnizoo_c::beginState_WaterFalling() {
	speed.x = 0.0;
	this->x_speed_inc = 0;
	speed.y = 1.0;
}

void daEnUnizoo_c::executeState_WaterFalling() {
	float yDelta = abs(pos.y - last_pos.y);                   //just read dude
	if (collMgr.isOnTopOfTile()) {                                //change of direction if needed

		u32 behaivour1 = collisionMgr_c::getTileBehaviour1At(pos.x, pos.y - 2, this->currentLayerID);
		if ((behaivour1 & 0xFF) != 0x20) {
			speed.x = 0;
			this->x_speed_inc = 0;
			doStateChange(&StateID_Wait);
		}
	} else {
		this->speed.x = 0;
		this->x_speed_inc = 0;
	}
	bool ret = this->calculateTileCollisions(true);                                 //calculate the tiles collision
	
	//this->animation();
}

void daEnUnizoo_c::endState_WaterFalling() {}