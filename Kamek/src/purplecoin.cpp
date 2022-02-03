#include <common.h>
#include <game.h>
#include <profile.h>
#include "boomeranghax.h"

const char *PurpleCoinFileList[] = {"purplecoin", 0};


class daEnPurpleCoin_c : public dEn_c {
public:
	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	mHeapAllocator_c allocator;
	nw4r::g3d::ResFile resFile;
	m3d::mdl_c bodyModel;

	bool type8fastJump;
	int type8timer;
	int type8timer2;

	static dActor_c *build();

	void updateModelMatrices();
	void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther);
	void yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther);

	bool collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther);

	bool collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther);
};

const SpriteData PurpleCoinSpriteData = { ProfileId::PurpleCoin, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile PurpleCoinProfile(&daEnPurpleCoin_c::build, SpriteId::PurpleCoin, &PurpleCoinSpriteData, ProfileId::PurpleCoin, ProfileId::PurpleCoin, "PurpleCoin", PurpleCoinFileList);



u8 hijackMusicWithSongName(const char *songName, int themeID, bool hasFast, int channelCount, int trackCount, int *wantRealStreamID);

void daEnPurpleCoin_c::playerCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	this->type8fastJump = true;
}
void daEnPurpleCoin_c::yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	this->type8fastJump = true;
}
bool daEnPurpleCoin_c::collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther) {
	this->type8fastJump = true;
	return false;
}
bool daEnPurpleCoin_c::collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther) {
	this->type8fastJump = true;
	return false;
}
bool daEnPurpleCoin_c::collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther) {
	return false;
}
bool daEnPurpleCoin_c::collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther) {
	return false;
}

bool daEnPurpleCoin_c::collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther) {
	return false;
}
bool daEnPurpleCoin_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther) {

	return false;
}
bool daEnPurpleCoin_c::collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther) {

	return false;
}
bool daEnPurpleCoin_c::collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther) {
	return false;
}
bool daEnPurpleCoin_c::collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther) {
	return false;
}

bool daEnPurpleCoin_c::collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther) {
	return false;
}


dActor_c *daEnPurpleCoin_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daEnPurpleCoin_c));
	daEnPurpleCoin_c *c = new(buffer) daEnPurpleCoin_c;

	return c;
}


extern int getNybbleValue(u32 settings, int fromNybble, int toNybble);

int daEnPurpleCoin_c::onCreate() {
	this->deleteForever = true;

	// Model creation	
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	this->resFile.data = getResource("purplecoin", "g3d/purplecoin.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("PurpleCoin");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Player(&bodyModel, 0);

	allocator.unlink();

	ActivePhysics::Info HitMeBaby;

	HitMeBaby.xDistToCenter = 0.0;
	HitMeBaby.yDistToCenter = 0.0;

	HitMeBaby.xDistToEdge = 7.5;
	HitMeBaby.yDistToEdge = 7.5;

	HitMeBaby.category1 = 0x3;
	HitMeBaby.category2 = 0x0;
	HitMeBaby.bitfield1 = 0x6F;
	HitMeBaby.bitfield2 = 0xffbafffe;
	HitMeBaby.unkShort1C = 0;
	HitMeBaby.callback = &dEn_c::collisionCallback;

	this->aPhysics.initWithStruct(this, &HitMeBaby);
	this->aPhysics.addToList();

	// Stuff I do understand
	this->scale = (Vec){0.2, 0.2, 0.2};

	this->rot.x = 0;
	this->rot.y = 0;
	this->rot.z = 0;

	this->pos.z = 4000;


	this->onExecute();
	return true;
}


int daEnPurpleCoin_c::onDelete() {
	return true;
}

int daEnPurpleCoin_c::onDraw() {
	bodyModel.scheduleForDrawing();
	return true;
}


void daEnPurpleCoin_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y - 14, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

int daEnPurpleCoin_c::onExecute() {
	bodyModel._vf1C();
	updateModelMatrices();
	if(!this->type8fastJump) {
		this->rot.y += 0x300;
	}
	else {
		this->rot.y += 0x2000;
		this->type8timer++;
		if(this->type8timer == 1) {
			nw4r::snd::StrmSoundHandle purpleHandle;
			//int sID;
			//hijackMusicWithSongName("sfx/PURPLECOIN", -1, false, 2, 1, &sID);
			//PlaySoundWithFunctionB4(SoundRelatedClass, &purpleHandle, sID, 1);
			dGameDisplay_c::instance->purplecoins++;
			dGameDisplay_c::instance->refreshPurpleCoins();
			this->removeMyActivePhysics();
		}
		if(this->type8timer < 6) {
			this->pos.y += 4;
			// this->scale.x -= 0.05;
			// this->scale.y -= 0.05;
			// this->scale.z -= 0.05;
		}
		if(this->type8timer > 5 && this->type8timer < 11) {
			this->pos.y -= 4;
			// this->scale.x -= 0.1;
			// this->scale.y -= 0.1;
			// this->scale.z -= 0.1;
		}
		if(this->type8timer == 11) {
			this->Delete(1);
		}
	}

	return true;
}