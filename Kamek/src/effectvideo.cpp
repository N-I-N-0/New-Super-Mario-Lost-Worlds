#include <common.h>
#include <game.h>
#include <g3dhax.h>
#include <sfx.h>

extern "C" bool SpawnEffect(const char*, int, Vec*, S16Vec*, Vec*);

class EffectVideo : public dEn_c {
public:
	int onCreate();
	int onExecute();
	int onDelete();

	u64 eventFlag;
	s32 timer;
	int delay;

	u32 effect;
	u8 type;
	float scale;
	bool globalSound;

	static dActor_c *build();

};

dActor_c *EffectVideo::build() {
	void *buffer = AllocFromGameHeap1(sizeof(EffectVideo));
	return new(buffer) EffectVideo;
}

const char* EffectVideoFileList[] = { 0 };
const SpriteData EffectVideoSpriteData = { ProfileId::EffectVideo, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile EffectVideoProfile(&EffectVideo::build, SpriteId::EffectVideo, &EffectVideoSpriteData, ProfileId::EffectVideo, ProfileId::EffectVideo, "EffectVideo", EffectVideoFileList);


int EffectVideo::onCreate() {
	this->timer = 0;

	char eventNum	= (this->settings >> 24) & 0xFF;

	this->eventFlag = (u64)1 << (eventNum - 1);
	
	this->type		= (this->settings >> 16) & 0x1;
	this->globalSound = (this->settings >> 17) & 0x1;
	this->effect	= this->settings & 0xFFF;
	this->scale		= float((this->settings >> 20) & 0xF) / 4.0;
	this->delay		= ((this->settings >> 12) & 0xF) * 30;
	
	if (this->scale == 0.0) { this->scale = 1.0; }

	this->onExecute();
	return true;
}


int EffectVideo::onDelete() {
	return true;
}


int EffectVideo::onExecute() {
	// OSReport("SFX: %d %08X\n", this->delay, this->settings);
	if (dFlagMgr_c::instance->flags & this->eventFlag) {
		OSReport("Triggered!\n");
		if (this->timer == this->delay) {
			if (this->type == 0) { // Plays a sound
				if(this->globalSound) 
					MapSoundPlayer(SoundRelatedClass, this->effect, 1);
				else
					PlaySoundAsync(this, this->effect);
			}
			else {	// Plays an Effect

				const char *efName = 0;

				efName = effects_name_list[this->effect % sizeof(effects_name_list)];


				if (efName != 0) {
					S16Vec nullRot = {0,0,0};
					Vec efScale = {scale,scale,scale};
					SpawnEffect(efName, 0, &this->pos, &nullRot, &efScale);
				}
			}
	
			this->timer = 0;
			if (this->delay == 0) { this->delay = -1; }
		}
		
		this->timer += 1;
	}
	return true;
}
