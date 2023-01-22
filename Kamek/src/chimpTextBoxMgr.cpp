#include "chimpNPC.h"
#include "boss.h"


CREATE_STATE(dChimpTBMgr_c, Init);
CREATE_STATE(dChimpTBMgr_c, Show);
CREATE_STATE(dChimpTBMgr_c, WaitForButton);
CREATE_STATE(dChimpTBMgr_c, WaitForInput);
CREATE_STATE(dChimpTBMgr_c, WaitForText1);
CREATE_STATE(dChimpTBMgr_c, WaitForText2);
CREATE_STATE(dChimpTBMgr_c, End);

dChimpTBMgr_c *dChimpTBMgr_c::instance = 0;

dChimpTBMgr_c *dChimpTBMgr_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dChimpTBMgr_c));
	dChimpTBMgr_c *c = new(buffer) dChimpTBMgr_c;

	instance = c;
	return c;
}

extern u32 isInItemBowling;


int dChimpTBMgr_c::onCreate() {
	if(!layoutLoaded) {
		if (!layout.loadArc("miniGameTextBox.arc", false))
			return false;

		static const char *brlanNames[9] = {
			"miniGameTextBox_appear.brlan",
			"miniGameTextBox_textMid2Up.brlan",
			"miniGameTextBox_textDown2Mid.brlan",
			"miniGameTextBox_textMid2Down.brlan",
			"miniGameTextBox_textUp2Mid.brlan",
			"miniGameTextBox_end.brlan",
			"miniGameTextBox_buttonAppear.brlan",
			"miniGameTextBox_buttonIdle.brlan",
			"miniGameTextBox_buttonDisappear.brlan",
		};

		static const char *groupNames[9] = {
			"A00_Window",
			"B00_Text",
			"B00_Text",
			"B00_Text",
			"B00_Text",
			"A00_Window",
			"C00_button",
			"C00_button",
			"C00_button",
		};

		static const int groupIDs[9] = {
			0,
			1,
			2,
			3,
			4,
			5,
			6,
			7,
			8,
		};

		layout.build("miniGameTextBox.brlyt");

		layout.loadAnimations(brlanNames, 9);
		layout.loadGroups(groupNames, groupIDs, 9);
		layout.disableAllAnimations();
		layout.resetAnim(BUTTON_APPEAR);

		layoutLoaded = true;

		T_text[0] = layout.findTextBoxByName("T_text_00");
		T_text[1] = layout.findTextBoxByName("T_textS_00");

		if(this->settings > SETTINGS_COUNT) this->settings = SETTINGS_COUNT;
	}

	isVisible = true;
	return true;
}


int dChimpTBMgr_c::onDelete() {
	return layout.free();
}

int dChimpTBMgr_c::onDraw() {
	if (isVisible) {
		layout.scheduleForDrawing();
	}

	return true;
}

int dChimpTBMgr_c::onExecute() {
	acState.execute();

	layout.execAnimations();
	layout.update();

	return true;
}


u8 firstTimeChimpBMG[4] = {0, 1, 2, 3};
u8 chimpBMG[11] = {4, 4, 4, 4, 4, 5, 6, 7, 8, 9, 10};

// Init State

void dChimpTBMgr_c::beginState_Init() {
	layout.enableNonLoopAnim(SHOW_ALL);

	if(this->settings > FIRST_TIME) {
		WriteBMGToTextBox(T_text[0], GetBMG(), 0x3EB, chimpBMG[this->settings-1], 0);
		WriteBMGToTextBox(T_text[1], GetBMG(), 0x3EB, chimpBMG[this->settings-1], 0);
	}
	else {
		MakeMarioEnterDemoMode();
		WriteBMGToTextBox(T_text[0], GetBMG(), 0x3EB, firstTimeChimpBMG[0], 0);
		WriteBMGToTextBox(T_text[1], GetBMG(), 0x3EB, firstTimeChimpBMG[0], 0);
	}
}

