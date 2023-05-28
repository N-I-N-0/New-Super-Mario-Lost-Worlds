#include "crowdClapper.h"

bool daNiceCoin_c::checkForCounts() {
	u8 targetEventID = (this->spriteFlagNum >> 8) & 0xFF;
	u8 triggeringEventID = this->spriteFlagNum & 0xFF;

	bool conditionsMet = !(!this->yellowCoinsToCollect && !this->blueCoinsToCollect && !this->purpleCoinsToCollect);
	if(this->yellowCoinsToCollect) 
		if(this->yellowCoinsCollected < this->yellowCoinsToCollect)
			conditionsMet = false;

	if(this->blueCoinsToCollect) 
		if(this->blueCoinsCollected < this->blueCoinsToCollect)
			conditionsMet = false;

	if(this->purpleCoinsToCollect) 
		if(this->purpleCoinsCollected < this->purpleCoinsToCollect)
			conditionsMet = false;

	if(triggeringEventID) {
		u64 eventFlag = ((u64)1 << (triggeringEventID - 1));
		if(dFlagMgr_c::instance->flags & eventFlag) {
			conditionsMet = true;
		}
	}

	if(conditionsMet && targetEventID) {
		dFlagMgr_c::instance->flags |= ((u64)1 << (targetEventID - 1));
	}

	return conditionsMet;
}

u32 doshit(u32 whatev) {
	return (whatev >> 4) & 1;
}