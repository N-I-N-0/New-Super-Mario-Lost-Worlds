#include <game.h>
#include <actors.h>

void doStarEffect(daYoshi_c* yoshi) {
	//OSReport("somePlayerID: %d\n", somePlayerID);
	OSReport("Yoshi: %p\n", yoshi);
	dAcPy_c *player;
	for (int i = 0; i < 4; i++) {
		if (player = dAcPy_c::findByID(i)) {
			if(player->getYoshi() == yoshi) {
				goto found;
			}
		}
	}
	return;
	found:
	dPlayerModelHandler_c *pmh = (dPlayerModelHandler_c*)(((u32)player) + 0x2A60);
    //pmh->mdlClass->startAnimation(132, 1.0f, 0.0f, 0.0f);
	pmh->mdlClass->enableStarEffects();
	//pmh->mdlClass->enableStarColours();
}