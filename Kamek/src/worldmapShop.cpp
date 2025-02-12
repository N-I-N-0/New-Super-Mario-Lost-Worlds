#include "courseSelectManager.h";

// TODO:  - 0x80926ba0 LoadFilesForWorldMap: re-add lakitu and items
//          - Warning: eventually the poweruplist was already changed before in boomeranghax, poweruphax or some stockItemFix file ...
//                     WorldMap folder list handled in worldmapGrid.S
//          - also add all new items
//        - The cursor doesn't get deleted together with the rest of the actor
//        - eventually make the shop have 2 pages with the first one being a new stockItem and the second the shop
//          - in this case bind the spawning of this actor to button 1
//          - no need for a mushroom house shop on the map
//        - unhardcode shop names and items
//          - load from planned worldInfo.bin
//        - re-add texmapcolouriser functionality with colours from worldInfo.bin
//        - check why the game crashes after the following sequence of events:
//          - open shop while on level node
//          - press 2 -> buying something while starting the level
//          - finish the level
//          - try to re-open/spawn the shop
//          >> probably the problem is that the bool containing the info whether the shop is spawned is true while there is no shop
//             the code in worldmapGird.cpp then tries to delete the non existend shop and crashes the game ...


const char* WMShopFileList[] = {NULL};
Profile WMShopProfile(&dWMShop_c::build, ProfileId::WMShop, NULL, ProfileId::WMShop, ProfileId::WMShop, "WMShop", WMShopFileList);

extern int PtrToWM_CS_SEQ_MNG;
extern "C" bool FUN_801017c0(int, int, int, int, int);
extern "C" void FUN_808fbd10(int);
extern "C" void dCourseSelectGuide_c__loadLives(int);

CREATE_STATE(dWMShop_c, Hidden);
CREATE_STATE(dWMShop_c, ShowWait);
CREATE_STATE(dWMShop_c, ButtonActivateWait);
CREATE_STATE(dWMShop_c, CoinCountdown);
CREATE_STATE(dWMShop_c, Wait);
CREATE_STATE(dWMShop_c, WaitForEndOfAnims);
CREATE_STATE(dWMShop_c, HideWait);


int getUnspentStarCoinCount() {
	SaveBlock *save = GetSaveFile()->GetBlock(-1);
	int coinsSpent = save->spentStarCoins;
	return getStarCoinCount() - coinsSpent;
}

int getStarCoinCount() {
	SaveBlock *save = GetSaveFile()->GetBlock(-1);
	int coinsEarned = 0;

	for (int w = 0; w < 10; w++) {
		for (int l = 0; l < 42; l++) {
			u32 conds = save->GetLevelCondition(w, l);

			if (conds & COND_COIN1) { coinsEarned++; }
			if (conds & COND_COIN2) { coinsEarned++; }
			if (conds & COND_COIN3) { coinsEarned++; }
		}
	}

	return coinsEarned;
}