void dChimpTBMgr_c::executeState_Init() {
	if(!layout.isAnimOn(SHOW_ALL)) {
		if(this->settings == FIRST_TIME) {
			acState.setState(&StateID_WaitForButton);
		}
		else {
			acState.setState(&StateID_End);
		}
	}
}

void dChimpTBMgr_c::endState_Init() {
	
}

// WaitForButton State

void dChimpTBMgr_c::beginState_WaitForButton() {
	layout.enableNonLoopAnim(BUTTON_APPEAR);
}

void dChimpTBMgr_c::executeState_WaitForButton() {
	if(!layout.isAnimOn(BUTTON_APPEAR)) {
		layout.enableLoopAnim(BUTTON_IDLE);
		acState.setState(&StateID_WaitForInput);
	}
}

void dChimpTBMgr_c::endState_WaitForButton() {
	
}

// WaitForInput State

void dChimpTBMgr_c::beginState_WaitForInput() {
}

void dChimpTBMgr_c::executeState_WaitForInput() {
	int nowPressed = Remocon_GetPressed(GetActiveRemocon());

	if(nowPressed & (WPAD_TWO | WPAD_A)) {
		textAnim = TEXT_NEXT1;

		//MapSoundPlayer(SoundRelatedClass, SFX_BUTTONPRESS, 1);
		layout.disableAllAnimations();
		layout.enableNonLoopAnim(BUTTON_DISAPPEAR);
		layout.enableNonLoopAnim(textAnim);
		acState.setState(&StateID_WaitForText1);
	}
	if(nowPressed & (WPAD_ONE | WPAD_B) && this->timer > 0) {
		textAnim = TEXT_BACK1;

		layout.enableNonLoopAnim(textAnim);
		acState.setState(&StateID_WaitForText1);
	}
}

void dChimpTBMgr_c::endState_WaitForInput() {
	
}

// WaitForText1 State

void dChimpTBMgr_c::beginState_WaitForText1() {
}

void dChimpTBMgr_c::executeState_WaitForText1() {
	if(!layout.isAnimOn(textAnim)) {
		if(textAnim == TEXT_NEXT1) {
			T_text[0]->alpha = 255;
			T_text[1]->alpha = 255;
			this->timer++;
		} 
		else {      // TEXT_BACK1
			T_text[0]->alpha = 192;
			T_text[1]->alpha = 192;
			this->timer--;
		}

		WriteBMGToTextBox(T_text[0], GetBMG(), 0x3EB, firstTimeChimpBMG[this->timer], 0);
		WriteBMGToTextBox(T_text[1], GetBMG(), 0x3EB, firstTimeChimpBMG[this->timer], 0);

		layout.enableNonLoopAnim(textAnim+1);
		acState.setState(&StateID_WaitForText2);
	}
}

void dChimpTBMgr_c::endState_WaitForText1() {
	
}

// WaitForText2 State

void dChimpTBMgr_c::beginState_WaitForText2() {
}

void dChimpTBMgr_c::executeState_WaitForText2() {
	if(!layout.isAnimOn(textAnim+1)) {
		if(this->timer > 2) {
			MakeMarioExitDemoMode();
			this->parent->isUntalking = true;
			acState.setState(&StateID_End);
		}
		else {
			if(textAnim == TEXT_NEXT1) {
				acState.setState(&StateID_WaitForButton);
			}
			else {      // TEXT_BACK1
				acState.setState(&StateID_WaitForInput);
			}
		}
	}
}

void dChimpTBMgr_c::endState_WaitForText2() {
	
}

// Show State

void dChimpTBMgr_c::beginState_Show() {
}

void dChimpTBMgr_c::executeState_Show() {
}

void dChimpTBMgr_c::endState_Show() {
	
}

// End State

void dChimpTBMgr_c::beginState_End() {
}

void dChimpTBMgr_c::executeState_End() {
}

void dChimpTBMgr_c::endState_End() {
	
}