#include <common.h>
#include <game.h>
#include <profile.h>


bool isActivePhysicInLocation(ActivePhysics* aPhysics, u8 rectID) {
	mRect rect;
	dCourseFull_c::instance->get(GetAreaNum())->getRectByID(rectID, &rect);
	
	return aPhysics->left() < rect.x + rect.width &&
           rect.x < aPhysics->right()             &&
           aPhysics->bottom() < rect.y            &&
           rect.y - rect.height < aPhysics->top();
}



class playerLocationEvent_c : public dStageActor_c {
public:
	u8 eventNums[4];
	bool toggelEvent;
	u8 rectID;

	void tryToTrigger(u8 player);
	bool isPlayerInLocation(dAc_Py_c* player);

	int onCreate();
	int onExecute();

	static dActor_c *build();
};

const char *PlayerLocationEventFileList[] = {0};
const SpriteData PlayerLocationEventSpriteData = { ProfileId::PlayerLocationEvent, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile PlayerLocationEventProfile(&playerLocationEvent_c::build, SpriteId::PlayerLocationEvent, &PlayerLocationEventSpriteData, ProfileId::PlayerLocationEvent, ProfileId::PlayerLocationEvent, "PlayerLocationEvent", PlayerLocationEventFileList);


dActor_c  *playerLocationEvent_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(playerLocationEvent_c));
	playerLocationEvent_c *c = new(buffer) playerLocationEvent_c;

	return c;
}


int playerLocationEvent_c::onCreate() {
	this->eventNums[0] = (this->settings >> 26 & 0b111111) - 1;
	this->eventNums[1] = (this->settings >> 20 & 0b111111) - 1;
	this->eventNums[2] = (this->settings >> 14 & 0b111111) - 1;
	this->eventNums[3] = (this->settings >> 8 & 0b111111) - 1;
	this->toggelEvent = this->settings >> 7 & 0b1;
	this->rectID = (this->settings & 0b11111);
	
	this->onExecute();
	return true;
}


bool playerLocationEvent_c::isPlayerInLocation(dAc_Py_c* player) {
	bool isInLocation = false;
	if (player->aPhysics.isLinkedIntoList) {
		if(isActivePhysicInLocation(&player->aPhysics, this->rectID)) {
			isInLocation = true;
		}
	}
	if (player->bPhysics.isLinkedIntoList) {
		if(isActivePhysicInLocation(&player->bPhysics, this->rectID)) {
			isInLocation = true;
		}
	}
	if (player->cPhysics.isLinkedIntoList) {
		if(isActivePhysicInLocation(&player->cPhysics, this->rectID)) {
			isInLocation = true;
		}
	}
	if (player->dPhysics.isLinkedIntoList) {
		if(isActivePhysicInLocation(&player->dPhysics, this->rectID)) {
			isInLocation = true;
		}
	}
	if (player->ePhysics.isLinkedIntoList) {
		if(isActivePhysicInLocation(&player->ePhysics, this->rectID)) {
			isInLocation = true;
		}
	}
	return isInLocation;
}



int playerLocationEvent_c::onExecute() {
	for (int i = 0; i < 4; i++) {
		if (dAc_Py_c *player = (dAc_Py_c*)dAcPy_c::findByID(i)) {
			if(this->isPlayerInLocation(player)) {
				this->tryToTrigger(i);
			}
		}
	}

	return true;
}


void playerLocationEvent_c::tryToTrigger(u8 player) {
	if(this->toggelEvent) {
		dFlagMgr_c::instance->set(eventNums[player], 0, !dFlagMgr_c::instance->active(eventNums[player]), false, false);
	} else {
		dFlagMgr_c::instance->set(eventNums[player], 0, true, false, false);
	}
}