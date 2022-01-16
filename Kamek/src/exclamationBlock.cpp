#include <common.h>
#include <game.h>
#include <g3dhax.h>
#include <sfx.h>
extern "C" void PlaySoundWithFunctionB4(void *spc, nw4r::snd::SoundHandle *handle, int id, int unk);




/*
- direction
- length
- id
- sub id
- next block
*/



// Exclamation Block


class daExclamationBlock_c : public daEnBlockMain_c {
public:
	TileRenderer tile;
	TileRenderer tileList[16];
	Physics::Info physicsInfo;
	Physics physicsList[16];
	Physics::Info physicsInfoList[16];

	int z;
	//u8 direction;
	u8 maxLength;
	u8 blockId;
	u8 subId;
	u8 activatedLength;
	daExclamationBlock_c* next;

	bool ranOnce;

	int wasIHit;
	int waitUntilNextDownMove;
	
	int counter;

	int onCreate();
	int onDelete();
	int onExecute();

	void calledWhenUpMoveExecutes();
	void calledWhenDownMoveExecutes();

	void blockWasHit(bool isDown);
	
	bool activateEntirely;
	u8 activateEntirelyDelay;
	
	void activateOne();
	bool testOne();
	void deactivate();

	USING_STATES(daExclamationBlock_c);
	DECLARE_STATE(Wait);

	static dActor_c *build();
};


CREATE_STATE(daExclamationBlock_c, Wait);


void daExclamationBlock_c::activateOne() {
	if (activatedLength < maxLength) {
		TileRenderer::List *list = dBgGm_c::instance->getTileRendererList(0);
		list->add(&tileList[activatedLength]);
		physicsList[activatedLength].addToList();
		activatedLength += 1;
	} else {
		if (next) {
			next->activateOne();
		}
	}
}

void daExclamationBlock_c::deactivate() {
	TileRenderer::List *list = dBgGm_c::instance->getTileRendererList(0);
	for (int i = 0; i < activatedLength; i++) {
		list->remove(&tileList[i]);
		physicsList[i].removeFromList();
	}
	activatedLength = 0;
	if (next) {
		next->deactivate();
	}
}

bool daExclamationBlock_c::testOne() {
	if (activatedLength < maxLength) {
		return true;
	} else {
		if (next) {
			return next->testOne();
		} else {
			return false;
		}
	}
}



int daExclamationBlock_c::onCreate() {
	direction = (this->settings & 0xF) % 4;
	maxLength = this->settings >> 4 & 0xF;
	blockId = this->settings >> 8 & 0xFF;
	subId = this->settings >> 16 & 0xFF;
	
	if (!ranOnce) {
		ranOnce = true;
		return false;
	}
	this->pos.y -= 8;
	blockInit(pos.y);
	this->sub_80021740();

	for (int i = 0; i < maxLength; i++) {
		physicsInfoList[i].x1 = -8;
		physicsInfoList[i].y1 = 16;
		physicsInfoList[i].x2 = 8;
		physicsInfoList[i].y2 = 0;

		switch(direction) {
			case 0:
				physicsInfoList[i].x1 += (i+1)*16;
				physicsInfoList[i].x2 += (i+1)*16;
				break;
			case 1:
				physicsInfoList[i].y1 -= (i+1)*16;
				physicsInfoList[i].y2 -= (i+1)*16;
				break;
			case 2:
				physicsInfoList[i].x1 -= (i+1)*16;
				physicsInfoList[i].x2 -= (i+1)*16;
				break;
			case 3:
				physicsInfoList[i].y1 += (i+1)*16;
				physicsInfoList[i].y2 += (i+1)*16;
				break;
			default:
				break;			
		}

		physicsInfoList[i].otherCallback1 = (void*)&PhysCB1;
		physicsInfoList[i].otherCallback2 = (void*)&PhysCB2;
		physicsInfoList[i].otherCallback3 = (void*)&PhysCB3;

		physicsList[i].setup(this, &physicsInfoList[i], 3, currentLayerID);
		physicsList[i].flagsMaybe = 0x260;
		physicsList[i].callback1 = (void*)&PhysCB4;
		physicsList[i].callback2 = (void*)&PhysCB5;
		physicsList[i].callback3 = (void*)&PhysCB6;
	}


	TileRenderer::List *list = dBgGm_c::instance->getTileRendererList(0);
	

	
	this->wasIHit = 0.0f;
	this->pos.z = 200.0f;

	tile.x = pos.x - 8;
	tile.y = -(16 + pos.y);
	/*if((this->settings >> 24 & 0xF) == 0) {
		tile.tileNumber = 0x0; //0x0 is invisible, 0x30 is brick and 0x31 is question
	}
	if((this->settings >> 24 & 0xF) == 1) {
		tile.tileNumber = 0x30; //0x0 is invisible, 0x30 is brick and 0x31 is question
	}
	if((this->settings >> 24 & 0xF) == 2) {
		tile.tileNumber = 0x31; //0x0 is invisible, 0x30 is brick and 0x31 is question
	}*/
	
	if (this->subId == 0) {
		this->_68B = 1;
		physics._D8 &= ~0b00101000;

		tile.tileNumber = 0x31;
		list->add(&tile);
		
		physicsInfo.x1 = -8;
		physicsInfo.y1 = 16;
		physicsInfo.x2 = 8;
		physicsInfo.y2 = 0;

		physicsInfo.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
		physicsInfo.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
		physicsInfo.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

		physics.setup(this, &physicsInfo, 3, currentLayerID);
		physics.flagsMaybe = 0x260;
		physics.callback1 = &daEnBlockMain_c::PhysicsCallback1;
		physics.callback2 = &daEnBlockMain_c::PhysicsCallback2;
		physics.callback3 = &daEnBlockMain_c::PhysicsCallback3;
		physics.addToList();
	}
	
	for (int i = 0; i < maxLength; i++) {
		tileList[i].x = pos.x - 8;
		tileList[i].y = -(16 + pos.y);
		tileList[i].tileNumber = 0x32;
		
		switch(direction) {
			case 0:
				tileList[i].x += (i+1)*16;
				break;
			case 1:
				tileList[i].y += (i+1)*16;
				break;
			case 2:
				tileList[i].x -= (i+1)*16;
				break;
			case 3:
				tileList[i].y -= (i+1)*16;
				break;
			default:
				break;
		}
	}
	
	daExclamationBlock_c *blockIter = 0;
	while (blockIter = (daExclamationBlock_c*)dEn_c::search(ExclamationBlock, blockIter)) {
		if (blockIter->blockId == this->blockId) {
			if (blockIter->subId == this->subId+1) {
				OSReport("id: %d, sub: %d\n", blockIter->blockId, blockIter->subId);
				this->next = blockIter;
			}
		}
	}

	this->spriteSomeRectX = 6000.0f;

	doStateChange(&daExclamationBlock_c::StateID_Wait);

	return true;
}


