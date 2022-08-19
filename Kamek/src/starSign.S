.text
.align 4
#include <asm_setup.S>

.extern AndR5WithNodeR4Props

.extern doneWithAddSignFlag
.extern doneWithSpawnStarSign
.extern doneWithSpawnStarSign2
.extern doneWithSpawnStarSign3
.extern strcmp
.extern SpawnWorldmapObject
.extern sub_801007D0
.extern doneWithPreserveR5
.extern doneWithLoadStarSign
.extern doneWithDoSomeCheating4
.extern doneWithDoSomeCheating5
.extern GetPositionOfWorldmapBone
.extern doneWithDoW8Checks
.extern doneWithDoW8Checks2
.extern getCollectedPowerStars__Fv
.extern StateID_LetterOpenAnim__22dCourseSelectManager_c
.extern doneWithCSMgrCheckForSignByte
.extern doneWithCSMgrCheckForSignByte2


.global AddSignFlag
AddSignFlag:
	bne doSignComparison

	# Restore Instructions
	lwz		r0, 0xC(r24)
	mulli	r0, r0, 0x1C0
	add		r3, r24, r0
	lwz		r0, 0x1C8(r3)
	oris	r0, r0, 0x8000
	stw		r0, 0x1C8(r3)

	b doneWithAddSignFlag

doSignComparison:
	mr		r3, r29
	lis		r4, signString@h
	ori		r4, r4, signString@l
	bl		strcmp

	cmpwi	r3, 0
	bne		_doneWithAddSignFlag

	lwz		r0, 0xC(r24)
	mulli	r0, r0, 0x1C0
	add		r3, r24, r0
	lwz		r0, 0x1C8(r3)
	ori		r0, r0, 0x200 # Literally the only flag that wasn't used
	stw		r0, 0x1C8(r3)

_doneWithAddSignFlag:
	b doneWithAddSignFlag


.global SpawnStarSign
SpawnStarSign:
	beq		checkForStarFlag
	b		doneWithSpawnStarSign
checkForStarFlag:
	lwz		r3, 0x338C(r21)
	addi	r0, r29, 0x6518
	mr		r4, r22
	li		r5, 0x200 # Our brand new sign flag
	mullw	r0, r3, r0
	add		r3, r21, r0
	addi	r3, r3, 0x3394
	bl		AndR5WithNodeR4Props

	cmpwi	r3, 0
	beq		_doneWithSpawnStarSign2

	li		r3, 0x2A5 #756 # WM_STARSIGN
	lbz		r4, 3(r31)

	addi	r0, r4, -0x31
	clrlwi	r4, r0, 24
	ori		r4, r4, 0x10

	mr		r5, r20

	#lis r6, float35@ha
	#lfs f1, float35@l(r6)
	#lis r6, posVec@h
	#ori r6, r6, posVec@l

	#lfs f0, 0(r5)
	#stfs f0, 0(r6)
	#lfs f0, 4(r5)
	#fadds f0, f0, f1
	#stfs f0, 4(r6)
	#lfs f0, 8(r5)
	#stfs f0, 8(r6)

	#mr r5, r6

	li		r6, 0
	bl		SpawnWorldmapObject

	mr		r23, r3
	b doneWithSpawnStarSign3


_doneWithSpawnStarSign2:
	b doneWithSpawnStarSign2


.global PreserveR5
PreserveR5:
	mr r20, r5
	bl sub_801007D0
	b doneWithPreserveR5


.global LoadStarSign
LoadStarSign:
	li		r0, 3
	stw		r0, 0(r3)
	lis		r4, W8LoadTable@h
	ori		r4, r4, W8LoadTable@l
	b		doneWithLoadStarSign


# Changes the arc filename to use depending of the new setting
.global DoSomeCheating1
DoSomeCheating1:
	lwz		r9, 4(r28)
	rlwinm	r9, r9, 28, 31, 31 # ((r27 >> 4) & 1)
	cmpwi	r9, 1
	beq		doCheating1
	lis		r4, cobBoard@h
	ori		r4, r4, cobBoard@l
	blr
doCheating1:
	lis		r4, cobSign@h
	ori		r4, r4, cobSign@l
	blr


# Changes the mdl0 filename to use depending of the new setting
.global DoSomeCheating2
DoSomeCheating2:
	lwz		r9, 4(r28)
	rlwinm	r9, r9, 28, 31, 31 # ((r9 >> 4) & 1)
	cmpwi	r9, 1
	beq		doCheating2
	lis		r4, cobBoard@h
	ori		r4, r4, cobBoard@l
	blr
doCheating2:
	lis		r4, cobSign@h
	ori		r4, r4, cobSign@l
	blr


# Changes the texsrt filename to use depending of the new setting
.global DoSomeCheating3
DoSomeCheating3:
	lwz		r9, 4(r28)
	rlwinm	r9, r9, 28, 31, 31 # ((r9 >> 4) & 1)
	cmpwi	r9, 1
	beq		doCheating3
	lis		r4, cobBoard@h
	ori		r4, r4, cobBoard@l
	blr
