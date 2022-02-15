#include <game.h>
#include "liq.h"

const char* KyasarinANL [] = { "kyasarin", NULL };


class daKyasarinEgg_c : public dEn_c {
public:

	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	void updateModelMatrices();
	

	static dActor_c* build();

	void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther); 

	bool collisionCat3_StarPower(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat5_Mario(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCatD_Drill(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat8_FencePunch(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat7_GroundPound(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat7_GroundPoundYoshi(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCatA_PenguinMario(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat11_PipeCannon(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat9_RollingObject(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat1_Fireball_E_Explosion(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat13_Hammer(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat14_YoshiFire(ActivePhysics* apThis, ActivePhysics* apOther);
	
	mHeapAllocator_c allocator;
	m3d::mdl_c bodyModel;
	m3d::anmChr_c animationChr;
	nw4r::g3d::ResFile resFile;

	char leftright;
	bool isDying;
	int temp;
	int timer;
};


class daKyasarin_c : public dEn_c {
public:

	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	void updateModelMatrices();
	void bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate);

	void TargetSetter();
	void AnimationSetter(char type, char direction);
	char Direction();

	//Ice crap
	void _vf148();
	void _vf14C();
	bool CreateIceActors();

	static dActor_c* build();

	void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther);
	
	bool collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther);				
	bool collisionCat5_Mario(ActivePhysics *apThis, ActivePhysics *apOther);					
	bool collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther);					
	bool collisionCat8_FencePunch(ActivePhysics *apThis, ActivePhysics *apOther);				
	bool collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther);				
	bool collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther);			
	bool collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther);				
	bool collisionCat11_PipeCannon(ActivePhysics *apThis, ActivePhysics *apOther);				
	bool collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther);			
	bool collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther);		
	//bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther);		
	bool collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther);					
	bool collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther);
	
	
	mHeapAllocator_c allocator;
	m3d::mdl_c bodyModel;
	m3d::anmChr_c animationChr;
	nw4r::g3d::ResFile resFile;

	dAc_Py_c* target;

	daKyasarinEgg_c* egg;

	int timer;
	int activeanim;
	Vec eggpos;
	char directionforround;
	char activedirection;

	USING_STATES(daKyasarin_c);
	DECLARE_STATE(Walk);
	DECLARE_STATE(Spit);
	DECLARE_STATE(Die);
};

CREATE_STATE(daKyasarin_c, Walk);
CREATE_STATE(daKyasarin_c, Spit);
CREATE_STATE(daKyasarin_c, Die);

void daKyasarin_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}


void daKyasarin_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate) {
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr(name);
	this->animationChr.bind(&this->bodyModel, anmChr, unk);
	this->bodyModel.bindAnim(&this->animationChr, unk2);
	this->animationChr.setUpdateRate(rate);
}

void daKyasarin_c::TargetSetter()
{
	target = GetSpecificPlayerActor(LiQ::NearestPlayer(this));
	return;
}

void daKyasarin_c::AnimationSetter(char type, char direction)
{
	switch (type)
	{
	case 'w':
		if (activeanim == 1) { break; }

		if (direction == 'l')
			bindAnimChr_and_setUpdateRate("wait_L", 1, 0.0, 1.0);
		else
			bindAnimChr_and_setUpdateRate("wait_R", 1, 0.0, 1.0);
		
		activeanim = 1;
		break;
	case 't':
		if (activeanim == 2) { break; }

		if (direction == 'l')
			bindAnimChr_and_setUpdateRate("throw_L1", 1, 0.0, 1.0);
		else
			bindAnimChr_and_setUpdateRate("throw_R1", 1, 0.0, 1.0);
		
		activeanim = 2;
		break;
	case '2':
		if (activeanim == 3) { break; }

		if (direction == 'l')
			bindAnimChr_and_setUpdateRate("throw_L2", 1, 0.0, 1.0);
		else
			bindAnimChr_and_setUpdateRate("throw_R2", 1, 0.0, 1.0);
		
		activeanim = 3;
		break;

	
	default:
		break;
	}
	return;
}

char daKyasarin_c::Direction()
{
	TargetSetter();
	if (target->pos.x < this->pos.x)
		return 'l';
	else
		return 'r';
}

