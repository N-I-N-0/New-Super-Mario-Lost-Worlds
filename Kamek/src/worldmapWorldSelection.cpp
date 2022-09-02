#include "worldmapWorldSelection.h"

/*****************************************************************/
/********** The following code was written by Asu-chan, **********/
/*************** with help and docs from NewerSMBW. **************/
/* If you want to use this code for any mod, please give credits */
/*****************************************************************/

CREATE_STATE(dWMWSEL_c, Hidden);
CREATE_STATE(dWMWSEL_c, ShowWait);
CREATE_STATE(dWMWSEL_c, ButtonAnimWait);
CREATE_STATE(dWMWSEL_c, WaitForInput);
CREATE_STATE(dWMWSEL_c, ArrowHit);
CREATE_STATE(dWMWSEL_c, Select);
CREATE_STATE(dWMWSEL_c, Exit);


dWMWSEL_c *dWMWSEL_c::instance = 0;

dActor_c *dWMWSEL_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dWMWSEL_c));
	dWMWSEL_c *c = new(buffer) dWMWSEL_c;

	instance = c;
	return (dActor_c*)c;
}

const char* EmptyWMSelectFileList[] = {NULL};
Profile WMSELECTProfile(&dWMWSEL_c::build, ProfileId::WM_SELECT, NULL, ProfileId::WM_SELECT, ProfileId::WM_SELECT, "WM World Selection", EmptyWMSelectFileList);



dWMWSEL_c::dWMWSEL_c() : acState(this, &StateID_Hidden) {
	layoutLoaded = false;
	isVisible = false;
}

int dWMWSEL_c::onCreate() {
	if(!layoutLoaded) {
		if (!layout.loadArc("worldSelector.arc", false))
			return false;

		static const char *brlanNames[16] = {
			"worldSelector_inWindow.brlan",
			"worldSelector_inArrow.brlan",
			"worldSelector_onArrow.brlan",
			"worldSelector_offArrow.brlan",
			"worldSelector_loopArrow.brlan",
			"worldSelector_hitArrow.brlan",
			"worldSelector_outArrow.brlan",
			"worldSelector_inBG.brlan",
			"worldSelector_outBG.brlan",
			"worldSelector_toBefore.brlan",
			"worldSelector_toNext.brlan",
			"worldSelector_onButton.brlan",
			"worldSelector_idleButton.brlan",
			"worldSelector_hitButton.brlan",
			"worldSelector_offButton.brlan",
			"worldSelector_outWindow.brlan",
		};

		static const char *groupNames[22] = {
			"A00_Window",
			"B00_arrowL", "B01_arrowR",
			"B00_arrowL", "B01_arrowR",
			"B00_arrowL", "B01_arrowR",
			"B00_arrowL", "B01_arrowR",
			"B00_arrowL", "B01_arrowR",
			"B00_arrowL", "B01_arrowR",
			"E00_BG",
			"E00_BG",
			"D00_page",
			"D00_page",
			"C00_selectButton",
			"C00_selectButton",
			"C00_selectButton",
			"C00_selectButton",
			"A00_Window",
		};

		static const int groupIDs[22] = {
			0,
			1, 1,
			2, 2,
			3, 3,
			4, 4,
			5, 5,
			6, 6,
			7,
			8,
			9,
			10,
			11,
			12,
			13,
			14,
			15,
		};

		layout.build("worldSelector.brlyt");
		layout.loadAnimations(brlanNames, 16);
		layout.loadGroups(groupNames, groupIDs, 22);

		layout.resetAnim(0);
		
		layout.update();
		layout.execAnimations();

		layout.disableAllAnimations();

		layout.drawOrder = 1;

		BGs[0] = layout.findPictureByName("P_worldBG_00");
		BGs[1] = layout.findPictureByName("P_worldBG_01");

		WorldNames[0] = layout.findTextBoxByName("T_worldName_00");
		WorldNames[1] = layout.findTextBoxByName("T_worldName_01");
		WorldNamesS[0] = layout.findTextBoxByName("T_worldNameS_00");
		WorldNamesS[1] = layout.findTextBoxByName("T_worldNameS_01");
		WorldIDs[0] = layout.findTextBoxByName("T_titleMulti_00");
		WorldIDs[1] = layout.findTextBoxByName("T_titleMulti_01");

		WriteBMGToTextBox( layout.findTextBoxByName("T_guideViewC_00"), GetBMG(), 2, 29, 0);

		layoutLoaded = true;
	}

	return true;
}


