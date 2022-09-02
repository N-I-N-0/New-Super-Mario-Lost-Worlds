#include <common.h>
#include <game.h>
#include <profile.h>
#include "path.h"

extern "C" dCourse_c::rail_s *GetRail(int id);

const char* BarrelCannonFileList[] = { "barrelCannon", 0 };

class daBarrelCannon_c : public dEnPath_c {
public:
	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	mHeapAllocator_c allocator;
	nw4r::g3d::ResFile resFile;
	m3d::mdl_c bodyModel;

	u8 rotation;
	
	dAcPy_c* players[4];
	u8 delays[4];

	s16 rotationS16;

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
	
	USING_STATES(daBarrelCannon_c);
	
	DECLARE_STATE(Wait);
	DECLARE_STATE(Rotate);
	DECLARE_STATE(Nothing);
};

CREATE_STATE(daBarrelCannon_c, Wait);
CREATE_STATE(daBarrelCannon_c, Rotate);
CREATE_STATE(daBarrelCannon_c, Nothing);


void daBarrelCannon_c::beginState_Wait() {}
void daBarrelCannon_c::executeState_Wait() {
	dStageActor_c* player = (dStageActor_c*)FindActorByType(PLAYER, 0);

	dx = player->pos.x - this->pos.x;
	dy = player->pos.y - this->pos.y;

	distance = sqrtf(pow(dx, 2) + pow(dy, 2));

	if (distance <= 64) {
		doStateChange(&StateID_FollowPath);
	}
}
void daBarrelCannon_c::endState_Wait() {}


void daBarrelCannon_c::beginState_Rotate() {}
void daBarrelCannon_c::executeState_Rotate() {
	if ((this->rot.y+1) % 0x8000) {
		this->rot.y++;
	} else {
		doStateChange(&StateID_Nothing);
	}
}
void daBarrelCannon_c::endState_Rotate() {}

void daBarrelCannon_c::beginState_Nothing() {}
void daBarrelCannon_c::executeState_Nothing() {}
void daBarrelCannon_c::endState_Nothing() {}






