#include <common.h>
#include <game.h>
#include "sfx.h"
#include "multiChooser.h"

// Replaces: EN_LIFT_ROTATION_HALF (Sprite 107; Profile ID 481 @ 80AF96F8)


daMultiChooser_c *daMultiChooser_c::instance = 0;
dActor_c *daMultiChooser_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daMultiChooser_c));
	daMultiChooser_c *c = new(buffer) daMultiChooser_c;

	instance = c;
	return c;
}


const char* MultiChooserFileList[] = {NULL};
Profile MultiChooserProfile(&daMultiChooser_c::build, ProfileId::MultiChooser, NULL, ProfileId::MultiChooser, ProfileId::MultiChooser, "MultiChooser", MultiChooserFileList);


#define ANIM_MULTICHOOSER_INWINDOW 0
#define ANIM_MULTICHOOSER_LOOPWINDOW 1
#define ANIM_MULTICHOOSER_INBUTTON 2
#define ANIM_MULTICHOOSER_ONBUTTON 3
#define ANIM_MULTICHOOSER_IDLEBUTTON 4
#define ANIM_MULTICHOOSER_HITBUTTON 5
#define ANIM_MULTICHOOSER_OFFBUTTON 6
#define ANIM_MULTICHOOSER_INBG 7
#define ANIM_MULTICHOOSER_OUTBG 8
#define ANIM_MULTICHOOSER_OUTWINDOW 9
#define ANIM_MULTICHOOSER_PAGERIGHT 10
#define ANIM_MULTICHOOSER_PAGELEFT 11
#define ANIM_MULTICHOOSER_INARROW_R 12
#define ANIM_MULTICHOOSER_INARROW_L 13
#define ANIM_MULTICHOOSER_LOOPARROW_R 14
#define ANIM_MULTICHOOSER_LOOPARROW_L 15
#define ANIM_MULTICHOOSER_HITARROW_R 16
#define ANIM_MULTICHOOSER_HITARROW_L 17
#define ANIM_MULTICHOOSER_OUTARROW_R 18
#define ANIM_MULTICHOOSER_OUTARROW_L 19

extern bool doneWithCosmic;
static nw4r::snd::StrmSoundHandle multiHandle;