int dWMWSEL_c::onDelete() {
	for(int i = 0; i < 3; i++)
		FreeFile(&fileHandles[i]);

	return layout.free();
}


int dWMWSEL_c::onExecute() {
	acState.execute();

	layout.execAnimations();
	layout.update();

	return true;
}

int dWMWSEL_c::onDraw() {
	if (isVisible) {
		layout.scheduleForDrawing();
	}

	return true;
}

void dWMWSEL_c::specialDraw1() {

}


void dWMWSEL_c::show() {
	// Get which world to start on
	SaveBlock *save = GetSaveFile()->GetBlock(-1);
	int wantedSection = save->current_world;//save->newerWorldID;

	// Get which worlds are unlocked, code stole from koopatlas/starcoin.cpp
	selectedWorld = -1;
	worldCount = 0;

	// OSReport("World IDs:");

	for (int i = 0; i < dLevelInfo_c::s_info.sectionCount(); i++) {
		dLevelInfo_c::section_s *section = dLevelInfo_c::s_info.getSectionByIndex(i);

		bool haveLevels = false;
		for (int j = 0; j < section->levelCount; j++) {
			dLevelInfo_c::entry_s *l = &section->levels[j];
			if (l->flags & 2) {
				if (save->GetLevelCondition(l->worldSlot, l->levelSlot) & COND_UNLOCKED) {
					haveLevels = true;
					break;
				}
			}
		}

		if (haveLevels) {
			if (i == wantedSection) {
				selectedWorld = worldCount;
			}
			worldIDs[worldCount++] = i;
			// OSReport(" %d", i);
		}
	}

	if (selectedWorld == -1) {
		selectedWorld = 0;
	}

	// OSReport("\n");

	for(int i = 0; i < 3; i++)
		FreeFile(&fileHandles[i]);
	
	this->loadWorld(selectedWorld, true);
	this->loadWorld(selectedWorld, false);

	// Now we can start
	acState.setState(&StateID_ShowWait);
}


// Hidden
void dWMWSEL_c::beginState_Hidden() {
	layout.resetAnim(SHOW_ALL);
	layout.resetAnim(IN_BG);
	layout.resetAnim(IN_ARROW_L);
	layout.resetAnim(IN_ARROW_R);
	layout.resetAnim(TO_NEXT);
	layout.resetAnim(ON_BUTTON);

}
void dWMWSEL_c::executeState_Hidden() { }
void dWMWSEL_c::endState_Hidden() { }


// ShowWait
void dWMWSEL_c::beginState_ShowWait() {
	MapSoundPlayer(SoundRelatedClass, SE_SYS_DIALOGUE_IN, 1);

	layout.disableAllAnimations();
	layout.enableNonLoopAnim(SHOW_ALL);
	layout.enableNonLoopAnim(IN_BG);
	isVisible = true;
}
void dWMWSEL_c::executeState_ShowWait() {
	if (!layout.isAnimOn(SHOW_ALL) && !layout.isAnimOn(IN_BG)) {
		layout.enableNonLoopAnim(ON_BUTTON);

		if(selectedWorld > 0) {
			layout.enableNonLoopAnim(IN_ARROW_L);
		}
		if(selectedWorld < worldCount-1) {
			layout.enableNonLoopAnim(IN_ARROW_R);
		}

		acState.setState(&StateID_ButtonAnimWait);
	}
}
void dWMWSEL_c::endState_ShowWait() {

}


// ButtonAnimWait
void dWMWSEL_c::beginState_ButtonAnimWait() {

}
void dWMWSEL_c::executeState_ButtonAnimWait() {
	if (!layout.isAnimOn(ON_BUTTON)) {
		UpdateSelectCursor(this->layout.findPaneByName("W_Button_00"), 0, false);

		acState.setState(&StateID_WaitForInput);
	}
}
void dWMWSEL_c::endState_ButtonAnimWait() {
	
}


