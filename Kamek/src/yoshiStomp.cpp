#include <common.h>
#include <game.h>
#include <g3dhax.h>
#include <sfx.h>
#include "yoshiquick.h"

int blueTime = 720;			//Time on which blue yoshi keeps a shell in his mouth if he ate one
int yellowTime = 960;		  //Time on which yellow yoshi keeps a shell in his mouth if he ate one
int doneRefreshingJauges;		//Global variable for the gameScene. For some reasons, making it a bool crashes the game

extern "C" float pow(float num, float power);

/*int getNybbleValue(u32 settings, int fromNybble, int toNybble) {	//This function is to get a nybble from a sprite's settings. It's probably more optimized to do the shifts and the AND by hand, but i prefer to prioritise the reading than the optimization:
	int numberOfNybble = (toNybble  - fromNybble) + 1;				//-Gets how many nybbles are used for the process (example: nybbles 4-6 -> there's nybbles 4, 5 and 6 used -> numberOfNybble = 3) 
	int valueToUse = 48 - (4 * toNybble);							//-Gets the value to use with the bitshift at the end 
	int fShit = pow(16, numberOfNybble) - 1;						//-Gets the value to use with the "&" operator at the end
	return ((settings >> valueToUse) & fShit);						//-Uses everything to make the nybble value, and return it
}*/

