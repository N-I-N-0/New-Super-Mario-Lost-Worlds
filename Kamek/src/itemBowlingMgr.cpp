#include "itemBowling.h"
#include "boss.h"


CREATE_STATE(dItemBowlingMgr_c, Init);
CREATE_STATE(dItemBowlingMgr_c, ShowTitle);
CREATE_STATE(dItemBowlingMgr_c, HideTitle);
CREATE_STATE(dItemBowlingMgr_c, ShowRules);
CREATE_STATE(dItemBowlingMgr_c, HideRules);
CREATE_STATE(dItemBowlingMgr_c, WaitForThrow);
CREATE_STATE(dItemBowlingMgr_c, WaitForShellDeath);
CREATE_STATE(dItemBowlingMgr_c, FollowShell);
CREATE_STATE(dItemBowlingMgr_c, GetResults);
CREATE_STATE(dItemBowlingMgr_c, WaitForResults);
CREATE_STATE(dItemBowlingMgr_c, End);

dItemBowlingMgr_c *dItemBowlingMgr_c::instance = 0;

dItemBowlingMgr_c *dItemBowlingMgr_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dItemBowlingMgr_c));
	dItemBowlingMgr_c *c = new(buffer) dItemBowlingMgr_c;

	instance = c;
	return c;
}

extern u32 isInItemBowling;

// extern "C" void* _8042A788;
extern "C" void doBGMStuff(void*, u32); // Lowers volume
extern "C" void doBGMStuff2(void*); // Puts volume back to how it was


