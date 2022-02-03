#include <common.h>
#include <game.h>
#include <profile.h>

u8 playersUpsideDown[4];

class dUpsideDownWall_c : public dStageActor_c {
public:
	bool allLocations;
	int locationID;

	void tryToTrigger();

	int onCreate();
	int onExecute();

	static dActor_c *build();
};

const char *UpsideDownWallFileList[] = {0};
const SpriteData UpsideDownWallSpriteData = { ProfileId::UpsideDownWall, 8, -8 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile UpsideDownWallProfile(&dUpsideDownWall_c::build, SpriteId::UpsideDownWall, &UpsideDownWallSpriteData, ProfileId::UpsideDownWall, ProfileId::UpsideDownWall, "UpsideDownWall", UpsideDownWallFileList);


dActor_c  *dUpsideDownWall_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dUpsideDownWall_c));
	dUpsideDownWall_c *c = new(buffer) dUpsideDownWall_c;

	return c;
}


int dUpsideDownWall_c::onCreate() {
	this->allLocations = this->settings >> 31 & 1;
	this->locationID = this->settings & 0xFF;

	return true;
}

inline u32 isInLocation(dAcPy_c* player, int locationID, dCourse_c* course) {
	mRect rect;
	course->getRectByID(locationID, &rect);

	OSReport("ID: %d, settings: %x, name: %d\n", player->id, player->settings, player->name);

	if(player->pos.x >= rect.x) {
		//OSReport("Check 1\n");
		if(player->pos.x <= rect.x + rect.width) {
			//OSReport("Check 2\n");
			if(player->pos.y <= rect.y) {
				//OSReport("Check 3\n");
				if(player->pos.y >= rect.y - rect.height) {
					//OSReport("Check 4\n");				
					return 16;
				}
			}
		}
	}
	return 0;
}

int dUpsideDownWall_c::onExecute() {
	for (int i = 0; i < 4; i++) {
		//OSReport("P1: %d, P2: %d, P3: %d, P4: %d\n", playersUpsideDown[0], playersUpsideDown[1], playersUpsideDown[2], playersUpsideDown[3]);
		dAcPy_c *player;
		if (player = dAcPy_c::findByID(i)) {
			OSReport("Player: %d\n", i);
			dCourse_c* course = dCourseFull_c::instance->get(GetAreaNum());
			if(allLocations) {
				u32 isPlayerInLocation = 0;
				for(int j = 0; j <= course->rectCount; j++) {
					if(isInLocation(player, j, course) != 0) {
						isPlayerInLocation = 16;
						break;
					}
				}
				playersUpsideDown[player->settings & 3] = isPlayerInLocation;
			} else {
				playersUpsideDown[player->settings & 3] = isInLocation(player, locationID, course);
			}
		}
	}
	return true;
}