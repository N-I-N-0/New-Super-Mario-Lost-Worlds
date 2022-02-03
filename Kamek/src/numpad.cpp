#include <common.h>
#include <game.h>
#include "sfx.h"
#include "numpad.h"



CREATE_STATE(dNumpadSpawner_c, Hidden);
CREATE_STATE(dNumpadSpawner_c, ShowWait);
CREATE_STATE(dNumpadSpawner_c, ButtonActivateWait);
CREATE_STATE(dNumpadSpawner_c, Wait);
CREATE_STATE(dNumpadSpawner_c, HideWait);

dNumpadSpawner_c *dNumpadSpawner_c::instance = 0;
dActor_c *dNumpadSpawner_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dNumpadSpawner_c));
	dNumpadSpawner_c *c = new(buffer) dNumpadSpawner_c;

	instance = c;
	return c;
}

const char *NumPadFileList[] = {0};
const SpriteData NumPadSpriteData = { ProfileId::NumPad, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile NumPadProfile(&dNumpadSpawner_c::build, SpriteId::NumPad, &NumPadSpriteData, ProfileId::NumPad, ProfileId::NumPad, "NumPad", NumPadFileList);


#define ANIM_BUTTON_APPEAR 0 //0, 0, 1, 2, 3, 4, 6, 8
#define ANIM_WINDOW_APPEAR 7
#define ANIM_BUTTON_DESELECT 9 //9, 10, 11, 12, 13, 14, 15, 16
#define ANIM_WINDOW_DISAPPEAR 16
#define ANIM_BUTTON_HIT 18 //19, 20, 21, 22, 23, 24, 25, 26
#define ANIM_BUTTON_SELECTED_IDLE 27 //28, 29, 30, 31, 32, 33, 34, 35
#define ANIM_BG_IN 36
#define ANIM_WINDOW_LOOP 37
#define ANIM_BG_OUT 38
#define ANIM_BUTTON_SELECT 39 //40, 41, 42, 43, 44, 45, 46, 47

int numberToSelect;
int num1 = -1;
int num2 = -1;
int num3 = -1;
int finalNumber;

/*****************************************************************************/
// Events

dNumpadSpawner_c::dNumpadSpawner_c() : state(this, &StateID_ShowWait) {
	layoutLoaded = false;
	visible = false;
}

int dNumpadSpawner_c::onCreate() {
	if (!layoutLoaded) {
		OSReport("1\n");
		bool gotFile = layout.loadArc("NumPad.arc", false);
		if (!gotFile)
			return false;

		selected = 1;
		lastTopRowChoice = 1;

		layout.build("NumPad.brlyt");

		OSReport("2\n");
		if (IsWideScreen()) {
			layout.layout.rootPane->scale.x = 1.0f;
		} else {
			layout.clippingEnabled = true;
			layout.clipX = 0;
			layout.clipY = 52;
			layout.clipWidth = 640;
			layout.clipHeight = 352;
			layout.layout.rootPane->scale.x = 0.731f;
			layout.layout.rootPane->scale.y = 0.7711f;
		}

		OSReport("3\n");
		static const char *brlanNames[] = {
			"NumPad_appearButtons.brlan",
			"NumPad_appearWindow.brlan",
			"NumPad_deselectButton.brlan",
			"NumPad_disappearWindow.brlan",
			"NumPad_hitButton.brlan",
			"NumPad_idleSelectedButton.brlan",
			"NumPad_inBG.brlan",
			"NumPad_loopWindow.brlan",
			"NumPad_outBG.brlan",
			"NumPad_selectButton.brlan",
		};

		OSReport("4\n");
		static const char *groupNames[] = {
			"A00_inWindow",     "A00_inWindow",     "A00_inWindow", 
			"G_nineButton_00",  "G_eightButton_00", "G_sevenButton_00",
			"G_sixButton_00",   "G_fiveButton_00",  "G_fourButton_00",
			"G_threeButton_00", "G_twoButton_00",   "G_oneButton_00",
			"G_nineButton_00",  "G_eightButton_00", "G_sevenButton_00",
			"G_sixButton_00",   "G_fiveButton_00",  "G_fourButton_00",
			"G_threeButton_00", "G_twoButton_00",   "G_oneButton_00",
			"G_nineButton_00",  "G_eightButton_00", "G_sevenButton_00",
			"G_sixButton_00",   "G_fiveButton_00",  "G_fourButton_00",
			"G_threeButton_00", "G_twoButton_00",   "G_oneButton_00",
			"G_nineButton_00",  "G_eightButton_00", "G_sevenButton_00",
			"G_sixButton_00",   "G_fiveButton_00",  "G_fourButton_00",
			"G_threeButton_00", "G_twoButton_00",   "G_oneButton_00",
			"G_nineButton_00",  "G_eightButton_00", "G_sevenButton_00",
			"G_sixButton_00",   "G_fiveButton_00",  "G_fourButton_00",
			"G_threeButton_00", "G_twoButton_00",   "G_oneButton_00",
			"C00_BG_00",        "C00_BG_00",
		};
		static const int brlanIDs[] = {
			1,  3,  7,  //A00_inWindow
			0,  0,  0,  //Numbers
			0,  0,  0,
			0,  0,  0,
			2,  2,  2,  //Numbers
			2,  2,  2,
			2,  2,  2,
			4,  4,  4,  //Numbers
			4,  4,  4,
			4,  4,  4,
			5,  5,  5,  //Numbers
			5,  5,  5,
			5,  5,  5,
			9, 9, 9, //Numbers
			9, 9, 9,
			9, 9, 9,
			8, 8,       //C00_BG_00
		};

		OSReport("5\n");

		layout.loadAnimations(brlanNames, 10);
		OSReport("loadAnimations\n");
		layout.loadGroups(groupNames, brlanIDs, 50);
		OSReport("loadGroups\n");
		layout.disableAllAnimations();
		OSReport("disableAllAnimations\n");

		layout.drawOrder = 140;
		OSReport("6\n");

		static const char *tbNames[] = {
			"T_numberS_00", "T_number_00",
			"T_one_00", "T_one_01",
			"T_two_00", "T_two_01",
			"T_three_00", "T_three_01",
			"T_four_00", "T_four_01",
			"T_five_00", "T_five_01",
			"T_six_00", "T_six_01",
			"T_seven_00", "T_seven_01",
			"T_eight_00", "T_eight_01",
			"T_nine_00", "T_nine_01",
		};
		layout.getTextBoxes(tbNames, &T_numberS_00, 20);

		OSReport("7\n");
		for (int i = 1; i < 10; i++) {
			char middle[16];
			sprintf(middle, "YesButtonMidd_0%d", i);
			BtnMid[i - 1] = layout.findPictureByName(middle);

			char right[16];
			sprintf(right, "YesButtonRigh_0%d", i);
			BtnRight[i - 1] = layout.findPictureByName(right);
			
			char left[16];
			sprintf(left, "YesButtonLeft_0%d", i);
			BtnLeft[i - 1] = layout.findPictureByName(left);
		}
		OSReport("8\n");
		Buttons[0] = layout.findPaneByName("P_oneButton_00");
		Buttons[1] = layout.findPaneByName("P_twoButton_00");
		Buttons[2] = layout.findPaneByName("P_threeButton_00");
		Buttons[3] = layout.findPaneByName("P_fourButton_00");
		Buttons[4] = layout.findPaneByName("P_fiveButton_00");
		Buttons[5] = layout.findPaneByName("P_sixButton_00");
		Buttons[6] = layout.findPaneByName("P_sevenButton_00");
		Buttons[7] = layout.findPaneByName("P_eightButton_00");
		Buttons[8] = layout.findPaneByName("P_nineButton_00");
		OSReport("Found buttons: %p, %p, %p, %p, %p, %p, %p, %p, %p\n", Buttons[0], Buttons[1], Buttons[2], Buttons[3], Buttons[4], Buttons[5], Buttons[6], Buttons[7], Buttons[8]);

		layoutLoaded = true;
	}

	visible = false;

	return true;
}

int dNumpadSpawner_c::onExecute() {
	state.execute();

	layout.execAnimations();
	layout.update();

	return true;
}

int dNumpadSpawner_c::onDraw() {
	if (visible) {
		layout.scheduleForDrawing();
	}
	
	return true;
}

int dNumpadSpawner_c::onDelete() {
	instance = 0;
	if (StageC4::instance)
		StageC4::instance->_1D = 0; // disable no-pause

	return layout.free();
}



// Hidden
void dNumpadSpawner_c::beginState_Hidden() { }
void dNumpadSpawner_c::executeState_Hidden() { }
void dNumpadSpawner_c::endState_Hidden() { }

// ShowWait
void dNumpadSpawner_c::beginState_ShowWait() {
	OSReport("9\n");
	nw4r::snd::SoundHandle handle;
	PlaySoundWithFunctionB4(SoundRelatedClass, &handle, SE_SYS_KO_DIALOGUE_IN, 1);
	OSReport("10\n");

	layout.disableAllAnimations();
	// layout.enableNonLoopAnim(ANIM_WINDOW_APPEAR);
	OSReport("11\n");
	visible = true;
	scaleEase = 0.0;
	StageC4::instance->_1D = 1; // enable no-pause
}
void dNumpadSpawner_c::executeState_ShowWait() {
	OSReport("12\n");
	// if (!layout.isAnimOn(ANIM_WINDOW_APPEAR)) {
		selected = 1;
		OSReport("13\n");
		// layout.enableNonLoopAnim(ANIM_BUTTON_APPEAR);
		// layout.enableNonLoopAnim(ANIM_BUTTON_APPEAR+1);
		// layout.enableNonLoopAnim(ANIM_BUTTON_APPEAR+2);
		// layout.enableNonLoopAnim(ANIM_BUTTON_APPEAR+3);
		// layout.enableNonLoopAnim(ANIM_BUTTON_APPEAR+4);
		// layout.enableNonLoopAnim(ANIM_BUTTON_APPEAR+5);
		// layout.enableNonLoopAnim(ANIM_BUTTON_APPEAR+6);
		// layout.enableNonLoopAnim(ANIM_BUTTON_APPEAR+7);
		// layout.enableNonLoopAnim(ANIM_BUTTON_APPEAR+8);
		OSReport("14\n");
		state.setState(&StateID_ButtonActivateWait);
	// }
}
void dNumpadSpawner_c::endState_ShowWait() {
	OSReport("15\n");
	nw4r::snd::SoundHandle handle;
	// PlaySoundWithFunctionB4(SoundRelatedClass, &handle, SE_OBJ_CLOUD_BLOCK_TO_JUGEM, 1);
	timer = 1;
}

// ButtonActivateWait
void dNumpadSpawner_c::beginState_ButtonActivateWait() { }
void dNumpadSpawner_c::executeState_ButtonActivateWait() {
	if (!layout.isAnyAnimOn())
		state.setState(&StateID_Wait);
}
void dNumpadSpawner_c::endState_ButtonActivateWait() { }

// Wait
void dNumpadSpawner_c::beginState_Wait() {
}
void dNumpadSpawner_c::executeState_Wait() {
	if (timer < 90) {
		scaleEase = -((cos(timer * 3.14 /20)-0.9)/timer*10)+1;
		timer++;
		return;
	}

	int nowPressed = Remocon_GetPressed(GetActiveRemocon());
	int newSelection = -1;

	if (nowPressed & WPAD_ONE) {
		// Hide the thing
		state.setState(&StateID_HideWait);

	} else if (nowPressed & WPAD_UP) {
		// Move up
		if (selected == 1)
			newSelection = 1;
		else if (selected == 2)
			newSelection = 2;
		else if (selected == 3)
			newSelection = 3;
		else if (selected == 4)
			newSelection = 1;
		else if (selected == 5)
			newSelection = 2;
		else if (selected == 6)
			newSelection = 3;
		else if (selected == 7)
			newSelection = 4;
		else if (selected == 8)
			newSelection = 5;
		else if (selected == 9)
			newSelection = 6;

	} else if (nowPressed & WPAD_DOWN) {
		// Move down
		if (selected == 1)
			newSelection = 4;
		else if (selected == 2)
			newSelection = 5;
		else if (selected == 3)
			newSelection = 6;
		else if (selected == 4)
			newSelection = 7;
		else if (selected == 5)
			newSelection = 8;
		else if (selected == 6)
			newSelection = 9;
		else if (selected == 7)
			newSelection = 7;
		else if (selected == 8)
			newSelection = 8;
		else if (selected == 9)
			newSelection = 9;

	} else if (nowPressed & WPAD_LEFT) {
		// Move left
		if (selected == 1)
			newSelection = 1;
		else if (selected == 2)
			newSelection = 1;
		else if (selected == 3)
			newSelection = 2;
		else if (selected == 4)
			newSelection = 4;
		else if (selected == 5)
			newSelection = 4;
		else if (selected == 6)
			newSelection = 5;
		else if (selected == 7)
			newSelection = 7;
		else if (selected == 8)
			newSelection = 7;
		else if (selected == 9)
			newSelection = 8;

	} else if (nowPressed & WPAD_RIGHT) {
		// Move right
		if (selected == 1)
			newSelection = 2;
		else if (selected == 2)
			newSelection = 3;
		else if (selected == 3)
			newSelection = 3;
		else if (selected == 4)
			newSelection = 5;
		else if (selected == 5)
			newSelection = 6;
		else if (selected == 6)
			newSelection = 6;
		else if (selected == 7)
			newSelection = 8;
		else if (selected == 8)
			newSelection = 9;
		else if (selected == 9)
			newSelection = 9;

	} else if (nowPressed & WPAD_TWO) {
		nw4r::snd::SoundHandle handle;
		PlaySoundWithFunctionB4(SoundRelatedClass, &handle, SE_SYS_DECIDE, 1);
		selectNumber(selected);
		int convert[10] = {0, 11, 10, 9, 8, 7, 6, 5, 4, 3};
		int animID = convert[selected];
		layout.enableNonLoopAnim(ANIM_BUTTON_HIT+animID);
	} else if (nowPressed & WPAD_PLUS) {
		doSummon();
	}

	if (newSelection > -1) {
		OSReport("oldSelection = %d\n", selected);
		OSReport("newSelection = %d\n", newSelection);
		nw4r::snd::SoundHandle handle;
		PlaySoundWithFunctionB4(SoundRelatedClass, &handle, SE_SYS_CURSOR, 1);

		int newconvert[10] = {0, 8, 7, 6, 5, 4, 3, 2, 1, 0};
		int convert[10] = {0, 11, 10, 9, 8, 7, 6, 5, 4, 3};
		// int newconvert[10] = {0, 6, 5, 4, 3, 2, 1, 2, 1, 0};
		int animID = convert[selected];
		int newanimID = newconvert[newSelection];
		layout.enableNonLoopAnim(ANIM_BUTTON_DESELECT+animID);
		layout.enableNonLoopAnim(ANIM_BUTTON_SELECT+newanimID);

		selected = newSelection;
	}
}
void dNumpadSpawner_c::endState_Wait() { }

// HideWait
void dNumpadSpawner_c::beginState_HideWait() {
	nw4r::snd::SoundHandle handle;
	PlaySoundWithFunctionB4(SoundRelatedClass, &handle, SE_SYS_DIALOGUE_OUT_AUTO, 1);
	layout.enableNonLoopAnim(ANIM_WINDOW_DISAPPEAR);
	layout.enableNonLoopAnim(ANIM_BUTTON_DESELECT+selected);

	timer = 26;
	// PlaySoundWithFunctionB4(SoundRelatedClass, &handle, SE_OBJ_CS_KINOHOUSE_DISAPP, 1);

	HideSelectCursor(SelectCursorPointer, 0);
}
void dNumpadSpawner_c::executeState_HideWait() {
	if (timer > 0) {
		timer--;
		scaleEase = -((cos(timer * 3.14 /13.5)-0.9)/timer*10)+1;
		if (scaleEase < 0.0f)
			scaleEase = 0.0f;
	}

	if (!layout.isAnimOn(ANIM_WINDOW_DISAPPEAR))
		this->Delete(1);
}
void dNumpadSpawner_c::endState_HideWait() {
	visible = false;
}

void dNumpadSpawner_c::selectNumber(int num) {
	OSReport("num is %d\n", num);
	if(numberToSelect == 0) {
		num1 = num;
		char str[3];
		sprintf(str, "%03d", num1);
		wchar_t numToShow[3];
		OSReport("yes %s\n", str);
		numToShow[0] = str[0];
		numToShow[1] = str[1];
		numToShow[2] = str[2];
		OSReport("yes %s\n", numToShow);
		T_number_00->SetString(numToShow, 0, 3);
		T_numberS_00->SetString(numToShow, 0, 3);
		numberToSelect++;
		return;
	}
	if(numberToSelect == 1) {
		num2 = num;
		char str[3];
		sprintf(str, "%03d", ((num1 * 10) + num2));
		wchar_t numToShow[3];
		OSReport("yes %s\n", str);
		numToShow[0] = str[0];
		numToShow[1] = str[1];
		numToShow[2] = str[2];
		OSReport("yes %s\n", numToShow);
		T_number_00->SetString(numToShow, 0, 3);
		T_numberS_00->SetString(numToShow, 0, 3);
		numberToSelect++;
		return;
	}
	if(numberToSelect == 2) {
		num3 = num;
		char str[3];
		sprintf(str, "%03d", ((num1 * 100) + (num2 * 10) + num3));
		wchar_t numToShow[3];
		OSReport("yes %s\n", str);
		numToShow[0] = str[0];
		numToShow[1] = str[1];
		numToShow[2] = str[2];
		OSReport("yes %s\n", numToShow);
		T_number_00->SetString(numToShow, 0, 3);
		T_numberS_00->SetString(numToShow, 0, 3);
		doSummon();
	}
}

void dNumpadSpawner_c::doSummon() {
	if(num2 == -1) {
		finalNumber = num1;
	}
	else if(num3 == -1) {
		finalNumber = (num1 * 10) + num2;
	}
	else if(num3 > -1) {
		finalNumber = (num1 * 100) + (num2 * 10) + num3;
	}
	OSReport("finalnumber = %d\n", finalNumber);
	dAcPy_c *player = dAcPy_c::findByID(0);
	CreateActor(finalNumber, 0, player->pos, 0, 0);
	num1 = -1;
	num2 = -1;
	num3 = -1;
	numberToSelect = 0;
	state.setState(&StateID_HideWait);
}