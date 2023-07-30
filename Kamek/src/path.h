#ifndef __PATH_H
#define __PATH_H

#include <dCourse.h>
#include <game.h>

float getDecimals(float x) {
	return (x - floor(x));
}



class dEnBlockPath_c : public daEnBlockMain_c {
public:
	dCourse_c* course;
	dCourse_c::rail_s* rail;
	dCourse_c::railNode_s* currentNode;
	dCourse_c::railNode_s* nextNode;
	int currentNodeNum, ignoreLastNodesCount;
	int steps;
	float speed;
	bool speedFromPath;

	bool loop;
	u8 waitForPlayer;
	int pathID;

	bool changeToDone;

	float dx;
	float dy;

	int distance;

	float ux;
	float uy;

	Vec stepVector;
	bool rotateNext;
	bool rotate0XNext;

	int stepCount;
	int stepsDone;
	float rest;

	u8 playerCollides;
	Vec2 offset;

	virtual void playerCollision(ActivePhysics* apThis, ActivePhysics* apOther);
	virtual void yoshiCollision(ActivePhysics* apThis, ActivePhysics* apOther);

	USING_STATES(dEnBlockPath_c);

	DECLARE_STATE(Wait);
	DECLARE_STATE(Init);
	DECLARE_STATE(FollowPath);
	DECLARE_STATE(Done);
};

CREATE_STATE(dEnBlockPath_c, Wait);
CREATE_STATE(dEnBlockPath_c, Init);
CREATE_STATE(dEnBlockPath_c, FollowPath);
CREATE_STATE(dEnBlockPath_c, Done);

void dEnBlockPath_c::beginState_Wait() { }
void dEnBlockPath_c::executeState_Wait() { }
void dEnBlockPath_c::endState_Wait() { }

void dEnBlockPath_c::beginState_Init() {
	this->waitForPlayer = this->settings >> 28 & 0b11;				//Bit 19-20
	speed = (float)(this->settings >> 16 & 0xF);					//Bit 29-32
	currentNodeNum = this->settings >> 8 & 0xFF;					//Bit 33-40
	pathID = this->settings >> 0 & 0xFF;							//Bit 41-48
	this->loop = false;

	speed /= 6;

	if (speed == 0) {
		speedFromPath = true;
	}

	if (pathID == 0) {
		changeToDone = true;
		return;
	}

	rail = GetRail(pathID);

	if (rail) {
		this->loop = rail->flags & 2; //this->settings >> 30 & 1;					   //Bit 18
		course = dCourseFull_c::instance->get(GetAreaNum());
		currentNode = &course->railNode[rail->startNode + currentNodeNum];
		nextNode = &course->railNode[rail->startNode + 1 + currentNodeNum];

		if(speedFromPath) {
			this->speed = currentNode->speed;
		}

		if ((rail->nodeCount - ignoreLastNodesCount) < currentNodeNum + 1) {
			OSReport("Sprite %d > WARNING: Surpased Node Count on Path %d\n", this->name, pathID);
			changeToDone = true;
		} else if ((rail->nodeCount - ignoreLastNodesCount) == currentNodeNum + 1) {
			if(loop) {
				nextNode = &course->railNode[rail->startNode];
				currentNodeNum = -1;
			} else {
				OSReport("Sprite %d > WARNING: Started on End Node on Path %d\n", this->name, pathID);
				this->pos.x = currentNode->xPos + offset.x;
				this->pos.y = (-currentNode->yPos) + offset.y;
				changeToDone = true;
			}
		}

		dx = nextNode->xPos - currentNode->xPos;
		dy = (-nextNode->yPos) - (-currentNode->yPos);

		distance = sqrtf(pow(dx, 2) + pow(dy, 2));

		ux = (dx / sqrtf((dx * dx) + (dy * dy)));
		uy = (dy / sqrtf((dx * dx) + (dy * dy)));

		stepVector.x = ux * speed;
		stepVector.y = uy * speed;
		if(abs(this->stepVector.x) <= 0.1f) {
			rotate0XNext = true;
		} else {
			rotateNext = true;
		}
		rest = 1 - getDecimals(distance / speed);

		stepCount = floor(distance / speed);

		stepsDone = 0;
	} else {
		OSReport("Sprite %d > WARNING: No Matching Path found (Path: %d)\n", this->name, pathID);
		changeToDone = true;
	}
}
void dEnBlockPath_c::executeState_Init() {
	if (changeToDone) {
		changeToDone = false;
		doStateChange(&StateID_Done);
	} else {
		this->pos.x = currentNode->xPos + offset.x;
		this->pos.y = (-currentNode->yPos) + offset.y;
		doStateChange(&StateID_FollowPath);
	}
}
void dEnBlockPath_c::endState_Init() { }

