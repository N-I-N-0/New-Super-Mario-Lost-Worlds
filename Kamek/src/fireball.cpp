#include "fireball.h"


class daGoldFireBall_c : public daFireBall_Base_c {
public:
	int onCreate(); //0x80040960
	int onExecute();
	int onDelete(); //0x8011AC90

	int playerId;
	int countedTwice;
	int unk;
	int isGold;
	int isDying;
	int radius;
	int waitingTime;
	daEnBlockMain_c* dataBank;
	daEnBlockMain_c* dataBank2;

	daGoldFireBall_c(); //0x8011A5B0

	void vf138(); //0x8011A690, just a blr
	void vf13C(); //0x8011A6A0, just a blr
	void updateSpawnLimits(); //0x8011A6B0
	void setZOrder_maybe(); //0x8011A730
	void calledInOnCreate_b(); //0x8011A7C0
	void calledInOnCreate_c(); //0x8011A8F0
	void initialAdjacentCheck(); //0x8011A940
	void theFuck(); //0x8011A9F0
	void doFireSplash(); //0x8011AAB0

	void collisionCallback(ActivePhysics *one, ActivePhysics *two); //0x8011AB90

	void checkSomethingAboutSlopesAndQuicksand(); //0x8011AED0
	void querySomethingByRCollDetector(); //0x8011AF60
	void checkSomethingElseAboutCSensor(); //0x8011AFC0
	void setSpeed(); //0x8011AFD0
	void CheckFireballLimit(); //0x8011B060

	static daGoldFireBall_c *build(); //0x8011A5B0
	~daGoldFireBall_c(); //0x8011B0B0

	int onCreate_orig();
	int onExecute_orig();
	void beginState_Move_orig();
	void executeState_Move_orig();
	void endState_Move_orig();

	void doGoldTileChange();
	void doTileThing(int tX, int tY);

	USING_STATES(daGoldFireBall_c);
	DECLARE_STATE(Move);
};

daGoldFireBall_c *daGoldFireBall_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daGoldFireBall_c));

	OSReport("sizeof: %d\n", sizeof(daGoldFireBall_c));

	return new(buffer) daGoldFireBall_c;
}

CREATE_STATE(daGoldFireBall_c, Move);

void daGoldFireBall_c::doGoldTileChange() {
	int currentX = this->pos.x;
	int currentY = this->pos.y;

	int moduloX = currentX % 16;
	int moduloY = currentY % 16;

	int tileX = ((currentX - moduloX) / 16);
	int tileY = -((currentY - moduloY) / 16);

	OSReport("Gold hit wall: %d %d\n", tileX, tileY);

	this->isDying = 1;
}

int daGoldFireBall_c::onCreate() {
	this->isGold = 1;
	this->radius = 16;

	int orig_val = this->onCreate_orig();

	OSReport("Hello :p\n");
	OSReport("Sprite: %d %X\n", this->name, this->name);

	return orig_val;
}

int uh5[] = {-64, -48, -32, -16, 0, 16, 32, 48, 64};
int questionIDs[] = {2, 0, 1, 2, 0xA, 0xD, 0x19, 0x15, 0x11, 0x18, 0xE};
int brickIDs[] = {0x20, 0x22, 0x22, 0x20, 0x21, 0x27, 0x2A, 0x39, 0x35, 0x31, 0x2C, 0x2E};

extern "C" u32 getBlockTypeFromTilenum(void* actor, u32 tilenum);
extern u8 returnByteAtPointer(u32 pointer, u32 fromHowMuch);

