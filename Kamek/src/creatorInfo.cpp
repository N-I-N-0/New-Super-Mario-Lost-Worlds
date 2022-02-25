// TODO:  - 0x80926ba0 LoadFilesForWorldMap: re-add lakitu and items
//          - Warning: eventually the poweruplist was already changed before in boomeranghax, poweruphax or some stockItemFix file ...
//                     WorldMap folder list handled in worldmapGrid.S
//          - also add all new items
//        - The cursor doesn't get deleted together with the rest of the actor
//        - eventually make the shop have 2 pages with the first one being a new stockItem and the second the shop
//          - in this case bind the spawning of this actor to button 1
//          - no need for a mushroom house shop on the map
//        - unhardcode shop names and items
//          - load from planned worldInfo.bin
//        - re-add texmapcolouriser functionality with colours from worldInfo.bin
//        - check why the game crashes after the following sequence of events:
//          - open shop while on level node
//          - press 2 -> buying something while starting the level
//          - finish the level
//          - try to re-open/spawn the shop
//          >> probably the problem is that the bool containing the info whether the shop is spawned is true while there is no shop
//             the code in worldmapGird.cpp then tries to delete the non existend shop and crashes the game ...

#ifndef __CREATOR_INFO_H
#define __CREATOR_INFO_H

//#include "koopatlas/core.h"
#include "texmapcolouriser.h"

class dCreatorInfo_c : public dActor_c {
	public:
		static dActor_c *build();
		static dCreatorInfo_c *instance;

		dCreatorInfo_c();

		int onCreate();
		int onDelete();
		int onExecute();
		int onDraw();

		bool layoutLoaded;
		m2d::EmbedLayout_c layout;

		bool visible;
		
		bool showLevelCredits;
		
		FileHandle fileHandles[5];
   		//BonusLevelsBinary *bonusLevelsFile;

   		TPLPalette *sampleOne, *sampleTwo;
   		TPLPalette *pfpOne, *pfpTwo;

		u32 currentStateID;

		u32 currentPage;

		int timer;

		nw4r::lyt::Picture *Samples[2];
		nw4r::lyt::Picture *PFPs[2];
		nw4r::lyt::TextBox *LevelNames[2];
		nw4r::lyt::TextBox *AuthorNames[2];
		nw4r::lyt::TextBox *AuthorQuotes[2];

		enum Animaiton {
			HIDE_ALL = 29, //BonusCourseSelect_outWindow
			SHOW_ALL = 0, //BonusCourseSelect_inWindow
		};

		void show(int shopNumber);

		void loadInfo();

		dStateWrapper_c<dCreatorInfo_c> state;

		USING_STATES(dCreatorInfo_c);
		DECLARE_STATE(Hidden);
		DECLARE_STATE(ShowWait);
		DECLARE_STATE(ButtonActivateWait);
		DECLARE_STATE(Wait);
		DECLARE_STATE(HideWait);
};

#endif

const char* CreatorFileList[] = {NULL};
Profile CreatorInfoProfile(&dCreatorInfo_c::build, ProfileId::CreatorInfo, NULL, ProfileId::CreatorInfo, ProfileId::CreatorInfo, "CreatorInfo", CreatorFileList);


extern int PtrToWM_CS_SEQ_MNG;
extern "C" bool FUN_801017c0(int, int, int, int, int);
extern "C" void FUN_808fbd10(int);
extern "C" void dCourseSelectGuide_c__loadLives(int);

CREATE_STATE(dCreatorInfo_c, Hidden);
CREATE_STATE(dCreatorInfo_c, ShowWait);
CREATE_STATE(dCreatorInfo_c, ButtonActivateWait);
CREATE_STATE(dCreatorInfo_c, Wait);
CREATE_STATE(dCreatorInfo_c, HideWait);



dCreatorInfo_c *dCreatorInfo_c::instance = 0;

dActor_c *dCreatorInfo_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dCreatorInfo_c));
	dCreatorInfo_c *c = new(buffer) dCreatorInfo_c;

	instance = c;
	return c;
}

dCreatorInfo_c::dCreatorInfo_c() : state(this, &StateID_Hidden) {
	layoutLoaded = false;
	visible = false;
}

