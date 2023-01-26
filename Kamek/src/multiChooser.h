#ifndef MULTICHOOSER_H
#define MULTICHOOSER_H 
#include <common.h>
#include <game.h>

class daMultiChooser_c : public dActor_c {
	public:
		daMultiChooser_c() : acState(this, &StateID_ShowWindow) { }

		int onCreate();
		int onDelete();
		int onExecute();
		int onDraw();

		m2d::EmbedLayout_c layout;
		dDvdLoader_c msgDataLoader;

		bool layoutLoaded;
		bool visible;
		bool hasPressed;

		int playerIDs[4];
		int playerPadder[4];
		int maxOffs;
		int currentSelection;

		nw4r::lyt::Pane *leftPlayerPanes[4];
		nw4r::lyt::Pane *rightPlayerPanes[4];

		int timer;
		bool isRightArrowHidden;
		bool isLeftArrowHidden;

		GameMgr* gameMgr;
		StartLevelInfo lvl;

		bool doesKonamiCode;

		dStateWrapper_c<daMultiChooser_c> acState;

		USING_STATES(daMultiChooser_c);
		DECLARE_STATE(ShowWindow);
		DECLARE_STATE(ShowButton);
		DECLARE_STATE(Wait);
		DECLARE_STATE(HideWindow);

		static daMultiChooser_c *instance;
		static dActor_c *build();

		void setRightPlayerPane(int id);
		void setLeftPlayerPane(int id);
};
#endif /* MULTICHOOSER_H */
