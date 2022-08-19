#ifndef __TALKBALLOON_H
#define __TALKBALLOON_H

#include <common.h>
#include <game.h>

class dTalkBalloon_c : public dStageActor_c {
public:
	dTalkBalloon_c() : acState(this, &StateID_Init) { }

	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	m2d::EmbedLayout_c layout;
	dStateWrapper_c<dTalkBalloon_c> acState;

	bool layoutLoaded, isVisible;

	u32 timer;
	u32 textAnim;
	bool isShown;

	nw4r::lyt::TextBox *T_text[2];
	nw4r::ut::TextWriter tbTextWriter;
	Vec2 syncPos;

	enum Animation {
		APPEAR = 0,
		WAIT = 1,
		END = 2,
		BEAK = 3,
		ONE_LINE = 4,
		TWO_LINE = 5,
		TEST00 = 6,
	};

	void showTB(int textID, Vec3 pos);
	void hideTB();
	void syncronisePos();

	static dActor_c *build();
	static dTalkBalloon_c *instance;

	USING_STATES(dTalkBalloon_c);
	DECLARE_STATE(Init);
	DECLARE_STATE(Appear);
	DECLARE_STATE(Wait);
	DECLARE_STATE(End);
};


#endif