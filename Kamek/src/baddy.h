#ifndef __BADDY_H
#define __BADDY_H

#include <common.h>
#include <game.h>
#include <dCourse.h>

extern "C" dCourse_c::rail_s * GetRail(int id);

class daEnLaunchStar_c;
class daEnShyCart_c;
class daEnRail_c;

daEnShyCart_c* playerCart[4] = {};

bool launchStarChipCollectedBeforeFlag[32][5];
bool launchStarChipCollectedAfterFlag[32][5];

u8 playerStatus[4] = { 0,0,0,0 };

class dEnPath_c : public dEn_c
{
public:
	dCourse_c* course;
	dCourse_c::rail_s* rail;
	dCourse_c::railNode_s* currentNode;
	dCourse_c::railNode_s* nextNode;
	int currentNodeNum;
	int steps;
	int speed;


	bool followPath;
	bool loop;
	u8 waitForPlayer;
	int pathID;


	float dx;
	float dy;

	int distance;

	float ux;
	float uy;

	Vec stepVector;

	int stepCount;
	int stepsDone;
	float rest;

	u8 playerCollides;

	virtual void playerCollision(ActivePhysics* apThis, ActivePhysics* apOther);
	virtual void yoshiCollision(ActivePhysics* apThis, ActivePhysics* apOther);

	USING_STATES(dEnPath_c);

	DECLARE_STATE(Wait);
	DECLARE_STATE(FollowPath);
	DECLARE_STATE(Done);
};

float getDecimals(float x)
{
	return (x - floor(x));
}


CREATE_STATE(dEnPath_c, Wait);
CREATE_STATE(dEnPath_c, FollowPath);
CREATE_STATE(dEnPath_c, Done);

void dEnPath_c::beginState_Wait() 
{
	OSReport("Begin Wait\n");

	this->followPath = this->settings >> 31 & 1;                  //Bit 17
	this->loop = this->settings >> 30 & 1;                       //Bit 18
	this->waitForPlayer = this->settings >> 28 & 0b11;                       //Bit 19-20
	speed = this->settings >> 16 & 0b1111;						    //Bit 29-32
	currentNodeNum = this->settings >> 8 & 0b11111111;						//Bit 33-40
	int pathID = this->settings >> 0 & 0b11111111;                               //Bit 41-48

	if (speed == 0)
	{
		OSReport("WARNING: CAN NOT DIVIDE BY ZERO!\n");
		doStateChange(&StateID_Done);
	}

	rail = GetRail(pathID);

	if (rail) {
		course = dCourseFull_c::instance->get(GetAreaNum());
		currentNode = &course->railNode[rail->startNode + currentNodeNum];
		nextNode = &course->railNode[rail->startNode + 1 + currentNodeNum];

		if (rail->nodeCount < currentNodeNum + 1)
		{
			OSReport("WARNING: SURPASED NODECOUNT\n");
			doStateChange(&StateID_Done);
		}

		dx = nextNode->xPos - currentNode->xPos;
		dy = (-nextNode->yPos) - (-currentNode->yPos);

		distance = sqrtf(pow(dx, 2) + pow(dy, 2));

		ux = (dx / sqrtf((dx * dx) + (dy * dy)));
		uy = (dy / sqrtf((dx * dx) + (dy * dy)));

		stepVector.x = ux * speed;
		stepVector.y = uy * speed;

		rest = 1 - getDecimals(distance / speed);

		stepCount = floor(distance / speed);

		stepsDone = 0;

		this->pos.x = currentNode->xPos;
		this->pos.y = -currentNode->yPos;
	}
	else {
		OSReport("WARNING: NO MATCHINCG PATH FOUND\n");
		doStateChange(&StateID_Done);
	}
}
void dEnPath_c::executeState_Wait() 
{
	//OSReport("Execute Wait\n");

	if (followPath)
	{
		this->pos.x = currentNode->xPos;
		this->pos.y = (-currentNode->yPos);
		doStateChange(&StateID_FollowPath);
	}
}
void dEnPath_c::endState_Wait() { OSReport("End Wait\n"); }

