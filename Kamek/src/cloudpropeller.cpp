#include <game.h>
#include <sfx.h>

const char* CPArcNameList [] = { "cloudpropeller", NULL };

class daCloudPropeller : public dEn_c {
public:

	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();


	void LoadModel(int id);
	void bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate);

	static dActor_c* build();

	void spriteCollision(ActivePhysics *apThis, ActivePhysics *apOther);

	int horizontalvertical; //0 = horizontal, ...
	int leftright; //0 = left, 1 = right
	double speed;
	double sizeofwind;
	double n;
	double m;
	double x;
	double y;

	mHeapAllocator_c allocator;
	m3d::mdl_c bodyModel;
	m3d::anmChr_c animationChr;
	nw4r::g3d::ResFile resFile;

};


void daCloudPropeller::LoadModel(int id)
{
	nw4r::g3d::ResMdl mdl;

	if (id == 1)
	{
		resFile.data = getResource("cloudpropeller", "g3d/propeller1.brres");
		mdl = this->resFile.GetResMdl("KoopaShip");
		bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
		SetupTextures_MapObj(&bodyModel, 0);
	}

	else if (id == 2)
	{
		resFile.data = getResource("cloudpropeller", "g3d/propeller2.brres");
		mdl = this->resFile.GetResMdl("KoopaShip");
		bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
		SetupTextures_MapObj(&bodyModel, 0);
	}

	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr("rotate");
	this->animationChr.setup(mdl, anmChr, &this->allocator, 0);

	return;
}


void daCloudPropeller::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate) {
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr(name);
	this->animationChr.bind(&this->bodyModel, anmChr, unk);
	this->bodyModel.bindAnim(&this->animationChr, unk2);
	this->animationChr.setUpdateRate(rate);
}

dActor_c* daCloudPropeller::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daCloudPropeller));
	return new(buffer) daCloudPropeller;
}

const SpriteData CloudPropellerSpriteData = 
{ ProfileId::CloudPropeller, 0, 0, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };

Profile CloudPropellerProfile(&daCloudPropeller::build, SpriteId::CloudPropeller, CloudPropellerSpriteData, ProfileId::CloudPropeller, ProfileId::CloudPropeller, "CloudPropeller", CPArcNameList);

void daCloudPropeller::spriteCollision(ActivePhysics* apThis, ActivePhysics* apOther)
{
	
	if (apOther->owner->name == AC_WATER_BUBBLE_CANNON)
	{
		if (((daBoomerangHax_c*)apOther->owner)->variation == 4)
		{
			x = abs(pos.x - apOther->owner->pos.x);
			y = m*x+n;
			//FINALLY move cloud
			if (horizontalvertical == 0)
			{
				if (leftright == 0)
				{
					apOther->owner->pos.x -= y;
				}
				else
				{
					apOther->owner->pos.x += y;
				}
			}
			else
			{
				apOther->owner->pos.y += y;
			}
		}
	}
	
	return;

}






int daCloudPropeller::onCreate() {
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	OSReport("daCloudPropeller init\n");

	if (settings >> 16 & 1) //Vertical
	{
		LoadModel(1);
		horizontalvertical = 1;
	}
	else //Horizontal
	{
		LoadModel(2);

		horizontalvertical = 0;

		if (settings >> 31 & 1)
		{
			leftright = 1;
		} 
		else 
		{
			leftright = 0;
		}
	}

	/*
	resFile.data = getResource("cloudpropeller", "g3d/propeller2.brres");
	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("KoopaShip");
	bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
	SetupTextures_MapOBJ(&bodyModel, 0);
	*/


	allocator.unlink(); 

	sizeofwind = (settings >> 20 & 0b111111) * 8;
	speed = (settings >> 8 & 0b1111) / 10.0;

	ActivePhysics::Info HitMeBaby; 
	if (leftright == 0) { HitMeBaby.xDistToCenter = -sizeofwind + 16;  }
	else { HitMeBaby.xDistToCenter = sizeofwind; }
	HitMeBaby.yDistToCenter = 0.0; 
	HitMeBaby.xDistToEdge = sizeofwind; 
	HitMeBaby.yDistToEdge = 64.0; 
	HitMeBaby.category1 = 0x3; 
	HitMeBaby.category2 = 0x0; 
	HitMeBaby.bitfield1 = 0x8; 
	HitMeBaby.bitfield2 = 0xFFC00000; 
	HitMeBaby.unkShort1C = 0; 
	HitMeBaby.callback = &dEn_c::collisionCallback; 
	this->aPhysics.initWithStruct(this, &HitMeBaby); 
	this->aPhysics.addToList();

	this->scale.x = 0.5;
	this->scale.y = 0.5;
	this->scale.z = 0.5;

	this->pos.y -= 8;
	
	if (horizontalvertical == 0)
	{
		if (leftright == 0)
		{
			this->rot.y = 0x4000;
			matrix.translation(pos.x + 54, pos.y, pos.z);
		}
		else
		{
			this->rot.y = -(0x4000);
			matrix.translation(pos.x - 38, pos.y, pos.z);
		}
	}
	bindAnimChr_and_setUpdateRate("rotate", 1, 0.0, 1.0);
	
	
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);
	
	//PlaySound(this, SE_DEMO_OP_SHIP_PROPELLER_1511f);
	PlaySoundWithFunctionB4(SoundRelatedClass, &handle, SE_AMB_SHIP_WIND, 1);
	
	n=2*speed;
	m=-n/(2 * sizeofwind);


	this->onExecute();

	return true;
}

int daCloudPropeller::onExecute() {

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);


	bodyModel._vf1C();

	if (this->animationChr.isAnimationDone()) {
		this->animationChr.setCurrentFrame(0.0);
	}

	return true;
}

int daCloudPropeller::onDelete() {
	return true;
}

int daCloudPropeller::onDraw() {
	bodyModel.scheduleForDrawing();

	return true;
}