#include <common.h>
#include <game.h>
#include <profile.h>
#include <stage.h>
#include "baddy.h"

const char* PullStarManagerFileList[] = { "launchStar", 0 };

class daEnPullStarManager_c : public dEn_c {
public:
	int onCreate();
	int onExecute();
	int onDelete();
	
	daEnPullStar_c* closestPull[4][8];
	daEnPullStar_c* selectedPullStar[4];
	
	static dActor_c* build();

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

const SpriteData PullStarManagerSpriteData = { ProfileId::PullStarManager, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile PullStarManagerProfile(&daEnPullStarManager_c::build, SpriteId::PullStarManager, &PullStarManagerSpriteData, ProfileId::PullStarManager, ProfileId::PullStarManager, "PullStarManager", PullStarManagerFileList);

u8 hijackMusicWithSongName(const char* songName, int themeID, bool hasFast, int channelCount, int trackCount, int* wantRealStreamID);

void daEnPullStarManager_c::playerCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	
}
void daEnPullStarManager_c::yoshiCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	//this->playerCollision(apThis, apOther);
}
bool daEnPullStarManager_c::collisionCat7_GroundPound(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnPullStarManager_c::collisionCat7_GroundPoundYoshi(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnPullStarManager_c::collisionCatD_Drill(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnPullStarManager_c::collisionCatA_PenguinMario(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnPullStarManager_c::collisionCat1_Fireball_E_Explosion(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnPullStarManager_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnPullStarManager_c::collisionCat9_RollingObject(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnPullStarManager_c::collisionCat13_Hammer(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnPullStarManager_c::collisionCat14_YoshiFire(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}
bool daEnPullStarManager_c::collisionCat3_StarPower(ActivePhysics* apThis, ActivePhysics* apOther) {
	return false;
}

dActor_c* daEnPullStarManager_c::build() 
{
	void* buffer = AllocFromGameHeap1(sizeof(daEnPullStarManager_c));
	daEnPullStarManager_c* c = new(buffer) daEnPullStarManager_c;

	return c;
}

extern int getNybbleValue(u32 settings, int fromNybble, int toNybble);

int daEnPullStarManager_c::onCreate() 
{
	OSReport("Starting daEnPullStarManager_c::onCreate()\n");
	
	for(int i = 0; i < 4; i = i + 1)
	{
		playerStatus[i] = 0;
		dragged[i] = false;
	}
	
	OSReport("Initialising PullMngr\n");

	dAcPy_c* player = (dAcPy_c*)FindActorByType(PLAYER, 0);

	for (int i = 0; i < GetActivePlayerCount(); i++)
	{
		lastPos[i].x = player->pos.x;
		lastPos[i].y = player->pos.y;
		lastPos[i].z = player->pos.z;

		player = (dAcPy_c*)FindActorByType(PLAYER, (Actor*)player);
	}

	OSReport("finished Initialising PullMngr\n");

	this->onExecute();
	return true;
}

int daEnPullStarManager_c::onDelete() 
{
	return true;
}

int daEnPullStarManager_c::onExecute()
{
	daPlBase_c* player = (daPlBase_c*)FindActorByType(PLAYER, 0);
	int currentDirection = -1;
	
	this->pos.x = player->pos.x;
	this->pos.y = player->pos.y;
	
	for (int i = 0; i < GetActivePlayerCount(); i++)
	{
		daEnPullStar_c* closestSearch = (daEnPullStar_c * )FindActorByType(PullStar, 0);
		
		if (GetRemoconMng()->controllers[i]->nowPressed & WPAD_B)
		{
			while (closestSearch != 0)
			{
				OSReport("Start search\n");
				
				if(selectedPullStar[i] != closestSearch)
				{
					float dx = closestSearch->pos.x - player->pos.x;
					float dy = (closestSearch->pos.y - player->pos.y);                           //(player->aPhysics.info.yDistToEdge + player->aPhysics.info.yDistToCenter)) - player->pos.y;
					float r = ((float)closestSearch->pullScale / 5.0f) * 512.0f;
					
					if(sqrtf((dx * dx) + (dy * dy)) > 1  && (dx * dx) + (dy * dy) <= (r * r) && closestSearch->active)
					{
						OSReport("Type is 1\n");
						
						OSReport("for player: %d\n", i);
						
						if(dx * tan(112.5 * M_PI / 180) < dy && dy >= dx * tan(67.5 * M_PI / 180))
						{
							currentDirection = 0;
						}
						else if(dx * tan(67.5 * M_PI / 180) > dy && dy >= dx * tan(22.5 * M_PI / 180))
						{
							currentDirection = 1;
						}
						else if(dx * tan(22.5 * M_PI / 180) > dy && dy >= dx * tan(-22.5 * M_PI / 180))
						{
							currentDirection = 2;
						}
						else if(dx * tan(-22.5 * M_PI / 180) > dy && dy >= dx * tan(-67.5 * M_PI / 180))
						{
							currentDirection = 3;
						}
						else if(dx * tan(-67.5 * M_PI / 180) > dy && dy <= dx * tan(-112.5 * M_PI / 180))
						{
							currentDirection = 4;
						}
						else if(dx * tan(-112.5 * M_PI / 180) < dy && dy <= dx * tan(-157.5 * M_PI / 180))
						{
							currentDirection = 5;
						}
						else if(dx * tan(-157.5 * M_PI / 180) < dy && dy <= dx * tan(-202.5 * M_PI / 180))
						{
							currentDirection = 6;
						}
						else if(dx * tan(-202.5 * M_PI / 180) < dy && dy <= dx * tan(-247.5 * M_PI / 180))
						{
							currentDirection = 7;
						}
						else
						{
							currentDirection = -1;
						}
						
						if(currentDirection >= 0)
						{
							if (closestPull[i][currentDirection] != 0)
							{
								OSReport("closest not null\n");

								float dxc = closestPull[i][currentDirection]->pos.x - player->pos.x;
								float dyc = (closestPull[i][currentDirection]->pos.y - player->pos.y);                              //- (player->aPhysics.info.yDistToEdge + player->aPhysics.info.yDistToCenter)) - player->pos.y;

								if (((dx * dx) + (dy * dy)) < ((dxc * dxc) + (dyc * dyc)))
								{
									closestPull[i][currentDirection] = closestSearch;
								}
							}
							else
							{
								OSReport("Closest null\n");

								closestPull[i][currentDirection] = closestSearch;
							}
						}
						currentDirection = -1;
					}
				}

				OSReport("searching for next Launch star Sprite\n");

				closestSearch = (daEnPullStar_c*)FindActorByType(PullStar, (Actor*)closestSearch);
				currentDirection = -1;

				OSReport("Found Launch star sprite\n");
			}
			
			for(int c = 0; c < 8; c++)
			{
				if(closestPull[i][c] == 0)
				{
					closestPull[i][c] == selectedPullStar[i];
				}
			}
			
			if(GetRemoconMng()->controllers[i]->heldButtons & WPAD_UP)
			{
				if(GetRemoconMng()->controllers[i]->heldButtons & WPAD_LEFT)
				{
					selectedPullStar[i] = closestPull[i][7];
				}
				else if(GetRemoconMng()->controllers[i]->heldButtons & WPAD_RIGHT)
				{
					selectedPullStar[i] = closestPull[i][1];
				}
				else
				{
					selectedPullStar[i] = closestPull[i][0];
				}
			}
			else if(GetRemoconMng()->controllers[i]->heldButtons & WPAD_RIGHT)
			{
				if(GetRemoconMng()->controllers[i]->heldButtons & WPAD_DOWN)
				{
					selectedPullStar[i] = closestPull[i][3];
				}
				else
				{
					selectedPullStar[i] = closestPull[i][2];
				}
			}
			else if(GetRemoconMng()->controllers[i]->heldButtons & WPAD_DOWN)
			{
				if(GetRemoconMng()->controllers[i]->heldButtons & WPAD_LEFT)
				{
					selectedPullStar[i] = closestPull[i][5];
				}
				else
				{
					selectedPullStar[i] = closestPull[i][4];
				}
			}
			else if(GetRemoconMng()->controllers[i]->heldButtons & WPAD_LEFT)
			{
				selectedPullStar[i] = closestPull[i][6];
			}
			else
			{
				for(int c = 0; c < 8; c++)
				{
					if(closestPull[i][c] != 0)
					{
						if (selectedPullStar[i] != 0)
						{
							//TODO Why the hell didn't I use the corrected y here?
							if((((selectedPullStar[i]->pos.x - player->pos.x) * (selectedPullStar[i]->pos.x - player->pos.x)) + ((selectedPullStar[i]->pos.y - player->pos.y) * (selectedPullStar[i]->pos.y - player->pos.y))) > (((closestPull[i][c]->pos.x - player->pos.x) * (closestPull[i][c]->pos.x - player->pos.x)) + ((closestPull[i][c]->pos.y - player->pos.y) * (closestPull[i][c]->pos.y - player->pos.y))))
							{
								selectedPullStar[i] = closestPull[i][c];
							}
						}
						else
						{
							selectedPullStar[i] = closestPull[i][c];
						}
					}
				}
			}
			
			for(int c = 0; c < 8; c++)
			{
				closestPull[i][c] = 0;
				closestSearch = 0;
			}
		}
		else if(GetRemoconMng()->controllers[i]->heldButtons & WPAD_B)
		{
			if (selectedPullStar[i] != 0)
			{
				OSReport("For LOOP\n");

				OSReport("Selected is active?: %d\n", selectedPullStar[i]->active);

				OSReport("selected is active\n");

				float selectedYPosCorrected = (selectedPullStar[i]->pos.y - (player->aPhysics.info.yDistToEdge + player->aPhysics.info.yDistToCenter));

				float dx = player->pos.x - selectedPullStar[i]->pos.x;
				float dy = player->pos.y - selectedPullStar[i]->pos.y;
				float r = ((float)selectedPullStar[i]->pullScale / 5.0f) * 512.0f;

				pullAngle[i].x = selectedPullStar[i]->pos.x - player->pos.x;
				pullAngle[i].y = selectedPullStar[i]->pos.y - player->pos.y;

				pullAngle[i].x /= 10 * selectedPullStar[i]->pullSpeed;
				pullAngle[i].y /= 10 * selectedPullStar[i]->pullSpeed;
				
				if(!player->collMgr.calculateAdjacentCollision())
				{
					player->pos.x = (lastPos[i].x + pullAngle[i].x);
				}
				else
				{
					player->pos.x = (lastPos[i].x);
				}
				
				if(pullAngle[i].y < 0)
				{
					if(!player->collMgr.calculateBelowCollision())
					{
						player->pos.y = (lastPos[i].y + pullAngle[i].y);
					}
					else
					{
						player->pos.y = (lastPos[i].y);
					}
				}
				else if(pullAngle[i].y > 0)
				{
					if(!player->collMgr.calculateAboveCollision(player->collMgr.outputMaybe))
					{
						player->pos.y = (lastPos[i].y + pullAngle[i].y);
					}
					else
					{
						player->pos.y = (lastPos[i].y);
					}
				}
				else
				{
					player->pos.y = (lastPos[i].y + pullAngle[i].y);
				}

				dragged[i] = true;

				lastPos[i].x = player->pos.x;
				lastPos[i].y = player->pos.y;				//if not here too the player would be jump over cause the continue
				lastPos[i].z = player->pos.z;

				player->useDemoControl();
				
				float speeed = pullAngle[i].x;
				
				player->moveInDirection(&(selectedPullStar[i]->pos.x), &pullAngle[i].x);            //is just here for setting the x speed
				
				player = (daPlBase_c*)FindActorByType(PLAYER, (Actor*)player);

				continue;
			}
		}
		if (dragged[i])
		{
			player->disableDemoControl();
		}

		dragged[i] = false;

		lastPos[i].x = player->pos.x;
		lastPos[i].y = player->pos.y;
		lastPos[i].z = player->pos.z;
		
		player = (daPlBase_c*)FindActorByType(PLAYER, (Actor*)player);
	}
}