int dItemBowlingMgr_c::onCreate() {
	if(!layoutLoaded) {
		OSReport("loading miniGameBowling\n");
		if (!layout.loadArc("miniGameBowling.arc", false))
			return false;

		OSReport("loaded miniGameBowling\n");

		static const char *brlanNames[7] = {
			"miniGameBowling_inWindow.brlan",
			"miniGameBowling_loopWindow.brlan",
			"miniGameBowling_inItem.brlan",
			"miniGameBowling_loopGetNum.brlan",
			"miniGameBowling_inTitle.brlan",
			"miniGameBowling_outTitle.brlan",
			"miniGameBowling_outWindow.brlan",
		};

		static const char *groupNames[19] = {
			"A00_Window",
			"A00_Window",
			"C00_Skino_00", "C01_fire_00", "C02_pro_00", "C03_pen_00", "C04_mame_00", "C05_ice_00", "C06_star_00",
			"C00_Skino_00", "C01_fire_00", "C02_pro_00", "C03_pen_00", "C04_mame_00", "C05_ice_00", "C06_star_00",
			"D00_title",
			"D00_title",
			"A00_Window",
		};

		static const int groupIDs[19] = {
			0,
			1,
			2, 2, 2, 2, 2, 2, 2,
			3, 3, 3, 3, 3, 3, 3,
			4,
			5,
			6,
		};

		layout.build("miniGameBowling.brlyt");

		layout.loadAnimations(brlanNames, 7);
		layout.loadGroups(groupNames, groupIDs, 19);
		layout.disableAllAnimations();

		layoutLoaded = true;

		N_info_00 = layout.findPaneByName("N_info_00");
		N_result_00 = layout.findPaneByName("N_result_00");
		N_result_01 = layout.findPaneByName("N_result_01");

		WinItemPanes[0] = layout.findPaneByName("N_Skino_00");
		WinItemPanes[1] = layout.findPaneByName("N_fire_00");
		WinItemPanes[2] = layout.findPaneByName("N_pro_00");
		WinItemPanes[3] = layout.findPaneByName("N_pen_00");
		WinItemPanes[4] = layout.findPaneByName("N_mame_00");
		WinItemPanes[5] = layout.findPaneByName("N_ice_00");
		WinItemPanes[6] = layout.findPaneByName("N_star_00");

		WinItemTextBoxes[0] = layout.findTextBoxByName("T_xNum_00");
		WinItemTextBoxes[1] = layout.findTextBoxByName("T_xNum_01");
		WinItemTextBoxes[2] = layout.findTextBoxByName("T_xNum_02");
		WinItemTextBoxes[3] = layout.findTextBoxByName("T_xNum_03");
		WinItemTextBoxes[4] = layout.findTextBoxByName("T_xNum_04");
		WinItemTextBoxes[5] = layout.findTextBoxByName("T_xNum_05");
		WinItemTextBoxes[6] = layout.findTextBoxByName("T_xNum_06");

		for(int i = 0; i < 7; i++) 
			WinItemPanes[i]->SetVisible(false);


		static const char *bmgTextboxesNames[8] = {
			"T_itemInfo_00",	// "Press 1 on the map" Text
			"T_infoS_01",		// "Press 1 on the map" Shadow
			"T_info_02",		// "There will be item balloons on the way"
			"T_info_00",		// "Throw a shell" Text
			"T_infoS_00",		// "Throw a shell" Shadow
			"T_info_03",		// "You win"
			"T_info_04",		// "Too bad"
			"T_title_00",		// "Item Bowling"
		};

		static const int bmgTextboxesIDs[8] = {
			9,
			9,
			12,
			10,
			10,
			2,
			7,
			11,
		};

		layout.setLangStrings(bmgTextboxesNames, bmgTextboxesIDs, 0x12D, 8);

		this->roomLength = this->settings & 0xFF;

		this->disableFlagMask = 0;
		this->mario = dAcPy_c::findByID(0);
		this->shell = 0;

		physicsBarrierInfoR.x1 = 48;
		physicsBarrierInfoR.y1 = 160;
		physicsBarrierInfoR.x2 = 64;
		physicsBarrierInfoR.y2 = -160;

		physicsBarrierInfoR.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
		physicsBarrierInfoR.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
		physicsBarrierInfoR.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

		physicsBarrierR.setup(this, &physicsBarrierInfoR, 3, currentLayerID);
		physicsBarrierR.flagsMaybe = 0x260;
		physicsBarrierR.callback1 = &daEnBlockMain_c::PhysicsCallback1;
		physicsBarrierR.callback2 = &daEnBlockMain_c::PhysicsCallback2;
		physicsBarrierR.callback3 = &daEnBlockMain_c::PhysicsCallback3;
		physicsBarrierR.addToList();

		physicsBarrierInfoL.x1 = -48;
		physicsBarrierInfoL.y1 = 160;
		physicsBarrierInfoL.x2 = -32;
		physicsBarrierInfoL.y2 = -160;

		physicsBarrierInfoL.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
		physicsBarrierInfoL.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
		physicsBarrierInfoL.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

		physicsBarrierL.setup(this, &physicsBarrierInfoL, 3, currentLayerID);
		physicsBarrierL.flagsMaybe = 0x260;
		physicsBarrierL.callback1 = &daEnBlockMain_c::PhysicsCallback1;
		physicsBarrierL.callback2 = &daEnBlockMain_c::PhysicsCallback2;
		physicsBarrierL.callback3 = &daEnBlockMain_c::PhysicsCallback3;
		physicsBarrierL.addToList();

		dStageActor_c *vPlatform = 0;
		while((vPlatform = (dEn_c*)fBase_c::search(AC_LIFT_RIDE_VMOVE, vPlatform)))
			if((vPlatform->settings >> 4) & 0x1)
				vPlatform->disableFlagMask |= 0x30;

		isInItemBowling = true;
	}

	isVisible = true;
	return true;
}


int dItemBowlingMgr_c::onDelete() {
	isInItemBowling = false;
	return layout.free();
}

int dItemBowlingMgr_c::onDraw() {
	if (isVisible) {
		layout.scheduleForDrawing();
	}

	return true;
}

void centerCameraOnObject(Vec3 pos, bool doX, bool doY) {
	if(doX) {
		float xOffs = pos.x - ClassWithCameraInfo::instance->getEffectiveScreenLeft() - (ClassWithCameraInfo::instance->screenWidth / 2) + ClassWithCameraInfo::instance->xOffset;
		ClassWithCameraInfo::instance->xOffset = xOffs;
	}
	if(doY) {
		float yOffs = pos.y - ClassWithCameraInfo::instance->screenTop + (ClassWithCameraInfo::instance->screenHeight / 2) + ClassWithCameraInfo::instance->yOffsetForTagScroll;
		ClassWithCameraInfo::instance->yOffsetForTagScroll = yOffs;
	}
}

