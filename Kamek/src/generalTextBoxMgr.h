#ifndef __GENERALTEXTBOXMGR_H
#define __GENERALTEXTBOXMGR_H

#include <common.h>
#include <game.h>
#include <g3dhax.h>
#include <sfx.h>

class dGeneralTBMgr_c : public dStageActor_c {
public:
	dGeneralTBMgr_c() : acState(this, &StateID_Init) { }

	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	m2d::EmbedLayout_c layout;
	dStateWrapper_c<dGeneralTBMgr_c> acState;

	bool layoutLoaded, isVisible;

	u32 timer;
	u32 textAnim;

	nw4r::lyt::TextBox *T_text[2];

	nw4r::lyt::Picture* bubbleLeft;
	nw4r::lyt::Picture* bubbleMiddle;
	nw4r::lyt::Picture* bubbleRight;
	nw4r::lyt::Picture* bubbleName;
	nw4r::lyt::TextBox* textName;
	dTexMapColouriser_c colorLeft;
	dTexMapColouriser_c colorMiddle;
	dTexMapColouriser_c colorRight;
	dTexMapColouriser_c colorName;

	enum Animation {
		SHOW_ALL = 0,
		TEXT_NEXT1 = 1,
		TEXT_NEXT2 = 2,
		TEXT_BACK1 = 3,
		TEXT_BACK2 = 4,
		HIDE_ALL = 5,
		BUTTON_APPEAR = 6,
		BUTTON_IDLE = 7,
		BUTTON_DISAPPEAR = 8,
	};

	static dActor_c *build();
	static dGeneralTBMgr_c *instance;

	USING_STATES(dGeneralTBMgr_c);
	DECLARE_STATE(Init);
	DECLARE_STATE(WaitForButton);
	DECLARE_STATE(WaitForInput);
	DECLARE_STATE(WaitForText1);
	DECLARE_STATE(WaitForText2);
	DECLARE_STATE(Show);
	DECLARE_STATE(End);
};


#endif