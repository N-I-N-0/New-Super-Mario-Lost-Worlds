#include <common.h>
#include <game.h>
#include <g3dhax.h>
#include <dMultiMng_c.h>

void ThwompHammer(dEn_c *thwomp, ActivePhysics *apThis, ActivePhysics *apOther) {
	if (thwomp->name == EN_DOSUN) {
		thwomp->dEn_c::collisionCat13_Hammer(apThis, apOther);
	}
	return;
}

void BooHammer(dEn_c *boo, ActivePhysics *apThis, ActivePhysics *apOther) {
	if (boo->name == EN_TERESA) {
		u32 playerId;
		int comboCount;
		dAcPy_c *player;
		dStageActor_c* hammer;

		SpawnEffect("Wm_en_teresavanish", 0, &boo->pos, &(S16Vec){0,0,0}, &(Vec){1.0, 1.0, 1.0});

		hammer = (dStageActor_c*)(apOther->owner);
		playerId = *(hammer->getPlrNo());
		player = dAcPy_c::findByID(playerId);
		u32 starCount = player->daPlBase_c::getStarCount();
		player->playKameHitSound(starCount, 0);
		comboCount = player->getComboCount(); //8009f630
		dScoreMng_c::instance->dScoreMng_c::sub_800E2190(boo, 0.0, 24.0, comboCount, playerId);
		if (playerId != -1) {
			dMultiMng_c::instance->dMultiMng_c::incEnemyDown(playerId);
		}

		PlaySoundAsync(boo, SE_EMY_TERESA_DEAD);
		boo->Delete(1);
	}
	return;
}

void UrchinHammer(dEn_c *urchin, ActivePhysics *apThis, ActivePhysics *apOther) {
	return;
}

void PakkunHammer(dEn_c *pakkun, ActivePhysics *apThis, ActivePhysics *apOther) {
	if (pakkun->settings >> 24 != 1) { //if not bony do normal hammer
		pakkun->dEn_c::collisionCat13_Hammer(apThis, apOther);
	}
}


#include "poweruphax.h"

void SetCullModeForMaterial(m3d::mdl_c *model, int materialID, GXCullMode mode);


dHammerSuitRenderer_c *dHammerSuitRenderer_c::build() {
	return new dHammerSuitRenderer_c;
}

dHammerSuitRenderer_c::dHammerSuitRenderer_c() { } dHammerSuitRenderer_c::~dHammerSuitRenderer_c() {
}

void dHammerSuitRenderer_c::setup(dPlayerModelHandler_c *handler) {
	setup(handler, 0);
}


void dPlayerModel_c::enableMetalEffect() {
	this->doesFunStuffsWithClr(&this->models[0].body,4);
	this->doesFunStuffsWithClr(&this->models[0].head,4);
}
void dPlayerModel_c::disableMetalEffect() {
	
}


