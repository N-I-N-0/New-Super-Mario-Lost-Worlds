.text
.align 4

#include <asm_setup.S>

.extern continueFromOldPlaySound
.extern PlaySoundForSoundPlayingClass__17SoundPlayingClassFiUiPUi
.extern newPlaySoundFromPosition__FP17SoundPlayingClassiP7Point2dUi
.extern newPlaySoundFromPosition2__FP17SoundPlayingClassiUiPUi

.global newPlaySoundCheck
newPlaySoundCheck:
	cmpwi r4, 2000 #If SFX ID is >= 2000
	bge _newPlaySound
	stwu r1, -0x80(r1) #Restore instruction
	b continueFromOldPlaySound
_newPlaySound:
	b newPlaySoundFromPosition__FP17SoundPlayingClassiP7Point2dUi

.global newPlaySoundCheck2
newPlaySoundCheck2:
	cmpwi r4, 2000 #If SFX ID is >= 2000
	bge _newPlaySound2
	b PlaySoundForSoundPlayingClass__17SoundPlayingClassFiUiPUi
_newPlaySound2:
	b newPlaySoundFromPosition2__FP17SoundPlayingClassiUiPUi