#ifndef __CROWDCLAPPER_H
#define __CROWDCLAPPER_H

#include <common.h>
#include <game.h>

class daNiceCoin_c : public dActorState_c {
public:
	u32 collectionID;	//0x3D0
	u32 thingsToDo;		//0x3D4
	u32 isRegular;		//0x3D8

	u32 yellowCoinsToCollect;	//0x3DC
	u32 blueCoinsToCollect;		//0x3E0
	u32 yellowCoinsCollected;	//0x3E4
	u32 blueCoinsCollected;		//0x3E8

	u32 unk;	//0x3EC

	// New

	u32 purpleCoinsToCollect;	//0x3F0
	u32 purpleCoinsCollected;	//0x3F4
	bool disableSFX;			//0x3F8

	bool checkForCounts();
};

#endif
