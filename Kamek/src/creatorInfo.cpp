#include "courseSelectManager.h";

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


const char* CreatorFileList[] = {NULL};
Profile CreatorInfoProfile(&dCreatorInfo_c::build, ProfileId::CreatorInfo, NULL, ProfileId::CreatorInfo, ProfileId::CreatorInfo, "CreatorInfo", CreatorFileList);


extern int PtrToWM_CS_SEQ_MNG;
extern "C" bool FUN_801017c0(int, int, int, int, int);
extern "C" void FUN_808fbd10(int);
extern "C" void dCourseSelectGuide_c__loadLives(int);

CREATE_STATE(dCreatorInfo_c, Hidden);
CREATE_STATE(dCreatorInfo_c, ShowWait);
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

		static const char *brlanNames[] = {
			"BonusCourseSelect_inWindow.brlan",
			"BonusCourseSelect_outWindow.brlan",
		};

		static const char *groupNames[] = {
			"A00_Window",
			"A00_Window",
		};

		static const int groupIDs[] = {
			0,
			1,
		};

		
		layout.build("BonusCourseSelect.brlyt");
		layout.loadAnimations(brlanNames, 2);
		layout.loadGroups(groupNames, groupIDs, 2);
		layout.disableAllAnimations();

		layoutLoaded = true;

		Sample = layout.findPictureByName("P_LevelSample_00");
		LevelName = layout.findTextBoxByName("T_titleMulti_00");
		AuthorNames = layout.findTextBoxByName("T_author_00");
		AuthorQuote = layout.findTextBoxByName("T_quote_00");

		WriteBMGToTextBox( layout.findTextBoxByName("T_guideViewC_00"), GetBMG(), 2, 29, 0);

		layout.enableNonLoopAnim(SHOW_ALL);

		if(IsWideScreen()) {
			nw4r::lyt::Pane* back = layout.findPaneByName("N_something_00");
			back->trans.x -= 75;
		}

		OSReport("loaded!\n");
	}

	return true;
}


int dCreatorInfo_c::onDelete() {
	for(int i = 0; i < 3; i++)
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


void dCreatorInfo_c::show() {
	state.setState(&StateID_ShowWait);
}

void makeLevelCreditsShowUp() {
	dCreatorInfo_c::instance->showLevelCredits = true;
}

// Hidden
void dCreatorInfo_c::beginState_Hidden() { }
void dCreatorInfo_c::executeState_Hidden() {
	if (showLevelCredits) {
		showLevelCredits = false;
		state.setState(&StateID_ShowWait);
		FUN_801017c0(PtrToWM_CS_SEQ_MNG, 0x35, 0, 0, 0x80);
	}
}
void dCreatorInfo_c::endState_Hidden() { }

// ShowWait
void dCreatorInfo_c::beginState_ShowWait() {
	layout.disableAllAnimations();
	layout.enableNonLoopAnim(SHOW_ALL);
	visible = true;

	loadInfo();
}
void dCreatorInfo_c::executeState_ShowWait() {
	if (!layout.isAnyAnimOn())
		state.setState(&StateID_Wait);
}
void dCreatorInfo_c::endState_ShowWait() {
	timer = 1;
}

// Wait
void dCreatorInfo_c::beginState_Wait() {
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
}
void dCreatorInfo_c::executeState_HideWait() {
	if (!layout.isAnimOn(HIDE_ALL)) {
		dActor_c* wmDirector = (dActor_c*)fBase_c::search(WM_DIRECTOR);
		FUN_808fbd10((int)wmDirector);				//unfreeze map

		state.setState(&StateID_Hidden);
		OSReport("HideWait change state to Hidden\n");
	}
}
void dCreatorInfo_c::endState_HideWait() {
	visible = false;
}



extern void mbstowcs(wchar_t *destination, const char *source, size_t count);

void dCreatorInfo_c::loadInfo() {
	// Get Node Number & Level ID
	uint worldNumber = getLevelInfoWorldNumber(CurrentWorldNumForWorldMap, CurrentWorldNumForWorldMapSub);
	u8 CurrentNodeNum = *(u8*)((int)(dCourseSelectManager_c::instance) + 0x4D7);
	u8 actualLevel = getActualLevelNum(CurrentNodeNum);

	OSReport("Gathering information about level %02d-%02d\n", worldNumber+1, actualLevel+1);


	char creditsFileName[64];
	wchar_t convertedCreditsText1[100];
	wchar_t convertedCreditsText2[100];

	// Load the level designers' names
	sprintf(creditsFileName, "/LevelCredits/%02d-%02da.txt", worldNumber+1, actualLevel+1);
	const char* creditsText1 = (const char*)LoadFile(&fileHandles[0], creditsFileName);
	if(creditsText1) {
		mbstowcs(convertedCreditsText1, creditsText1, fileHandles[0].length);

		AuthorNames->SetString(convertedCreditsText1);
		FreeFile(&fileHandles[0]);
	} else {
		const wchar_t* noCreditsFile = L"You should\nnot see this\nhere!\nReport this\non Discord\nplease!";
		AuthorNames->SetString(noCreditsFile);
	}
	
	// Load the some comment for the level
	sprintf(creditsFileName, "/LevelCredits/%02d-%02db.txt", worldNumber+1, actualLevel+1);
	const char* creditsText2 = (const char*)LoadFile(&fileHandles[0], creditsFileName);
	if(creditsText2) {
		mbstowcs(convertedCreditsText2, creditsText2, fileHandles[0].length);

		AuthorQuote->SetString(convertedCreditsText2);
		FreeFile(&fileHandles[0]);
	} else {
		const wchar_t* noCreditsFile = L"";
		AuthorQuote->SetString(noCreditsFile, 0);
	}
	
	
	
	
	// Find the level name
	dLevelInfo_c::entry_s *levelNameEntry = dLevelInfo_c::s_info.searchBySlot(worldNumber+1, actualLevel+1);

	if(levelNameEntry) {
		wchar_t levelNameStr[32];
		const char *sourceName = dLevelInfo_c::s_info.getNameForLevel(levelNameEntry);
		mbstowcs(levelNameStr, sourceName, 32);

		LevelName->SetString(levelNameStr);
	}

	// Change the Level Preview Picture
	if(fileHandles[1].filePtr) // Free the previous one if there was any
		FreeFile(&fileHandles[1]);

	char sampleName[64];
	sprintf(sampleName, "/LevelSamples/%02d-%02d.tpl", worldNumber+1, actualLevel+1);

	TPLPalette *sample = (TPLPalette*)LoadFile(&fileHandles[1], sampleName);
	if(sample) {
		Sample->material->texMaps[0].ReplaceImage(sample, 0);
	} else {
		TPLPalette *error = (TPLPalette*)LoadFile(&fileHandles[1], "/LevelSamples/error.tpl");
		if(error) {
			Sample->material->texMaps[0].ReplaceImage(error, 0);
		}
	}
}