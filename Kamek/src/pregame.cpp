#include <game.h>
#include <newer.h>

class PregameLytHandler {
	public:
		m2d::EmbedLayout_c layout;

		nw4r::lyt::Pane *rootPane;

		nw4r::lyt::TextBox
			*T_minus_00, *T_world_00, *T_worldNum_00,
			*T_pictureFont_00, *T_corseNum_00,
			*T_remainder_00, *T_remainder_01, *T_remainder_02, *T_remainder_03,
			*T_remainder_10, *T_remainder_11, *T_remainder_12, *T_remainder_13,
			*T_x_00, *T_x_01, *T_x_02, *T_x_03, *T_x_10, *T_x_11, *T_x_12, *T_x_13,
			*T_x_00_o, *T_x_10_o,
			*T_otasukePlay_00, *T_otasukePlay_01,
			*T_recommend_00, *T_remainder_00_o, *T_remainder_10_o;

		nw4r::lyt::Picture
			*P_Wx_00[9], *P_coin_00, *P_free_00,
			*P_batB_0x[4], *P_bat_00,
			*P_batB_1x[4], *P_bat_01,
			*P_batB_2x[4], *P_bat_02,
			*P_batB_3x[4], *P_bat_03,
			*P_luijiIcon_00_o, *P_luijiIcon_10_o, *P_coinStage_00;

		nw4r::lyt::Pane
			*N_mario_00, *N_luiji_00, *N_kinoB_01, *N_kinoY_00,
			*N_zankiPos_x[4], *N_zanki_00,
			*Null_battPosxP[4], *N_batt_x[4],
			*N_batt, *N_otasukePlay_00;

		u8 layoutLoaded, somethingHasBeenDone, isVisible, hasShownLuigiThing_orSomething;

		u32 currentStateID;

		u32 _2E8;

		u32 countdownToEndabilityCopy, activePlayerCountMultBy4_maybe;
		u32 batteryLevels[4];
		u32 pgCountdown;

		void hijack_loadLevelNumber(); // replaces 80B6BDD0
};

// Notes:
// Deleted; P_coinStage_00, T_recommend_00, T_worldNum_00,
// T_-_00, T_pictureFont_00, T_corseNum_00, T_world_00
// P_Wx_00, P_coin_00, P_free_00

extern char CurrentLevel;
extern char CurrentWorld;

void getNewerLevelNumberString(int world, int level, wchar_t *dest) {
	static const wchar_t *numberKinds[] = {
		// 0-19 are handled by code
		// To insert a picturefont character:
		// \x0B\x01YY\xZZZZ
		// YY is the character code, ZZZZ is ignored
		L"A", // 20, alternate
		L"\x0B\x0148\xBEEF", // 21, tower
		L"\x0B\x0148\xBEEF" L"2", // 22, tower 2
		L"\x0B\x012E\xBEEF", // 23, castle
		L"\x0B\x012F\xBEEF", // 24, fortress
		L"\x0B\x013D\xBEEF", // 25, final castle
		L"\x0B\x014D\xBEEF", // 26, train
		L"\x0B\x0132\xBEEF", // 27, airship
		L"Palace", // 28, switch palace
		L"\x0B\x0147\xBEEF", // 29, yoshi's house
		L"\x0B\x014E\xBEEF" L"1", // 30, key 1
		L"\x0B\x014E\xBEEF" L"2", // 31, key 2
		L"\x0B\x014E\xBEEF" L"3", // 32, key 3
		L"\x0B\x014E\xBEEF" L"4", // 33, key 4
		L"\x0B\x014E\xBEEF" L"5", // 34, key 5
		L"\x0B\x014E\xBEEF" L"6", // 35, key 6
		L"\x0B\x0138\xBEEF", // 36, music house
		L"\x0B\x0133\xBEEF", // 37, shop
		L"\x0B\x0139\xBEEF", // 38, challenge house
		L"\x0B\x0151\xBEEF", // 39, red switch palace
		L"\x0B\x0152\xBEEF", // 40, blue switch palace
		L"\x0B\x0153\xBEEF", // 41, yellow switch palace
		L"\x0B\x0154\xBEEF", // 42, green switch palace
	};

	dest[0] = (world >= 10) ? (world-10+'A') : (world+'0');
	dest[1] = '-';
	if (level >= 20) {
		wcscpy(&dest[2], numberKinds[level-20]);
	} else if (level >= 10) {
		dest[2] = '1';
		dest[3] = ('0' - 10) + level;
		dest[4] = 0;
	} else {
		dest[2] = '0' + level;
		dest[3] = 0;
	}
}

void LoadPregameStyleNameAndNumber(m2d::EmbedLayout_c *layout) {
	nw4r::lyt::TextBox
		*LevelNumShadow, *LevelNum,
		*LevelNameShadow, *LevelName;

	LevelNumShadow = layout->findTextBoxByName("LevelNumShadow");
	LevelNum = layout->findTextBoxByName("LevelNum");
	LevelNameShadow = layout->findTextBoxByName("LevelNameShadow");
	LevelName = layout->findTextBoxByName("LevelName");

	// work out the thing now
	dLevelInfo_c::entry_s *level = dLevelInfo_c::s_info.searchBySlot(CurrentWorld, getActualLevelNum(CurrentLevel));
	if (level) {
		wchar_t convLevelName[160];
		const char *srcLevelName = dLevelInfo_c::s_info.getNameForLevel(level);
		int i = 0;
		while (i < 159 && srcLevelName[i]) {
			convLevelName[i] = srcLevelName[i];
			i++;
		}
		convLevelName[i] = 0;
		LevelNameShadow->SetString(convLevelName);
		LevelName->SetString(convLevelName);

		wchar_t levelNumber[32];
		wcscpy(levelNumber, L"World ");
		getNewerLevelNumberString(level->displayWorld, level->displayLevel, &levelNumber[6]);

		LevelNum->SetString(levelNumber);

		// make the picture shadowy
		int sidx = 0;
		while (levelNumber[sidx]) {
			if (levelNumber[sidx] == 11) {
				levelNumber[sidx+1] = 0x200 | (levelNumber[sidx+1]&0xFF);
				sidx += 2;
			}
			sidx++;
		}
		LevelNumShadow->SetString(levelNumber);

	} else {
		LevelNameShadow->SetString(L"Not found in LevelInfo!");
		LevelName->SetString(L"Not found in LevelInfo!");
	}
}


extern bool playerFuckeryEnabled;
extern u32 realPlayer;
#include "fileload.h"
void PregameLytHandler::hijack_loadLevelNumber() {
	if(playerFuckeryEnabled) {
		Player_Active[0] = 1;
		Player_Active[1] = 0;
		Player_Active[2] = 0;
		Player_Active[3] = 0;
		Player_ID[0] = Player_ID[realPlayer];
	}
	
	LoadPregameStyleNameAndNumber(&layout);

	nw4r::lyt::Picture *LevelSample;
	LevelSample = layout.findPictureByName("LevelSample");

	// this is not the greatest way to read a file but I suppose it works in a pinch
	char tplName[64];
	sprintf(tplName, "/LevelSamples/%02d-%02d.tpl", CurrentWorld+1, getActualLevelNum(CurrentLevel)+1);
	static File tpl;
	if (tpl.open(tplName)) {
		LevelSample->material->texMaps[0].ReplaceImage((TPLPalette*)tpl.ptr(), 0);
	}
}




