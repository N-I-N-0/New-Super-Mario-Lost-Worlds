#include <common.h>
#include <game.h>
#include <profile.h>

const char* LeafPileFileList[] = {"leafPile", "PoisonMushroom", 0};

class daEnLeafPile_c : public dEn_c {
public:
	int onCreate();
	int onExecute();
	//int onDelete();
	int onDraw();

	void collect();

	mHeapAllocator_c allocator;
	nw4r::g3d::ResFile resFile;
	m3d::mdl_c bodyModel;
	m3d::anmClr_c clrAnimation;
	nw4r::g3d::ResAnmClr anmClr;
	
	int counter;

	int destroyType;

	//int type;

	static dActor_c* build();

	void bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate);

	void updateModelMatrices();
	void playerCollision(ActivePhysics* apThis, ActivePhysics* apOther);
	void yoshiCollision(ActivePhysics* apThis, ActivePhysics* apOther);

	bool collisionCat7_GroundPound(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat7_GroundPoundYoshi(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCatD_Drill(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCatA_PenguinMario(ActivePhysics* apThis, ActivePhysics* apOther);

	bool collisionCat1_Fireball_E_Explosion(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat9_RollingObject(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat13_Hammer(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat14_YoshiFire(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat3_StarPower(ActivePhysics* apThis, ActivePhysics* apOther);

	USING_STATES(daEnLeafPile_c);
	DECLARE_STATE(Appear);
	DECLARE_STATE(Wait);
	DECLARE_STATE(WaitForRespawn);
};

CREATE_STATE(daEnLeafPile_c, Appear);
CREATE_STATE(daEnLeafPile_c, Wait);
CREATE_STATE(daEnLeafPile_c, WaitForRespawn);

const SpriteData LeafPileSpriteData = { ProfileId::LeafPile, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile LeafPileProfile(&daEnLeafPile_c::build, SpriteId::LeafPile, &LeafPileSpriteData, ProfileId::LeafPile, ProfileId::LeafPile, "LeafPile", LeafPileFileList);



u8 hijackMusicWithSongName(const char* songName, int themeID, bool hasFast, int channelCount, int trackCount, int* wantRealStreamID);

void daEnLeafPile_c::playerCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->destroyType = 0;
	this->collect();
}

void daEnLeafPile_c::yoshiCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->playerCollision(apThis, apOther);
}

bool daEnLeafPile_c::collisionCat7_GroundPound(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->destroyType = 0;
	this->collect();
	return true;
}

bool daEnLeafPile_c::collisionCat7_GroundPoundYoshi(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->destroyType = 0;
	this->collect();
	return true;
}

bool daEnLeafPile_c::collisionCatD_Drill(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->destroyType = 0;
	this->collect();
	return true;
}

bool daEnLeafPile_c::collisionCatA_PenguinMario(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->destroyType = 0;
	this->collect();
	return true;
}

bool daEnLeafPile_c::collisionCat1_Fireball_E_Explosion(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->destroyType = 1;
	this->collect();
	return true;
}

bool daEnLeafPile_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->destroyType = 0;
	this->collect();
	return true;
}

bool daEnLeafPile_c::collisionCat9_RollingObject(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->destroyType = 0;
	this->collect();
	return true;
}

bool daEnLeafPile_c::collisionCat13_Hammer(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->destroyType = 0;
	this->collect();
	return true;
}

bool daEnLeafPile_c::collisionCat14_YoshiFire(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->destroyType = 1;
	this->Delete(1);
	return true;
}

bool daEnLeafPile_c::collisionCat3_StarPower(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->destroyType = 0;
	this->collect();
	return true;
}


dActor_c* daEnLeafPile_c::build() {
	void* buffer = AllocFromGameHeap1(sizeof(daEnLeafPile_c));
	daEnLeafPile_c* c = new(buffer) daEnLeafPile_c;

	return c;
}


extern int getNybbleValue(u32 settings, int fromNybble, int toNybble);

int daEnLeafPile_c::onCreate() {
	//this->type = this->settings >> 28 & 0xF;

	this->deleteForever = false;
	this->destroyType = 2;


	this->counter = 0;


	// Model creation
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	this->resFile.data = getResource("leafPile", "g3d/leafPile.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("karehayama");
	bodyModel.setup(mdl, &allocator, 0x128, 1, 0);
	//SetupTextures_Player(&bodyModel, 0);

	anmClr = this->resFile.GetResAnmClr("karehayama");
	this->clrAnimation.setup(mdl, anmClr, &this->allocator, 0, 1);
	this->clrAnimation.bind(&this->bodyModel, anmClr, 0, 0);
	this->bodyModel.bindAnim(&this->clrAnimation);
	this->clrAnimation.setFrameForEntry(80.0f, 0);
	this->clrAnimation.setUpdateRateForEntry(1.0f, 0);


	allocator.unlink();

	ActivePhysics::Info HitMeBaby;

	HitMeBaby.xDistToCenter = 0.0;
	HitMeBaby.yDistToCenter = 0.0;

	HitMeBaby.xDistToEdge = 7.5;
	HitMeBaby.yDistToEdge = 7.5;

	HitMeBaby.category1 = 0x3;
	HitMeBaby.category2 = 0x0;
	HitMeBaby.bitfield1 = 0x4F;
	HitMeBaby.bitfield2 = 0x8028E;
	HitMeBaby.unkShort1C = 0;
	HitMeBaby.callback = &dEn_c::collisionCallback;

	this->aPhysics.initWithStruct(this, &HitMeBaby);
	this->aPhysics.addToList();

	this->disableEatIn();

	// Stuff I do understand
	this->scale = (Vec){0.05f, 0.05f, 0.05f};

	this->rot.x = 0;
	this->rot.y = 0;
	this->rot.z = 0;

	//this->pos.y -= 4;
	this->pos.z = 0;

	doStateChange(&StateID_Wait);

	this->onExecute();
	return true;
}


//int daEnLeafPile_c::onDelete() {
//	return true;
//}

void daEnLeafPile_c::collect() {
	doStateChange(&StateID_WaitForRespawn);

	S16Vec nullRot = {0,0,0};
	Vec efScale = {0.05f, 0.05f, 0.05f};
	Vec oneVec = {1.0f, 1.0f, 1.0f};
	nw4r::snd::SoundHandle handle;

	switch (this->destroyType) {
		case 0:
			SpawnEffect("rk_karehayama0", 0, &this->pos, &nullRot, &efScale);
			PlaySoundWithFunctionB4(SoundRelatedClass, &handle, SE_PLY_FOOTNOTE_LEAF, 1);
			break;
		case 1:
			SpawnEffect("Wm_en_explosion", 0, &this->pos, &nullRot, &oneVec);
			PlaySoundWithFunctionB4(SoundRelatedClass, &handle, SE_OBJ_PNGN_ICE_THAW, 1);
			break;
		default:
			break;
	}

	Actors content = EN_ITEM;
	u32 set;
	Vec spawnPos = pos;
	switch(this->settings & 0xFF) {
		case 0:
			return;
		case 1:
			break;
		case 2:
			set = 0x0B000000;
			break;
		case 3:
			set = 0x0B000009;
			break;
		case 4:
			set = 0x0B000015;
			break;
		case 5:
			set = 0x0B00000E;
			break;
		case 6:
			set = 0x0B000011;
			break;
		case 7:
			set = 0x0B000019;
			break;
		case 8:
			set = 0x0B000001;
			break;
		case 9:
			set = 0x0B000007;
			break;
		case 10:
			set = 0x0B000006;
			break;
		case 11:
			set = 0x0B000014;
			break;
		case 12:
			set = 0x0B000010;
			break;
		case 13:
			set = 0x0B00000F;
			break;
		case 14:
			set = 0x0B000013;
			break;
		case 15:
			set = 0x0B000016;
			break;
		case 16:
			set = 0x0B000012;
			break;
		case 17:
			content = AC_YOSHI_EGG;
			//set = 0x008003cc04060000;
			break;
		case 18:
			content = EN_COIN_JUMP;
			//set = 0x008003cc04060000;
			break;
		case 19:
			spawnPos.y += 10;
			content = PoisonShroom;
			//set = 0x008003cc04060000;
			break;
		/*case 148:
			set = 0x008003cc04060000;
			break;*/
		default:
			break;
	}

	CreateActor(content, set, &spawnPos, 0, this->currentLayerID);
}


void daEnLeafPile_c::beginState_Wait() {}
void daEnLeafPile_c::executeState_Wait() {}
void daEnLeafPile_c::endState_Wait() {}

void daEnLeafPile_c::beginState_WaitForRespawn() {
	this->counter = 0;
	this->clrAnimation.setFrameForEntry(0.0f, 0);
	this->aPhysics.removeFromList();
}
void daEnLeafPile_c::executeState_WaitForRespawn() {
	if(this->counter >= 600) {
		doStateChange(&StateID_Appear);
	} else {
		this->counter++;
	}
}
void daEnLeafPile_c::endState_WaitForRespawn() {}


void daEnLeafPile_c::beginState_Appear() {
	this->aPhysics.addToList();
}
void daEnLeafPile_c::executeState_Appear() {
	//OSReport("clr: %f\n", this->clrAnimation.getFrameForEntry(0));
	if (this->clrAnimation.getFrameForEntry(0) != 79.0f) {
		this->clrAnimation.process();
	} else {
		doStateChange(&StateID_Wait);
	}
}
void daEnLeafPile_c::endState_Appear() {}



int daEnLeafPile_c::onDraw() {
	bodyModel.scheduleForDrawing();
	return true;
}


void daEnLeafPile_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y - 4, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

int daEnLeafPile_c::onExecute() {
	bodyModel._vf1C();
	updateModelMatrices();

	this->acState.execute();

	//OSReport("LeafPile State: %s\n", this->acState.getCurrentState()->getName());
	//OSReport("counter: %d\n", counter);

	return true;
}