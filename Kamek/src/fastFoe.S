.text
#include <asm_setup.S>

.extern doneWithTowerBlockSpeed
.extern doneWithTowerBlockDelay
.extern doneWithCenterSpeed1
.extern doneWithCenterSpeed2
.extern doneWithCenterSpeed3
.extern doneWithChange2CastleMazeSpeed1
.extern doneWithChange2CastleMazeSpeed2
.extern doneWithChange2CastleMazeSpeed3
.extern doneWithDryBonesUpdateRate
.extern doneWithdEnCDoubleSpeedX1
.extern doneWithdEnCDoubleSpeedX2
.extern doneWithdEnCDoubleSpeedY1
.extern doneWithdEnCDoubleSpeedY2
.extern doneWithDoublePipeSpeed
.extern doneWithDoubleCanonSpeed
.extern doneWithDoubleCoinSpeed
.extern doneWithDoubleBulletSpeed
.extern doneWithDoublePlatformSpeed
.extern doneWithDoubleFbaseSpeed3
.extern doneWithDoubleTileAnimSpeed
.extern doneWithDoublePakkunSpeed2
.extern mazeAutoscrollSpeed
.extern processOperation_fBase_c
.extern __ptmf_scall
.extern onExecute__17daEnDpakkunBase_cFv

.extern isEnemyAllowedToBeFast__FP5dEn_cb
.extern setUpdateRate__Q23m3d6banm_cFf

.extern isFastFoe

################
# Tower Blocks #
################

.global TowerBlockSpeed
TowerBlockSpeed:
	lfsx f3, r6, r0

	lis r9, isFastFoe@h
	ori r9, r9, isFastFoe@l
	lbz r9, 0(r9)
	cmpwi r9, 0
	beq returnFromTowerBlockSpeed

	lis r9, floatTwo@h
	ori r9, r9, floatTwo@l
	lfs f2, 0(r9)

	fmuls f3, f3, f2

returnFromTowerBlockSpeed:
	b doneWithTowerBlockSpeed


.global TowerBlockDelay
TowerBlockDelay:
	lis r9, isFastFoe@h
	ori r9, r9, isFastFoe@l
	lbz r9, 0(r9)
	cmpwi r9, 0
	beq TowerBlockDelay_4

	slwi      r6, r6, 3
	slwi      r0, r0, 3
	b returnFromTowerBlockDelay

TowerBlockDelay_4:
	slwi      r6, r6, 4
	slwi      r0, r0, 4

returnFromTowerBlockDelay:
	b doneWithTowerBlockDelay

###############################
# Rotation Controller (Swing) #
###############################

.global CenterSpeed1
CenterSpeed1:
	add r3, r3, r0

	lis r9, isFastFoe@h
	ori r9, r9, isFastFoe@l
	lbz r9, 0(r9)
	cmpwi r9, 0
	beq returnFromCenterSpeed1

	add r3, r3, r0

returnFromCenterSpeed1:
	b doneWithCenterSpeed1


.global CenterSpeed2
CenterSpeed2:
	rlwinm r0, r0, 20,24,27

	lis r9, isFastFoe@h
	ori r9, r9, isFastFoe@l
	lbz r9, 0(r9)
	cmpwi r9, 0
	beq returnFromCenterSpeed2

	#srwi r0, r0, 1
	li r0, 0

returnFromCenterSpeed2:
	b doneWithCenterSpeed2


.global CenterSpeed3
CenterSpeed3:
	rlwinm r0, r0, 20,24,27

	lis r9, isFastFoe@h
	ori r9, r9, isFastFoe@l
	lbz r9, 0(r9)
	cmpwi r9, 0
	beq returnFromCenterSpeed3

	#srwi r0, r0, 1
	li r0, 0

returnFromCenterSpeed3:
	b doneWithCenterSpeed3

#############
# Dry Bones #
#############

.global DryBonesUpdateRate
DryBonesUpdateRate:
	lis r9, isFastFoe@h
	ori r9, r9, isFastFoe@l
	lbz r9, 0(r9)
	cmpwi r9, 0
	beq returnFromDryBonesUpdateRate
	cmpwi r30, 1
	beq returnFromDryBonesUpdateRate
	cmpwi r30, 2
	beq returnFromDryBonesUpdateRate

	addi r3, r29, 0x70C
	lis r9, floatTwo@h
	ori r9, r9, floatTwo@l
	lfs f1, 0(r9)
	bl setUpdateRate__Q23m3d6banm_cFf