void dWMShop_c::ShopModel_c::setupItem(float x, float y, ItemTypes type) {
	static const char* Produce[ITEM_TYPE_COUNT][4] = { 
		{ "I_kinoko", 		"g3d/I_kinoko.brres", 			"I_kinoko", 			"wait2" },
		{ "I_fireflower", 	"g3d/I_fireflower.brres", 		"I_fireflower", 		"wait2" },
		{ "I_propeller", 	"g3d/I_propeller.brres", 		"I_propeller_model", 	"wait2" },
		{ "I_iceflower", 	"g3d/I_iceflower.brres", 		"I_iceflower", 			"wait2" },
		{ "I_penguin", 		"g3d/I_penguin.brres", 			"I_penguin", 			"wait2" },
		{ "I_kinoko_bundle","g3d/I_mini_kinoko.brres", 		"I_mini_kinoko", 		"wait2" },
		{ "I_star", 		"g3d/I_star.brres", 			"I_star", 				"wait2" },
		{ "I_hammer", 		"g3d/I_fireflower.brres",		"I_fireflower",			"wait2" },
		{ "I_gold",			"g3d/I_fireflower.brres",		"I_fireflower", 		"wait2" },
		{ "I_spike",		"g3d/I_fireflower.brres",		"I_fireflower", 		"wait2" },
		{ "I_boomerang",	"g3d/I_fireflower.brres",		"I_fireflower", 		"wait2" },
		{ "I_frog",			"g3d/I_fireflower.brres",		"I_fireflower", 		"wait2" },
		{ "I_cloud",		"g3d/I_fireflower.brres",		"I_fireflower", 		"wait2" },
		{ "I_wand",			"g3d/I_fireflower.brres",		"I_fireflower", 		"wait2" },
		{ "I_kinoko_bundle","g3d/I_life_kinoko.brres", 		"I_life_kinoko", 		"wait2" },
		
	};

	this->x = x;
	this->y = y;
	
	int id = (int)type;
	isLakitu = false;

	allocator.link(-1, GameHeaps[0], 0, 0x20);

	res.data = getResource(Produce[id][0], Produce[id][1]);
	nw4r::g3d::ResMdl mdlRes = res.GetResMdl(Produce[id][2]);
	model.setup(mdlRes, &allocator, 0x224, 1, 0);

	SetupTextures_Item(&model, 1);

	nw4r::g3d::ResAnmChr anmChr = res.GetResAnmChr(Produce[id][3]);
	animation.setup(mdlRes, anmChr, &allocator, 0);
	playAnim(Produce[id][3], 1.0f, 0);

	allocator.unlink();
}

void dWMShop_c::ShopModel_c::setupLakitu(int id) {
	static const char* models[10] = { 
		"g3d/yoshi.brres", "g3d/desert.brres", "g3d/mountain.brres", "g3d/sakura.brres", "g3d/santa.brres", 
		"g3d/ghost.brres", "g3d/space.brres", "g3d/koopa.brres", "g3d/sewer.brres", "g3d/goldwood.brres" 
	};

	scaleFactor = 1.2f;
	scaleEase = 0.0f;
	this->isLakitu = true;

	allocator.link(-1, GameHeaps[0], 0, 0x20);

	res.data = getResource("Rosalina", "g3d/rosalina.brres"/*models[id]*/);
	nw4r::g3d::ResMdl mdlRes = res.GetResMdl("rosalina");
	model.setup(mdlRes, &allocator, 0x224, 1, 0);
	SetupTextures_Enemy(&model, 0);

	nw4r::g3d::ResAnmChr anmChr = res.GetResAnmChr("wait");
	animation.setup(mdlRes, anmChr, &allocator, 0);

	playAnim("wait", 1.0f, 0);
	playingNotEnough = false;

	allocator.unlink();
}

void dWMShop_c::ShopModel_c::playAnim(const char *name, float rate, char loop) {
	nw4r::g3d::ResAnmChr anmChr = res.GetResAnmChr(name);
	animation.bind(&model, anmChr, loop);
	model.bindAnim(&animation, 0.0f);
	animation.setUpdateRate(rate);
}

void dWMShop_c::ShopModel_c::execute() {
	model._vf1C();

	if(this->animation.isAnimationDone()) {
		if (this->isLakitu) {
			playAnim("wait", 1.0f, 0);
			this->animation.setCurrentFrame(0.0);
			if (playingNotEnough) {
				playingNotEnough = false;
			}
		}
	}	
}

void dWMShop_c::ShopModel_c::draw() {
	mMtx mtx;
	mtx.translation(x, y, 1000.0f);
	model.setDrawMatrix(mtx);

	float buttonScale = 1.0f;
	if(associatedButton >= 0) {
		if(associatedButton < 4) buttonScale = dWMShop_c::instance->Buttons[associatedButton]->effectiveMtx[1][1];
		else if(associatedButton == 4) buttonScale = dWMShop_c::instance->Btn1Base->effectiveMtx[1][1];
		else if(associatedButton == 5) buttonScale = dWMShop_c::instance->Btn2Base->effectiveMtx[1][1];
	}

	float s = scaleFactor * scaleEase * buttonScale;

	Vec scale = {s, s, s};
	model.setScale(&scale);

	model.calcWorld(false);
	model.scheduleForDrawing();
}



