#include <common.h>
#include <game.h>
#include <profile.h>
#include "baddy.h"

const char* LaunchStarFileList[] = { "launchStar", 0 };

void initStarArraysMidway() {
	for (int i = 0; i < 32; i++) {
		for (int j = 0; j < 5; j++) {
			launchStarChipCollectedAfterFlag[i][j] = false;
		}
		OSReport("Inner Array: %p\n", launchStarChipCollectedBeforeFlag[i]);
	}
}
void initStarArrays() {
	for (int i = 0; i < 32; i++) {
		for (int j = 0; j < 5; j++) {
			launchStarChipCollectedBeforeFlag[i][j] = false;
			launchStarChipCollectedAfterFlag[i][j] = false;
		}
		OSReport("Inner Array: %p\n", launchStarChipCollectedBeforeFlag[i]);
	}
}

class daEnLaunchStar_c : public dEn_c {
public:
	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	mHeapAllocator_c allocator;
	nw4r::g3d::ResFile resFile;
	nw4r::g3d::ResFile resFileAnim;
	m3d::mdl_c bodyModel;

	m3d::anmChr_c chrAnimation;
	 

	bool isSuperLaunchStar;
	bool active;
	bool isActivatedByEvent;

	int id;
	int starRotation;
	int distance;

	int destinationWorld;
	int destinationSubWorld;
	
	int speed;

	float timePlayer[4];

	int collected;

	/*int id;
	int collected;
	bool active;

	int destinationX;
	int destinationY;
	int multiplicator;*/

	dStageActor_c* actorsCurrentlyShooting[4];

	static dActor_c* build();

