.text
.align 4

#include <asm_setup.S>

.extern UpdateSelectCursor

.global fileSelectModes
fileSelectModes:
	lis r4, fileSelectLists@h
	ori r4, r4, fileSelectLists@l
	blr

.global newCursors
newCursors:
	cmpwi r0, 0x5
	beq doSpeedMode
	bge doHardMode
doCoinBattle:
	lwz r3, 0x2E4(r3)
	li r4, 0x1
	li r5, 0x0
	b UpdateSelectCursor

doSpeedMode:
	lwz r3, 0x2E8(r3)
	li r4, 0x1
	li r5, 0x0
	b UpdateSelectCursor

doHardMode:
	lwz r3, 0x2EC(r3)
	li r4, 0x1
	li r5, 0x0
	b UpdateSelectCursor


.data
.global fileSelectLists
fileSelectLists:
	.long W_multiButton_00
	.long W_multiCoin_00
	.long W_multiButton_01
	.long W_multiCoin_01

W_multiButton_00:
	.string "W_multiButton_00"

W_multiCoin_00:
	.string "W_multiCoin_00"

W_multiButton_01:
	.string "W_multiButton_01"

W_multiCoin_01:
	.string "W_multiCoin_01"