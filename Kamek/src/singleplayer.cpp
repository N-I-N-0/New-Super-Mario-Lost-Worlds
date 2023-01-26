#include <game.h>
#include <common.h>

bool playerFuckeryEnabled = false;
u32 PlayerActiveBackup[4] = {0, 0, 0, 0};
u32 PlayerIDBackup[4] = {0, 0, 0, 0};
u32 realPlayer;

/*
int dAcPy_c::newOnCreate() {
	int orig_val = this->onCreate_orig();

	if(playerFuckeryEnabled) {
		this->which_player = realPlayer;
	}

	return orig_val;
}

int dAcPy_c::newOnExecute() {
	int orig_val = this->onExecute_orig();

	if(playerFuckeryEnabled) {
		if(this->which_player != realPlayer) {
			this->aPhysics.info.xDistToCenter = 0.0;
			this->aPhysics.info.yDistToCenter = 0.0;
			this->removeMyActivePhysics();
			this->scale = (Vec){0, 0, 0};
			this->pos = GetSpecificPlayerActor(realPlayer)->pos;
		}
	}

	return orig_val;
}
*/