/*class dGameDisplay_c : dBase_c {	//The gameScene layout's class
	public:
		m2d::EmbedLayout_c layout;
		mEf::es2 effect;
		u32 _330;
		u32 _334;
		u32 _338;
		u32 _33C;
		u32 _340;
		u32 _344;
		u32 _348;
		u32 _34C;
		u32 _350;
		u32 _354;
		u32 _358;
		u32 _35C;
		u32 _360;
		u32 _364;
		u32 _368;
		u32 _36C;
		u32 _370;
		u32 _374;
		u32 _378;
		u32 _37C;
		u32 _380;
		u32 _384;
		u32 _388;
		u32 _38C;
		u32 _390;
		u32 _394;
		u32 _398;
		u32 _39C;
		u32 _3A0;
		u32 _3A4;
		dStateMgr_c state;
		u32 _3CC;
		u32 _3D0;
		u32 _3D4;
		u32 _3D8;
		u32 coins;
		u32 timer;
		u32 _3E4;
		u32 score;
		u32 _3EC;
		u32 _3F0;
		u32 _3F4;
		u32 _3F8;
		u32 _3FC;
		u32 _400;
		u32 _404;
		u32 _408;
		u32 _40C;
		u32 _410;
		u32 _414;
		u32 _418;
		u32 _41C;
		u32 _420;
		u32 _424;
		u32 _428;
		u32 _42C;
		u32 _430;
		u32 mustAtLeast2ForScoreToCount;
		u32 _438;
		u32 _43C;
		u32 _440;
		u32 _444;
		u8 _448;
		u8 _449;
		u8 _44A;
		u8 _44B;
		u32 _44C;
		u8 _450;
		u8 _451;
		u8 _452;
		u8 _453;
		u8 _454;
		u8 _455;
		u8 _456;
		u8 _457;
		u32 _458;
		u32 _45C;
		u32 _460;
		u32 _464;
		u32 _468;
		u32 _46C;
		u32 _470;
		u32 _474;
		u32 _478;
		u32 _47C;
		u32 _480;
		u32 _484;
		u32 _488;
		u32 _48C;
		u32 _490;
		nw4r::lyt::Picture* p_collectOff_00;
		nw4r::lyt::Picture* p_collection_00;
		nw4r::lyt::Picture* p_collectOff_01;
		nw4r::lyt::Picture* p_collection_01;
		nw4r::lyt::Picture* p_collectOff_02;
		nw4r::lyt::Picture* p_collection_02;
		nw4r::lyt::Picture* p_marioIcon_00;
		nw4r::lyt::Picture* p_luijiIcon_00;
		nw4r::lyt::Picture* p_kinoB_00;
		nw4r::lyt::Picture* p_kinoY_00;
		u32 _4BC;
		u32 _4C0;
		u32 _4C4;
		u32 _4C8;
		u32 _4CC;
		u32 _4D0;
		u32 _4D4;
		u32 _4D8;
		nw4r::lyt::TextBox* coinBox;
		nw4r::lyt::TextBox* timerBox;
		nw4r::lyt::TextBox* scoreBox;
		u32 _4E8;
		u32 _4EC;
		u32 _4F0;
		u32 _4F4;
		u8  _4F8;
		void* StarCoin1;
		void* StarCoin2;
		void* StarCoin3;
		u8  _505;
		u8  _506;
		u8  _507;
		u8 blob1[0x51C - 0x508];
		u32 _51C;
		u8 blob2[0x5A0 - 0x508];
		u32 _5A0;
	   
		static dGameDisplay_c* instance;
		int onCreate_orig();
		int onExecute_orig();
		int newOnCreate();
		int newOnExecute();
};*/
/*
int dGameDisplay_c::newOnCreate() {
	int orig_val = this->onCreate_orig();	//Execute the original onCreate

	doneRefreshingJauges = 0;				//Setting the jauge refreshing trigger to 0 (false) [for some reasons, hiding the jauges here crashes, so doing it in the onExecute using this value to only do it once]

	return orig_val;						//Return the original onCreate
}

int dGameDisplay_c::newOnExecute() {
	int orig_val = this->onExecute_orig();									//Execute the original onExecute

	if(!doneRefreshingJauges) {												//If the jauge refreshing wasn't done yet:
		nw4r::lyt::Picture *jaugeCast;										//-Create the jauge cast pointer
		nw4r::lyt::Picture *jaugeYellow;									//-Create the yellow jauge pointer
		nw4r::lyt::Picture *jaugeBlue;										//-Create the blue jauge pointer

		jaugeCast = this->layout.findPictureByName("P_jaugeCast_00");		//-Store the jauge cast
		jaugeYellow = this->layout.findPictureByName("P_yellowJauge_00");	//-Store the yellow jauge
		jaugeBlue = this->layout.findPictureByName("P_blueJauge_00");		//-Store the blue jauge

		jaugeCast->SetVisible(false);										//-Make the jauge cast invisible
		jaugeYellow->SetVisible(false);										//-Make the yellow jauge invisible
		jaugeBlue->SetVisible(false);										//-Make the blue jauge invisible
		doneRefreshingJauges = 1;											//-Set the "are we done refreshing the jauges" to true
	}

	return orig_val;														//Return the original onExecute
}

void showJauges(bool isYellow) {
	nw4r::lyt::Picture *jaugeCast = dGameDisplay_c::instance->layout.findPictureByName("P_jaugeCast_00");										//Store the jauge cast
	nw4r::lyt::Picture *jaugeColor = dGameDisplay_c::instance->layout.findPictureByName(((isYellow) ? "P_yellowJauge_00" : "P_blueJauge_00"));	//Store the correct jauge
	jaugeCast->SetVisible(true);																												//Make the jauge cast invisible
	jaugeColor->SetVisible(true);																												//Make the correct jauge invisible
}

void hideJauges() {
	nw4r::lyt::Picture *jaugeCast = dGameDisplay_c::instance->layout.findPictureByName("P_jaugeCast_00");		//Store the jauge cast
	nw4r::lyt::Picture *jaugeYellow = dGameDisplay_c::instance->layout.findPictureByName("P_yellowJauge_00");	//Store the yellow jauge
	nw4r::lyt::Picture *jaugeBlue = dGameDisplay_c::instance->layout.findPictureByName("P_blueJauge_00");		//Store the blue jauge
	jaugeCast->SetVisible(false);																				//Make the jauge cast invisible
	jaugeYellow->SetVisible(false);																				//Make the yellow jauge invisible
	jaugeBlue->SetVisible(false);																				//Make the blue jauge invisible
}*/

