extern "C" bool daEnKuriboBase_c__isFrozen(int);
extern dStateBase_c StateID_Turn__Goomba;
extern dStateBase_c StateID_Ice__Goomba;
extern dStateBase_c StateID_DieFall__Goomba;
extern dStateBase_c StateID_DieOther__Goomba;
extern dStateBase_c StateID_Walk__Goomba;

#include <common.h>
#include <game.h>
#include <profile.h>

const char* GoombaTowerFileList[] = { "kuribo", 0 };


class daEnGoombaTower_c : public dEn_c
{
public:
	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	static dActor_c* build();

	dStageActor_c* goombas[16];

	u8 stackSize;
	u8 firstGoombaID;
};

const SpriteData GoombaTowerSpriteData = { ProfileId::GoombaTower, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile GoombaTowerProfile(&daEnGoombaTower_c::build, SpriteId::GoombaTower, &GoombaTowerSpriteData, ProfileId::GoombaTower, ProfileId::GoombaTower, "GoombaTower", GoombaTowerFileList);

dActor_c* daEnGoombaTower_c::build() 
{
	void* buffer = AllocFromGameHeap1(sizeof(daEnGoombaTower_c));
	daEnGoombaTower_c* c = new(buffer) daEnGoombaTower_c;

	return c;
}

int daEnGoombaTower_c::onCreate()
{
	this->stackSize = (this->settings >> 12 & 0xF) + 1;

	OSReport("StackSize: %d\n", this->stackSize);

	Vec3 goombaPos;

	for (int i = 0; i < this->stackSize; i++)
	{
		goombaPos.x = this->pos.x;
		goombaPos.y = this->pos.y + (17 * i);
		goombaPos.z = this->pos.z - (500 * i);
		this->goombas[i] = (dStageActor_c*)CreateActor(51, 0, goombaPos, 0, 0);

		//goombas[i]->collMgr.pPos = goombas[0]->collMgr;
	}

	firstGoombaID = 0;

	goombas[0]->collMgr.yDeltaMaybe = 14.5 * (stackSize - 1);
	((lineSensor_s*)(goombas[0]->collMgr.pAboveInfo))->lineB += (0x11000) * (stackSize - 1);
	((lineSensor_s*)(goombas[0]->collMgr.pAdjacentInfo))->flags = (0x6);
	//((lineSensor_s*)(goombas[0]->collMgr.pAboveInfo))->flags = (0x1);
	//((lineSensor_s*)(goombas[0]->collMgr.pAboveInfo))->lineA *= 6;

	/*lineSensor_s below(-5 << 12, 5 << 12, 0 << 12);
	pointSensor_s above(0 << 12, 12 << 12);
	lineSensor_s adjacent(6 << 12, 9 << 12, 6 << 12);

	goombas[0]->collMgr.pBelowInfo = &below;
	goombas[0]->collMgr.pAboveInfo = &above;
	goombas[0]->collMgr.pAdjacentInfo = &adjacent;
	goombas[0]->collMgr.calculateBelowCollisionWithSmokeEffect();*/

	for (int i = 1; i < this->stackSize; i++)
	{
		this->goombas[i]->speed.x = this->goombas[0]->speed.x;
		this->goombas[i]->speed.y = this->goombas[0]->speed.y;
		this->goombas[i]->speed.z = this->goombas[0]->speed.z;

		this->goombas[i]->direction = this->goombas[0]->direction;

		this->goombas[i]->collMgr.clear1();
		this->goombas[i]->collMgr.clear2();
	}

	this->onExecute();
	return true;
}


int daEnGoombaTower_c::onDelete() 
{
	return true;
}

int daEnGoombaTower_c::onDraw() 
{
	return true;
}

int daEnGoombaTower_c::onExecute()
{
	//dStateBase_c* turnstate;

	u8 nulloombas = 0;
	/*dStateBase_c* zeroPlayerState = ((dEn_c*)this->goombas[firstGoombaID])->acState.getCurrentState();
	

	for (int i = 0; i < this->stackSize; i++)
	{
		if (goombas[i] != 0)
		{
			turnstate = ((dEn_c*)this->goombas[i])->acState.getCurrentState();

			if (turnstate == &StateID_Turn__Goomba)
			{
				if (zeroPlayerState == &StateID_Walk__Goomba)
				{
					((dEn_c*)goombas[firstGoombaID])->doStateChange(&StateID_Turn__Goomba);

					zeroPlayerState = ((dEn_c*)this->goombas[firstGoombaID])->acState.getCurrentState();
				}
			}
			else if (turnstate != &StateID_Walk__Goomba)
			{
				goombas[i] = 0;

				if (i == firstGoombaID)
				{
					for (int j = 0; j < this->stackSize; j++)
					{
						if (goombas[j] != 0)
						{
							firstGoombaID = j;
							break;
						}
					}

					zeroPlayerState = ((dEn_c*)this->goombas[firstGoombaID])->acState.getCurrentState();
				}
			}

		}
		else
		{
			nulloombas += 1;
		}
	}

	if (goombas[firstGoombaID] != 0)
	{
		this->pos.x = goombas[firstGoombaID]->pos.x;
		this->pos.y = goombas[firstGoombaID]->pos.y;
		this->pos.z = goombas[firstGoombaID]->pos.z;
	}
	else
	{
		this->Delete(1);
		return true;
	}

	for (int i = 0; i < this->stackSize; i++)
	{
		if (this->goombas[i] != 0)
		{
			turnstate = ((dEn_c*)this->goombas[i])->acState.getCurrentState();

			if (turnstate != zeroPlayerState)
			{
				((dEn_c*)goombas[i])->doStateChange(zeroPlayerState);
			}

			this->goombas[i]->pos.x = this->goombas[firstGoombaID]->pos.x;
			this->goombas[i]->pos.y = this->goombas[firstGoombaID]->pos.y + (17 * (i - nulloombas));
			this->goombas[i]->pos.z = this->goombas[firstGoombaID]->pos.z - (500 * (i - nulloombas));

			this->goombas[i]->speed.x = this->goombas[firstGoombaID]->speed.x;
			this->goombas[i]->speed.y = this->goombas[firstGoombaID]->speed.y;
			this->goombas[i]->speed.z = this->goombas[firstGoombaID]->speed.z;
		}
	}*/

	for (int i = 0; i < this->stackSize; i++)
	{
		if (this->goombas[i] != 0)
		{
			this->goombas[i]->pos.x = this->goombas[firstGoombaID]->pos.x;
			this->goombas[i]->pos.y = this->goombas[firstGoombaID]->pos.y + (17 * (i - nulloombas));
			this->goombas[i]->pos.z = this->goombas[firstGoombaID]->pos.z - (500 * (i - nulloombas));

			this->goombas[i]->speed.x = this->goombas[firstGoombaID]->speed.x;
			this->goombas[i]->speed.y = this->goombas[firstGoombaID]->speed.y;
			this->goombas[i]->speed.z = this->goombas[firstGoombaID]->speed.z;

			//this->goombas[i]->direction = this->goombas[firstGoombaID]->direction;

			//goombas[i]->collMgr = goombas[firstGoombaID]->collMgr;

			//((lineSensor_s*)(goombas[0]->collMgr.pAdjacentInfo))->distanceFromCenter = 66;
			//((lineSensor_s*)(goombas[0]->collMgr.pAboveInfo))->lineB = (0x10000) * stackSize;
		}
	}

	lineSensor_s* Ab = ((lineSensor_s*)(goombas[0]->collMgr.pAboveInfo));
	lineSensor_s* B = ((lineSensor_s*)(goombas[0]->collMgr.pBelowInfo));
	lineSensor_s* Aj = ((lineSensor_s*)(goombas[0]->collMgr.pAdjacentInfo));

	OSReport("Ab: flags %x, lA %x, lB %x, d %x | B: flags %x, lA %x, lB %x, d %x | Aj: flags %x, lA %x, lB %x, d %x\n", Ab->flags, Ab->lineA, Ab->lineB, Ab->distanceFromCenter, B->flags, B->lineA, B->lineB, B->distanceFromCenter, Aj->flags, Aj->lineA, Aj->lineB, Aj->distanceFromCenter);

	return true;
}