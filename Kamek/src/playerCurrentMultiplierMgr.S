.text
.align 4

#include <asm_setup.S>

.extern doesPlayerCurrentMultiplierMgrExist__Fv
.extern currentMultiplier
.extern currentMultiplier2
.extern returnFromWaterMultiplier
.extern returnFromWaterMultiplier2
.extern returnFromWaterMultiplier3

.global setWaterMultiplier
setWaterMultiplier:
	stwu sp, -0x20(sp)
	mflr r0
	stw r0, 0x24(sp)
	stw r3, 0x1C(sp)
	stw r4, 0x18(sp)
	stw r5, 0x14(sp)
	stw r6, 0x10(sp)
	stw r7, 0x6(sp)

	bl		doesPlayerCurrentMultiplierMgrExist__Fv
	cmpwi   r3, 0
	beq		restore
	lis r3, currentMultiplier@h
	ori r3, r3, currentMultiplier@l
	lfs		f0, 0(r3)

restore:
	fmuls	f0, f0, f1	#restored instruction
	lwz r7, 0x6(sp)
	lwz r6, 0x10(sp)
	lwz r5, 0x14(sp)
	lwz r4, 0x18(sp)
	lwz r3, 0x1C(sp)
	lwz r0, 0x24(sp)
	mtlr r0
	addi sp, sp, 0x20
	b		returnFromWaterMultiplier


.global setWaterMultiplier2
setWaterMultiplier2:
	stwu sp, -0x20(sp)
	mflr r0
	stw r0, 0x24(sp)
	stw r3, 0x1C(sp)
	stw r4, 0x18(sp)
	stw r5, 0x14(sp)
	stw r6, 0x10(sp)
	stw r7, 0x6(sp)

	bl		doesPlayerCurrentMultiplierMgrExist__Fv
	cmpwi   r3, 0
	beq		restore2
	lis r3, currentMultiplier2@h
	ori r3, r3, currentMultiplier2@l
	lfs		f0, 0(r3)

restore2:
	fmuls	f0, f0, f3	#restored instruction
	lwz r7, 0x6(sp)
	lwz r6, 0x10(sp)
	lwz r5, 0x14(sp)
	lwz r4, 0x18(sp)
	lwz r3, 0x1C(sp)
	lwz r0, 0x24(sp)
	mtlr r0
	addi sp, sp, 0x20
	b		returnFromWaterMultiplier2


.global setWaterMultiplier3
setWaterMultiplier3:
	stwu sp, -0x20(sp)
	mflr r0
	stw r0, 0x24(sp)
	stw r3, 0x1C(sp)
	stw r4, 0x18(sp)
	stw r5, 0x14(sp)
	stw r6, 0x10(sp)
	stw r7, 0x6(sp)

	bl		doesPlayerCurrentMultiplierMgrExist__Fv
	cmpwi   r3, 0
	
	lis r3, origSpeed@h
	ori r3, r3, origSpeed@l
	lfs		f5, 0(r3) 					#restored instruction
	
	beq		restore3
	lis r3, currentMultiplier@h
	ori r3, r3, currentMultiplier@l
	lfs		f3, 0(r3)

	lis r3, penguinAdd@h
	ori r3, r3, penguinAdd@l
	lfs		f5, 0(r3)
	fmuls	f3, f3, f5
	
	fmuls	f5, f0, f3					#f0 is current, f3 is multiplier, f5 is 8.25f replacement)

restore3:
	lwz r7, 0x6(sp)
	lwz r6, 0x10(sp)
	lwz r5, 0x14(sp)
	lwz r4, 0x18(sp)
	lwz r3, 0x1C(sp)
	lwz r0, 0x24(sp)
	mtlr r0
	addi sp, sp, 0x20
	b		returnFromWaterMultiplier3

.align 4
.data
.global origSpeed
origSpeed: .float 8.25
.global penguinAdd
penguinAdd: .float 10.0