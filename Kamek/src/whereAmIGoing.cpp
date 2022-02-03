#include <common.h>
#include <game.h>
#include <profile.h>
#include <dCourse.h> 

class whereAmIGoing : public dStageActor_c {
public:
	float width;
	float height;
	float posY;
	float posX;
	u8 type;

	
	int zoneNum;

	

	dStageActor_c* moving[8];
	u8 movingDone[8];
	bool movingDirection[8];

	int yayCameraID;
	nw4r::g3d::CameraData* yayCameraData;

	int onCreate();
	int onExecute();

	static dActor_c* build();
};

const char* WhereAmIGoingFileList[] = { 0 };
const SpriteData WhereAmIGoingSpriteData = { ProfileId::WhereAmIGoing, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile WhereAmIGoingProfile(&whereAmIGoing::build, SpriteId::WhereAmIGoing, &WhereAmIGoingSpriteData, ProfileId::WhereAmIGoing, ProfileId::WhereAmIGoing, "WhereAmIGoing", WhereAmIGoingFileList);


dActor_c* whereAmIGoing::build() {
	void* buffer = AllocFromGameHeap1(sizeof(whereAmIGoing));
	whereAmIGoing* c = new(buffer) whereAmIGoing;

	return c;
}

int whereAmIGoing::onCreate() {

	this->yayCameraID = GetCurrentCameraID();

	this->yayCameraData = GetCameraByID(this->yayCameraID);

	OSReport("Camera ID: %d\n", this->yayCameraID);

	this->type = this->settings >> 28 & 0xF;

	if (this->type == 0)
	{
		this->posX = this->settings >> 18 & 0b1111111111;
		this->posX *= -1;
	}
	else if (this->type == 1)
	{
		this->posY = this->settings >> 18 & 0b1111111111;
		this->posY *= -1;
	}

	if (this->type == 0)
	{
		this->width = this->settings >> 0 & 0xFFFF;
	}
	else if (this->type == 1)
	{
		this->height = this->settings >> 0 & 0xFFFF;
	}


	OSReport("Where Created Type:%d\n", this->type);
	OSReport("Where Created PosY:%d\n", this->posY);
	OSReport("Where Created Height:%d\n", this->height);


	dCourse_c* course = dCourseFull_c::instance->get(GetAreaNum());
	zoneNum = GetZoneNum();
	//dCourse_c::bounding_s* boundings = &course->getBoundingByID(zoneNum);
	dCourse_c::zone_s* zone = course->getZoneByID(zoneNum);

	for (int i = 0; i < 8; i++) {
		moving[i] = 0;
		movingDirection[i] = false;
		movingDone[i] = 31;
	}

	this->onExecute();
	return true;
}

int whereAmIGoing::onExecute() {

	dStageActor_c* current;
	Actor* currentActor;

	currentActor = FindActorByType(PLAYER, 0);
	current = (dStageActor_c*)currentActor;

	OSReport("Type: %d\n", this->type);

	/*Actor* cameraActor;
	cameraActor = FindActorByType(CAMERA, 0);
	int i = 0;

	nw4r::g3d::Camera* cami;

	while (cameraActor != 0)
	{
		cami = (nw4r::g3d::Camera*)cameraActor;



		//OSReport("camera class: %s    ID: %d\n", cameraActor->class_name, i);
		cameraActor = FindActorByType(CAMERA, cameraActor);
		i++;
	}*/

	//currentZoneID

	while (current != 0)
	{
		//OSReport("Where Found Player\n");
		/*OSReport("Active: %d\n", yayCameraData != 0);

		OSReport("Camera Data Pos x: %f\n", this->yayCameraData->cameraPos.x);
		OSReport("Camera Data Pos y: %f\n", this->yayCameraData->cameraPos.y);
		OSReport("Camera Data Pos z: %f\n", this->yayCameraData->cameraPos.z);

		OSReport("Camera Data Up x: %f\n", this->yayCameraData->cameraUp.x);
		OSReport("Camera Data Up y: %f\n", this->yayCameraData->cameraUp.y);
		OSReport("Camera Data Up z: %f\n", this->yayCameraData->cameraUp.z);

		OSReport("Camera Data Target x: %f\n", this->yayCameraData->cameraTarget.x);
		OSReport("Camera Data Target y: %f\n", this->yayCameraData->cameraTarget.y);
		OSReport("Camera Data Target z: %f\n", this->yayCameraData->cameraTarget.z);

		OSReport("Camera Data Twist x: %f\n", this->yayCameraData->cameraRotate.x);
		OSReport("Camera Data Twist y: %f\n", this->yayCameraData->cameraRotate.y);
		OSReport("Camera Data Twist z: %f\n", this->yayCameraData->cameraRotate.z);*/


		/*OSReport("ClassWithCameraInfo _34: %f\n", ClassWithCameraInfo::instance->_34);

		OSReport("ClassWithCameraInfo screenLeft: %f\n", ClassWithCameraInfo::instance->screenLeft);
		OSReport("ClassWithCameraInfo screenTop: %f\n", ClassWithCameraInfo::instance->screenTop);

		OSReport("ClassWithCameraInfo screen Width: %f\n", ClassWithCameraInfo::instance->screenWidth);
		OSReport("ClassWithCameraInfo screen Height: %f\n", ClassWithCameraInfo::instance->screenHeight);

		OSReport("ClassWithCameraInfo centre X: %f\n", ClassWithCameraInfo::instance->screenCentreX);
		OSReport("ClassWithCameraInfo centre y: %f\n", ClassWithCameraInfo::instance->screenCentreY);

		OSReport("ClassWithCameraInfo offset X: %f\n", ClassWithCameraInfo::instance->xOffset);

		current->pos.x += current->speed.x + 2;

		OSReport("Player X: %f\n", current->pos.x);

		//ClassWithCameraInfo::instance->screenCentreY += 0.001;


		/*if (outOfZone(current->pos, false, current->currentZoneID))
		{
			if (this->type == 0)
			{
				if (current->pos.x < this->posX + (this->width / 2))
				{
					OSReport("Where moved Player\n");
					current->pos.x = this->width + this->posX - 10;
				}
				else if (current->pos.x > this->posX + (this->width / 2))
				{
					current->pos.x = this->posX + 10;
					OSReport("Where moved Player\n");
				}
			}
			else if (this->type == 1)
			{
				if (current->pos.y < this->posY - this->height)
				{
					OSReport("Where moved Player\n");
					current->pos.y = this->posY;
				}
				else if (current->pos.y > this->posY)
				{
					current->pos.y = this->posY - this->height;
					OSReport("Where moved Player\n");
				}
			}
		}*/

		Remocon* pIn = RemoconMng->controllers[current->which_player];

		if (this->type == 0) {
			OSReport("Speed: %f\n", current->speed.x);
			OSReport("x: %f\n", current->pos.x);
			OSReport("last x: %f\n", current->last_pos.x);

			if (pIn->heldButtons & WPAD_LEFT && current->speed.x > -0.11 && current->speed.x < 0) {
				if (!((current->collMgr.outputMaybe & (0x15 << 0)) || (current->collMgr.outputMaybe & (0x15 << 1)))) {
					OSReport("End of Zone\n");
				}
			}
		}
		else if (this->type == 1) {
			/*OSReport("Y: %f \n", current->pos.y);
			OSReport("screen centre: %f \n", ClassWithCameraInfo::instance->screenCentreY);
			OSReport("screen centre: %f \n", ClassWithCameraInfo::instance->screenHeight);
			OSReport("rechnen: %f \n", ClassWithCameraInfo::instance->screenCentreY - (ClassWithCameraInfo::instance->screenHeight / 2));*/

			OSReport("Where Created Type:%d\n", this->type);
			OSReport("Where Created PosY:%f\n", this->posY);
			OSReport("Where Created Height:%f\n", this->height);
			OSReport("PosY:%f\n", current->pos.y);

			if (current->pos.y < this->posY - height) {
				OSReport("End of Zone Bottom\n");


				/*bool jap = false;
				for (int i = 0; i < 8; i++)
				{
					if (current == moving[i])
					{
						jap = true;
					}
				}
				if (jap == false) {
					for (int i = 0; i < 8; i++)
					{
						if (moving[i] == 0) {
							moving[i] = current;
							movingDirection[i] = false;
							movingDone[i] = 0;
							break;
						}
					}
				}*/


				current->pos.y = this->posY;
			}
			else if (current->pos.y > this->posY - 48) {
				OSReport("End of Zone Top\n");


				/*bool jap = false;
				for (int i = 0; i < 8; i++)
				{
					if (current == moving[i])
					{
						jap = true;
					}
				}
				if (jap == false) {
					for (int i = 0; i < 8; i++)
					{
						if (moving[i] == 0) {
							moving[i] = current;
							movingDirection[i] = true;
							movingDone[i] = 0;
							break;
						}
					}
				}*/


				current->pos.y = this->posY - height + 48;
			}

			OSReport("PosY:%f\n", current->pos.y);
		}

		currentActor = FindActorByType(PLAYER, currentActor);
		current = (dStageActor_c*)currentActor;
	}

	/*for (int i = 0; i < 8; i++)
	{
		if (moving[i] != 0) {
			OSReport("JAAAAAA\n");
			if (movingDirection[i] == false) {
				OSReport("JOOOOO\n");
				if (movingDone[i] < 127) {
					OSReport("YEEEEEET\n");
					moving[i]->pos.y = this->posY;
					movingDone[i] += 1;
				}
				else {
					moving[i]->pos.y = this->posY;
					movingDone[i] = 0;
					moving[i] = 0;
					OSReport("YAAAAAATT\n");
				}
			}
			else {
				OSReport("JOOOOOOOOOOOOOOOOOOOOOO\n");
				if (movingDone[i] < 127) {
					OSReport("YEEEEEEEEEEEEEEEEEEEEET\n");
					moving[i]->pos.y = this->posY - this->height + 200;
					movingDone[i] = 0;
					moving[i] = 0;
					OSReport("YAAAAAAAAAAAAAAAAAAAAAAATT\n");
				}
			}
		}
	}*/				

	return true;
}