#include <common.h>
#include <game.h>
#include <profile.h>

extern "C" dCourse_c::rail_s * GetRail(int id);

const char* BoshiFileList[] = { "boshi", 0 };


class daEnBoshi_c : public dEn_c {
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

	//1,3,4 may be unecesary
	u8 moveDirection;
	float aTestFloat;
	s16 leftRotPtr;
	s16 rightRotPtr;

	dCourse_c* course;
	dCourse_c::rail_s* rail;
	dCourse_c::railNode_s* currentNode;
	dCourse_c::railNode_s* nextNode;
	int currentNodeNum;
	int steps;
	int speed;
	float moveX, moveY;

	bool done;

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

	USING_STATES(daEnBoshi_c);

	DECLARE_STATE(Wait);
	DECLARE_STATE(Run);
};


CREATE_STATE(daEnBoshi_c, Wait);
CREATE_STATE(daEnBoshi_c, Run);


void daEnBoshi_c::beginState_Wait() { OSReport("HI 0.7 \n"); bindAnimChr_and_setUpdateRate("YB_Rwait", 1, 0.0, 1.0); }
void daEnBoshi_c::executeState_Wait() {
	if (!done)
	{
		dStageActor_c* player = (dStageActor_c*)FindActorByType(PLAYER, 0);

		OSReport("HI 1\n");

		if (player->pos.x >= this->pos.x - 64)
		{
			OSReport("HI 2\n");
			doStateChange(&StateID_Run);
		}
	}
	else
	{
		dStageActor_c* player = (dStageActor_c*)FindActorByType(PLAYER, 0);

		if (player->pos.x < this->pos.x && player->pos.x > this->pos.x - 64)
		{
			//boss::OutroSetup(0);
		}
	}
}
void daEnBoshi_c::endState_Wait() { OSReport("HI 2.3\n"); }


void daEnBoshi_c::beginState_Run() 
{
	OSReport("HI 2.7\n"); 

	switch ((int)currentNode->speed)
	{
	case 0:
		bindAnimChr_and_setUpdateRate("YB_Rwait", 1, 0.0, 1.5);
		break;
	case 1:
		bindAnimChr_and_setUpdateRate("YB_Rrun", 1, 0.0, 1.5);
		break;
	case 2:
		bindAnimChr_and_setUpdateRate("YB_Rb_dash", 1, 0.0, 1.5);
		break;
	case 3:
		bindAnimChr_and_setUpdateRate("YB_Rb_dash2", 1, 0.0, 1.5);
		break;
	}
}
void daEnBoshi_c::executeState_Run() {
	OSReport("HI 3\n");

	if (!done)
	{
		float dx = nextNode->xPos - currentNode->xPos;
		float dy = (-nextNode->yPos) - (-currentNode->yPos);

		float ux = (dx / sqrtf((dx * dx) + (dy * dy)));
		float uy = (dy / sqrtf((dx * dx) + (dy * dy)));

		this->pos.y += uy * speed;
		this->pos.x += ux * speed;

		if (dx <= 0)
		{
			this->rot.y = 0x4000;
			this->rot.y *= -1;
		}
		else
		{
			this->rot.y = 0x4000;
		}

		OSReport("x: %f; y: %f\n", this->pos.x, this->pos.y);
		OSReport("nx: %f; ny: %f\n", currentNode->xPos, -currentNode->yPos);
		OSReport("dx: %f; dy: %f\n", dx, dy);
		OSReport("ux: %f; uy: %f\n", ux, uy);

		if (this->pos.x > dx + currentNode->xPos - 2 && this->pos.x < dx + currentNode->xPos + 2 && this->pos.y > dy - currentNode->yPos - 2 && this->pos.y < dy - currentNode->yPos + 2)
		{
			done = true;
		}
	}
	else
	{
		currentNodeNum++;
		currentNode = nextNode;
		nextNode = &course->railNode[rail->startNode + 1 + currentNodeNum];
		this->pos.x = currentNode->xPos;
		this->pos.y = -currentNode->yPos;

		if (rail->nodeCount == currentNodeNum + 1)
		{
			doStateChange(&StateID_Wait);
			this->rot.y = 0x4000;
			this->rot.y *= -1;
		}
		else
		{
			done = false;
		}

		switch ((int)currentNode->speed)
		{
		case 0:
			bindAnimChr_and_setUpdateRate("YB_Rwait", 1, 0.0, 1.5);
			break;
		case 1:
			bindAnimChr_and_setUpdateRate("YB_Rrun", 1, 0.0, 1.5);
			break;
		case 2:
			bindAnimChr_and_setUpdateRate("YB_Rb_dash", 1, 0.0, 1.5);
			break;
		case 3:
			bindAnimChr_and_setUpdateRate("YB_Rb_dash2", 1, 0.0, 1.5);
			break;
		}

		dStageActor_c* spawner = CreateActor(51, 0, this->pos, 0, 0);
	}

	/*if (steps) {
		this->pos.x += moveX;
		this->pos.y += moveY;
		this->steps--;
		OSReport("HI 4\n");
	}
	else {
		OSReport("HI 5\n");
		float remainX = (nextNode->xPos - this->pos.x);
		float remainY = (nextNode->yPos - this->pos.y);
		float nextL = this->speed - sqrtf((remainX * remainX) + (remainY * remainY));
		currentNodeNum++;
		currentNode = nextNode;
		nextNode = &course->railNode[rail->startNode + 1 + currentNodeNum];
		float dx = (nextNode->xPos - currentNode->xPos);
		float dy = (nextNode->yPos - currentNode->yPos);
		float distance = sqrtf((dx * dx) + (dy * dy));
		this->pos.x = currentNode->xPos + (nextL / distance * dx);
		this->pos.y = currentNode->yPos - (nextL / distance * dy);
		moveX = this->speed / distance * dx;
		moveY = -(this->speed / distance * dy);
		dx = (nextNode->xPos - this->pos.x);
		dy = (-nextNode->yPos - this->pos.y);
		distance = sqrtf((dx * dx) + (dy * dy));
		this->steps = (int)floor(distance / speed);
	}*/
}
void daEnBoshi_c::endState_Run() { OSReport("HI 5.3\n"); }


