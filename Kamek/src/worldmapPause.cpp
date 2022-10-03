#include <game.h>
#include <common.h>
#include "worldmapPause.h"





CREATE_STATE(dSettings_c, InitWait);
CREATE_STATE(dSettings_c, Appear);
CREATE_STATE(dSettings_c, ButtonChange);
CREATE_STATE(dSettings_c, WaitForInput);
CREATE_STATE(dSettings_c, ButtonHit);
CREATE_STATE(dSettings_c, ShowInfo);
CREATE_STATE(dSettings_c, Close);

dSettings_c::dSettings_c() : layout(), acState(this, &StateID_InitWait) {
	layoutLoaded = 0;
}

// dSettings_c::~dSettings_c() {
// 	delete &layout;
// 	delete &acState;
// 	FreeFromGameHeap1(this);
// }

dActor_c *dSettings_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dSettings_c));
	return (dActor_c*)new(buffer) dSettings_c;
}

const char* EmptyWMSettingsFileList[] = {NULL};
Profile WMSETTINGSProfile(&dSettings_c::build, ProfileId::WM_SETTINGS, NULL, ProfileId::WM_SETTINGS, ProfileId::WM_SETTINGS, "WM Settings", EmptyWMSettingsFileList);



int dSettings_c::onCreate() {
	if(!layoutLoaded) {
		if (!layout.loadArc("settingsMenu.arc", false))
			return false;

		static const char *brlanNames[10] = {
			"settingsMenu_inWindow.brlan",
			"settingsMenu_loopWindow.brlan",
			"settingsMenu_inButton.brlan",
			"settingsMenu_onButton.brlan",
			"settingsMenu_idleButton.brlan",
			"settingsMenu_hitButton.brlan",
			"settingsMenu_offButton.brlan",
			"settingsMenu_inBG.brlan",
			"settingsMenu_outBG.brlan",
			"settingsMenu_outWindow.brlan",
		};

		static const char *groupNames[40] = {
			"A00_inWindow",
			"A00_inWindow",
			"B00_Button", "B01_Button", "B02_Button", "C00_InfoButton", "C01_InfoButton", "C02_InfoButton", "D00_centerButton",
			"B00_Button", "B01_Button", "B02_Button", "C00_InfoButton", "C01_InfoButton", "C02_InfoButton", "D00_centerButton",
			"B00_Button", "B01_Button", "B02_Button", "C00_InfoButton", "C01_InfoButton", "C02_InfoButton", "D00_centerButton",
			"B00_Button", "B01_Button", "B02_Button", "C00_InfoButton", "C01_InfoButton", "C02_InfoButton", "D00_centerButton",
			"B00_Button", "B01_Button", "B02_Button", "C00_InfoButton", "C01_InfoButton", "C02_InfoButton", "D00_centerButton",
			"E00_BG_00",
			"E00_BG_00",
			"A00_inWindow",
		};

		static const char *btnNames[10] = {
			"P_buttonE_00", "P_buttonE_01", "P_buttonE_02",
			"P_buttonD_00", "P_buttonD_01", "P_buttonD_02",
			"P_infoButton_00", "P_infoButton_01", "P_infoButton_02",
			"P_centerBase_00",
		};

		static const char *txtNames[12] = {
			"T_buttonTxt_00", "T_buttonTxt_01", "T_buttonTxt_02",
			"T_buttonTxtS_00", "T_buttonTxtS_01", "T_buttonTxtS_02",
			"T_info_00", "T_info_01", "T_info_02",
			"T_infoS_00", "T_infoS_01", "T_infoS_02",
		};

		static const char *rightNames[6] = {
			"N_button_00", "N_button_01", "N_button_02",
			"T_settingS_00", "T_settingS_01", "T_settingS_02",
		};

		static const int groupIDs[40] = {
			0,
			1,
			2, 2, 2, 2, 2, 2, 2,
			3, 3, 3, 3, 3, 3, 3,
			4, 4, 4, 4, 4, 4, 4,
			5, 5, 5, 5, 5, 5, 5,
			6, 6, 6, 6, 6, 6, 6,
			7,
			8,
			9,
		};

		layout.build("settingsMenu.brlyt");
		layout.loadAnimations(brlanNames, 10);
		layout.loadGroups(groupNames, groupIDs, 40);

		rootPane = layout.getRootPane();

		layout.getPictures(btnNames, (nw4r::lyt::Picture **)&enabledButtons, 10);
		layout.getTextBoxes(txtNames, (nw4r::lyt::TextBox **)&buttonTextBoxes, 12);
		layout.getPanes(rightNames, (nw4r::lyt::Pane **)&rightPanes, 6);

		static const char *bmgTextboxesNames[10] = {
			"T_question_00",	// "Settings" Text
			"T_question_00",	// "Settings" Shadow
			"T_setting_00",		// "Quick Restart" Text
			"T_settingS_00",	// "Quick Restart" Shadow
			"T_setting_01",		// "No Powerup Freeze" Text
			"T_settingS_01",	// "No Powerup Freeze" Shadow
			"T_setting_02",		// "Hard Mode" Text
			"T_settingS_02",	// "Hard Mode" Shadow
			"T_center_00",		// "OK" Text
			"T_center_01",		// "OK" Shadow
		};

		static const int bmgTextboxesIDs[10] = {
			12,
			12,
			13,
			13,
			14,
			14,
			15,
			15,
			18,
			18,
		};

		layout.setLangStrings(bmgTextboxesNames, bmgTextboxesIDs, 0x3EA, 10);

		layout.resetAnim(0);
		
		layout.update();
		layout.execAnimations();

		layout.disableAllAnimations();

		// rootPane->SetVisible(false);
		rootPane->flag &= 0xFE;

		// layout.drawOrder = 140;

		layoutLoaded = true;
	}

	OSReport("loaded settings\n");

	return true;
}

