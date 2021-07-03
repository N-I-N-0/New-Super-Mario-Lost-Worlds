#include <common.h>
#include <game.h>
#include <g3dhax.h>
#include <sfx.h>

const char* SoundEffects[5] = {
	"sfx/GoldFlower_hit",
	"sfx/GoldFlower_out",
	"sfx/GoldFlower_get",
	"sfx/GoldFlower_throw",
	"sfx/GoldFlower_get"
};

extern "C" void PlaySoundWithFunctionB4(void *spc, nw4r::snd::SoundHandle *handle, int id, int unk);
static nw4r::snd::StrmSoundHandle handlee;

u8 hijackMusicWithSongName(const char *songName, int themeID, bool hasFast, int channelCount, int trackCount, int *wantRealStreamID, bool doTheResetThing);

int NewSFXPlayer(int id) {
	if (handlee.Exists())
		handlee.Stop(0);

	int sID;
	hijackMusicWithSongName(SoundEffects[id], -1, false, 2, 1, &sID, true);
	OSReport("sID: %d\n", sID);
	//PlaySoundWithFunctionB4(SoundRelatedClass, &handlee, sID, 1);
	return sID;
}

void newPlaySoundFromPosition(SoundPlayingClass *pointer, int ID, Vec2 *pos, u32 flags) {
	int sID = NewSFXPlayer(ID - 2000);
	pointer->PlaySoundAtPosition(sID, pos, flags);
}

void newPlaySoundFromPosition2(SoundPlayingClass *pointer, int ID, u32 dunno, u32 *handle) {
	int sID = NewSFXPlayer(ID - 2000);
	pointer->PlaySoundForSoundPlayingClass(sID, dunno, handle);
}