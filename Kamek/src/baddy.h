#ifndef __BADDY_H
#define __BADDY_H

#include <common.h>
#include <game.h>
#include <dCourse.h>

extern "C" dCourse_c::rail_s * GetRail(int id);

class daEnLaunchStar_c;
class daEnPullStar_c;
class daEnPullStarManager_c;
class daEnSuperLaunchStar_c;
class daEnShyCart_c;
class daEnRail_c;

daEnShyCart_c* playerCart[4] = {};

bool launchStarChipCollectedBeforeFlag[32][5];
bool launchStarChipCollectedAfterFlag[32][5];
bool alreadyChecked;
bool chekpointActivated;

u8 playerStatus[4] = { 0,0,0,0 };
Vec pullAngle[4] = { 0,0,0,0 };
Vec lastPos[4] = { 0,0,0,0 };
bool dragged[4] = { false,false,false,false };

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
#endif