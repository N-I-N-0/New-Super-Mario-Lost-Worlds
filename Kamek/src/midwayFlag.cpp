#include <common.h>
#include <game.h>
#include <dCourse.h>
#include <g3dhax.h>
#include <sfx.h>
#include <stage.h>
#include "midwayFlag.h"

extern "C" bool midwayFlagOnCreate(daChukanPoint_c* self); // 0x807e2130
extern "C" int midwayFlagOnExecute(daChukanPoint_c* self); // 0x807e24c0
// extern "C" void stuffRelatingToCollisions(daChukanPoint_c* self, float, float, float); // 0x800957B0
extern "C" void doSpriteMovement(daChukanPoint_c* self); // 0x800955F0
extern "C" unsigned char checkWater(float x, float y, u8 layer, float *water_height); // 0x80075274 | 0 = None, 1 = Water, 2 = ???, 3 = Lava, 4 = Poison



void midwayFlagCollisionCallback(ActivePhysics *one, ActivePhysics *two) {
	// OSReport("Collision callback called! %s %s\n", one->owner->name_string, two->owner->name_string);
	daChukanPoint_c* self = (daChukanPoint_c*)one->owner;
	dStageActor_c* other = (dStageActor_c*)two->owner;

	switch (other->name) {
		case EN_JUMPDAI:
			if (!self->enableGravity) break;

			u8* jumpdaiTimer = (u8*)((u32)(other) + 0x632);

			if(*jumpdaiTimer == 0 && self->speed.y < 0.0f) {
				*jumpdaiTimer = 0x11;
				PlaySound(self, SE_PLY_JUMPDAI);
			}
			else if(*jumpdaiTimer == 8) {
				if (!self->isInLiquid()) self->speed.y = 6.0f;
				else self->speed.y = 5.0f;
			}
			break;
	}

	self->activePhysicsCallback(one, two);
}


bool midwayFlagNewOnCreate(daChukanPoint_c* self) {
    int color = self->settings >> 24 & 0xF; // Nibble 6

	int entranceID = self->settings >> 16 & 0xFF; // Nibble 7-8

	int nybble5 = self->settings >> 28 & 0xF; // Nibble 5
	bool loadFacingLeft = nybble5 & 0b1000;
	bool secondCheckpoint = nybble5 & 0b100;
	self->enableGravity = nybble5 & 0b10;

	int rotation = self->settings & 0xFFFF;

    self->settings = color << 24 | entranceID << 16 | loadFacingLeft << 4 | secondCheckpoint; // Needed more bits so I need to do this

    bool ret = midwayFlagOnCreate(self);
	self->activePhysicsCallback = self->aPhysics.info.callback;
	self->aPhysics.info.callback = midwayFlagCollisionCallback;
	self->aPhysics.info.category1 = 0x3;
	self->aPhysics.info.category2 = 0x0;
	self->aPhysics.info.bitfield1 = 0x4F;
	// self->aPhysics.info.bitfield2 = 0xffbafffe;
	self->aPhysics.info.unkShort1C = 0;


	// Rotation stuff

	// rotation / 0xF * 0xFFFF == rotation * 0x1111
	// BUT 0x1111 doesn't work with some high values for some reason so I'm using 0xFFF
	self->rot.x = (float)(rotation); // X is vertical axis (0xFFF ~= 22.5 degrees)
	self->rot.y = 0xBFF4; // Y is horizontal axis
	self->rot.z = 0; // Z is ... an axis >.>
	self->direction = 1; // Heading left.

	// Can't rotate active physics sadly :(
	
	self->max_speed.x = 0;
	self->speed.x = 0;
    self->liquid = 0;

	if (self->enableGravity) {
		self->max_speed.y = -4.0;
		self->speed.y = -4.0;
		self->y_speed_inc = -0.1875;
	} else {
		self->max_speed.y = 0.0;
		self->speed.y = 0.0;
		self->y_speed_inc = 0.0;
	}

	self->acState.setState(&daChukanPoint_c::StateID_Wait);

	Vec2 rotationBalance;
	float angle = ((rotation / (float)(0xFFFF)) * 360.0) - 90.0;
	if (angle < 0.0) angle += 360.0;
	rotationBalance.x = cos(angle * M_PI / 180.0);
	rotationBalance.y = sin(angle * M_PI / 180.0);

	self->pos.y += 8.0;


	// Physics stuff

	Vec2 aPhysicsCenter = (Vec2){0.0, 0.0};
	aPhysicsCenter.x = -24.0 * rotationBalance.x;
	aPhysicsCenter.y = -24.0 * rotationBalance.y;

	Vec2 aPhysicsEdge = (Vec2){0.0, 0.0};
	aPhysicsEdge.x = 4.0 + abs(20.0 * rotationBalance.x);
	aPhysicsEdge.y = 4.0 + abs(20.0 * rotationBalance.y);

	self->aPhysics.info.xDistToCenter = (float)aPhysicsCenter.x;
	self->aPhysics.info.yDistToCenter = (float)aPhysicsCenter.y;
	self->aPhysics.info.xDistToEdge = (float)aPhysicsEdge.x;
	self->aPhysics.info.yDistToEdge = (float)aPhysicsEdge.y;


	// Tile collider

	// These fucking rects do something for the tile rect
	// spriteSomeRectX = 28.0f;
	// spriteSomeRectY = 32.0f;
	self->_320 = 0.0f;
	self->_324 = 16.0f;

	static const pointSensor_s below(0<<12, 0<<12);
	static const pointSensor_s above(0<<12, 12<<12);
	static const lineSensor_s adjacent(6<<12, 9<<12, 8<<12);

	self->collMgr.init(self, &below, &above, &adjacent);
	self->collMgr.calculateBelowCollisionWithSmokeEffect();

	bool cmgr_returnValue = self->collMgr.isOnTopOfTile();

	if (self->collMgr.isOnTopOfTile())
		self->isBouncing = false;
	else
		self->isBouncing = true;

    return ret;
}


