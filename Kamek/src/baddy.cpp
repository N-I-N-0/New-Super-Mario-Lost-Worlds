#include <game.h>
#include "baddy.h"

void setCheckpointActivatedTrue()
{
	chekpointActivated = true;

	OSReport("Set Checkpoint Activated True\n");
}

bool checkStarChipReset(bool afterCheckpoint)
{
	if (!alreadyChecked)
	{
		if (afterCheckpoint)
		{
			for (int i = 0; i < 32; i++)
			{
				for (int j = 0; j < 5; j++)
				{
					launchStarChipCollectedAfterFlag[i][j] = false;
				}
			}

			alreadyChecked = true;
			return true;
		}
		else
		{
			for (int i = 0; i < 32; i++)
			{
				for (int j = 0; j < 5; j++)
				{
					launchStarChipCollectedBeforeFlag[i][j] = false;
					launchStarChipCollectedAfterFlag[i][j] = false;
				}
			}

			alreadyChecked = true;
			return false;
		}
	}

	return false;
}

float tan(float x)
{
	return sin(x) / cos(x);
}

double tan(double x)
{
	return sin((float)x) / cos((float)x);
}

float tanAngle(float x)
{
	return sin(x * M_PI / 180) / cos(x * M_PI / 180);
}

double tanAngle(double x)
{
	return sin((float)(x * M_PI / 180)) / cos((float)(x * M_PI / 180));
}