void dEnBlockPath_c::beginState_FollowPath() { }
void dEnBlockPath_c::executeState_FollowPath() {
	//OSReport("Execute Follow: %d, %d\n", this->waitForPlayer, this->playerCollides);
	if (stepsDone == stepCount) {
		this->rotateNext = false;
		this->rotate0XNext = false;
		if (waitForPlayer == 0 || (waitForPlayer > 0 && playerCollides)) {
			if (waitForPlayer == 1) {
				waitForPlayer = 0;
			} else if (waitForPlayer > 2) {
				OSReport("Sprite %d > WARNING: Undefined Wait for Player Setting on Path %d\n", this->name, pathID);
				doStateChange(&StateID_Done);
			}

			currentNodeNum++;
			currentNode = nextNode;
			nextNode = &course->railNode[rail->startNode + 1 + currentNodeNum];

			if(speedFromPath) {
				this->speed = currentNode->speed;
			}

			if ((rail->nodeCount - ignoreLastNodesCount) == currentNodeNum + 1) {
				if (!loop) {
					doStateChange(&StateID_Done);
				} else {
					nextNode = &course->railNode[rail->startNode];
					currentNodeNum = -1;

					dx = nextNode->xPos - currentNode->xPos;
					dy = (-nextNode->yPos) - (-currentNode->yPos);

					ux = (dx / sqrtf((dx * dx) + (dy * dy)));
					uy = (dy / sqrtf((dx * dx) + (dy * dy)));

					stepVector.x = ux * speed;
					stepVector.y = uy * speed;
					if(abs(this->stepVector.x) <= 0.1f) {
						rotate0XNext = true;
					} else {
						rotateNext = true;
					}
					
					this->pos.x = currentNode->xPos + offset.x;
					this->pos.y = (-currentNode->yPos) + offset.y;

					this->pos.x += stepVector.x * rest;
					this->pos.y += stepVector.y * rest;

					distance = (sqrtf(pow(dx, 2) + pow(dy, 2))) - (rest * speed);

					stepCount = floor(distance / speed);

					rest = 1 - getDecimals(distance / speed);

					stepsDone = 0;
				}
			} else if ((rail->nodeCount - ignoreLastNodesCount) < currentNodeNum) {
				OSReport("Sprite %d > WARNING: Surpased Node Count on Path %d\n", this->name, pathID);
				doStateChange(&StateID_Done);
			} else {
				dx = nextNode->xPos - currentNode->xPos;
				dy = (-nextNode->yPos) - (-currentNode->yPos);

				ux = (dx / sqrtf((dx * dx) + (dy * dy)));
				uy = (dy / sqrtf((dx * dx) + (dy * dy)));

				stepVector.x = ux * speed;
				stepVector.y = uy * speed;
				if(abs(this->stepVector.x) <= 0.1f) {
					rotate0XNext = true;
				} else {
					rotateNext = true;
				}

				this->pos.x = currentNode->xPos + offset.x;
				this->pos.y = (-currentNode->yPos) + offset.y;

				this->pos.x += stepVector.x * rest;
				this->pos.y += stepVector.y * rest;

				distance = (sqrtf(pow(dx, 2) + pow(dy, 2))) - (rest * speed);

				stepCount = floor(distance / speed);

				rest = 1 - getDecimals(distance / speed);

				stepsDone = 0;
			}
		}
	} else {
		if (waitForPlayer == 0) {
			stepsDone++;

			this->pos.x += stepVector.x;
			this->pos.y += stepVector.y;
		} else if (waitForPlayer == 1) {
			if (playerCollides) {
				waitForPlayer = 0;

				stepsDone++;

				this->pos.x += stepVector.x;
				this->pos.y += stepVector.y;
			}
		} else if (waitForPlayer == 2) {
			if (playerCollides) {
				stepsDone++;

				this->pos.x += stepVector.x;
				this->pos.y += stepVector.y;
			}
		} else {
			OSReport("Sprite %d > WARNING: Undefined Wait for Player Setting on Path %d\n", this->name, pathID);
			doStateChange(&StateID_Done);
		}
	}

	if (playerCollides == 5) {
		playerCollides = 0;
	} else if (playerCollides) {
		playerCollides += 1;
	}
}
void dEnBlockPath_c::endState_FollowPath() { }

