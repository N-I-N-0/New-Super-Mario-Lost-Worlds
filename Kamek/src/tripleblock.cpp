#include <common.h>
#include <game.h>
#include <actors.h>
#include "sfx.h"

class daEnTripleBlock_c : public daEnBlockMain_c {
public:
	TileRenderer tileRight;
	TileRenderer tileMiddle;
	TileRenderer tileLeft;
	Physics::Info physicsInfo;

	int onCreate();
	int onDelete();
	int onExecute();

	void calledWhenUpMoveExecutes();
	void calledWhenDownMoveExecutes();

	void blockWasHit(bool isDown);
	
	virtual bool isOutOfView();
	virtual void willBeDeleted();
	
	float wasIHit;
	int doOneTime;

	USING_STATES(daEnTripleBlock_c);
	DECLARE_STATE(Wait);

	static dActor_c *build();
};


CREATE_STATE(daEnTripleBlock_c, Wait);


int daEnTripleBlock_c::onCreate() {
	this->pos.y -= 8;
	blockInit(pos.y);
	this->sub_80021740();

	physicsInfo.x1 = -24;
	physicsInfo.y1 = 16;
	physicsInfo.x2 = 24;
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
	
	this->_68B = 1;
	physics._D8 &= ~0b00101000;
	
	TileRenderer::List *list = dBgGm_c::instance->getTileRendererList(0);
	
	//Right
	list->add(&tileRight);

	tileRight.x = pos.x + 8;
	tileRight.y = -(16 + pos.y);
	tileRight.tileNumber = 0x99;

	//Middle
	list->add(&tileMiddle);

	tileMiddle.x = pos.x - 8;
	tileMiddle.y = -(16 + pos.y);
	tileMiddle.tileNumber = 0x98;

	//Left
	list->add(&tileLeft);

	tileLeft.x = pos.x - 24;
	tileLeft.y = -(16 + pos.y);
	tileLeft.tileNumber = 0x97;

	this->wasIHit = 0.0f;
	this->pos.z = 200.0f;
	
	this->spriteSomeRectX = 6000.0f;
	
	doStateChange(&daEnTripleBlock_c::StateID_Wait);

	return true;
}


int daEnTripleBlock_c::onDelete() {
	TileRenderer::List *list = dBgGm_c::instance->getTileRendererList(0);
	list->remove(&tileRight);
	list->remove(&tileMiddle);
	list->remove(&tileLeft);

	physics.removeFromList();

	return true;
}


int daEnTripleBlock_c::onExecute() {
	acState.execute();
	physics.update();
	blockUpdate();

	tileRight.setPosition(pos.x+8, -(16+pos.y), pos.z);
	tileRight.setVars(scale.x);
	
	tileMiddle.setPosition(pos.x-8, -(16+pos.y), pos.z);
	tileMiddle.setVars(scale.x);
	
	tileLeft.setPosition(pos.x-24, -(16+pos.y), pos.z);
	tileLeft.setVars(scale.x);

	// now check zone bounds based on state
	if (acState.getCurrentState()->isEqual(&StateID_Wait)) {
		checkZoneBoundaries(0);
	}

	return true;
}


const char *TripleBlockFileList[] = {0};
const SpriteData TripleBlockSpriteData = { ProfileId::TripleBlock, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile TripleBlockProfile(&daEnTripleBlock_c::build, SpriteId::TripleBlock, &TripleBlockSpriteData, ProfileId::TripleBlock, ProfileId::TripleBlock, "TripleBlock", TripleBlockFileList);

dActor_c *daEnTripleBlock_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daEnTripleBlock_c));
	return new(buffer) daEnTripleBlock_c;
}


extern "C" int CheckExistingPowerup(void * Player);

