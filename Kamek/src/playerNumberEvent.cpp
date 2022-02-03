#include <common.h>
#include <game.h>
#include <profile.h>


class playerNumberEvent : public dStageActor_c {
public:
	u8 eventNums[4];

	bool useTriggerEvent;
	bool repeat;
	bool toggelEvent;
	int interval;
	int timer;

	void tryToTrigger();

	int onCreate();
	int onExecute();

	static dActor_c *build();
};

const char *PlayerNumberEventFileList[] = {0};
const SpriteData playerNumberEventSpriteData = { ProfileId::PlayerNumberEvent, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile playerNumberEventProfile(&playerNumberEvent::build, SpriteId::PlayerNumberEvent, &playerNumberEventSpriteData, ProfileId::PlayerNumberEvent, ProfileId::PlayerNumberEvent, "PlayerNumberEvent", PlayerNumberEventFileList);


dActor_c  *playerNumberEvent::build() {
	void *buffer = AllocFromGameHeap1(sizeof(playerNumberEvent));
	playerNumberEvent *c = new(buffer) playerNumberEvent;

	return c;
}


int playerNumberEvent::onCreate() {
	this->eventNums[0] = (this->settings >> 26 & 0b111111) - 1;
	this->eventNums[1] = (this->settings >> 20 & 0b111111) - 1;
	this->eventNums[2] = (this->settings >> 14 & 0b111111) - 1;
	this->eventNums[3] = (this->settings >> 8 & 0b111111) - 1;

	this->useTriggerEvent = this->settings >> 7 & 0b1;
	this->repeat = this->settings >> 6 & 0b1;
	this->toggelEvent = this->settings >> 5 & 0b1;
	this->interval = (this->settings & 0b11111) + 1;
	this->timer = 0;

	/*OSReport("useTriggerEvent: %s\n", useTriggerEvent ? "true" : "false");
	OSReport("repeat: %s\n", repeat ? "true" : "false");
	OSReport("toggelEvent: %s\n", toggelEvent ? "true" : "false");
	OSReport("interval: %d\n", interval);
	OSReport("eventNums: %d\n", eventNums[0]);
	OSReport("eventNums: %d\n", eventNums[1]);
	OSReport("eventNums: %d\n", eventNums[2]);
	OSReport("eventNums: %d\n", eventNums[3]);*/

	this->onExecute();
	return true;
}


int playerNumberEvent::onExecute() {
	//OSReport("spriteFlagArray: %s\n", this->spriteFlagArray[5] ? "true" : "false");
	
	//OSReport("playerNumber: %d\n", GetActivePlayerCount());
	if(this->timer == 0) {
		if(this->useTriggerEvent) {
			bool flagOn = ((dFlagMgr_c::instance->flags & spriteFlagMask) != 0);
			//OSReport("Is flag on: %s\n", flagOn ? "true" : "false");
			if(flagOn) {
				this->tryToTrigger();
				if(!this->repeat) {
					dFlagMgr_c::instance->flags &= ~spriteFlagMask;		//deactivate triggering event after being done!
					return true;
				}
			} else {
				return true;
			}
		} else {
			this->tryToTrigger();
			if(!this->repeat) {
				this->Delete(1);
				return true;
			}
		}
		this->timer = this->interval*60;
	} else {
		this->timer--;
	}

	return true;
}


void playerNumberEvent::tryToTrigger() {
	//OSReport("tryToTrigger: %d\n", eventNums[GetActivePlayerCount()-1]);
	if(this->toggelEvent) {
		dFlagMgr_c::instance->set(eventNums[GetActivePlayerCount()-1], 0, !dFlagMgr_c::instance->active(eventNums[GetActivePlayerCount()-1]), false, false);
	} else {
		dFlagMgr_c::instance->set(eventNums[GetActivePlayerCount()-1], 0, true, false, false);
	}
}