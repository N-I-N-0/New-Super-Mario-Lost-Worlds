#include "courseSelectManager.h"

CREATE_STATE(dCourseSelectManager_c, ShopWait);
CREATE_STATE(dCourseSelectManager_c, CreatorInfoWait);

void dCourseSelectManager_c::beginState_ShopWait() {
	MapSoundPlayer(SoundRelatedClass, 0x79, 1);

	this->sub_80931170();
	this->sub_809310F0();

	this->shopPtr->visible = true;
	this->shopPtr->showShop = true;
}

void dCourseSelectManager_c::executeState_ShopWait() {
	if(!this->shopPtr->visible) {
		state.setState(&StateID_KeyWait);
	}
}

void dCourseSelectManager_c::endState_ShopWait() {
	this->doesSomethingWithShop = 0;
	this->sub_80931090();
	this->sub_80931110();
}

void dCourseSelectManager_c::beginState_CreatorInfoWait() {
	MapSoundPlayer(SoundRelatedClass, 0x79, 1);

	this->sub_80931170();
	this->sub_809310F0();

	this->creatorInfoPtr->visible = true;
	this->creatorInfoPtr->showLevelCredits = true;
}

void dCourseSelectManager_c::executeState_CreatorInfoWait() {
	if(!this->creatorInfoPtr->visible) {
		state.setState(&StateID_KeyWait);
	}
}

void dCourseSelectManager_c::endState_CreatorInfoWait() {
	this->doesSomethingWithCreatorInfo = 0;
	this->sub_80931090();
	this->sub_80931110();
}


void CheckForButtonPress() {
	Remocon* rem = GetActiveRemocon();

	if(rem->isShaking) {
		dCourseSelectManager_c::instance->doesSomethingWithCreatorInfo = 1;
	}
	else {
		if(rem->controllerType == 0) {	//Wiimote
			if(rem->nowPressed & WPAD_B) {
				dCourseSelectManager_c::instance->doesSomethingWithShop = 1;
			}
		}
		else {						// == 1 <-> Wiimote + Nunchuck
			if(rem->nowPressed & 0x2000) {
				dCourseSelectManager_c::instance->doesSomethingWithShop = 1;
			}
		}
	}
}