int dCreatorInfo_c::onCreate() {
	if(!layoutLoaded) {
		OSReport("loading...\n");
		bool gotFile = layout.loadArc("BonusCourseSelect.arc", false);
		if (!gotFile)
			return false;
		
		OSReport("gotFile!\n");

		static const char *brlanNames[18] = {
			"BonusCourseSelect_inWindow.brlan",
			"BonusCourseSelect_inPause.brlan",
			"BonusCourseSelect_outPause.brlan",
			"BonusCourseSelect_inArrow.brlan",
			"BonusCourseSelect_onArrow.brlan",
			"BonusCourseSelect_offArrow.brlan",
			"BonusCourseSelect_loopArrow.brlan",
			"BonusCourseSelect_hitArrow.brlan",
			"BonusCourseSelect_outArrow.brlan",
			"BonusCourseSelect_toBefore.brlan",
			"BonusCourseSelect_toNext.brlan",
			"BonusCourseSelect_outWindow.brlan",
			"BonusCourseSelect_loopBG.brlan",
			"BonusCourseSelect_inButton.brlan",
			"BonusCourseSelect_onButton.brlan",
			"BonusCourseSelect_idleButton.brlan",
			"BonusCourseSelect_hitButton.brlan",
			"BonusCourseSelect_offButton.brlan",
		};

		static const char *groupNames[36] = {
			"A00_Window",
			"D00_Pause",
			"D00_Pause",
			"E00_arrowL", "E01_arrowR", "E02_arrowCL", "E03_arrowCR",
			"E00_arrowL", "E01_arrowR", "E02_arrowCL", "E03_arrowCR",
			"E00_arrowL", "E01_arrowR", "E02_arrowCL", "E03_arrowCR",
			"E00_arrowL", "E01_arrowR", "E02_arrowCL", "E03_arrowCR",
			"E00_arrowL", "E01_arrowR", "E02_arrowCL", "E03_arrowCR",
			"E00_arrowL", "E01_arrowR", "E02_arrowCL", "E03_arrowCR",
			"F00_page",
			"F00_page",
			"A00_Window",
			"C00_BG",
			"B02_centerButton",
			"B02_centerButton",
			"B02_centerButton",
			"B02_centerButton",
			"B02_centerButton",
		};

		static const int groupIDs[36] = {
			0,
			1,
			2,
			3, 3, 3, 3,
			4, 4, 4, 4,
			5, 5, 5, 5,
			6, 6, 6, 6,
			7, 7, 7, 7, 
			8, 8, 8, 8, 
			9,
			10,
			11,
			12,
			13,
			14,
			15,
			16,
			17,
		};

		layout.build("BonusCourseSelect.brlyt");
		OSReport("Build layout!\n");
		//layout.loadAnimations(brlanNames, 18);
		OSReport("Loaded Animations!\n");
		layout.loadGroups(groupNames, groupIDs, 36);
		OSReport("Loaded Groups!\n");
		layout.disableAllAnimations();
		OSReport("Disabled all animations!\n");

		// layout.drawOrder = 140;

		layoutLoaded = true;

		Samples[0] = layout.findPictureByName("P_LevelSample_00");
		Samples[1] = layout.findPictureByName("P_LevelSample_01");
		PFPs[0] = layout.findPictureByName("P_author_00");
		PFPs[1] = layout.findPictureByName("P_author_01");

		OSReport("found something 1\n");

		LevelNames[0] = layout.findTextBoxByName("T_titleMulti_00");
		LevelNames[1] = layout.findTextBoxByName("T_titleMulti_01");
		AuthorNames[0] = layout.findTextBoxByName("T_author_00");
		AuthorNames[1] = layout.findTextBoxByName("T_author_01");
		AuthorQuotes[0] = layout.findTextBoxByName("T_quote_01");
		AuthorQuotes[1] = layout.findTextBoxByName("T_quote_00");

		OSReport("found something 2\n");

		//WriteBMGToTextBox( layout.findTextBoxByName("T_guideViewC_00"), GetBMG(), 2, 29, 0);

		// if (IsWideScreen()) {
		// 	N_flipbook_00->scale.x *= 1.372693726937269f;
		// }

		//layout.enableLoopAnim(30);
		OSReport("enable anim 30\n");

		//layout.enableNonLoopAnim(0); // inWindow
		OSReport("enable anim 0\n");
		//layout.enableNonLoopAnim(31); // inButton
		OSReport("enable anim 31\n");


   		//bonusLevelsFile = (BonusLevelsBinary*)LoadFile(&fileHandles[0], "/NewerRes/BonusLevels.bin");
		//this->loadLevel(0, false);
		//this->loadLevel(0, true);

		// MakeScene(currentHeap, 0x24);
		// LoadSceneLights(this, 1);
		//LoadMapScene();
		//GameSetup__LoadScene(0); // "lol, stolen from GAME_SETUP" -Ninji, before 2013 probably :p

		OSReport("loaded!\n");
	}

	return true;
}


int dCreatorInfo_c::onDelete() {
	for(int i = 0; i < 5; i++)
		FreeFile(&fileHandles[i]);
	return layout.free();
}


int dCreatorInfo_c::onExecute() {
	state.execute();

	layout.execAnimations();
	layout.update();

	return true;
}

int dCreatorInfo_c::onDraw() {
	if (visible)
		layout.scheduleForDrawing();

	return true;
}


void dCreatorInfo_c::show(int shopNumber) {
	//shopKind = shopNumber;
	state.setState(&StateID_ShowWait);
}

void makeLevelCreditsShowUp() {
	dCreatorInfo_c::instance->showLevelCredits = true;
}

bool isShakingForCredits() {
	Remocon* rem = GetActiveRemocon();
	if(rem->controllerType == 0) {	//Wiimote
		OSReport("isShaking: %d\n", rem->isShaking);
		return rem->isShaking > 0;
	} else {						// == 1 <-> Wiimote + Nunchuck
		return rem->isShaking > 0;
	}
}

