#include <common.h>
#include <game.h>
#include <profile.h>


class cutsceneMgr : public dStageActor_c {
public:
	nw4r::lyt::Picture *_1;
	nw4r::lyt::Picture *_2;
	nw4r::lyt::Picture *_3;
	nw4r::lyt::Picture *_4;
	nw4r::lyt::Picture *_5;
	nw4r::lyt::Picture *_6;
	nw4r::lyt::Picture *_7;
	nw4r::lyt::Picture *_31;
	nw4r::lyt::Picture *_8;
	nw4r::lyt::Picture *_9;
	nw4r::lyt::Picture *_10;
	nw4r::lyt::Picture *_11;
	nw4r::lyt::Picture *_12;
	nw4r::lyt::Picture *_13;
	nw4r::lyt::Picture *_14;
	nw4r::lyt::Picture *_15;
	nw4r::lyt::Picture *_16;
	nw4r::lyt::TextBox *_18;
	nw4r::lyt::TextBox *_19;
	nw4r::lyt::TextBox *_20;
	nw4r::lyt::TextBox *_21;
	nw4r::lyt::TextBox *_22;
	nw4r::lyt::TextBox *_23;
	nw4r::lyt::TextBox *_24;
	nw4r::lyt::TextBox *_25;
	nw4r::lyt::TextBox *_26;
	nw4r::lyt::TextBox *_27;
	nw4r::lyt::TextBox *_28;
	nw4r::lyt::TextBox *_29;
	nw4r::lyt::TextBox *_30;

	nw4r::lyt::Pane *_32;

	bool ran60times;
	bool visibility;																					//store the visibility bool
	bool isLuigiActive;
	bool isKinoBActive;
	bool isKinoYActive;

	void switchGameDisplayVisibility();

	int onCreate();
	int onExecute();

	static dActor_c *build();
};

