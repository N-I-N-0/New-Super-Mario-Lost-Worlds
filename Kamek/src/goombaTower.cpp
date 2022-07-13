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
	
	dStateBase_c* lastFirstGoombaState;
	
	bool isAnyGoombaInView();
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
		OSReport("Creating goomba: %d\n", i);
		goombaPos.x = this->pos.x;
		goombaPos.y = this->pos.y + (17 * i);
		goombaPos.z = this->pos.z - (500 * i);
		this->goombas[i] = (dStageActor_c*)CreateActor(51, 0, goombaPos, 0, 0);
	}

	firstGoombaID = 0;

	for (int i = 1; i < this->stackSize; i++)
	{
		OSReport("setting up goomba: %d\n", i);
		this->goombas[i]->speed.x = this->goombas[0]->speed.x;
		this->goombas[i]->speed.y = this->goombas[0]->speed.y;
		this->goombas[i]->speed.z = this->goombas[0]->speed.z;

		this->goombas[i]->direction = this->goombas[0]->direction;
	}
	
	dStateBase_c* firstGoombaState = ((dEn_c*)this->goombas[firstGoombaID])->acState.getCurrentState();

    if(this->isOutOfView())
	{
		OSReport("out of view\n");
		for (int i = firstGoombaID; i < this->stackSize; i++)
		{
			OSReport("killing goomba: %d\n", i);
			
			((dEn_c*)this->goombas[i])->doStateChange(&StateID_DieFall__Goomba);
			
			this->goombas[i] = 0;
		}
	}		
	
	lastFirstGoombaState = firstGoombaState;

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

bool isGoombaStillThere(dStageActor_c* goombaIn)
{
	dStageActor_c* goomba = (dStageActor_c*)FindActorByType(EN_KURIBO, 0);
	
	while(goomba != 0)
	{
		if(goomba == goombaIn)
		{
			return true;
		}
		
		goomba = (dStageActor_c*)FindActorByType(EN_KURIBO, (Actor*)goomba);
	}
	
	return false;
}

bool daEnGoombaTower_c::isAnyGoombaInView()
{
	for(int i = 0; i < this->stackSize; i++)
	{
		if(goombas[i] != 0)
		{
			if(!goombas[i]->isOutOfView())
			{
				return true;
			}
		}
	}
	
	return false;
}

