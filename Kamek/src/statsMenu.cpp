#include <common.h>
#include <game.h>
#include "sfx.h"

extern int globalCoins;
extern char CurrentLevel, CurrentWorld;

class dStatsMenu_c : public dStageActor_c {
	public:
		static dActor_c *build();
		static dStatsMenu_c *instance;

		dStatsMenu_c();

		int onCreate();
		int onDelete();
		int onExecute();
		int onDraw();

		int beforeExecute() { return true; }
		int afterExecute(int) { return true; }
		int beforeDraw() { return true; }
		int afterDraw(int) { return true; }

		void goMap();
		void goAgain();
		void goReplay();
		void switchButtons(int oldButton, int newButton);
		void setStats();
		void activate(int pScene, int pSceneParams, int pPowerupStoreType, int pWipe);
		bool wasActiveAlready;

		m2d::EmbedLayout_c layout;
		bool layoutLoaded;

		bool visible;
		float scaleEase;
		int timer;

		int count;
		int autoselectCountdown;

		int* structWithWorldData;

		int selected;
		
		int coinsAtStart;
		
		int scene, sceneParams, powerupStoreType, wipe;

		nw4r::lyt::TextBox
			*coinCount;

		nw4r::lyt::TextBox
			*Countdown;

		nw4r::lyt::Picture
			*BtnLeft[3], *BtnMid[3], *BtnRight[3];

		nw4r::lyt::TexMap
			*selectedMapLeft, *selectedMapMid, *selectedMapRight,
			*notSelectedMapLeft, *notSelectedMapMid, *notSelectedMapRight;

		nw4r::lyt::Picture
			*starCoinCircle, *starCoin;

		nw4r::lyt::Pane
			*Buttons[3];

		dStateWrapper_c<dStatsMenu_c> state;

		USING_STATES(dStatsMenu_c);
		DECLARE_STATE(Hidden);
		DECLARE_STATE(WaitForWipe);
		DECLARE_STATE(CountdownWait);
		DECLARE_STATE(ShowWait);
		DECLARE_STATE(Wait);
};

CREATE_STATE(dStatsMenu_c, Hidden);
CREATE_STATE(dStatsMenu_c, WaitForWipe);
CREATE_STATE(dStatsMenu_c, CountdownWait);
CREATE_STATE(dStatsMenu_c, ShowWait);
CREATE_STATE(dStatsMenu_c, Wait);

dStatsMenu_c *dStatsMenu_c::instance = 0;
dActor_c *dStatsMenu_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dStatsMenu_c));
	dStatsMenu_c *c = new(buffer) dStatsMenu_c;

	instance = c;
	return c;
}

