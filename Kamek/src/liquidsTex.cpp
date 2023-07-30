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


u32 tryToGetBehaviourForQuicksandEffects(float x, float y, u8 layer) {
	u32 behaviour = collisionMgr_c::getTileBehaviour2At(x, y, layer);
	if ((behaviour >> 16 & 0xFF) != 0x03) {
		//OSReport("Behaviour at (%f, %f): 0x%X\n", x, y, behaviour);
		behaviour = collisionMgr_c::getTileBehaviour2At(x, y - 32, layer);
	}
	return behaviour;
}


extern void FUN_8008fba0(int player_num, mEf::es2* effect, char* effName, u32 unk1, Vec* pos, u32 unk2, float* unk3);

u32 lastQuicksandEffects1Type[] = {0, 0, 0, 0};

char* quicksandEffects1[] = {
	"Wm_mr_sandsplash",
	"Wm_mr_sandsplashSnow",
	"Wm_mr_sandsplashRed",
	"Wm_mr_sandsplashMud",
};
void getSandSubType1(u32 player_num, u32 effectPtr, char* effName, u32 unk1, u32 posPtr, u32 unk2, float* unk3) {
	Vec* pos = (Vec*)posPtr;
	u32 behaviour = tryToGetBehaviourForQuicksandEffects(pos->x, pos->y, 0);
	if ((behaviour >> 16 & 0xFF) != 0x03) {
		FUN_8008fba0(player_num, (mEf::es2*)effectPtr, quicksandEffects1[lastQuicksandEffects1Type[player_num]], unk1, pos, unk2, unk3);
	} else {
		u8 subType = behaviour >> 24 & 0xFF;
		lastQuicksandEffects1Type[player_num] = subType;
		//OSReport("I'm interesting: Sand subType at (%f, %f): 0x%X\n", pos->x, pos->y, subType);
		FUN_8008fba0(player_num, (mEf::es2*)effectPtr, quicksandEffects1[subType], unk1, pos, unk2, unk3);
	}
}


u32 lastQuicksandEffects2Type[] = {0, 0, 0, 0};

char* quicksandEffects2[] = {
	"Wm_mr_quicksand",
	"Wm_mr_quicksandSnow",
	"Wm_mr_quicksandRed",
	"Wm_mr_quicksandMud",
};
void getSandSubType2(u32 player_num, u32 effectPtr, char* effName, u32 unk1, u32 posPtr, u32 unk2, float* unk3) {
	Vec* pos = (Vec*)posPtr;
	u32 behaviour = tryToGetBehaviourForQuicksandEffects(pos->x, pos->y, 0);
	if ((behaviour >> 16 & 0xFF) != 0x03) { //if we can't find quicksand tiles use last saved subType
		FUN_8008fba0(player_num, (mEf::es2*)effectPtr, quicksandEffects2[lastQuicksandEffects2Type[player_num]], unk1, pos, unk2, unk3);
	} else {
		u8 subType = behaviour >> 24 & 0xFF;
		lastQuicksandEffects2Type[player_num] = subType;
		FUN_8008fba0(player_num, (mEf::es2*)effectPtr, quicksandEffects2[subType], unk1, pos, unk2, unk3);
	}
}

extern void FUN_8008fb60(int player_num, char* effName, u32 unk, Vec* pos, S16Vec* rot, Vec* scale);

u32 lastQuicksandEffects3Type[] = {0, 0, 0, 0};

char* quicksandEffects3[] = {
		"Wm_mr_sanddive",
		"Wm_mr_sanddive_m",
		"Wm_mr_sanddive_s",
	
		"Wm_mr_sanddiveSnow",
		"Wm_mr_sanddive_mSnow",
		"Wm_mr_sanddive_sSnow",

		"Wm_mr_sanddiveRed",
		"Wm_mr_sanddive_mRed",
		"Wm_mr_sanddive_sRed",

		"Wm_mr_sanddiveMud",
		"Wm_mr_sanddive_mMud",
		"Wm_mr_sanddive_sMud",
};
void getSandSubType3(u32 player_num, u32 index, u32 unk, u32 posPtr, u32 rotPtr, u32 scalePtr) {
	Vec* pos = (Vec*)posPtr;
	S16Vec* rot = (S16Vec*)rotPtr;
	Vec* scale = (Vec*)scalePtr;
	u32 behaviour = tryToGetBehaviourForQuicksandEffects(pos->x, pos->y, 0);
	if ((behaviour >> 16 & 0xFF) != 0x03) {
		FUN_8008fb60(player_num, quicksandEffects3[lastQuicksandEffects3Type[player_num]*3+index], unk, pos, rot, scale);
	} else {
		u8 subType = behaviour >> 24 & 0xFF;
		lastQuicksandEffects3Type[player_num] = subType;
		//OSReport("Sand subType at (%f, %f): 0x%X\n", pos->x, pos->y, subType);
		FUN_8008fb60(player_num, quicksandEffects3[subType*3+index], unk, pos, rot, scale);
	}
}



u32 lastQuicksandEffects4Type[] = {0, 0, 0, 0};

char* quicksandEffects4[] = {
	"Wm_mr_sanddive_s",
	"Wm_mr_sanddive_sSnow",
	"Wm_mr_sanddive_sRed",
	"Wm_mr_sanddive_sMud",
};
void getSandSubType4(u32 player_num, char* effName, u32 unk, u32 posPtr, u32 rotPtr, u32 scalePtr) {
	Vec* pos = (Vec*)posPtr;
	S16Vec* rot = (S16Vec*)rotPtr;
	Vec* scale = (Vec*)scalePtr;
	u32 behaviour = tryToGetBehaviourForQuicksandEffects(pos->x, pos->y, 0);
	if ((behaviour >> 16 & 0xFF) != 0x03) { //if we can't find quicksand tiles use last saved subType
		FUN_8008fb60(player_num, quicksandEffects4[lastQuicksandEffects4Type[player_num]], unk, pos, rot, scale);
	} else {
		u8 subType = behaviour >> 24 & 0xFF;
		lastQuicksandEffects4Type[player_num] = subType;
		//OSReport("Sand subType at (%f, %f): 0x%X\n", pos->x, pos->y, subType);
		FUN_8008fb60(player_num, quicksandEffects4[subType], unk, pos, rot, scale);
	}
}