const SpriteData BoshiSpriteData = { ProfileId::Boshi, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile BoshiProfile(&daEnBoshi_c::build, SpriteId::Boshi, &BoshiSpriteData, ProfileId::Boshi, ProfileId::Boshi, "Boshi", BoshiFileList);



u8 hijackMusicWithSongName(const char* songName, int themeID, bool hasFast, int channelCount, int trackCount, int* wantRealStreamID);

void daEnBoshi_c::playerCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
}
void daEnBoshi_c::yoshiCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
}
bool daEnBoshi_c::collisionCat7_GroundPound(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnBoshi_c::collisionCat7_GroundPoundYoshi(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnBoshi_c::collisionCatD_Drill(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnBoshi_c::collisionCatA_PenguinMario(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}

bool daEnBoshi_c::collisionCat1_Fireball_E_Explosion(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnBoshi_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics* apThis, ActivePhysics* apOther) {

	return false;
}
bool daEnBoshi_c::collisionCat9_RollingObject(ActivePhysics* apThis, ActivePhysics* apOther) {

	return false;
}
bool daEnBoshi_c::collisionCat13_Hammer(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnBoshi_c::collisionCat14_YoshiFire(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}

bool daEnBoshi_c::collisionCat3_StarPower(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}


dActor_c* daEnBoshi_c::build() {
	void* buffer = AllocFromGameHeap1(sizeof(daEnBoshi_c));
	daEnBoshi_c* c = new(buffer) daEnBoshi_c;

	return c;
}


extern int getNybbleValue(u32 settings, int fromNybble, int toNybble);

void daEnBoshi_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate)
{
	nw4r::g3d::ResAnmChr anmChr = this->resFileAnim.GetResAnmChr(name);
	this->chrAnimation.bind(&this->bodyModel, anmChr, unk);
	this->bodyModel.bindAnim(&this->chrAnimation, unk2);
	this->chrAnimation.setUpdateRate(rate);
}

int daEnBoshi_c::onCreate() {
	this->deleteForever = true;

	// Model creation	
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	this->resFile.data = getResource("Yoshi", "g3d/model.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("YB_model");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Player(&bodyModel, 0);


	this->resFileAnim.data = getResource("Y_rcha", "g3d/model.brres");
	nw4r::g3d::ResAnmChr anmChr = this->resFileAnim.GetResAnmChr("YB_Rwait");
	this->chrAnimation.setup(mdl, anmChr, &this->allocator, 0);

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


	this->disableEatIn();

	OSReport("HI 0\n");


	this->aTestFloat = 0.25f;

	done = false;


	speed = this->settings >> 16 & 0b1111;						    //Bit 29-32
	//speed += 1;
	//speed *= 2;
	//speed = 8;
	currentNodeNum = this->settings >> 8 & 0b11111111;						//Bit 33-40
	int pathID = this->settings >> 0 & 0b11111111;                               //Bit 41-48
	OSReport("speed: %d, currentNodeNum: %d, pathID: %d\n", speed, currentNodeNum, pathID);
	if (pathID) {
		rail = GetRail(pathID);
		course = dCourseFull_c::instance->get(GetAreaNum());
		currentNode = &course->railNode[rail->startNode + currentNodeNum];
		nextNode = &course->railNode[rail->startNode + 1 + currentNodeNum];
		OSReport("pathID: %d\n", pathID);
		OSReport("startNode: %d\n", currentNodeNum);
		OSReport("N1.x: %d, N1.y: %d, N2.x: %d, N2.y: %d\n", currentNode->xPos, currentNode->yPos, nextNode->xPos, nextNode->yPos);
		//OSReport("Ballon: %f, %f\n", this->pos.x, this->pos.y);
		this->pos.x = currentNode->xPos;
		this->pos.y = -currentNode->yPos;
		//OSReport("Ballon: %f, %f\n", this->pos.x, this->pos.y);
		OSReport("HI 0.1\n");
	}
	else {
		OSReport("HI 0.2\n");
	}



	// Stuff I do understand
	this->scale = (Vec){ 1, 1, 1 };

	this->rot.x = 0;
	this->rot.y = 0x4000;
	this->rot.y *= -1;
	this->rot.z = 0;

	this->pos.z = 4000;

	bindAnimChr_and_setUpdateRate("YB_Rwait", 1, 0.0, 1.0);

	OSReport("HI 0.3\n");

	doStateChange(&StateID_Wait);

	OSReport("HI 0.4\n");

	this->onExecute();
	return true;
}


int daEnBoshi_c::onDelete() {
	return true;
}

int daEnBoshi_c::onDraw() {
	bodyModel.scheduleForDrawing();
	return true;
}


void daEnBoshi_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

int daEnBoshi_c::onExecute() {
	bodyModel._vf1C();
	updateModelMatrices();
	acState.execute();

	OSReport("HI 6\n");

	if (this->chrAnimation.isAnimationDone()) {
		this->chrAnimation.setCurrentFrame(0.0);
	}

	return true;
}