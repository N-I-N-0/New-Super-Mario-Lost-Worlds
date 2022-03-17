#include <common.h>
#include <game.h>
#include <g3dhax.h>
#include <sfx.h>

extern "C" bool SpawnEffect(const char*, int, Vec*, S16Vec*, Vec*);

class EffectVideo : public dEn_c {
	int onCreate();
	int onExecute();
	int onDelete();

	u64 eventFlag;
	s32 timer;
	u32 delay;

	u32 effect;
	u8 type;
	float effect_scale;

	static EffectVideo *build();

};


EffectVideo *EffectVideo::build() {
	void *buffer = AllocFromGameHeap1(sizeof(EffectVideo));
	return new(buffer) EffectVideo;
}


int EffectVideo::onCreate() {
	
	this->timer = 0;

	char eventNum	= (this->settings >> 24) & 0xFF;

	this->eventFlag = (u64)1 << (eventNum - 1);
	
	this->type		= (this->settings >> 16) & 0xF;
	this->effect	= this->settings & 0xFFF;
	this->effect_scale	= float((this->settings >> 20) & 0xF) / 4.0;
	this->delay		= (this->settings >> 12) & 0xF * 30;
	
	if (this->effect_scale == 0.0) { this->effect_scale = 1.0; }

	this->onExecute();
	return true;
}


int EffectVideo::onDelete() {
	return true;
}


int EffectVideo::onExecute() {

	if (dFlagMgr_c::instance->flags & this->eventFlag) {

		if (this->timer == this->delay) {

			if (this->type == 0) { // Plays a sound
				PlaySoundAsync(this, this->effect);
			}
	
			else {	// Plays an Effect

				
				// Effect notes:
				// Values 1-675 will always work
				// Values 676-746 require Iggy (sprites 337/372) or the Magic Fireball Spawn Area
				// Values 747-821 require Bowser Jr. (sprites 211/405/406) or the Bowser Jr. Castle Cutscene (sprite 408)
				// Values 822-906 require Kamek (sprite 383)
				// Values 907-981 require Bowser (sprite 419) and Bowser Fireballs (sprite 449) with the custom code
				// Values 982-1045 require Larry (sprites 189/192) or the Magic Fireball Spawn Area
				// Values 1046-1109 require Lemmy (sprites 340/381)
				// Values 1110-1174 require Ludwig (sprites 348/365)
				// Values 1175-1258 require Morton (sprites 344/349)
				// Values 1259-1329 require Roy (sprite 364) or the Magic Fireball Spawn Area
				// Values 1330-1395 require Wendy (sprites 336/375)
				// Values 1396-1608 only work in Multiplayer mode
				// Values 1609-1661 only work if the Ambush flag in the Area Settings is enabled

				const char *efName = effects_name_list[this->effect];

				if (efName != 0) {
					S16Vec zero = {0, 0, 0};
					Vec3 scale_vec = {this->effect_scale, this->effect_scale, this->effect_scale};
					SpawnEffect(efName, 0, &this->pos, &zero, &scale_vec);
				}
			}
	
			this->timer = 0;
			if (this->delay == 0) { this->delay = -1; }
		}
		
		this->timer += 1;
	}
	return true;
}