dWMShop_c *dWMShop_c::instance = 0;

dActor_c* dWMShop_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dWMShop_c));
	dWMShop_c *c = new(buffer) dWMShop_c;

	instance = c;
	return c;
}

dWMShop_c::dWMShop_c() : state(this, &StateID_Hidden) {
	layoutLoaded = false;
	visible = false;
}

int dWMShop_c::onCreate() {
	showShop = false;
	if (!layoutLoaded) {
		bool gotFile = layout.loadArc("shop.arc", false);
		if (!gotFile)
			return false;

		selected = 0;
		lastTopRowChoice = 0;

		layout.build("shop.brlyt");

		if (IsWideScreen()) {
			layout.layout.rootPane->scale.x = 0.735f;
		} else {
			layout.clippingEnabled = true;
			layout.clipX = 0;
			layout.clipY = 52;
			layout.clipWidth = 640;
			layout.clipHeight = 352;
			layout.layout.rootPane->scale.x = 0.731f;
			layout.layout.rootPane->scale.y = 0.7711f;
		}

		static const char *brlanNames[] = {
			"shop_Show.brlan",
			"shop_Hide.brlan",
			"shop_InButton.brlan",
			"shop_OnButton.brlan",
			"shop_IdleButton.brlan",
			"shop_HitButton.brlan",
			"shop_OffButton.brlan",
			"shop_CountCoin.brlan"
		};
		static const char *groupNames[] = {
			"BaseGroup", "BaseGroup",
			"GBtn00", "GBtn01", "GBtn02", "GBtn03", "GBtn1", "GBtn2",
			"GBtn00", "GBtn01", "GBtn02", "GBtn03", "GBtn1", "GBtn2",
			"GBtn00", "GBtn01", "GBtn02", "GBtn03", "GBtn1", "GBtn2",
			"GBtn00", "GBtn01", "GBtn02", "GBtn03", "GBtn1", "GBtn2",
			"GBtn00", "GBtn01", "GBtn02", "GBtn03", "GBtn1", "GBtn2",
			"GCoinCount"
		};
		static const int brlanIDs[] = {
			0, 1,
			2, 2, 2, 2, 2, 2,
			3, 3, 3, 3, 3, 3,
			4, 4, 4, 4, 4, 4,
			5, 5, 5, 5, 5, 5,
			6, 6, 6, 6, 6, 6,
			7,
		};

		layout.loadAnimations(brlanNames, 8);
		layout.loadGroups(groupNames, brlanIDs, 33);
		layout.disableAllAnimations();

		layout.drawOrder = 1;

		static const char *tbNames[] = {
			"Title", "TitleShadow",
			"CoinCount", "CoinCountShadow",
			"BackText", "BuyText",
		};
		layout.getTextBoxes(tbNames, &Title, 6);

		// Warning: weird code coming up
		const char *crap = "000102031\0" "2\0";
		char name[12];
		for (int i = 0; i < 6; i++) {
			strcpy(name, "BtnLeftXX");
			name[7] = crap[i*2];
			name[8] = crap[i*2+1];
			BtnLeft[i] = layout.findPictureByName(name);

			strcpy(name, "BtnMidXX");
			name[6] = crap[i*2];
			name[7] = crap[i*2+1];
			BtnMid[i] = layout.findPictureByName(name);

			strcpy(name, "BtnRightXX");
			name[8] = crap[i*2];
			name[9] = crap[i*2+1];
			BtnRight[i] = layout.findPictureByName(name);

			strcpy(name, "BtnXX");
			name[3] = crap[i*2];
			name[4] = crap[i*2+1];
			Buttons[i] = layout.findPaneByName(name);
		}

		Btn1Base = layout.findPaneByName("Btn1");
		Btn2Base = layout.findPaneByName("Btn2");
		
		/*leftCol.setTexMap(BtnLeft[0]->material->texMaps);
		midCol.setTexMap(BtnMid[0]->material->texMaps);
		rightCol.setTexMap(BtnRight[0]->material->texMaps);

		for (int i = 1; i < 6; i++) {
			leftCol.applyAlso(BtnLeft[i]->material->texMaps);
			midCol.applyAlso(BtnMid[i]->material->texMaps);
			rightCol.applyAlso(BtnRight[i]->material->texMaps);
		}*/

		this->base_type = 2; // This is necessary for specialDraw1 to be executed.

		layoutLoaded = true;
	}

	return true;
}


