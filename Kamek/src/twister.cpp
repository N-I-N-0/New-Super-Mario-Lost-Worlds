#include <game.h>
#include "path.h"


const char* TwisterFileList[] = { "twister", NULL };

class daEnTwister_c : public dEnPath_c {
public:

	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	void updateModelMatrices();
	
	mEf::es2 effects[2];
	
	static dActor_c* build();

	void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther);
	void spriteCollision(ActivePhysics *apThis, ActivePhysics *apOther);
	void yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther);

	//bool collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat5_Mario(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat8_FencePunch(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat11_PipeCannon(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther); 
	//bool collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther); 
	
	mHeapAllocator_c allocator;
	m3d::mdl_c bodyModel;
	m3d::anmChr_c animationChr;
	nw4r::g3d::ResFile resFile;

};


void daEnTwister_c::playerCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	playerCollides = true;

	daPlBase_c* player = (daPlBase_c*)apOther->owner;

	if (player->pos.y <= this->pos.y + 16) {
		if(player->input.getPressedTwo() || player->input.getHeldTwo()) {
			player->speed.y = 8.0f;
		} else {
			player->speed.y = 6.0f;
		}
	}
}
void daEnTwister_c::yoshiCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	playerCollision(apThis, apOther);
}

void daEnTwister_c::spriteCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	dStageActor_c* sprite = (dStageActor_c*)apOther->owner;

	if (sprite->pos.y <= this->pos.y + 16) {
		sprite->speed.y = 4;
	}
}


void daEnTwister_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

dActor_c* daEnTwister_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daEnTwister_c));
	return new(buffer) daEnTwister_c;
}

const SpriteData TwisterSpriteData = { ProfileId::Twister, 0, 0, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile TwisterProfile(&daEnTwister_c::build, SpriteId::Twister, &TwisterSpriteData, ProfileId::Twister, ProfileId::Twister, "Twister", TwisterFileList);


int daEnTwister_c::onCreate() {
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	resFile.data = getResource("twister", "g3d/twister.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("twister");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Enemy(&bodyModel, 0);

	allocator.unlink(); 

	this->scale.x = 0.15;
	this->scale.y = 0.15;
	this->scale.z = 0.15;

	ActivePhysics::Info HitMeBaby;
	HitMeBaby.xDistToCenter = 0.0;
	HitMeBaby.yDistToCenter = 16.0;
	HitMeBaby.xDistToEdge = 10.0;
	HitMeBaby.yDistToEdge = 28.0;
	HitMeBaby.category1 = 0x5;
	HitMeBaby.category2 = 0x0;
	HitMeBaby.bitfield1 = 0x4F;
	HitMeBaby.bitfield2 = 0x0;
	HitMeBaby.unkShort1C = 0;
	HitMeBaby.callback = &dEn_c::collisionCallback;
	this->aPhysics.initWithStruct(this, &HitMeBaby);
	this->aPhysics.addToList();
	
	effects[0].spawn("Wm_en_spindamage_rg", 0, &(Vec){this->pos.x, this->pos.y + 24.0, 0}, &(S16Vec){0,0,0}, &(Vec){1.0, 1.0, 1.0});
	effects[1].spawn("Wm_en_spindamage_big_rg", 0, &(Vec){this->pos.x, this->pos.y + 48.0, 0}, &(S16Vec){0,0,0}, &(Vec){1.0, 1.0, 1.0});
	
	doStateChange(&StateID_Init);
	
	return true;
}

int daEnTwister_c::onExecute() {
	bodyModel._vf1C();
	updateModelMatrices();
	acState.execute();

	effects[0].spawn("Wm_en_spindamage_rg", 0, &(Vec){this->pos.x, this->pos.y + 24.0, 0}, &(S16Vec){0,0,0}, &(Vec){1.0, 1.0, 1.0});
	effects[1].spawn("Wm_en_spindamage_big_rg", 0, &(Vec){this->pos.x, this->pos.y + 48.0, 0}, &(S16Vec){0,0,0}, &(Vec){1.0, 1.0, 1.0});
	
	this->rot.y += 0x400;

	return true;
}

int daEnTwister_c::onDelete() {
	return true;
}

int daEnTwister_c::onDraw() {
	pos.y += 4;
	bodyModel.scheduleForDrawing();
	pos.y -= 4;

	return true;
}