int dItemBowlingMgr_c::onExecute() {
	acState.execute();

	// OSReport("effective: %f, actual: %f\n", ClassWithCameraInfo::instance->getEffectiveScreenLeft(), ClassWithCameraInfo::instance->screenLeft);

	// if(shell == 0) {
	// 	while((shell = (dEn_c*)fBase_c::search(EN_NOKONOKO, shell)))
	// 		if((shell->settings >> 4) & 0x1)
	// 			break;
	// }
	// else {
	// 	OSReport("Shell State: %p %s\n", shell->acState.getCurrentState(), shell->acState.getCurrentState()->getName());
	// }
	// OSReport("Mario State: %p %s\n", mario->states2.getCurrentState(), mario->states2.getCurrentState()->getName());
	
	layout.execAnimations();
	layout.update();

	return true;
}

extern "C" u8 NextStageActorDisableFlags;

// Init State

void dItemBowlingMgr_c::beginState_Init() {
	this->hideAll();
	layout.resetAnim(SHOW_ALL);
	layout.resetAnim(SHOW_TITLE);
	MakeMarioEnterDemoMode();

	OSReport("Initializing Item Bowling.\n");
	StageC4::instance->_1D = 1;
	this->roomLimit = (this->pos.x + ((float)roomLength * 16.0f)) - ClassWithCameraInfo::instance->getEffectiveScreenLeft() - (ClassWithCameraInfo::instance->screenWidth / 2);
	ClassWithCameraInfo::instance->xOffset = this->roomLimit;

	mario->disableFlagMask |= 0x10;
	NextStageActorDisableFlags |= 0x10;

	this->timer = 0;
}

void dItemBowlingMgr_c::executeState_Init() {
	// OSReport("Executing Item Bowling.\n");
	if(this->timer > 60) {
		if(ClassWithCameraInfo::instance->xOffset > 0.0f) {
			ClassWithCameraInfo::instance->xOffset -= 4.0f;
		}
		else {
			// StageC4::instance->_1D = 0;
			ClassWithCameraInfo::instance->xOffset = 0.0f;

			layout.enableNonLoopAnim(SHOW_TITLE);
			acState.setState(&StateID_ShowTitle);
		}
	}

	this->timer++;
}

void dItemBowlingMgr_c::endState_Init() {
	
}

// ShowTitle State

void dItemBowlingMgr_c::beginState_ShowTitle() {
	OSReport("ShowTitle Item Bowling.\n");
	this->timer = 0;
}

void dItemBowlingMgr_c::executeState_ShowTitle() {
	if(!layout.isAnimOn(SHOW_TITLE)) {
		if(this->timer > 120) {
			layout.enableNonLoopAnim(HIDE_TITLE);
			acState.setState(&StateID_HideTitle);
		}

		this->timer++;
	}
}

void dItemBowlingMgr_c::endState_ShowTitle() {
	
}

// HideTitle State

void dItemBowlingMgr_c::beginState_HideTitle() {
	OSReport("HideTitle Item Bowling.\n");
	this->timer = 0;
}

void dItemBowlingMgr_c::executeState_HideTitle() {
	if(!layout.isAnimOn(HIDE_TITLE)) {
		layout.enableNonLoopAnim(SHOW_ALL);
		doBGMStuff(_8042A788, 8);
		if(this->timer > 60) {
			MapSoundPlayer(SoundRelatedClass, SE_SYS_DIALOGUE_IN, 1);

			this->showRules();
			acState.setState(&StateID_ShowRules);
		}

		this->timer++;
	}
}

void dItemBowlingMgr_c::endState_HideTitle() {
	
}

// ShowRules State

void dItemBowlingMgr_c::beginState_ShowRules() {
	OSReport("ShowRules Item Bowling.\n");

}

void dItemBowlingMgr_c::executeState_ShowRules() {
	if(!layout.isAnimOn(SHOW_ALL)) {
		int nowPressed = Remocon_GetPressed(GetActiveRemocon());
		if(nowPressed & (WPAD_TWO | WPAD_A)) {
			NextStageActorDisableFlags &= ~0x10;
			MapSoundPlayer(SoundRelatedClass, 0x79, 1);

			layout.enableNonLoopAnim(HIDE_ALL);
			doBGMStuff2(_8042A788);
			acState.setState(&StateID_HideRules);
		}
	}
}

