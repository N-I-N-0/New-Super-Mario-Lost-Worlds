#ifndef __EN_ITEM_H
#define __EN_ITEM_H

#include <game.h>

class daEnItem_c : public dEn_c {

public:
	u8 data[2472];

	void AddPhysics();

	USING_STATES(daEnItem_c);
	REF_NINTENDO_STATE(UpMove);
	REF_NINTENDO_STATE(DownMove);
	REF_NINTENDO_STATE(DropMove);
	REF_NINTENDO_STATE(RedCoinFlash);
	REF_NINTENDO_STATE(Wait);
	REF_NINTENDO_STATE(KinokoMove);
	REF_NINTENDO_STATE(PropellerMove);
	REF_NINTENDO_STATE(EggCreateUp);
	REF_NINTENDO_STATE(ObliquelyMove);
	REF_NINTENDO_STATE(MeterDropMove);
	REF_NINTENDO_STATE(CoinJump);
	REF_NINTENDO_STATE(ControlMove);
	REF_NINTENDO_STATE(FreeDrop);
	REF_NINTENDO_STATE(ObliquelyJumpMove);
	REF_NINTENDO_STATE(BlockAppearMultiJumpUp);
	REF_NINTENDO_STATE(BlockAppearMultiJumpDown);
	REF_NINTENDO_STATE(BigCoinJump);
	REF_NINTENDO_STATE(BigCoinBreak);
	REF_NINTENDO_STATE(AkoyaLinkWait);
	REF_NINTENDO_STATE(EatOut);
};




#endif