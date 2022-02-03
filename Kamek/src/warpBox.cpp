#include <common.h>
#include <game.h>
#include <profile.h>

const char* WarpBoxFileList[] = { "warpBox", 0 };


class daEnWarpBox_c : public dEn_c {
public:
	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	mHeapAllocator_c allocator;
	nw4r::g3d::ResFile resFile;
	m3d::mdl_c bodyModel;

	daEnWarpBox_c* otherBox;
	int id;

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

const SpriteData WarpBoxSpriteData = { ProfileId::WarpBox, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile WarpBoxProfile(&daEnWarpBox_c::build, SpriteId::WarpBox, &WarpBoxSpriteData, ProfileId::WarpBox, ProfileId::WarpBox, "WarpBox", WarpBoxFileList);



u8 hijackMusicWithSongName(const char* songName, int themeID, bool hasFast, int channelCount, int trackCount, int* wantRealStreamID);

void daEnWarpBox_c::playerCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
}
void daEnWarpBox_c::yoshiCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
}
bool daEnWarpBox_c::collisionCat7_GroundPound(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnWarpBox_c::collisionCat7_GroundPoundYoshi(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnWarpBox_c::collisionCatD_Drill(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnWarpBox_c::collisionCatA_PenguinMario(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}

bool daEnWarpBox_c::collisionCat1_Fireball_E_Explosion(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnWarpBox_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics* apThis, ActivePhysics* apOther) {

	return false;
}
bool daEnWarpBox_c::collisionCat9_RollingObject(ActivePhysics* apThis, ActivePhysics* apOther) {

	return false;
}
bool daEnWarpBox_c::collisionCat13_Hammer(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnWarpBox_c::collisionCat14_YoshiFire(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}

bool daEnWarpBox_c::collisionCat3_StarPower(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}


dActor_c* daEnWarpBox_c::build() {
	void* buffer = AllocFromGameHeap1(sizeof(daEnWarpBox_c));
	daEnWarpBox_c* c = new(buffer) daEnWarpBox_c;

	return c;
}


extern int getNybbleValue(u32 settings, int fromNybble, int toNybble);

int daEnWarpBox_c::onCreate() {
	this->deleteForever = true;

	// Model creation	
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	this->resFile.data = getResource("warpBox", "g3d/warpBox.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("WarpBox");
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

	this->pos.z = -4000;


	this->id = this->settings >> 24 & 0xFF;

	OSReport("Box ID: %d\n", this->id);
	
	this->otherBox = (daEnWarpBox_c*)FindActorByType(WarpBox, 0);

	while (this->otherBox != 0)
	{
		if (this->otherBox->id == this->id && this->otherBox != this) {
			break;
		}
		else {
			this->otherBox = (daEnWarpBox_c*)FindActorByType(WarpBox, (Actor*)this->otherBox);
		}
	}

	OSReport("Other Box: %p\n", this->otherBox);

	//this->onExecute();
	return true;
}


int daEnWarpBox_c::onDelete() {
	return true;
}

int daEnWarpBox_c::onDraw() {
	bodyModel.scheduleForDrawing();
	return true;
}


void daEnWarpBox_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

int daEnWarpBox_c::onExecute() {

	dStageActor_c* current = (dStageActor_c*)FindActorByType(PLAYER, 0);

	while (current != 0)
	{
		//OSReport("Speed: %f\n", current->speed.y);
		//OSReport("Inc Speed: %f\n", current->y_speed_inc);

		//current->y_speed_inc = -0.17;
		//current->speed.y *= -1;
		//current->y_speed_inc *= -1;
		//current->speed.y -= (current->speed.y + current->y_speed_inc) * 2;

		Remocon* pIn = RemoconMng->controllers[current->which_player];

		if (Remocon_GetPressed(pIn) & WPAD_TWO)
		{
			OSReport("2 Pressed\n");
			this->speed.y += 4;
		}

		//OSReport("Tileset 1: %08x \n", current->collMgr.getTileBehaviour1At(current->pos.x, current->pos.y - 32, 0));
		//OSReport("Tileset 2: %08x \n", current->collMgr.getTileBehaviour2At(current->pos.x, current->pos.y - 32, 0));
		//OSReport("Tileset 1: %08x \n", current->collMgr.getTileBehaviour1At(current->pos.x, current->pos.y - 32, 1));
		//OSReport("Tileset 2: %08x \n", current->collMgr.getTileBehaviour2At(current->pos.x, current->pos.y - 32, 1));
		//OSReport("Tileset 1: %08x \n", current->collMgr.getTileBehaviour1At(current->pos.x, current->pos.y - 32, 2));
		//OSReport("Tileset 2: %08x \n", current->collMgr.getTileBehaviour2At(current->pos.x, current->pos.y - 32, 2));
		//OSReport("Pos Y: %f \n", current->pos.y);

		//current->aPhysics.unkArray[0].y += 1;
		//current->aPhysics.update();

		//OSReport("Speed Post: %f\n", current->speed.y);
		//OSReport("Inc Speed Post: %f\n", current->y_speed_inc);
		current = (dStageActor_c*)FindActorByType(PLAYER, (Actor*)current);
	}




	if (this->otherBox == 0) {
		/*daEnWarpBox_c* currentBox;

		currentBox = (daEnWarpBox_c*)FindActorByType(WarpBox, 0);

		while (currentBox != 0)
		{
			if (currentBox->id == this->id && currentBox != this) {
				this->otherBox = currentBox;
			}
			currentBox = (daEnWarpBox_c*)FindActorByType(WarpBox, (Actor*)currentBox);
		}

		OSReport("Other Box: %p\n", this->otherBox);*/

		this->otherBox = (daEnWarpBox_c*)FindActorByType(WarpBox, 0);

		while (this->otherBox != 0)
		{
			if (this->otherBox->id == this->id && this->otherBox != this) {
				break;
			}
			else {
				this->otherBox = (daEnWarpBox_c*)FindActorByType(WarpBox, (Actor*)this->otherBox);
			}
		}

		OSReport("Other Box: %p\n", this->otherBox);
	}
	else {

		bodyModel._vf1C();

		dStageActor_c* current;
		Actor* currentActor;

		currentActor = FindActorByType(PLAYER, 0);
		current = (dStageActor_c*)currentActor;

		while (current != 0)
		{
			if (current->pos.y <= this->pos.y + 7.5 && current->pos.y >= this->pos.y - 7.5 && current->pos.x <= this->pos.x + 7.5 && current->pos.x >= this->pos.x - 7.5) {
				OSReport("Warp Collide\n");

				current->pos.y = otherBox->pos.y;
				current->pos.x = otherBox->pos.x;
				current->last_pos.y = otherBox->pos.y;
				current->last_pos.x = otherBox->pos.x;
			}

			currentActor = FindActorByType(PLAYER, currentActor);
			current = (dStageActor_c*)currentActor;
		}
	}

	return true;
}