#include "courseSelectManager.h"

int getLevelInfoWorldNumber(int world, int subWorld) {
	/*  for WXa this returns X      (00-07)
	    for WXb this returns X + 8  (08-15)
		for WXc this returns X + 16 (16-23)
	    don't use W9!  */
	return 8 * subWorld + world;
}

extern "C" u8 CurrentWorldNumForWorldMap;
extern "C" u8 CurrentWorldNumForWorldMapSub;
extern "C" bool GetCurrentHomeToadHouseID();
u8 StartArrowRotation;

static const wchar_t *numberKinds2[] = {
	L"0",
	L"1",
	L"2",
	L"3",
	L"4",
	L"5",
	L"6",
	L"7",
	L"8",
	L"9",
	L"10",
	L"11",
	L"12",
	L"13",
	L"14",
	L"15",
	L"16",
	L"17",
	L"18",
	L"19",
	L"\x0B" L"A", // 20, alternate
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

static const wchar_t *numberKinds3[] = {
	L" 1",
	L" 2",
	L" 3",
	L" 4",
	L" 5",
	L" 6",
	L" 7",
	L" 8",
	L"S1",
	L"S2",
	L"S3",
	L"S4",
	L"S5",
	L"S6",
	L"S7",
	L"S8",
	L" A",
	L" B",
	L" C",
	L" D",
};

bool getLevelNameForDisplayNums(int world, int level, wchar_t* worldString, wchar_t* levelString) {
	wcscpy(&worldString[0], numberKinds3[world-1]);
	wcscpy(&levelString[0], numberKinds2[level]);

	
	if(level < 20) return false;
	
	return true;
}

class dWMManager_c : public dActor_c {
public:
	static dWMManager_c *build();
	static dWMManager_c *instance;
	
	dWMManager_c();
	
	int onCreate();
	int onDelete();
	int onExecute();
	int onDraw();
	
	u8 previousNodeNum;
	u8 shakeFrame;
	SaveBlock* save;
	
	m2d::EmbedLayout_c* layout;
	
	nw4r::lyt::TextBox
		*worldName, *shopText, *creatorText, *worldNum, *cSelect, *cSelectPic;
	
	mEf::es2 effects[7];
	
	int worldNumber;
	
	void spawnEffects();
};

void dWMManager_c::spawnEffects() {
	const S16Vec efRot = {0x1800, 0, 0};
	switch(CurrentWorldNumForWorldMap) {
		case 6:
			effects[0].spawn("Wm_mr_slipsmoke_ss", 0, &(Vec){655.0, -100.0, -230.0}, &efRot, &(Vec){25.0, 15.0, 10.0});
			break;
		default:
			break;
	}
}

dWMManager_c *dWMManager_c::instance = 0;

dWMManager_c *dWMManager_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dWMManager_c));
	dWMManager_c *c = new(buffer) dWMManager_c;

	instance = c;
	return c;
}

dWMManager_c::dWMManager_c() {

}