void daEnTripleBlock_c::blockWasHit(bool isDown) {
	pos.y = initialY;
	
	int p = CheckExistingPowerup(dAcPy_c::findByID(this->playerID)); // Powerups - 0 = small; 1 = big; 2 = fire; 3 = mini; 4 = prop; 5 = peng; 6 = ice; 7 = hammer
	
	Vec coinPosL = (Vec){this->pos.x - 16, this->pos.y + 8, this->pos.z}; //Setting non-GP coinLeft position
	Vec coinPosR = (Vec){this->pos.x + 16, this->pos.y + 8, this->pos.z}; //Setting non-GP coinRight position
	
	nw4r::snd::SoundHandle handle; //Sound Handle
	
	u32 enitemsettings; //EN_ITEM settings
	static u32 reggieToIG[] = {0x0,0x1,0x2,0x7,0x9,0xE,0x11,0x15,0x19,0x6,0x2}; //Mushroom, Star, Coin, 1UP, Fire Flower, Ice Flower, Penguin, Propeller, Mini Shroom, Hammer, 10 Coins
	u32 powerupToSet = reggieToIG[(this->settings >> 20 & 0xF)]; //Spritedata ID to EN_ITEM ID
	enitemsettings = 0 | (powerupToSet << 0) | (2 << 18) | (4 << 9) | (2 << 10) | (this->playerID + 8 << 16); //Setting non-GP settings
	
	u32 coinsettings; //Coins settings (which are EN_ITEM variant)
	coinsettings = 0 | (0x2 << 0) | (2 << 18) | (4 << 9) | (2 << 10) | (this->playerID + 8 << 16); //Setting non-GP settings
	
	if(isGroundPound) { //If the player groundpounded the block, change some setting and positions
		enitemsettings = 0 | (powerupToSet << 0) | (3 << 18) | (4 << 9) | (2 << 10) | (this->playerID + 8 << 16); //Changing value 0xD98 from 2 to 3
		coinsettings = 0 | (0x2 << 0) | (3 << 18) | (4 << 9) | (2 << 10) | (this->playerID + 8 << 16); //Changing value 0xD98 from 2 to 3
		coinPosL = (Vec){this->pos.x - 16, this->pos.y - 8, this->pos.z}; //Changing pos.y
		coinPosR = (Vec){this->pos.x + 16, this->pos.y - 8, this->pos.z}; //Changing pos.y
	}
	if((powerupToSet != 0x2 && powerupToSet != 0x15) || p == 0 || p == 3) { //Propeller does a different sound and Coin doesn't do any sound
		PlaySoundWithFunctionB4(SoundRelatedClass, &handle, SE_OBJ_ITEM_APPEAR, 1); //Item sound
	}
	if(powerupToSet == 0x15 && p != 0) { //If powerup = propeller
		PlaySoundWithFunctionB4(SoundRelatedClass, &handle, SE_OBJ_ITEM_PRPL_APPEAR, 1); //Propeller sound
	}
	dStageActor_c *PowerUp = dStageActor_c::create(EN_ITEM, enitemsettings, &this->pos, 0, 0); //Creating PowerUp
	dStageActor_c *coinLeft = dStageActor_c::create(EN_ITEM, coinsettings, &coinPosL, 0, 0); //Creating coinLeft
	dStageActor_c *coinRight = dStageActor_c::create(EN_ITEM, coinsettings, &coinPosR, 0, 0); //Creating coinRight
	PlaySoundWithFunctionB4(SoundRelatedClass, &handle, SE_OBJ_GET_COIN, 1); //Coin sound
	if(powerupToSet != 0x15 || p == 0 || p == 3) { //If powerup =/= propeller OR actual powerup = 0
		PowerUp->pos.z = 100.0f; //make the powerup behind the triple block
	}

	physics.setup(this, &physicsInfo, 3, currentLayerID);
	physics.addToList();
	
	if((this->settings >> 20 & 0xF) == 10) { //If 10 Coins variant, then only add 0.1 to wasIHit
		this->wasIHit += 0.1f;
	}
	else { //Otherwise, add 1
		this->wasIHit = 1.0f;
	}
	doStateChange(&StateID_Wait); //Back to waiting state
}



void daEnTripleBlock_c::calledWhenUpMoveExecutes() {
	if (initialY >= pos.y)
		blockWasHit(false);
}

void daEnTripleBlock_c::calledWhenDownMoveExecutes() {
	if (initialY <= pos.y)
		blockWasHit(true);
}



void daEnTripleBlock_c::beginState_Wait() {
}

void daEnTripleBlock_c::endState_Wait() {
}

void daEnTripleBlock_c::executeState_Wait() {
	if(this->wasIHit < 1.0f) {
		int result = blockResult();

		if (result == 0)
			return;

		if (result == 1) {
			doStateChange(&daEnBlockMain_c::StateID_UpMove);
			anotherFlag = 2;
			isGroundPound = false;
		} else {
			doStateChange(&daEnBlockMain_c::StateID_DownMove);
			anotherFlag = 1;
			isGroundPound = true;
		}
	}
	if(this->wasIHit > 0.9f && this->doOneTime == 0) { //Was already hit ? Change the tiles.
		tileRight.tileNumber = 0xA9;
		tileMiddle.tileNumber = 0xA8;
		tileLeft.tileNumber = 0xA7;
		this->_68D = 1;
		this->doOneTime++;
	}
}

bool daEnTripleBlock_c::isOutOfView() {
}

void daEnTripleBlock_c::willBeDeleted() {
	return;
}