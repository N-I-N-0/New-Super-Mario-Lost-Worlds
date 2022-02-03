#include <common.h>
#include <game.h>
#include <profile.h>


class gameStateEvent : public dStageActor_c {
public:
	u8 targetEvent;
	bool toggelEvent;
	bool useTriggerEvent;
	u8 type;

	bool starcoin1;
	bool starcoin2;
	bool starcoin3;
	u8 world;
	u8 level;

	bool exitOrCheckpointTaken;
	bool secretExit;

	bool hardmodeHasToBeOff;

	bool useGeqInsteadOfLeq;
	int time;
	u8 livesType; //0=avg, 1=min, 2=max
	u8 lives;

	void tryToTrigger();

	int onCreate();
	int onExecute();

	static dActor_c *build();
};

const char *GameStateEventFileList[] = {0};
const SpriteData gameStateEventSpriteData = { ProfileId::GameStateEvent, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile gameStateEventProfile(&gameStateEvent::build, SpriteId::GameStateEvent, &gameStateEventSpriteData, ProfileId::GameStateEvent, ProfileId::GameStateEvent, "GameStateEvent", GameStateEventFileList);


dActor_c  *gameStateEvent::build() {
	void *buffer = AllocFromGameHeap1(sizeof(gameStateEvent));
	gameStateEvent *c = new(buffer) gameStateEvent;

	return c;
}


int gameStateEvent::onCreate() {
	this->targetEvent = (this->settings & 0b111111) - 1;
	this->toggelEvent = this->settings >> 6 & 0b1;
	this->useTriggerEvent = this->settings >> 7 & 0b1;

	this->type = this->settings >> 28 & 0xF;

	switch(this->type) {
		case 0:
			this->starcoin1 = this->settings >> 27 & 0b1;
			this->starcoin2 = this->settings >> 26 & 0b1;
			this->starcoin3 = this->settings >> 25 & 0b1;
			this->world = this->settings >> 20 & 0xF;
			this->level = this->settings >> 14 & 0b111111;
			break;
		case 1:
			this->exitOrCheckpointTaken = this->settings >> 27 & 0b1;
			this->secretExit = this->settings >> 26 & 0b1;
			this->world = this->settings >> 20 & 0xF;
			this->level = this->settings >> 14 & 0b111111;
			break;
		case 2:
			this->exitOrCheckpointTaken = this->settings >> 27 & 0b1;
			break;
		case 3:
			this->hardmodeHasToBeOff = this->settings >> 27 & 0b1;
			break;
		case 4:
			this->useGeqInsteadOfLeq = this->settings >> 27 & 0b1;
			this->time = this->settings >> 17 & 0b1111111111;
			break;
		case 5:
			this->useGeqInsteadOfLeq = this->settings >> 27 & 0b1;
			this->livesType = this->settings >> 25 & 0b11;
			this->lives = this->settings >> 18 & 0b1111111;
			break;
		default:
			OSReport("Wrong sprite settings!\n");
	}

	OSReport("useTriggerEvent: %s\n", useTriggerEvent ? "true" : "false");
	OSReport("toggelEvent: %s\n", toggelEvent ? "true" : "false");
	OSReport("targetEvent: %d\n", targetEvent);
	OSReport("type: %d\n", type);
	OSReport("starcoin1: %s\n", starcoin1 ? "true" : "false");
	OSReport("starcoin2: %s\n", starcoin2 ? "true" : "false");
	OSReport("starcoin3: %s\n", starcoin3 ? "true" : "false");
	OSReport("world: %d\n", type);
	OSReport("level: %d\n", type);
	OSReport("exitOrCheckpointTaken: %s\n", exitOrCheckpointTaken ? "true" : "false");
	OSReport("secretExit: %s\n", secretExit ? "true" : "false");
	OSReport("hardmodeHasToBeOff: %s\n", hardmodeHasToBeOff ? "true" : "false");
	OSReport("useGeqInsteadOfLeq: %s\n", useGeqInsteadOfLeq ? "true" : "false");
	OSReport("time: %d\n", time);
	OSReport("livesType: %d\n", livesType);
	OSReport("lives: %d\n\n", lives);

	this->onExecute();
	return true;
}


int gameStateEvent::onExecute() {
	if(this->useTriggerEvent) {
		bool flagOn = ((dFlagMgr_c::instance->flags & spriteFlagMask) != 0);
		OSReport("Is flag on: %s\n", flagOn ? "true" : "false");
		if(flagOn) {
			this->tryToTrigger();
			dFlagMgr_c::instance->flags &= ~spriteFlagMask;		//deactivate triggering event after being done!
		}
	} else {
		this->tryToTrigger();
		this->Delete(1);
	}

	return true;
}

extern bool enableHardMode;
extern u32 GameTimerB;
void gameStateEvent::tryToTrigger() {
	bool doTrigger = false;
	SaveBlock *save = GetSaveFile()->GetBlock(-1);
	float actualLives = .0f;
	u8 count = 0;

	switch(this->type) {
		case 0:
			if(world < 10)
				if(level < 42)
					if(save->CheckLevelCondition(world, level, COND_COIN1) == starcoin1)
						if(save->CheckLevelCondition(world, level, COND_COIN2) == starcoin2)
							if(save->CheckLevelCondition(world, level, COND_COIN3) == starcoin3)
								doTrigger = true;
			break;
		case 1:
			if(world < 10)
				if(level < 42)
					if(save->CheckLevelCondition(world, level, secretExit ? COND_SECRET : COND_NORMAL) == exitOrCheckpointTaken)
						doTrigger = true;
			break;
		case 2:
			if((GameMgrP->eight.checkpointEntranceID != 255 && GameMgrP->eight.checkpointWorldID == LastLevelPlayed[0] && GameMgrP->eight.checkpointLevelID == LastLevelPlayed[1]) == exitOrCheckpointTaken)
				doTrigger = true;
			break;
		case 3:
			if(enableHardMode != hardmodeHasToBeOff)
				doTrigger = true;
			break;
		case 4:
			if(useGeqInsteadOfLeq)
				if(time*4096 >= GameTimerB)
					doTrigger = true;
			else
				if(time*4096 <= GameTimerB)
					doTrigger = true;

			break;
		case 5:
			for (int i = 0; i < 4; i++) {
				if (Player_Active[i] != 0) {

					if(livesType == 0)
						actualLives += Player_Lives[Player_ID[i]];
					else if(livesType == 1)
						if(actualLives > Player_Lives[Player_ID[i]])
							actualLives = Player_Lives[Player_ID[i]];
					else
						if(actualLives < Player_Lives[Player_ID[i]])
							actualLives = Player_Lives[Player_ID[i]];

					count++;
				}
			}
			if(livesType == 0)
				actualLives /= count;

			if(useGeqInsteadOfLeq)
				if(lives >= actualLives)
					doTrigger = true;
			else
				if(lives <= actualLives)
					doTrigger = true;
			break;
		default:
			this->Delete(1);
	}

	if(doTrigger) {
		if(this->toggelEvent) {
			dFlagMgr_c::instance->set(this->targetEvent, 0, !dFlagMgr_c::instance->active(this->targetEvent), false, false);
		} else {
			dFlagMgr_c::instance->set(this->targetEvent , 0, true, false, false);
		}
	}
}