// WaitForInput
void dWMWSEL_c::beginState_WaitForInput() {

}
void dWMWSEL_c::executeState_WaitForInput() {
	if (!layout.isAnimOn(IN_ARROW_L) && !layout.isAnimOn(IN_ARROW_R) && !layout.isAnimOn(OUT_ARROW_L) && !layout.isAnimOn(OUT_ARROW_R)) {
		int nowPressed = Remocon_GetPressed(GetActiveRemocon());
		if(nowPressed & WPAD_LEFT && selectedWorld > 0) {
			this->moveWorld();
			selectedWorld--;
			this->loadWorld(selectedWorld, true);

			if(selectedWorld == worldCount-2) {
				layout.enableNonLoopAnim(IN_ARROW_R);
			}

			MapSoundPlayer(SoundRelatedClass, 0x7D, 1);
			layout.enableNonLoopAnim(HIT_ARROW_L);
			layout.enableNonLoopAnim(TO_BEFORE);
			acState.setState(&StateID_ArrowHit);
		}
		else if(nowPressed & WPAD_RIGHT && selectedWorld < worldCount-1) {
			this->moveWorld();
			selectedWorld++;
			this->loadWorld(selectedWorld, true);

			if(selectedWorld == 1) {
				layout.enableNonLoopAnim(IN_ARROW_L);
			}

			MapSoundPlayer(SoundRelatedClass, 0x7D, 1);
			layout.enableNonLoopAnim(HIT_ARROW_R);
			layout.enableNonLoopAnim(TO_NEXT);
			acState.setState(&StateID_ArrowHit);
		}
		else if(nowPressed & WPAD_TWO) {
			MapSoundPlayer(SoundRelatedClass, 0x639, 1);
			layout.enableNonLoopAnim(HIT_BUTTON);
			acState.setState(&StateID_Select);
		}
		else if(nowPressed & WPAD_ONE) {
			HideSelectCursor((void*)SelectCursorPointer, 0);
			MapSoundPlayer(SoundRelatedClass, SE_SYS_DIALOGUE_OUT_AUTO, 1);
			layout.enableNonLoopAnim(HIDE_ALL);
			layout.enableNonLoopAnim(OUT_BG);
			acState.setState(&StateID_Exit);
		}
	}
}
void dWMWSEL_c::endState_WaitForInput() {
	
}


// ArrowHit
void dWMWSEL_c::beginState_ArrowHit() {

}
void dWMWSEL_c::executeState_ArrowHit() {
	if (!layout.isAnimOn(HIT_ARROW_L) && !layout.isAnimOn(HIT_ARROW_R) && !layout.isAnimOn(IN_ARROW_L) && !layout.isAnimOn(IN_ARROW_R) && !layout.isAnimOn(TO_NEXT) && !layout.isAnimOn(TO_BEFORE)) {
		if(selectedWorld == 0) {
			layout.enableNonLoopAnim(OUT_ARROW_L);
		}
		else if(selectedWorld == worldCount-1) {
			layout.enableNonLoopAnim(OUT_ARROW_R);
		}

		acState.setState(&StateID_WaitForInput);
	}
}
void dWMWSEL_c::endState_ArrowHit() {
	
}
					//	 1  2  3   4  5   6    7    8   9   A   B   C   D   E