void daGoldFireBall_c::doTileThing(int tX, int tY) {
	/*u32 tilebehaviour = collMgr.getTileBehaviour1At((float)(tileX + uh5[j]), (float)(tileY + uh5[i]), currentLayerID);
	u32 tilebehaviour2 = collMgr.getTileBehaviour2At((float)(tileX + uh5[j]), (float)(tileY + uh5[i]), currentLayerID);
	OSReport("tilebehaviour=%8X %8X\n", tilebehaviour, tilebehaviour2);
	if(tilebehaviour == 0x00000010) {
		dBgGm_c::instance->placeTile(tileX + uh5[j], -tileY + uh5[i] + 32, currentLayerID, 0);
		Vec coinPos = (Vec){tileX + uh5[j], tileY + uh5[i], this->pos.z};
		CreateActor(401, 0, coinPos, 0, 0);
	}*/
	int ttX = tX / 16;
	int ttY = tY / 16;

	u32 tilebehaviour0 = collMgr.getTileBehaviour1At((float)(tX), (float)(tY), 0);
	//u32 tilebehaviour2 = collMgr.getTileBehaviour2At((float)(tX), (float)(tY), 0);
	int tng = (int)(dBgGm_c::instance->getTileNumAtPosition(tX, -tY, 0, 1));
	int tn = tng & 0xFF;
	int tv = (tng >> 8) / 4;
	//u32 bt = getBlockTypeFromTilenum(this, tn);
	//u64 bh = dBgGm_c::instance->getBehaviourForTileNum(tn);
	//u64 tb = dBgGm_c::instance->getTileBit(tX, -tY, 0);
	//u64 tb2 = dBgGm_c::instance->getTileBit(tX, tY, 0);
	//u32 pt = dBgGm_c::instance->getPointerToTile2(tX, -tY, 0);
	//u32 vv = *((unsigned int*)((u32)pt+0x7));
	//u32 newptr = pt+0x2;
	//u32 moduloptr = newptr % 4;
	//newptr = newptr - moduloptr;
	//u8 vv = returnByteAtPointer(newptr, moduloptr);
	u32 ci = dBgGm_c::instance->getCollisionInfo(tX, -tY, 0);
	u32 ci2 = dBgGm_c::instance->getCollisionInfo(tX, tY, 0);

	if(tn == 0x30 || tn == 0x31) {
		if(tilebehaviour0 == 0x10 || tilebehaviour0 == 0x5) {
			//OSReport("tilebehaviour=%X-%X-%d-%d-%X-%X-%X at %d %d\n", tilebehaviour0, tilebehaviour2, tn, bt, bh, tb, tb2, tX, tY);
			OSReport("tilebehaviour=%X-", tilebehaviour0);
			OSReport("%X-%X-", tn, tv);
			//OSReport("%d-%X-", bt, bh);
			//OSReport("%X-%X-", tb, tb2);
			//OSReport("%X-%X-%X-", pt, newptr, vv);
			OSReport("%X-%X ", ci, ci2);
			OSReport("at %d %d ", tX, tY);
			OSReport("/ %d %d\n", ttX, ttY);

			dBgGm_c::instance->placeTile(tX, -tY, currentLayerID, 0);
			if(tilebehaviour0 & 0x10 && tv == 0) {
				Vec coinPos = (Vec){tX, tY, this->pos.z};
				CreateActor(401, 0, coinPos, 0, 0); //EN_COIN
			}
			else {
				Vec blockPos = (Vec){tX, tY, this->pos.z};
				int idx = ((tilebehaviour0 & 0x10) ? brickIDs[tv] : questionIDs[tv]);
				int blockSettings = 0x01080000 | (this->which_player << 16) | idx;
				//OSReport("Spawning with settings %X (%X)\n", blockSettings, idx);
				CreateActor(59, blockSettings, blockPos, 0, 0); //EN_BLOCK
			}
		}
	}
}

