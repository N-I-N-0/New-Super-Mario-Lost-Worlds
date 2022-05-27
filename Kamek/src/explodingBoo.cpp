#include <game.h>


class daEnTeresa_c : dEn_c {
public:
    u8 unk1[0x7ab-0x524];
    mEf::es2 effect;
    ActivePhysics a2Physics;
    VEC2 field651_0x978;
    VEC2 field652_0x980;
    VEC2 field653_0x988;
    u8 unk2[0x9ab-0x990];
    int stop_counter;
    u8 unk3[0x9bf-0x9b0];
    int counter_1;
    int counter_3;
    u8 unk4[0x9cf-0x9c8];
    u32 is_big_boo;
    u32 nybble12;
    u32 nybble11;
    u8 unk5[0x9dd-0x9dc];
    s16 current_stop_go;
    s16 field703_0x9e0;
    s16 counter_2;
    u8 unk6[0x9e7-0x9e4];
	nw4r::snd::SoundHandle exploding_alarm;
	u32 warning_counter;
	bool doWarning;
	
	int checkExplosion();
	int origOnExecute();
	int newBooCreate();
	int origOnCreate();
	
	void getTargetPos(Point2d* vector);
	bool inExplosionRange(int);
	
	void beginState_ExplodeWarning();
	void executeState_ExplodeWarning();
	
	USING_STATES(daEnTeresa_c);
	DECLARE_STATE(Explode);
	REF_NINTENDO_STATE(Move);
};

CREATE_STATE(daEnTeresa_c, Explode);

int daEnTeresa_c::newBooCreate() {
	int ret = this->origOnCreate();

	doWarning = false;
	warning_counter = 0;
	*(u32*)&exploding_alarm = 0;
	
	
	return ret;
}

void daEnTeresa_c::beginState_Explode() {
	this->stop_counter = 0;
	this->scale = (Vec){0.0f, 0.0f, 0.0f};
	//PlaySoundAsync(this, SE_EMY_KERONPA_ALARM);

	PlaySoundAsync(this, SE_BOSS_JR_BOMB_BURST);
	SpawnEffect("Wm_en_explosion", 0, &this->pos, &(S16Vec){0,0,0}, &(Vec){5.0, 5.0, 5.0});
	SpawnEffect("Wm_ob_icepoison", 0, &this->pos, &(S16Vec){0,0,0}, &(Vec){3.5, 3.5, 3.5});
}

void daEnTeresa_c::executeState_Explode() {
	OSReport("Counter: %d\n", this->stop_counter);
	if (this->stop_counter < 20) {
		float add = (pow((this->stop_counter), 2))/100.0f + 1.5f;
		OSReport("add: %f\n", add);
		this->aPhysics.info.xDistToEdge += add;
		this->aPhysics.info.yDistToEdge += add;
	} else {
		if(exploding_alarm.Exists()) {
			exploding_alarm.Stop(1);
			doWarning = false;
		}
		this->Delete(this->deleteForever);
	}
	this->stop_counter++;
}

void daEnTeresa_c::endState_Explode() {}


void daEnTeresa_c::beginState_ExplodeWarning() {
	PlaySoundWithFunctionB4(SoundRelatedClass, &exploding_alarm, SE_EMY_KERONPA_ALARM, 1);
	exploding_alarm.SetVolume(2.0, 60);
	//SpawnEffect("Wm_ob_icepoison", 0, &this->pos, &(S16Vec){0,0,0}, &(Vec){1.5, 1.5, 1.5});
	this->warning_counter = 0;
}
void daEnTeresa_c::executeState_ExplodeWarning() {
	if(!this->inExplosionRange(80)) {
		if(exploding_alarm.Exists()) {
			exploding_alarm.Stop(1);
			doWarning = false;
		}
	}
	
	//OSReport("State size: 0x%X\n", sizeof(dStateWrapper_c<dActorState_c>));
	
	//this->executeState_Move();
	
	if(this->warning_counter > 180) {
		if(this->acState.getCurrentState() != &StateID_Explode) {
			this->doStateChange(&StateID_Explode);
		}
	}
	
	this->warning_counter++;
}



/*u32 daEnTeresa_c::checkExplosion(Point2d* vector) {
	this->getTargetPos(vector);
	
	if(((this->settings >> 24) & 0xF) == 1) {
		
		if (aPhysics.left() < vector->x + 20 && vector->x < aPhysics.right() && aPhysics.bottom() < vector->y && vector->y - 20 < aPhysics.top()) {
			this->doStateChange(&StateID_Explode);
		}
		
	}

	this->getTargetPos(vector);	
	return;
}*/


bool daEnTeresa_c::inExplosionRange(int radius = 40) {
	Point2d vec;
	this->getTargetPos(&vec);

    Point2d circleDistance;
	
	circleDistance.x = abs(this->pos.x - vec.x);
    circleDistance.y = abs(this->pos.y - vec.y);

    if (circleDistance.x > (10 + radius)) { return false; } //player width  + circle radius
    if (circleDistance.y > (20 + radius)) { return false; } //player height + circle radius

    if (circleDistance.x <= (10)) { return true; } 
    if (circleDistance.y <= (20)) { return true; }

    float cornerDistance_sq = (circleDistance.x - 10)*(circleDistance.x - 10) + (circleDistance.y - 20)*(circleDistance.y - 20);

    return (cornerDistance_sq <= radius); //<= radius
}


int daEnTeresa_c::checkExplosion() {
	int ret = this->origOnExecute();
	
	CDPrintCurrentAddress();
	
	if (doWarning) {
		this->executeState_ExplodeWarning();
	}
	
	if((this->name == EN_TERESA) && ((this->settings >> 24 & 0xF) == 1)) {
		if(this->inExplosionRange(80) && (this->acState.getCurrentState() != &StateID_Explode && !doWarning)) {
			this->beginState_ExplodeWarning();
			doWarning = true;
		}
		
		
		if(this->inExplosionRange() && this->acState.getCurrentState() != &StateID_Explode) {
			this->doStateChange(&StateID_Explode);
		}
	} //else: EN_BIG_TERESA
	
	return ret;
}


//80aaedd0 daEnTeresa_c::playerCollision


