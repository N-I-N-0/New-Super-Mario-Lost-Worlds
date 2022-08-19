#include "talkBalloon.h"

class daKanbanArrow_c : public dActorState_c {
public:
	mHeapAllocator_c allocator;
	nw4r::g3d::ResFile resFile;
	m3d::mdl_c model;
	m3d::anmChr_c animationChr;

	float animFrame;
	u32 unk; // Unused?

	bool showingTB;


	void executeState_Wait();

};

void daKanbanArrow_c::executeState_Wait() {
	u8 textID = (this->settings >> 28) & 0xF; // Nybble 5
	if(textID) {
		bool doIt = false;
		for(int i = 0; i < 4; i++) {
			if(daPlBase_c* player = GetPlayerOrYoshi(i)) {
				if(player->pos.x > this->pos.x - 48.0f && player->pos.x < this->pos.x + 48.0f
				&& player->pos.y > this->pos.y - 48.0f && player->pos.y < this->pos.y + 48.0f) {
					doIt = true;
				}
			}
		}

		if(doIt && !showingTB) {
			int textIDsArray[] = {0xD, 0xE, 0xF, 0x10, 0x11, 0x12};
			dTalkBalloon_c::instance->showTB(textIDsArray[textID-1], this->pos);
			showingTB = true;
		}
		else if(!doIt && showingTB) {
			dTalkBalloon_c::instance->hideTB();
			showingTB = false;
		}
	}
}