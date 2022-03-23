#include <common.h>
#include <game.h>
#include <profile.h>
#include <stage.h>
#include "baddy.h"

const char* LaunchStarFileList[] = { "launchStar", 0 };

extern char CurrentLevel, CurrentWorld;
extern "C" void ExitStageReal(int scene, int sceneParams, int powerupStoreType, int wipe);

extern "C" u8 CurrentWorldNumForWorldMap;
extern "C" u8 CurrentWorldNumForWorldMapSub;

extern bool shootIntoWorldNext;
extern u8 destinationWorld;
extern u8 destinationSubWorld;

/*void initStarArraysMidway() {
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
}*/

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

	/*static daEnLaunchStar_c currentPullstarP1;
	static daEnLaunchStar_c currentPullstarP2;
	static daEnLaunchStar_c currentPullstarP3;
	static daEnLaunchStar_c currentPullstarP4;*/          //When I'm right, I actully don't need those in th end

	//static daEnLaunchStar_c PullManager;
	
	int type;

	u8 pullScale;
	u8 pullSpeed;

	bool lastShaking[4];

	//----------------------------------------

	bool isSuperLaunchStar;
	bool active;
	bool isActivatedByEvent;

	int id;
	int starRotation;
	int distance;

	int destinationWorld;
	int destinationSubWorld;
	
	int speed;

	bool afterCheckpoint;

	int speedx;
	int speedy;

	float timePlayer[4];
	bool shootplayer[4];

	int collected;

	int timerActivation;

	/*int id;
	int collected;
	bool active;

	int destinationX;
	int destinationY;
	int multiplicator;*/

	u8 shakingTimer[4];

	dAcPy_c* actorsCurrentlyShooting[4];
	
	
	
	daEnLaunchStar_c* closestPull[4][8];   //0 = Left; 1 = Right; 2 = Up; 3 = Down
	daEnLaunchStar_c* selectedPullStar[4];
	int pullReleaseTimer[4];
	
	

	static dActor_c* build();

	bool isShaking(u8 id, dPlayerInput_c* pInput);

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

	//int type;
};

