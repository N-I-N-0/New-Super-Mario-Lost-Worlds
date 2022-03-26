.text

#include <asm_setup.S>

.align 4
.extern OSReport
.extern ActivateWipe__Fi
.extern DoStartLevel__FPvP14StartLevelInfo
.extern ActiveWiimote
.extern GameMgrP
.extern dEventOpeningTitle_c_StateID_PressButtonExitAnimeEndWait

.extern ExitStage__Fiiii
.extern DoSceneChange__FUsUiUi

.set HW_VISOLID 0xCD000024
.set redColor 0x54FF4C01

.global TitleScreenExec
TitleScreenExec:
	lis r4, redColor@h
	ori r4, r4, redColor@l
	lis r3, HW_VISOLID@ha
	stw r4, HW_VISOLID@l(r3)
	
	li r3, 3
	li r4, 0
	li r5, 0  
	bl DoSceneChange__FUsUiUi
	b ret
	
	li    r4, 0x0200                # WPAD_TWO
    lis   r3, ActiveWiimote@ha
    lwz   r3, ActiveWiimote@l(r3)
    lwz   r12, 0(r3)
    lwz   r12, 0x20(r12)
    mtctr r12
    bctrl
    cmpwi r3, 1
    beq   ret
    lis   r3, ActiveWiimote@ha
    lwz   r3, ActiveWiimote@l(r3)
    lwz   r4, 0x18(r3)
    # r3 contains the pressed buttons
    andi. r5, r4, 0x1010                # WPAD_PLUS | WPAD_MINUS
    cmpwi r5, 0x1010
    bne   ret
    # plus and minus are pressed
    lis   r3, text@h
    ori   r3, r3, text@l
    bl    OSReport
    li    r3, 4
    bl    ActivateWipe__Fi
    lis   r3, GameMgrP@ha
    lwz   r3, GameMgrP@l(r3)
    lis   r4, LevelInfo11@h
    ori   r4, r4, LevelInfo11@l
    # BUG: save file is not selected. Default to save file 1
    # TODO: make the level configurable
    bl    DoStartLevel__FPvP14StartLevelInfo
    b     ret

goToSaveSelect:
    addi  r3, r31, 0x208
    lis   r4, dEventOpeningTitle_c_StateID_PressButtonExitAnimeEndWait@ha
    addi  r4, r4, dEventOpeningTitle_c_StateID_PressButtonExitAnimeEndWait@l
    lwz   r12, 0x208(r31)
    lwz   r12, 0x18(r12)
    mtctr r12
    bctrl

ret:
    lwz   r0, 0x14(r1)
    lwz   r31, 0xC(r1)
    addi  r1, r1, 0x10
    mtlr  r0
    blr

.data
text: .string "dEventOpeningTitle_c: Starting 1-1...\n"

LevelInfo11:
.long 0
.byte 0
.byte 0xFF
.byte 0
.byte 0
.long 0
.byte 0
.byte 0
.byte 0
.byte 0
