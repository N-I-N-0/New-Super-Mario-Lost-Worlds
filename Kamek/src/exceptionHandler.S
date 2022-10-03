.text
.align 4
.extern AXSetMasterVolume
.global noAudioAfterCrash
noAudioAfterCrash:
    // Original instruction
    mr r28, r3

    // Mute audio
    li r3, 0
    b AXSetMasterVolume
	blr