void dHammerSuitRenderer_c::setup(dPlayerModelHandler_c *handler, int sceneID) {
	victim = (dPlayerModel_c*)handler->mdlClass;

	allocator.link(-1, GameHeaps[0], 0, 0x20);

	nw4r::g3d::ResFile hammer(getResource("hammerM", "g3d/suit.brres"));
	nw4r::g3d::ResFile boomer(getResource("boomerangM", "g3d/suit.brres"));
	nw4r::g3d::ResFile spike(getResource("spikeM", "g3d/suit.brres"));
	nw4r::g3d::ResFile cloud(getResource("cloudM", "g3d/suit.brres"));
	nw4r::g3d::ResFile frog(getResource("frogM", "g3d/suit.brres"));

	if (victim->player_id_2 <= 1) {
		/********/
		/*Hammer*/
		/********/
		nw4r::g3d::ResMdl rmhammer = hammer.GetResMdl((victim->player_id_2 == 0) ? "marioHelmet" : "luigiHelmet");
		hammerHelmet.setup(rmhammer, &allocator, 0, 1, 0);
		SetupTextures_MapObj(&hammerHelmet, sceneID);
		/***********/
		/*Boomerang*/
		/***********/
		nw4r::g3d::ResMdl rmboomer = boomer.GetResMdl((victim->player_id_2 == 0) ? "marioHelmet" : "luigiHelmet");
		boomerHelmet.setup(rmboomer, &allocator, 0, 1, 0);
		SetupTextures_MapObj(&boomerHelmet, sceneID);
		/*******/
		/*Spike*/
		/*******/
		nw4r::g3d::ResMdl rmspike = spike.GetResMdl((victim->player_id_2 == 0) ? "marioHelmet" : "luigiHelmet");
		spikeHelmet.setup(rmspike, &allocator, 0, 1, 0);
		SetupTextures_MapObj(&spikeHelmet, sceneID);
		/*******/
		/*Cloud*/
		/*******/
		nw4r::g3d::ResMdl rmcloud = cloud.GetResMdl((victim->player_id_2 == 0) ? "marioHelmet" : "luigiHelmet");
		cloudHelmet.setup(rmcloud, &allocator, 0, 1, 0);
		SetupTextures_Player(&cloudHelmet, sceneID);
		/******/
		/*Frog*/
		/******/
		nw4r::g3d::ResMdl rmfrog = frog.GetResMdl((victim->player_id_2 == 0) ? "marioHelmet" : "luigiHelmet");
		frogHead.setup(rmfrog, &allocator, 0, 1, 0);
		SetupTextures_Player(&frogHead, sceneID);
	}

	const char *shellNames[] = {
		"shell", "shell", "shell", "shell", "shell"
	};
	/********/
	/*Hammer*/
	/********/
	hammerShell.setup(hammer.GetResMdl(shellNames[victim->player_id_2]), &allocator, 0, 1, 0);
	SetupTextures_MapObj(&hammerShell, sceneID);
	/***********/
	/*Boomerang*/
	/***********/
	boomerShell.setup(boomer.GetResMdl(shellNames[victim->player_id_2]), &allocator, 0, 1, 0);
	SetupTextures_MapObj(&boomerShell, sceneID);
	/*******/
	/*Spike*/
	/*******/
	spikeShell.setup(spike.GetResMdl(shellNames[victim->player_id_2]), &allocator, 0, 1, 0);
	SetupTextures_MapObj(&spikeShell, sceneID);

	allocator.unlink();


	victimModel = &victim->models[0].body;
	nw4r::g3d::ResMdl *playerResMdl =
		(nw4r::g3d::ResMdl*)(((u32)victimModel->scnObj) + 0xE8);

	//headNodeID = playerResMdl->GetResNode("player_head").GetID();
	if (victim->player_id_2 <= 1) {
		nw4r::g3d::ResNode face_1 = playerResMdl->GetResNode("face_1");
		headNodeID = face_1.GetID();
	}

	nw4r::g3d::ResNode skl_root = playerResMdl->GetResNode("skl_root");
	rootNodeID = skl_root.GetID();
}