void daKyasarin_c::playerCollision(ActivePhysics* apThis, ActivePhysics* apOther)
{
	if (acState.getCurrentState() != &StateID_Die)
		DamagePlayer(this, apThis, apOther);
}

bool daKyasarin_c::collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther) {

	PlaySound(this, SE_EMY_DOWN);
	doStateChange(&StateID_Die);
    return true;
}
bool daKyasarin_c::collisionCat5_Mario(ActivePhysics *apThis, ActivePhysics *apOther) {
	// When player collides using butt slide
	playerCollision(apThis, apOther);
    return true;
}
bool daKyasarin_c::collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther) {
	// When player drills with propeller
	playerCollision(apThis, apOther);
    return true;
}
bool daKyasarin_c::collisionCat8_FencePunch(ActivePhysics *apThis, ActivePhysics *apOther) {
	// When player punches fence behind
	playerCollision(apThis, apOther);
    return true;
}
bool daKyasarin_c::collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther) {
	// When player ground pounds
	playerCollision(apThis, apOther);
    return true;
}
bool daKyasarin_c::collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther) {
	// When player ground pounds with Yoshi
	playerCollision(apThis, apOther);
    return true;
}
bool daKyasarin_c::collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther) {
	// When player slides with penguin suit
	PlaySound(this, SE_EMY_DOWN);
	doStateChange(&StateID_Die);
    return true;
}
bool daKyasarin_c::collisionCat11_PipeCannon(ActivePhysics *apThis, ActivePhysics *apOther) {
	// When player shoots from pipe cannon
	PlaySound(this, SE_EMY_DOWN);
	doStateChange(&StateID_Die);
    return true;
}
bool daKyasarin_c::collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther) {
	// When player throws object at sprite
	PlaySound(this, SE_EMY_DOWN);
	doStateChange(&StateID_Die);
	return true;
}
bool daKyasarin_c::collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther) {
	PlaySound(this, SE_EMY_DOWN);
	doStateChange(&StateID_Die);
    return true;
}
bool daKyasarin_c::collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther) {
	PlaySound(this, SE_EMY_DOWN);
	doStateChange(&StateID_Die);
    return true;
}
bool daKyasarin_c::collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther) {
	PlaySound(this, SE_EMY_DOWN);
	doStateChange(&StateID_Die);
    return true;
}


void daKyasarin_c::_vf148() {
	dEn_c::_vf148();
	doStateChange(&StateID_Die);
}
void daKyasarin_c::_vf14C() {
	dEn_c::_vf14C();
	doStateChange(&StateID_Die);
}


extern "C" void sub_80024C20(void);
extern "C" void __destroy_arr(void*, void(*)(void), int, int);
//extern "C" __destroy_arr(struct DoSomethingCool, void(*)(void), int cnt, int bar);

bool daKyasarin_c::CreateIceActors() {
	struct DoSomethingCool my_struct = { 0, this->pos, {1.7, 2.2, 1.5}, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
	this->frzMgr.Create_ICEACTORs((void*)&my_struct, 1);
	__destroy_arr((void*)&my_struct, sub_80024C20, 0x3C, 1);
	animationChr.setUpdateRate(0.0f);
	return true;
}


dActor_c* daKyasarin_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daKyasarin_c));
	return new(buffer) daKyasarin_c;
}

const SpriteData KyasarinSpriteData = 
{ ProfileId::Kyasarin, 0, 0, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };

Profile KyasarinProfile(&daKyasarin_c::build, SpriteId::Kyasarin, &KyasarinSpriteData, ProfileId::Kyasarin, ProfileId::Kyasarin, "Kyasarin", KyasarinANL);


