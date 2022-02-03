extern "C" bool daEnKuriboBase_c__isFrozen(int);

#include <common.h>
#include <game.h>
#include <profile.h>

const char* GoombaTowerFileList[] = { "kuribo", 0 };


class daEnGoombaTower_c : public dEn_c {
public:
	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	static dActor_c* build();

	dStageActor_c* goombas[16];
	int goombasDirection[16];

	u8 stackSize;

	int goombaTurn;
	dStateBase_c* turnstate;

	bool starting;
};

const SpriteData GoombaTowerSpriteData = { ProfileId::GoombaTower, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile GoombaTowerProfile(&daEnGoombaTower_c::build, SpriteId::GoombaTower, &GoombaTowerSpriteData, ProfileId::GoombaTower, ProfileId::GoombaTower, "GoombaTower", GoombaTowerFileList);

dActor_c* daEnGoombaTower_c::build() {
	void* buffer = AllocFromGameHeap1(sizeof(daEnGoombaTower_c));
	daEnGoombaTower_c* c = new(buffer) daEnGoombaTower_c;

	return c;
}

int daEnGoombaTower_c::onCreate() {

	this->stackSize = (this->settings >> 12 & 0xF) + 1;

	this->starting = true;

	this->onExecute();
	return true;
}


int daEnGoombaTower_c::onDelete() {
	return true;
}

int daEnGoombaTower_c::onDraw() {
	return true;
}

extern dStateBase_c StateID_Turn__Goomba;
extern dStateBase_c StateID_Ice__Goomba;
extern dStateBase_c StateID_DieFall__Goomba;
extern dStateBase_c StateID_DieOther__Goomba;
extern dStateBase_c StateID_Walk__Goomba;

int daEnGoombaTower_c::onExecute() {

	//OSReport("Goomba 0 x,y,z: %f, %f, %f\n", this->goombas[0]->pos.x, this->goombas[0]->pos.y, this->goombas[0]->pos.z);

	OSReport("Goomba alive: %d, %d, %d, %d, %d, %d, %d, %d\n", goombas[0], goombas[1], goombas[2], goombas[3], goombas[4], goombas[5], goombas[6], goombas[7]);
	//OSReport("Goomba state: %d, %d, %d, %d, %d, %d, %d, %d\n", goombas[0]->acState.getCurrentState()->getName(), goombas[1]->acState.getCurrentState()->getName(), goombas[2]->acState.getCurrentState()->getName(), goombas[3]->acState.getCurrentState()->getName(), goombas[4]->acState.getCurrentState()->getName(), goombas[5]->acState.getCurrentState()->getName(), goombas[6]->acState.getCurrentState()->getName(), goombas[7]->acState.getCurrentState()->getName());
	OSReport("Goomba rot: %f, %f, %f\n", goombas[3]->rot.x, goombas[3]->rot.y, goombas[3]->rot.z);

	dStageActor_c* player = (dStageActor_c*)FindActorByType(PLAYER, 0);

	OSReport("StackSize: %d\n", this->stackSize);

	OSReport("111111111111111\n");

	if (this->starting) //&& player->pos.x >= this->pos.x - 128 && player->pos.x <= this->pos.x + 128)
	{
		OSReport("222222222222222222222222222222222\n");
		Vec3 goombaPos;
		OSReport("2.55555555555555555555555\n");
		for (int i = 0; i < this->stackSize; i++)
		{
			OSReport("333333333333333333333333333333333\n");
			if (this->goombas[i] == 0)
			{
				OSReport("4444444444444444444444\n");
				for (int j = i + 1; j < this->stackSize; j++)
				{
					OSReport("5555555555555555555555555555555555\n");
					this->goombas[j - 1] = this->goombas[j];
				}
			}
			OSReport("2266666666666666666666666666666662\n");
			goombaPos.x = this->pos.x;
			OSReport("2222777777777777777777777777777777\n");
			goombaPos.y = this->pos.y + (16 * i);
			OSReport("28888888888888888888888888888888888888\n");
			goombaPos.z = this->pos.z;
			OSReport("2229999999999999999999999999999999992\n");
			this->goombas[i] = (dStageActor_c*)CreateActor(51, 0, goombaPos, 0, 0);
			OSReport("222221010110101010101010101010\n");
		}
		starting = false;

		goombaTurn = goombas[0]->direction;
	
		u8 nulloombas = 0;
		for (int i = 1; i < this->stackSize; i++)
		{
			if (this->goombas[i])
			{
				nulloombas += 1;
			}
			else
			{
				this->goombas[i]->pos.x = this->goombas[0]->pos.x;
				this->goombas[i]->pos.y = this->goombas[0]->pos.y + (17 * (i - nulloombas));
				this->goombas[i]->pos.z = this->goombas[0]->pos.z - (500 * (i - nulloombas));

				this->goombas[i]->speed.x = this->goombas[0]->speed.x;
				this->goombas[i]->speed.y = this->goombas[0]->speed.y;
				this->goombas[i]->speed.z = this->goombas[0]->speed.z;

				this->goombas[i]->direction = this->goombas[0]->direction;
				this->goombasDirection[i] = this->goombas[i]->direction;
			}
		}
	}

	if (!starting)
	{
		bool hasAlreadyTurned = false;
		for (int i = 0; i < this->stackSize; i++)
		{
			OSReport("1 Goomba %d: %f, %f, %d \n", i, this->goombas[i]->rot.x, this->goombas[i]->rot.y, this->goombas[i]->direction);

			turnstate = ((dEn_c*)this->goombas[i])->acState.getCurrentState();

			if (turnstate == &StateID_DieFall__Goomba || turnstate == &StateID_DieOther__Goomba || turnstate == &StateID_Ice__Goomba)
			{
				this->goombas[i] = 0;

				/* OSReport("2 Goomba %d: %f, %f \n", i, this->goombas[i]->rot.x, this->goombas[i]->rot.y);
				this->goombas[i]->scale.x = 0;
				this->goombas[i]->scale.y = 0;
				this->goombas[i]->scale.z = 0;

				OSReport("3 Goomba %d: %f, %f \n", i, this->goombas[i]->rot.x, this->goombas[i]->rot.y);

				dStageActor_c* goombaTemp = this->goombas[i];

				OSReport("4 Goomba %d: %f, %f \n", i, this->goombas[i]->rot.x, this->goombas[i]->rot.y);

				for (int j = i + 1; j <= this->stackSize; j++)
				{
					OSReport("5 Goomba %d, %d: %f, %f \n", i, j, this->goombas[i]->rot.x, this->goombas[i]->rot.y);

					if (this->goombas[j]->scale.x == 0 || j == this->stackSize)
					{
						OSReport("6 Goomba %d, %d: %f, %f \n", i, j, this->goombas[i]->rot.x, this->goombas[i]->rot.y);
						this->goombas[j - 1] = goombaTemp;
					}
					else
					{
						OSReport("7 Goomba %d, %d: %f, %f \n", i, j, this->goombas[i]->rot.x, this->goombas[i]->rot.y);
						this->goombas[j - 1] = this->goombas[j];
					}
				} */
			}
			else if(!hasAlreadyTurned)
			{
				bool wall = this->goombas[i]->collMgr.calculateAdjacentCollision(0);

				OSReport("Pi Goomba %d: %d \n", i, wall);

				if (wall)
				{
					hasAlreadyTurned = true;

					((dEn_c*)this->goombas[0])->doStateChange((dStateBase_c*)&StateID_Turn__Goomba);

					//this->goombas[0]->direction *= -1;
				}
			}
		}


		u8 nulloombas = 0;
		for (int i = 0; i < this->stackSize; i++)     //i auf 0 ge�ndert
		{
			if (this->goombas[i] == 0)
			{
				nulloombas += 1;
			}
			else
			{
				this->goombas[i]->pos.x = this->goombas[0]->pos.x;
				this->goombas[i]->pos.y = this->goombas[0]->pos.y + (17 * (i - nulloombas));
				this->goombas[i]->pos.z = this->goombas[0]->pos.z - (500 * (i - nulloombas));

				this->goombas[i]->speed.x = this->goombas[0]->speed.x;
				this->goombas[i]->speed.y = this->goombas[0]->speed.y;
				this->goombas[i]->speed.z = this->goombas[0]->speed.z;

				this->goombas[i]->direction = this->goombas[0]->direction;

				/*if (this->goombasDirection[i] != this->goombas[i]->direction)
				{
					for (int j = 0; i < this->stackSize; i++)
					{
						//this->goombas[j]->direction = this->goombas[i]->direction;
						//this->goombasDirection[j] = this->goombas[i]->direction;
						((dEn_c*)this->goombas[j])->acState.setState((dStateBase_c*)StateID_Turn__Goomba);
					}
				}*/
			}

			turnstate = ((dEn_c*)this->goombas[0])->acState.getCurrentState();

			//OSReport("State: %p, %p\n", turnstate, &StateID_Turn__Goomba);
			if (turnstate == &StateID_Turn__Goomba) OSReport("A Goomba Turn\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

		}

		//if (goombas[0] != 0 && goombas[0]->direction != goombaTurn) {
			//turnstate = ((dEn_c*)this->goombas[0])->acState.getCurrentState();

			
			//OSReport("Goomba State %s: %p \n", turnstate->getName(), turnstate);
		//}


		if (goombas[0] != 0)
		{
			this->pos.x = goombas[0]->pos.x;
			this->pos.y = goombas[0]->pos.y;
			this->pos.z = goombas[0]->pos.z;

		}
		else
		{
			this->Delete(1);
		}

	}

	return true;
}