returnFromDryBonesUpdateRate:
	lwz r31, 0x1C(sp)
	b doneWithDryBonesUpdateRate

#########
# dEn_c #
#########

.global dEnCDoubleSpeedX1
dEnCDoubleSpeedX1:
	lfs f1, 0xE8(r3)

	lis r9, floatTwo@h
	ori r9, r9, floatTwo@l
	lfs f4, 0(r9)

	li r4, 0
	bl isEnemyAllowedToBeFast__FP5dEn_cb
	cmpwi r3, 0
	beq returnFromdEnCDoubleSpeedX1

	fmuls f1, f1, f4

returnFromdEnCDoubleSpeedX1:
	mr r3, r30
	b doneWithdEnCDoubleSpeedX1

.global dEnCDoubleSpeedX2
dEnCDoubleSpeedX2:
	lfs f0, 0xE8(r30)

	lis r9, floatTwo@h
	ori r9, r9, floatTwo@l
	lfs f4, 0(r9)

	mr r3, r30
	li r4, 0
	bl isEnemyAllowedToBeFast__FP5dEn_cb
	cmpwi r3, 0
	beq returnFromdEnCDoubleSpeedX2

	fmuls f0, f0, f4

returnFromdEnCDoubleSpeedX2:
	b doneWithdEnCDoubleSpeedX2

.global dEnCDoubleSpeedY1
dEnCDoubleSpeedY1:
	lfs f2, 0xEC(r30)

	lis r9, floatTwo@h
	ori r9, r9, floatTwo@l
	lfs f4, 0(r9)

	mr r3, r30
	li r4, 1
	bl isEnemyAllowedToBeFast__FP5dEn_cb
	cmpwi r3, 0
	beq returnFromdEnCDoubleSpeedY1

	fmuls f2, f2, f4

returnFromdEnCDoubleSpeedY1:
	b doneWithdEnCDoubleSpeedY1

.global dEnCDoubleSpeedY2
dEnCDoubleSpeedY2:
	lfs f1, 0xEC(r30)

	lis r9, floatTwo@h
	ori r9, r9, floatTwo@l
	lfs f4, 0(r9)

	mr r3, r30
	li r4, 1
	bl isEnemyAllowedToBeFast__FP5dEn_cb
	cmpwi r3, 0
	beq returnFromdEnCDoubleSpeedY2

	fmuls f1, f1, f4

returnFromdEnCDoubleSpeedY2:
	b doneWithdEnCDoubleSpeedY2

########
# Pipe #
########

.global DoublePipeSpeed
DoublePipeSpeed:
	lis r8, isFastFoe@h
	ori r8, r8, isFastFoe@l
	lbz r8, 0(r8)

	cmpwi r8, 1
	bne noDoublePipeSpeed

	lis r8, floatTwo@h
	ori r8, r8, floatTwo@l
	lfs f5, 0(r8)
	fmuls f6, f5, f6

noDoublePipeSpeed:
	stfs f6, 0x5D4(r3)
	b doneWithDoublePipeSpeed

#################
# Cannon/Bullet #
#################


.global DoubleCanonSpeed
DoubleCanonSpeed:
	lis r4, isFastFoe@ha
	lbz r4, isFastFoe@l(r4)

	cmpwi r4, 1
	bne noDoubleCanonSpeed

	lis r4, floatOne@h
	ori r4, r4, floatOne@l
	lfs f0, 0(r4)
	b doneWithDoubleCanonSpeed

noDoubleCanonSpeed:
	lis r4, floatPointFive@h
	ori r4, r4, floatPointFive@l
	lfs f0, 0(r4)
	b doneWithDoubleCanonSpeed


.global DoubleBulletSpeed
DoubleBulletSpeed:
	lis r4, isFastFoe@ha
	lbz r4, isFastFoe@l(r4)

	cmpwi r4, 1
	bne noDoubleBulletSpeed

	lis r4, floatTwo@h
	ori r4, r4, floatTwo@l
	lfs f3, 0(r4)
	fmuls f2, f3, f2

