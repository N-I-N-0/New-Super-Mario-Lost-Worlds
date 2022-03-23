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

void setCheckpointActivatedTrue();

bool checkStarChipReset(bool afterCheckpoint);

float tan(float x);
double tan(double x);
float tanAngle(float x);
double tanAngle(double x);
#endif