void dEnPath_c::beginState_FollowPath() { OSReport("Begin Follow\n"); }
void dEnPath_c::executeState_FollowPath()
{
	//OSReport("Execute Follow: %d, %d\n", this->waitForPlayer, this->playerCollides);
	if (stepsDone == stepCount)
	{
		if (waitForPlayer == 0 || (waitForPlayer > 0 && playerCollides))
		{
			if (waitForPlayer == 1)
			{
				waitForPlayer = 0;
			}
			else if (waitForPlayer > 2)
			{
				OSReport("WARNING: UNDEFINED WAITFORPLAYER\n");
				doStateChange(&StateID_Done);
			}

			currentNodeNum++;
			currentNode = nextNode;
			nextNode = &course->railNode[rail->startNode + 1 + currentNodeNum];

			if (rail->nodeCount == currentNodeNum + 1)
			{
				if (!loop)
				{
					doStateChange(&StateID_Done);
				}
				else
				{
					int nextNodeNum = this->settings >> 8 & 0b11111111;

					nextNode = &course->railNode[rail->startNode + nextNodeNum];


					dx = nextNode->xPos - currentNode->xPos;
					dy = (-nextNode->yPos) - (-currentNode->yPos);

					ux = (dx / sqrtf((dx * dx) + (dy * dy)));
					uy = (dy / sqrtf((dx * dx) + (dy * dy)));

					stepVector.x = ux * speed;
					stepVector.y = uy * speed;

					this->pos.x = currentNode->xPos;
					this->pos.y = (-currentNode->yPos);

					this->pos.x += stepVector.x * rest;
					this->pos.y += stepVector.y * rest;

					distance = (sqrtf(pow(dx, 2) + pow(dy, 2))) - (rest * speed);

					stepCount = floor(distance / speed);

					rest = 1 - getDecimals(distance / speed);

					stepsDone = 0;
				}
			}
			else if (rail->nodeCount == currentNodeNum)
			{
				if (!loop)
				{
					OSReport("WARNING: SURPASED NODECOUNT\n");
					doStateChange(&StateID_Done);
				}
				else
				{
					currentNodeNum = this->settings >> 8 & 0b11111111;
					currentNode = &course->railNode[rail->startNode + currentNodeNum];
					nextNode = &course->railNode[rail->startNode + 1 + currentNodeNum];

					dx = nextNode->xPos - currentNode->xPos;
					dy = (-nextNode->yPos) - (-currentNode->yPos);

					ux = (dx / sqrtf((dx * dx) + (dy * dy)));
					uy = (dy / sqrtf((dx * dx) + (dy * dy)));

					stepVector.x = ux * speed;
					stepVector.y = uy * speed;

					this->pos.x = currentNode->xPos;
					this->pos.y = (-currentNode->yPos);

					this->pos.x += stepVector.x * rest;
					this->pos.y += stepVector.y * rest;

					distance = (sqrtf(pow(dx, 2) + pow(dy, 2))) - (rest * speed);

					stepCount = floor(distance / speed);

					rest = 1 - getDecimals(distance / speed);

					stepsDone = 0;
				}
			}
			else if (rail->nodeCount < currentNodeNum)
			{
				OSReport("WARNING: SURPASED NODECOUNT\n");
				doStateChange(&StateID_Done);
			}
			else
			{
				dx = nextNode->xPos - currentNode->xPos;
				dy = (-nextNode->yPos) - (-currentNode->yPos);

				ux = (dx / sqrtf((dx * dx) + (dy * dy)));
				uy = (dy / sqrtf((dx * dx) + (dy * dy)));

				stepVector.x = ux * speed;
				stepVector.y = uy * speed;

				this->pos.x = currentNode->xPos;
				this->pos.y = (-currentNode->yPos);

				this->pos.x += stepVector.x * rest;
				this->pos.y += stepVector.y * rest;

				distance = (sqrtf(pow(dx, 2) + pow(dy, 2))) - (rest * speed);

				stepCount = floor(distance / speed);

				rest = 1 - getDecimals(distance / speed);

				stepsDone = 0;
			}
		}
	}
	else
	{
		if (waitForPlayer == 0)
		{
			stepsDone++;

			this->pos.x += stepVector.x;
			this->pos.y += stepVector.y;
		}
		else if (waitForPlayer == 1)
		{
			if (playerCollides)
			{
				waitForPlayer = 0;

				stepsDone++;

				this->pos.x += stepVector.x;
				this->pos.y += stepVector.y;
			}
		}
		else if (waitForPlayer == 2)
		{
			if (playerCollides)
			{
				stepsDone++;

				this->pos.x += stepVector.x;
				this->pos.y += stepVector.y;
			}
		}
		else
		{
			OSReport("WARNING: UNDEFINED WAITFORPLAYER\n");
			doStateChange(&StateID_Done);
		}
	}

	if (playerCollides == 5)
	{
		playerCollides = 0;
	}
	else if (playerCollides)
	{
		playerCollides += 1;
	}
}
void dEnPath_c::endState_FollowPath() { OSReport("End Follow\n"); }

void dEnPath_c::beginState_Done() { OSReport("Begin Done\n"); }
void dEnPath_c::executeState_Done() { /*OSReport("Execute Done\n");*/ }
void dEnPath_c::endState_Done() { OSReport("End Done\n"); }

void dEnPath_c::playerCollision(ActivePhysics* apThis, ActivePhysics* apOther) 
{
	playerCollides = true;
}

void dEnPath_c::yoshiCollision(ActivePhysics* apThis, ActivePhysics* apOther) 
{
	playerCollides = true;
}

#endif