/*****************************************************************************/
// Events
int daMultiChooser_c::onCreate() {
	if (!layoutLoaded) {
		if (!layout.loadArc("multiChooser.arc", false))
			return false;

		static const char *brlanNames[16] = {
			"multiChooser_inWindow.brlan",
			"multiChooser_loopWindow.brlan",
			"multiChooser_inButton.brlan",
			"multiChooser_onButton.brlan",
			"multiChooser_idleButton.brlan",
			"multiChooser_hitButton.brlan",
			"multiChooser_offButton.brlan",
			"multiChooser_inBG.brlan",
			"multiChooser_outBG.brlan",
			"multiChooser_outWindow.brlan",
			"multiChooser_pageRight.brlan",
			"multiChooser_pageLeft.brlan",
			"multiChooser_inArrow.brlan",
			"multiChooser_loopArrow.brlan",
			"multiChooser_hitArrow.brlan",
			"multiChooser_outArrow.brlan",
		};

		static const char *groupNames[20] = {
			"A00_inWindow", "A00_inWindow",
			"B02_centerButton", "B02_centerButton", "B02_centerButton", "B02_centerButton", "B02_centerButton",
			"C00_BG_00", "C00_BG_00",
			"A00_inWindow",
			"D00_Pages_00", "D00_Pages_00",
			"E00_arrowR", "E01_arrowL",
			"E00_arrowR", "E01_arrowL",
			"E00_arrowR", "E01_arrowL",
			"E00_arrowR", "E01_arrowL"
		};

		static const int groupIDs[20] = {
			0, 1,
			2, 3, 4, 5, 6,
			7, 8,
			9,
			10, 11,
			12, 12,
			13, 13,
			14, 14,
			15, 15
		};

		layout.build("multiChooser.brlyt");

		// if (IsWideScreen()) {
		// 	layout.layout.rootPane->scale.x = 0.7711f;
		// }

		layout.loadAnimations(brlanNames, 16);
		layout.loadGroups(groupNames, groupIDs, 20);
		layout.disableAllAnimations();

		layout.drawOrder = 140;

		layoutLoaded = true;

		this->leftPlayerPanes[0] = this->layout.findPaneByName("P_mar_00");
		this->leftPlayerPanes[1] = this->layout.findPaneByName("P_lui_00");
		this->leftPlayerPanes[2] = this->layout.findPaneByName("P_kinoB_00");
		this->leftPlayerPanes[3] = this->layout.findPaneByName("P_kinoY_00");

		this->rightPlayerPanes[0] = this->layout.findPaneByName("P_mar_01");
		this->rightPlayerPanes[1] = this->layout.findPaneByName("P_lui_01");
		this->rightPlayerPanes[2] = this->layout.findPaneByName("P_kinoB_01");
		this->rightPlayerPanes[3] = this->layout.findPaneByName("P_kinoY_01");


		static const char *bmgTextboxesNames[4] = {
			"T_otehonText_00",	// "Select a Player" Text
			"T_otehonTextS_00",	// "Select a Player" Shadow
			"T_question_00",	// "Who's going to play" Text
			"T_questionS_00",	// "Who's going to play" Shadow
		};

		static const int bmgTextboxesIDs[4] = {
			9,
			9,
			10,
			10,
		};

		layout.setLangStrings(bmgTextboxesNames, bmgTextboxesIDs, 0x3EA, 4);

		setRightPlayerPane(0);

		OSReport("found %d players\n", GetActivePlayerCount());
		this->maxOffs = GetActivePlayerCount()-1;

		if(Player_Active[1]) {
			this->playerIDs[1] = Player_ID[1];
			this->playerPadder[1] = 1;
		}
		else if(Player_Active[2]) {
			this->playerPadder[1] = 2;
		}
		else if(Player_Active[3]) {
			this->playerPadder[1] = 3;
		}

		if(Player_Active[2]) {
			this->playerIDs[2] = Player_ID[2];
			this->playerPadder[2] = 2;
		}
		else if(Player_Active[3]) {
			this->playerPadder[2] = 3;
		}

		if(Player_Active[3]) {
			this->playerIDs[3] = Player_ID[3];
			this->playerPadder[3] = 3;
		}
	}

	visible = false;

	return true;
}

int daMultiChooser_c::onExecute() {
	acState.execute();

	layout.execAnimations();
	layout.update();

	return true;
}

int daMultiChooser_c::onDraw() {
	if (visible) {
		layout.scheduleForDrawing();
	}
	
	return true;
}

int daMultiChooser_c::onDelete() {
	instance = 0;


	return layout.free();
}

/*****************************************************************************/
// Load Resources
CREATE_STATE(daMultiChooser_c, ShowWindow);

void daMultiChooser_c::beginState_ShowWindow() {
	layout.disableAllAnimations();
	layout.enableNonLoopAnim(ANIM_MULTICHOOSER_INWINDOW);
	PlaySoundWithFunctionB4(SoundRelatedClass, &multiHandle, 0x7F, 1);
	visible = true;
}

void daMultiChooser_c::executeState_ShowWindow() {
	if (!layout.isAnimOn(ANIM_MULTICHOOSER_INWINDOW)) {
		acState.setState(&StateID_ShowButton);
	}
}

void daMultiChooser_c::endState_ShowWindow() {

}

/*****************************************************************************/
// Load Resources
CREATE_STATE(daMultiChooser_c, ShowButton);

void daMultiChooser_c::beginState_ShowButton() {
	layout.disableAllAnimations();
	layout.enableNonLoopAnim(ANIM_MULTICHOOSER_INBUTTON);
	layout.enableNonLoopAnim(ANIM_MULTICHOOSER_INARROW_R);
	isLeftArrowHidden = true;
}

void daMultiChooser_c::executeState_ShowButton() {
	if (!layout.isAnimOn(ANIM_MULTICHOOSER_INBUTTON)) {
		acState.setState(&StateID_Wait);
	}
}