int daEnGoombaTower_c::onExecute()
{
	//TODO maybe include ice freeze variable too if I find a way to make that one work
	//TODO maybe change respawn behavior (when, with how many goombas with which condition etc.)
	//TODO maybe change/stop the waling animation of the goombas atop of another — if possible
	
	for(int i = 0; i < this->stackSize; i++)
	{
		if(goombas[i] != 0)
		{
			if(!isGoombaStillThere(goombas[i]))
			{
				goombas[i] = 0;
			}
		}
	}
	
	if(goombas[firstGoombaID] != 0 && isAnyGoombaInView())
	{
		dStateBase_c* firstGoombaState = ((dEn_c*)this->goombas[firstGoombaID])->acState.getCurrentState();

		u8 goombasAlive = 0;

		for (int i = firstGoombaID; i < this->stackSize; i++)
		{
			if (goombas[i] != 0)
			{
				dStateBase_c* goombaState = ((dEn_c*)this->goombas[i])->acState.getCurrentState();
				
				//OSReport("Goomba %d State: %s\n", i, goombaState->getName());
				//OSReport("Rotation: %f | %f | %f\n", goombas[i]->rot.x, goombas[i]->rot.y, goombas[i]->rot.z);
				
				if(goombaState->isEqual(&StateID_DieFall__Goomba) || goombaState->isEqual(&StateID_DieOther__Goomba) || goombaState->isEqual(&StateID_Ice__Goomba))
				{	
					goombas[i] = 0;
					
					if(i == firstGoombaID)
					{
						for(int j = firstGoombaID + 1; j < this->stackSize; j++)
						{
							if(goombas[j] != 0)
							{
								firstGoombaID = j;
								firstGoombaState = ((dEn_c*)this->goombas[firstGoombaID])->acState.getCurrentState();
								break;
							}
							else if(j == this->stackSize - 1)
							{
								firstGoombaID = 0;
								this->stackSize = 0;
								this->Delete(1);
								return true;
							}
						}
					}
					
					continue;
				}
				
				goombasAlive += 1;

				if(goombaState->isNotEqual(firstGoombaState))
				{
					if(goombaState->isNotEqual(&StateID_Turn__Goomba))
					{
						((dEn_c*)this->goombas[i])->doStateChange(firstGoombaState);
					}
					else if(goombaState->isEqual(&StateID_Turn__Goomba))
					{
						if(firstGoombaState->isNotEqual(&StateID_Turn__Goomba) && lastFirstGoombaState->isNotEqual(&StateID_Turn__Goomba))
						{
							for(int j = 0; j < this->stackSize; j++)
							{
								if(goombas[j] != 0)
								{
									if((((dEn_c*)this->goombas[j])->acState.getCurrentState())->isEqual(&StateID_Turn__Goomba) || (((dEn_c*)this->goombas[j])->acState.getCurrentState())->isEqual(&StateID_Walk__Goomba))
									{
										((dEn_c*)this->goombas[j])->doStateChange(&StateID_Turn__Goomba);
									}
								}
							}
							
							firstGoombaState = ((dEn_c*)this->goombas[firstGoombaID])->acState.getCurrentState();
						}
						else
						{
							for(int j = 0; j < this->stackSize; j++)
							{
								if(goombas[j] != 0)
								{
									((dEn_c*)this->goombas[j])->doStateChange(firstGoombaState);
								}
							}
						}
					}
				}
				
				if(goombas[i] != 0)
				{
					this->goombas[i]->rot.x = this->goombas[firstGoombaID]->rot.x;
					this->goombas[i]->rot.y = this->goombas[firstGoombaID]->rot.y;
					this->goombas[i]->rot.z = this->goombas[firstGoombaID]->rot.z;
				}
			}
		}
		
		for (int i = firstGoombaID; i < this->stackSize; i++)
		{	
			if(goombas[i] != 0)
			{
				dStateBase_c* goombaState = ((dEn_c*)this->goombas[i])->acState.getCurrentState();
				
				//OSReport("Goomba %d StateAfter: %s\n", i, goombaState->getName());
			}
		}

		int goombasToDo = goombasAlive;
		
		for (int i = firstGoombaID; i < this->stackSize; i++)
		{
			if (this->goombas[i] != 0)
			{
				this->goombas[i]->pos.x = this->goombas[firstGoombaID]->pos.x;
				this->goombas[i]->pos.y = this->goombas[firstGoombaID]->pos.y + (17 * (goombasAlive - goombasToDo));
				this->goombas[i]->pos.z = this->goombas[firstGoombaID]->pos.z - (500 * (goombasAlive - goombasToDo));

				this->goombas[i]->speed.x = this->goombas[firstGoombaID]->speed.x;
				this->goombas[i]->speed.y = this->goombas[firstGoombaID]->speed.y;
				this->goombas[i]->speed.z = this->goombas[firstGoombaID]->speed.z;
				
				goombasToDo -= 1;
			}
		}
		
		firstGoombaState = ((dEn_c*)this->goombas[firstGoombaID])->acState.getCurrentState();
		lastFirstGoombaState = firstGoombaState;
	}
	else
	{
		if(!this->isOutOfView())  
		{
			if(goombas[firstGoombaID] == 0)
			{
				this->stackSize = (this->settings >> 12 & 0xF) + 1;

				//OSReport("StackSize: %d\n", this->stackSize);

				Vec3 goombaPos;

				for (int i = 0; i < this->stackSize; i++)
				{
					goombaPos.x = this->pos.x;
					goombaPos.y = this->pos.y + (17 * i);
					goombaPos.z = this->pos.z - (500 * i);
					this->goombas[i] = (dStageActor_c*)CreateActor(51, 0, goombaPos, 0, 0);
				}

				firstGoombaID = 0;
				
				dStateBase_c* firstGoombaState = ((dEn_c*)this->goombas[firstGoombaID])->acState.getCurrentState();
				lastFirstGoombaState = firstGoombaState;

				for (int i = 1; i < this->stackSize; i++)
				{
					this->goombas[i]->speed.x = this->goombas[0]->speed.x;
					this->goombas[i]->speed.y = this->goombas[0]->speed.y;
					this->goombas[i]->speed.z = this->goombas[0]->speed.z;

					this->goombas[i]->direction = this->goombas[0]->direction;
				}
				
				this->onExecute();
			}
		}
		else
		{	
			if(goombas[firstGoombaID] != 0)
			{
				for (int i = firstGoombaID; i < this->stackSize; i++)
				{
					goombas[i]->Delete(1);
					goombas[i] = 0;
				}	
			}
		}
	}
	
	return true;
}