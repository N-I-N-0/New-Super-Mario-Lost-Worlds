#include <common.h>
#include <game.h>
#include <profile.h>
#include <actors.h>

const char* StarChipFileList[] = { "starChip", 0 };


class daEnStarChip_c : public dEn_c {
public:
	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	mHeapAllocator_c allocator;
	nw4r::g3d::ResFile resFile;
	m3d::mdl_c bodyModel;


	int id;
	int star_chip_id;

	bool type8fastJump;
	int type8timer;
	int type8timer2;

	bool collected;

	bool afterCheckpoint;

	daEnLaunchStar_c* star;


	static dActor_c* build();

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

const SpriteData StarChipSpriteData = { ProfileId::StarChip, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile StarChipProfile(&daEnStarChip_c::build, SpriteId::StarChip, &StarChipSpriteData, ProfileId::StarChip, ProfileId::StarChip, "StarChip", StarChipFileList);



u8 hijackMusicWithSongName(const char* songName, int themeID, bool hasFast, int channelCount, int trackCount, int* wantRealStreamID);

void daEnStarChip_c::playerCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->type8fastJump = true;
}
void daEnStarChip_c::yoshiCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	playerCollision(apThis, apOther);
}
bool daEnStarChip_c::collisionCat7_GroundPound(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->type8fastJump = true;
	return false;
}
bool daEnStarChip_c::collisionCat7_GroundPoundYoshi(ActivePhysics* apThis, ActivePhysics* apOther) {
	this->type8fastJump = true;
	return false;
}
bool daEnStarChip_c::collisionCatD_Drill(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnStarChip_c::collisionCatA_PenguinMario(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}

bool daEnStarChip_c::collisionCat1_Fireball_E_Explosion(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnStarChip_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics* apThis, ActivePhysics* apOther) {

	return false;
}
bool daEnStarChip_c::collisionCat9_RollingObject(ActivePhysics* apThis, ActivePhysics* apOther) {

	return false;
}
bool daEnStarChip_c::collisionCat13_Hammer(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnStarChip_c::collisionCat14_YoshiFire(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}

bool daEnStarChip_c::collisionCat3_StarPower(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}


dActor_c* daEnStarChip_c::build() {
	void* buffer = AllocFromGameHeap1(sizeof(daEnStarChip_c));
	daEnStarChip_c* c = new(buffer) daEnStarChip_c;

	return c;
}


extern int getNybbleValue(u32 settings, int fromNybble, int toNybble);

int daEnStarChip_c::onCreate() {
	this->deleteForever = true;
	
	bool pullIs = this->settings >> 8 & 0b1;
	
	// Model creation	
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	this->resFile.data = getResource("starChip", "g3d/starChip.brres");
	nw4r::g3d::ResMdl mdl;
	if(!pullIs)
	{
		mdl = this->resFile.GetResMdl("StarChip");
	}
	else
	{
		mdl = this->resFile.GetResMdl("StarChipPull");
	}
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Player(&bodyModel, 0);

	allocator.unlink();

	ActivePhysics::Info HitMeBaby;

	HitMeBaby.xDistToCenter = 0.0;
	HitMeBaby.yDistToCenter = 0.0;

	HitMeBaby.xDistToEdge = 7.5;
	HitMeBaby.yDistToEdge = 7.5;

	HitMeBaby.category1 = 0x3;
	HitMeBaby.category2 = 0x0;
	HitMeBaby.bitfield1 = 0x6F;
	HitMeBaby.bitfield2 = 0xffbafffe;
	HitMeBaby.unkShort1C = 0;
	HitMeBaby.callback = &dEn_c::collisionCallback;

	this->aPhysics.initWithStruct(this, &HitMeBaby);
	this->aPhysics.addToList();

	// Stuff I do understand
	this->scale = (Vec){ 0.2, 0.2, 0.2 };

	this->rot.x = 0;
	this->rot.y = 0;
	this->rot.z = 0;

	this->pos.z = 4000;

	this->star_chip_id = this->settings >> 29 & 0b111;

	this->afterCheckpoint = this->settings >> 28 & 1;

	this->id = this->settings >> 0 & 0b111111;

	OSReport("Star Chip ID: %d\n", this->id);

	star = (daEnLaunchStar_c*)FindActorByType(LaunchStar, 0);

	while (star != 0)
	{
		if (star->id == this->id)
		{
			break;
		}
		star = (daEnLaunchStar_c*)FindActorByType(LaunchStar, (Actor*)star);
	}

	/*for (int i = 0; i < 5; i++)
	{
		OSReport("launchstarchip: %d\n", launchStarChipCollected[this->id][i]);
		if (launchStarChipCollected[this->id][i] == 0);
		{
			this->star_chip_id = i;
			launchStarChipCollected[this->id][i] = 1;
			break;
		}
	}*/

	OSReport("StarCHIP ID: %d\n", this->star_chip_id);

	
	if (launchStarChipCollectedBeforeFlag[this->id][this->star_chip_id] == true || launchStarChipCollectedAfterFlag[this->id][this->star_chip_id] == true)
	{
		collected = true;
	}
	else
	{
		collected = false;
	}

	checkStarChipReset(afterCheckpoint);

	this->onExecute();
	return true;
}


int daEnStarChip_c::onDelete() {
	return true;
}

int daEnStarChip_c::onDraw() {
	bodyModel.scheduleForDrawing();
	return true;
}


void daEnStarChip_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

int daEnStarChip_c::onExecute() {
	bodyModel._vf1C();
	updateModelMatrices();

	if (this->star == 0) {
		star = (daEnLaunchStar_c*)FindActorByType(LaunchStar, 0);
		while (star != 0)
		{
			if (star->id == this->id)
			{
				break;
			}
			star = (daEnLaunchStar_c*)FindActorByType(LaunchStar, (Actor*)star);
		}
	}

	if (!this->type8fastJump) {
		this->rot.y += 0x300;
		if (this->collected)
		{
			this->Delete(1);
		}
	}
	else {
		this->rot.y += 0x2000;
		this->type8timer++;
		if (this->type8timer == 1) {
			this->removeMyActivePhysics();
		}
		if (this->type8timer < 6) {
			this->pos.y += 4;
			this->scale.x -= 0.05;
			this->scale.y -= 0.05;
			this->scale.z -= 0.05;
		}
		if (this->type8timer > 5 && this->type8timer < 11) {
			this->pos.y -= 4;
			this->scale.x -= 0.1;
			this->scale.y -= 0.1;
			this->scale.z -= 0.1;
		}
		if (this->type8timer == 11) {
			if (!midwayFlagActivated) {
				launchStarChipCollectedBeforeFlag[this->id][this->star_chip_id] = true;
			}
			else {
				if (launchStarChipCollectedBeforeFlag[this->id][this->star_chip_id] != true) {
					launchStarChipCollectedAfterFlag[this->id][this->star_chip_id] = true;
				}
			}

			this->collected = true;

			this->Delete(1);
		}
	}

	return true;
}