int daKyasarin_c::onCreate() {

	target = GetSpecificPlayerActor(LiQ::NearestPlayer(this));

	allocator.link(-1, GameHeaps[0], 0, 0x20);

	resFile.data = getResource("kyasarin", "g3d/kyasarin.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("kyasarin");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_Enemy(&bodyModel, 0);
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr("wait_L");
	this->animationChr.setup(mdl, anmChr, &this->allocator, 0);

	allocator.unlink(); 

	ActivePhysics::Info HitMeBaby; 
	HitMeBaby.xDistToCenter = 0.0; 
	HitMeBaby.yDistToCenter = 18.0; 
	HitMeBaby.xDistToEdge = 10.0; 
	HitMeBaby.yDistToEdge = 18.0; 
	HitMeBaby.category1 = 0x3; 
	HitMeBaby.category2 = 0x0; 
	HitMeBaby.bitfield1 = 0x4F; 
	HitMeBaby.bitfield2 = 0xFFFFFFFF; 
	HitMeBaby.unkShort1C = 0; 
	HitMeBaby.callback = &dEn_c::collisionCallback; 
	this->aPhysics.initWithStruct(this, &HitMeBaby); 
	this->aPhysics.addToList(); 

	this->scale.x = 1.0; 
	this->scale.y = 1.0; 
	this->scale.z = 1.0; 

	bindAnimChr_and_setUpdateRate("wait_L", 1, 0.0, 1.0);

	doStateChange(&StateID_Walk);

	this->onExecute();

	return true;
}

int daKyasarin_c::onExecute() {
	acState.execute();

	updateModelMatrices();
	bodyModel._vf1C();

	if (this->animationChr.isAnimationDone()) {
		this->animationChr.setCurrentFrame(0.0);
	}

	return true;
}

int daKyasarin_c::onDelete() {
	return true;
}

int daKyasarin_c::onDraw() {
	bodyModel.scheduleForDrawing();

	return true;
}

void daKyasarin_c::beginState_Walk() 
{ 
	this->timer = 0;

	directionforround = Direction();

	if (directionforround == 'l')
		bindAnimChr_and_setUpdateRate("wait_L", 1, 0.0, 1.0);
	else
		bindAnimChr_and_setUpdateRate("wait_R", 1, 0.0, 1.0);

	//OSReport("Posx %x\n", pos.x);
}
void daKyasarin_c::executeState_Walk() 
{
	if (timer < 60)
	{
		pos.x += 0.5;
	}
	else if (timer > 90)
	{
		pos.x -= 0.5;
	}

	if (timer == 150)
	{
		//OSReport("Posx %x\n", pos.x);
		doStateChange(&StateID_Spit);
	}

	this->timer += 1;
}
void daKyasarin_c::endState_Walk() { }

void daKyasarin_c::beginState_Spit() 
{ 
	timer = 0;
	directionforround = Direction();

	if (directionforround == 'l')
		bindAnimChr_and_setUpdateRate("throw_L1", 1, 0.0, 1.0);
	else
		bindAnimChr_and_setUpdateRate("throw_R1", 1, 0.0, 1.0);
}
void daKyasarin_c::executeState_Spit() 
{ 
	if (timer == 48)
	{
		switch (directionforround)
		{
		case 'l':
			eggpos = (Vec){pos.x - 12.0, pos.y + 24.0, pos.z + 4};
			break;
		case 'r':
			eggpos = (Vec){pos.x + 12.0, pos.y + 24.0, pos.z + 4};
			break;
		default:
			break;
		}
		egg = (daKyasarinEgg_c*)CreateActor(KyasarinEgg, 0, eggpos, 0, 0);
		egg->leftright = directionforround;
	}
	if (timer == 78)
	{
		if (directionforround == 'l')
			bindAnimChr_and_setUpdateRate("throw_L2", 1, 0.0, 1.0);
		else
			bindAnimChr_and_setUpdateRate("throw_R2", 1, 0.0, 1.0);
	}
	if (timer == 87)
	{
		doStateChange(&StateID_Walk);
	}

	timer += 1;
}
void daKyasarin_c::endState_Spit() { }

void daKyasarin_c::beginState_Die() 
{ 
	timer = 0;

	this->removeMyActivePhysics();

	if (directionforround == 'l')
		bindAnimChr_and_setUpdateRate("freeze_L", 1, 0.0, 1.0);
	else
		bindAnimChr_and_setUpdateRate("freeze_R", 1, 0.0, 1.0);
	animationChr.setUpdateRate(0.0f);
}
void daKyasarin_c::executeState_Die() 
{
	if (timer == 300)
	{
		this->Delete(1);
	}
	
	dEn_c::dieFall_Execute();

	this->timer += 1;
}
void daKyasarin_c::endState_Die() { }