void daMultiChooser_c::endState_ShowButton() {
	layout.disableAllAnimations();
	layout.enableNonLoopAnim(ANIM_MULTICHOOSER_ONBUTTON);
	UpdateSelectCursor(this->layout.findPaneByName("N_buttonO_00"), 0, false);
}

/*****************************************************************************/
// Waiting
CREATE_STATE(daMultiChooser_c, Wait);

static const int secretCode[] = {
	WPAD_UP,WPAD_UP,WPAD_DOWN,WPAD_DOWN,
	WPAD_LEFT,WPAD_RIGHT,WPAD_LEFT,WPAD_RIGHT,
	WPAD_B,WPAD_A,0
};
static const int secretCodeButtons = WPAD_UP|WPAD_DOWN|WPAD_LEFT|WPAD_RIGHT|WPAD_A|WPAD_B;
static int secretCodeIndex = 0;

void daMultiChooser_c::beginState_Wait() {
	this->timer = 0;
	secretCodeIndex = 0;
}

void daMultiChooser_c::executeState_Wait() {
	if(!layout.isAnimOn(ANIM_MULTICHOOSER_HITARROW_R) && currentSelection == maxOffs && !isRightArrowHidden) {
		layout.enableNonLoopAnim(ANIM_MULTICHOOSER_OUTARROW_R);
		isRightArrowHidden = true;
	}
	if(!layout.isAnimOn(ANIM_MULTICHOOSER_HITARROW_R) && currentSelection < maxOffs && isRightArrowHidden) {
		layout.enableNonLoopAnim(ANIM_MULTICHOOSER_INARROW_R);
		isRightArrowHidden = false;
	}

	if(!layout.isAnimOn(ANIM_MULTICHOOSER_HITARROW_L) && currentSelection == 0 && !isLeftArrowHidden) {
		layout.enableNonLoopAnim(ANIM_MULTICHOOSER_OUTARROW_L);
		isLeftArrowHidden = true;
	}
	if(!layout.isAnimOn(ANIM_MULTICHOOSER_HITARROW_L) && currentSelection > 0 && isLeftArrowHidden) {
		layout.enableNonLoopAnim(ANIM_MULTICHOOSER_INARROW_L);
		isLeftArrowHidden = false;
	}

	if(!layout.isAnimOn(ANIM_MULTICHOOSER_ONBUTTON) && !layout.isAnimOn(ANIM_MULTICHOOSER_HITBUTTON)) {
		int nowPressed = Remocon_GetPressed(GetActiveRemocon());


		if ((nowPressed & secretCodeButtons) && !this->hasPressed) {
			int nextKey = secretCode[secretCodeIndex];
			if (nowPressed & nextKey) {
				secretCodeIndex++;
				if (secretCode[secretCodeIndex] == 0) {
					secretCodeIndex = 0;

					this->doesKonamiCode = true;
					acState.setState(&StateID_HideWindow);
				}
				return;
			}
			else {
				secretCodeIndex = 0;
			}
		}

		if(nowPressed & WPAD_TWO && !this->hasPressed) {
			// layout.disableAllAnimations();
			layout.enableNonLoopAnim(ANIM_MULTICHOOSER_HITBUTTON);
			PlaySoundWithFunctionB4(SoundRelatedClass, &multiHandle, 0x79, 1);
			HideSelectCursor((void*)SelectCursorPointer, 0);
			this->hasPressed = true;
			return;
		}
		else if(nowPressed & WPAD_RIGHT && !this->hasPressed) {
			if(currentSelection < maxOffs) {
				setRightPlayerPane(playerIDs[this->playerPadder[currentSelection+1]]);
				setLeftPlayerPane(playerIDs[this->playerPadder[currentSelection]]);

				// layout.disableAllAnimations();
				layout.enableNonLoopAnim(ANIM_MULTICHOOSER_PAGELEFT);
				layout.enableNonLoopAnim(ANIM_MULTICHOOSER_HITARROW_R);
				PlaySoundWithFunctionB4(SoundRelatedClass, &multiHandle, 0x78, 1);
				currentSelection++;
			}
			else {
				PlaySoundWithFunctionB4(SoundRelatedClass, &multiHandle, 0x7B, 1);
			}
			OSReport("currentSelection: %d\n", currentSelection);
		}
		else if(nowPressed & WPAD_LEFT && !this->hasPressed) {
			if(currentSelection > 0) {
				setRightPlayerPane(playerIDs[this->playerPadder[currentSelection]]);
				setLeftPlayerPane(playerIDs[this->playerPadder[currentSelection-1]]);

				// layout.disableAllAnimations();
				layout.enableNonLoopAnim(ANIM_MULTICHOOSER_PAGERIGHT);
				layout.enableNonLoopAnim(ANIM_MULTICHOOSER_HITARROW_L);
				PlaySoundWithFunctionB4(SoundRelatedClass, &multiHandle, 0x78, 1);
				currentSelection--;
			}
			else {
				PlaySoundWithFunctionB4(SoundRelatedClass, &multiHandle, 0x7B, 1);
			}
			OSReport("currentSelection: %d\n", currentSelection);
		}

		if(this->hasPressed) {
			this->timer++;
		}

		if(this->timer >= 30) {
			acState.setState(&StateID_HideWindow);
		}
	}
}