const SpriteData BarrelCannonSpriteData = { ProfileId::BarrelCannon, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile BarrelCannonProfile(&daBarrelCannon_c::build, SpriteId::BarrelCannon, &BarrelCannonSpriteData, ProfileId::BarrelCannon, ProfileId::BarrelCannon, "BarrelCannon", BarrelCannonFileList);


void daBarrelCannon_c::playerCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	dAcPy_c* player = (dAcPy_c*)apOther->owner;
	if (delays[player->characterID] == 0) {
		players[player->characterID] = player;
		delays[player->characterID] = 60;
	}
}
void daBarrelCannon_c::yoshiCollision(ActivePhysics* apThis, ActivePhysics* apOther) {

}
bool daBarrelCannon_c::collisionCat7_GroundPound(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daBarrelCannon_c::collisionCat7_GroundPoundYoshi(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daBarrelCannon_c::collisionCatD_Drill(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daBarrelCannon_c::collisionCatA_PenguinMario(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}

bool daBarrelCannon_c::collisionCat1_Fireball_E_Explosion(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daBarrelCannon_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daBarrelCannon_c::collisionCat9_RollingObject(ActivePhysics* apThis, ActivePhysics* apOther) {

	return false;
}
bool daBarrelCannon_c::collisionCat13_Hammer(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daBarrelCannon_c::collisionCat14_YoshiFire(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}

bool daBarrelCannon_c::collisionCat3_StarPower(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}


dActor_c* daBarrelCannon_c::build() {
	void* buffer = AllocFromGameHeap1(sizeof(daBarrelCannon_c));
	daBarrelCannon_c* c = new(buffer) daBarrelCannon_c;

	return c;
}

int daBarrelCannon_c::onCreate() {
	this->deleteForever = true;

	// Model creation	
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	this->resFile.data = getResource("barrelCannon", "g3d/barrelCannon.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("barrelCannon");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Player(&bodyModel, 0);

	allocator.unlink();

	this->rotation = this->settings >> 29 & 0b111;
	this->rot.z = 0x2000*this->rotation;


	ActivePhysics::Info HitMeBaby;

	// Note: if this gets changed, also change the point where the default
	HitMeBaby.xDistToCenter = 0.0;
	HitMeBaby.yDistToCenter = 12.0;
	HitMeBaby.xDistToEdge = 12.0;
	HitMeBaby.yDistToEdge = 12.0;

	HitMeBaby.category1 = 0x3;
	HitMeBaby.category2 = 0x0;
	HitMeBaby.bitfield1 = 0x4F;
	HitMeBaby.bitfield2 = 0x0;
	HitMeBaby.unkShort1C = 0;
	HitMeBaby.callback = &dEn_c::collisionCallback;

	this->aPhysics.initWithStruct(this, &HitMeBaby);
	this->aPhysics.addToList();
	

	this->disableEatIn();


	this->scale = (Vec){2, 2, 2};

	this->pos.z = 4000;

	players[0] = 0;
	players[1] = 0;
	players[2] = 0;
	players[3] = 0;
	
	delays[0] = 0;
	delays[1] = 0;
	delays[2] = 0;
	delays[3] = 0;
	
	beginState_Init();
	executeState_Init();
	doStateChange(&StateID_Wait);
	
	this->onExecute();
	return true;
}


int daBarrelCannon_c::onDelete() {
	return true;
}

int daBarrelCannon_c::onDraw() {
	bodyModel.scheduleForDrawing();
	return true;
}


void daBarrelCannon_c::updateModelMatrices() {
	Vec p = this->pos;
	switch (this->rotation) {
		case 1:
			p.y += 3;
			p.x += 9;
			break;
		case 2:
			p.y += 12;
			p.x += 12;
			break;
		case 3:
			p.y += 18;
			p.x += 9;
			break;
		case 4:
			p.y += 24;
			break;
		case 5:
			p.y += 21;
			p.x -= 9;
			break;
		case 6:
			p.y += 12;
			p.x -= 12;
			break;
		case 7:
			p.y += 3;
			p.x -= 12;
			break;
		case 0:
		default:
		
	}

	matrix.translation(p.x, p.y, p.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

int daBarrelCannon_c::onExecute() {
	bodyModel._vf1C();
	updateModelMatrices();
	acState.execute();

	bool flagOn = ((dFlagMgr_c::instance->flags & spriteFlagMask) != 0);
	if(flagOn) {
		
		return true;
	}

	int i = 0;
	while (i < 4) {
		if(delays[i] >= 0) {
			delays[i]--;
			continue;
		}
		
		if(players[i] != 0) {
			players[i]->pos.x = this->pos.x;
			players[i]->pos.y = this->pos.y + 7;
			players[i]->setFlag(0xbb);
			if(players[i]->input.heldButtons & WPAD_TWO) {
				players[i]->clearFlag(0xbb);
				float x = 0.0f, y = 0.0f;
				switch (this->rotation) {
					case 1:
						x = -10.6066f;
						y = 10.6066f;
						break;
					case 2:
						x = -15.0f;
						y = 0.0f;
						break;
					case 3:
						x = -10.6066f;
						y = -10.6066f;
						break;
					case 4:
						x = 0.0f;
						y = -15.0f;
						break;
					case 5:
						x = 10.6066f;
						y = -10.6066f;
						break;
					case 6:
						x = 15.0f;
						y = 0.0f;
						break;
					case 7:
						x = 10.6066f;
						y = 10.6066f;
						break;
					case 0:
					default:
						x = 0.0f;
						y = 15.0f;
				}
				players[i]->pipeCannonShot(0, y, x);
				players[i] = 0;
			}
		}
		i++;
	}
}


