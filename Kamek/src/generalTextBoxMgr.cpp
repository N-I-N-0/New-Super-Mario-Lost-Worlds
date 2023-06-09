#include "generalTextBoxMgr.h"

const char* GeneralTBMgrFileList[] = { NULL };

CREATE_STATE(dGeneralTBMgr_c, Init);
CREATE_STATE(dGeneralTBMgr_c, Show);
CREATE_STATE(dGeneralTBMgr_c, WaitForButton);
CREATE_STATE(dGeneralTBMgr_c, WaitForInput);
CREATE_STATE(dGeneralTBMgr_c, WaitForText1);
CREATE_STATE(dGeneralTBMgr_c, WaitForText2);
CREATE_STATE(dGeneralTBMgr_c, End);

dGeneralTBMgr_c *dGeneralTBMgr_c::instance = 0;

dActor_c *dGeneralTBMgr_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dGeneralTBMgr_c));
	dGeneralTBMgr_c *c = new(buffer) dGeneralTBMgr_c;

	instance = c;
	return c;
}

const SpriteData GeneralTBMgrSpriteData = { ProfileId::GeneralTBMgr, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile GeneralTBMgrProfile(&dGeneralTBMgr_c::build, SpriteId::GeneralTBMgr, &GeneralTBMgrSpriteData, ProfileId::GeneralTBMgr, ProfileId::GeneralTBMgr, "Speech Bubble", GeneralTBMgrFileList);


int dGeneralTBMgr_c::onCreate() {
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

		//bubbleWindow = layout.findWindowByName("W_Balloon_00");
		/*nw4r::ut::Color color;
		color.r = 255;
		color.g = 0;
		color.b = 0;
		color.a = 255;
		bubbleWindow->SetVtxColor(0, color);
		bubbleWindow->SetVtxColor(1, color);
		bubbleWindow->SetVtxColor(2, color);
		bubbleWindow->SetVtxColor(3, color);*/

		bubbleLeft = layout.findPictureByName("Picture_00");
		bubbleMiddle = layout.findPictureByName("Picture_01");
		bubbleRight = layout.findPictureByName("Picture_02");
		bubbleName = layout.findPictureByName("Picture_03");
		textName = layout.findTextBoxByName("TextBox_00");
		textName->SetString(L"Rosalina");
		colorLeft.setTexMap(bubbleLeft->material->texMaps);
		colorMiddle.setTexMap(bubbleMiddle->material->texMaps);
		colorRight.setTexMap(bubbleRight->material->texMaps);
		colorName.setTexMap(bubbleName->material->texMaps);
		//int h = 175, s = 127, l = 160; //rosalina
		//int h = 0, s = 127, l = 160; //peach
		//int h = 115, s = 127, l = 60; //Shroob Princess?
		int h = 35, s = 100, l = 20; //Daisy
		colorLeft.colourise(h, s, l);
		colorMiddle.colourise(h, s, l);
		colorRight.colourise(h, s, l);
		colorName.colourise(h, s, l+10);
	}

	isVisible = true;
	return true;
}


int dGeneralTBMgr_c::onDelete() {
	return layout.free();
}

int dGeneralTBMgr_c::onDraw() {
	if (isVisible) {
		layout.scheduleForDrawing();
	}

	return true;
}

int dGeneralTBMgr_c::onExecute() {
	acState.execute();

	layout.execAnimations();
	layout.update();

	return true;
}


// Init State

void dGeneralTBMgr_c::beginState_Init() {
	layout.enableNonLoopAnim(SHOW_ALL);

	MakeMarioEnterDemoMode();
	//WriteBMGToTextBox(T_text[0], GetBMG(), 0x3EB, firstTimeChimpBMG[0], 0);
	//WriteBMGToTextBox(T_text[1], GetBMG(), 0x3EB, firstTimeChimpBMG[0], 0);
}

void dGeneralTBMgr_c::executeState_Init() {
	if(!layout.isAnimOn(SHOW_ALL)) {
		//if(this->settings == FIRST_TIME) {
			acState.setState(&StateID_WaitForButton);
		/*}
		else {
			acState.setState(&StateID_End);
		}*/
	}
}

void dGeneralTBMgr_c::endState_Init() {
	
}

// WaitForButton State

void dGeneralTBMgr_c::beginState_WaitForButton() {
	layout.enableNonLoopAnim(BUTTON_APPEAR);
}

void dGeneralTBMgr_c::executeState_WaitForButton() {
	if(!layout.isAnimOn(BUTTON_APPEAR)) {
		layout.enableLoopAnim(BUTTON_IDLE);
		acState.setState(&StateID_WaitForInput);
	}
}

void dGeneralTBMgr_c::endState_WaitForButton() {
	
}

// WaitForInput State

void dGeneralTBMgr_c::beginState_WaitForInput() {
}

void dGeneralTBMgr_c::executeState_WaitForInput() {
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

void dGeneralTBMgr_c::endState_WaitForInput() {
	
}

// WaitForText1 State

void dGeneralTBMgr_c::beginState_WaitForText1() {
}

void dGeneralTBMgr_c::executeState_WaitForText1() {
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

		//WriteBMGToTextBox(T_text[0], GetBMG(), 0x3EB, firstTimeChimpBMG[this->timer], 0);
		//WriteBMGToTextBox(T_text[1], GetBMG(), 0x3EB, firstTimeChimpBMG[this->timer], 0);

		layout.enableNonLoopAnim(textAnim+1);
		acState.setState(&StateID_WaitForText2);
	}
}

void dGeneralTBMgr_c::endState_WaitForText1() {
	
}

// WaitForText2 State

void dGeneralTBMgr_c::beginState_WaitForText2() {
}

void dGeneralTBMgr_c::executeState_WaitForText2() {
	if(!layout.isAnimOn(textAnim+1)) {
		if(this->timer > 2) {
			MakeMarioExitDemoMode();
			//this->parent->isUntalking = true;
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

void dGeneralTBMgr_c::endState_WaitForText2() {
	
}

// Show State

void dGeneralTBMgr_c::beginState_Show() {
}

void dGeneralTBMgr_c::executeState_Show() {
}

void dGeneralTBMgr_c::endState_Show() {
	
}

// End State

void dGeneralTBMgr_c::beginState_End() {
	const wchar_t* noText = L"";
	T_text[0]->SetString(noText, 0);
	T_text[1]->SetString(noText, 0);
	layout.enableNonLoopAnim(HIDE_ALL);
}

void dGeneralTBMgr_c::executeState_End() {
	if(!layout.isAnimOn(HIDE_ALL)) {
		this->Delete(1);
	}
}

void dGeneralTBMgr_c::endState_End() {
	
}