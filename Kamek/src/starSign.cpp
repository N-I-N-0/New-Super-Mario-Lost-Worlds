#include <common.h>
#include <game.h>
#include <profile.h>
#include "courseSelectManager.h"
#include "starSign.h"

CREATE_STATE(dStarSign_c, StartWait);
CREATE_STATE(dStarSign_c, StartAnime);
CREATE_STATE(dStarSign_c, WaitForExit);
CREATE_STATE(dStarSign_c, ExitAnime);

dStarSign_c::dStarSign_c() : layout(), acState(this, &StateID_StartWait) {
	this->layoutLoaded = 0;
}

// dStarSign_c::~dStarSign_c() {
// 	delete &layout;
// 	delete &acState;
// 	FreeFromGameHeap1(this);
// }

dActor_c *dStarSign_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dStarSign_c));
	return (dActor_c*)new(buffer) dStarSign_c;
}

const char* EmptyStarSignFileList[] = {NULL};
Profile StarSignProfile(&dStarSign_c::build, ProfileId::WM_STARSIGN, NULL, ProfileId::WM_STARSIGN, ProfileId::WM_STARSIGN, "WM_STARSIGN", EmptyStarSignFileList);


int dStarSign_c::onCreate() {
	if(!layoutLoaded) {
		if (!layout.loadArc("starSign.arc", false))
			return false;

		static const char *brlanNames[2] = {
			"starSign_inWindow.brlan",
			"starSign_outWindow.brlan",
		};

		static const char *groupNames[2] = {
			"A00_window",
			"A00_window",
		};

		static const int groupIDs[2] = {
			0,
			1,
		};

		layout.build("starSign.brlyt");
		layout.loadAnimations(brlanNames, 2);
		layout.loadGroups(groupNames, groupIDs, 2);

		rootPane = layout.getRootPane();

		WriteBMGToTextBox(layout.findTextBoxByName("T_message_00"), GetBMG(), 0x3EC, 0, 0);

		layout.resetAnim(0);
		
		// layout.update();
		// layout.execAnimations();

		// layout.disableAllAnimations();

		rootPane->SetVisible(false);
		// rootPane->flag &= 0xFE;
		rootPane->scale.x = 0.7f;
		rootPane->scale.y = 0.7f;
		rootPane->trans.y += 110.0f;

		// layout.drawOrder = 140;

		layoutLoaded = true;
		isCurrentlyActive = false;
		exitAnime = false;
	}

	return true;
}

int dStarSign_c::onDelete() {
	return layout.free();
}

int dStarSign_c::beforeExecute() {
	if(dBase_c::beforeExecute()) {
		return (CheckIfWeCantDoStuff() == 0);
	}
	return false;
}

int dStarSign_c::onExecute() {
	if(this->layoutLoaded && this->isCurrentlyActive) {
		acState.execute();
		layout.execAnimations();
		layout.update();
	}

	return true;
}

int dStarSign_c::onDraw() {
	if(this->layoutLoaded && this->isCurrentlyActive) {
		layout.scheduleForDrawing();
	}

	return true;
}

// StartWait state

void dStarSign_c::beginState_StartWait() {
	this->exitAnime = false;
	this->isEnding = false;
}

void dStarSign_c::executeState_StartWait() {
	if(this->startAnime)
		acState.setState(&StateID_StartAnime);
}

void dStarSign_c::endState_StartWait() {
	this->startAnime = false;
}

// StartAnime state

void dStarSign_c::beginState_StartAnime() {
	layout.enableNonLoopAnim(0);
	rootPane->SetVisible(true);
	this->isEnding = true;
	MapSoundPlayer(SoundRelatedClass, 0x7E, 1);
}

void dStarSign_c::executeState_StartAnime() {
	if(!layout.isAnimOn(0)) 
		acState.setState(&StateID_WaitForExit);
}

void dStarSign_c::endState_StartAnime() {
	this->isEnding = false;
}

// WaitForExit state

void dStarSign_c::beginState_WaitForExit() {

}

void dStarSign_c::executeState_WaitForExit() {
	if(this->exitAnime)
		acState.setState(&StateID_ExitAnime);
}

void dStarSign_c::endState_WaitForExit() {
	this->exitAnime = false;
}

// ExitAnime state

void dStarSign_c::beginState_ExitAnime() {
	layout.enableNonLoopAnim(1);
	this->isEnding = true;
}

void dStarSign_c::executeState_ExitAnime() {
	if(!layout.isAnimOn(1)) 
		acState.setState(&StateID_StartWait);
}

void dStarSign_c::endState_ExitAnime() {
	this->isEnding = false;
	this->isCurrentlyActive = false;
	rootPane->SetVisible(false);
}


// CSMgr stuff

CREATE_STATE(dCourseSelectManager_c, LetterOpenAnim);
CREATE_STATE(dCourseSelectManager_c, LetterDisp);
CREATE_STATE(dCourseSelectManager_c, LetterExitAnim);

void dCourseSelectManager_c::beginState_LetterOpenAnim() {
	this->doesLetterOpenAnime = false;
	this->signPtr->isCurrentlyActive = true;
	this->signPtr->startAnime = true;
}

void dCourseSelectManager_c::executeState_LetterOpenAnim() {
	OSReport("CSMgr: %s\n", state.getCurrentState()->getName());
	if(!this->signPtr->isEnding)
		state.setState(&StateID_LetterDisp);
}

void dCourseSelectManager_c::endState_LetterOpenAnim() { }

void dCourseSelectManager_c::beginState_LetterDisp() { }

void dCourseSelectManager_c::executeState_LetterDisp() {
	OSReport("CSMgr: %s\n", state.getCurrentState()->getName());
	if(this->startedSomeMsgThing) 
		state.setState(&StateID_LetterExitAnim);
}

void dCourseSelectManager_c::endState_LetterDisp() { }

void dCourseSelectManager_c::beginState_LetterExitAnim() {
	this->startedSomeMsgThing = false;
	this->signPtr->exitAnime = true;
}

void dCourseSelectManager_c::executeState_LetterExitAnim() {
	OSReport("CSMgr: %s\n", state.getCurrentState()->getName());
	if(this->signPtr->isEnding)
		state.setState(&StateID_KeyWait);
}

void dCourseSelectManager_c::endState_LetterExitAnim() {
	this->endedSomeMsgThing = false;
}


// Misc

extern "C" u8 CurrentWorldNumForWorldMap;
u32 CheckForWorld() {
	switch(CurrentWorldNumForWorldMap) {
		case 8:
			return 1;
		case 7:
			return 2;
		default:
			return 0;
	}
}

#include "levelinfo.h"
u32 getCollectedPowerStars() {
	u32 collectedStars = 0;

	SaveBlock *save = GetSaveFile()->GetBlock(-1);
	for(int sect = 0; sect < dLevelInfo_c::s_info.sectionCount(); sect++) {
		dLevelInfo_c::section_s *thisSect = dLevelInfo_c::s_info.getSectionByIndex(sect);

		for(int lev = 0; lev < thisSect->levelCount; lev++) {
			dLevelInfo_c::entry_s *level = &thisSect->levels[lev];

			if((level->flags & 2) && (save->GetLevelCondition(level->worldSlot, level->levelSlot) & COND_NORMAL)) {
				collectedStars++;
			}
		}
	}

	return collectedStars;
}