int dWMShop_c::onDelete() {
	deleteModels();
	return layout.free();
}


int dWMShop_c::onExecute() {
	state.execute();

	if (visible) {
		lakituModel->execute();

		for (int i = 0; i < 12; i++)
			itemModels[i].execute();
	}

	layout.execAnimations();
	layout.update();

	return true;
}

int dWMShop_c::onDraw() {
	if (visible)
		layout.scheduleForDrawing();

	return true;
}

void dWMShop_c::specialDraw1() {
	if (visible) {
		lakituModel->scaleEase = scaleEase * 2.5f;
		lakituModel->draw();
		for (int i = 0; i < 12; i++) {
			itemModels[i].scaleEase = scaleEase;
			itemModels[i].draw();
		}
	}
}


void dWMShop_c::show(int shopNumber) {
	shopKind = shopNumber;
	state.setState(&StateID_ShowWait);
}

void makeShopShowUp() {
	dWMShop_c::instance->showShop = true;
}

// Hidden
void dWMShop_c::beginState_Hidden() { }
void dWMShop_c::executeState_Hidden() {
	if (showShop) {
		showShop = false;
		state.setState(&StateID_ShowWait);
		FUN_801017c0(PtrToWM_CS_SEQ_MNG, 0x35, 0, 0, 0x80);
	}
}
void dWMShop_c::endState_Hidden() { }

// ShowWait
void dWMShop_c::beginState_ShowWait() {
	MapSoundPlayer(SoundRelatedClass, SE_SYS_DIALOGUE_IN, 1);

	layout.disableAllAnimations();
	layout.enableNonLoopAnim(SHOW_ALL);
	visible = true;
	scaleEase = 0.0;

	loadInfo();
	loadModels();
}
void dWMShop_c::executeState_ShowWait() {
	if (!layout.isAnimOn(SHOW_ALL)) {
		selected = 0;
		layout.enableNonLoopAnim(ON_BUTTON);
		state.setState(&StateID_ButtonActivateWait);
	}
}
void dWMShop_c::endState_ShowWait() {
	MapSoundPlayer(SoundRelatedClass, SE_OBJ_CLOUD_BLOCK_TO_JUGEM, 1);
	timer = 1;
}

// ButtonActivateWait
void dWMShop_c::beginState_ButtonActivateWait() { }
void dWMShop_c::executeState_ButtonActivateWait() {
	if (!layout.isAnyAnimOn())
		state.setState(&StateID_Wait);
}
void dWMShop_c::endState_ButtonActivateWait() { }