noDoubleBulletSpeed:
	stfs f2, 0xE8(r28)
	b doneWithDoubleBulletSpeed

##############
# SpeedToPos #
##############

#.global DoubleSpeedToPos
#DoubleSpeedToPos:
#	lis r4, isFastFoe@h
#	ori r4, r4, isFastFoe@l
#	lbz r4, 0(r4)
#
#	cmpwi r4, 1
#
#	lis r4, floatTwo@h
#	ori r4, r4, floatTwo@l
#	lfs f2, 0(r4)
#
#	addi r4, r3, 0xE8
#	bne noDoubleSpeedToPos
#
#	lfs f1, 0(r4)
#	fmuls f1, f2, f1
#	stfs f1, 0(r4)
#
#	lfs f1, 4(r4)
#	fmuls f1, f2, f1
#	stfs f1, 4(r4)
#
#	lfs f1, 8(r4)
#	fmuls f1, f2, f1
#	stfs f1, 8(r4)
#
#noDoubleSpeedToPos:
#	b doneWithDoubleSpeedToPos


###########
# Podoboo #
###########

.global DoublePodobooSpeed
DoublePodobooSpeed:
	lis r4, isFastFoe@ha
	lbz r4, isFastFoe@l(r4)

	cmpwi r4, 1
	bne noDoublePodobooSpeed

	lfs f1, 0xEC(r30)
	lis r4, floatTwo@h
	lfs f2, floatTwo@l(r4)
	fmuls f1, f1, f2
	stfs f1, 0xEC(r30)

noDoublePodobooSpeed:
	lwz r4, 0xB58(r30) # Restore instruction
	blr

.global DoublePodobooInc
DoublePodobooInc:
	lis r5, isFastFoe@ha
	lbz r5, isFastFoe@l(r5)

	cmpwi r5, 1
	bne noDoublePodobooInc

	lis r5, floatFour@h
	lfs f4, floatFour@l(r5)
	fmuls f3, f3, f4

noDoublePodobooInc:
	stfs f3, 0x114(r30) # Restore instruction
	blr


.global HalfPodobooDelay
HalfPodobooDelay:
	lis r4, isFastFoe@ha
	lbz r4, isFastFoe@l(r4)

	cmpwi r4, 1
	bne noHalfPodobooDelay

	mulli     r0, r0, 30
	blr
	
noHalfPodobooDelay:
	mulli     r0, r0, 60
	blr


#########
# Coins #
#########

.global DoubleCoinSpeed
DoubleCoinSpeed:
	lis r6, isFastFoe@ha
	lbz r6, isFastFoe@l(r6)

	cmpwi r6, 1
	bne noDoubleCoinSpeed

	addi r3, r3, 2
	b doneWithDoubleCoinSpeed

noDoubleCoinSpeed:
	addi r3, r3, 1
	b doneWithDoubleCoinSpeed


###################
# Basic Platforms #
###################

.global DoublePlatformSpeed
DoublePlatformSpeed:
	lis r4, isFastFoe@ha
	lbz r4, isFastFoe@l(r4)

	cmpwi r4, 1
	bne noDoublePlatformSpeed
	fadds f3, f3, f6

noDoublePlatformSpeed:
	fadds f3, f3, f6
	b doneWithDoublePlatformSpeed
	

##########
# Pakkun #
##########

.global DoublePakkunSpeed1
DoublePakkunSpeed1:
	mr r30, r3 # Restore instruction

	stw r29, 0x4(sp)
	mr r29, r4,
	blr

.global DoublePakkunSpeed2
DoublePakkunSpeed2:
	lis r4, isFastFoe@ha
	lbz r4, isFastFoe@l(r4)

	cmpwi r4, 1
	bne noDoublePakkunSpeed

	cmpwi r29, 0
	bne noDoublePakkunSpeed

	mr r3, r30
	li r4, 1
	bl onExecute__17daEnDpakkunBase_cFv

noDoublePakkunSpeed:
	lwz r29, 0x4(sp)
	lwz r31, 0xC(sp) # Restore instruction
	b doneWithDoublePakkunSpeed2