int daYoshi2_c::newOnExecute() { //demoStateTracker (1418), states2 (1464)
	/*******************************/
	/*Execute the original onCreate*/
	/*******************************/
	int orig_val = this->onExecute_orig();

	/*****************/
	/*State Debugging*/
	/*****************/
	/*nw4r::lyt::TextBox *stateText = dGameDisplay_c::instance->layout.findTextBoxByName("T_state_00");	//Store the debugging TextBox Pane in stateText

	wchar_t stupidTextW[100];																			//Make a wchar_t to store the future state name
	const char *stupidText = this->states2.getCurrentState()->getName();								//Get the current state as a const char*
	int charCount = 0;																					//Create the current char count
	
	while (*stupidText != 0 && charCount < 99) {														//Convert the const char* to the wchar_t:
		stupidTextW[charCount] = *stupidText;															//-Set the current char from the wchar_t to the one from the const char*
		stupidText++;																					//-Increment the const char* count
		charCount++;																					//-Increment the current char count
	}																									//-Repeat the process until all the chars from 0 to 98 is converted
	stupidTextW[charCount] = 0;																			//-Set the last char (99) of the wchar_t to 0

	stateText->SetString(stupidTextW);																	//Insert the wchar_t into the debugging TextBox Pane (stateText)

	/*****************/
	/*Jauges Updating*/
	/*****************/
	/*nw4r::lyt::Picture *shellJauge = dGameDisplay_c::instance->layout.findPictureByName((getNybbleValue(this->settings, 12, 12) == 2) ? "P_yellowJauge_00" : "P_blueJauge_00"); //Store the Picture Pane of the jauge to use in shellJauge
	*/
	u32 color = getNybbleValue(this->settings, 12, 12);
	if(getNybbleValue(this->settings, 5, 5)) {							//If nybble 5 (is there a shell in yoshi's mouth) is triggered, then:
		if(!this->isTimerEnabled) {										//-If the swallow timer isn't enabled yet:
			this->isTimerEnabled = true;								//--Enable it
			S16Vec nullRot = {0,0,0};
			if(color == 4 || color == 7) {
				dStageActor_c *wings = create(EN_YOSHIWINGS, this->id, &this->pos, &nullRot, 0);
				this->wingID = wings->id;
			}
		}
		if(!this->doOneTime) {											//-If not done yet:
			//showJauges((getNybbleValue(this->settings, 12, 12) == 2));	//--Show the correct jauge depending of yoshi's color (yellow or blue)
			this->swallowTimer = ((color == 2 || color == 3) ? yellowTime : blueTime);						//--Set the time before swallowing the shell
			//shellJauge->size.y = 49.0;									//--Set the correct jauge's maximum scale
			this->doOneTime = true;										//--Set the check to "done"
		}
	}
	else {																//Otherwise:
		if(this->isTimerEnabled) {										//-If the swallow timer is enabled:
			this->isTimerEnabled = false;								//--Disable it
			this->doOneTime = false;									//--Set the check to "not done yet"
			if(color == 4 || color == 7) {
				Actor_SearchByID(this->wingID)->Delete(1);
			}
			//hideJauges();												//--Hide the jauges
		}
	}

	if(this->isTimerEnabled && this->swallowTimer > 0) {				//If the swallow timer is enabled and the swallow timer is greater than 0:
		this->swallowTimer--;											//-Decrease the swallow timer by 1
		//shellJauge->size.y -= (49.0f / ((getNybbleValue(this->settings, 12, 12) == 2) ? yellowTime : blueTime));				//-Decrease the correct jauge's Y Scale by its rate (its maximum scale divided by the waiting frames until swallowing the shell)
	}
	if(this->isTimerEnabled && this->swallowTimer <= 0) {				//If the swallow timer is enabled and the swallow timer is less or equal to 0:
		this->eatenID = 0;												//-Remove the current eaten shell from yoshi's mouth
		this->settings ^= 0x10000000;									//-Untrigger nybble 5 (is there a shell in yoshi's mouth)
	}

	/**************************************/
	/*Return the original onCreate's value*/
	/**************************************/
	return orig_val;
}

void daYoshi2_c::newEndState_HipAttack() {
	endState_HipAttack_orig();	//Execute the original endState_HipAttack
	this->doneGP = false;		//Untrigger the groundpounding check
}