const char *StatsMenuFileList[] = {0};
const SpriteData StatsMenuSpriteData = { ProfileId::NumPad, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile StatsMenuProfile(&dStatsMenu_c::build, SpriteId::StatsMenu, &StatsMenuSpriteData, ProfileId::StatsMenu, ProfileId::StatsMenu, "StatsMenu", StatsMenuFileList);


#define ANIM_WINDOW_IN 0
#define ANIM_WINDOW_LOOP 1
#define ANIM_WINDOW_OUT 2
#define ANIM_BUTTON_HIT 3 //3, 4, 5
#define ANIM_BUTTON_IDLE 6 //6, 7, 8
#define ANIM_BUTTON_OFF 9 //9, 10, 11
#define ANIM_BUTTON_ON 12 //12, 13, 14


dStatsMenu_c::dStatsMenu_c() : state(this, &StateID_Hidden) {
	layoutLoaded = false;
	visible = false;
}

int dStatsMenu_c::onCreate() {
	if(RESTART_CRSIN_LevelStartStruct.isReplay) this->Delete(1);
	wasActiveAlready = false;
	
	count = 180;
	autoselectCountdown = 180;

	coinsAtStart = globalCoins;
	
	if (!layoutLoaded) {
		bool gotFile = layout.loadArc("statsMenu.arc", false);
		if (!gotFile)
			return false;

		selected = 0;

		layout.build("statsMenu.brlyt");

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

		static const char *brlanNames[] = {
			"statsMenu_hitButton.brlan",
			"statsMenu_idleButton.brlan",
			"statsMenu_inWindow.brlan",
			"statsMenu_loopWindow.brlan",
			"statsMenu_offButon.brlan",
			"statsMenu_onButton.brlan",
			"statsMenu_outWindow.brlan",
		};

		static const char *groupNames[] = {
			"A00_inWindow",     "A00_inWindow",   "A00_inWindow", 
			"B00_tuzukuButton", "B02_menuButton", "Group_00",
			"B00_tuzukuButton", "B02_menuButton", "Group_00",
			"B00_tuzukuButton", "B02_menuButton", "Group_00",
			"B00_tuzukuButton", "B02_menuButton", "Group_00",
		};
		static const int brlanIDs[] = {
			2,  3,  6,  //A00_inWindow (in, loop, out)
			0,  0,  0,  //Buttons      (hit)
			1,  1,  1,  //Buttons      (idle)
			4,  4,  4,  //Buttons      (off)
			5,  5,  5,  //Buttons      (on)
		};

		layout.loadAnimations(brlanNames, 7);
		layout.loadGroups(groupNames, brlanIDs, 15);
		layout.disableAllAnimations();
		
		layout.drawOrder = 140;
		
		
		BtnLeft[0]  = layout.findPictureByName("Btn1_Left");
		BtnMid[0]   = layout.findPictureByName("Btn1_Middle");
		BtnRight[0] = layout.findPictureByName("Btn1_Right");
		BtnLeft[1]  = layout.findPictureByName("Btn2_Left");
		BtnMid[1]   = layout.findPictureByName("Btn2_Middle");
		BtnRight[1] = layout.findPictureByName("Btn2_Right");
		BtnLeft[2]  = layout.findPictureByName("Btn2_Left_00");
		BtnMid[2]   = layout.findPictureByName("Btn2_Middle_00");
		BtnRight[2] = layout.findPictureByName("Btn2_Right_00");
		
		selectedMapLeft = BtnLeft[0]->material->texMaps;
		selectedMapMid = BtnMid[0]->material->texMaps;
		selectedMapRight = BtnRight[0]->material->texMaps;
		notSelectedMapLeft = BtnLeft[1]->material->texMaps;
		notSelectedMapMid = BtnMid[1]->material->texMaps;
		notSelectedMapRight = BtnRight[1]->material->texMaps;
		
		Buttons[0] = layout.findPictureByName("P_SBBase_00");
		Buttons[1] = layout.findPictureByName("P_SBBase_02");
		Buttons[2] = layout.findPictureByName("P_SBBase_01");
		
		Countdown = layout.findTextBoxByName("Countdown");
		coinCount = layout.findTextBoxByName("coinCount");
		
		layoutLoaded = true;
	}

	visible = false;

	return true;
}

void dStatsMenu_c::switchButtons(int oldButton, int newButton) {
	BtnLeft[oldButton]->material->texMaps = notSelectedMapLeft;
	BtnMid[oldButton]->material->texMaps = notSelectedMapMid;
	BtnRight[oldButton]->material->texMaps = notSelectedMapRight;
	BtnLeft[newButton]->material->texMaps = selectedMapLeft;
	BtnMid[newButton]->material->texMaps = selectedMapMid;
	BtnRight[newButton]->material->texMaps = selectedMapRight;
	
	layout.enableNonLoopAnim(ANIM_BUTTON_OFF+oldButton);
	layout.enableNonLoopAnim(ANIM_BUTTON_ON+newButton);	
}

void dStatsMenu_c::setStats() {
	if (dGameDisplay_c::instance->layout.findPictureByName("P_collection_00")->flag == 1) {
		starCoinCircle = layout.findPictureByName("starCoinCircle1");
		starCoin = layout.findPictureByName("starCoin1");
		starCoinCircle->SetVisible(false);
		starCoin->SetVisible(true);
	}
	if (dGameDisplay_c::instance->layout.findPictureByName("P_collection_01")->flag == 1) {
		starCoinCircle = layout.findPictureByName("starCoinCircle2");
		starCoin = layout.findPictureByName("starCoin2");
		starCoinCircle->SetVisible(false);
		starCoin->SetVisible(true);
	}
	if (dGameDisplay_c::instance->layout.findPictureByName("P_collection_02")->flag == 1) {
		starCoinCircle = layout.findPictureByName("starCoinCircle3");
		starCoin = layout.findPictureByName("starCoin3");
		starCoinCircle->SetVisible(false);
		starCoin->SetVisible(true);
	}
	
	int collectedCoins = globalCoins-coinsAtStart;
	wchar_t textCount[9];
	sprintf((char*)textCount, "% 9d", collectedCoins);
	//wchar_t wchar_tCount;
	int digits = 0;
    while (collectedCoins != 0)
    {
        collectedCoins = collectedCoins / 10;
        digits++;
    }
	OSReport("digits: %d\n\n\n", digits);
	OSReport("textCount: %s\n\n\n", textCount);
	coinCount->SetString(textCount, 0, 9);
}

int dStatsMenu_c::onExecute() {
	state.execute();

	layout.execAnimations();
	layout.update();

	return true;
}

int dStatsMenu_c::onDraw() {
	if (visible) {
		layout.scheduleForDrawing();
	}
	
	return true;
}

int dStatsMenu_c::onDelete() {
	instance = 0;
	OSReport("onDeleteeeeeeeeeeeeeeeeeeeee\n");
	if (StageC4::instance)
		StageC4::instance->_1D = 0; // disable no-pause

	return layout.free();
}



// Hidden
void dStatsMenu_c::beginState_Hidden() { }
void dStatsMenu_c::executeState_Hidden() { }
void dStatsMenu_c::endState_Hidden() { }

void dStatsMenu_c::beginState_WaitForWipe() { }
void dStatsMenu_c::executeState_WaitForWipe() { 
	if (count <= 0) state.setState(&StateID_ShowWait);
	else count--;
}
void dStatsMenu_c::endState_WaitForWipe() { }

void dStatsMenu_c::activate(int pScene, int pSceneParams, int pPowerupStoreType, int pWipe) {
	this->scene = pScene;
	this->sceneParams = pSceneParams;
	this->powerupStoreType = pPowerupStoreType;
	this->wipe = pWipe;
	

	if(powerupStoreType == LOSE_LEVEL) {
		//Buttons[2]->SetVisible(false);
		switchButtons(selected++, selected);
		Buttons[0]->trans = (Vec){100.0f, -100.0f, 0.0f};
		Buttons[1]->trans = (Vec){-100.0f, -100.0f, 0.0f};
		Buttons[2]->SetVisible(false);
	}
	
	state.setState(&StateID_WaitForWipe);
	this->wasActiveAlready = true;
}

// ShowWait
void dStatsMenu_c::beginState_ShowWait() {
	nw4r::snd::SoundHandle handle;
	PlaySoundWithFunctionB4(SoundRelatedClass, &handle, SE_SYS_KO_DIALOGUE_IN, 1);

	layout.disableAllAnimations();
	layout.enableNonLoopAnim(ANIM_WINDOW_IN);
	visible = true;
	scaleEase = 0.0;
	StageC4::instance->_1D = 1; // enable no-pause
}
void dStatsMenu_c::executeState_ShowWait() {
	if (!layout.isAnimOn(ANIM_WINDOW_IN)) {
		if(powerupStoreType == BEAT_LEVEL) selected = 0;
		else                               selected = 1;
		layout.enableNonLoopAnim(ANIM_BUTTON_ON);
		layout.enableNonLoopAnim(ANIM_BUTTON_ON+1);
		layout.enableNonLoopAnim(ANIM_BUTTON_ON+2);
		state.setState(&StateID_CountdownWait);
	}
}
void dStatsMenu_c::endState_ShowWait() {
	nw4r::snd::SoundHandle handle;
	// PlaySoundWithFunctionB4(SoundRelatedClass, &handle, SE_OBJ_CLOUD_BLOCK_TO_JUGEM, 1);
	
	//SaveBlock *save = GetSaveFile()->GetBlock(-1);
	//structWithWorldData = (int*)&(save->completions[0][0]);
	//OSReport("structWithWorldData: %x, %p", structWithWorldData, structWithWorldData);
	//OSReport("Star Coins: %d, %d, %d\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n", CheckIfWeHaveASpecificStarCoin(structWithWorldData, CurrentWorld, CurrentLevel, COND_COIN1), CheckIfWeHaveASpecificStarCoin(structWithWorldData, CurrentWorld, CurrentLevel, COND_COIN2), CheckIfWeHaveASpecificStarCoin(structWithWorldData, CurrentWorld, CurrentLevel, COND_COIN3));
	//u32 conds = save->GetLevelCondition(CurrentWorld, CurrentLevel);

	setStats();
	
	timer = 1;
}

void dStatsMenu_c::beginState_CountdownWait() { }
void dStatsMenu_c::executeState_CountdownWait() { 
	wchar_t textCount[1];
	int nowPressed = Remocon_GetPressed(GetActiveRemocon());
	if(nowPressed == WPAD_TWO) {
		if(powerupStoreType == BEAT_LEVEL) goMap();
		else                               goAgain();
	} else if(nowPressed == WPAD_RIGHT) {
		//set new selected Button and go to Wait State
		textCount[0] = ' ';
		Countdown->SetString(textCount, 0, 3);
		
		if(powerupStoreType == BEAT_LEVEL) switchButtons(selected++, selected);
		else                               switchButtons(selected--, selected);
		
		state.setState(&StateID_Wait);
	} else if(nowPressed != 0) {
		textCount[0] = ' ';
		Countdown->SetString(textCount, 0, 3);
		state.setState(&StateID_Wait);
	}

	if (autoselectCountdown % 60 == 0) {
		textCount[0] = '0'+autoselectCountdown/60;
		Countdown->SetString(textCount, 0, 3);
		if (autoselectCountdown / 60 == 0) 
			if(powerupStoreType == BEAT_LEVEL) goMap();
			else                               goAgain();
	}
	autoselectCountdown--;
}
void dStatsMenu_c::endState_CountdownWait() { }


void dStatsMenu_c::goMap() { 
	ExitStage(WORLD_MAP, 0, BEAT_LEVEL, MARIO_WIPE);
}

void dStatsMenu_c::goAgain() { 
	RESTART_CRSIN_LevelStartStruct.screenType = ST_NORMAL;
	RESTART_CRSIN_LevelStartStruct.world1 = CurrentWorld;
	RESTART_CRSIN_LevelStartStruct.world2 = CurrentWorld;
	RESTART_CRSIN_LevelStartStruct.level1 = CurrentLevel;
	RESTART_CRSIN_LevelStartStruct.level2 = CurrentLevel;
	RESTART_CRSIN_LevelStartStruct.area = 0;
	RESTART_CRSIN_LevelStartStruct.entrance = 0xFF;
	RESTART_CRSIN_LevelStartStruct.isReplay = false;
	//DoSceneChange(RESTART_CRSIN, 0, 0);
	DontShowPreGame = true;
	ExitStage(RESTART_CRSIN, 0, powerupStoreType, MARIO_WIPE);	
}

void dStatsMenu_c::goReplay() { 
	RESTART_CRSIN_LevelStartStruct.screenType = ST_HINT_MOVIE;		//Otekara folder
	RESTART_CRSIN_LevelStartStruct.replayType = RT_SUPER_SKILLS;
	RESTART_CRSIN_LevelStartStruct.world1 = CurrentWorld;
	RESTART_CRSIN_LevelStartStruct.world2 = CurrentWorld;
	RESTART_CRSIN_LevelStartStruct.level1 = CurrentLevel;
	RESTART_CRSIN_LevelStartStruct.level2 = CurrentLevel;
	RESTART_CRSIN_LevelStartStruct.area = 0;
	RESTART_CRSIN_LevelStartStruct.entrance = 0xFF;
	RESTART_CRSIN_LevelStartStruct.isReplay = true;
	//DoSceneChange(RESTART_CRSIN, 0, 0);
	DontShowPreGame = true;
	ExitStage(RESTART_CRSIN, 0, powerupStoreType, MARIO_WIPE);
}

// Wait
void dStatsMenu_c::beginState_Wait() {
}
void dStatsMenu_c::executeState_Wait() {
	/*if (timer < 90) {
		scaleEase = -((cos(timer * 3.14 /20)-0.9)/timer*10)+1;
		timer++;
		return;
	}*/

	int nowPressed = Remocon_GetPressed(GetActiveRemocon());
	
	if (nowPressed & WPAD_RIGHT) {
		if(powerupStoreType == BEAT_LEVEL) {
			if(selected < 2) switchButtons(selected++, selected); 
		} else {
			if(selected > 0) switchButtons(selected--, selected); 
		}
	} else if (nowPressed & WPAD_LEFT) {
		if(powerupStoreType == BEAT_LEVEL) {
			if(selected > 0) switchButtons(selected--, selected);
		} else {
			if(selected < 1) switchButtons(selected++, selected);	
		}
	} else if (nowPressed & WPAD_TWO) {
		switch(selected) {
			case 0:
				goMap();
				break;
			case 1:
				goAgain();
				break;
			case 2:
				goReplay();
				break;
		}
		
	}

}
void dStatsMenu_c::endState_Wait() { }


#undef ANIM_WINDOW_IN
#undef ANIM_WINDOW_LOOP
#undef ANIM_WINDOW_OUT
#undef ANIM_BUTTON_HIT
#undef ANIM_BUTTON_IDLE
#undef ANIM_BUTTON_OFF
#undef ANIM_BUTTON_ON