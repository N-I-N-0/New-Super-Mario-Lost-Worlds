#include <game.h>
#include <common.h>
#include "levelinfo.h"

bool isFastFoe;

class daObjCenter_c : public dStageActor_c {
public:
	u16 field_392;
	u16 offset;
	u16 rotDisplacement;
	u16 swingArc;
	u8 isReversed;
	u8 counter;
	u8 rotationID;

	u8 unk[2];

	u8 field_39F;

	int onExecute_orig();
	int newOnExecute();
};

int daObjCenter_c::newOnExecute() {
	int orig_val = this->onExecute_orig();

	// OSReport("CenterState: %s\n", this->acState.getCurrentState()->getName());
	// if(this->settings == 0x20511003 || this->settings == 0x20D11003) {
	// 	OSReport("-------\nSets: %08X\n", this->settings);
	// 	OSReport("field_392: %d\n", this->field_392);
	// 	OSReport("offset: %X\n", this->offset);
	// 	OSReport("rotDisplacement: %d\n", this->rotDisplacement);
	// 	OSReport("swingArc: %d\n", this->swingArc);
	// 	OSReport("isReversed: %d\n", this->isReversed);
	// 	OSReport("counter: %d\n", this->counter);
	// 	OSReport("rotationID: %d\n", this->rotationID);
	// 	OSReport("unk1: %d\n", this->unk[0]);
	// 	OSReport("unk2: %d\n", this->unk[1]);
	// 	OSReport("field_39F: %d\n", this->field_39F);
	// }

	return orig_val;
}

extern bool isFastFoe;
u16 globalWhitelist[] =  {51, 52, 54, 55, 56, // Goomba, Paragoomba, Koopa, Parakoopa, Buzzy Beetle
							87, 57, 58, 163, // Spike Top, Spiny, UD Spiny, Dry Bones
							241, 242, 243, 244, 245, 246, 247, 21, // Koopalings & Controller
							270, 271, 272, 273, 268, 255, 267, 24, // CKoopalings & CController
							578, 579, // Moving Pipe Up, Moving Pipe Down
							470, 471, 556, // Bill Blaster, Banzai Bill Blaser, Rising/Falling Bill Blaster
							127, 128, 129, 130, // Bullet Bill, Red Bullet Bill, Banzai Bill, Red Banzai Bill
							80, // Podoboo
							91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, // Piranha Plants
							235, // Checkpoint
							457, 432, 459, 473, // Vertical Moving Platform, Horizontal Moving Platform, Fall-when-on Platform, LineC Platform
						};
// u16 xWhitelist[0] = {};
// u16 yWhitelist[0] = {};

#define whitelistLen (sizeof(globalWhitelist) / 2)

bool isEnemyAllowedToBeFast(dEn_c *ptr, bool isY) {
	if(!isFastFoe)
		return false;

	for(int i = 0; i < whitelistLen; i++) {
		if(ptr->name == globalWhitelist[i])
			return true;
	}

	// if(!isY) {
	// 	for(int i = 0; i < 1; i++) {
	// 		if(ptr->name == xWhitelist[i])
	// 			return true;
	// 	}
	// }
	// else {
	// 	for(int i = 0; i < 1; i++) {
	// 		if(ptr->name == yWhitelist[i])
	// 			return true;
	// 	}
	// }

	return false;
}