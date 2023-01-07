#ifndef __CSM_H
#define __CSM_H

#include <game.h>
#include <common.h>

#include "worldmapShop.h"
#include "creatorInfo.h"

class dStarSign_c;
class dSettings_c;


class dCourseSelectManager_c : public dBase_c {
public:
	dStateWrapper_c<dCourseSelectManager_c> state;

	u32 courseSelectMenuPtr;
	u32 numberOfPeopleChangePtr;
	u32 stockItemPtr;
	u32 whateverthisisPtr;
	u32 collectionCoinPtr;
	u32 worldSelectPtr;

	u32 unk1;

	u8 guide[0x450];

	u32 ptr2dPlayer0;
	u32 ptr2dPlayer1;
	u32 ptr2dPlayer2;
	u32 ptr2dPlayer3;

	u32 easyPairingPtr;
	u32 continuePtr;
	u32 yesNoWindowPtr;
	u32 someMsgPtr;
	u32 letterWindowPtr;

	u8 layoutLoaded;

	u8 field_53D;
	u8 doesSomethingWithMenuSelect;
	u8 doesWorldSelect;
	u8 field_540;
	u8 doesContinueCheckWait;
	u8 doesSaveWindowOpen;
	u8 field_543;
	u8 doesMsgOpenAnimeEndWait;
	u8 startedSomeMsgThing;
	u8 endedSomeMsgThing;
	u8 doesYesNoWindowOpenAnimeEndWait;
	u8 doesStockItemSelectWait;
	u8 doSetPowerupsInWm2dPlayer;
	u8 field_54A;
	u8 field_54B;
	u8 field_54C;
	u8 doesLetterWindowWait;
	u8 _54E;
	u8 _54F;

	u32 selectedMenuItem;
	u32 field_554;
	u32 countdownForSomethingInMenuSelect;
	u32 somethingCopiedIntoYesNoWindow;

	u8 unk2[0x10];

	// New stuff

	dWMShop_c *shopPtr;					// 0x570
	dCreatorInfo_c *creatorInfoPtr;		// 0x574
	u8 doesSomethingWithShop;			// 0x578
	u8 doesSomethingWithCreatorInfo;	// 0x579

	u8 pad2;							// 0x57A

	u8 doesLetterOpenAnime; 			// 0x57B

	dSettings_c *settingsPtr;			// 0x57C
	dStarSign_c *signPtr;				// 0x580

	u8 pad[12];

	static dCourseSelectManager_c* instance;

	void sub_80931170();
	void sub_809310F0();

	void sub_80931090();
	void sub_80931110();

	void changeToNormalState();

	void endState_CharacterChangeWait_new();
	
	USING_STATES(dCourseSelectManager_c);
	REF_NINTENDO_STATE(KeyWait);
	REF_NINTENDO_STATE(StockItemSelectWait);
	DECLARE_STATE(ShopWait);
	DECLARE_STATE(CreatorInfoWait);

	DECLARE_STATE(SettingsWait);
	DECLARE_STATE(LetterOpenAnim);
	DECLARE_STATE(LetterDisp);
	DECLARE_STATE(LetterExitAnim);
};

#endif