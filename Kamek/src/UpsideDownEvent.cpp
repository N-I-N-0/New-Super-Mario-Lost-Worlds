#include <common.h>
#include <game.h>
#include <profile.h>


class upsideDownEvent : public dStageActor_c {
public:
	bool deactivateInsteadOfActivate;
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

const char *UpsideDownEventFileList[] = {0};
const SpriteData upsideDownEventSpriteData = { ProfileId::UpsideDownEvent, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile upsideDownEventProfile(&upsideDownEvent::build, SpriteId::UpsideDownEvent, &upsideDownEventSpriteData, ProfileId::UpsideDownEvent, ProfileId::UpsideDownEvent, "UpsideDownEvent", UpsideDownEventFileList);


dActor_c  *upsideDownEvent::build() {
	void *buffer = AllocFromGameHeap1(sizeof(upsideDownEvent));
	upsideDownEvent *c = new(buffer) upsideDownEvent;

	return c;
}


int upsideDownEvent::onCreate() {
	this->deactivateInsteadOfActivate = this->settings >> 8 & 0b1;		//0 activate, 1 deactivate
	this->useTriggerEvent = this->settings >> 7 & 0b1;
	this->repeat = this->settings >> 6 & 0b1;
	this->toggelEvent = this->settings >> 5 & 0b1;
	this->interval = (this->settings & 0b11111) + 1;
	this->timer = 0;

	OSReport("useTriggerEvent: %s\n", useTriggerEvent ? "true" : "false");
	OSReport("repeat: %s\n", repeat ? "true" : "false");
	OSReport("toggelEvent: %s\n", toggelEvent ? "true" : "false");
	OSReport("interval: %d\n", interval);
	OSReport("deactivateInsteadOfActivate: %s\n", deactivateInsteadOfActivate ? "true" : "false");

	this->onExecute();
	return true;
}


int upsideDownEvent::onExecute() {
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

int IsGravityEnabled2 = 0;
extern int IsGravityEnabled;
void upsideDownEvent::tryToTrigger() {
	//OSReport("tryToTrigger: %d\n", eventNums[GetActivePlayerCount()-1]);
	if(this->toggelEvent) {
		if(IsGravityEnabled == 0)
			IsGravityEnabled = 16;
		else
			IsGravityEnabled = 0;
	} else {
		if(deactivateInsteadOfActivate)
			IsGravityEnabled = 0;
		else
			IsGravityEnabled = 16;
	}
}