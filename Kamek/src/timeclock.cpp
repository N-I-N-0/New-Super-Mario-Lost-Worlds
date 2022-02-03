#include <common.h>
#include <game.h>
#include <g3dhax.h>
#include <sfx.h>
#include <timekeeper.h>

const char* TimeClockFileList [] = { "clock", 0 };

extern "C" void PlaySoundWithFunctionB4(void *spc, nw4r::snd::SoundHandle *handle, int id, int unk);
static nw4r::snd::StrmSoundHandle handle;



//used in timeclock.S
bool slowDownTime = false;	
int getTimeSfxIndex() {
	dCourse_c *course = dCourseFull_c::instance->get(GetAreaNum());
	dCourse_c::zone_s *zone = course->getZoneByID(GetZoneNum());
	return zone->unk3;
}




u8 hijackMusicWithSongName(const char *songName, int themeID, bool hasFast, int channelCount, int trackCount, int *wantRealStreamID);

class daTimeClock_c : public dEn_c {
public:
	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	mHeapAllocator_c allocator;
	m3d::mdl_c bodyModel;
	
	int immaGonnaDisappear;
	int timer;

	static dActor_c *build();

	void updateModelMatrices();
	void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther);

	bool collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther);

	bool collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther);
};

const SpriteData timeClockSpriteData = { ProfileId::TimeClock, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile timeClockEventProfile(&daTimeClock_c::build, SpriteId::TimeClock, &timeClockSpriteData, ProfileId::TimeClock, ProfileId::TimeClock, "TimeClock", TimeClockFileList);



void daTimeClock_c::playerCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	int addTime = ((this->settings >> 20 & 0xF) * 10);
	PlaySoundWithFunctionB4(SoundRelatedClass, &handle, SE_DEMO_OP_V_PCH_INTO_CAKE, 1); //Audio[1045], replaced with the clock sound
	if(dGameDisplay_c::instance->timer <= 100 && dGameDisplay_c::instance->timer + addTime > 100)
		slowDownTime = true;
	TimeKeeper::instance->setTime(dGameDisplay_c::instance->timer + addTime);
	removeMyActivePhysics();
	// ClassWithCameraInfo *cwci = ClassWithCameraInfo::instance;
	// Vec debug = ConvertStagePositionIntoScreenPosition(&dGameDisplay_c::instance->timerBox->trans, &this->pos, ClassWithCameraInfo::instance);
	// OSReport("trans.x = %d trans.y = %d trans.z %d\n", dGameDisplay_c::instance->timerBox->trans.x, dGameDisplay_c::instance->timerBox->trans.y, dGameDisplay_c::instance->timerBox->trans.z);
	// OSReport("pos.x = %d pos.y = %d pos.z %d\n", this->pos.x, this->pos.y, this->pos.z);
	// OSReport("newtrans.x = %d newtrans.y = %d newtrans.z = %d\n", debug.x, debug.y, debug.z);
	// dGameDisplay_c::instance->timerBox->trans = ConvertStagePositionIntoScreenPosition(&dGameDisplay_c::instance->timerBox->trans, &this->pos, ClassWithCameraInfo::instance);
	this->immaGonnaDisappear++;
}

bool daTimeClock_c::collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther) {
	PlaySoundWithFunctionB4(SoundRelatedClass, &handle, SE_DEMO_OP_V_PCH_INTO_CAKE, 1); //Audio[1045], replaced with the clock sound
	TimeKeeper::instance->setTime(dGameDisplay_c::instance->timer + ((this->settings >> 20 & 0xF) * 10));
	removeMyActivePhysics();
	this->immaGonnaDisappear++;
	return false;
}
bool daTimeClock_c::collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther) {
	return collisionCat7_GroundPound(apThis, apOther);
}
bool daTimeClock_c::collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther) {
	return collisionCat9_RollingObject(apThis, apOther);
}
bool daTimeClock_c::collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther) {
	PlaySoundWithFunctionB4(SoundRelatedClass, &handle, SE_DEMO_OP_V_PCH_INTO_CAKE, 1); //Audio[1045], replaced with the clock sound
	TimeKeeper::instance->setTime(dGameDisplay_c::instance->timer + ((this->settings >> 20 & 0xF) * 10));
	removeMyActivePhysics();
	this->immaGonnaDisappear++;
}

bool daTimeClock_c::collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther) {
	return collisionCat9_RollingObject(apThis, apOther);
}
bool daTimeClock_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther) {
	return collisionCat9_RollingObject(apThis, apOther);
}
bool daTimeClock_c::collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther) {
	return true;
}
bool daTimeClock_c::collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther) {
	return collisionCat9_RollingObject(apThis, apOther);
}
bool daTimeClock_c::collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther) {
	return collisionCat1_Fireball_E_Explosion(apThis, apOther);
}

bool daTimeClock_c::collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther) {
	return true;
}



dActor_c *daTimeClock_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daTimeClock_c));
	return new(buffer) daTimeClock_c;
}


int daTimeClock_c::onCreate() {

	allocator.link(-1, GameHeaps[0], 0, 0x20);

	nw4r::g3d::ResFile rf(getResource("clock", "g3d/clock.brres"));
	if((this->settings >> 16 & 0xF) == 0) {
		bodyModel.setup(rf.GetResMdl("ClockGreen"), &allocator, 0x224, 1, 0);
	}
	if((this->settings >> 16 & 0xF) == 1) {
		bodyModel.setup(rf.GetResMdl("ClockBlue"), &allocator, 0x224, 1, 0);
	}
	SetupTextures_MapObj(&bodyModel, 0);

	allocator.unlink();

	ActivePhysics::Info HitMeBaby;
	HitMeBaby.xDistToCenter = 0.0;
	HitMeBaby.yDistToCenter = 0.0;
	HitMeBaby.xDistToEdge = 12.0;
	HitMeBaby.yDistToEdge = 15.0;
	HitMeBaby.category1 = 0x5;
	HitMeBaby.category2 = 0x0;
	HitMeBaby.bitfield1 = 0x4F;
	HitMeBaby.bitfield2 = 0xFFFFFFFF;
	HitMeBaby.unkShort1C = 0;
	HitMeBaby.callback = &dEn_c::collisionCallback;

	this->aPhysics.initWithStruct(this, &HitMeBaby);
	this->aPhysics.addToList();

	this->scale.x = 50.0;
	this->scale.y = 50.0;
	this->scale.z = 50.0;

	this->pos.z = 3300.0;

	this->onExecute();
	return true;
}


int daTimeClock_c::onDelete() {
	return true;
}

int daTimeClock_c::onDraw() {
	bodyModel.scheduleForDrawing();
	return true;
}


void daTimeClock_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

int daTimeClock_c::onExecute() {
	updateModelMatrices();

	if(immaGonnaDisappear == 1) {
		this->rot.y -= 0x1000;
		this->timer++;
		this->scale = (Vec){this->scale.x - 2.1f, this->scale.y - 2.1f, this->scale.z - 2.1f};
		if(this->timer == 1) {
			S16Vec rotation = {0,0,0};
			Vec scale = {1.0, 1.0, 1.0};
			SpawnEffect("Wm_ob_starcoinget_ring", 0, &this->pos, &rotation, &scale);	
		}
		if(this->timer < 8) {
			this->pos.y += 2;
		}
		if(this->timer > 7 && this->timer < 16) {
			this->pos.y -= 2;
		}
		if(this->timer > 16) {
			this->Delete(1);
		}
	}
	else {
		this->rot.y -= 0x200;
	}
	return true;
}

