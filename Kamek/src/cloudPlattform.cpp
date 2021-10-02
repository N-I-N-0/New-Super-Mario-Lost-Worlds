#include <common.h>
#include <game.h>
#include <profile.h>
#include "boomeranghax.h"
#include "path.h"

const char* CloudPlattformFileList[] = { "cloudM", 0 };


class daEnCloudPlattform_c : public dEnPath_c {
public:
	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();


	mHeapAllocator_c allocator;
	nw4r::g3d::ResFile resFile;

	m3d::mdl_c bodyModel;

	int alreadyOnTop;

	float XSpeed;
	bool canI;
	u32 cmgr_returnValue;
	bool isOnTopOfTile;
	bool active;
	u16 timer;

	StandOnTopCollider sotCollider;


	/*u8 moveDirection;
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


	bool followPath;
	bool loop;
	int pathID;


	float dx;
	float dy;

	int distance;

	float ux;
	float uy;

	Vec stepVector;

	int stepCount;
	int stepsDone;

	bool playerCollides;*/


	static dActor_c* build();

	void updateModelMatrices();

	void spriteCollision(ActivePhysics* apThis, ActivePhysics* apOther);
	void playerCollision(ActivePhysics* apThis, ActivePhysics* apOther);
	void yoshiCollision(ActivePhysics* apThis, ActivePhysics* apOther);