void dEnBlockPath_c::beginState_Done() { }
void dEnBlockPath_c::executeState_Done() { }
void dEnBlockPath_c::endState_Done() { }

void dEnBlockPath_c::playerCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	playerCollides = true;
}

void dEnBlockPath_c::yoshiCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	playerCollides = true;
}








class dEnPath_c : public dEn_c {
public:
	dCourse_c* course;
	dCourse_c::rail_s* rail;
	dCourse_c::railNode_s* currentNode;
	dCourse_c::railNode_s* nextNode;
	int currentNodeNum, ignoreLastNodesCount;
	int steps;
	float speed;
	bool speedFromPath;

	bool loop;
	u8 waitForPlayer;
	int pathID;

	bool changeToDone;

	float dx;
	float dy;

	int distance;

	float ux;
	float uy;

	Vec stepVector;
	bool rotateNext;
	bool rotate0XNext;

	int stepCount;
	int stepsDone;
	float rest;

	u8 playerCollides;

	Vec2 offset;

	virtual void playerCollision(ActivePhysics* apThis, ActivePhysics* apOther);
	virtual void yoshiCollision(ActivePhysics* apThis, ActivePhysics* apOther);

	USING_STATES(dEnPath_c);

	DECLARE_STATE(Wait);
	DECLARE_STATE(Init);
	DECLARE_STATE(FollowPath);
	DECLARE_STATE(Done);
};

CREATE_STATE(dEnPath_c, Wait);
CREATE_STATE(dEnPath_c, Init);
CREATE_STATE(dEnPath_c, FollowPath);
CREATE_STATE(dEnPath_c, Done);

void dEnPath_c::beginState_Wait() { }
void dEnPath_c::executeState_Wait() { }
void dEnPath_c::endState_Wait() { }

void dEnPath_c::beginState_Init() {
	this->waitForPlayer = this->settings >> 28 & 0b11;				//Bit 19-20
	speed = (float)(this->settings >> 16 & 0xF);					//Bit 29-32
	currentNodeNum = this->settings >> 8 & 0xFF;					//Bit 33-40
	pathID = this->settings >> 0 & 0xFF;							//Bit 41-48
	this->loop = false;

	speed /= 6;

	if (speed == 0) {
		speedFromPath = true;
	}

	if (pathID == 0) {
		changeToDone = true;
		return;
	}

	rail = GetRail(pathID);

	if (rail) {
		this->loop = rail->flags & 2; //this->settings >> 30 & 1;					   //Bit 18
		course = dCourseFull_c::instance->get(GetAreaNum());
		currentNode = &course->railNode[rail->startNode + currentNodeNum];
		nextNode = &course->railNode[rail->startNode + 1 + currentNodeNum];

		if(speedFromPath) {
			this->speed = currentNode->speed;
		}

		if ((rail->nodeCount - ignoreLastNodesCount) < currentNodeNum + 1) {
			OSReport("Sprite %d > WARNING: Surpased Node Count on Path %d\n", this->name, pathID);
			changeToDone = true;
		} else if ((rail->nodeCount - ignoreLastNodesCount) == currentNodeNum + 1) {
			if(loop) {
				nextNode = &course->railNode[rail->startNode];
				currentNodeNum = -1;
			} else {
				OSReport("Sprite %d > WARNING: Started on End Node on Path %d\n", this->name, pathID);
				this->pos.x = currentNode->xPos + offset.x;
				this->pos.y = (-currentNode->yPos) + offset.y;
				changeToDone = true;
			}
		}

		dx = nextNode->xPos - currentNode->xPos;
		dy = (-nextNode->yPos) - (-currentNode->yPos);

		distance = sqrtf(pow(dx, 2) + pow(dy, 2));

		ux = (dx / sqrtf((dx * dx) + (dy * dy)));
		uy = (dy / sqrtf((dx * dx) + (dy * dy)));

		stepVector.x = ux * speed;
		stepVector.y = uy * speed;
		if(abs(this->stepVector.x) <= 0.1f) {
			rotate0XNext = true;
		} else {
			rotateNext = true;
		}
		rest = 1 - getDecimals(distance / speed);

		stepCount = floor(distance / speed);

		stepsDone = 0;
	} else {
		OSReport("Sprite %d > WARNING: No Matching Path found (Path: %d)\n", this->name, pathID);
		changeToDone = true;
	}
}
void dEnPath_c::executeState_Init() {
	if (changeToDone) {
		changeToDone = false;
		doStateChange(&StateID_Done);
	} else {
		this->pos.x = currentNode->xPos + offset.x;
		this->pos.y = (-currentNode->yPos) + offset.y;
		doStateChange(&StateID_FollowPath);
	}
}
void dEnPath_c::endState_Init() { }

