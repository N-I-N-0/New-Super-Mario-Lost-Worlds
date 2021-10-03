#include <game.h>
#include "path.h"

const char* SpawnerFileList[] = { 0 };

class dPathSpawner_c;
class dSpawnerDataBank_c;

class dActorSpawner_c : public dStageActor_c {
public:
    static dActor_c *build(); // Method to allocate memory for the actor.

    int onCreate(); // Called once when the actor is created.
    int onExecute(); // Called every frame that the actor is in existence.

    bool newActorIsAlive();

    bool ranOnce;
	
    bool automaticRespawn; 
    bool despawnWithoutEvent;
    bool saveDespawnLocation;
    bool doMultiSpawning;
	u32 actorID;
	u32 dbSettings;
	u8 dbEventId2;
	u8 dbEventId1;
	u16 dbSpriteFlagNum;
	u8 useLocation;
	u8 spawnerID;
	dPathSpawner_c* pathController;

	dStageActor_c* newActor;
    u16 spawnedId;
};

class dSpawnerDataBank_c : public dStageActor_c {
public:
    static dActor_c *build(); // Method to allocate memory for the actor.

    int onCreate(); // Called once when the actor is created.
    int onExecute(); // Called every frame that the actor is in existence.
};

class dPathSpawner_c : public dPath_c {
public:
    static dActor_c *build(); // Method to allocate memory for the actor.

    int onCreate(); // Called once when the actor is created.
    int onExecute(); // Called every frame that the actor is in existence.

	int spawnerID;
};

const SpriteData NewActorSpawnerSpriteData = { ProfileId::NewActorSpawner, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile NewActorSpawnerProfile(&dActorSpawner_c::build, SpriteId::NewActorSpawner, NewActorSpawnerSpriteData, ProfileId::NewActorSpawner, ProfileId::NewActorSpawner, "NewActorSpawner", SpawnerFileList);


dActor_c* dActorSpawner_c::build() {
    void* buffer = AllocFromGameHeap1(sizeof(dActorSpawner_c));
    return new(buffer) dActorSpawner_c;
}

int dActorSpawner_c::onCreate() {
    // Return the first time in order to get the values of eventId1 and eventId2.
    if (this->ranOnce == false) {
        this->ranOnce = true;
        return false; // Retry onCreate().
    }
	
	this->spawnerID = this->eventId1;

	dSpawnerDataBank_c* dataBank = 0;
	while(dataBank = (dSpawnerDataBank_c*)fBase_c::search(NewActorSpawnerDataBank, dataBank)) {
		OSReport("Found Data Bank with ID: %d\n", dataBank->currentLayerID);
		if(dataBank->currentLayerID == this->spawnerID) {
			break;
		}
	}
	if(dataBank) {
		this->dbSettings = dataBank->settings;
		this->dbEventId1 = dataBank->eventId1;
		this->dbEventId2 = dataBank->eventId2;
		this->dbSpriteFlagNum = dataBank->spriteFlagNum;
		OSReport("Actor Spawner Data Bank found! Settings copied, deleting Data Bank!\n");
		dataBank->Delete(1);
	} else {
		OSReport("No Actor Spawner Data Bank found for Spawner ID: %d\nDeleting Actor Spawner!\n", this->spawnerID);
		this->Delete(1);
	}

	pathController = 0;
	while(pathController = (dPathSpawner_c*)fBase_c::search(NewActorSpawnerPath, (fBase_c*)pathController)) {
		if(pathController->spawnerID == this->spawnerID) {
			break;
		}
	}
	if(pathController) {
		OSReport("Actor Spawner Path Controller found!\n");
	} else {
		OSReport("No Actor Spawner Path Controller found for Spawner ID: %d\n", this->spawnerID);
	}
	
	doMultiSpawning = this->settings >> 16 & 1;
	saveDespawnLocation = this->settings >> 17 & 1;
	despawnWithoutEvent = this->settings >> 18 & 1;
	automaticRespawn = this->settings >> 19 & 1;
	
	actorID = this->settings >> 20;
	
	useLocation = this->settings & 0xFF;
	
    return true;
}

int dActorSpawner_c::onExecute() {
	if(doMultiSpawning) {
		if((dFlagMgr_c::instance->flags & (1ULL << (eventId2 - 1))) != 0) {
			Vec spawnPos = this->pos;
			if(useLocation) {
				mRect rect;
				dCourseFull_c::instance->get(GetAreaNum())->getRectByID(useLocation, &rect);

				// Round the positions down/up to get the rectangle
				int left = rect.x;
				//int right = left + rect.width;
				int top = -rect.y;
				int bottom = top + rect.height;

				spawnPos.x = GenerateRandomNumber(rect.width) + left;
				spawnPos.y = GenerateRandomNumber(rect.height) + bottom; 
			}

			dStageActor_c *ac = dStageActor_c::create((Actors)actorID, dbSettings, &spawnPos, 0, this->currentLayerID);
			
			ac->spriteFlagNum = dbSpriteFlagNum;
			ac->eventId1 = dbEventId1;
			ac->eventId2 = dbEventId2;
		}
	} else {
		if((dFlagMgr_c::instance->flags & (1ULL << (eventId2 - 1))) != 0) {
			if(!newActor) {
				newActor = dStageActor_c::create((Actors)actorID, dbSettings, &pos, 0, this->currentLayerID);
				if(pathController) {
					pathController->acState.setState(&dPath_c::StateID_Init);
				}
			} else {
				if(pathController) {
					newActor->pos = pathController->pos;
				}
			}
		} else {
			if(newActor) {
				newActor->Delete(1);
				newActor = 0;
			}
		}
	}



    return true;
}

bool dActorSpawner_c::newActorIsAlive() {
    // Return true if the search doesn't return 0; the actor is alive.
    return true; //fBase_c::search(this->newActor->id) != 0;
}











const SpriteData NewActorSpawnerDataBankSpriteData = { ProfileId::NewActorSpawnerDataBank, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile NewActorSpawnerDataBankProfile(&dSpawnerDataBank_c::build, SpriteId::NewActorSpawnerDataBank, NewActorSpawnerDataBankSpriteData, ProfileId::NewActorSpawnerDataBank, ProfileId::NewActorSpawnerDataBank, "NewActorSpawnerDataBank", SpawnerFileList);


dActor_c* dSpawnerDataBank_c::build() {
    void* buffer = AllocFromGameHeap1(sizeof(dSpawnerDataBank_c));
    return new(buffer) dSpawnerDataBank_c;
}


int dSpawnerDataBank_c::onCreate() {
	//this->currentLayerID; use the layer ID as the spawnerID
    return true;
}

int dSpawnerDataBank_c::onExecute() {
    return true;
}













const SpriteData NewActorSpawnerPathSpriteData = { ProfileId::NewActorSpawnerPath, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile NewActorSpawnerPathProfile(&dPathSpawner_c::build, SpriteId::NewActorSpawnerPath, NewActorSpawnerPathSpriteData, ProfileId::NewActorSpawnerPath, ProfileId::NewActorSpawnerPath, "NewActorSpawnerPath", SpawnerFileList);


dActor_c* dPathSpawner_c::build() {
    void* buffer = AllocFromGameHeap1(sizeof(dPathSpawner_c));
    return new(buffer) dPathSpawner_c;
}


int dPathSpawner_c::onCreate() {
	this->spawnerID = this->settings >> 20 & 0xFF;
	OSReport("Actor Spawner Path Controller: %d\n", this->spawnerID);
	
    return true;
}

int dPathSpawner_c::onExecute() {
	acState.execute();

    return true;
}