void dItemBowlingMgr_c::endState_ShowRules() {
	
}

// HideRules State

void dItemBowlingMgr_c::beginState_HideRules() {
	OSReport("HideRules Item Bowling.\n");

}

void dItemBowlingMgr_c::executeState_HideRules() {
	if(!layout.isAnimOn(HIDE_ALL)) {
		MakeMarioExitDemoMode();
		acState.setState(&StateID_WaitForThrow);
	}
}

void dItemBowlingMgr_c::endState_HideRules() {
	// layout.resetAnim(SHOW_ALL);
	// layout.resetAnim(SHOW_TITLE);
	// layout.disableAllAnimations();
}

// WaitForThrow State

void dItemBowlingMgr_c::beginState_WaitForThrow() {
	OSReport("WaitForThrow Item Bowling.\n");

	shell = 0;
	while((shell = (dEn_c*)fBase_c::search(EN_NOKONOKO, shell))) {
		if((shell->settings >> 4) & 0x1) {
			this->shellID = shell->id;
			break;
		}
	}
}

extern "C" dStateBase_c ShellSlideState;

void dItemBowlingMgr_c::executeState_WaitForThrow() {
	if(shell->acState.getCurrentState()->isEqual(&ShellSlideState)) {
		this->freezeMarioPos = mario->pos;
		// mario->removeMyActivePhysics();
		// MakeMarioEnterDemoMode();

		// dFlagMgr_c::instance->flags |= ((u64)1 << (15 - 1));
		physicsBarrierR.removeFromList();
		physicsBarrierL.removeFromList();
		NextStageActorDisableFlags |= 0x20;

		if(shell->direction) { // Left
			acState.setState(&StateID_GetResults);
		}
		else { // Right
			acState.setState(&StateID_FollowShell);
		}
	}
}

void dItemBowlingMgr_c::endState_WaitForThrow() {
	
}

// WaitForShellDeath State

void dItemBowlingMgr_c::beginState_WaitForShellDeath() {
	OSReport("WaitForShellDeath Item Bowling.\n");
}

void dItemBowlingMgr_c::executeState_WaitForShellDeath() {
}

void dItemBowlingMgr_c::endState_WaitForShellDeath() {
	
}

// FollowShell State

void dItemBowlingMgr_c::beginState_FollowShell() {
	OSReport("FollowShell Item Bowling.\n");
	StageC4::instance->_1D = 1;
	this->timer = 0;
	this->baseScreenWall = - ClassWithCameraInfo::instance->getEffectiveScreenLeft() - (ClassWithCameraInfo::instance->screenWidth / 2);
}

void dItemBowlingMgr_c::executeState_FollowShell() {
	if(this->timer == 80) {
		NextStageActorDisableFlags |= 0x10;
	}
	

	if(ClassWithCameraInfo::instance->xOffset < this->roomLimit) {
		ClassWithCameraInfo::instance->xOffset = (shell->pos.x) + this->baseScreenWall;
	}
	else {
		ClassWithCameraInfo::instance->xOffset = this->roomLimit;
		// layout.findPaneByName("N_gameBowling_00")->trans.x += ((float)roomLength * 8.0f);
		acState.setState(&StateID_GetResults);
	}

	this->timer++;
}

void dItemBowlingMgr_c::endState_FollowShell() {
	
}

// GetResults State

void dItemBowlingMgr_c::beginState_GetResults() {
	OSReport("GetResults Item Bowling.\n");
}

void dItemBowlingMgr_c::executeState_GetResults() {
	mario->pos.x = this->freezeMarioPos.x;
	if(fBase_c::search(this->shellID) == 0) {
		if(this->totalItemCount > 0) {
			MapSoundPlayer(SoundRelatedClass, 0x5F2, 1);
			this->showWin();
		}
		else {
			MapSoundPlayer(SoundRelatedClass, 0x5F3, 1);
			this->showLose();
		}

		layout.enableNonLoopAnim(SHOW_ALL);
		doBGMStuff(_8042A788, 8);
		acState.setState(&StateID_WaitForResults);
	}
}

