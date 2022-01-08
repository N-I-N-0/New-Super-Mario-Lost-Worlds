#include <common.h>
#include <game.h>
#include <g3dhax.h>
#include <sfx.h>
extern "C" void PlaySoundWithFunctionB4(void *spc, nw4r::snd::SoundHandle *handle, int id, int unk);


// Egg Block

class daEggBlock_c : public daEnBlockMain_c {
public:
	TileRenderer tile;
	Physics::Info physicsInfo;

	int wasIHit;

	int onCreate();
	int onDelete();
	int onExecute();

	void calledWhenUpMoveExecutes();
	void calledWhenDownMoveExecutes();

	void blockWasHit(bool isDown);

	USING_STATES(daEggBlock_c);
	DECLARE_STATE(Wait);

	static dActor_c *build();
};


CREATE_STATE(daEggBlock_c, Wait);


int daEggBlock_c::onCreate() {
	this->pos.y -= 8;
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
	switch(this->settings & 0xF) {
		case 0:
			tile.tileNumber = 0x8C;		//green
			break;

		case 1:
			tile.tileNumber = 0x8D;		//light blue
			break;

		case 2:
			tile.tileNumber = 0x8E;		//yellow
			break;

		case 3:
			tile.tileNumber = 0x9C;		//pink
			break;

		case 4:
			tile.tileNumber = 0x9D;		//dark blue
			break;

		case 5:
			tile.tileNumber = 0x9E;		//orange
			break;

		case 6:
			tile.tileNumber = 0x9F;		//red
			break;

		case 7:
			tile.tileNumber = 0xAC;		//purple
			break;

		case 8:
			tile.tileNumber = 0xAD;		//white
			break;

		case 9:
			tile.tileNumber = 0xAE;		//black
			break;

		case 10:
		default:
			tile.tileNumber = 0xAF;		//all - rainbow
			break;
	}

	this->spriteSomeRectX = 6000.0f;

	doStateChange(&daEggBlock_c::StateID_Wait);

	return true;
}


int daEggBlock_c::onDelete() {
	TileRenderer::List *list = dBgGm_c::instance->getTileRendererList(0);
	list->remove(&tile);

	physics.removeFromList();

	return true;
}


int daEggBlock_c::onExecute() {
	acState.execute();
	physics.update();
	blockUpdate();

	tile.setPosition(pos.x-8, -(16+pos.y), pos.z);
	tile.setVars(scale.x);

	if (acState.getCurrentState()->isEqual(&StateID_Wait)) {
		checkZoneBoundaries(0);
	}

	return true;
}


dActor_c *daEggBlock_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daEggBlock_c));
	return new(buffer) daEggBlock_c;
}

const char *EggBlockFileList[] = {0};
const SpriteData EggBlockSpriteData = { ProfileId::EggBlock, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile EggBlockProfile(&daEggBlock_c::build, SpriteId::EggBlock, EggBlockSpriteData, ProfileId::EggBlock, ProfileId::EggBlock, "EggBlock", EggBlockFileList);


void daEggBlock_c::blockWasHit(bool isDown) {
	pos.y = initialY;
	int shroomSettings = isGroundPound ? 1 : 0;
	//dStageActor_c *PoisonMushroom = dStageActor_c::create(PoisonShroom, shroomSettings, &this->pos, 0, 0); //Creating Poison Mushroom
	//PoisonMushroom->pos.z = this->pos.z - 200;
	
	u32 set;
	/*switch(this->settings & 0b1111) {
		case 0:
			return;
		case 1:
			break;
		case 2:
			set = 0x0B000000;
			break;
		default:
			break;
	}*/
	CreateActor(AC_YOSHI_EGG, /*set*/0, &pos, 0, this->currentLayerID);
	
	nw4r::snd::SoundHandle handle;
	PlaySoundWithFunctionB4(SoundRelatedClass, &handle, SE_OBJ_ITEM_APPEAR, 1);
	this->wasIHit = 1;


	physics.setup(this, &physicsInfo, 3, currentLayerID);
	physics.addToList();
	
	doStateChange(&StateID_Wait);
}



void daEggBlock_c::calledWhenUpMoveExecutes() {
	if (initialY >= pos.y)
		blockWasHit(false);
}

void daEggBlock_c::calledWhenDownMoveExecutes() {
	if (initialY <= pos.y)
		blockWasHit(true);
}



void daEggBlock_c::beginState_Wait() {
}

void daEggBlock_c::endState_Wait() {
}

void daEggBlock_c::executeState_Wait() {
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