void dEnPath_c::beginState_FollowPath() { }
void dEnPath_c::executeState_FollowPath() {
	//OSReport("Execute Follow: %d, %d\n", this->waitForPlayer, this->playerCollides);
	if (stepsDone == stepCount) {
		this->rotateNext = false;
		this->rotate0XNext = false;
		if (waitForPlayer == 0 || (waitForPlayer > 0 && playerCollides)) {
			if (waitForPlayer == 1) {
				waitForPlayer = 0;
			} else if (waitForPlayer > 2) {
				OSReport("Sprite %d > WARNING: Undefined Wait for Player Setting on Path %d\n", this->name, pathID);
				doStateChange(&StateID_Done);
			}

			currentNodeNum++;
			currentNode = nextNode;
			nextNode = &course->railNode[rail->startNode + 1 + currentNodeNum];

			if(speedFromPath) {
				this->speed = currentNode->speed;
			}

			if ((rail->nodeCount - ignoreLastNodesCount) == currentNodeNum + 1) {
				if (!loop) {
					doStateChange(&StateID_Done);
				} else {
					nextNode = &course->railNode[rail->startNode];
					currentNodeNum = -1;

					dx = nextNode->xPos - currentNode->xPos;
					dy = (-nextNode->yPos) - (-currentNode->yPos);

					ux = (dx / sqrtf((dx * dx) + (dy * dy)));
					uy = (dy / sqrtf((dx * dx) + (dy * dy)));

					stepVector.x = ux * speed;
					stepVector.y = uy * speed;
					if(abs(this->stepVector.x) <= 0.1f) {
						rotate0XNext = true;
					} else {
						rotateNext = true;
					}
					
					this->pos.x = currentNode->xPos + offset.x;
					this->pos.y = (-currentNode->yPos) + offset.y;

					this->pos.x += stepVector.x * rest;
					this->pos.y += stepVector.y * rest;

					distance = (sqrtf(pow(dx, 2) + pow(dy, 2))) - (rest * speed);

					stepCount = floor(distance / speed);

					rest = 1 - getDecimals(distance / speed);

					stepsDone = 0;
				}
			} else if ((rail->nodeCount - ignoreLastNodesCount) < currentNodeNum) {
				OSReport("Sprite %d > WARNING: Surpased Node Count on Path %d\n", this->name, pathID);
				doStateChange(&StateID_Done);
			} else {
				dx = nextNode->xPos - currentNode->xPos;
				dy = (-nextNode->yPos) - (-currentNode->yPos);

				ux = (dx / sqrtf((dx * dx) + (dy * dy)));
				uy = (dy / sqrtf((dx * dx) + (dy * dy)));

				stepVector.x = ux * speed;
				stepVector.y = uy * speed;
				if(abs(this->stepVector.x) <= 0.1f) {
					rotate0XNext = true;
				} else {
					rotateNext = true;
				}

				this->pos.x = currentNode->xPos + offset.x;
				this->pos.y = (-currentNode->yPos) + offset.y;

				this->pos.x += stepVector.x * rest;
				this->pos.y += stepVector.y * rest;

				distance = (sqrtf(pow(dx, 2) + pow(dy, 2))) - (rest * speed);

				stepCount = floor(distance / speed);

				rest = 1 - getDecimals(distance / speed);

				stepsDone = 0;
			}
		}
	} else {
		if (waitForPlayer == 0) {
			stepsDone++;

			this->pos.x += stepVector.x;
			this->pos.y += stepVector.y;
		} else if (waitForPlayer == 1) {
			if (playerCollides) {
				waitForPlayer = 0;

				stepsDone++;

				this->pos.x += stepVector.x;
				this->pos.y += stepVector.y;
			}
		} else if (waitForPlayer == 2) {
			if (playerCollides) {
				stepsDone++;

				this->pos.x += stepVector.x;
				this->pos.y += stepVector.y;
			}
		} else {
			OSReport("Sprite %d > WARNING: Undefined Wait for Player Setting on Path %d\n", this->name, pathID);
			doStateChange(&StateID_Done);
		}
	}

	if (playerCollides == 5) {
		playerCollides = 0;
	} else if (playerCollides) {
		playerCollides += 1;
	}
}
void dEnPath_c::endState_FollowPath() { }