void daMultiChooser_c::endState_Wait() {

}

void daMultiChooser_c::setRightPlayerPane(int id) {
	for(int i = 0; i < 4; i++) {
		if(i == id) {
			rightPlayerPanes[i]->SetVisible(true);
		}
		else {
			rightPlayerPanes[i]->SetVisible(false);
		}
	}
}

void daMultiChooser_c::setLeftPlayerPane(int id) {
	for(int i = 0; i < 4; i++) {
		if(i == id) {
			leftPlayerPanes[i]->SetVisible(true);
		}
		else {
			leftPlayerPanes[i]->SetVisible(false);
		}
	}
}


/*****************************************************************************/
// Load Resources
CREATE_STATE(daMultiChooser_c, HideWindow);

extern bool playerFuckeryEnabled;
extern u32 PlayerActiveBackup[4];
extern u32 PlayerIDBackup[4];
extern u32 realPlayer;

void daMultiChooser_c::beginState_HideWindow() {
	layout.disableAllAnimations();
	layout.enableNonLoopAnim(ANIM_MULTICHOOSER_OUTWINDOW);
	PlaySoundWithFunctionB4(SoundRelatedClass, &multiHandle, 0x82 /*0x7A*/, 1);
}

void daMultiChooser_c::executeState_HideWindow() {
	if (!layout.isAnimOn(ANIM_MULTICHOOSER_OUTWINDOW)) {
		if(!this->doesKonamiCode) {
			for(int i = 0; i < 4; i++) {
				PlayerActiveBackup[i] = Player_Active[i];
				PlayerIDBackup[i] = Player_ID[i];
			}

			playerFuckeryEnabled = true;
			realPlayer = this->playerPadder[currentSelection];
		}

		DoStartLevel(this->gameMgr, &this->lvl);
		this->Delete();
	}
}

void daMultiChooser_c::endState_HideWindow() {

}



class dScWorldMap_c : public dActor_c {
	int onCreate_orig();
	int newOnCreate();
};

int dScWorldMap_c::newOnCreate() {
	int orig_val = this->onCreate_orig();

	if(playerFuckeryEnabled) {
		for(int i = 0; i < 4; i++) {
			Player_Active[i] = PlayerActiveBackup[i];
			Player_ID[i] = PlayerIDBackup[i];
		}
		playerFuckeryEnabled = false;
	}

	return orig_val;
}




void newStartLevel(GameMgr *gameMgr, StartLevelInfo *level) {
	//dLevelInfo_c::entry_s *lvl = dLevelInfo_c::s_info.searchBySlot(level->world1, level->level1);
	//if((lvl->vignette == 5 || level->level1 == 26 || level->level1 == 28) && GetActivePlayerCount() > 1) {
	if(false) {
		daMultiChooser_c* chooser = (daMultiChooser_c*)CreateObject(ProfileId::MultiChooser, 0, 0); //WM_GRID
		chooser->gameMgr = gameMgr;
		chooser->lvl = *level;
	} else {
		DoStartLevel(gameMgr, level);
	}
}