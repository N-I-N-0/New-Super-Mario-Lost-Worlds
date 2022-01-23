#include <game.h>
#include "daEnShell_c.h"




/*
- make sure no walking sounds can be heard
- only take damage when shell is destroyed
- add check that the shell pointers are valid
- fix rotate on wall collision
- prevent players from making damage to controlled shells?


*/


#define MIN_SHELL_SPEED 2.0f
#define MAX_SHELL_SPEED 4.5f
#define CHG_SHELL_SPEED 0.025f

CREATE_STATE(daPlBase_c, ShellConnect);

daEnShell_c* blueShells[4];	//blue shell pointers
int blueShellTimers[4];		//blue shell timers

void daPlBase_c::beginState_ShellConnect() {
	this->setFlag(0xBB);	//makes invisible
	//this->setFlag(0x25);	//propeller indicator?
	//this->aPhysics.removeFromList();
	//ActivePhysics* ap1 = (ActivePhysics*)((int)this+4452);
	//ActivePhysics* ap1 = (ActivePhysics*)((int)this+4452);
	//this->setFlag(0x71);
	this->useDemoControl();
	blueShellTimers[this->settings % 4] = 0;
	daEnShell_c* shell = blueShells[this->settings % 4];
	shell->speed.x = MIN_SHELL_SPEED + abs(this->speed.x / 2);
	if(shell->speed.x > MAX_SHELL_SPEED) {
		shell->speed.x = MAX_SHELL_SPEED;
	}
	if(this->rot.y < 0) {
		shell->speed.x *= -1;
	}
	shell->doStateChange(&daEnShell_c::StateID_Slide);
}

void daPlBase_c::executeState_ShellConnect(){
	daEnShell_c* shell = blueShells[this->settings % 4];
	Vec bindPos = shell->pos;
	bindPos.y -= 4;
	this->pos = bindPos;
	
	if(blueShellTimers[this->settings % 4] < 90) {
		blueShellTimers[this->settings % 4]++;
	} else {
		Remocon* con = GetRemoconMng()->controllers[this->settings % 4];
		if(con->isShaking != 0) {
			this->states2.setState(&daPlBase_c::StateID_Jump);
			shell->Delete(1);
		}
		
		if(con->nowPressed & WPAD_TWO && shell->collMgr.isOnTopOfTile()) { //add is on ground check here
			shell->speed.y = 6.0f;
		}
		
		if(con->heldButtons & WPAD_LEFT) {
			if(shell->speed.x > 0) {
				if(shell->speed.x > MIN_SHELL_SPEED) {
					shell->speed.x -= CHG_SHELL_SPEED;
				}
			} else {
				if(shell->speed.x > -MAX_SHELL_SPEED) {
					shell->speed.x -= CHG_SHELL_SPEED;
				}
			}
		}

		if(con->heldButtons & WPAD_RIGHT) {
			if(shell->speed.x > 0) {
				if(shell->speed.x < MAX_SHELL_SPEED) {
					shell->speed.x += CHG_SHELL_SPEED;
				}
			} else {
				if(shell->speed.x < -MIN_SHELL_SPEED) {
					shell->speed.x += CHG_SHELL_SPEED;
				}				
			}
		}

	}
}

void daPlBase_c::endState_ShellConnect() {
	this->clearFlag(0xBB);
	//this->clearFlag(0x71);
	this->disableDemoControl();
}



extern "C" dStageActor_c *Actor_SearchByID(u32 actorID);
void setPlayerStateBlueShell(dAcPy_c* player) {			//setPlayerStateBlueShell__FP7dAcPy_c
	dStageActor_c* carrying = Actor_SearchByID(*(u32*)((u32)(player)+0x2A78)); //Actor_SearchByID(player->idWeAreCarrying)
	//OSReport("ID: %d, Pointer: %p, Name: %d\n", player->idWeAreCarrying, carrying, carrying->name);
	if (carrying->name == 54 || carrying->name == 55) {	//if Koopa or Parakoopa Shell
		blueShells[player->settings % 4] = (daEnShell_c*)carrying;
		player->states2.setState(&daPlBase_c::StateID_ShellConnect);
	}
}








