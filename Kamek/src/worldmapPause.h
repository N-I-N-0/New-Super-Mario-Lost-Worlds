#ifndef __WORLDMAPPAUSE_H
#define __WORLDMAPPAUSE_H

#include <game.h>
#include <common.h>

class dSettings_c : public dBase_c {
public:
	dSettings_c();
	// ~dSettings_c();

	m2d::EmbedLayout_c layout; // 0x70
	dStateWrapper_c<dSettings_c> acState; // 0x208

	nw4r::lyt::Pane *rootPane; // 0x244

	u8 layoutLoaded; // 0x248
	u8 isCurrentlyActive; // 0x249

	u8 selectedRow;
	u8 selectedLine;

	u32 timer;
	u32 isOnInfo;

	nw4r::lyt::Picture *enabledButtons[3];
	nw4r::lyt::Picture *disabledButtons[3];
	nw4r::lyt::Picture *infoButtons[3];
	nw4r::lyt::Picture *centerButton;

	nw4r::lyt::TextBox *buttonTextBoxes[3];
	nw4r::lyt::TextBox *buttonTextBoxesS[3];
	nw4r::lyt::TextBox *infoTextBoxes[3];
	nw4r::lyt::TextBox *infoTextBoxesS[3];

	nw4r::lyt::Pane *rightPanes[6];


	static dActor_c *build();

	int onCreate();
	int onDelete();
	int onExecute();
	int onDraw();

	int beforeExecute();

	USING_STATES(dSettings_c);
	DECLARE_STATE(InitWait);
	DECLARE_STATE(Appear);
	DECLARE_STATE(ButtonChange);
	DECLARE_STATE(WaitForInput);
	DECLARE_STATE(ButtonHit);
	DECLARE_STATE(ShowInfo);
	DECLARE_STATE(Close);

};


#endif