const char *CutsceneMgrFileList[] = {0};
const SpriteData cutsceneMgrSpriteData = { ProfileId::CutsceneMgr, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile cutsceneMgrProfile(&cutsceneMgr::build, SpriteId::CutsceneMgr, &cutsceneMgrSpriteData, ProfileId::CutsceneMgr, ProfileId::CutsceneMgr, "CutsceneMgr", CutsceneMgrFileList);


dActor_c  *cutsceneMgr::build() {
	void *buffer = AllocFromGameHeap1(sizeof(cutsceneMgr));
	cutsceneMgr *c = new(buffer) cutsceneMgr;

	return c;
}


int cutsceneMgr::onCreate() {
	return true;
}


int cutsceneMgr::onExecute() {
	if(ran60times < 60) {
		ran60times++;
	} else {
		if(ran60times == 60) {
			ran60times = 61;
			OSReport("========================================\n");
			this->_1 = dGameDisplay_c::instance->layout.findPictureByName("P_baseS_01");						//getting the panes, too lazy to name them all
			OSReport("1\n");
			this->_2 = dGameDisplay_c::instance->layout.findPictureByName("P_base_01");
			OSReport("2\n");
			this->_3 = dGameDisplay_c::instance->layout.findPictureByName("P_otasukeIcon_00");
			OSReport("3\n");
			this->_4 = dGameDisplay_c::instance->layout.findPictureByName("P_marioIcon_00");
			OSReport("4\n");
			this->_5 = dGameDisplay_c::instance->layout.findPictureByName("P_luijiIcon_00");
			OSReport("5\n");
			this->_6 = dGameDisplay_c::instance->layout.findPictureByName("P_kinoB_00");
			OSReport("6\n");
			this->_7 = dGameDisplay_c::instance->layout.findPictureByName("P_kinoY_00");
			OSReport("7\n");
			this->_31 = dGameDisplay_c::instance->layout.findPictureByName("P_baseS_00");
			OSReport("8\n");
			this->_8 = dGameDisplay_c::instance->layout.findPictureByName("P_base_00");
			OSReport("9\n");
			this->_9 = dGameDisplay_c::instance->layout.findPictureByName("P_coin_00");
			OSReport("10\n");
			this->_10 = dGameDisplay_c::instance->layout.findPictureByName("P_collectOff_00");
			OSReport("11\n");
			this->_11 = dGameDisplay_c::instance->layout.findPictureByName("P_collectOff_01");
			OSReport("12\n");
			this->_12 = dGameDisplay_c::instance->layout.findPictureByName("P_collectOff_02");
			OSReport("13\n");
			this->_13 = dGameDisplay_c::instance->layout.findPictureByName("P_collection_00");
			OSReport("14\n");
			this->_14 = dGameDisplay_c::instance->layout.findPictureByName("P_collection_01");
			OSReport("15\n");
			this->_15 = dGameDisplay_c::instance->layout.findPictureByName("P_collection_02");
			OSReport("16\n");
			this->_16 = dGameDisplay_c::instance->layout.findPictureByName("P_timer_00");
			OSReport("17\n");
			this->_18 = dGameDisplay_c::instance->layout.findTextBoxByName("T_otaChuS_00");
			OSReport("18\n");
			this->_19 = dGameDisplay_c::instance->layout.findTextBoxByName("T_otaChu_00");
			OSReport("19\n");
			this->_20 = dGameDisplay_c::instance->layout.findTextBoxByName("T_x_01");
			OSReport("20\n");
			this->_21 = dGameDisplay_c::instance->layout.findTextBoxByName("T_x_02");
			OSReport("21\n");
			this->_22 = dGameDisplay_c::instance->layout.findTextBoxByName("T_x_03");
			OSReport("22\n");
			this->_23 = dGameDisplay_c::instance->layout.findTextBoxByName("T_x_04");
			OSReport("23\n");
			this->_24 = dGameDisplay_c::instance->layout.findTextBoxByName("T_left_00");
			OSReport("24\n");
			this->_25 = dGameDisplay_c::instance->layout.findTextBoxByName("T_left_01");
			OSReport("25\n");
			this->_26 = dGameDisplay_c::instance->layout.findTextBoxByName("T_left_02");
			OSReport("26\n");
			this->_27 = dGameDisplay_c::instance->layout.findTextBoxByName("T_left_03");
			OSReport("27\n");
			this->_28 = dGameDisplay_c::instance->layout.findTextBoxByName("T_coin_00");
			OSReport("28\n");
			this->_29 = dGameDisplay_c::instance->layout.findTextBoxByName("T_score_00");
			OSReport("29\n");
			this->_30 = dGameDisplay_c::instance->layout.findTextBoxByName("T_time_00");
			OSReport("30\n");
			this->_32 = dGameDisplay_c::instance->layout.findPaneByName("N_collection_00");
			OSReport("31\n");

			if(_5->flag == 1) {																					//if luigi's pane is visible, luigi is active
				isLuigiActive = true;
			}
			if(_6->flag == 1) {																					//if kinob's pane is visible, kinob is active
				isKinoBActive = true;
			}
			if(_7->flag == 1) {																					//if kinoy's pane is visible, kinoy is active
				isKinoYActive = true;
			}
			
			this->visibility = true;

		} else {
			OSReport("trigger");
			if((dFlagMgr_c::instance->flags & spriteFlagMask) != 0)
				if(visibility)
					switchGameDisplayVisibility();
			else
				if(!visibility)
					switchGameDisplayVisibility();
		}
	}
	return true;
}


void cutsceneMgr::switchGameDisplayVisibility() {
	visibility != visibility;
	_1->SetVisible(visibility);																			//set the panes visibilities
	_2->SetVisible(visibility);
	_3->SetVisible(visibility);
	_4->SetVisible(visibility);
	if(isLuigiActive) {																					//if luigi is active, affect his panes
		_5->SetVisible(visibility);
		_21->SetVisible(visibility);
		_25->SetVisible(visibility);
	}
	
	if(isKinoBActive) {																					//if kinob is active, affect his panes
		_6->SetVisible(visibility);
		_22->SetVisible(visibility);
		_26->SetVisible(visibility);
	}
	
	if(isKinoYActive) {																					//if kinoy is active, affect his panes
		_7->SetVisible(visibility);
		_23->SetVisible(visibility);
		_27->SetVisible(visibility);
	}
	_8->SetVisible(visibility);
	_9->SetVisible(visibility);
	_10->SetVisible(visibility);
	_11->SetVisible(visibility);
	_12->SetVisible(visibility);
	_13->SetVisible(visibility);
	_14->SetVisible(visibility);
	_15->SetVisible(visibility);
	_16->SetVisible(visibility);
	_18->SetVisible(visibility);
	_19->SetVisible(visibility);
	_20->SetVisible(visibility);
	_24->SetVisible(visibility);
	_28->SetVisible(visibility);
	_29->SetVisible(visibility);
	_30->SetVisible(visibility);
	_31->SetVisible(visibility);
	_32->SetVisible(visibility);
}