doCheating3:
	lis		r4, cobSign@h
	ori		r4, r4, cobSign@l
	blr


# Changes the visibility depending of the new setting to prevent being deleted due to collecting all of a world's coins, plus check if collected stars are >=35
.global DoSomeCheating4
DoSomeCheating4:
	lwz		r3, 4(r31)
	rlwinm	r3, r3, 28, 31, 31 # ((r31 >> 4) & 1)
	cmpwi	r3, 1
	bne		dontCheat4

	bl		getCollectedPowerStars__Fv
	li		r0, 0
	cmpwi	r3, 35
	bge		dontCheat4
	li		r0, 1

dontCheat4:
	stb		r0, 0x124(r31)
	b		doneWithDoSomeCheating4


# Depending of the new setting, make the sign be at the first bridge piece's position (padded by 3.5 on Y and by 60.0 on Z)
.global DoSomeCheating5
DoSomeCheating5:
	addi	r5, r30, 0xAC # Restore instruction

	lwz		r6, 4(r30)
	rlwinm	r6, r6, 28, 31, 31 # ((r30 >> 4) & 1)
	cmpwi	r6, 1
	bne		dontCheat5

	lis		r4, bridge00@h
	ori		r4, r4, bridge00@l
	bl		GetPositionOfWorldmapBone

	lfs		f0, 0xB0(r30)
	lis		r6, float35@ha
	lfs		f1, float35@l(r6)
	fadds	f0, f0, f1
	stfs	f0, 0xB0(r30)

	lfs		f0, 0xB4(r30)
	lis		r6, float60@ha
	lfs		f1, float60@l(r6)
	fadds	f0, f0, f1
	stfs	f0, 0xB4(r30)

	b		doneWithDoSomeCheating5
dontCheat5:
	bl		GetPositionOfWorldmapBone
	b		doneWithDoSomeCheating5
	blr


.global DoW8Checks
DoW8Checks:
	lis		r31, 1 # Restore instruction
	cmpwi	r3, 1
	bne		doW8Check
	b		doneWithDoW8Checks
doW8Check:
	lwz		r3, 0x338C(r30)
	addi	r0, r31, 0x6518
	mr		r4, r29
	mullw	r0, r3, r0
	li		r5, 0x200
	add		r3, r30, r0
	addi	r3, r3, 0x3394
	bl		AndR5WithNodeR4Props

	cmpwi	r3, 0
	bne		checkForStarCount
_enoughStars:
	li		r3, 0
	b		doneWithDoW8Checks2
checkForStarCount:
	bl		getCollectedPowerStars__Fv
	cmpwi	r3, 35
	bge		_enoughStars
	li		r3, 2
	b		doneWithDoW8Checks2


.global CSMgrCheckForSignByte
CSMgrCheckForSignByte:
	lbz		r0, 0x57B(r3)
	cmpwi	r0, 0
	beq		Byte578_is_0

	lis		r4, StateID_LetterOpenAnim__22dCourseSelectManager_c@h
	ori		r4, r4, StateID_LetterOpenAnim__22dCourseSelectManager_c@l
	lwzu	r12, 0x70(r3)
	lwz		r12, 0x18(r12)
	mtctr	r12
	bctrl
	b		doneWithCSMgrCheckForSignByte2

Byte578_is_0:
	lbz		r0, 0x53E(r3) # Restore instruction
	b		doneWithCSMgrCheckForSignByte


.global HoldBoardReturnValue
HoldBoardReturnValue:
	lis		r4, BoardReturnHolder@ha
	stw		r3, BoardReturnHolder@l(r4)
	lwz		r4, 0x22C(r31) # Restore instruction
	blr


.global ChangeCSMgrByte
ChangeCSMgrByte:
	lis		r5, BoardReturnHolder@ha
	lwz		r5, BoardReturnHolder@l(r5)
	cmpwi	r5, 2
	beq		useNewByte
	stb		r0, 0x544(r4)
	blr
useNewByte:
	stb		r0, 0x57B(r4)
	blr


.data
.align 4

.global signString
signString: .string "sign"
.align 4

.global cobKoopaCastle
cobKoopaCastle: .string "cobKoopaCastle"
.align 4

.global cobfirepillar
cobfirepillar: .string "cobfirepillar"
.align 4

.global cobSign
cobSign: .string "cobSign"
.align 4

.global cobBoard
cobBoard: .string "cobBoard"
.align 4

.global bridge00
bridge00: .string "bridge00"
.align 4

.global W8LoadTable
W8LoadTable:
	.long cobKoopaCastle
	.long cobfirepillar
	.long cobSign

float35: .float 3.5
float60: .float 60.0
posVec: .float 0.0, 0.0, 0.0

BoardReturnHolder: .long 0