// Wait
void dWMShop_c::beginState_Wait() {
	showSelectCursor();
}
void dWMShop_c::executeState_Wait() {
	if (timer < 90) {
		scaleEase = -((cos(timer * 3.14 /20)-0.9)/timer*10)+1;
		timer++;
		return;
	}

	int nowPressed = Remocon_GetPressed(GetActiveRemocon());

	int newSelection = -1;

	if (nowPressed & WPAD_ONE) {
		// Hide the thing
		state.setState(&StateID_HideWait);

	} else if (nowPressed & WPAD_UP) {
		// Move up
		if (selected == 4)
			newSelection = lastTopRowChoice;
		else if (selected == 5)
			newSelection = 4;

	} else if (nowPressed & WPAD_DOWN) {
		// Move down
		if (selected <= 3)
			newSelection = 4;
		else if (selected == 4)
			newSelection = 5;

	} else if (nowPressed & WPAD_LEFT) {
		// Just use the previous choice
		if (selected > 0)
			newSelection = selected - 1;

	} else if (nowPressed & WPAD_RIGHT) {
		// Just use the next choice
		if (selected < 5)
			newSelection = selected + 1;

	} else if (nowPressed & WPAD_TWO) {
		if (!lakituModel->playingNotEnough) {
			layout.enableNonLoopAnim(HIT_BUTTON+selected);
			buyItem(selected);
		} else
			OSReport("Not Enough is still playing!\n");
	}

	if (newSelection > -1) {
		MapSoundPlayer(SoundRelatedClass, SE_SYS_CURSOR, 1);

		layout.enableNonLoopAnim(OFF_BUTTON+selected);
		layout.enableNonLoopAnim(ON_BUTTON+newSelection);

		selected = newSelection;
		if (newSelection <= 3)
			lastTopRowChoice = newSelection;

		HideSelectCursor(SelectCursorPointer, 0);
		state.setState(&StateID_WaitForEndOfAnims);
	}
}
void dWMShop_c::endState_Wait() { }

// WaitForEndOfAnims
void dWMShop_c::beginState_WaitForEndOfAnims() {}
void dWMShop_c::executeState_WaitForEndOfAnims() {
	for(int i = 0; i < 6; i++)
		if(layout.isAnimOn(ON_BUTTON+i))
			return;
	for(int i = 0; i < 6; i++)
		if(layout.isAnimOn(OFF_BUTTON+i))
			return;

	state.setState(&StateID_Wait);
}
void dWMShop_c::endState_WaitForEndOfAnims() {}

// HideWait
void dWMShop_c::beginState_HideWait() {
	MapSoundPlayer(SoundRelatedClass, SE_SYS_DIALOGUE_OUT_AUTO, 1);
	layout.enableNonLoopAnim(HIDE_ALL);
	layout.enableNonLoopAnim(OFF_BUTTON+selected);

	timer = 26;
	MapSoundPlayer(SoundRelatedClass, SE_OBJ_CS_KINOHOUSE_DISAPP, 1);

	HideSelectCursor(SelectCursorPointer, 0);
}
void dWMShop_c::executeState_HideWait() {
	if (timer > 0) {
		timer--;
		scaleEase = -((cos(timer * 3.14 /13.5)-0.9)/timer*10)+1;
		if (scaleEase < 0.0f)
			scaleEase = 0.0f;
	}

	if (!layout.isAnimOn(HIDE_ALL) && timer == 0) {
		dActor_c* wmDirector = (dActor_c*)fBase_c::search(WM_DIRECTOR);
		// dCourseSelectManager_c::instance->layoutLoaded = 1;	//unhide gameScene
		FUN_808fbd10((int)wmDirector);				//unfreeze map

		state.setState(&StateID_Hidden);
	}
}
void dWMShop_c::endState_HideWait() {
	deleteModels();
	visible = false;
}


// Powerup Listing:
// 0 = Mushroom		- 1
	// 1 = Fireflower	- 2
	// 2 = Iceflower	- 2
		// 3 = Penguin		- 3
		// 4 = Propeller	- 3
	// 5 = MiniShroom	- 2
	// 6 = Starman		- 2
		// 7 = Hammer		- 3
	// 8 = 1-ups		- 2
//
// Format: 1coin, 1coin, 2coins, 3coins, 5coins[3] (Value 6-9), 8coins[5] (Value 10-15)
//
// Possible 5 coin combos =  2,2,2  /  1,2,3  /  2,3,2  /  3,2,3  /  3,3,3
// Possible 8 coin combos =  1,1,2,3,3  /  1,2,2,3,3  /  1,2,3,3,3  /  2,2,2,3,3  /  2,2,3,3,3  /  1,3,3,3,3  /  2,3,3,3,3  /  3,3,3,3,3

