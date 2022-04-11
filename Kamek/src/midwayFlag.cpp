bool midwayFlagActivated;

bool launchStarChipCollectedBeforeFlag[32][5];      //These are here cause they get reset by one function here
bool launchStarChipCollectedAfterFlag[32][5];

void SetMidwayBoolTrue() {
	midwayFlagActivated = true;
	
	OSReport("SetMidwayTrue\n");
}

bool SetMidwayBoolTrue2(bool isActive) {
	midwayFlagActivated = isActive;
	
	for (int i = 0; i < 32; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			launchStarChipCollectedAfterFlag[i][j] = false;
		}
	}
	
	OSReport("SetMidwayTrue2\n");
	
	return isActive;
}

void SetMidwayBoolFalse() {
	midwayFlagActivated = false;
	
	for (int i = 0; i < 32; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			launchStarChipCollectedBeforeFlag[i][j] = false;
			launchStarChipCollectedAfterFlag[i][j] = false;
		}
	}
	
	OSReport("SetMidwayFalse\n");
}

