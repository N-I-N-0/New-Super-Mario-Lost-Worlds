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

	worldName->SetString(L"Test World Name");
	shopText->SetString(L"Shop " L"\x0B\x0123");

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