	bool collisionCat3_StarPower(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat14_YoshiFire(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCatD_Drill(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat7_GroundPound(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat7_GroundPoundYoshi(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat9_RollingObject(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat1_Fireball_E_Explosion(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat13_Hammer(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCatA_PenguinMario(ActivePhysics* apThis, ActivePhysics* apOther);

	/*USING_STATES(daEnCloudPlattform_c);

	DECLARE_STATE(Wait);
	DECLARE_STATE(Run);*/
};


//CREATE_STATE(daEnCloudPlattform_c, Wait);
//CREATE_STATE(daEnCloudPlattform_c, Run);


/*void daEnCloudPlattform_c::beginState_Wait() { OSReport("HI 0.7 \n");}
void daEnCloudPlattform_c::executeState_Wait() {
	if (stepsDone == 0 && followPath)
	{
		dStageActor_c* player = (dStageActor_c*)FindActorByType(PLAYER, 0);

		OSReport("HI 1\n");

		this->pos.x = currentNode->xPos;
		this->pos.y = (-currentNode->yPos);

		if (!loop)
		{
			if (playerCollides && player->pos.y > this->pos.y && player->speed.y <= 0)
			{
				OSReport("HI 2\n");
				doStateChange(&StateID_Run);
			}
		}
		else
		{
			doStateChange(&StateID_Run);
		}
	}
}
void daEnCloudPlattform_c::endState_Wait() { OSReport("HI 2.3\n"); }


void daEnCloudPlattform_c::beginState_Run()
{
	OSReport("HI 2.7\n");
}
void daEnCloudPlattform_c::executeState_Run() {
	OSReport("HI 3\n");

	if (stepsDone == stepCount)
	{
		currentNodeNum++;
		currentNode = nextNode;
		nextNode = &course->railNode[rail->startNode + 1 + currentNodeNum];

		OSReport("RailNodeCount: %daaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa, CNN: %d\n", rail->nodeCount, currentNodeNum);

		if (rail->nodeCount == currentNodeNum + 1)
		{
			if (!loop)
			{
				doStateChange(&StateID_Wait);
			}
			else
			{
				int nextNodeNum = this->settings >> 8 & 0b11111111;

				nextNode = &course->railNode[rail->startNode + nextNodeNum];


				dx = nextNode->xPos - currentNode->xPos;
				dy = (-nextNode->yPos) - (-currentNode->yPos);

				distance = sqrtf(pow(dx, 2) + pow(dy, 2));

				ux = (dx / sqrtf((dx * dx) + (dy * dy)));
				uy = (dy / sqrtf((dx * dx) + (dy * dy)));

				stepVector.x = ux * speed;
				stepVector.y = uy * speed;

				stepCount = floor(distance / speed);

				stepsDone = 0;

				this->pos.x = currentNode->xPos;
				this->pos.y = (-currentNode->yPos);
			}
		}
		else if (rail->nodeCount < currentNodeNum + 1)
		{
			currentNodeNum = this->settings >> 8 & 0b11111111;
			currentNode = &course->railNode[rail->startNode + currentNodeNum];
			nextNode = &course->railNode[rail->startNode + 1 + currentNodeNum];


			dx = nextNode->xPos - currentNode->xPos;
			dy = (-nextNode->yPos) - (-currentNode->yPos);

			distance = sqrtf(pow(dx, 2) + pow(dy, 2));

			ux = (dx / sqrtf((dx * dx) + (dy * dy)));
			uy = (dy / sqrtf((dx * dx) + (dy * dy)));

			stepVector.x = ux * speed;
			stepVector.y = uy * speed;

			stepCount = floor(distance / speed);

			stepsDone = 0;

			this->pos.x = currentNode->xPos;
			this->pos.y = (-currentNode->yPos);
		}
		else
		{
			dx = nextNode->xPos - currentNode->xPos;
			dy = (-nextNode->yPos) - (-currentNode->yPos);

			distance = sqrtf(pow(dx, 2) + pow(dy, 2));

			ux = (dx / sqrtf((dx * dx) + (dy * dy)));
			uy = (dy / sqrtf((dx * dx) + (dy * dy)));

			stepVector.x = ux * speed;
			stepVector.y = uy * speed;

			stepCount = floor(distance / speed);

			stepsDone = 0;

			this->pos.x = currentNode->xPos;
			this->pos.y = (-currentNode->yPos);
		}
	}
	else
	{
		stepsDone++;

		this->pos.x += stepVector.x;
		this->pos.y += stepVector.y;
	}
}

void daEnCloudPlattform_c::endState_Run() { OSReport("HI 5.3\n"); }*/




const SpriteData CloudPlattformSpriteData = { ProfileId::CloudPlattform, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile CloudPlattformProfile(&daEnCloudPlattform_c::build, SpriteId::CloudPlattform, CloudPlattformSpriteData, ProfileId::CloudPlattform, ProfileId::CloudPlattform, "CloudPlattform", CloudPlattformFileList);



u8 hijackMusicWithSongName(const char* songName, int themeID, bool hasFast, int channelCount, int trackCount, int* wantRealStreamID);

void daEnCloudPlattform_c::spriteCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	if (this->active == true)
	{
		this->active = false;
	}
}
void daEnCloudPlattform_c::playerCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	daPlBase_c* player = (daPlBase_c*)apOther->owner;

	OSReport("PLLLAYERUP %d\n", CheckExistingPowerup(player));

	if (this->active == true && CheckExistingPowerup(player) != 13 && CheckExistingPowerup(player) != 3)
	{
		this->active = false;
	}

	playerCollides = true;
}
void daEnCloudPlattform_c::yoshiCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	daPlBase_c* player = (daPlBase_c*)apOther->owner;

	if (this->active == true && CheckExistingPowerup(player) != 13 && CheckExistingPowerup(player) != 3)
	{
		this->active = false;
	}

	playerCollides = true;
}
bool daEnCloudPlattform_c::collisionCat7_GroundPound(ActivePhysics* apThis, ActivePhysics* apOther) {
	daPlBase_c* player = (daPlBase_c*)apOther->owner;

	if (this->active == true)
	{
		this->active = false;
	}
	return false;
}
bool daEnCloudPlattform_c::collisionCat7_GroundPoundYoshi(ActivePhysics* apThis, ActivePhysics* apOther) {
	daPlBase_c* player = (daPlBase_c*)apOther->owner;

	if (this->active == true)
	{
		this->active = false;
	}
	return false;
}
bool daEnCloudPlattform_c::collisionCatD_Drill(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnCloudPlattform_c::collisionCatA_PenguinMario(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}

bool daEnCloudPlattform_c::collisionCat1_Fireball_E_Explosion(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnCloudPlattform_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics* apThis, ActivePhysics* apOther) {

	return false;
}
bool daEnCloudPlattform_c::collisionCat9_RollingObject(ActivePhysics* apThis, ActivePhysics* apOther) {

	return false;
}
bool daEnCloudPlattform_c::collisionCat13_Hammer(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnCloudPlattform_c::collisionCat14_YoshiFire(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}

bool daEnCloudPlattform_c::collisionCat3_StarPower(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}


dActor_c* daEnCloudPlattform_c::build() {
	void* buffer = AllocFromGameHeap1(sizeof(daEnCloudPlattform_c));
	daEnCloudPlattform_c* c = new(buffer) daEnCloudPlattform_c;

	return c;
}


extern int getNybbleValue(u32 settings, int fromNybble, int toNybble);

int daEnCloudPlattform_c::onCreate() {
	this->alreadyOnTop = 0;
	this->canI = false;
	OSReport("daCloudPlattform_c::onCreate()!!!\n");

	this->deleteForever = true;

	// Model creation	
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	this->resFile.data = getResource("cloudM", "g3d/power_cloud.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("spincloud");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_MapObj(&bodyModel, 0);

	allocator.unlink();

	/*this->followPath = this->settings >> 31 & 1;

	this->loop = this->settings >> 30 & 1;

	this->aTestFloat = 0.25f;

	done = false;

	speed = this->settings >> 16 & 0b1111;						    //Bit 29-32
	currentNodeNum = this->settings >> 8 & 0b11111111;						//Bit 33-40
	int pathID = this->settings >> 0 & 0b11111111;                               //Bit 41-48
	OSReport("speed: %d, currentNodeNum: %d, pathID: %d\n", speed, currentNodeNum, pathID);
	if (pathID) {
		rail = GetRail(pathID);
		course = dCourseFull_c::instance->get(GetAreaNum());
		currentNode = &course->railNode[rail->startNode + currentNodeNum];
		nextNode = &course->railNode[rail->startNode + 1 + currentNodeNum];


		dx = nextNode->xPos - currentNode->xPos;
		dy = (-nextNode->yPos) - (-currentNode->yPos);

		distance = sqrtf(pow(dx, 2) + pow(dy, 2));

		ux = (dx / sqrtf((dx * dx) + (dy * dy)));
		uy = (dy / sqrtf((dx * dx) + (dy * dy)));

		stepVector.x = ux * speed;
		stepVector.y = uy * speed;

		stepCount = floor(distance / speed);

		stepsDone = 0;


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


	playerCollides = false;*/


	// Stuff I do understand
	this->scale = (Vec){1.2, 1.2, 1.2};
	// this->scale = (Vec){ 0.0, 0.0, 0.0 };

	this->rot.x = 0;
	this->rot.y = 0;
	this->rot.z = 0;
	//this->direction = (this->settings >> 8) & 0xF; // Heading where the player look.


	float fWidth = 3 * 16.0f;
	sotCollider.init(this,
		/*xOffset=*/-23.0f, /*yOffset=*/7.0f,
		/*topYOffset=*/0,
		/*rightSize=*/fWidth, /*leftSize=*/0,
		/*rotation=*/0, /*_45=*/1
	);

	// What is this for. I dunno
	sotCollider._47 = 0xA;
	sotCollider.flags = 0x80180 | 0xC00;

	sotCollider.addToList();


	ActivePhysics::Info HitMeBaby;

	HitMeBaby.xDistToCenter = 0.0;
	HitMeBaby.yDistToCenter = 8.0;

	HitMeBaby.xDistToEdge = 24;
	HitMeBaby.yDistToEdge = 1;

	HitMeBaby.category1 = 0x3;
	HitMeBaby.category2 = 0x0;
	HitMeBaby.bitfield1 = 0x6F;
	HitMeBaby.bitfield2 = 0xffbafffe;
	HitMeBaby.unkShort1C = 0;
	HitMeBaby.callback = &dEn_c::collisionCallback;

	this->aPhysics.initWithStruct(this, &HitMeBaby);
	this->aPhysics.addToList();

	this->pos.y += 16;
	this->pos.z = 4000;

	this->active = true;
	this->timer = 0;

	doStateChange(&StateID_Init);

	this->onExecute();
	return true;
}


int daEnCloudPlattform_c::onDelete() {
	this->Delete(1);
	return true;
}

int daEnCloudPlattform_c::onDraw() {
	bodyModel.scheduleForDrawing();
	return true;
}


void daEnCloudPlattform_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}


int daEnCloudPlattform_c::onExecute() {
	bodyModel._vf1C();
	updateModelMatrices();
	acState.execute();

	//OSReport("Execute\n");

	if (!active)
	{
		timer += 1;
		if (timer == 1)
		{
			float fWidth = 0;
			sotCollider.init(this,
				/*xOffset=*/-23.0f, /*yOffset=*/7.0f,
				/*topYOffset=*/0,
				/*rightSize=*/fWidth, /*leftSize=*/0,
				/*rotation=*/0, /*_45=*/1
			);

			// What is this for. I dunno
			sotCollider._47 = 0xA;
			sotCollider.flags = 0x80180 | 0xC00;

			sotCollider.addToList();
		}
		if (timer < 25)
		{
			this->scale.x -= 0.05;
			this->scale.y -= 0.05;
			this->scale.z -= 0.05;
		}
		if (timer == 25)
		{
			this->scale.x = 0;
			this->scale.y = 0;
			this->scale.z = 0;
		}
		if (timer > 104)
		{
			this->scale.x += 0.05;
			this->scale.y += 0.05;
			this->scale.z += 0.05;
		}
		if (timer == 128)
		{
			this->scale.x = 1.2;
			this->scale.y = 1.2;
			this->scale.z = 1.2;

			float fWidth = 3 * 16.0f;
			sotCollider.init(this,
				/*xOffset=*/-23.0f, /*yOffset=*/7.0f,
				/*topYOffset=*/0,
				/*rightSize=*/fWidth, /*leftSize=*/0,
				/*rotation=*/0, /*_45=*/1
			);

			// What is this for. I dunno
			sotCollider._47 = 0xA;
			sotCollider.flags = 0x80180 | 0xC00;

			sotCollider.addToList();

			timer = 0;
			active = true;
		}
	}

	/*OSReport("speed: %d, followPath: %d, pathID: %d, currentNodeNum: %d, loop: %d, posxy: %f|%f\n", this->speed, this->followPath, this->pathID, this->currentNodeNum, this->loop, this->pos.x, this->pos.y);

	if (this->followPath)
	{
		OSReport("FollowsPath\n");
		if (!done)
		{
			OSReport("NotDone\n");

			float dx = nextNode->xPos - currentNode->xPos;
			float dy = (-nextNode->yPos) - (-currentNode->yPos);

			float ux = (dx / sqrtf((dx * dx) + (dy * dy)));
			float uy = (dy / sqrtf((dx * dx) + (dy * dy)));

			this->pos.y += uy * speed;
			this->pos.x += ux * speed;

			if (dx <= 0)
			{
				//this->rot.y = 0x4000;
				//this->rot.y *= -1;
			}
			else
			{
				//this->rot.y = 0x4000;
			}

			OSReport("x: %f; y: %f\n", this->pos.x, this->pos.y);
			OSReport("nx: %f; ny: %f\n", nextNode->xPos, -nextNode->yPos);
			OSReport("dx: %f; dy: %f\n", dx, dy);
			OSReport("ux: %f; uy: %f\n", ux, uy);

			if (this->pos.x > dx + currentNode->xPos - 4 && this->pos.x < dx + currentNode->xPos + 4 && this->pos.y > dy - currentNode->yPos - 4 && this->pos.y < dy - currentNode->yPos + 4)
			{
				done = true;
			}
		}
		else
		{
			OSReport("nodecount: %d, currentnodenum: %d\n", rail->nodeCount, currentNodeNum);
			//if (rail->nodeCount > currentNodeNum + 1)
			//{
				currentNodeNum++;
				currentNode = nextNode;
				nextNode = &course->railNode[rail->startNode + 1 + currentNodeNum];
				this->pos.x = currentNode->xPos;
				this->pos.y = -currentNode->yPos;
			//}

			if (rail->nodeCount != currentNodeNum + 1)
			{
				done = false;
			}
		}
	}*/

	//playerCollides = false;

	sotCollider.update();
	return true;
}