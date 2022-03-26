#ifndef __NSMLW_H
#define __NSMLW_H

#include <common.h>
#include <game.h>
#include <g3dhax.h>

extern "C" void destroyBarrel(dStageActor_c*);

namespace LiQ {
	char NearestPlayer(dStageActor_c* actor) 
	{
		char nearest = -1;
		float current = 1000000000000000000000000000000.0;

		for(char ii = 0; ii < 4; ii++) {
			dStageActor_c* player = GetSpecificPlayerActor(ii);
			if(!player) {
				continue;
			}
			float distance = VECDistance(&actor->pos, &player->pos);
			if(distance < current) {
				current = distance;
				nearest = ii;
			}
		}
		if(nearest < 0) {
		}
		return nearest;
	}
	
}

#endif