#include <common.h>
#include <game.h>
#include <profile.h>
#include <stage.h>

const char* SuperLaunchStarFileList[] = { "launchStar", 0 };

extern char CurrentLevel, CurrentWorld;
extern "C" void ExitStageReal(int scene, int sceneParams, int powerupStoreType, int wipe);

extern "C" u8 CurrentWorldNumForWorldMap;
extern "C" u8 CurrentWorldNumForWorldMapSub;

extern bool shootIntoWorldNext;
extern u8 destinationWorld;
extern u8 destinationSubWorld;

class daEnSuperLaunchStar_c : public dEn_c {
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

	bool active;
	bool isActivatedByEvent;

	int id;

	int superDestinationWorld;
	int superDestinationSubWorld;

	bool afterCheckpoint;

	int timerActivation;

	dAcPy_c* actorsCurrentlyShooting[4];

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
};

const SpriteData SuperLaunchStarSpriteData = { ProfileId::SuperLaunchStar, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile SuperLaunchStarProfile(&daEnSuperLaunchStar_c::build, SpriteId::SuperLaunchStar, &SuperLaunchStarSpriteData, ProfileId::SuperLaunchStar, ProfileId::SuperLaunchStar, "SuperLaunchStar", SuperLaunchStarFileList);

u8 hijackMusicWithSongName(const char* songName, int themeID, bool hasFast, int channelCount, int trackCount, int* wantRealStreamID);

void daEnSuperLaunchStar_c::playerCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	OSReport("Collided SuperLaunchStar\n");
	if (this->active)
	{
		OSReport("Is Active Super Launch Star\n");

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
				return;
			}
		}
	}
}
void daEnSuperLaunchStar_c::yoshiCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	//this->playerCollision(apThis, apOther);
}
bool daEnSuperLaunchStar_c::collisionCat7_GroundPound(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnSuperLaunchStar_c::collisionCat7_GroundPoundYoshi(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnSuperLaunchStar_c::collisionCatD_Drill(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnSuperLaunchStar_c::collisionCatA_PenguinMario(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnSuperLaunchStar_c::collisionCat1_Fireball_E_Explosion(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnSuperLaunchStar_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnSuperLaunchStar_c::collisionCat9_RollingObject(ActivePhysics* apThis, ActivePhysics* apOther) {

	return false;
}
bool daEnSuperLaunchStar_c::collisionCat13_Hammer(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnSuperLaunchStar_c::collisionCat14_YoshiFire(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnSuperLaunchStar_c::collisionCat3_StarPower(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}

dActor_c* daEnSuperLaunchStar_c::build() 
{
	void* buffer = AllocFromGameHeap1(sizeof(daEnSuperLaunchStar_c));
	daEnSuperLaunchStar_c* c = new(buffer) daEnSuperLaunchStar_c;

	return c;
}

extern int getNybbleValue(u32 settings, int fromNybble, int toNybble);

void daEnSuperLaunchStar_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate) 
{
	nw4r::g3d::ResAnmChr anmChr = this->resFileAnim.GetResAnmChr(name);
	this->chrAnimation.bind(&this->bodyModel, anmChr, unk);
	this->bodyModel.bindAnim(&this->chrAnimation, unk2);
	this->chrAnimation.setUpdateRate(rate);
}

int daEnSuperLaunchStar_c::onCreate() 
{
	OSReport("Starting daEnSuperLaunchStar_c::onCreate()\n");
	
	// Model creation	
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	nw4r::g3d::ResMdl mdl;
	
	this->resFile.data = getResource("launchStar", "g3d/launchStar.brres");
	mdl = this->resFile.GetResMdl("LaunchStar");
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

	//TODO Change bitfield data for colissions and co

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
	
	this->bindAnimChr_and_setUpdateRate("idle", 1, 0.0, 1.0);

	this->active = this->settings >> 30 & 1;						
	this->isActivatedByEvent = this->settings >> 29 & 1;				

	this->id = this->settings >> 24 & 0b11111;
	
	this->superDestinationWorld = this->settings >> 8 & 0xF;			
	this->superDestinationSubWorld = this->settings >> 4 & 0xF;

	this->afterCheckpoint = this->settings >> 3 & 1;

	OSReport("ID: %d\n", this->id);

	OSReport("Eight Launch One: %d\n", GameMgrP->eight.checkpointEntranceID);

	timerActivation = 0;

	//checkStarChipReset(afterCheckpoint);

	this->onExecute();
	return true;
}

int daEnSuperLaunchStar_c::onDelete() 
{
	/*for(int i = 0; i < 5; i++)
	{
		alreadyCheckedCollected[this->id][i] = false;
	}*/
	
	return true;
}

int daEnSuperLaunchStar_c::onDraw() 
{
	bodyModel.scheduleForDrawing();
	return true;
}

void daEnSuperLaunchStar_c::updateModelMatrices() 
{
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

int daEnSuperLaunchStar_c::onExecute()
{
	bodyModel._vf1C();
	updateModelMatrices();
	chrAnimation.setUpdateRate(1.0f);
	if (this->chrAnimation.isAnimationDone())
	{
		this->chrAnimation.setCurrentFrame(0.0);
	}
	
	if (this->active)
	{
		for (int i = 0; i < 5; i++)
		{
			if (launchStarChipCollectedAfterFlag[this->id][i] != true && launchStarChipCollectedBeforeFlag[this->id][i] != true)
			{
				this->active = false;
				this->onExecute();
				return true;
			}
		}
		
		dAcPy_c* player = (dAcPy_c*)FindActorByType(PLAYER, 0);
		Remocon* pIn = RemoconMng->controllers[0];
		dPlayerInput_c* pInput = &player->input;
		
		if (timerActivation == 60 && this->actorsCurrentlyShooting[0] != 0)
		{

			for (int i = 0; i < GetActivePlayerCount(); i++)
			{
				player->pos.x = this->actorsCurrentlyShooting[0]->pos.x;
				player->pos.y = this->actorsCurrentlyShooting[0]->pos.y;
				player->pos.z = this->actorsCurrentlyShooting[0]->pos.z;
			
				shootIntoWorldNext = true;
				destinationWorld = this->superDestinationWorld;
				destinationSubWorld = this->superDestinationSubWorld;
				
				playerStatus[i] = 1;
				
				player->cannonShot(1,1,1);

				player = (dAcPy_c*)FindActorByType(PLAYER, (Actor*)player);
			}

			for (int i = 0; i < 4; i++)
			{
				this->actorsCurrentlyShooting[i] = 0;
				playerStatus[i] = 0;
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
				
				playerStatus[i] = 1;

				player = (dAcPy_c*)FindActorByType(PLAYER, (Actor*)player);
			}
		}

		//OSReport("Timer: %d\n", timerActivation);
		return true;
	}
	else
	{
		this->scale = (Vec){ 0, 0, 0 };

		/*OSReport("-------------------------------------------------------------------------------\n");
		OSReport("Eight Super Launch two: %d\n", GameMgrP->eight.checkpointEntranceID);
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
		OSReport("-------------------------------------------------------------------------------\n");*/

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