void dItemBowlingMgr_c::endState_GetResults() {
	
}

// WaitForResults State

void dItemBowlingMgr_c::beginState_WaitForResults() {
	OSReport("WaitForResults Item Bowling.\n");
}

void dItemBowlingMgr_c::executeState_WaitForResults() {
	if(!layout.isAnyAnimOn()) {
		for(int i = 0; i < 7; i++) {
			layout.enableLoopAnim(ITEM_NUM+i);
		}
	}


	if(!layout.isAnimOn(SHOW_ALL)) {
		// OSReport("Shown stuff.\n");
		int nowPressed = Remocon_GetPressed(GetActiveRemocon());
		if(nowPressed & (WPAD_TWO | WPAD_A)) {
			MapSoundPlayer(SoundRelatedClass, SE_SYS_DIALOGUE_OUT, 1);
			doBGMStuff2(_8042A788);
			layout.enableNonLoopAnim(HIDE_ALL);
			acState.setState(&StateID_End);
		}

		int nowHeld = Remocon_GetButtons(GetActiveRemocon());
		if(nowHeld & WPAD_LEFT) {
			ClassWithCameraInfo::instance->xOffset--;
		}
		else if(nowHeld & WPAD_RIGHT) {
			ClassWithCameraInfo::instance->xOffset++;
		}
	}
}

void dItemBowlingMgr_c::endState_WaitForResults() {
	
}

// End State

void dItemBowlingMgr_c::beginState_End() {
	OSReport("End Item Bowling.\n");
	this->timer = 0;
}

void dItemBowlingMgr_c::executeState_End() {
	if(!layout.isAnimOn(HIDE_ALL)) {
		if(this->timer == 60) {
			ExitStage(WORLD_MAP, 0, BEAT_LEVEL, MARIO_WIPE);
		}
		this->timer++;
	}
}

void dItemBowlingMgr_c::endState_End() {
	
}

// Pane visibility stuff

u8 itemToInventory[7] = {0, 1, 2, 4, 5, 3, 6};

void dItemBowlingMgr_c::showRules() {
	N_info_00->SetVisible(true);
	N_result_00->SetVisible(false);
	N_result_01->SetVisible(false);
}

void dItemBowlingMgr_c::showWin() {
	N_info_00->SetVisible(false);
	N_result_00->SetVisible(true);
	N_result_01->SetVisible(false);

	s8 itemsGot[7] = {-1, -1, -1, -1, -1, -1, -1};
	int itemTypeCount = 0;
	for(int i = 0; i < 7; i++) {
		if(wonItemsCount[i] > 0) {
			SaveBlock *save = GetSaveFile()->GetBlock(-1);
			save->new_powerups_available[itemToInventory[i]] += wonItemsCount[i];

			WinItemPanes[i]->SetVisible(true);

			wchar_t count[3] = {L'x', L'X', 0};
			count[1] = wonItemsCount[i] + '0'; // No need for a sprintf, it's not like you could get 10 times the same item anyway
			WinItemTextBoxes[i]->SetString(count);

			itemsGot[itemTypeCount++] = i;

			layout.enableNonLoopAnim(IN_ITEM+i);
		}
	}

	for(int i = 0; i < itemTypeCount; i++) {
		char posPaneName[32];
		// wchar_t wposPaneName_t[32];
		snprintf(posPaneName, 32, "N_%dP_Pos_%02d", itemTypeCount, i);
		OSReport("Positionning: %d %d\n", itemTypeCount, i);
		// mbstowcs(wposPaneName_t, posPaneName, 32);

		WinItemPanes[itemsGot[i]]->trans = layout.findPaneByName(posPaneName)->trans;
	}
}

void dItemBowlingMgr_c::showLose() {
	N_info_00->SetVisible(false);
	N_result_00->SetVisible(false);
	N_result_01->SetVisible(true);
}

void dItemBowlingMgr_c::hideAll() {
	N_info_00->SetVisible(false);
	N_result_00->SetVisible(false);
	N_result_01->SetVisible(false);
}