const dWMShop_c::ItemTypes dWMShop_c::Inventory[10][12] = { 
	{ // Yoshi's Island
		MUSHROOM, FIRE_FLOWER, ICE_FLOWER, PROPELLER,
		HAMMER, GOLD_FLOWER, SPIKE_SHROOM,
		BOOMERANG, FROG, CLOUD, WAND, ONE_UP
	},
	{ // Desert
		MUSHROOM, FIRE_FLOWER, ICE_FLOWER, PROPELLER,
		FIRE_FLOWER, STARMAN, FIRE_FLOWER,
		MUSHROOM, FIRE_FLOWER, FIRE_FLOWER, PROPELLER, PROPELLER
	},
	{ // Mountain
		MUSHROOM, FIRE_FLOWER, MINI_SHROOM, PROPELLER,
		MUSHROOM, MINI_SHROOM, PROPELLER,
		MUSHROOM, MINI_SHROOM, PROPELLER, PROPELLER, HAMMER
	},
	{ // Japan
		MUSHROOM, FIRE_FLOWER, ONE_UP, HAMMER,
		ONE_UP, ONE_UP, ONE_UP,
		PROPELLER, ICE_FLOWER, ONE_UP, FIRE_FLOWER, PROPELLER
	},
	{ // FreezeFlame
		MUSHROOM, FIRE_FLOWER, ICE_FLOWER, PENGUIN,
		ICE_FLOWER, PENGUIN, ICE_FLOWER,
		ICE_FLOWER, PENGUIN, PENGUIN, PENGUIN, ICE_FLOWER
	},
	{ // Ghost
		MUSHROOM, FIRE_FLOWER, STARMAN, PROPELLER,
		MINI_SHROOM, PROPELLER, MINI_SHROOM,
		PROPELLER, PROPELLER, MUSHROOM, PROPELLER, PROPELLER
	},
	{ // Space
		MUSHROOM, STARMAN, ONE_UP, HAMMER,
		STARMAN, STARMAN, STARMAN,
		HAMMER, HAMMER, ONE_UP, HAMMER, HAMMER
	},
	{ // Koopa
		MUSHROOM, ONE_UP, PROPELLER, HAMMER,
		HAMMER, PROPELLER, HAMMER,
		PROPELLER, HAMMER, PROPELLER, HAMMER, PROPELLER
	},
	{ // Unknown
		MUSHROOM, MUSHROOM, MUSHROOM, MUSHROOM,
		MUSHROOM, MUSHROOM, MUSHROOM,
		MUSHROOM, MUSHROOM, MUSHROOM, MUSHROOM, MUSHROOM
	},
	{ // Goldwood
		MUSHROOM, FIRE_FLOWER, ONE_UP, PENGUIN,
		FIRE_FLOWER, PROPELLER, FIRE_FLOWER,
		FIRE_FLOWER, FIRE_FLOWER, STARMAN, FIRE_FLOWER, FIRE_FLOWER
	}
};