extern void ewhipAttackStage4();
void daPlBase_c::newHipAttackStage4() {
	ewhipAttackStage4();																												//Execute the original hipAttackStage4
	// OSReport("hipAttackStage4 %08X\n", this->which_player);																			//Debugging
	daYoshi2_c *CuteYoshi = (daYoshi2_c*)dAcPy_c::findByID(this->which_player)->getYoshi();															//Get the current yoshi
	// OSReport("meh %08X\n", CuteYoshi->settings);																						//Debugging
	if(!CuteYoshi) return;
	if(CuteYoshi->settings != 0) {																										//If its settings aren't null (so the yoshi exists):
		u32 color = getNybbleValue(CuteYoshi->settings, 12, 12);
		if((color == 2 || color == 3) && getNybbleValue(CuteYoshi->settings, 5, 5) && !CuteYoshi->doneGP) {								//-If this yoshi is yellow or green, has a shell in his mouth and the groundpounding check isn't triggered:
			S16Vec nullRot = {0,0,0};																									//--Set the ongoing effect's rotation to 0 on all axes
			VEC3 vecOne = {0.2f, 0.2f, 0.2f};																							//--Set the ongoing effect's scale to 0.2 on all axes
			SpawnEffect("Wm_ob_powdown_ind", 0, &CuteYoshi->pos, &nullRot, &vecOne);													//--Create a POW effect with the rotation and scale from above
			nw4r::snd::SoundHandle powHandle;																							//--Create a SoundHandle for the ongoing SFX
			PlaySoundWithFunctionB4(SoundRelatedClass, &powHandle, SE_OBJ_POW_BLOCK_QUAKE, 1);											//--Play a POW SFX in the above SoundHandle
	
			dActor_c *iter = 0;																											//--Create an actor pointer
			while(iter = (dActor_c*)dActor_c::searchByBaseType(2, iter)) {																//--For every actor found by the searchByBaseType function:
				dEn_c *sa = (dEn_c*)iter;																								//---Convert it to a dStageActor_c
	
				if((sa->name == EN_KURIBO || sa->name == EN_PATA_KURIBO ||																//---If it's a goomba, or a paragoomba,
					sa->name == EN_NOKONOKO || sa->name == EN_PATAPATA ||																//---or a koopa, or a parakoopa,
					sa->name == EN_MET || sa->name == EN_TOGEZO || sa->name == EN_SAKASA_TOGEZO ||										//---or a buzzy beetle, or a spiny, or an upside-down spiny,
					sa->name == EN_JIMEN_PAKKUN || sa->name == EN_UP_DOKAN_PAKKUN ||													//---or a piranha plant, or an UPPipe piranha plant,
					sa->name == EN_DOWN_DOKAN_PAKKUN || sa->name == EN_RIGHT_DOKAN_PAKKUN ||											//---or a DOWNPipe piranha plant, or a RIGHTPipe piranha plant,
					sa->name == EN_LEFT_DOKAN_PAKKUN || sa->name == EN_UP_DOKAN_FPAKKUN ||												//---or a LEFTPipe piranha plant, or an UPPipe fire piranha plant,
					sa->name == EN_DOWN_DOKAN_FPAKKUN || sa->name == EN_RIGHT_DOKAN_FPAKKUN ||											//---or a DOWNPipe fire piranha plant, or a RIGHTPipe fire piranha plant,
					sa->name == EN_LEFT_DOKAN_FPAKKUN ||																				//---or a LEFTPipe fire piranha plant,
					sa->name == EN_JIMEN_FPAKKUN ||																						//---or a fire piranha plant,
					sa->name == EN_WATERPAKKUN || sa->name == EN_WALK_PAKKUN) && CuteYoshi->eatenID != sa->id) {						//---or a river piranha plant, or a walking piranha plant, and it's not the shell currently in yoshi's mouth:
					int range = 64; //one tile = 16																						//----Set the range to 64 (4 tiles)
					if(sa->pos.x < (CuteYoshi->pos.x + range) && sa->pos.x > (CuteYoshi->pos.x - range)) {								//----If the actor is in the range in the X axis:
						if(sa->pos.y < (CuteYoshi->pos.y + range) && sa->pos.y > (CuteYoshi->pos.y - range)) {							//-----If the actor is in the range in the Y axis:
							sa->collisionCat13_Hammer(&sa->aPhysics, &CuteYoshi->aPhysics);												//------Kill it like it it was touched by a hammer
						}
					}
				}
			}
			CuteYoshi->doneGP = true;																									//--Trigger the groundpounding check
		}
	}
}