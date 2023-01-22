#include "chimpNPC.h"

const char* CNPCarcNameList [] = {
	"Chimp",
	NULL
};



void dChimpNPC_c::playerCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	dAcPy_c *player = (dAcPy_c*)apOther->owner;

	// OSReport("Flag 9B: %d; Flag 9C: %d; ", player->testFlag(0x9B), player->testFlag(0x9C));
	// OSReport("Flag 9D: %d; Flag 92: %d;\n", player->testFlag(0x9D), player->testFlag(0x92));
	// OSReport("Flag A9: %d; Flag 8F: %d; ", player->testFlag(0xA9), player->testFlag(0x8F));
	// OSReport("Flag A1: %d; Flag 2B: %d\n", player->testFlag(0xA1), player->testFlag(0x2B));
	// OSReport("Flag 03: %d; Flag 05: %d; ", player->testFlag(3), player->testFlag(5));
	// OSReport("Flag 0A: %d; Flag 0C: %d\n", player->testFlag(0xA), player->testFlag(0xC));
	// OSReport("Flag 0E: %d; Flag 88: %d; ", player->testFlag(0xE), player->testFlag(0x88));
	// OSReport("Flag C0: %d; Flag 48: %d\n", player->testFlag(0xC0), player->testFlag(0x48));

	if(player->testFlag(0xA9) && !this->waitForTopOfTile[player->characterID]) { // Is spinjumping
		bindAnimChr_and_setUpdateRate("spin", 1, 0.0, 1.0);
		//MapSoundPlayer(SoundRelatedClass, SFX_CHIMP_AWFULNOISE, 1);
		this->isSpinning = true;
		this->waitForTopOfTile[player->characterID] = true;
		this->players[player->characterID] = player;
	}
}

// bool dChimpNPC_c::collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther) {
// 	return collisionCat9_RollingObject(apThis, apOther);
// }
// bool dChimpNPC_c::collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther) {
// 	return collisionCat9_RollingObject(apThis, apOther);
// }
// bool dChimpNPC_c::collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther) {
// 	return collisionCat9_RollingObject(apThis, apOther);
// }
// bool dChimpNPC_c::collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther) {
// 	return collisionCat9_RollingObject(apThis, apOther);
// }

// bool dChimpNPC_c::collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther) {
// 	return true;
// }
// bool dChimpNPC_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther) {
// 	return true;
// }
// bool dChimpNPC_c::collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther) {
// 	return true;
// }
// bool dChimpNPC_c::collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther) {
// 	return collisionCat9_RollingObject(apThis, apOther);
// }
// bool dChimpNPC_c::collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther) {
// 	return collisionCat1_Fireball_E_Explosion(apThis, apOther);
// }

// bool dChimpNPC_c::collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther) {
// 	return collisionCat9_RollingObject(apThis, apOther);
// }



dChimpNPC_c *dChimpNPC_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dChimpNPC_c));
	return new(buffer) dChimpNPC_c;
}

void dChimpNPC_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate) { // Stole from shyguy.cpp
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr(name);
	this->chrAnimation.bind(&this->bodyModel, anmChr, unk);
	this->bodyModel.bindAnim(&this->chrAnimation, unk2);
	this->chrAnimation.setUpdateRate(rate);
}

int dChimpNPC_c::onCreate() {
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	this->resFile = getResource("Chimp", "g3d/Chimp.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("Chimp");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Enemy(&bodyModel, 0);

	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr("wait");
	this->chrAnimation.setup(mdl, anmChr, &this->allocator, 0);

	allocator.unlink();

	ActivePhysics::Info HitMeBaby;
	HitMeBaby.xDistToCenter = 2.0;
	HitMeBaby.yDistToCenter = 0.0;
	HitMeBaby.xDistToEdge = 10.0;
	HitMeBaby.yDistToEdge = 16.0;
	HitMeBaby.category = 0x5;
	HitMeBaby.attack = 0x0;
	HitMeBaby.categoryBitfield = 0x4F;
	HitMeBaby.attackBitfield = 0xFFC00000;
	HitMeBaby.miscFlags = 0;
	HitMeBaby.callback = &dEn_c::collisionCallback;

	this->aPhysics.initWithStruct(this, &HitMeBaby);
	this->aPhysics.addToList();
	this->pos.z = 1468.0;

	this->rot.y = 0xE000;
	this->scale = (Vec){0.20, 0.20, 0.20};

	this->isSpinning = false;
	this->doneRotating = true;

	u32 houseType = this->settings & 0xF; // 0: Item Bowling; 1: Penguin Slide; 2: Sand Slide; 3: Gearmo; 4: Bullet Bill Parkour
	u32 tbSettings = 0;
	SaveBlock *save = GetSaveFile()->GetBlock(-1);
	if(save->pad[0x10] & SAVE_CHIMP_VISITED) { // If visited The Chimp already
		if(save->pad[0x10 & (1<<(houseType+1))]) { // If played this type of minigame before
			tbSettings = houseType + 1;
		}
		else {
			tbSettings = houseType + 6;
		}
		bindAnimChr_and_setUpdateRate("wait", 1, 0.0, 1.0);
	}
	else {
		bindAnimChr_and_setUpdateRate("talk", 1, 0.0, 1.0); //surprised but don't have the anim
		this->isSurprised = true;
		save->pad[0x10] |= SAVE_CHIMP_VISITED;
	}
	save->pad[0x10] |= (1<<(houseType+1));

	this->textBoxMgr = (dChimpTBMgr_c*)CreateParentedObject(EN_MAME_KURIBO, this, tbSettings, 0);
	this->textBoxMgr->parent = this;


	// this->onExecute();
	return true;
}


int dChimpNPC_c::onDelete() {
	return true;
}

int dChimpNPC_c::onDraw() {
	bodyModel.scheduleForDrawing();
	return true;
}


void dChimpNPC_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y - 17.5f, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);

	bodyModel._vf1C();
}

int dChimpNPC_c::onExecute() {
	updateModelMatrices();

	for(int i = 0; i < 4; i++) 
		if(this->waitForTopOfTile[i]) 
			if(this->players[i]->collMgr.isOnTopOfTile()) 
				this->waitForTopOfTile[i] = false;

	if(this->isSurprised && !this->doneReacting && this->chrAnimation.currentFrame >= 3.0f) {
		//MapSoundPlayer(SoundRelatedClass, SFX_CHIMP_TALK, 1);
		this->doneReacting = true;;
	}

	if(this->chrAnimation.isAnimationDone()) {
		if(this->isSpinning) {
			bindAnimChr_and_setUpdateRate("wait", 1, 0.0, 1.0);
			this->isSpinning = false;
		}
		else if(this->isSurprised) {
			bindAnimChr_and_setUpdateRate("talk", 1, 0.0, 1.0);
			this->isSurprised = false;
		}
		else if(this->isUntalking) {
			bindAnimChr_and_setUpdateRate("talk2wait", 1, 0.0, 1.0);
			this->isUntalking = false;
			this->isSpinning = true;
		}
		else {
			this->chrAnimation.setCurrentFrame(0.0);
		}
	}

	u32 newdir = dSprite_c__getXDirectionOfFurthestPlayerRelativeToVEC3(this, this->pos);
	if(newdir != this->direction) {
		this->direction = newdir;
		this->doneRotating = false;
	}

	if(!this->doneRotating) {
		this->doneRotating = SmoothRotation(&this->rot.y, (this->direction) ? 0xE000 : 0x2000, 0x400);
	}

	// this->rot.x += 0x200;
	return true;
}

