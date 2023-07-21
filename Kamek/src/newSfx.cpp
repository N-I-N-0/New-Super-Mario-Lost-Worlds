#include <common.h>
#include <game.h>
#include <g3dhax.h>
#include <sfx.h>

void playSoundDistanceScreenPositionVec(nw4r::snd::SoundHandle handle, Vec2* pos, int id, float volume = 1.0, float pitch = 1.0, float distance = 500.0) {
	float v = max<float>(0.0, (1.0 - (sqrtf(pos->x * pos->x + pos->y * pos->y) / distance)) * 1.0);
	if (v <= 0.0) return;
	else if (v > 1.0) v = 1.0;

	PlaySoundWithFunctionB4(SoundRelatedClass, &handle, id, 1);
	handle.SetVolume(volume * v, 1);
	if (pitch != 1.0) handle.SetPitch(pitch);
}

void newPlaySoundFromPosition(SoundPlayingClass *pointer, int ID, Vec2 *pos, u32 flags) {
	static nw4r::snd::SoundHandle handle_newPlaySoundFromPosition; // Sound Handle
	playSoundDistanceScreenPositionVec(handle_newPlaySoundFromPosition, pos, ID);
}

void newPlaySoundFromPosition2(SoundPlayingClass *pointer, int ID, u32 dunno, u32 *handle) {
	static nw4r::snd::SoundHandle handle_newPlaySoundFromPosition2; // Sound Handle
	PlaySoundWithFunctionB4(SoundRelatedClass, &handle_newPlaySoundFromPosition2, ID, 1);
	handle_newPlaySoundFromPosition2.SetVolume(1.0f, 1);
}