	void bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate);

	void updateModelMatrices();
	void playerCollision(ActivePhysics* apThis, ActivePhysics* apOther);
	void yoshiCollision(ActivePhysics* apThis, ActivePhysics* apOther);

	bool collisionCat7_GroundPound(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat7_GroundPoundYoshi(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCatD_Drill(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCatA_PenguinMario(ActivePhysics* apThis, ActivePhysics* apOther);

	bool collisionCat1_Fireball_E_Explosion(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat9_RollingObject(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat13_Hammer(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat14_YoshiFire(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat3_StarPower(ActivePhysics* apThis, ActivePhysics* apOther);

	int type;
};

const SpriteData LaunchStarSpriteData = { ProfileId::LaunchStar, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile LaunchStarProfile(&daEnLaunchStar_c::build, SpriteId::LaunchStar, &LaunchStarSpriteData, ProfileId::LaunchStar, ProfileId::LaunchStar, "LaunchStar", LaunchStarFileList);

u8 hijackMusicWithSongName(const char* songName, int themeID, bool hasFast, int channelCount, int trackCount, int* wantRealStreamID);

void daEnLaunchStar_c::playerCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	OSReport("Collided LaunchStar\n");
	if (this->active)
	{
		OSReport("Is Active Launch Star\n");

		//OSReport("getsMoved: %d\n", apOther->owner->getsMoved)

		if (playerStatus[apOther->owner->which_player] != 0)
		{
			return;
		}

		for (int i = 0; i < 4; i++)
		{
			if (this->actorsCurrentlyShooting[i] == 0)
			{
				OSReport("Yes\n");
				this->actorsCurrentlyShooting[i] = apOther->owner;
				playerStatus[apOther->owner->which_player] = 1;

				//OSReport("getsMoved: %d\n", this->actorsCurrentlyShooting[i]->getsMoved);

				return;
			}
		}
	}
}
void daEnLaunchStar_c::yoshiCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	//this->playerCollision(apThis, apOther);
}
bool daEnLaunchStar_c::collisionCat7_GroundPound(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnLaunchStar_c::collisionCat7_GroundPoundYoshi(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnLaunchStar_c::collisionCatD_Drill(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnLaunchStar_c::collisionCatA_PenguinMario(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}

bool daEnLaunchStar_c::collisionCat1_Fireball_E_Explosion(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnLaunchStar_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnLaunchStar_c::collisionCat9_RollingObject(ActivePhysics* apThis, ActivePhysics* apOther) {

	return false;
}
bool daEnLaunchStar_c::collisionCat13_Hammer(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnLaunchStar_c::collisionCat14_YoshiFire(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}

bool daEnLaunchStar_c::collisionCat3_StarPower(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}


dActor_c* daEnLaunchStar_c::build() 
{
	void* buffer = AllocFromGameHeap1(sizeof(daEnLaunchStar_c));
	daEnLaunchStar_c* c = new(buffer) daEnLaunchStar_c;

	return c;
}


extern int getNybbleValue(u32 settings, int fromNybble, int toNybble);

void daEnLaunchStar_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate) 
{
	nw4r::g3d::ResAnmChr anmChr = this->resFileAnim.GetResAnmChr(name);
	this->chrAnimation.bind(&this->bodyModel, anmChr, unk);
	this->bodyModel.bindAnim(&this->chrAnimation, unk2);
	this->chrAnimation.setUpdateRate(rate);
}

int daEnLaunchStar_c::onCreate() 
{
	this->deleteForever = true;

	// Model creation	
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	this->resFile.data = getResource("launchStar", "g3d/launchStar.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("LaunchStar");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Player(&bodyModel, 0);


	this->resFileAnim.data = getResource("launchStar", "g3d/launchStarAnim.brres");
	nw4r::g3d::ResAnmChr anmChr = this->resFileAnim.GetResAnmChr("idle");
	this->chrAnimation.setup(mdl, anmChr, &this->allocator, 0);

	allocator.unlink();

	ActivePhysics::Info HitMeBaby;

	HitMeBaby.xDistToCenter = 0.0;
	HitMeBaby.yDistToCenter = 0.0;

	HitMeBaby.xDistToEdge = 15.0;
	HitMeBaby.yDistToEdge = 15.0;

	HitMeBaby.category1 = 0x3;
	HitMeBaby.category2 = 0x0;
	HitMeBaby.bitfield1 = 0x4F;
	HitMeBaby.bitfield2 = 0x8028E;
	HitMeBaby.unkShort1C = 0;
	HitMeBaby.callback = &dEn_c::collisionCallback;

	this->aPhysics.initWithStruct(this, &HitMeBaby);
	this->aPhysics.addToList();

	this->disableEatIn();

	// Stuff I do understand
	this->scale = (Vec){ 0.2, 0.2, 0.2 };

	this->rot.x = 0;
	this->rot.y = 0;
	this->rot.z = 0;

	this->pos.z = 4000;

	daEnLaunchStar_c::bindAnimChr_and_setUpdateRate("idle", 1, 0.0, 1.0);


	this->isSuperLaunchStar = this->settings >> 31 & 1;						//Bit 17 (0=false, 1=true)
	this->active = this->settings >> 30 & 1;								//Bit 18 (0=false, 1=true)
	this->isActivatedByEvent = this->settings >> 29 & 1;					//Bit 19 (0=false, 1=true)

	this->id = this->settings >> 24 & 0b11111;								//Bit 20-24 (2^5=32 possible launchstars)

	this->starRotation = this->settings >> 16 & 0xFF;						//Bit 25-32 (2^8=256 values)
	
	this->distance = this->settings >> 12 & 0xF;							//Bit 33-36 (2^4=16 values)

	if (this->isSuperLaunchStar) {
		this->destinationWorld = this->settings >> 8 & 0xF;					//Bit 37-40 (2^4=16 main worlds)
		this->destinationSubWorld = this->settings >> 4 & 0xF;				//Bit 41-44 (2^4=16 sub worlds)
	}
	else {
		this->speed = this->settings >> 4 & 0xFF;							//Bit 37-44 (2^8=256 different speed values)
	}

	this->speed *= 2;

	this->collected = 0;

	OSReport("ID: %d\n", this->id);

	OSReport("Eight Launch One: %d\n", GameMgrP->eight.checkpointEntranceID);
	
	


	/*this->id = this->settings >> 3 & 0b111111;
	this->collected = 0;
	this->multiplicator = 1;

	this->destinationX = this->settings >> 9 & 0b111111;
	this->destinationY = this->settings >> 15 & 0b111111;
	this->multiplicator += this->settings >> 21 & 0b11;
	this->active = this->settings >> 1 & 0b1;

	OSReport("Settings: %x\n", this->settings);
	OSReport("Active: %d\n", this->active);
	OSReport("ID: %d\n", this->id);
	OSReport("Destination x: %d\n", this->destinationX);
	OSReport("Destination y: %d\n", this->destinationY);
	OSReport("Multi: %d\n", this->multiplicator);

	this->destinationX *= this->multiplicator;
	this->destinationY *= this->multiplicator;

	OSReport("Destination x: %d\n", this->destinationX);
	OSReport("Destination y: %d\n", this->destinationY);
	
	/*OSReport("\n");
	OSReport("1: %d\n", this->settings >> 0 & 0b1);
	OSReport("2: %d\n", this->settings >> 1 & 0b1);
	OSReport("3: %d\n", this->settings >> 2 & 0b1);
	OSReport("4: %d\n", this->settings >> 3 & 0b1);
	OSReport("5: %d\n", this->settings >> 4 & 0b1);
	OSReport("6: %d\n", this->settings >> 5 & 0b1);
	OSReport("7: %d\n", this->settings >> 6 & 0b1);
	OSReport("8: %d\n", this->settings >> 7 & 0b1);
	OSReport("9: %d\n", this->settings >> 8 & 0b1);
	OSReport("10: %d\n", this->settings >> 9 & 0b1);
	OSReport("11: %d\n", this->settings >> 10 & 0b1);
	OSReport("12: %d\n", this->settings >> 11 & 0b1);
	OSReport("13: %d\n", this->settings >> 12 & 0b1);
	OSReport("14: %d\n", this->settings >> 13 & 0b1);
	OSReport("15: %d\n", this->settings >> 14 & 0b1);
	OSReport("16: %d\n", this->settings >> 15 & 0b1);
	OSReport("1: %d\n", this->settings >> 16 & 0b1);
	OSReport("2: %d\n", this->settings >> 17 & 0b1);
	OSReport("3: %d\n", this->settings >> 18 & 0b1);
	OSReport("4: %d\n", this->settings >> 19 & 0b1);
	OSReport("5: %d\n", this->settings >> 20 & 0b1);
	OSReport("6: %d\n", this->settings >> 21 & 0b1);
	OSReport("7: %d\n", this->settings >> 22 & 0b1);
	OSReport("8: %d\n", this->settings >> 23 & 0b1);
	OSReport("9: %d\n", this->settings >> 24 & 0b1);
	OSReport("10: %d\n", this->settings >> 25 & 0b1);
	OSReport("11: %d\n", this->settings >> 26 & 0b1);
	OSReport("12: %d\n", this->settings >> 27 & 0b1);
	OSReport("13: %d\n", this->settings >> 28 & 0b1);
	OSReport("14: %d\n", this->settings >> 29 & 0b1);
	OSReport("15: %d\n", this->settings >> 30 & 0b1);
	OSReport("16: %d\n", this->settings >> 31 & 0b1);
	OSReport("\n");

	OSReport("0-3: %d\n", this->settings >> 0 & 0b1111);
	OSReport("4-7: %d\n", this->settings >> 4 & 0b1111);*/

	//allLaunchStars[id] = this;

	//OSReport("%d\n", GetActivePlayerCount());

	this->onExecute();
	return true;
}


int daEnLaunchStar_c::onDelete() 
{
	return true;
}

int daEnLaunchStar_c::onDraw() 
{
	bodyModel.scheduleForDrawing();
	return true;
}


void daEnLaunchStar_c::updateModelMatrices() 
{
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

int daEnLaunchStar_c::onExecute() 
{
	if (this->active){
		for (int i = 0; i < 4; i++){
			if (this->actorsCurrentlyShooting[i] != 0){
				//OSReport("getsMoved 3: %d\n", this->actorsCurrentlyShooting[i]->getsMoved);

				if ((this->actorsCurrentlyShooting[i]->collMgr.outputMaybe & (0x15 << 0)) || (this->actorsCurrentlyShooting[i]->collMgr.outputMaybe & (0x15 << 1)) || this->actorsCurrentlyShooting[i]->collMgr.isOnTopOfTile()) {     //if hit a wall
					this->actorsCurrentlyShooting[i]->speed.x = 0;
					this->actorsCurrentlyShooting[i]->speed.y = 0;
					this->actorsCurrentlyShooting[i]->rot.x = 0;
					playerStatus[this->actorsCurrentlyShooting[i]->which_player] = 0;
					this->actorsCurrentlyShooting[i] = 0;
					this->timePlayer[i] = 0;
				}
				else
				{
					if (this->starRotation <= 180)
					{
						//OSReport("X: %f\n", this->pos.x + (this->speed * this->t[i] * cos(((this->starRotation * 2) * M_PI) / 180)));
						//OSReport("Y: %f\n", this->pos.y + (this->speed * this->t[i] * sin(((this->starRotation * 2) * M_PI)) - (this->t[i] * this->t[i])));
						this->actorsCurrentlyShooting[i]->pos.x = this->pos.x + (this->speed * (this->timePlayer[i]/16) * cos(((this->starRotation * 2) * M_PI) / 180));
						//this->cannonBalls[i]->pos.x = this->pos.x;
						//this->cannonBalls[i]->pos.y = this->pos.y;
						//this->cannonBalls[i]->pos.y = this->pos.y + (this->cannonBalls[i]->pos.x * tan(((this->starRotation * 2) * M_PI) / 180)) - (2 / (2 * (this->speed * this->speed) * (cos(((this->starRotation * 2) * M_PI) / 180) * cos(((this->starRotation * 2) * M_PI) / 180))) * (this->cannonBalls[i]->pos.x * this->cannonBalls[i]->pos.x));

						this->actorsCurrentlyShooting[i]->pos.y = (this->pos.y + (this->speed * (this->timePlayer[i] / 16) * sin(((this->starRotation * 2) * M_PI) / 180) - ((this->timePlayer[i] / 16) * (this->timePlayer[i] / 16)))) - (this->actorsCurrentlyShooting[i]->speed.y + this->actorsCurrentlyShooting[i]->y_speed_inc);
						this->timePlayer[i] += 4;
					}
					else
					{
						//this->cannonBalls[i]->pos.x = this->pos.x - (this->speed * this->t[i] * cos(((this->starRotation * 2) * M_PI) / 180));
						//this->actorsCurrentlyShooting[i]->pos.x = this->pos.x;
						//this->cannonBalls[i]->pos.y = this->pos.y;
						//this->actorsCurrentlyShooting[i]->pos.y = this->pos.y + (this->speed * this->timePlayer[i] * sin(((this->starRotation * 2) * M_PI) / 180) - (this->timePlayer[i] * this->timePlayer[i]));
						//this->timePlayer[i] += 1;

						this->actorsCurrentlyShooting[i]->pos.x = -(this->pos.x + (this->speed * (this->timePlayer[i] / 8) * cos(((this->starRotation * 2) * M_PI) / 180)));
						this->actorsCurrentlyShooting[i]->pos.y = (this->pos.y + (this->speed * (this->timePlayer[i] / 8) * sin(((this->starRotation * 2) * M_PI) / 180) - ((this->timePlayer[i] / 8) * (this->timePlayer[i] / 8)))) - (this->actorsCurrentlyShooting[i]->speed.y + this->actorsCurrentlyShooting[i]->y_speed_inc);
						this->timePlayer[i] += 4;
					}

					/*daPlBase_c* player = (daPlBase_c*)this->actorsCurrentlyShooting[i];
					player->setFlag(0x7f);
					player->setFlag(0x11);
					player->setFlag(0xbd);
					player->setFlag(0x88);
					//player->setFlag(0x8e);
					//player->setFlag(0x7a);
					daPlBase_c::setPipeCannonCollisionCategory(player);
					*/
					//OSReport("%f | %f\n", this->cannonBalls[i]->pos.x, this->cannonBalls[i]->pos.y);
				}



				/*this->cannonBalls[i]->pos.x = this->cannonBalls[i]->last_pos.x + this->destinationX / 100 - this->cannonBalls[i]->speed.x; //(this->cannonBalls[i]->speed.x + this->cannonBalls[i]->x_speed_inc);
				this->cannonBalls[i]->pos.y = this->cannonBalls[i]->last_pos.y + this->destinationY / 100 - this->cannonBalls[i]->speed.y; //(this->cannonBalls[i]->speed.y + this->cannonBalls[i]->y_speed_inc);

				this->cannonBalls[i]->rot.x += 0x300;

				this->cannonBallsDone[i] += 1;

				if (this->cannonBallsDone[i] >= 100) {
					this->cannonBalls[i]->rot.x -= 768 * 100;
					this->cannonBalls[i] = 0;
					this->cannonBallsDone[i] = 0;
					playerInWork[this->cannonBalls[i]->which_player] = false;
				}*/
			}
		}

		this->scale = (Vec){ 0.2, 0.2, 0.2 };
		this->rot.y += 0.2;
		this->rot.x -= 0.3;
		this->rot.z += 0.4;
		bodyModel._vf1C();
		updateModelMatrices();

		/*if (this->chrAnimation.isAnimationDone()) {
			this->chrAnimation.setCurrentFrame(0.0);
		}*/
		return true;
	}
	else
	{
		this->scale = (Vec){ 0, 0, 0 };

		OSReport("Collected %d\n", this->collected);
		OSReport("Eight Launch two: %d\n", GameMgrP->eight.checkpointEntranceID);
		OSReport("Collected After 1: %d\n", launchStarChipCollectedAfterFlag[this->id][0]);
		OSReport("Collected After 2: %d\n", launchStarChipCollectedAfterFlag[this->id][1]);
		OSReport("Collected After 3: %d\n", launchStarChipCollectedAfterFlag[this->id][2]);
		OSReport("Collected After 4: %d\n", launchStarChipCollectedAfterFlag[this->id][3]);
		OSReport("Collected After 5: %d\n", launchStarChipCollectedAfterFlag[this->id][4]);
		OSReport(" \n");
		OSReport("Collected Before 1: %d\n", launchStarChipCollectedBeforeFlag[this->id][0]);
		OSReport("Collected Before 2: %d\n", launchStarChipCollectedBeforeFlag[this->id][1]);
		OSReport("Collected Before 3: %d\n", launchStarChipCollectedBeforeFlag[this->id][2]);
		OSReport("Collected Before 4: %d\n", launchStarChipCollectedBeforeFlag[this->id][3]);
		OSReport("Collected Before 5: %d\n", launchStarChipCollectedBeforeFlag[this->id][4]);


		for (int i = 0; i < 5; i++) {
			if (launchStarChipCollectedAfterFlag[this->id][i] != true && launchStarChipCollectedBeforeFlag[this->id][i] != true) {
				return true;
			}
		}

		this->active = true;
		return true;
	}
}