int dWMManager_c::onCreate() {
	
	int fd = IOS_Open("/dev/net/ncd/manage", 0);
	ioctlv data[2];
	u8 mac[6];
	data[0].len = 0;
	data[1].data = mac;
	data[1].len = 0x6;
	
	
	IOS_Ioctlv(fd, 8, 0, 2, data); // NCDGetWirelessMacAddress, 32 out, 6 out
	
	OSReport("Mac: %02X:%02X:%02X:%02X:%02X:%02X\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	IOS_Close(fd);
	
	layout = (m2d::EmbedLayout_c*)((int)(dCourseSelectManager_c::instance) + 208);
	
	save = GetSaveFile()->GetBlock(-1);
	
	worldName = layout->findTextBoxByName("T_world_name");
	shopText = layout->findTextBoxByName("T_guideViewL_00");
	creatorText = layout->findTextBoxByName("T_guideViewLS_01");
	worldNum = layout->findTextBoxByName("T_worldNum_00");
	cSelect = layout->findTextBoxByName("T_cSelect_00");
	cSelectPic = layout->findTextBoxByName("T_cSelect_pic");

	this->worldNumber = getLevelInfoWorldNumber(CurrentWorldNumForWorldMap, CurrentWorldNumForWorldMapSub);
	
	u8 count = 0;
	for(int i = 0; i<4; i++) {
		if (QueryPlayerAvailability(i)) {
			count++;
		}
	}
	setDiscordPresence(count, worldNumber, false);
	
	dLevelInfo_c::entry_s *level = dLevelInfo_c::s_info.searchByDisplayNum(worldNumber+1, 100);

	const char *worldNameText = dLevelInfo_c::s_info.getNameForLevel(level);
	wchar_t convertedWorldName[32];
	mbstowcs(convertedWorldName, worldNameText, 32);

	worldName->SetString(convertedWorldName);
	shopText->SetString(L"Shop " L"\x0B\x0123");
	creatorText->SetString(L"\x0B\x0143 Creator");
	
	dActor_c* player = (dActor_c*)fBase_c::search(WM_PLAYER);
	OSReport("Player: %p\n", player);
	dActor_c::create(AC_ITEM_KEY, 0, &player->pos, 0);		//spawn a paratroopa
	
	
	/*dActor_c* shop = (dActor_c*)fBase_c::search(WMShop);
	if(!shop) {
		dActor_c::create(WMShop, 0, 0, 0);
	}*/
	
	/*dActor_c* levelCredits = (dActor_c*)fBase_c::search(CreatorInfo);
	if(!levelCredits) {
		dActor_c::create(CreatorInfo, 0, 0, 0);
	}*/

/*	dActor_c* worldSelect = (dActor_c*)fBase_c::search(WM_NOTE);
	if(!worldSelect) {
		dActor_c::create(WM_NOTE, 0, 0, 0);
	}*/
	
	/*Vec airshipPos = player->pos;
	airshipPos.x += 20;
	dActor_c::create(WMAirships, 0, &airshipPos, 0);
	airshipPos.y += 200;
	dActor_c::create(WMAirships, 1, &airshipPos, 0);
	airshipPos.y -= 400;
	dActor_c::create(WMAirships, 2, &airshipPos, 0);
	
	airshipPos.x += 200;
	dActor_c::create(WMAirships, 3, &airshipPos, 0);
	airshipPos.y += 400;
	dActor_c::create(WMAirships, 4, &airshipPos, 0);
	airshipPos.y -= 200;
	dActor_c::create(WMAirships, 5, &airshipPos, 0);
	
	airshipPos.x += 200;
	dActor_c::create(WMAirships, 6, &airshipPos, 0);*/
	
	return true;
}


int dWMManager_c::onDelete() {
	return true;
}



int dWMManager_c::onExecute() {
	Remocon* rem = GetActiveRemocon();
	int nowPressed = Remocon_GetPressed(rem);
	if(rem->controllerType == 0) {	//Wiimote
		shopText->SetString(L"Shop " L"\x0B\x0123");
	} else {
		shopText->SetString(L"Shop " L"\x0B\x012B");
	}
	
	WriteBMGToTextBox(creatorText, GetBMG(), 7, 2, 0);
	
	//if(nowPressed & WPAD_B && nowPressed & WPAD_A) DoSceneChange(FILE_LIST, 0, 0);
	if(nowPressed & WPAD_B && nowPressed & WPAD_A) DoSceneChange(TARZAN_ROPE, 0, 0); //Sound Test
	
	u8 CurrentNodeNum = *(u8*)((int)(dCourseSelectManager_c::instance) + 0x4D7);



	//OSReport("GetCurrentHomeToadHouseID: %s\n", GetCurrentHomeToadHouseID() ? "true" : "false");
	
	
	  
	/*for(int i = 0; i < 10; i++) {
		OSReport("toad_level_idx %d: %d\n", i, save->toad_level_idx[i]);
	}*/

	if(CurrentNodeNum != previousNodeNum) {
		wchar_t levelString[4];
		wchar_t worldString[4];
		uint worldNumber = getLevelInfoWorldNumber(CurrentWorldNumForWorldMap, CurrentWorldNumForWorldMapSub);

		cSelect->SetString(L"    ");
		cSelectPic->SetString(L"    ");
		worldNum->SetString(L"    ");

		if(CurrentNodeNum != 255) {
			if(CurrentNodeNum == 38) {
				wcscpy(&worldString[0], numberKinds3[worldNumber]);
				
				switch(save->new_toad_level_idx[worldNumber]) {
					case 0:
						switch(StartArrowRotation) {
							case 1:
								cSelectPic->SetString(L"\x0B\x0135");
								break;
							case 2:
								cSelectPic->SetString(L"\x0B\x0143");
								break;
							case 3:
								cSelectPic->SetString(L"\x0B\x0144");
								break;
							default:
								cSelectPic->SetString(L"\x0B\x0134");
								break;
						}
						break;
					case 1:
					case 4:
						cSelectPic->SetString(L"\x0B\x0138");
						//yellow?
						break;
					case 2:
					case 5:
						cSelectPic->SetString(L"\x0B\x0133");
						//red?
						break;
					default:
						cSelectPic->SetString(L"\x0B\x0139");
						//grren?
						break;
				}
				cSelect->SetString(L"");
				worldNum->SetString(worldString);
			} else {
				dLevelInfo_c::entry_s *level = dLevelInfo_c::s_info.searchBySlot(worldNumber, CurrentNodeNum);
				if(level) {
					bool isPictureLevel = getLevelNameForDisplayNums(level->displayWorld, level->displayLevel, &worldString[0], &levelString[0]);
				
					OSReport("level: %d, world: %d, displayLevel: %d, displayWorld: %d\n", CurrentNodeNum, worldNumber, level->displayLevel, level->displayWorld);
					
					worldNum->SetString(worldString);
					if(isPictureLevel) {
						cSelectPic->SetString(levelString);
						cSelect->SetString(L"");
					} else {
						cSelect->SetString(levelString);
						cSelectPic->SetString(L"");
					}
				} else {
					OSReport("Level %02x-%02d not found in LevelInfo\n", worldNumber+1, CurrentNodeNum+1);
					wcscpy(&worldString[0], numberKinds3[worldNumber]);
					cSelectPic->SetString(L"");
					cSelect->SetString(L"?");
					worldNum->SetString(worldString);
				}
			}
			previousNodeNum = CurrentNodeNum;
		} else {
			OSReport("is -1\n");
			wcscpy(&worldString[0], numberKinds3[worldNumber]);
			cSelectPic->SetString(L"\x0B\x0136");
			cSelect->SetString(L"");
			worldNum->SetString(worldString);
		}
	}
	
	spawnEffects();
	
	return true;
}

int dWMManager_c::onDraw() {
	return true;
}