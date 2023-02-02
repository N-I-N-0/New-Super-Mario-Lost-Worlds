#include <game.h>
#include <common.h>
#include "levelinfo.h"

bool isFastFoe;

extern bool isFastFoe;
u16 globalWhitelist[] =  {EN_KURIBO, EN_PATA_KURIBO, EN_NOKONOKO, EN_PATAPATA, EN_MET, // Goomba, Paragoomba, Koopa, Parakoopa, Buzzy Beetle
							EN_TOGEMET, EN_TOGEZO, EN_SAKASA_TOGEZO, EN_KARON, // Spike Top, Spiny, UD Spiny, Dry Bones
							EN_BOSS_LARRY, EN_BOSS_WENDY, EN_BOSS_IGGY, EN_BOSS_LEMMY, EN_BOSS_MORTON, EN_BOSS_ROY, EN_BOSS_LUDWIG, TORIDE_KOKOOPA_DEMO, // Koopalings & Controller
							OBJ_MORTON, OBJ_ROY, OBJ_LARRY, OBJ_WENDY, OBJ_IGGY_SLED, OBJ_LUDWIG, CASTLE_LEMMY_THROW_BALL, CASTLE_KOKOOPA_DEMO_1ST, // CKoopalings & CController
							ZOOM_PIPE, ZOOM_PIPE_DOWN, // Moving Pipe Up, Moving Pipe Down
							EN_KILLER_HOUDAI, EN_MAGNUM_KILLER_HOUDAI, EN_KILLER_HOUDAI_SLIDE, // Bill Blaster, Banzai Bill Blaser, Rising/Falling Bill Blaster
							EN_KILLER, EN_SEARCH_KILLER, EN_MAGNUM_KILLER, EN_SEARCH_MAGNUM_KILLER, // Bullet Bill, Red Bullet Bill, Banzai Bill, Red Banzai Bill
							EN_BUBBLE, // Podoboo
							EN_UP_DOKAN_PAKKUN, EN_DOWN_DOKAN_PAKKUN, EN_RIGHT_DOKAN_PAKKUN, EN_LEFT_DOKAN_PAKKUN, EN_UP_DOKAN_FPAKKUN, EN_DOWN_DOKAN_FPAKKUN, EN_RIGHT_DOKAN_FPAKKUN, EN_LEFT_DOKAN_FPAKKUN, EN_JIMEN_PAKKUN, EN_JIMEN_BIG_PAKKUN, EN_JIMEN_FPAKKUN, EN_JIMEN_BIG_FPAKKUN, // Piranha Plants
							CHUKAN_POINT, // Checkpoint
							AC_LIFT_RIDE_VMOVE, AC_LIFT_RIDE_HMOVE, AC_LIFT_FALL, AC_LINE_LIFT, // Vertical Moving Platform, Horizontal Moving Platform, Fall-when-on Platform, LineC Platform
						
							//EN_ITEM, PoisonShroom, CloudSpawner, DarkStar, SuperBubble, SecretGem,
							FishingBoo, ShyGuy, ShyGuyGiant, Topman, MiniSidestepper, GoombaTower,
							BusuBusu, Poihana, Byugo, WingTenten, ParaBones, Kyasarin, KyasarinEgg,
							Frogoon, ThunderCloud, Twister, CheepCheepModel, CheepCheepController,
							PokeyHead, ScreamingPillar, ThornyFlower, Octo, OctoBullet, Scuttlebug,
							FlyingBook, Teren,
						};
// u16 xWhitelist[0] = {};
// u16 yWhitelist[0] = {};

#define whitelistLen (sizeof(globalWhitelist) / 2)

bool isEnemyAllowedToBeFast(dEn_c *ptr, bool isY) {
	if(!isFastFoe)
		return false;

	for(int i = 0; i < whitelistLen; i++) {
		if(ptr->name == globalWhitelist[i])
			return true;
	}

	// if(!isY) {
	// 	for(int i = 0; i < 1; i++) {
	// 		if(ptr->name == xWhitelist[i])
	// 			return true;
	// 	}
	// }
	// else {
	// 	for(int i = 0; i < 1; i++) {
	// 		if(ptr->name == yWhitelist[i])
	// 			return true;
	// 	}
	// }

	return false;
}