int daExclamationBlock_c::onDelete() {
	TileRenderer::List *list = dBgGm_c::instance->getTileRendererList(0);
	list->remove(&tile);
	for (int i = 0; i < activatedLength; i++) {
		list->remove(&tileList[i]);
		physicsList[i].removeFromList();
	}

	physics.removeFromList();

	return true;
}


int daExclamationBlock_c::onExecute() {
	acState.execute();
	physics.update();
	blockUpdate();

	tile.setPosition(pos.x-8, -(16+pos.y), pos.z);
	tile.setVars(scale.x);

	if(subId == 0 && activateEntirely) {
		if(activateEntirelyDelay < 2) {
			activateEntirelyDelay++;
		} else {
			if(testOne()) {
				activateOne();
			} else {
				tile.tileNumber = 0x32;
				activateEntirely = false;
			}
			activateEntirelyDelay = 0;
		}
	}


	if (subId == 0 && activatedLength != 0) {		
		if (counter >= 600) {
			deactivate();
			tile.tileNumber = 0x31;
			counter = 0;
		} else {
			counter++;
		}
	} else {
		counter = 0;
	}

	if (acState.getCurrentState()->isEqual(&StateID_Wait)) {
		checkZoneBoundaries(0);
	}

	return true;
}


dActor_c *daExclamationBlock_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daExclamationBlock_c));
	return new(buffer) daExclamationBlock_c;
}

const char *ExclamationBlockFileList[] = {0};
const SpriteData ExclamationBlockSpriteData = { ProfileId::ExclamationBlock, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile ExclamationBlockProfile(&daExclamationBlock_c::build, SpriteId::ExclamationBlock, ExclamationBlockSpriteData, ProfileId::ExclamationBlock, ProfileId::ExclamationBlock, "ExclamationBlock", ExclamationBlockFileList);


void daExclamationBlock_c::blockWasHit(bool isDown) {
	pos.y = initialY;
	
	if(isDown) {
		activateEntirely = true;
		activateEntirelyDelay = 0;
	}
	
	activateOne();
	if (!testOne()) {
		tile.tileNumber = 0x32;
	}


	physics.setup(this, &physicsInfo, 3, currentLayerID);
	physics.addToList();
	
	doStateChange(&StateID_Wait);
}



void daExclamationBlock_c::calledWhenUpMoveExecutes() {
	if (initialY >= pos.y)
		blockWasHit(false);
}

void daExclamationBlock_c::calledWhenDownMoveExecutes() {
	if (initialY <= pos.y)
		blockWasHit(true);
}



void daExclamationBlock_c::beginState_Wait() {
}

void daExclamationBlock_c::endState_Wait() {
}

void daExclamationBlock_c::executeState_Wait() {
	int result = blockResult();


	if (result == 0)
		return;

	OSReport("blockResult: %d\n", result);

	dAcPy_c *player = (dAcPy_c*)GetSpecificPlayerActor(0);
	OSReport("Flag: %d\n", player->testFlag(0x1E));

	if (result == 1) {
		
		doStateChange(&daEnBlockMain_c::StateID_UpMove);
		anotherFlag = 2;
		isGroundPound = false;
	} else {
		//if(waitUntilNextDownMove > 0) {
		//	waitUntilNextDownMove--;
		//} else {
			doStateChange(&daEnBlockMain_c::StateID_DownMove);
			anotherFlag |= 1;
			isGroundPound = true;
			physics._D8 &= ~0b00101000;
			//_660 = 8;
		//	waitUntilNextDownMove = 6;
		//}
	}
}