void dWMShop_c::loadModels() {
	lakituModel = new ShopModel_c;
	lakituModel->setupLakitu(shopKind);
	lakituModel->associatedButton = -1;
	lakituModel->x = 240.0f;
	lakituModel->y = 200.0f;
	if (!IsWideScreen()) {
		lakituModel->x = (0.731f * (lakituModel->x + 416.0f)) - 292.0f;
		lakituModel->y *= 0.7711f;
		lakituModel->y += 52.0f;
		lakituModel->scaleFactor = 1.2f;
	}

	static const float itemPos[ITEM_COUNT][2] = {
		{357.0f, 276.0f},
		{450.0f, 276.0f},
		{543.0f, 276.0f},
		{636.0f, 276.0f},

		{380.0f, 190.0f},
		{462.0f, 190.0f},
		{544.0f, 190.0f},

		{363.0f, 104.0f},
		{413.0f, 104.0f},
		{463.0f, 104.0f},
		{513.0f, 104.0f},
		{563.0f, 104.0f},
	};

	itemModels = new ShopModel_c[ITEM_COUNT];
	
	for (int i = 0; i < ITEM_COUNT; i++) {
		float effectiveX = itemPos[i][0];
		float effectiveY = itemPos[i][1];
		if (!IsWideScreen()) {
			effectiveX = (0.731f * (effectiveX + 416.0f)) - 296.0f;
			effectiveY *= 0.7711f;
			effectiveY += 50.0f;
			itemModels[i].scaleFactor = 1.75f;
		} else {
			itemModels[i].scaleFactor = 2.3f;	
		}
		itemModels[i].setupItem(effectiveX, effectiveY, Inventory[shopKind][i]);


		if(i < 5) itemModels[i].associatedButton = i;
		else if(i < 7) itemModels[i].associatedButton = 4;
		else itemModels[i].associatedButton = 5;
	}
}
void dWMShop_c::deleteModels() {
	if (lakituModel)
		delete lakituModel;
	lakituModel = 0;

	if (itemModels)
		delete[] itemModels;
	itemModels = 0;
}


void dWMShop_c::loadInfo() {
	SaveBlock *save = GetSaveFile()->GetBlock(-1);

	//missing color information in original save file!
	s16 hue        = 138;
	s8  saturation = 50;
	s8  lightness  = 30;
	
	//leftCol.colourise(save->hudHintH, save->hudHintS, save->hudHintL);
	//midCol.colourise(save->hudHintH, save->hudHintS, save->hudHintL);
	//rightCol.colourise(save->hudHintH, save->hudHintS, save->hudHintL);
	/*leftCol.colourise(hue, saturation, lightness);
	midCol.colourise(hue, saturation, lightness);
	rightCol.colourise(hue, saturation, lightness);*/

	// find out the shop name
	dLevelInfo_c::entry_s *shopNameEntry =
		dLevelInfo_c::s_info.searchBySlot(shopKind, 98);

	wchar_t shopName[100];
	// TODO: refactor this a bit
	const char *sourceName = dLevelInfo_c::s_info.getNameForLevel(shopNameEntry);
	int charCount = 0;
	
	while (*sourceName != 0 && charCount < 99) {
		shopName[charCount] = *sourceName;
		sourceName++;
		charCount++;
	}
	shopName[charCount] = 0;

	Title->SetString(shopName);
	TitleShadow->SetString(shopName);

	// load the coin count
	int scCount = getUnspentStarCoinCount();
	WriteNumberToTextBox(&scCount, CoinCount, false);
	WriteNumberToTextBox(&scCount, CoinCountShadow, false);


	//WriteBMGToTextBox(BackText, GetBMG(), 2, 58, 0);
	//WriteBMGToTextBox(BuyText, GetBMG(), 302, 4, 0);
}