int daGoldFireBall_c::onExecute() {
	int orig_val = this->onExecute_orig();

	if(this->isDying) {
		//OSReport("isDying\n");
		int i = 0;

		int curx = this->pos.x;
		int cury = this->pos.y;

		int tileX = (this->pos.x - (curx % 16));
		int tileY = (this->pos.y - (cury % 16));
		if(this->radius == 16) {
			for(int i = 3; i < 6; i++) {
				for(int j = 3; j < 6; j++) {
					doTileThing(tileX + uh5[j], tileY + uh5[i]);
				}
			}
			Vec effScale = {0.2f, 0.2f, 0.2f};
			SpawnEffect("Wm_ob_powgold_ind", 0, &this->pos, 0, &effScale);
		}
		if(this->radius == 32) {
			for(int i = 1; i < 7; i++) {
				for(int j = 1; j < 7; j++) {
					if((i == 1 || i == 7) && (j == 1 || j == 2 || j == 6 || j == 7)) {
						continue;
					}
					if((i == 1 || i == 2 || i == 6 || i == 7) && (j == 1 || j == 7)) {
						continue;
					}
					doTileThing(tileX + uh5[j], tileY + uh5[i]);
				}
			}
		}
		if(this->radius == 48) {
			for(int i = 0; i < 8; i++) {
				for(int j = 0; j < 8; j++) {
					if((i == 0 || i == 8) && (j == 0 || j == 1 || j == 7 || j == 8)) {
						continue;
					}
					if((i == 0 || i == 1 || i == 7 || i == 8) && (j == 0 || j == 8)) {
						continue;
					}
					doTileThing(tileX + uh5[j], tileY + uh5[i]);
				}
			}
		}

		while(this->radius <= 64 && this->waitingTime == 0) {
			// Set dataBank equal to a pointer towards the first dat bank found, and then every time
			// after that find the one after the one just found (that's what the second parameter does).
			// If the loop finishes:
			if(dataBank != (daEnBlockMain_c*)1) {
				dataBank = (daEnBlockMain_c*) fBase_c::search(EN_OBJ_HATENA_BLOCK, dataBank);
			}
			if(dataBank2 != (daEnBlockMain_c*)1) {
				dataBank2 = (daEnBlockMain_c*) fBase_c::search(EN_OBJ_RENGA_BLOCK, dataBank2);
			}

			if(dataBank == 0) { dataBank = (daEnBlockMain_c*)1; }
			if(dataBank2 == 0) { dataBank2 = (daEnBlockMain_c*)1; }

			if (dataBank == (daEnBlockMain_c*)1 && dataBank2 == (daEnBlockMain_c*)1) {
				this->radius += 16;
				this->waitingTime = 5;
				dataBank = 0;
				dataBank2 = 0;
			}

			if((((dataBank->pos.x-this->pos.x) * (dataBank->pos.x-this->pos.x)) + ((dataBank->pos.y-this->pos.y) * (dataBank->pos.y-this->pos.y))) < (this->radius * this->radius)) //(x-center_x)^2 + (y - center_y)^2 < radius^2
			{
				dataBank->_67F = 1;
			}
			if((((dataBank2->pos.x-this->pos.x) * (dataBank2->pos.x-this->pos.x)) + ((dataBank2->pos.y-this->pos.y) * (dataBank2->pos.y-this->pos.y))) < (this->radius * this->radius)) //(x-center_x)^2 + (y - center_y)^2 < radius^2
			{
				if(dataBank2->settings & 0xF != 0) {
					dataBank2->_67F = 1;
				}
				else {
					//dBgGm_c::instance->placeTile(dataBank2->pos.x - 8, -dataBank2->pos.y - 16.0f, currentLayerID, 0x801E); //Coin Tile
					Vec coinPos = (Vec){dataBank2->pos.x - 8, dataBank2->pos.y + 16, dataBank2->pos.z};
					CreateActor(401, 0, coinPos, 0, 0); //EN_COIN=401=147
					dataBank2->Delete(1);
				}
			}

			//OSReport("i = %d; dataBank = %d; dataBank2 = %d\n", i, dataBank, dataBank2);
			i++;
		}

		if(this->radius > 64 && this->waitingTime < -1) {
			this->Delete(1);
		}
		this->waitingTime--;
	}

	return orig_val;
}



void daGoldFireBall_c::beginState_Move() { 
	this->beginState_Move_orig();
}

void daGoldFireBall_c::endState_Move() { 
	this->executeState_Move_orig();
}

void daGoldFireBall_c::executeState_Move() { 
	this->endState_Move_orig();
}