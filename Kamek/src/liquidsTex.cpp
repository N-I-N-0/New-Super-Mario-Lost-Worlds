u8 liquidSubTypes[80];
u8 nextSplashSubType;

extern "C" u8 checkWater(float, float, u8, float*);

u8 getLiquidSubType(float x, float y, u8 layer, float* unk) { //getLiquidSubType__FP7Point3d
	for (int i = 0; i < 80; i++) {
		WaterData* data = &dWaterManager_c::instance->data[i];
		if(data->isInUse) {
			OSReport("Liquid %d width: %f\n", i, data->width);
			if ((data->x <= x) && (x <= data->x + data->width)) {
				if ((y <= data->y) && (data->y - data->height <= y)) {
					nextSplashSubType = liquidSubTypes[i];
					OSReport("Found liquid: %d\n", i);
					CDPrintCurrentAddress();
					goto foundLiquidSubType;
				}
			}
		}
	}
	//nextSplashSubType = 0; //don't do this because global subType check

	foundLiquidSubType:
	return checkWater(x, y, layer, unk);
}