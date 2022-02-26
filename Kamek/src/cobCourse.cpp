#include <game.h>
#include "levelinfo.h"

extern "C" void bindAnimCobCourse(u32 thisPtr, u32 animID, float unk1, float unk2);
extern "C" u8 CurrentWorldNumForWorldMap;
extern "C" u8 CurrentWorldNumForWorldMapSub;
extern "C" int getLevelInfoWorldNumber(int world, int subWorld);

void cobCourseNewColor(u32 thisPtr, u32 animID, float unk1, float unk2) {
	uint worldNum = getLevelInfoWorldNumber(CurrentWorldNumForWorldMap, CurrentWorldNumForWorldMapSub);
	uint levelNum = *(u32*)(thisPtr + 4) & 0xFF;

	dLevelInfo_c::entry_s *level = dLevelInfo_c::s_info.searchBySlot(worldNum, levelNum);

	if(level) {
		if(level->flags & 0x20) {
			OSReport("SExit: %02d-%02d\n", worldNum+1, levelNum+1);

			SaveBlock *save = GetSaveFile()->GetBlock(-1);
			u32 conds = save->GetLevelCondition(worldNum, levelNum);

			if(!((conds & COND_NORMAL) && (conds & COND_SECRET))) {
				bindAnimCobCourse(thisPtr, 3, unk1, unk2);
				return;
			}
		}
	}

	bindAnimCobCourse(thisPtr, 0, unk1, unk2);
}