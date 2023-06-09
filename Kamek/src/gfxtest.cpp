#include <game.h>
#include "path.h"


const char* gfxTestFileList[] = { NULL };

class daGfxTest_c : public dStageActor_c {
public:

	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	mEf::es2 effect;

	int selectedSFX;
	dAcPy_c* player;

	static dActor_c* build();
};


dActor_c* daGfxTest_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daGfxTest_c));
	return new(buffer) daGfxTest_c;
}

const SpriteData GfxTestSpriteData = { ProfileId::GfxTest, 0, 0, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile GfxTestProfile(&daGfxTest_c::build, SpriteId::GfxTest, &GfxTestSpriteData, ProfileId::GfxTest, ProfileId::GfxTest, "GfxTest", gfxTestFileList);


int daGfxTest_c::onCreate() {
	this->player = dAcPy_c::findByID(0);
	MakeMarioEnterDemoMode();
	this->selectedSFX = 0;

	return true;
}

int daGfxTest_c::onExecute() {
	Remocon* rem = RemoconMng->controllers[player->which_player];
	if(rem->nowPressed & WPAD_TWO || rem->heldButtons & WPAD_TWO) {
		const char* gfx = effects_name_list[this->selectedSFX];
		OSReport("GFX %d: %s\n", this->selectedSFX, gfx);
		Vec tempVec = (Vec){ player->pos.x, player->pos.y + 40, player->pos.z };
		effect.spawn(gfx, 0, &tempVec, &(S16Vec){0,0,0}, &(Vec){1.0, 1.0, 1.0});
	
	}
	if(rem->nowPressed & WPAD_ONE) {
		MakeMarioExitDemoMode();
		this->Delete(1);
	}
	if(rem->nowPressed & WPAD_UP) {
		this->selectedSFX++;
		this->selectedSFX %= sizeof(effects_name_list) / 4;
		const char* gfx = effects_name_list[this->selectedSFX];
		OSReport("GFX %d: %s\n", this->selectedSFX, gfx);
	} else if(rem->nowPressed & WPAD_DOWN) {
		this->selectedSFX--;
		if(this->selectedSFX < 0) this->selectedSFX += sizeof(effects_name_list) / 4;
		const char* gfx = effects_name_list[this->selectedSFX];
		OSReport("GFX %d: %s\n", this->selectedSFX, gfx);
	} else if(rem->nowPressed & WPAD_RIGHT || rem->heldButtons & WPAD_RIGHT) {
		this->selectedSFX+=5;
		this->selectedSFX %= sizeof(effects_name_list) / 4;
		const char* gfx = effects_name_list[this->selectedSFX];
		OSReport("GFX %d: %s\n", this->selectedSFX, gfx);
	} else if(rem->nowPressed & WPAD_LEFT || rem->heldButtons & WPAD_LEFT) {
		this->selectedSFX-=5;
		if(this->selectedSFX < 0) this->selectedSFX += sizeof(effects_name_list) / 4;
		const char* gfx = effects_name_list[this->selectedSFX];
		OSReport("GFX %d: %s\n", this->selectedSFX, gfx);
	} else if(rem->nowPressed & WPAD_PLUS || rem->heldButtons & WPAD_PLUS) {
		this->selectedSFX+=20;
		this->selectedSFX %= sizeof(effects_name_list) / 4;
		const char* gfx = effects_name_list[this->selectedSFX];
		OSReport("GFX %d: %s\n", this->selectedSFX, gfx);
	} else if(rem->nowPressed & WPAD_MINUS || rem->heldButtons & WPAD_MINUS) {
		this->selectedSFX-=20;
		if(this->selectedSFX < 0) this->selectedSFX += sizeof(effects_name_list) / 4;
		const char* gfx = effects_name_list[this->selectedSFX];
		OSReport("GFX %d: %s\n", this->selectedSFX, gfx);
	}

return true;
}

int daGfxTest_c::onDelete() {
	return true;
}

int daGfxTest_c::onDraw() {
	return true;
}