/*u8 realWorldIDs[14] = 	{0, 1, 1,  3, 3,  1,   1,   6,  12, 8,  9,  11, 10, 1};
u8 nodeIDs[14] = 		{1, 2, 41, 2, 64, 105, 129, 7,  2,  2,  1,  17, 4,  180};
u8 NWIDs[14] = 			{1, 2, 3,  4, 5,  6,   7,   8,  9,  10, 11, 12, 13, 14};
u8 musicIDs[14] = 		{1, 2, 3,  4, 5,  6,   7,   9 , 18, 12, 13, 17, 14, 16};
u32 FSTextColour0[14] = 	{0xFFFF99FF, 0xFF9E46FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF00FF, 0xFFFFFFFF, 0xF8B878FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xF0D8ACFF};
u32 FSTextColour1[14] = 	{0x1FB423FF, 0x663300FF, 0xA37413FF, 0xFF80C0FF, 0x00FFFFFF, 0xF79400FF, 0x9AA7B6FF, 0xCC6600FF, 0xFF9900FF, 0xCE6906FF, 0x99D704FF, 0xB8B8B8FF, 0x9134C6FF, 0x333333FF};
u32 FSHintColour0[14] = 	{0x173714FF, 0x6B623BFF, 0x493521FF, 0x650916FF, 0x083D78FF, 0x2A1B36FF, 0x4AD1CAFF, 0x262626FF, 0x365B18FF, 0x3D0000FF, 0x2B4A1FFF, 0x333333FF, 0x763584FF, 0x2D5017FF};
u32 FSHintColour1[14] = 	{0x3C9135FF, 0xBFAF82FF, 0xA77D53FF, 0xC65B68FF, 0x298DF2FF, 0x6C3D94FF, 0xC6F1ECFF, 0x871F1FFF, 0xA5E079FF, 0xD4500BFF, 0xA6ED14FF, 0x996655FF, 0x267088FF, 0x53BC2BFF};
u32 HUDTextColours0[14] = 	{0xFFFF99FF, 0xFF9E46FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF00FF, 0xFFFFFFFF, 0xF8B878FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xF0D8ACFF};
u32 HUDTextColours1[14] = 	{0x1FB423FF, 0x663300FF, 0xA37413FF, 0xFF80C0FF, 0x00FFFFFF, 0xF79400FF, 0x9AA7B6FF, 0xCC6600FF, 0xFF9900FF, 0xCE6906FF, 0x99D704FF, 0xB8B8B8FF, 0x9134C6FF, 0x333333FF};
s16 HUDHSL[14][3] = {{117, 46, 11}, {59, 33, 16}, {19, 40, -9}, {350, 63, 11}, {201, 93, 12}, {273, 45, -15}, {180, 79, 18}, {0, 60, 3}, {26, 80, 12}, {31, 100, 0}, {77, 100, 0}, {20, 50, -28}, {271, 69, 8}, {90, 67, -29}};
u8 titlescreenLevels[14][2] = {{0, 10}, {0, 11}, {0, 13}, {1, 10}, {1, 11}, {1, 13}, {2, 7}, {2, 9}, {3, 9}, {2, 11}, {2, 12}, {3, 11}, {2, 13}, {3, 8}};
const char* worldNames[14] = {
	"Yoshi's Island",
	"Rubble Ruins",
	"Mushroom Peaks",
	"Sakura Village",
	"Freezeflame Glacier",
	"Pumpkin Boneyard",
	"Sky Mountain",
	"Koopa Planet",
	"Special World",
	"Goldwood Forest",
	"Mini-Mega Island",
	"Sky City",
	"Crystal Caves",
	"Bombard Cliffs",
};
const wchar_t* wworldNames_t[14] = {
	L"Yoshi's Island",
	L"Rubble Ruins",
	L"Mushroom Peaks",
	L"Sakura Village",
	L"Freezeflame Glacier",
	L"Pumpkin Boneyard",
	L"Sky Mountain",
	L"Koopa Planet",
	L"Special World",
	L"Goldwood Forest",
	L"Mini-Mega Island",
	L"Sky City",
	L"Crystal Caves",
	L"Bombard Cliffs",
};*/

//#include "worldinfo.h"


void dWMWSEL_c::moveWorld() {
	FreeFile(&fileHandles[1]);

	fileHandles[1].filePtr = fileHandles[2].filePtr;
	fileHandles[1].length = fileHandles[2].length;
	this->BGs[0]->material->texMaps[0].ReplaceImage((TPLPalette*)fileHandles[1].filePtr, 0);

	this->WorldNames[0]->SetString(this->WorldNames[1]->stringBuf);
	this->WorldNamesS[0]->SetString(this->WorldNamesS[1]->stringBuf);

	this->WorldNames[0]->colour1 = this->WorldNames[1]->colour1;
	this->WorldNames[0]->colour2 = this->WorldNames[1]->colour2;
	this->WorldNamesS[0]->colour1 = this->WorldNamesS[1]->colour1;
	this->WorldNamesS[0]->colour2 = this->WorldNamesS[1]->colour2;

	this->WorldIDs[0]->SetString(this->WorldIDs[1]->stringBuf);

	this->WorldIDs[0]->colour1 = this->WorldIDs[1]->colour1;
	this->WorldIDs[0]->colour2 = this->WorldIDs[1]->colour2;
}