###################
# fBase_c hackery #
###################

.global DoubleFbaseSpeed1
DoubleFbaseSpeed1:
	mr r28, r3 # Restore Instruction
	stw r27, 0xC(sp)
	mr r27, r7
	stw r26, 0x8(sp)
	blr

.global DoubleFbaseSpeed2
DoubleFbaseSpeed2:
	mr r3, r31 # Restore Instruction
	lwz r27, 0xC(sp)
	lwz r26, 0x8(sp)
	blr

.global DoubleFbaseSpeed3
DoubleFbaseSpeed3:
	cmpwi r27, 0
	beq noDoubleFbaseSpeed3

	mr r26, r3

	mr r3, r28
	bl isEnemyAllowedToBeFast__FP5dEn_cb
	cmpwi r3, 1
	mr r3, r26
	bne noDoubleFbaseSpeed3

	mr r3, r28
	mr r12, r29
	bl __ptmf_scall
	nop

noDoubleFbaseSpeed3:
	cmpwi r3, 0 # Restore Instruction
	b doneWithDoubleFbaseSpeed3

.global DontDoubleFbase
DontDoubleFbase:
	li r7, 0
	b processOperation_fBase_c

.global DoDoubleFbase
DoDoubleFbase:
	li r7, 1
	b processOperation_fBase_c



#######################
# 2-Castle Autoscroll #
#######################

.global Change2CastleMazeSpeed1
Change2CastleMazeSpeed1:
	lis r4, isFastFoe@ha
	lbz r4, isFastFoe@l(r4)

	cmpwi r4, 1
	bne noDoubleCastleMazeSpeed1

	lis r4, floatOnePointTwo@ha
	lfs f0, floatOnePointTwo@l(r4)
	b doneWithChange2CastleMazeSpeed1

noDoubleCastleMazeSpeed1:
	lis r4, mazeAutoscrollSpeed@ha
	lfs f0, mazeAutoscrollSpeed@l(r4)
	b doneWithChange2CastleMazeSpeed1

.global Change2CastleMazeSpeed2
Change2CastleMazeSpeed2:
	lis r6, isFastFoe@ha
	lbz r6, isFastFoe@l(r6)

	cmpwi r6, 1
	bne noDoubleCastleMazeSpeed2
	
	lis r6, floatOnePointTwo@ha
	lfs f1, floatOnePointTwo@l(r6)
	b doneWithChange2CastleMazeSpeed2

noDoubleCastleMazeSpeed2:
	lis r6, mazeAutoscrollSpeed@ha
	lfs f1, mazeAutoscrollSpeed@l(r6)
	b doneWithChange2CastleMazeSpeed2

.global Change2CastleMazeSpeed3
Change2CastleMazeSpeed3:
	lis r6, isFastFoe@ha
	lbz r6, isFastFoe@l(r6)

	cmpwi r6, 1
	bne noDoubleCastleMazeSpeed3
	
	lis r6, floatOnePointTwo@ha
	lfs f0, floatOnePointTwo@l(r6)
	b doneWithChange2CastleMazeSpeed3

noDoubleCastleMazeSpeed3:
	lis r6, mazeAutoscrollSpeed@ha
	lfs f0, mazeAutoscrollSpeed@l(r6)
	b doneWithChange2CastleMazeSpeed3


###########################################
# Double Anim Tile Speed (Thanks to Nin0) #
###########################################

.global DoubleTileAnimSpeed
DoubleTileAnimSpeed:
	lis r7, isFastFoe@ha
	lbz r7, isFastFoe@l(r7)

	cmpwi r7, 1
	bne noDoubleTileAnimSpeed

	addi r0, r4, 2
	b doneWithDoubleTileAnimSpeed

noDoubleTileAnimSpeed:
	addi r0, r4, 1
	b doneWithDoubleTileAnimSpeed



.data
.global floatPointFive
floatPointFive:
	.float 0.5

.global floatOne
floatOne:
	.float 1.0

.global floatTwo
floatTwo:
	.float 2.0

.global floatFour
floatFour:
	.float 4.0

.global floatOnePointTwo
floatOnePointTwo:
	.float 1.2