void dWMShop_c::buyItem(int item) {
	static int itemDefs[6][3] = {
		// Cost, Start Index, Count
		{1, 0, 1}, {2, 1, 1}, {2, 2, 1}, {3, 3, 1},
		{5, 4, 3}, {8, 7, 5}
	};

	int cost = itemDefs[item][0], cash = getUnspentStarCoinCount();

	if (cost > cash) {
		OSReport("Started playing Not Enough\n");
		//lakituModel->playingNotEnough = true;
		//lakituModel->playAnim("notenough", 1.0f, 1);
		MapSoundPlayer(SoundRelatedClass, SE_SYS_INVALID, 1);
		return;
	}

	MapSoundPlayer(SoundRelatedClass, SE_SYS_DECIDE, 1);

	SaveFile *file = GetSaveFile();
	SaveBlock *block = file->GetBlock(file->header.current_file);

	coinsRemaining = cost;

	// Work out what we need to apply
	int appliedItems[ITEM_TYPE_COUNT];
	for (int i = 0; i < ITEM_TYPE_COUNT; i++)
		appliedItems[i] = 0;

	int invStartIndex = itemDefs[item][1], invCount = itemDefs[item][2];
	for (int i = 0; i < invCount; i++)
		appliedItems[(int)Inventory[shopKind][invStartIndex+i]]++;

	dStockItem_c* stockItemPtr = (dStockItem_c*)fBase_c::search(STOCK_ITEM);
	for (int i = 0; i < 14; i++) {
		block->new_powerups_available[i] += appliedItems[i];

		if (block->new_powerups_available[i] > 99)
			block->new_powerups_available[i] = 99;

		stockItemPtr->newCounts[i] = block->new_powerups_available[i];

	}

	// Apply lives to everyone
	for (int i = 0; i < 4; i++) {
		if (Player_Active[i]) {
			int id = Player_ID[i];
			Player_Lives[id] += appliedItems[(int)ONE_UP];
			if (Player_Lives[id] > 99)
				Player_Lives[id] = 99;
		}
	}

	if (appliedItems[(int)ONE_UP] > 0)
		MapSoundPlayer(SoundRelatedClass, SE_SYS_100COIN_ONE_UP, 1);

	dCourseSelectGuide_c__loadLives((int)(dCourseSelectManager_c::instance) + 200);


	state.setState(&StateID_CoinCountdown);
	HideSelectCursor(SelectCursorPointer, 0);
}


void dWMShop_c::beginState_CoinCountdown() {
	timerForCoinCountdown = 8;
}

void dWMShop_c::endState_CoinCountdown() { }

void dWMShop_c::executeState_CoinCountdown() {
	timerForCoinCountdown--;
	if (timerForCoinCountdown <= 0) {

		SaveBlock *save = GetSaveFile()->GetBlock(-1);
		save->spentStarCoins++;

		// load the coin count
		int scCount = getUnspentStarCoinCount();
		WriteNumberToTextBox(&scCount, CoinCount, false);
		WriteNumberToTextBox(&scCount, CoinCountShadow, false);

		layout.enableNonLoopAnim(COUNT_COIN);
		VEC3 efPos = {
			CoinCount->effectiveMtx[0][3],
			CoinCount->effectiveMtx[1][3],
			0.0f};

		// ARGHHHHHHHHHHHHHHHHh.
		if (IsWideScreen()) {
			float div = 5.0f;
			if (scCount < 100)
				div = 3.6f;
			if (scCount < 10)
				div = 2.7f;
			efPos.x -= (CoinCount->size.x / div);
			efPos.y -= (CoinCount->size.y / 2.0f);
		} else {
			float div = 1.0f;
			if (scCount < 1000)
				div = 5.8f;
			if (scCount < 100)
				div = 8.2f;
			if (scCount < 10)
				div = 14.5f;
			efPos.x += (CoinCount->size.x / div);
			efPos.y -= (CoinCount->size.y / 2.8f);
		}

		VEC3 efScale = {0.7f, 0.7f, 0.7f};
		SpawnEffect("Wm_2d_moviecoinvanish", 0, &efPos, 0, &efScale);

		coinsRemaining--;
		if (coinsRemaining <= 0) {
			MapSoundPlayer(SoundRelatedClass, SE_PLY_GET_ITEM_AGAIN, 1);
			state.setState(&StateID_Wait);
		} else {
			MapSoundPlayer(SoundRelatedClass, SE_SYS_STAR_COIN_PAY, 1);
			beginState_CoinCountdown();
		}
	}
}


void dWMShop_c::showSelectCursor() {
	switch (selected) {
		case 4: UpdateSelectCursor(Btn1Base, 0, false); break;
		case 5: UpdateSelectCursor(Btn2Base, 0, false); break;
		default: UpdateSelectCursor(Buttons[selected], 0, false);
	}
}

