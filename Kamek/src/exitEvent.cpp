#include <game.h>
#include <stage.h>
#include "msgbox.h"

const char *ExitEventFileList[] = {"OpeningScene", 0};

class dExitEvent_c : public dStageActor_c {
	public:
		static dActor_c *build();

		bool hasBeenActivated;
		bool hasExitedStage;
		int onExecute();
};

/*****************************************************************************/
// Glue Code
dActor_c *dExitEvent_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dExitEvent_c));
	dExitEvent_c *c = new(buffer) dExitEvent_c;
	return c;
}

const SpriteData ExitEventSpriteData = { ProfileId::ExitEvent, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile ExitEventProfile(&dExitEvent_c::build, SpriteId::ExitEvent, &ExitEventSpriteData, ProfileId::ExitEvent, ProfileId::ExitEvent, "ExitEvent", ExitEventFileList);


int dExitEvent_c::onExecute() {
	if (dFlagMgr_c::instance->flags & spriteFlagMask) {
		if (!hasBeenActivated) {
//			OSReport("Activating Palace Dude\n");
			hasBeenActivated = true;

			int delay;
			if (!(settings & 0xF000000)) {
				delay = 270;
				StopBGMMusic();
				SaveBlock *save = GetSaveFile()->GetBlock(-1);
				GameMgrP->switchPalaceFlag |= (1 << (settings >> 28));
			} else {
				delay = 1020;
			}

			dMsgBoxManager_c::instance->showMessage(
				settings & 0xFFFFFF, // message ID
				false, // cannot cancel
				delay // delay
				);
		}
	}

	if (hasBeenActivated) {
		if (hasExitedStage)
			return true;
//		OSReport("Palace Dude is activated, %d\n", dMsgBoxManager_c::instance->visible);
		if (!dMsgBoxManager_c::instance->visible) {
//			OSReport("Exiting\n");
			u32 wmsettings = 0;
			if (settings & 0xF000000) {
				SaveGame(0, false);
				wmsettings = 0x80000000;
			}
			SaveGame(0, true);
			ExitStage(WORLD_MAP, wmsettings, BEAT_LEVEL, MARIO_WIPE);
			hasExitedStage = true;
		}
	}

	return true;

}


