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
	int waitUntilNextDownMove;

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
	this->sub_80021740();

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

	this->_68B = 1;
	physics._D8 &= ~0b00101000;

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
	
	static u8 tileNumbers[] = {0xB6, 0xC4, 0xB7, 0xB5, 0xC5, 0xC7, 0xC6, 0xD5, 0xD6, 0xD4, 0xD7};
	
	tile.tileNumber = tileNumbers[this->settings & 0xF];

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
Profile EggBlockProfile(&daEggBlock_c::build, SpriteId::EggBlock, &EggBlockSpriteData, ProfileId::EggBlock, ProfileId::EggBlock, "EggBlock", EggBlockFileList);


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
	
	Vec eggPos = pos;
	eggPos.y += (isDown ? -8 : 8);
	
	u8 color = this->settings & 0xF;	//0 = light blue; 1 = pink; 2 = yellow; 3 = green; 4 = blue; 5 = red; 6 = orange; 7 = brown; 8 = white; 9 = purple.
	CreateActor(AC_YOSHI_EGG, /*set*/(color > 9 ? 0 : (u32)(1 << 21 | color)), &eggPos, 0, this->currentLayerID);
	
	nw4r::snd::SoundHandle handle;
	PlaySoundWithFunctionB4(SoundRelatedClass, &handle, SE_PLY_YOSHI_EGG_APPEAR, 1);
	//this->wasIHit = 1;


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