void dWMWSEL_c::loadWorld(u32 worldNum, bool isLeft) {
	/*u32 currentWorld = worldIDs[worldNum]-1;
	WorldInfo *worldInfo = dWorldInfo_c::instance->getWorldInfo(currentWorld);

	// World BG
	char bgName[32];
	sprintf(bgName, "/Worlds/BG%d.tpl", currentWorld);

	TPLPalette *bg = (TPLPalette*)LoadFile(&fileHandles[1+isLeft], bgName);
	BGs[isLeft]->material->texMaps[0].ReplaceImage(bg, 0);

	// World Name Text
	const char *worldName = dWorldInfo_c::instance->getNameForWorld(currentWorld);
	wchar_t wworldName_t[32];
	mbstowcs(wworldName_t, worldName, 32);
	this->WorldNames[isLeft]->SetString(wworldName_t);
	this->WorldNamesS[isLeft]->SetString(wworldName_t);

	// World Name Color
	this->WorldNames[isLeft]->colour1 = worldInfo->HUDTextColour0;
	this->WorldNames[isLeft]->colour2 = worldInfo->HUDTextColour1;

	// World ID Text
	char worldID[32];
	wchar_t wworldID_t[32];
	sprintf(worldID, "World %X", currentWorld+1);
	mbstowcs(wworldID_t, worldID, 32);
	this->WorldIDs[isLeft]->SetString(wworldID_t);

	// World ID Color
	this->WorldIDs[isLeft]->colour1 = worldInfo->FSTextColour0;
	this->WorldIDs[isLeft]->colour2 = worldInfo->FSTextColour1;*/
}


// Select
void dWMWSEL_c::beginState_Select() {
	/*u32 selworld = worldIDs[selectedWorld]-1;
	WorldInfo *worldInfo = dWorldInfo_c::instance->getWorldInfo(selworld);

	SaveBlock *save = GetSaveFile()->GetBlock(-1);
	save->current_world = worldInfo->worldmapID;
	save->current_path_node = worldInfo->nodeID;

	strncpy(save->newerWorldName, dWorldInfo_c::instance->getNameForWorld(selworld), 32);
	save->newerWorldName[31] = 0;
	save->newerWorldID = worldInfo->NWID;
	save->currentMapMusic = worldInfo->musicID;

	save->fsTextColours[0] = worldInfo->FSTextColour0;
	save->fsHintColours[0] = worldInfo->FSHintColour0;
	save->hudTextColours[0] = worldInfo->HUDTextColour0;
	save->fsTextColours[1] = worldInfo->FSTextColour1;
	save->fsHintColours[1] = worldInfo->FSHintColour1;
	save->hudTextColours[1] = worldInfo->HUDTextColour1;

	save->hudHintH = worldInfo->HUDH;
	save->hudHintS = worldInfo->HUDS;
	save->hudHintL = worldInfo->HUDL;

	if (!(save->titleScreenWorld == 3 && save->titleScreenLevel == 10)) {
		save->titleScreenWorld = worldInfo->titlescreenWorld;
		save->titleScreenLevel = worldInfo->titlescreenLevel;
	}

	dScKoopatlas_c::instance->keepMusicPlaying = true;
	dKPMusic::play(0);*/

	ActivateWipe(WIPE_CIRCLE);
	DoSceneChange(WORLD_MAP, 0x00000500, 0);
}

void dWMWSEL_c::executeState_Select() {
	
}

void dWMWSEL_c::endState_Select() {
	
}


// Exit
void dWMWSEL_c::beginState_Exit() {

}
void dWMWSEL_c::executeState_Exit() {
	if (!layout.isAnimOn(HIDE_ALL) && !layout.isAnimOn(OUT_BG)) {
		this->isVisible = false;
		
		acState.setState(&StateID_Hidden);
	}
}
void dWMWSEL_c::endState_Exit() {
	
}



void makeWorldSelectionShowUp() {
	dWMWSEL_c::instance->isVisible = true;
}