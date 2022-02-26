#ifndef __CREATOR_INFO_H
#define __CREATOR_INFO_H

#include <game.h>
#include <common.h>
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

		m2d::EmbedLayout_c layout;
		bool layoutLoaded;

		bool visible;
		
		bool showLevelCredits;
		
		FileHandle fileHandles[3];

		u32 currentStateID;

		u32 currentPage;

		int timer;

		nw4r::lyt::Picture *Sample;
		nw4r::lyt::TextBox *LevelName;
		nw4r::lyt::TextBox *AuthorNames;
		nw4r::lyt::TextBox *AuthorQuote;

		enum Animaiton {
			SHOW_ALL = 0, //BonusCourseSelect_inWindow
			HIDE_ALL = 1, //BonusCourseSelect_outWindow
		};

		void show();

		void loadInfo();

		dStateWrapper_c<dCreatorInfo_c> state;

		USING_STATES(dCreatorInfo_c);
		DECLARE_STATE(Hidden);
		DECLARE_STATE(ShowWait);
		DECLARE_STATE(Wait);
		DECLARE_STATE(HideWait);
};

#endif