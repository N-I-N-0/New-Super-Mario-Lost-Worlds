#include <game.h>
#include <common.h>
#include <actors.h>

void shellSpriteCollision(dEn_c *me, ActivePhysics *apThis, ActivePhysics *apOther) {
	if(apOther->owner->name == EN_JUMPDAI) {
		u8* jumpdaiTimer = (u8*)((u32)(apOther->owner) + 0x632);
		if(*jumpdaiTimer == 0) {
			*jumpdaiTimer = 0x11;

			MapSoundPlayer(SoundRelatedClass, 0x297, 1);
		}
		else if(*jumpdaiTimer == 8) {
			me->speed.y = 6.0f;
		}
	}
}