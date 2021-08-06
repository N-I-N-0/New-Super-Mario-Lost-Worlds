int getLevelInfoWorldNumber(int world, int subWorld) {
	/*  for WXa this returns X      (00-07)
	    for WXb this returns X + 8  (08-15)
		for WXc this returns X + 16 (16-23)
	    don't use W9!  */
	return 8 * subWorld + world;
}

extern "C" u8 CurrentWorldNumForWorldMap;
extern "C" u8 CurrentWorldNumForWorldMapSub;

class dWMManager_c : public dActor_c {
public:
	static dWMManager_c *build();
	static dWMManager_c *instance;
	
	dWMManager_c();
	
	int onCreate();
	int onDelete();
	int onExecute();
	int onDraw();
	
	m2d::EmbedLayout_c* layout;
	
	nw4r::lyt::TextBox
		*worldName, *shopText;
};

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
	dActor_c* csMng = (dActor_c*)fBase_c::search(COURSE_SELECT_MANAGER);
	layout = (m2d::EmbedLayout_c*)((int)(csMng) + 208);
	
	worldName = layout->findTextBoxByName("T_world_name");
	shopText = layout->findTextBoxByName("T_guideViewL_00");

	uint worldNum = getLevelInfoWorldNumber(CurrentWorldNumForWorldMap, CurrentWorldNumForWorldMapSub);
	
	dLevelInfo_c::entry_s *level = dLevelInfo_c::s_info.searchByDisplayNum(worldNum+1, 100);

	const char *worldNameText = dLevelInfo_c::s_info.getNameForLevel(level);
	wchar_t convertedWorldName[32];
	for (int i = 0; i < 32; i++) {
		convertedWorldName[i] = worldNameText[i];
		if (convertedWorldName[i] == 0)
			break;
	}
	convertedWorldName[31] = 0;

	worldName->SetString(convertedWorldName);
	shopText->SetString(L"Shop " L"\x0B\x0123");
	
	dActor_c* player = (dActor_c*)fBase_c::search(WM_PLAYER);
	OSReport("Player: %p\n", player);
	dActor_c::create(AC_ITEM_KEY, 0, &player->pos, 0);		//spawn a paratroopa
	
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
	return true;
}

int dWMManager_c::onDraw() {
	return true;
}