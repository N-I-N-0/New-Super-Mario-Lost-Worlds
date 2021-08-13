.text
.align 4

#include <asm_setup.S>

.extern continueIsWXS1
.extern continueWithPat0SetupFromWXS0
.extern LAB_808e81d4
.extern StartArrowRotation
.extern continueNodeSetup

#808e81bc
.global isWXS1
isWXS1:
	bne		isWXS2
	b		continueIsWXS1					#808e81c0

isWXS2:
	cmpwi	r0,0x2
	bne		isWXS3
	lis		r4, TwoFloat@h
	lfs		f1, TwoFloat@l(r4)
	addi	r3, r29, 0x1b8
	li		r4, 0x0
	b		continueWithPat0SetupFromWXS0	#808e81b0

isWXS3:
	cmpwi	r0,0x3
	bne		notWXS3
	lis		r4, ThreeFloat@h
	lfs		f1, ThreeFloat@l(r4)
	addi	r3, r29, 0x1b8
	li		r4, 0x0
	b		continueWithPat0SetupFromWXS0	#808e81b0

notWXS3:
	b		LAB_808e81d4					#808e81d4



.global saveStartNodeRotationForIcon
saveStartNodeRotationForIcon:				#808e819c
	rlwinm.	r0, r0, 0x10, 0x1c, 0x1f
	lis		r3, StartArrowRotation@h
	ori		r3, r3, StartArrowRotation@l
	stb		r0, 0(r3)
	b		continueNodeSetup
	


TwoFloat:
	.float 2.0

ThreeFloat:
	.float 3.0