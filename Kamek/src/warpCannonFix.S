.text
.align 4

#include <asm_setup.S>

.extern returnFromCheckWarpCannon1 #warp cannon
.extern returnFromCheckWarpCannon2 #not warp cannon
.extern goToSpecificWorld__Fcc

.global checkWarpCannon
checkWarpCannon:
	lis		r3, shootIntoWorldNext@h
	ori		r3, r3, shootIntoWorldNext@l
	lbz     r4, 0(r3)
	cmpwi	r4, 0
	beq		noWarp
	
	li		r4, 0
	stb		r4, 0x0(r3)					#shootIntoWorldNext = false
	
	lis		r3, destinationWorld@h
	ori		r3, r3, destinationWorld@l
	lbz		r3, 0x0(r3)

	lis		r4, destinationSubWorld@h
	ori		r4, r4, destinationSubWorld@l
	lbz		r4, 0x0(r4)
	
	bl		goToSpecificWorld__Fcc
	
	lwz		r5, -0x5724 (r13)			#restored instructions, maybe about "save after entering world"
	li		r0, 0x3
	stw		r0, 0x370(r5)
	
	li		r0, 6						#store currentLevelNode
	stw		r0, 0x44(r5)
	
	b		returnFromCheckWarpCannon1

noWarp:
	li		r3, 0
	b		returnFromCheckWarpCannon2


.data
.align 4
.global destinationWorld
destinationWorld: .byte 0

.global destinationSubWorld
destinationSubWorld: .byte 0

.global shootIntoWorldNext
shootIntoWorldNext: .byte 0