#include "talkBalloon.h"

CREATE_STATE(dTalkBalloon_c, Init);
CREATE_STATE(dTalkBalloon_c, Appear);
CREATE_STATE(dTalkBalloon_c, Wait);
CREATE_STATE(dTalkBalloon_c, End);

dTalkBalloon_c *dTalkBalloon_c::instance = 0;

dActor_c *dTalkBalloon_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dTalkBalloon_c));
	dTalkBalloon_c *c = new(buffer) dTalkBalloon_c;

	instance = c;
	return c;
}


const char* talkBallonFileList[] = { NULL };
const SpriteData talkBalloonData = {ProfileId::TALKBALLOON, 0, 0, 0, 0, 0x1000, 0x1000, 0, 0, 0, 0, 2};
Profile talkBallonProfile(&dTalkBalloon_c::build, SpriteId::TALKBALLOON, &talkBalloonData, ProfileId::TALKBALLOON, ProfileId::TALKBALLOON, "TALKBALLOON", talkBallonFileList);



extern "C" void CalcLayoutPos(Vec2* pos); // Thanks to CLF78 for that one!

int dTalkBalloon_c::onCreate() {
	if(!layoutLoaded) {
		if (!layout.loadArc("talkBalloon.arc", false))
			return false;

		static const char *brlanNames[7] = {
			"talkBalloon_appear.brlan",
			"talkBalloon_wait.brlan",
			"talkBalloon_end.brlan",
			"talkBalloon_beak.brlan",
			"talkBalloon_oneLine.brlan",
			"talkBalloon_twoLine.brlan",
			"talkBalloon_Test00.brlan",
		};

		static const char *groupNames[7] = {
			"A00_balloon",
			"A00_balloon",
			"A00_balloon",
			"B00_beak",
			"C00_win",
			"C00_win",
			"A00_balloon",
		};

		static const int groupIDs[7] = {
			0,
			1,
			2,
			3,
			4,
			5,
			6,
		};

		layout.build("talkBalloon.brlyt");

		layout.loadAnimations(brlanNames, 7);
		layout.loadGroups(groupNames, groupIDs, 7);
		layout.disableAllAnimations();

		layout.resetAnim(END, true);

		layoutLoaded = true;

		T_text[0] = layout.findTextBoxByName("TxtText");
		T_text[1] = layout.findTextBoxByName("ShaText");

		tbTextWriter.font = T_text[0]->font;
		tbTextWriter.SetFontSize(T_text[0]->fontSizeX, T_text[0]->fontSizeY);
		tbTextWriter.lineSpace = T_text[0]->lineSpace;
		tbTextWriter.charSpace = T_text[0]->charSpace;
		if (T_text[0]->tagProc != 0)
			tbTextWriter.tagProcessor = T_text[0]->tagProc;

		layout.layout.rootPane->trans.y = 1000.0f;

		// OSReport("crated: %p\n", T_text[0]->tagProc);
	}

	// isVisible = true;
	return true;
}


int dTalkBalloon_c::onDelete() {
	return layout.free();
}

int dTalkBalloon_c::onDraw() {
	if (isVisible) {
		layout.scheduleForDrawing();
	}

	return true;
}

int dTalkBalloon_c::onExecute() {
	acState.execute();

	layout.update();

	this->syncronisePos();

	return true;
}

void dTalkBalloon_c::syncronisePos() {
	Vec2 lytPos = (Vec2){syncPos.x, syncPos.y + 32.0f};
	CalcLayoutPos(&lytPos);
	layout.layout.rootPane->trans.x = lytPos.x;
	layout.layout.rootPane->trans.y = lytPos.y;
}

int wcslen_nsmbw(const wchar_t* txt) {
	int currLen = 0;
	int currChar = 0;
	while(txt[currChar] != 0) {
		if(txt[currChar] == 0xB) {
			currChar += 2;
		}

		currChar++;
		currLen++;
	}
	return currLen;
}

int getLineCount(const wchar_t* txt) {
	int lineCount = 0;

	for(int i = 0; i < wcslen_nsmbw(txt); i++) 
		if(txt[i] == L'\n') lineCount++;

	return lineCount;
}

void dTalkBalloon_c::showTB(int textID, Vec3 pos) {
	if(!isShown) {
		isShown = true;

		syncPos = (Vec2){pos.x, pos.y};
		this->syncronisePos();

		WriteBMGToTextBox(T_text[0], GetBMG(), 0x12D,textID, 0);
		WriteBMGToTextBox(T_text[1], GetBMG(), 0x12D, textID, 0);
		T_text[0]->DrawSelf(layout.drawInfo);

		bool twoLines = getLineCount(GetBMGMessage(0x12D, textID));

		// float width = tbTextWriter.CalcStringWidth(T_text[0]->stringBuf, wcslen_nsmbw(T_text[0]->stringBuf));
		float width = T_text[0]->textLen;
		layout.resetAnim(ONE_LINE);
		layout.resetAnim(TWO_LINE);
		layout.enableNonLoopAnim(ONE_LINE+twoLines);
		// OSReport("len: %f %d\n", width, wcslen_nsmbw(T_text[0]->stringBuf));
		OSReport("len2: %f %p\n", T_text[0]->textLen, &T_text[0]->textLen);
		if(width > 510.0f) width = 510.0f;
		for(int i = 0; i < (int)width; i++) {
			layout.execAnimations();
		}

		layout.animsEnabled[ONE_LINE+twoLines] = false;

		acState.setState(&StateID_Appear);
	}
}

void dTalkBalloon_c::hideTB() {
	if(isShown) {
		isShown = false;

		acState.setState(&StateID_End);
	}
}

// Init State

void dTalkBalloon_c::beginState_Init() {
	isVisible = true;

	layout.enableNonLoopAnim(BEAK);

	for(int i = 0; i < 43; i++) {
		layout.execAnimations();
	}

	layout.animsEnabled[BEAK] = false;
}

void dTalkBalloon_c::executeState_Init() {
}

void dTalkBalloon_c::endState_Init() {
	
}

// Appear State

void dTalkBalloon_c::beginState_Appear() {
	layout.enableNonLoopAnim(APPEAR);
}

void dTalkBalloon_c::executeState_Appear() {
	if(!layout.isAnimOn(APPEAR)) {
		acState.setState(&StateID_Wait);
	}
	else {
		layout.execAnimations();
	}
}

void dTalkBalloon_c::endState_Appear() {
	
}

// Wait State

void dTalkBalloon_c::beginState_Wait() {

}

void dTalkBalloon_c::executeState_Wait() {

}

void dTalkBalloon_c::endState_Wait() {
	
}

// End State

void dTalkBalloon_c::beginState_End() {
	layout.enableNonLoopAnim(END);
}

void dTalkBalloon_c::executeState_End() {
	if(!layout.isAnimOn(END)) {
		acState.setState(&StateID_Wait);
	}
	else {
		layout.execAnimations();
	}
}

void dTalkBalloon_c::endState_End() {
	
}