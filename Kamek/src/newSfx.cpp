#include <common.h>
#include <game.h>
#include <g3dhax.h>
#include <sfx.h>

const char* SoundEffects[] = {
	"sfx/GoldFlower_hit",			//2000
	"sfx/GoldFlower_out",			//2001
	"sfx/GoldFlower_get",			//2002
	"sfx/GoldFlower_throw",			//2003
	"sfx/GoldFlower_get",			//2004
	"sfx/FlipPanel"					//2005
};

extern "C" void PlaySoundWithFunctionB4(void *spc, nw4r::snd::SoundHandle *handle, int id, int unk);
static nw4r::snd::StrmSoundHandle handlee;

u8 hijackMusicWithSongName(const char *songName, int themeID, bool hasFast, int channelCount, int trackCount, int *wantRealStreamID, bool doTheResetThing);

int NewSFXPlayer(int id, bool withFunctionB4 = false) {
	if (handlee.Exists())
		handlee.Stop(0);

	int sID;
	hijackMusicWithSongName(SoundEffects[id], -1, false, 2, 1, &sID, true);
	OSReport("sID: %d\n", sID);
	if(withFunctionB4) PlaySoundWithFunctionB4(SoundRelatedClass, &handlee, sID, 1);
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