void dEnPath_c::beginState_Done() { }
void dEnPath_c::executeState_Done() { }
void dEnPath_c::endState_Done() { }

void dEnPath_c::playerCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	playerCollides = true;
}

void dEnPath_c::yoshiCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	playerCollides = true;
}








class dPath_c : public dActorState_c {
public:
	dCourse_c* course;
	dCourse_c::rail_s* rail;
	dCourse_c::railNode_s* currentNode;
	dCourse_c::railNode_s* nextNode;
	int currentNodeNum, ignoreLastNodesCount;
	int steps;
	float speed;
	bool speedFromPath;

	bool loop;
	u8 waitForPlayer;
	int pathID;

	bool changeToDone;

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

	Vec2 offset;

	USING_STATES(dPath_c);

	DECLARE_STATE(Wait);
	DECLARE_STATE(Init);
	DECLARE_STATE(FollowPath);
	DECLARE_STATE(Done);
};

CREATE_STATE(dPath_c, Wait);
CREATE_STATE(dPath_c, Init);
CREATE_STATE(dPath_c, FollowPath);
CREATE_STATE(dPath_c, Done);

void dPath_c::beginState_Wait() { }
void dPath_c::executeState_Wait() { }
void dPath_c::endState_Wait() { }

void dPath_c::beginState_Init() {
	this->waitForPlayer = this->settings >> 28 & 0b11;					//Bit 19-20
	speed = this->settings >> 16 & 0xF;									//Bit 29-32
	currentNodeNum = this->settings >> 8 & 0xFF;						//Bit 33-40
	pathID = this->settings >> 0 & 0xFF;								//Bit 41-48
	this->loop = false;

	if (speed == 0) {
		speedFromPath = true;
	}

	if (pathID == 0) {
		changeToDone = true;
		return;
	}

	rail = GetRail(pathID);

	if (rail) {
		this->loop = rail->flags & 2; //this->settings >> 30 & 1;					   //Bit 18
		course = dCourseFull_c::instance->get(GetAreaNum());
		currentNode = &course->railNode[rail->startNode + currentNodeNum];
		nextNode = &course->railNode[rail->startNode + 1 + currentNodeNum];

		if(speedFromPath) {
			this->speed = currentNode->speed;
		}

		if ((rail->nodeCount - ignoreLastNodesCount) < currentNodeNum + 1) {
			OSReport("Sprite %d > WARNING: Surpased Node Count on Path %d\n", this->name, pathID);
			changeToDone = true;
		} else if ((rail->nodeCount - ignoreLastNodesCount) == currentNodeNum + 1) {
			if(loop) {
				nextNode = &course->railNode[rail->startNode];
				currentNodeNum = -1;
			} else {
				OSReport("Sprite %d > WARNING: Started on End Node on Path %d\n", this->name, pathID);
				this->pos.x = currentNode->xPos + offset.x;
				this->pos.y = (-currentNode->yPos) + offset.y;
				changeToDone = true;
			}
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
	} else {
		OSReport("Sprite %d > WARNING: No Matching Path found (Path: %d)\n", this->name, pathID);
		changeToDone = true;
	}
}
void dPath_c::executeState_Init() {
	if (changeToDone) {
		changeToDone = false;
		acState.setState(&StateID_Done);
	} else {
		this->pos.x = currentNode->xPos + offset.x;
		this->pos.y = (-currentNode->yPos) + offset.y;
		acState.setState(&StateID_FollowPath);
	}
}
void dPath_c::endState_Init() { }

void dPath_c::beginState_FollowPath() { }
void dPath_c::executeState_FollowPath() {
	//OSReport("Execute Follow: %d, %d\n", this->waitForPlayer, this->playerCollides);
	if (stepsDone == stepCount) {
		if (waitForPlayer == 0 || (waitForPlayer > 0 && playerCollides)) {
			if (waitForPlayer == 1) {
				waitForPlayer = 0;
			} else if (waitForPlayer > 2) {
				OSReport("Sprite %d > WARNING: Undefined Wait for Player Setting on Path %d\n", this->name, pathID);
				acState.setState(&StateID_Done);
			}

			currentNodeNum++;
			currentNode = nextNode;
			nextNode = &course->railNode[rail->startNode + 1 + currentNodeNum];

			if(speedFromPath) {
				this->speed = currentNode->speed;
			}

			if ((rail->nodeCount - ignoreLastNodesCount) == currentNodeNum + 1) {
				if (!loop) {
					acState.setState(&StateID_Done);
				} else {
					nextNode = &course->railNode[rail->startNode];
					currentNodeNum = -1;

					dx = nextNode->xPos - currentNode->xPos;
					dy = (-nextNode->yPos) - (-currentNode->yPos);

					ux = (dx / sqrtf((dx * dx) + (dy * dy)));
					uy = (dy / sqrtf((dx * dx) + (dy * dy)));

					stepVector.x = ux * speed;
					stepVector.y = uy * speed;

					this->pos.x = currentNode->xPos + offset.x;
					this->pos.y = (-currentNode->yPos) + offset.y;

					this->pos.x += stepVector.x * rest;
					this->pos.y += stepVector.y * rest;

					distance = (sqrtf(pow(dx, 2) + pow(dy, 2))) - (rest * speed);

					stepCount = floor(distance / speed);

					rest = 1 - getDecimals(distance / speed);

					stepsDone = 0;
				}
			} else if ((rail->nodeCount - ignoreLastNodesCount) < currentNodeNum) {
				OSReport("Sprite %d > WARNING: Surpased Node Count on Path %d\n", this->name, pathID);
				acState.setState(&StateID_Done);
			} else {
				dx = nextNode->xPos - currentNode->xPos;
				dy = (-nextNode->yPos) - (-currentNode->yPos);

				ux = (dx / sqrtf((dx * dx) + (dy * dy)));
				uy = (dy / sqrtf((dx * dx) + (dy * dy)));

				stepVector.x = ux * speed;
				stepVector.y = uy * speed;

				this->pos.x = currentNode->xPos + offset.x;
				this->pos.y = (-currentNode->yPos) + offset.y;

				this->pos.x += stepVector.x * rest;
				this->pos.y += stepVector.y * rest;

				distance = (sqrtf(pow(dx, 2) + pow(dy, 2))) - (rest * speed);

				stepCount = floor(distance / speed);

				rest = 1 - getDecimals(distance / speed);

				stepsDone = 0;
			}
		}
	} else {
		if (waitForPlayer == 0) {
			stepsDone++;

			this->pos.x += stepVector.x;
			this->pos.y += stepVector.y;
		} else if (waitForPlayer == 1) {
			if (playerCollides) {
				waitForPlayer = 0;

				stepsDone++;

				this->pos.x += stepVector.x;
				this->pos.y += stepVector.y;
			}
		} else if (waitForPlayer == 2) {
			if (playerCollides) {
				stepsDone++;

				this->pos.x += stepVector.x;
				this->pos.y += stepVector.y;
			}
		} else {
			OSReport("Sprite %d > WARNING: Undefined Wait for Player Setting on Path %d\n", this->name, pathID);
			acState.setState(&StateID_Done);
		}
	}

	if (playerCollides == 5) {
		playerCollides = 0;
	} else if (playerCollides) {
		playerCollides += 1;
	}
}
void dPath_c::endState_FollowPath() { }

void dPath_c::beginState_Done() { }
void dPath_c::executeState_Done() { }
void dPath_c::endState_Done() { }

#endif