// Hidden
void dCreatorInfo_c::beginState_Hidden() { }
void dCreatorInfo_c::executeState_Hidden() {
	if (showLevelCredits) {
		showLevelCredits = false;
		state.setState(&StateID_ShowWait);
		FUN_801017c0(PtrToWM_CS_SEQ_MNG, 0x35, 0, 0, 0x80);
		//dActor_c* csMng = (dActor_c*)fBase_c::search(COURSE_SELECT_MANAGER);
		//*(u8*)((int)(csMng) + 0x53C) = 0;					//hide gameScene
	}
}
void dCreatorInfo_c::endState_Hidden() { }

// ShowWait
void dCreatorInfo_c::beginState_ShowWait() {
	MapSoundPlayer(SoundRelatedClass, SE_SYS_DIALOGUE_IN, 1);

	layout.disableAllAnimations();
	layout.enableNonLoopAnim(SHOW_ALL);
	visible = true;

	loadInfo();
}
void dCreatorInfo_c::executeState_ShowWait() {

}
void dCreatorInfo_c::endState_ShowWait() {
	MapSoundPlayer(SoundRelatedClass, SE_OBJ_CLOUD_BLOCK_TO_JUGEM, 1);
	timer = 1;
}

// ButtonActivateWait
void dCreatorInfo_c::beginState_ButtonActivateWait() { }
void dCreatorInfo_c::executeState_ButtonActivateWait() {
	if (!layout.isAnyAnimOn())
		state.setState(&StateID_Wait);
}
void dCreatorInfo_c::endState_ButtonActivateWait() { }

// Wait
void dCreatorInfo_c::beginState_Wait() {
	//showSelectCursor();
}
void dCreatorInfo_c::executeState_Wait() {
	int nowPressed = Remocon_GetPressed(GetActiveRemocon());
	if (nowPressed & WPAD_ONE) {
		state.setState(&StateID_HideWait);
	}
}
void dCreatorInfo_c::endState_Wait() { }

// HideWait
void dCreatorInfo_c::beginState_HideWait() {
	MapSoundPlayer(SoundRelatedClass, SE_SYS_DIALOGUE_OUT_AUTO, 1);
	layout.enableNonLoopAnim(HIDE_ALL);
	//layout.enableNonLoopAnim(DEACTIVATE_BUTTON+selected);

	timer = 26;
	MapSoundPlayer(SoundRelatedClass, SE_OBJ_CS_KINOHOUSE_DISAPP, 1);

	HideSelectCursor(SelectCursorPointer, 0);
}
void dCreatorInfo_c::executeState_HideWait() {
	if (timer > 0) {
		timer--;
	} else {
		dActor_c* csMng = (dActor_c*)fBase_c::search(COURSE_SELECT_MANAGER);
		dActor_c* wmDirector = (dActor_c*)fBase_c::search(WM_DIRECTOR);
		//*(u8*)((int)(csMng) + 0x53C) = 1;			//unhide gameScene
		FUN_808fbd10((int)wmDirector);				//unfreeze map
	}

	if (!layout.isAnimOn(HIDE_ALL))
		state.setState(&StateID_Hidden);
}
void dCreatorInfo_c::endState_HideWait() {
	visible = false;
}





void dCreatorInfo_c::loadInfo() {
	/*SaveBlock *save = GetSaveFile()->GetBlock(-1);

	//missing color information in original save file!
	s16 hue        = 138;
	s8  saturation = 50;
	s8  lightness  = 30;
	
	//leftCol.colourise(save->hudHintH, save->hudHintS, save->hudHintL);
	//midCol.colourise(save->hudHintH, save->hudHintS, save->hudHintL);
	//rightCol.colourise(save->hudHintH, save->hudHintS, save->hudHintL);
	leftCol.colourise(hue, saturation, lightness);
	midCol.colourise(hue, saturation, lightness);
	rightCol.colourise(hue, saturation, lightness);

	// find out the shop name
	dLevelInfo_c::entry_s *shopNameEntry =
		dLevelInfo_c::s_info.searchBySlot(shopKind, 98);

	wchar_t shopName[100];
	// TODO: refactor this a bit
	const char *sourceName = dLevelInfo_c::s_info.getNameForLevel(shopNameEntry);
	int charCount = 0;
	
	while (*sourceName != 0 && charCount < 99) {
		shopName[charCount] = *sourceName;
		sourceName++;
		charCount++;
	}
	shopName[charCount] = 0;

	Title->SetString(shopName);
	TitleShadow->SetString(shopName);

	// load the coin count
	int scCount = getUnspentStarCoinCount();
	WriteNumberToTextBox(&scCount, CoinCount, false);
	WriteNumberToTextBox(&scCount, CoinCountShadow, false);


	WriteBMGToTextBox(BackText, GetBMG(), 2, 58, 0);
	WriteBMGToTextBox(BuyText, GetBMG(), 302, 4, 0);*/
}