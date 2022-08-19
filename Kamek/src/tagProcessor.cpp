#include <common.h>

extern "C" u32 loadBMGStringToMyBuffer(void* tagProc, void* bmg, u32 unk, u8 result);

u8 tagConstants[] = {14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 25, 24, 29, 30, 31, 88, 89, 91, 92, 93};

u32 fmtTag5_getLevelPicture(void* tagProc, void* bmg, void* tagArgsPointer) {
	u8 tag = *(u8*)(tagArgsPointer);

	return loadBMGStringToMyBuffer(tagProc, bmg, 0, tagConstants[tag]);
}