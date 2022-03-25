u8 liquidSubTypes[80];
u8 nextSplashSubType;

extern "C" u8 checkWater(float, float, u8, float*);

u8 getLiquidSubType(float x, float y, u8 layer, float* unk) { //getLiquidSubType__FP7Point3d
	for (int i = 0; i < 80; i++) {
		WaterData* data = &dWaterManager_c::instance->data[i];
		if(data->isInUse) {
			if ((data->x <= x) && (x <= data->x + data->width)) {
				if ((y <= data->y) && (data->y - data->height <= y)) {
					nextSplashSubType = liquidSubTypes[i];
					goto foundLiquidSubType;
				}
			}
		}
	}
	//nextSplashSubType = 0; //don't do this because global subType check

	foundLiquidSubType:
	return checkWater(x, y, layer, unk);
}


u32 tryToGetBehaivourForQuicksandEffects(float x, float y, u8 layer) {
	u32 behaivour = collisionMgr_c::getTileBehaviour2At(x, y, layer);
	if ((behaivour >> 16 & 0xFF) != 0x03) {
		OSReport("Behaivour at (%f, %f): 0x%X\n", x, y, behaivour);
		behaivour = collisionMgr_c::getTileBehaviour2At(x, y - 32, layer);
	}
	return behaivour;
}


extern void FUN_8008fba0(int player_num, mEf::es2* effect, char* effName, u32 unk1, Vec* pos, u32 unk2, float* unk3);

char* quicksandEffects1[] = {
	"Wm_mr_sandsplash",
	"Wm_mr_sandsplash",
	"Wm_mr_sandsplash",
	"Wm_mr_sandsplash",
	"Wm_mr_sandsplash",
};
void getSandSubType1(u32 player_num, u32 effectPtr, char* effName, u32 unk1, u32 posPtr, u32 unk2, float* unk3) {	//getLiquidSubType__FP7Point3d
	Vec* pos = (Vec*)posPtr;																						//couldn't find the function name needed for the yaml
	u32 behaivour = tryToGetBehaivourForQuicksandEffects(pos->x, pos->y, 0);
	if ((behaivour >> 16 & 0xFF) != 0x03) { //if we can't find quicksand tiles simply use the original code as fallback!
		FUN_8008fba0(player_num, (mEf::es2*)effectPtr, effName, unk1, pos, unk2, unk3);
		return;
	}

	u8 subType = behaivour >> 24 & 0xFF;
	OSReport("Sand subType at (%f, %f): 0x%X\n", pos->x, pos->y, subType);
	FUN_8008fba0(player_num, (mEf::es2*)effectPtr, quicksandEffects1[subType], unk1, pos, unk2, unk3);
	return;
}

char* quicksandEffects2[] = {
	"Wm_mr_quicksand",
	"Wm_mr_quicksand",
	"Wm_mr_quicksand",
	"Wm_mr_quicksand",
	"Wm_mr_quicksand",
};
void getSandSubType2(u32 player_num, u32 effectPtr, char* effName, u32 unk1, u32 posPtr, u32 unk2, float* unk3) {	//getLiquidSubType__FP7Point3d
	Vec* pos = (Vec*)posPtr;																						//couldn't find the function name needed for the yaml
	u32 behaivour = tryToGetBehaivourForQuicksandEffects(pos->x, pos->y, 0);
	if ((behaivour >> 16 & 0xFF) != 0x03) { //if we can't find quicksand tiles simply use the original code as fallback!
		FUN_8008fba0(player_num, (mEf::es2*)effectPtr, effName, unk1, pos, unk2, unk3);
		return;
	}

	u8 subType = behaivour >> 24 & 0xFF;
	OSReport("Sand subType at (%f, %f): 0x%X\n", pos->x, pos->y, subType);
	FUN_8008fba0(player_num, (mEf::es2*)effectPtr, quicksandEffects2[subType], unk1, pos, unk2, unk3);
	return;
}

extern void FUN_8008fb60(int player_num, char* effName, u32 unk, Vec* pos, S16Vec* rot, Vec* scale);

char* quicksandEffects3[] = {
		"Wm_mr_sanddive",
		"Wm_mr_sanddive_m",
		"Wm_mr_sanddive_s",
	
		"Wm_mr_sanddive",
		"Wm_mr_sanddive_m",
		"Wm_mr_sanddive_s",

		"Wm_mr_sanddive",
		"Wm_mr_sanddive_m",
		"Wm_mr_sanddive_s",

		"Wm_mr_sanddive",
		"Wm_mr_sanddive_m",
		"Wm_mr_sanddive_s",

		"Wm_mr_sanddive",
		"Wm_mr_sanddive_m",
		"Wm_mr_sanddive_s",
};
void getSandSubType3(u32 player_num, u32 index, u32 unk, u32 posPtr, u32 rotPtr, u32 scalePtr) {	//getLiquidSubType__FP7Point3d
	Vec* pos = (Vec*)posPtr;																		//couldn't find the function name needed for the yaml
	S16Vec* rot = (S16Vec*)rotPtr;
	Vec* scale = (Vec*)scalePtr;
	u32 behaivour = tryToGetBehaivourForQuicksandEffects(pos->x, pos->y, 0);
	if ((behaivour >> 16 & 0xFF) != 0x03) { //if we can't find quicksand tiles simply use the original code as fallback!
		FUN_8008fb60(player_num, quicksandEffects3[index], unk, pos, rot, scale);
		return;
	}

	u8 subType = behaivour >> 24 & 0xFF;
	OSReport("Sand subType at (%f, %f): 0x%X\n", pos->x, pos->y, subType);
	FUN_8008fb60(player_num, quicksandEffects3[subType*3+index], unk, pos, rot, scale);
	return;
}



char* quicksandEffects4[] = {
	"Wm_mr_sanddive_s",
	"Wm_mr_sanddive_s",
	"Wm_mr_sanddive_s",
	"Wm_mr_sanddive_s",
	"Wm_mr_sanddive_s",
};
void getSandSubType4(u32 player_num, char* effName, u32 unk, u32 posPtr, u32 rotPtr, u32 scalePtr) {	//getLiquidSubType__FP7Point3d
	Vec* pos = (Vec*)posPtr;																			//couldn't find the function name needed for the yaml
	S16Vec* rot = (S16Vec*)rotPtr;
	Vec* scale = (Vec*)scalePtr;
	u32 behaivour = tryToGetBehaivourForQuicksandEffects(pos->x, pos->y, 0);
	if ((behaivour >> 16 & 0xFF) != 0x03) { //if we can't find quicksand tiles simply use the original code as fallback!
		FUN_8008fb60(player_num, quicksandEffects4[0], unk, pos, rot, scale);
		return;
	}

	u8 subType = behaivour >> 24 & 0xFF;
	OSReport("Sand subType at (%f, %f): 0x%X\n", pos->x, pos->y, subType);
	FUN_8008fb60(player_num, quicksandEffects4[subType], unk, pos, rot, scale);
	return;
}