#include <common.h>
#include <game.h>
#include <g3dhax.h>
#include <sfx.h>
extern "C" void PlaySoundWithFunctionB4(void *spc, nw4r::snd::SoundHandle *handle, int id, int unk);

//Wrote by RedStoneMatt because CLF78 isn't able to do it :p


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
// Trap Block

class daTrapBlock_c : public daEnBlockMain_c {
public:
	TileRenderer tile;
	Physics::Info physicsInfo;

	int wasIHit;
	int doOneTime;

	int onCreate();
	int onDelete();
	int onExecute();

	void calledWhenUpMoveExecutes();
	void calledWhenDownMoveExecutes();

	void blockWasHit(bool isDown);

	USING_STATES(daTrapBlock_c);
	DECLARE_STATE(Wait);

	static daTrapBlock_c *build();
};


CREATE_STATE(daTrapBlock_c, Wait);


int daTrapBlock_c::onCreate() {
	blockInit(pos.y);

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

	TileRenderer::List *list = dBgGm_c::instance->getTileRendererList(0);
	list->add(&tile);

	tile.x = pos.x - 8;
	tile.y = -(16 + pos.y);
	if((this->settings >> 24 & 0xF) == 0) {
		tile.tileNumber = 0x0; //0x0 is invisible, 0x30 is brick and 0x31 is question
	}
	if((this->settings >> 24 & 0xF) == 1) {
		tile.tileNumber = 0x30; //0x0 is invisible, 0x30 is brick and 0x31 is question
	}
	if((this->settings >> 24 & 0xF) == 2) {
		tile.tileNumber = 0x31; //0x0 is invisible, 0x30 is brick and 0x31 is question
	}

	this->spriteSomeRectX = 6000.0f;

	doStateChange(&daTrapBlock_c::StateID_Wait);

	return true;
}


int daTrapBlock_c::onDelete() {
	TileRenderer::List *list = dBgGm_c::instance->getTileRendererList(0);
	list->remove(&tile);

	physics.removeFromList();

	return true;
}


int daTrapBlock_c::onExecute() {
	acState.execute();
	physics.update();
	blockUpdate();

	tile.setPosition(pos.x-8, -(16+pos.y), pos.z);
	tile.setVars(scale.x);

	return true;
}


daTrapBlock_c *daTrapBlock_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daTrapBlock_c));
	return new(buffer) daTrapBlock_c;
}


void daTrapBlock_c::blockWasHit(bool isDown) {
	pos.y = initialY;
	int shroomSettings = isGroundPound ? 1 : 0;
	dStageActor_c *PoisonMushroom = dStageActor_c::create(AC_ITEM_KEY, shroomSettings, &this->pos, 0, 0); //Creating Poison Mushroom
	PoisonMushroom->pos.z = this->pos.z - 200;
	nw4r::snd::SoundHandle handle;
	PlaySoundWithFunctionB4(SoundRelatedClass, &handle, SE_OBJ_ITEM_APPEAR, 1);
	this->wasIHit = 1;


	physics.setup(this, &physicsInfo, 3, currentLayerID);
	physics.addToList();
	
	doStateChange(&StateID_Wait);
}



void daTrapBlock_c::calledWhenUpMoveExecutes() {
	if (initialY >= pos.y)
		blockWasHit(false);
}

void daTrapBlock_c::calledWhenDownMoveExecutes() {
	if (initialY <= pos.y)
		blockWasHit(true);
}



void daTrapBlock_c::beginState_Wait() {
}

void daTrapBlock_c::endState_Wait() {
}

void daTrapBlock_c::executeState_Wait() {
	if(this->wasIHit == 0) {
		int result = blockResult();

		if (result == 0)
			return;

		if (result == 1) {
			if((this->settings >> 24 & 0xF) == 0) { tile.tileNumber = 0x31; }
			doStateChange(&daEnBlockMain_c::StateID_UpMove);
			anotherFlag = 2;
			isGroundPound = false;
		} else {
			if((this->settings >> 24 & 0xF) != 0) {
				doStateChange(&daEnBlockMain_c::StateID_DownMove);
				anotherFlag = 1;
				isGroundPound = true;
			}
		}
	}
	if(this->wasIHit == 1 && this->doOneTime == 0) { //Was already hit ? Change the tiles.
		tile.tileNumber = 0x32; //Brown Block
		this->doOneTime++;
	}
}