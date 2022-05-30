.text
.align 4

#include <asm_setup.S>



.extern toadBlockEditsReturn #0x809c3034
.extern FUN_P1_800fd140
.global toadBlockEdits
toadBlockEdits: #0x809c3030
    // Check spritedata
    lwz r3, 0x4(r30)
    rlwinm. r3, r3, 3, 31, 31  // r3 = (r3 <<< 3) & 1
    beq _OptionNotEnabled
_OptionEnabled:
    // (Just return true)
    li r3, 1
    b _End
_OptionNotEnabled:
    // (Original code)
    bl FUN_P1_800fd140
_End:
    b toadBlockEditsReturn



.extern ToadBlock_DisappearWithBooLaugh
.extern player_set_15daEnBlockMain_cFii
.global checkToadBlockNoToad #0x809c39e8
checkToadBlockNoToad:
    // Check spritedata
    lwz r0, 0x4(r31)
    rlwinm. r0, r0, 4, 31, 31  // r0 = (r0 <<< 4) & 1
    beq _OptionNotEnabled
_OptionEnabled2:
    b ToadBlock_DisappearWithBooLaugh
_OptionNotEnabled2:
    // (Original code)
    b player_set_15daEnBlockMain_cFii