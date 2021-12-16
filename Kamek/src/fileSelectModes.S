.text
.align 4

#include <asm_setup.S>

.extern UpdateSelectCursor
.extern returnToCoin
.extern returnToMulti
.extern returnFromNewButtons
.extern returnFromNewButtons2
.extern returnFromNewButtons3
.extern ActiveWiimoteID
.extern returnTo80783C4C
.extern returnTo80783CE8

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



.global newSelectableButtonsLR
newSelectableButtonsLR:
	bne notCoinBattle
	b returnToCoin

notCoinBattle:
	cmpwi r29, 0x5
	beq speedIsSelected
	bge hardIsSelected
	b returnToMulti

speedIsSelected:
	lis        r4, ActiveWiimoteID@h
	ori        r4, r4, ActiveWiimoteID@l
    lwz        r0,0x0(r4)
	rlwinm     r0,r0,0x2,0x0,0x1d
    add        r4,r30,r0
	lwz        r4,0x4(r4)
    lwz        r0,0x1c(r4)
	
    rlwinm.    r0,r0,0x0,0x1d,0x1d                   #r0 = r0 & 4 == 0? (right)
	bne        is_right_speed

	li r29, 0x5
	b returnFromNewButtons

is_right_speed:
	li r29, 0x6
	b returnFromNewButtons


hardIsSelected:
	lis        r4, ActiveWiimoteID@h
	ori        r4, r4, ActiveWiimoteID@l
    lwz        r0,0x0(r4)
	rlwinm     r0,r0,0x2,0x0,0x1d
    add        r4,r30,r0
	lwz        r4,0x4(r4)
    lwz        r0,0x1c(r4)

    rlwinm.    r0,r0,0x0,0x1c,0x1c                   #r0 = r0 & 8 == 0? (right)
	bne        is_left_coin

	li r29, 0x6
	b returnFromNewButtons

is_left_coin:
	li r29, 0x5
	b returnFromNewButtons
	


.global newSelectableButtonsDown
newSelectableButtonsDown:
	blt		   cr1, returnTo_LAB_80783CE8

	lis        r4, ActiveWiimoteID@h
	ori        r4, r4, ActiveWiimoteID@l
    lwz        r0,0x0(r4)
	rlwinm     r0,r0,0x2,0x0,0x1d
    add        r4,r30,r0
	lwz        r4,0x4(r4)
    lwz        r0,0x1c(r4)

	rlwinm.    r0,r0,0x0,0x1f,0x1f                   #r0 = r0 & 1 == 0? (right)
	beq        dontMoveDown

	cmpwi      r29, 0x3
	beq        down_to_speed
	cmpwi      r29, 0x4
	beq        down_to_hard

dontMoveDown:
	b          returnTo80783C4C

returnTo_LAB_80783CE8:
	b          returnTo80783CE8

down_to_speed:
	li         r29, 0x5
	b          returnFromNewButtons

down_to_hard:
	li         r29, 0x6
	b          returnFromNewButtons




.global newSelectableButtonsUp
newSelectableButtonsUp:
	stw     r29, 0x2A8(r3)
	cmpwi   r29, 0x5
	beq     up_from_speed
	cmpwi   r29, 0x6
	beq     up_from_hard
	cmpwi   r29, 0x3
	b       returnFromNewButtons3

up_from_speed:
	li      r29, 0x3
	b       returnFromNewButtons

up_from_hard:
	li      r29, 0x4
	b       returnFromNewButtons
	









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