const SpriteData LaunchStarSpriteData = { ProfileId::LaunchStar, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile LaunchStarProfile(&daEnLaunchStar_c::build, SpriteId::LaunchStar, &LaunchStarSpriteData, ProfileId::LaunchStar, ProfileId::LaunchStar, "LaunchStar", LaunchStarFileList);

u8 hijackMusicWithSongName(const char* songName, int themeID, bool hasFast, int channelCount, int trackCount, int* wantRealStreamID);

void daEnLaunchStar_c::playerCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	if (type == 0)
	{
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
					this->actorsCurrentlyShooting[i] = (dAcPy_c*)apOther->owner;
					playerStatus[apOther->owner->which_player] = 1;

					//OSReport("getsMoved: %d\n", this->actorsCurrentlyShooting[i]->getsMoved);

					return;
				}
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
	OSReport("Starting daEnLaunchStar_c::onCreate()\n");

	type = this->settings >> 0 & 0b11;

	if (type != 2)
	{
		// Model creation	
		allocator.link(-1, GameHeaps[0], 0, 0x20);

		nw4r::g3d::ResMdl mdl;

		if (type == 1)
		{
			this->resFile.data = getResource("launchStar", "g3d/pullPull.brres");
			mdl = this->resFile.GetResMdl("pullPull");
			bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
			SetupTextures_Player(&bodyModel, 0);

			//TODO annimation of Pullstar not Launchstar

			this->resFileAnim.data = getResource("launchStar", "g3d/launchStarAnim.brres");
			nw4r::g3d::ResAnmChr anmChr = this->resFileAnim.GetResAnmChr("idle");
			this->chrAnimation.setup(mdl, anmChr, &this->allocator, 0);
		}
		else if (type == 0)
		{
			this->resFile.data = getResource("launchStar", "g3d/launchStar.brres");
			mdl = this->resFile.GetResMdl("LaunchStar");
			bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
			SetupTextures_Player(&bodyModel, 0);

			this->resFileAnim.data = getResource("launchStar", "g3d/launchStarAnim.brres");
			nw4r::g3d::ResAnmChr anmChr = this->resFileAnim.GetResAnmChr("idle");
			this->chrAnimation.setup(mdl, anmChr, &this->allocator, 0);
		}

		allocator.unlink();
	}
	else
	{
		OSReport("Is Pull Mngr\n");
		for(int i = 0; i < 4; i = i + 1)
		{
			playerStatus[i] = 0;
			dragged[i] = false;
		}
	}

	if (type != 2)
	{
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

		this->pos.z = -4000;

		if (type == 0)
		{
			this->bindAnimChr_and_setUpdateRate("idle", 1, 0.0, 1.0);
		}


		this->isSuperLaunchStar = this->settings >> 31 & 1;						//Bit 17 (0=false, 1=true)
		this->active = this->settings >> 30 & 1;								//Bit 18 (0=false, 1=true)
		this->isActivatedByEvent = this->settings >> 29 & 1;					//Bit 19 (0=false, 1=true)

		this->id = this->settings >> 24 & 0b11111;								//Bit 20-24 (2^5=32 possible launchstars)

		//this->starRotation = this->settings >> 16 & 0xFF;						//Bit 25-32 (2^8=256 values)

		this->speedx = this->settings >> 18 & 0b111111;							//Bit 33-36 (2^4=16 values)
		this->speedy = this->settings >> 12 & 0b111111;

		this->pullScale = this->settings >> 18 & 0b111111;							//Bit 33-36 (2^4=16 values)
		this->pullSpeed = this->settings >> 12 & 0b111111;

		if (type == 1)
		{
			this->scale = (Vec){ 0.13f,0.13f,0.13f };
		}

		if (this->isSuperLaunchStar) {
			this->destinationWorld = this->settings >> 8 & 0xF;					//Bit 37-40 (2^4=16 main worlds)
			this->destinationSubWorld = this->settings >> 4 & 0xF;				//Bit 41-44 (2^4=16 sub worlds)
		}
		else {
			this->speed = this->settings >> 4 & 0xFF;							//Bit 37-44 (2^8=256 different speed values)
		}

		this->afterCheckpoint = this->settings >> 3 & 1;

		this->speed *= 2;

		this->collected = 0;

		OSReport("ID: %d\n", this->id);

		OSReport("Eight Launch One: %d\n", GameMgrP->eight.checkpointEntranceID);

		timerActivation = 0;



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

		checkStarChipReset(afterCheckpoint);
	}
	else
	{
		OSReport("Initialising PullMngr\n");

		dAcPy_c* player = (dAcPy_c*)FindActorByType(PLAYER, 0);

		for (int i = 0; i < GetActivePlayerCount(); i++)
		{
			lastPos[i].x = player->pos.x;
			lastPos[i].y = player->pos.y;
			lastPos[i].z = player->pos.z;

			player = (dAcPy_c*)FindActorByType(PLAYER, (Actor*)player);
		}

		OSReport("finished Initialising PullMngr\n");
	}

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

bool daEnLaunchStar_c::isShaking(u8 id, dPlayerInput_c* pInput)
{
	/*if (shakingTimer[id] == 0)
	{
		if (pInput->areWeShaking())
		{
			shakingTimer[id] = 1;
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (shakingTimer[id] < 12)                  //if(shakingTimer[id] < 9)
	{
		if (pInput->areWeShaking())
		{
			shakingTimer[id] = 1;
			return true;
		}
		else
		{
			shakingTimer[id] += 1;
			return true;
		}
	}
	else if (shakingTimer[id] >= 12)
	{
		if (pInput->areWeShaking())
		{
			shakingTimer[id] = 1;
			return true;
		}
		else
		{
			shakingTimer[id] = 0;
			return false;
		}
	}*/

	return (pInput->heldButtons & WPAD_B) > 0;
}

int daEnLaunchStar_c::onExecute()
{
	if (type != 2)
	{
		bodyModel._vf1C();
		updateModelMatrices();
		chrAnimation.setUpdateRate(1.0f);
		if (this->chrAnimation.isAnimationDone())
		{
			this->chrAnimation.setCurrentFrame(0.0);
		}

		if (type == 1)
		{
			OSReport("CheckpointActivated: %d\n", chekpointActivated);

			if (this->active)
			{
				/*dAcPy_c* player = (dAcPy_c*)FindActorByType(PLAYER, 0);

				for (int i = 0; i < GetActivePlayerCount(); i++)
				{
					Remocon* pIn = RemoconMng->controllers[i];
					dPlayerInput_c* pInput = &player->input;

					float dx = player->pos.x - this->pos.x;
					float dy = player->pos.y - this->pos.y;

					bool shakingBool = this->isShaking(i, pInput);

					OSReport("dx: %f, dy: %f\n", dx, dy);

					OSReport("Shaking: %d, %d, %d, %d, %d\n", pInput->areWeShaking(), pInput->shaking1, pInput->shaking2, shakingBool, pIn->heldButtons & WPAD_B);

					if ((dx * dx) + (dy * dy) <= ((pullScale / 5) * 64) * ((pullScale / 5) * 64))
					{
						OSReport("Inside! \n");

						if (shakingBool)
						{
							if (lastShaking[i] == false)   //was with angle 0
							{
								pullAngle[i].x = this->pos.x - player->pos.x;
								pullAngle[i].y = this->pos.y - player->pos.y;
								pullAngle[i].z = this->pos.z - player->pos.z;


								pullAngle[i].x /= 10 * pullSpeed;
								pullAngle[i].y /= 10 * pullSpeed;
								pullAngle[i].z /= 10 * pullSpeed;

								this->lastPos[i].x = player->pos.x;
								this->lastPos[i].y = player->pos.y;
								this->lastPos[i].z = player->pos.z;

								//Why is everything below inside this if XD

								player->pos.x = (this->lastPos[i].x + pullAngle[i].x);
								player->pos.y = (this->lastPos[i].y + pullAngle[i].y);
								player->pos.z = (this->lastPos[i].z + pullAngle[i].z);

								if ((this->pos.x > lastPos[i].x&& this->pos.x < player->pos.x) || (this->pos.x < lastPos[i].x && this->pos.x > player->pos.x) || (this->pos.y > lastPos[i].y&& this->pos.y < player->pos.y) || (this->pos.y < lastPos[i].y && this->pos.y > player->pos.y) || (this->pos.z > lastPos[i].z&& this->pos.z < player->pos.z) || (this->pos.z < lastPos[i].z && this->pos.z > player->pos.z))
								{
									pullAngle[i].x = 0;
									pullAngle[i].y = 0;
									pullAngle[i].z = 0;
								}

								if (!(pullAngle[i].x == 0 && pullAngle[i].y == 0))
								{
									this->lastPos[i].x = player->pos.x;
									this->lastPos[i].y = player->pos.y;
									this->lastPos[i].z = player->pos.z;
								}
								else
								{
									player->pos.x = this->pos.x;
									player->pos.y = this->pos.y;
									player->pos.z = this->pos.z;
								}

								/*player->pos.x -= player->speed.x; //+ player->x_speed_inc;
								player->pos.y -= player->speed.y; //+ player->y_speed_inc;
								player->pos.z -= player->speed.z;



								/*player->speed.x = 0;
								player->speed.y = 0;
								player->speed.z = 0;
								player->x_speed_inc = 0;
								player->y_speed_inc = 0;/
							}
							else
							{
								OSReport("Speed before: %f, %f, %f\n", player->speed.x, player->speed.y, player->speed.z);

								/player->speed.x = 0;
								player->speed.y = -player->y_speed_inc;
								player->speed.z = 0;/

								/player->pos.x = player->last_pos.x;
								player->pos.y = player->last_pos.y;
								player->pos.z = player->last_pos.z;/

								OSReport("speeed: %d\n", pullSpeed);

								player->pos.x = this->lastPos[i].x + pullAngle[i].x;
								player->pos.y = this->lastPos[i].y + pullAngle[i].y;
								player->pos.z = this->lastPos[i].z + pullAngle[i].z;

								if ((this->pos.x > lastPos[i].x&& this->pos.x < player->pos.x) || (this->pos.x < lastPos[i].x && this->pos.x > player->pos.x) || (this->pos.y > lastPos[i].y&& this->pos.y < player->pos.y) || (this->pos.y < lastPos[i].y && this->pos.y > player->pos.y) || (this->pos.z > lastPos[i].z&& this->pos.z < player->pos.z) || (this->pos.z < lastPos[i].z && this->pos.z > player->pos.z))
								{
									pullAngle[i].x = 0;
									pullAngle[i].y = 0;
									pullAngle[i].z = 0;
								}

								if (!(pullAngle[i].x == 0 && pullAngle[i].y == 0))
								{
									this->lastPos[i].x = player->pos.x;
									this->lastPos[i].y = player->pos.y;
									this->lastPos[i].z = player->pos.z;
								}
								else
								{
									player->pos.x = this->pos.x;
									player->pos.y = this->pos.y;
									player->pos.z = this->pos.z;
								}

								OSReport("Speed aftzer: %f, %f, %f\n", player->speed.x, player->speed.y, player->speed.z);

								/*player->pos.x -= player->speed.x;
								player->pos.y -= player->speed.y - player->y_speed_inc;
								player->pos.z -= player->speed.z;/
							}
						}
						else if (lastShaking[i] == true)
						{
							//if (pullAngle[i].x != 0 && pullAngle[i].y != 0 && pullAngle[i].z != 0)   //Why is this even here? 
							//{
							player->speed.x = pullAngle[i].x;
							player->speed.y = pullAngle[i].y;
							player->speed.z = pullAngle[i].z;

							pullAngle[i].x = 0;
							pullAngle[i].y = 0;
							pullAngle[i].z = 0;
							//}
						}

						lastShaking[i] = shakingBool;
					}
					else  //einfach vergessen XD uff
					{
						lastShaking[i] = false;

						/*this->lastPos[i].x = player->pos.x;
						this->lastPos[i].y = player->pos.y;
						this->lastPos[i].z = player->pos.z;/
					}

					/*this->lastPos[i].x = player->pos.x;
					this->lastPos[i].y = player->pos.y;       //Why did those above exist when this was a thing  (me afterwards readding them withput noticing this comment)
					this->lastPos[i].z = player->pos.z;/



					player = (dAcPy_c*)FindActorByType(PLAYER, (Actor*)player);
				}*/

				this->scale = (Vec){ 0.13f,0.13f,0.13f };
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


				for (int i = 0; i < 5; i++)
				{
					if (launchStarChipCollectedAfterFlag[this->id][i] != true && launchStarChipCollectedBeforeFlag[this->id][i] != true)
					{
						return true;
					}
				}

				this->active = true;
				return true;
			}
		}
		else
		{
			if (!isSuperLaunchStar)
			{
				OSReport("CheckpointActivated: %d\n", chekpointActivated);

				if (this->active)
				{
					if (timerActivation == 60 && this->actorsCurrentlyShooting[0] != 0)
					{
						dAcPy_c* player = (dAcPy_c*)FindActorByType(PLAYER, 0);

						for (int i = 0; i < GetActivePlayerCount(); i++)
						{
							if (player != this->actorsCurrentlyShooting[0] && player != this->actorsCurrentlyShooting[1] && player != this->actorsCurrentlyShooting[2] && player != this->actorsCurrentlyShooting[3])
							{
								player->pos.x = this->actorsCurrentlyShooting[0]->pos.x;
								player->pos.y = this->actorsCurrentlyShooting[0]->pos.y;
								player->pos.z = this->actorsCurrentlyShooting[0]->pos.z;
							}

							player->pipeCannonShot(0, speedx, speedy);

							player = (dAcPy_c*)FindActorByType(PLAYER, (Actor*)player);
						}

						for (int i = 0; i < 4; i++)
						{
							this->actorsCurrentlyShooting[i] = 0;
						}

						this->timerActivation = 0;
					}
					else if (timerActivation < 60 && this->actorsCurrentlyShooting[0] != 0)
					{
						timerActivation += 1;

						dAcPy_c* player = (dAcPy_c*)FindActorByType(PLAYER, 0);

						for (int i = 0; player != 0; i++)
						{
							player->pos.x = this->pos.x;
							player->pos.y = this->pos.y;
							player->pos.z = this->pos.z;

							player = (dAcPy_c*)FindActorByType(PLAYER, (Actor*)player);
						}
					}

					OSReport("Timer: %d\n", timerActivation);

					/*for (int i = 0; i < 4; i++){
						if (this->actorsCurrentlyShooting[i] != 0){
							if (!shootplayer[i])
							{
								//this->actorsCurrentlyShooting[i]->cannonShot(16, 20, 20);
								this->actorsCurrentlyShooting[i]->pipeCannonShot(0, 8, 8);

								shootplayer[i] = true;
							}

							//OSReport("getsMoved 3: %d\n", this->actorsCurrentlyShooting[i]->getsMoved);

							if ((this->actorsCurrentlyShooting[i]->collMgr.outputMaybe & (0x15 << 0)) || (this->actorsCurrentlyShooting[i]->collMgr.outputMaybe & (0x15 << 1)) || this->actorsCurrentlyShooting[i]->collMgr.isOnTopOfTile()) {     //if hit a wall
								this->actorsCurrentlyShooting[i]->speed.x = 0;
								this->actorsCurrentlyShooting[i]->speed.y = 0;
								this->actorsCurrentlyShooting[i]->rot.x = 0;
								playerStatus[this->actorsCurrentlyShooting[i]->which_player] = 0;
								this->actorsCurrentlyShooting[i] = 0;
								this->timePlayer[i] = 0;
							}
							/*else
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

								//OSReport("%f | %f\n", this->cannonBalls[i]->pos.x, this->cannonBalls[i]->pos.y);
							}*/



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
							//}
						//}

					this->scale = (Vec){ 0.2, 0.2, 0.2 };
					this->rot.y += 0x200;
					this->rot.x -= 0x300;
					this->rot.z += 0x400;
					//bodyModel._vf1C();
					//updateModelMatrices();

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


					for (int i = 0; i < 5; i++)
					{
						if (launchStarChipCollectedAfterFlag[this->id][i] != true && launchStarChipCollectedBeforeFlag[this->id][i] != true)
						{
							return true;
						}
					}

					this->active = true;
					return true;
				}
			}
			else
			{
				if (this->active)
				{
					dAcPy_c* player = (dAcPy_c*)FindActorByType(PLAYER, 0);
					Remocon* pIn = RemoconMng->controllers[0];
					dPlayerInput_c* pInput = &player->input;
					
					
					
					if (this->active)
					{
						if (timerActivation == 60 && this->actorsCurrentlyShooting[0] != 0)
						{

							for (int i = 0; i < GetActivePlayerCount(); i++)
							{
								if (player != this->actorsCurrentlyShooting[0] && player != this->actorsCurrentlyShooting[1] && player != this->actorsCurrentlyShooting[2] && player != this->actorsCurrentlyShooting[3])
								{
									player->pos.x = this->actorsCurrentlyShooting[0]->pos.x;
									player->pos.y = this->actorsCurrentlyShooting[0]->pos.y;
									player->pos.z = this->actorsCurrentlyShooting[0]->pos.z;
								}
	
								shootIntoWorldNext = true;
								destinationWorld = 4;
								destinationSubWorld = 1;
								
								player->cannonShot(1,1,1);

								player = (dAcPy_c*)FindActorByType(PLAYER, (Actor*)player);
							}

							for (int i = 0; i < 4; i++)
							{
								this->actorsCurrentlyShooting[i] = 0;
							}

							this->timerActivation = 0;
						}
						else if (timerActivation < 60 && this->actorsCurrentlyShooting[0] != 0)
						{
							timerActivation += 1;

							dAcPy_c* player = (dAcPy_c*)FindActorByType(PLAYER, 0);

							for (int i = 0; player != 0; i++)
							{
								player->pos.x = this->pos.x;
								player->pos.y = this->pos.y;
								player->pos.z = this->pos.z;

								player = (dAcPy_c*)FindActorByType(PLAYER, (Actor*)player);
							}
						}

						OSReport("Timer: %d\n", timerActivation);
					
					

					/*RESTART_CRSIN_LevelStartStruct.replayType = 0;
					RESTART_CRSIN_LevelStartStruct.replayDuration = 0;

					RESTART_CRSIN_LevelStartStruct.entrance = 0xFF;
					RESTART_CRSIN_LevelStartStruct.area = 0;
					RESTART_CRSIN_LevelStartStruct.world1 = 7;
					RESTART_CRSIN_LevelStartStruct.world2 = 7;
					RESTART_CRSIN_LevelStartStruct.level1 = 1;
					RESTART_CRSIN_LevelStartStruct.level2 = 1;

					//DoSceneChange(RESTART_CRSIN, 0, 0);

					CurrentLevel = 1;

					CurrentWorld = 7;

					CurrentWorldNumForWorldMap = 7;
					CurrentWorldNumForWorldMapSub = 0;

					//ExitStage(WORLD_MAP, 0, 0, 1);

					///DoSceneChange(WM_MAP, 0, 0);

					/*OSReport("%c, %d - %c, %d\n", CurrentLevel, CurrentLevel, CurrentWorld, CurrentWorld);

					CurrentLevel = 1;

					CurrentWorld = 7;

					OSReport("%c, %d - %c, %d\n", CurrentLevel, CurrentLevel, CurrentWorld, CurrentWorld);*/


					//player->cannonShot(1, 1, 1);

					/*if (pInput->areWeShaking())
					{
						for (int i = 0; i < GetActivePlayerCount(); i++)
						{
							pIn = RemoconMng->controllers[i];
							dPlayerInput_c* pInput = &player->input;

							player->cannonShot(1, 1, 1);

							player = (dAcPy_c*)FindActorByType(PLAYER, (Actor*)player);
						}

						OSReport("\n");
						OSReport("\n");

						OSReport("dStage32C_c _124[0]: %d, %x, %p\n", dStage32C_c::instance->_124[0], dStage32C_c::instance->_124[0], dStage32C_c::instance->_124[0]);
						OSReport("dStage32C_c _124[1]: %d, %x, %p\n", dStage32C_c::instance->_124[1], dStage32C_c::instance->_124[1], dStage32C_c::instance->_124[1]);
						OSReport("dStage32C_c _124[2]: %d, %x, %p\n", dStage32C_c::instance->_124[2], dStage32C_c::instance->_124[2], dStage32C_c::instance->_124[2]);
						OSReport("dStage32C_c _124[3]: %d, %x, %p\n", dStage32C_c::instance->_124[3], dStage32C_c::instance->_124[3], dStage32C_c::instance->_124[3]);

						OSReport("dStage32C_c _16C: %d, %x, %p\n", dStage32C_c::instance->_16C, dStage32C_c::instance->_16C, dStage32C_c::instance->_16C);

						OSReport("\n");

						for (int i = 0; i < 20; i++)
						{
							OSReport("StageE4 ___[%d]: %d, %x, %p\n", i, StageE4::instance->___[i], StageE4::instance->___[i], StageE4::instance->___[i]);
						}

						OSReport("\n");

						//StoredShorts dStageActorMgr_c
						//StoredBytes dStageActorMgr_c

						OSReport("dStageActorMgr_c _BC8: %d, %x, %p\n", dStageActorMgr_c::instance->_BC8, dStageActorMgr_c::instance->_BC8, dStageActorMgr_c::instance->_BC8);
						OSReport("dStageActorMgr_c _BCA: bool = %d\n", dStageActorMgr_c::instance->_BCA);
						OSReport("dStageActorMgr_c _BD4: %d, %x, %p\n", dStageActorMgr_c::instance->_BD4, dStageActorMgr_c::instance->_BD4, dStageActorMgr_c::instance->_BD4);
						OSReport("dStageActorMgr_c _BD8: %d, %x, %p\n", dStageActorMgr_c::instance->_BD8, dStageActorMgr_c::instance->_BD8, dStageActorMgr_c::instance->_BD8);

						OSReport("\n");

						OSReport("Stage80 acparamb1: float %f\n", Stage80::instance->acparamb1);
						OSReport("Stage80 acparambptr: pointer %p\n", Stage80::instance->acparambptr);
						OSReport("Stage80 acparamb2: float %f\n", Stage80::instance->acparamb2);
						OSReport("Stage80 acparamb3: float %f\n", Stage80::instance->acparamb3);
						OSReport("Stage80 acparamb4: float %f\n", Stage80::instance->acparamb4);

						OSReport("Stage80 acparam1: float %f\n", Stage80::instance->acparam1);
						OSReport("Stage80 acparamptr: pointer %p\n", Stage80::instance->acparamptr);
						OSReport("Stage80 acparam2: float %f\n", Stage80::instance->acparam2);
						OSReport("Stage80 acparam3: float %f\n", Stage80::instance->acparam3);
						OSReport("Stage80 acparam4: float %f\n", Stage80::instance->acparam4);

						OSReport("Stage80 someFlag: %d, %x\n", Stage80::instance->someFlag, Stage80::instance->someFlag);

						OSReport("Stage80 _34: %d, %x, %p\n", Stage80::instance->_34, Stage80::instance->_34, Stage80::instance->_34);

						OSReport("Stage80 _38: %d, %x, %p\n", Stage80::instance->_38, Stage80::instance->_38, Stage80::instance->_38);
						OSReport("Stage80 _39: %d, %x, %p\n", Stage80::instance->_39, Stage80::instance->_39, Stage80::instance->_39);

						OSReport("\n");

						OSReport("StageC4 stageNum: int %d\n", StageC4::instance->stageNum);

						OSReport("StageC4 _10: %d, %x, %p\n", StageC4::instance->_10, StageC4::instance->_10, StageC4::instance->_10);
						OSReport("StageC4 _14: %d, %x, %p\n", StageC4::instance->_14, StageC4::instance->_14, StageC4::instance->_14);

						OSReport("StageC4 flags: int %d, %x, %p\n", StageC4::instance->flags, StageC4::instance->flags, StageC4::instance->flags);
						OSReport("StageC4 _19: int %d, %x, %p\n", StageC4::instance->_19, StageC4::instance->_19, StageC4::instance->_19);
						OSReport("StageC4 _1A: int %d, %x, %p\n", StageC4::instance->_1A, StageC4::instance->_1A, StageC4::instance->_1A);
						OSReport("StageC4 _1C: int %d, %x, %p\n", StageC4::instance->_1C, StageC4::instance->_1C, StageC4::instance->_1C);
						OSReport("StageC4 _1D: int %d, %x, %p\n", StageC4::instance->_1D, StageC4::instance->_1D, StageC4::instance->_1D);

						OSReport("\n");
						OSReport("\n");*/
					}
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


					for (int i = 0; i < 5; i++)
					{
						if (launchStarChipCollectedAfterFlag[this->id][i] != true && launchStarChipCollectedBeforeFlag[this->id][i] != true)
						{
							return true;
						}
					}

					this->active = true;
					return true;
				}
			}
		}
	}
	else
	{
		daPlBase_c* player = (daPlBase_c*)FindActorByType(PLAYER, 0);
		int currentDirection = -1;
		
		this->pos.x = player->pos.x;
		this->pos.y = player->pos.y;
		
		for (int i = 0; i < GetActivePlayerCount(); i++)
		{
			daEnLaunchStar_c* closestSearch = (daEnLaunchStar_c * )FindActorByType(LaunchStar, 0);
			
			if (GetRemoconMng()->controllers[i]->nowPressed & WPAD_B)
			{
				while (closestSearch != 0)
				{
					OSReport("Start search\n");

					if (closestSearch->type == 1)
					{
						if(selectedPullStar[i] != closestSearch)
						{
							float dx = closestSearch->pos.x - player->pos.x;
							float dy = (closestSearch->pos.y - player->pos.y);                           //(player->aPhysics.info.yDistToEdge + player->aPhysics.info.yDistToCenter)) - player->pos.y;
							float r = ((float)closestSearch->pullScale / 5.0f) * 512.0f;
							
							if(sqrtf((dx * dx) + (dy * dy)) > 1  && (dx * dx) + (dy * dy) <= (r * r) && closestSearch->active)
							{
								OSReport("Type is 1\n");
								
								OSReport("for player: %d\n", i);
								
								if(dx * tan(112.5 * M_PI / 180) < dy && dy >= dx * tan(67.5 * M_PI / 180))
								{
									currentDirection = 0;
								}
								else if(dx * tan(67.5 * M_PI / 180) > dy && dy >= dx * tan(22.5 * M_PI / 180))
								{
									currentDirection = 1;
								}
								else if(dx * tan(22.5 * M_PI / 180) > dy && dy >= dx * tan(-22.5 * M_PI / 180))
								{
									currentDirection = 2;
								}
								else if(dx * tan(-22.5 * M_PI / 180) > dy && dy >= dx * tan(-67.5 * M_PI / 180))
								{
									currentDirection = 3;
								}
								else if(dx * tan(-67.5 * M_PI / 180) > dy && dy <= dx * tan(-112.5 * M_PI / 180))
								{
									currentDirection = 4;
								}
								else if(dx * tan(-112.5 * M_PI / 180) < dy && dy <= dx * tan(-157.5 * M_PI / 180))
								{
									currentDirection = 5;
								}
								else if(dx * tan(-157.5 * M_PI / 180) < dy && dy <= dx * tan(-202.5 * M_PI / 180))
								{
									currentDirection = 6;
								}
								else if(dx * tan(-202.5 * M_PI / 180) < dy && dy <= dx * tan(-247.5 * M_PI / 180))
								{
									currentDirection = 7;
								}
								else
								{
									currentDirection = -1;
								}
								
								if(currentDirection >= 0)
								{
									if (closestPull[i][currentDirection] != 0)
									{
										OSReport("closest not null\n");

										float dxc = closestPull[i][currentDirection]->pos.x - player->pos.x;
										float dyc = (closestPull[i][currentDirection]->pos.y - player->pos.y);                              //- (player->aPhysics.info.yDistToEdge + player->aPhysics.info.yDistToCenter)) - player->pos.y;

										if (((dx * dx) + (dy * dy)) < ((dxc * dxc) + (dyc * dyc)))
										{
											closestPull[i][currentDirection] = closestSearch;
										}
									}
									else
									{
										OSReport("Closest null\n");

										closestPull[i][currentDirection] = closestSearch;
									}
								}
								currentDirection = -1;
							}
						}
					}

					OSReport("searching for next Launch star Sprite\n");

					closestSearch = (daEnLaunchStar_c*)FindActorByType(LaunchStar, (Actor*)closestSearch);
					currentDirection = -1;

					OSReport("Found Launch star sprite\n");
				}
				
				for(int c = 0; c < 8; c++)
				{
					if(closestPull[i][c] == 0)
					{
						closestPull[i][c] == selectedPullStar[i];
					}
				}
				
				if(GetRemoconMng()->controllers[i]->heldButtons & WPAD_UP)
				{
					if(GetRemoconMng()->controllers[i]->heldButtons & WPAD_LEFT)
					{
						selectedPullStar[i] = closestPull[i][7];
					}
					else if(GetRemoconMng()->controllers[i]->heldButtons & WPAD_RIGHT)
					{
						selectedPullStar[i] = closestPull[i][1];
					}
					else
					{
						selectedPullStar[i] = closestPull[i][0];
					}
				}
				else if(GetRemoconMng()->controllers[i]->heldButtons & WPAD_RIGHT)
				{
					if(GetRemoconMng()->controllers[i]->heldButtons & WPAD_DOWN)
					{
						selectedPullStar[i] = closestPull[i][3];
					}
					else
					{
						selectedPullStar[i] = closestPull[i][2];
					}
				}
				else if(GetRemoconMng()->controllers[i]->heldButtons & WPAD_DOWN)
				{
					if(GetRemoconMng()->controllers[i]->heldButtons & WPAD_LEFT)
					{
						selectedPullStar[i] = closestPull[i][5];
					}
					else
					{
						selectedPullStar[i] = closestPull[i][4];
					}
				}
				else if(GetRemoconMng()->controllers[i]->heldButtons & WPAD_LEFT)
				{
					selectedPullStar[i] = closestPull[i][6];
				}
				else
				{
					for(int c = 0; c < 8; c++)
					{
						if(closestPull[i][c] != 0)
						{
							if (selectedPullStar[i] != 0)
							{
								//TODO Why the hell didn't I use the corrected y here?
								if((((selectedPullStar[i]->pos.x - player->pos.x) * (selectedPullStar[i]->pos.x - player->pos.x)) + ((selectedPullStar[i]->pos.y - player->pos.y) * (selectedPullStar[i]->pos.y - player->pos.y))) > (((closestPull[i][c]->pos.x - player->pos.x) * (closestPull[i][c]->pos.x - player->pos.x)) + ((closestPull[i][c]->pos.y - player->pos.y) * (closestPull[i][c]->pos.y - player->pos.y))))
								{
									selectedPullStar[i] = closestPull[i][c];
								}
							}
							else
							{
								selectedPullStar[i] = closestPull[i][c];
							}
						}
					}
				}
				
				for(int c = 0; c < 8; c++)
				{
					closestPull[i][c] = 0;
					closestSearch = 0;
				}
			}
			else if(GetRemoconMng()->controllers[i]->heldButtons & WPAD_B)
			{
				if (selectedPullStar[i] != 0)
				{
					OSReport("For LOOP\n");

					OSReport("Selected is active?: %d\n", selectedPullStar[i]->active);

					OSReport("selected is active\n");

					float selectedYPosCorrected = (selectedPullStar[i]->pos.y - (player->aPhysics.info.yDistToEdge + player->aPhysics.info.yDistToCenter));

					float dx = player->pos.x - selectedPullStar[i]->pos.x;
					float dy = player->pos.y - selectedPullStar[i]->pos.y;
					float r = ((float)selectedPullStar[i]->pullScale / 5.0f) * 512.0f;

					//OSReport("ppos: %f|%f ; cpos: %f|%f ; d: %f|%f ; i: %d ; pullScale: %d ; d^2: %f|%f ; r: %f ; r^2: %f\n", player->pos.x, player->pos.y, closest[i]->pos.x, closestYPosCorrected, dx, dy, i, closest[i]->pullScale, (dx * dx), (dy * dy), r, r * r);

					//player->justFaceSpecificDirection(0);
					
					//OSReport("INSIDE\n");

					//OSReport("pullAngle: %f | %f | %f\n", pullAngle[i].x, pullAngle[i].y, pullAngle[i].z);

					pullAngle[i].x = selectedPullStar[i]->pos.x - player->pos.x;
					pullAngle[i].y = selectedPullStar[i]->pos.y - player->pos.y;

					pullAngle[i].x /= 10 * selectedPullStar[i]->pullSpeed;
					pullAngle[i].y /= 10 * selectedPullStar[i]->pullSpeed;

					//player->speed.x = 0;
					//player->speed.y = 0;
					
					if(!player->collMgr.calculateAdjacentCollision())
					{
						player->pos.x = (lastPos[i].x + pullAngle[i].x);
					}
					else
					{
						player->pos.x = (lastPos[i].x);
					}
					
					if(pullAngle[i].y < 0)
					{
						if(!player->collMgr.calculateBelowCollision())
						{
							player->pos.y = (lastPos[i].y + pullAngle[i].y);
						}
						else
						{
							player->pos.y = (lastPos[i].y);
						}
					}
					else if(pullAngle[i].y > 0)
					{
						if(!player->collMgr.calculateAboveCollision(player->collMgr.outputMaybe))
						{
							player->pos.y = (lastPos[i].y + pullAngle[i].y);
						}
						else
						{
							player->pos.y = (lastPos[i].y);
						}
					}
					else
					{
						player->pos.y = (lastPos[i].y + pullAngle[i].y);
					}

					dragged[i] = true;

					lastPos[i].x = player->pos.x;
					lastPos[i].y = player->pos.y;				//if not here too the player would be jump over cause the continue
					lastPos[i].z = player->pos.z;

					//pInput->deactivate();

					player->useDemoControl();
					
					float speeed = pullAngle[i].x;
					
					player->moveInDirection(&(selectedPullStar[i]->pos.x), &pullAngle[i].x);            //is just here for setting the x speed
					
					player = (daPlBase_c*)FindActorByType(PLAYER, (Actor*)player);

					continue;
				}
			}
			if (dragged[i])
			{
				player->disableDemoControl();
				//pullReleaseTimer[i] = 1;
				//selectedPullStar[i] = 0; 
			}
			
			/*if(pullReleaseTimer[i] > 0)
			{
				if(pullReleaseTimer[i] >= 13)
				{
					selectedPullStar[i] = 0;
					
					pullReleaseTimer[i] = 0;
				}
				else
				{
					pullReleaseTimer[i]++;
				}
			}*/

			dragged[i] = false;

			lastPos[i].x = player->pos.x;
			lastPos[i].y = player->pos.y;
			lastPos[i].z = player->pos.z;
			
			player = (daPlBase_c*)FindActorByType(PLAYER, (Actor*)player);
		}
		
		
		/*daEnLaunchStar_c* closestSearch = (daEnLaunchStar_c * )FindActorByType(LaunchStar, 0);
		daEnLaunchStar_c* closest[4] = { 0,0,0,0 };

		OSReport("Start closest\n");

		while (closestSearch != 0)
		{
			OSReport("Start search\n");

			if (closestSearch->type == 1)
			{
				OSReport("Type is 1\n");

				dAcPy_c* player = (dAcPy_c*)FindActorByType(PLAYER, 0);

				this->pos = (Vec){player->pos.x ,player->pos.y,player->pos.z };

				OSReport("found first player\n");

				for (int i = 0; i < GetActivePlayerCount(); i++)
				{
					OSReport("for player: %d\n", i);

					if (closest[i] != 0)
					{
						OSReport("closest not null\n");
						
						float dx = player->pos.x - closestSearch->pos.x;
						float dy = player->pos.y - (closestSearch->pos.y - (player->aPhysics.info.yDistToEdge + player->aPhysics.info.yDistToCenter));

						float dxc = player->pos.x - closest[i]->pos.x;
						float dyc = player->pos.y - (closest[i]->pos.y - (player->aPhysics.info.yDistToEdge + player->aPhysics.info.yDistToCenter));

						if (((dx * dx) + (dy * dy)) > ((dxc * dxc) + (dyc * dyc)))
						{
							closest[i] = closestSearch;
						}

						player = (dAcPy_c*)FindActorByType(PLAYER, (Actor*)player);
					}
					else
					{
						OSReport("Closest null\n");

						closest[i] = closestSearch;
					}
				}
			}

			OSReport("searching for next Launch star Sprite\n");

			closestSearch = (daEnLaunchStar_c*)FindActorByType(LaunchStar, (Actor*)closestSearch);

			OSReport("Found Launch star sprite\n");
		}
		
		daEnLaunchStar_c* closestLeftSearch = (daEnLaunchStar_c * )FindActorByType(LaunchStar, 0);
		daEnLaunchStar_c* closestLeft[4] = { 0,0,0,0 };

		OSReport("Start closestLeft\n");

		while (closestLeftSearch != 0)
		{
			OSReport("Start search\n");

			if (closestLeftSearch->type == 1 && closestLeftSearch->pos.x)
			{
				OSReport("Type is 1\n");

				for (int i = 0; i < GetActivePlayerCount(); i++)
				{
					OSReport("for player: %d\n", i);

					if (closestLeft[i] != 0)
					{
						OSReport("closestLeft not null\n");
						
						float dx = player->pos.x - closestLeftSearch->pos.x;
						float dy = player->pos.y - (closestLeftSearch->pos.y - (player->aPhysics.info.yDistToEdge + player->aPhysics.info.yDistToCenter));

						float dxc = player->pos.x - closestLeft[i]->pos.x;
						float dyc = player->pos.y - (closestLeft[i]->pos.y - (player->aPhysics.info.yDistToEdge + player->aPhysics.info.yDistToCenter));

						if (((dx * dx) + (dy * dy)) > ((dxc * dxc) + (dyc * dyc)))
						{
							closestLeft[i] = closestLeftSearch;
						}

						player = (dAcPy_c*)FindActorByType(PLAYER, (Actor*)player);
					}
					else
					{
						OSReport("closestLeft null\n");

						closestLeft[i] = closestLeftSearch;
					}
				}
			}

			OSReport("searching for next Launch star Sprite\n");

			closestLeftSearch = (daEnLaunchStar_c*)FindActorByType(LaunchStar, (Actor*)closestLeftSearch);

			OSReport("Found Launch star sprite\n");
		}*/
		

		//player = (daPlBase_c*)FindActorByType(PLAYER, 0);

		//OSReport("starting moving\n");

		//for (int i = 0; i < GetActivePlayerCount(); i++)
		//{
			/*OSReport("pfi: %x\n", player->input.permanentForcedButtons);
			if (player->input.permanentForcedButtons != 0x0008)
			{
				//player->input.setPermanentForcedButtons(0x0008);
				player->input.permanentForcedButtons = 0x0008;
				OSReport("pfiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii: %x\n", player->input.permanentForcedButtons);
			}*/
		//}
	}
}