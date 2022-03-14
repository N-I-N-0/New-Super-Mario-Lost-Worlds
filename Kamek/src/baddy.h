#ifndef __BADDY_H
#define __BADDY_H

#include <common.h>
#include <game.h>
#include <dCourse.h>

extern "C" dCourse_c::rail_s * GetRail(int id);

class daEnLaunchStar_c;
class daEnShyCart_c;
class daEnRail_c;

daEnShyCart_c* playerCart[4] = {};

bool launchStarChipCollectedBeforeFlag[32][5];
bool launchStarChipCollectedAfterFlag[32][5];

u8 playerStatus[4] = { 0,0,0,0 };

#endif