int dSettings_c::onDelete() {
	return layout.free();
}

int dSettings_c::beforeExecute() {
	if(dBase_c::beforeExecute()) {
		return (QueryGlobal5758(-1) == 0);
	}
	return false;
}

int dSettings_c::onExecute() {
	if(this->isCurrentlyActive) {
		acState.execute();
		layout.execAnimations();
		layout.update();
	}

	return true;
}

int dSettings_c::onDraw() {
	if(this->layoutLoaded && this->isCurrentlyActive) {
		layout.scheduleForDrawing();
	}

	return true;
}


void dSettings_c::beginState_InitWait() {

}

void dSettings_c::executeState_InitWait() {
	this->selectedLine = 0;
	this->selectedRow = 0;

	rootPane->SetVisible(true);

	layout.disableAllAnimations();

	for (int i = 9; i < 16; i++) {
		layout.resetAnim(i);
	}

	SaveBlock *save = GetSaveFile()->GetBlock(-1);
	for(int i = 0; i < 3; i++) {
		infoTextBoxes[i]->SetVisible(false);
		infoTextBoxesS[i]->SetVisible(false);

		bool setting = save->pad[0x10 + i];

		enabledButtons[i]->SetVisible(setting);
		disabledButtons[i]->SetVisible(!setting);

		WriteBMGToTextBox(buttonTextBoxes[i], GetBMG(), 0x3EA, 16+(!setting), 0);
		WriteBMGToTextBox(buttonTextBoxesS[i], GetBMG(), 0x3EA, 16+(!setting), 0);
	}

	layout.enableNonLoopAnim(0);
	layout.enableNonLoopAnim(37);

	acState.setState(&StateID_Appear);
}

void dSettings_c::endState_InitWait() {
	
}


void dSettings_c::beginState_Appear() {
	
}

void dSettings_c::executeState_Appear() {
	if(!layout.isAnimOn(0) && !layout.isAnimOn(37)) {
		layout.enableNonLoopAnim(9);

		acState.setState(&StateID_ButtonChange);
	}
}

void dSettings_c::endState_Appear() {

}


void dSettings_c::beginState_ButtonChange() {
	HideSelectCursor((void*)SelectCursorPointer, 0);
}

void dSettings_c::executeState_ButtonChange() {
	if(!layout.isAnimOn(9) && !layout.isAnimOn(10) && !layout.isAnimOn(11) && !layout.isAnimOn(12) && !layout.isAnimOn(13) && !layout.isAnimOn(14) && !layout.isAnimOn(15)) {
		int btnID = -1;

		if(this->selectedLine == 3) btnID = 9;
		else if(this->selectedRow == 1) btnID = this->selectedLine + 6;
		else {
			SaveBlock *save = GetSaveFile()->GetBlock(-1);
			if(save->pad[0x10 + this->selectedLine]) btnID = this->selectedLine;
			else btnID = this->selectedLine + 3;
		}

		UpdateSelectCursor(enabledButtons[btnID], 0, false);

		acState.setState(&StateID_WaitForInput);
	}
}

void dSettings_c::endState_ButtonChange() {

}


void dSettings_c::beginState_WaitForInput() {
	
}