bool calculateTileCollisions(daChukanPoint_c* self) {
	// Returns true if sprite should turn, false if not.

	self->HandleXSpeed();
	self->HandleYSpeed();
	doSpriteMovement(self);


	bool cmgr_returnValue = self->collMgr.isOnTopOfTile();
	// OSReport(self->collMgr.sCollBelow.)
	self->collMgr.calculateBelowCollisionWithSmokeEffect();

	if (self->isBouncing) {
		// stuffRelatingToCollisions(self, 0.1875f, 1.0f, 0.5f);
		if (self->speed.y != 0.0f)
			self->isBouncing = false;
	}

	if (self->collMgr.isOnTopOfTile()) {
		// Walking into a tile branch

		if (cmgr_returnValue == 0)
			self->isBouncing = true;
			// OSReport("0x%x\n", self->collMgr.calculateBelowCollisionWithSmokeEffect());

		if (self->speed.x != 0.0f) {
			//playWmEnIronEffect();
		}

		self->speed.y = 0.0f;
	} else {
		self->x_speed_inc = 0.0f;
	}

	// Bouncing checks
	if (self->_34A & 4) {
		Vec v = (Vec){0.0f, 1.0f, 0.0f};
		self->collMgr.pSpeed = &v;

		if (self->collMgr.calculateAboveCollision(self->collMgr.outputMaybe))
			self->speed.y = 0.0f;

		self->collMgr.pSpeed = &self->speed;

	} else {
		if (self->collMgr.calculateAboveCollision(self->collMgr.outputMaybe))
			self->speed.y = 0.0f;
	}

	self->collMgr.calculateAdjacentCollision(0);

	// Switch Direction
	if (self->collMgr.outputMaybe & (0x15 << self->direction)) {
		if (self->collMgr.isOnTopOfTile()) {
			self->isBouncing = true;
		}
		return true;
	}
	return false;
}


int midwayFlagNewOnExecute(daChukanPoint_c* self) {
    if (self->enableGravity) calculateTileCollisions(self);

    u8 newLiquid = checkWater(self->pos.x, self->pos.y + 20.0f, self->currentLayerID, (float*)0x0);
    if (newLiquid != self->liquid) {
        if (newLiquid == 0) {
            if (self->acState.getCurrentState() != &daChukanPoint_c::StateID_Wait) {
                self->acState.setState(&daChukanPoint_c::StateID_Wait);
                if (self->enableGravity) {
					self->max_speed.y = -4.0;
					self->y_speed_inc = -0.1875;
				}
            }
        }
        else {
            if (self->acState.getCurrentState() != &daChukanPoint_c::StateID_SeaWait) {
                self->acState.setState(&daChukanPoint_c::StateID_SeaWait);
				if (self->enableGravity) {
					self->max_speed.y = -1.0;
					if (self->speed.y < -1.0) self->speed.y = -1.0;
					self->y_speed_inc = -0.09375;
				}
            }
        }
        self->liquid = newLiquid;
    }

    int ret = midwayFlagOnExecute(self);
    return ret;
}

bool daChukanPoint_c::isInLiquid() {
	return this->liquid != 0;
}

bool midwayFlagActivated;

bool launchStarChipCollectedBeforeFlag[32][5];      //These are here cause they get reset by one function here
bool launchStarChipCollectedAfterFlag[32][5];

void SetMidwayBoolTrue() {
	midwayFlagActivated = true;
	
	OSReport("SetMidwayTrue\n");
}

bool SetMidwayBoolTrue2(bool isActive) {
	midwayFlagActivated = isActive;
	
	for (int i = 0; i < 32; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			launchStarChipCollectedAfterFlag[i][j] = false;
		}
	}
	
	OSReport("SetMidwayTrue2\n");
	
	return isActive;
}

void SetMidwayBoolFalse() {
	midwayFlagActivated = false;
	
	for (int i = 0; i < 32; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			launchStarChipCollectedBeforeFlag[i][j] = false;
			launchStarChipCollectedAfterFlag[i][j] = false;
		}
	}
	
	OSReport("SetMidwayFalse\n");
}

