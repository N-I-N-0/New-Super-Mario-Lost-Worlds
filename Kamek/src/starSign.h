#ifndef __STARSIGN_H
#define __STARSIGN_H

class dStarSign_c : public dBase_c {
public:
	dStarSign_c();
	// ~dStarSign_c();

	m2d::EmbedLayout_c layout; // 0x70
	dStateWrapper_c<dStarSign_c> acState; // 0x208

	nw4r::lyt::Pane *rootPane; // 0x244

	u8 layoutLoaded; // 0x248
	u8 isCurrentlyActive;
	u8 startAnime;
	u8 exitAnime;
	u8 isEnding;

	u32 timer;

	// nw4r::lyt::TextBox *T_message_00;


	static dActor_c *build();

	int onCreate();
	int onDelete();
	int onExecute();
	int onDraw();

	int beforeExecute();

	USING_STATES(dStarSign_c);
	DECLARE_STATE(StartWait);
	DECLARE_STATE(StartAnime);
	DECLARE_STATE(WaitForExit);
	DECLARE_STATE(ExitAnime);

};

#endif