void dSettings_c::executeState_WaitForInput() {
	int nowPressed = Remocon_GetPressed(GetActiveRemocon());

	if((nowPressed & WPAD_RIGHT  || nowPressed & WPAD_LEFT) && this->selectedLine != 3) {
		layout.enableNonLoopAnim(30 + this->selectedLine + (this->selectedRow * 3));

		this->selectedRow = !this->selectedRow;

		layout.enableNonLoopAnim(9 + this->selectedLine + (this->selectedRow * 3));

		MapSoundPlayer(SoundRelatedClass, 0x78, 1);
		acState.setState(&StateID_ButtonChange);
	}
	else if(nowPressed & WPAD_UP) {
		layout.enableNonLoopAnim((this->selectedLine == 3) ? 36 : (30 + this->selectedLine + (this->selectedRow * 3)));

		if(this->selectedLine == 0) this->selectedLine = 3;
		else this->selectedLine--;

		layout.enableNonLoopAnim((this->selectedLine == 3) ? 15 : (9 + this->selectedLine + (this->selectedRow * 3)));

		MapSoundPlayer(SoundRelatedClass, 0x78, 1);
		acState.setState(&StateID_ButtonChange);
	}
	else if(nowPressed & WPAD_DOWN) {
		layout.enableNonLoopAnim((this->selectedLine == 3) ? 36 : (30 + this->selectedLine + (this->selectedRow * 3)));

		if(this->selectedLine == 3) this->selectedLine = 0;
		else this->selectedLine++;

		layout.enableNonLoopAnim((this->selectedLine == 3) ? 15 : (9 + this->selectedLine + (this->selectedRow * 3)));

		MapSoundPlayer(SoundRelatedClass, 0x78, 1);
		acState.setState(&StateID_ButtonChange);
	}
	else if(nowPressed & WPAD_TWO) {
		layout.enableNonLoopAnim((this->selectedLine == 3) ? 29 : (23 + this->selectedLine + (this->selectedRow * 3)));

		MapSoundPlayer(SoundRelatedClass, 0x79, 1);
		acState.setState(&StateID_ButtonHit);
	}
	else if(CheckIfMenuShouldBeCancelledForSpecifiedWiimote(0)) {
		layout.enableNonLoopAnim(38);
		layout.enableNonLoopAnim(39);

		HideSelectCursor((void*)SelectCursorPointer, 0);
		acState.setState(&StateID_Close);
	}
}

void dSettings_c::endState_WaitForInput() {

}


void dSettings_c::beginState_ButtonHit() {
	this->timer = 0;

	if(this->selectedLine == 3) HideSelectCursor((void*)SelectCursorPointer, 0);
}

void dSettings_c::executeState_ButtonHit() {
	if(this->selectedRow == 0 && this->selectedLine != 3 && this->timer < 3) {
		this->timer++;

		if(this->timer == 2) {
			SaveBlock *save = GetSaveFile()->GetBlock(-1);

			bool setting = save->pad[0x10 + this->selectedLine];

			save->pad[0x10 + this->selectedLine] = !setting;

			enabledButtons[this->selectedLine]->SetVisible(!setting);
			disabledButtons[this->selectedLine]->SetVisible(setting);

			WriteBMGToTextBox(buttonTextBoxes[this->selectedLine], GetBMG(), 0x3EA, 16+(setting), 0);
			WriteBMGToTextBox(buttonTextBoxesS[this->selectedLine], GetBMG(), 0x3EA, 16+(setting), 0);
		}
	}

	if(this->selectedRow == 1 && this->selectedLine != 3) {
		this->timer++;

		if(this->timer == 2) {
			for(int i = 0; i < 6; i++) {
				rightPanes[i]->SetVisible(this->isOnInfo);
			}
			centerButton->SetVisible(this->isOnInfo);

			for(int i = 0; i < 3; i++) {
				if(i != this->selectedLine) {
					infoButtons[i]->SetVisible(this->isOnInfo);
				}
				else {
					infoTextBoxes[i]->SetVisible(!this->isOnInfo);
					infoTextBoxesS[i]->SetVisible(!this->isOnInfo);
				}
			}

			this->isOnInfo = !this->isOnInfo;
		}
	}

	if(!layout.isAnimOn(23) && !layout.isAnimOn(24) && !layout.isAnimOn(25) && !layout.isAnimOn(26) && !layout.isAnimOn(27) && !layout.isAnimOn(28) && !layout.isAnimOn(29)) {
		if(this->selectedLine == 3) {
			layout.enableNonLoopAnim(38);
			layout.enableNonLoopAnim(39);

			acState.setState(&StateID_Close);
		}
		else if(this->selectedRow == 1 && this->isOnInfo) {
			acState.setState(&StateID_ShowInfo);
		}
		else {
			acState.setState(&StateID_WaitForInput);
		}
	}
}

void dSettings_c::endState_ButtonHit() {
	
}


void dSettings_c::beginState_ShowInfo() {

}

void dSettings_c::executeState_ShowInfo() {
	int nowPressed = Remocon_GetPressed(GetActiveRemocon());

	if(nowPressed & WPAD_TWO) {
		layout.enableNonLoopAnim(23 + this->selectedLine + (this->selectedRow * 3));

		MapSoundPlayer(SoundRelatedClass, 0x79, 1);
		acState.setState(&StateID_ButtonHit);
	}
}

void dSettings_c::endState_ShowInfo() {
	
}


void dSettings_c::beginState_Close() {
	MapSoundPlayer(SoundRelatedClass, 122, 1);
}

void dSettings_c::executeState_Close() {
	if(!layout.isAnimOn(38) && !layout.isAnimOn(39)) {
		this->isCurrentlyActive = 0;

		acState.setState(&StateID_InitWait);
	}
}

void dSettings_c::endState_Close() {
	
}


CREATE_STATE(dCourseSelectManager_c, SettingsWait);

void dCourseSelectManager_c::beginState_SettingsWait() {

}

void dCourseSelectManager_c::executeState_SettingsWait() {
	if(!this->settingsPtr->isCurrentlyActive) {
		this->changeToNormalState();
	}
}

void dCourseSelectManager_c::endState_SettingsWait() {
	
}