void dHammerSuitRenderer_c::draw() {
	if (victim->powerup_id < 7 || victim->powerup_id == 8 || victim->powerup_id == 13) {
		return;
	}

	if(victim->powerup_id == 7) {
		if (victim->player_id_2 <= 1) {
			// Materials: 2=hair 3=hat; Modes: BACK=visible ALL=invisible
			SetCullModeForMaterial(&victim->getCurrentModel()->head, 3, GX_CULL_ALL);
	
			Mtx headMtx;
			victimModel->getMatrixForNode(headNodeID, headMtx);
	
			hammerHelmet.setDrawMatrix(headMtx);
			hammerHelmet.setScale(1.0f, 1.0f, 1.0f);
			hammerHelmet.calcWorld(false);
	
			hammerHelmet.scheduleForDrawing();
		}
	
		Mtx rootMtx;
		victimModel->getMatrixForNode(rootNodeID, rootMtx);
	
		hammerShell.setDrawMatrix(rootMtx);
		hammerShell.setScale(1.0f, 1.0f, 1.0f);
		hammerShell.calcWorld(false);
	
		hammerShell.scheduleForDrawing();
	}
	
	if(victim->powerup_id == 8) {
		victim->enableMetalEffect();
	}
	
	if(victim->powerup_id == 9) {
		if (victim->player_id_2 <= 1) {
			// Materials: 2=hair 3=hat; Modes: BACK=visible ALL=invisible
			SetCullModeForMaterial(&victim->getCurrentModel()->head, 3, GX_CULL_ALL);
	
			Mtx headMtx;
			victimModel->getMatrixForNode(headNodeID, headMtx);
	
			spikeHelmet.setDrawMatrix(headMtx);
			spikeHelmet.setScale(1.0f, 1.0f, 1.0f);
			spikeHelmet.calcWorld(false);
	
			spikeHelmet.scheduleForDrawing();
		}
	
		Mtx rootMtx;
		victimModel->getMatrixForNode(rootNodeID, rootMtx);
	
		spikeShell.setDrawMatrix(rootMtx);
		spikeShell.setScale(1.0f, 1.0f, 1.0f);
		spikeShell.calcWorld(false);
	
		spikeShell.scheduleForDrawing();
	}
	if(victim->powerup_id == 10) {
		if (victim->player_id_2 <= 1) {
			// Materials: 2=hair 3=hat; Modes: BACK=visible ALL=invisible
			SetCullModeForMaterial(&victim->getCurrentModel()->head, 3, GX_CULL_ALL);
	
			Mtx headMtx;
			victimModel->getMatrixForNode(headNodeID, headMtx);
	
			boomerHelmet.setDrawMatrix(headMtx);
			boomerHelmet.setScale(1.0f, 1.0f, 1.0f);
			boomerHelmet.calcWorld(false);
	
			boomerHelmet.scheduleForDrawing();
		}
	
		Mtx rootMtx;
		victimModel->getMatrixForNode(rootNodeID, rootMtx);
	
		boomerShell.setDrawMatrix(rootMtx);
		boomerShell.setScale(1.0f, 1.0f, 1.0f);
		boomerShell.calcWorld(false);
	
		boomerShell.scheduleForDrawing();
	}
	/*if(victim->powerup_id == 11) {
		if (victim->player_id_2 <= 1) {
			// Materials: 2=hair 3=hat; Modes: BACK=visible ALL=invisible
			SetCullModeForMaterial(&victim->getCurrentModel()->head, 3, GX_CULL_ALL);
	
			Mtx headMtx;
			victimModel->getMatrixForNode(headNodeID, headMtx);
	
			frogHead.setDrawMatrix(headMtx);
			frogHead.setScale(1.0f, 1.0f, 1.0f);
			frogHead.calcWorld(false);
	
			frogHead.scheduleForDrawing();
		}
	
		Mtx rootMtx;
		victimModel->getMatrixForNode(rootNodeID, rootMtx);
	}*/
	if(victim->powerup_id == 12) {
		if (victim->player_id_2 <= 1) {
			// Materials: 2=hair 3=hat; Modes: BACK=visible ALL=invisible
			SetCullModeForMaterial(&victim->getCurrentModel()->head, 3, GX_CULL_ALL);
	
			Mtx headMtx;
			victimModel->getMatrixForNode(headNodeID, headMtx);
	
			cloudHelmet.setDrawMatrix(headMtx);
			cloudHelmet.setScale(1.0f, 1.0f, 1.0f);
			cloudHelmet.calcWorld(false);
	
			cloudHelmet.scheduleForDrawing();
		}
	
		Mtx rootMtx;
		victimModel->getMatrixForNode(rootNodeID, rootMtx);
	}
}








// NEW VERSION
void CrapUpPositions(Vec *out, const Vec *in);
extern float stockItemPowerupModelScale;
extern float stockItemPlayerModelScale;

