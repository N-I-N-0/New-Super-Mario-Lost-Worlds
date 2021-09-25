#include <common.h>
#include <game.h>
#include <profile.h>


class randomLooperEvent : public dStageActor_c {
public:
	u8 minDelayActivate, maxDelayActivate, minDelayDeactivate, maxDelayDeactivate, repeated;
	int timer;
	bool active;

	void changeTriggerEventState();

	int onCreate();
	int onExecute();

	static dActor_c *build();
};

const char *RandomLooperEventFileList[] = {0};
const SpriteData randomLooperEventSpriteData = { ProfileId::RandomLooperEvent, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile randomLooperEventProfile(&randomLooperEvent::build, SpriteId::RandomLooperEvent, randomLooperEventSpriteData, ProfileId::RandomLooperEvent, ProfileId::RandomLooperEvent, "RandomLooperEvent", RandomLooperEventFileList);


dActor_c  *randomLooperEvent::build() {
	void *buffer = AllocFromGameHeap1(sizeof(randomLooperEvent));
	randomLooperEvent *c = new(buffer) randomLooperEvent;

	return c;
}


/*
target event id
trigger event id
min delay
max delay
min delay2
max delay2
repeat x times
*/

int randomLooperEvent::onCreate() {
	this->minDelayActivate = (this->settings & 0xFF);
	this->maxDelayActivate = (this->settings >> 8 & 0xFF);
	this->minDelayDeactivate = (this->settings >> 16 & 0xFF);
	this->maxDelayDeactivate = (this->settings >> 24 & 0xFF);

	this->timer = GenerateRandomNumber(this->maxDelayDeactivate - this->minDelayDeactivate) + this->minDelayDeactivate;;
	this->repeated = 0;

	return true;
}


int randomLooperEvent::onExecute() {
	if(this->currentLayerID != 0) {
		repeated++;
		if(this->currentLayerID > repeated) {
			this->Delete(1);
		}
	}
	
	bool flagOn = ((dFlagMgr_c::instance->flags & (1ULL << (eventId2 - 1))) != 0);
	//OSReport("Is flag on: %s\n", flagOn ? "true" : "false");
	if(flagOn) {
		if(this->timer <= 0) {
			if(active) {
				timer = GenerateRandomNumber(this->maxDelayDeactivate - this->minDelayDeactivate) + this->minDelayDeactivate;
			} else {
				timer = GenerateRandomNumber(this->maxDelayActivate - this->minDelayActivate) + this->minDelayActivate;
			}
			active = !active;

			this->changeTriggerEventState();
		} else {
			timer--;
		}
	}
	return true;
}


void randomLooperEvent::changeTriggerEventState() {
	dFlagMgr_c::instance->set(this->eventId1, 0, !dFlagMgr_c::instance->active(this->eventId1), false, false);
}