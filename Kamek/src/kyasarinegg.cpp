#include <game.h>

const char* KyasarinEggANL [] = { "kyasarin", NULL };

//Class in kyasarin.cpp

void daKyasarinEgg_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

void daKyasarinEgg_c::playerCollision(ActivePhysics* apThis, ActivePhysics* apOther)
{
	char hitType = 0;
	hitType = usedForDeterminingStatePress_or_playerCollision(this, apThis, apOther, 2);
	
	if (hitType > 0)
	{
		PlaySound(this, SE_EMY_DOWN);
		isDying = true;
	}
	else
	{
		if (!isDying)
			this->_vf220(apOther->owner);
	}
	return;
}

bool daKyasarinEgg_c::collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther) {

	PlaySound(this, SE_EMY_DOWN);
	isDying = true;
    return true;
}
bool daKyasarinEgg_c::collisionCat5_Mario(ActivePhysics *apThis, ActivePhysics *apOther) {
	// When player collides using butt slide
	PlaySound(this, SE_EMY_DOWN);
	isDying = true;
    return true;
}
bool daKyasarinEgg_c::collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther) {
	// When player drills with propeller
	PlaySound(this, SE_EMY_DOWN);
	isDying = true;
    return true;
}
bool daKyasarinEgg_c::collisionCat8_FencePunch(ActivePhysics *apThis, ActivePhysics *apOther) {
	// When player punches fence behind
	playerCollision(apThis, apOther);
    return true;
}
bool daKyasarinEgg_c::collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther) {
	// When player ground pounds
	PlaySound(this, SE_EMY_DOWN);
	isDying = true;
    return true;
}
bool daKyasarinEgg_c::collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther) {
	// When player ground pounds with Yoshi
	PlaySound(this, SE_EMY_DOWN);
	isDying = true;
    return true;
}
bool daKyasarinEgg_c::collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther) {
	// When player slides with penguin suit
	PlaySound(this, SE_EMY_DOWN);
	isDying = true;
    return true;
}
bool daKyasarinEgg_c::collisionCat11_PipeCannon(ActivePhysics *apThis, ActivePhysics *apOther) {
	// When player shoots from pipe cannon
	PlaySound(this, SE_EMY_DOWN);
	isDying = true;
    return true;
}
bool daKyasarinEgg_c::collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther) {
	// When player throws object at sprite
	PlaySound(this, SE_EMY_DOWN);
	isDying = true;
	return true;
}
bool daKyasarinEgg_c::collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther) {

    return true;
}
bool daKyasarinEgg_c::collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther) {
	PlaySound(this, SE_EMY_DOWN);
	isDying = true;
    return true;
}
bool daKyasarinEgg_c::collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther) {

    return true;
}
bool daKyasarinEgg_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther) {
	// When collides with ice ball
    return true;
}

dActor_c* daKyasarinEgg_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daKyasarinEgg_c));
	return new(buffer) daKyasarinEgg_c;
}

const SpriteData KyasarinEggSpriteData = 
{ ProfileId::KyasarinEgg, 0, 0, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };

Profile KyasarinEggProfile(&daKyasarinEgg_c::build, SpriteId::KyasarinEgg, &KyasarinEggSpriteData, ProfileId::KyasarinEgg, ProfileId::KyasarinEgg, "KyasarinEgg", KyasarinEggANL);


int daKyasarinEgg_c::onCreate() {
	this->isDying = false;
	this->temp = 0;
	this->timer = 0;

	allocator.link(-1, GameHeaps[0], 0, 0x20);

	resFile.data = getResource("kyasarin", "g3d/kyasarin.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("kyasarin_rock");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_MapObj(&bodyModel, 0);

	allocator.unlink(); 

	ActivePhysics::Info HitMeBaby; 
	HitMeBaby.xDistToCenter = 0.0; 
	HitMeBaby.yDistToCenter = 0.0; 
	HitMeBaby.xDistToEdge = 8.0; 
	HitMeBaby.yDistToEdge = 6.0; 
	HitMeBaby.category1 = 0x3; 
	HitMeBaby.category2 = 0x0; 
	HitMeBaby.bitfield1 = 0x4F; 
	HitMeBaby.bitfield2 = 0xFFBAFFFE; 
	HitMeBaby.unkShort1C = 0; 
	HitMeBaby.callback = &dEn_c::collisionCallback; 
	this->aPhysics.initWithStruct(this, &HitMeBaby); 
	this->aPhysics.addToList(); 

	this->scale.x = 0.75; 
	this->scale.y = 0.75; 
	this->scale.z = 0.75; 



	this->onExecute();

	return true;
}

int daKyasarinEgg_c::onExecute() {
	updateModelMatrices();
	bodyModel._vf1C();

	if (isDying)
	{
		if (temp == 0)
		{
			this->removeMyActivePhysics();
			temp = 1;
		}
		if (this->timer == 300)
		{
			this->Delete(1);
		}
		dEn_c::dieFall_Execute();
		this->timer += 1;
	}
	else
	{
		//rot.x += 0x200;

		if (leftright == 'l')
			pos.x -= 1;
		else if (leftright == 'r')
			pos.x += 1;
	}
	return true;
}

int daKyasarinEgg_c::onDelete() {
	return true;
}

int daKyasarinEgg_c::onDraw() {
	bodyModel.scheduleForDrawing();

	return true;
}