void dStockItem_c::setScalesOfSomeThings() {
	nw4r::lyt::Pane *ppos = N_forUse_PPos[playerCount];

	int howManyPlayers = 0;
	for (int i = 0; i < 4; i++) {
		if (isPlayerActive[i]) {
			int picID = getIconPictureIDforPlayer(howManyPlayers);
			int charID = Player_ID[i];

			if (picID != 24) {
				nw4r::lyt::Picture *pic = P_icon[picID];

				Vec in, out;

				in.x = pic->effectiveMtx[0][3];
				in.y = pic->effectiveMtx[1][3];
				in.z = pic->effectiveMtx[2][3];

				CrapUpPositions(&out, &in);

				u8 *wmp = (u8*)player2d[charID];

				if(!IsWideScreen()) {
					out.y -= 12;
					stockItemPlayerModelScale = 2.8f;
				} else {
					stockItemPlayerModelScale = 3.2f;
				}

				
				*((float*)(wmp+0xAC)) = out.x;
				*((float*)(wmp+0xB0)) = out.y;
				*((float*)(wmp+0xB4)) = out.z;
				*((float*)(wmp+0x220)) = 0.89999998f;
				*((float*)(wmp+0x224)) = 0.89999998f;
				*((float*)(wmp+0x228)) = 0.89999998f;
				*((float*)(wmp+0x25C)) = 26.0f;
			}
			howManyPlayers++;
		}
	}


	for (int i = 0; i < 14; i++) {
		u8 *item = (u8*)newItemPtr[i];

		nw4r::lyt::Pane *icon = newIconPanes[i];

		Vec in, out;
		in.x = icon->effectiveMtx[0][3];
		in.y = icon->effectiveMtx[1][3];
		in.z = icon->effectiveMtx[2][3];

		CrapUpPositions(&out, &in);

		nw4r::lyt::Pane *shdRoot = shadow->rootPane;

		

		if(!IsWideScreen()) {
			stockItemPowerupModelScale = 2.6f;
			out.y += 8;
		} else {
			stockItemPowerupModelScale = 3.2f;
		}

		*((float*)(item+0xAC)) = out.x;
		*((float*)(item+0xB0)) = out.y;
		*((float*)(item+0xB4)) = out.z;

		*((float*)(item+0x1F4)) = P_buttonBase[i]->scale.x;
		*((float*)(item+0x1F8)) = P_buttonBase[i]->scale.y;
		*((float*)(item+0x1FC)) = 1.0f;
	}


	nw4r::lyt::Pane *shdRoot = shadow->rootPane;
	shdRoot->trans.x = N_stockItem->effectiveMtx[0][3];
	shdRoot->trans.y = N_stockItem->effectiveMtx[1][3];
	shdRoot->trans.z = N_stockItem->effectiveMtx[2][3];
	shdRoot->scale.x = N_stockItem_01->effectiveMtx[0][0];
	shdRoot->scale.y = N_stockItem_01->effectiveMtx[1][1];

	for (int i = 0; i < 7; i++) {
		shadow->buttonBases[i]->scale = newButtonBase[i]->scale;
	}
	shadow->hammerButtonBase->scale = newButtonBase[7]->scale;
	shadow->boomerButtonBase->scale = newButtonBase[8]->scale;
	shadow->spikeButtonBase->scale = newButtonBase[9]->scale;
	shadow->bombButtonBase->scale = newButtonBase[10]->scale;
	shadow->frogButtonBase->scale = newButtonBase[11]->scale;
	shadow->thunderButtonBase->scale = newButtonBase[12]->scale;
	shadow->cloudButtonBase->scale = newButtonBase[13]->scale;
}

int BlkItemSpawn::doStupidOSReport() {
    OSReport("itemType = %d\n", this->itemType);
}








dPlayerModelBase_c::ModelThing additionalModels[4][2];
mHeapAllocator_c additionalModelsAllocators[4];

dPlayerModelBase_c::ModelThing* getCurrentModel(dPlayerModel_c* self) {
	//if(self->player_id_2 == 1)
	//	return &additionalModels[1][0];
	
	
	
	if (self->currentPlayerModelID < 4) {
		return &self->models[self->currentPlayerModelID];
	} else {
		return &additionalModels[self->player_id_2][self->currentPlayerModelID-4];
	}
}




extern "C" void createPlayerModelOrig(dPlayerModel_c*);

void initializeAdditionalModels(dPlayerModel_c* self) {
	if(self->player_id_2 == 1) {

		additionalModelsAllocators[self->player_id_2].link(-1, GameHeaps[0], 0, 0x20);

		nw4r::g3d::ResMdl mdl1 = self->modelResFile.GetResMdl("FRB_model");
		additionalModels[self->player_id_2][0].body.setup(mdl1, &additionalModelsAllocators, 0x93b, 1, 0);
		SetupTextures_Player(&additionalModels[self->player_id_2][0].body, 0);

		nw4r::g3d::ResMdl mdl2 = self->modelResFile.GetResMdl("FRH_model");
		additionalModels[self->player_id_2][0].head.setup(mdl2, &additionalModelsAllocators, 0x97b, 1, 0);
		SetupTextures_Player(&additionalModels[self->player_id_2][0].head, 0);


	/*	nw4r::g3d::ResMdl mdl3 = self->modelResFile.GetResMdl("GOB_model");
		additionalModels[self->player_id_2][1].body.setup(mdl3, &additionalModelsAllocators, 0x93b, 1, 0);
		SetupTextures_Player(&additionalModels[self->player_id_2][1].body, 0);

		nw4r::g3d::ResMdl mdl4 = self->modelResFile.GetResMdl("GOH_model");
		additionalModels[self->player_id_2][1].head.setup(mdl4, &additionalModelsAllocators, 0x97b, 1, 0);
		SetupTextures_Player(&additionalModels[self->player_id_2][1].head, 0);*/

		additionalModelsAllocators[self->player_id_2].unlink();

	}

	createPlayerModelOrig(self);
}




void dPlayerMdl__draw(dPlayerModel_c* self) {
	dPlayerModelBase_c::ModelThing* model = getCurrentModel(self);
	model->body.scheduleForDrawing();
	model->head.scheduleForDrawing();
	self->setSoftLight(&model->body);
	self->setSoftLight(&model->head);
}










