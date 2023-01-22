#include <asm_setup.S>

.text

.extern doneWithBounceShell
.extern doneWithSpringboardShell
.extern doneWithDoCloudBounceAnim
.extern actuallyDoCloudBounceAnim
.extern doneWithNoPlayerIce
.extern doGPSlide
.extern doneWithNoPlayerGP
.extern doneWithNoPlayerGP2
.extern doneWithNoPlayerFire
.extern doneWithNoPlayerSpin
.extern doneWithNoPlayerSpin2
.extern doneWithNoPlayerSlide
.extern doneWithNoPlayerSlide2
.extern unk_80353CD8
.extern _savegpr_26
.extern doneWithGPSlide
.extern shellSpriteCollision__FP5dEn_cP13ActivePhysicsP13ActivePhysics

.global BounceShell
BounceShell:
	mr r5, r3

	lwz r3, 0x244(r31)
	cmpwi r3, 0
	beq returnBounceShell

	lwz r3, 0(r3)
	cmpwi r3, 0
	beq returnBounceShell
	
	mr r4, r3
	lhz r3, 8(r3)
	addi r0, r3, -0x1D2
	cntlzw r0, r0
	srwi r3, r0, 5

	cmpwi r3, 0
	beq returnBounceShell

	lis r3, float5p5@h
	ori r3, r3, float5p5@l
	lfs f0, 0(r3)

	li r3, 1
	stb r3, 0x548(r4)

returnBounceShell:
	mr r3, r5
	stfs f0, 0xEC(r31) # Restore Instruction
	b doneWithBounceShell


.global SpringboardShell
SpringboardShell:
	mr r3, r28
	mr r4, r29
	mr r5, r30
	bl shellSpriteCollision__FP5dEn_cP13ActivePhysicsP13ActivePhysics
	
	lhz r0, 0x38(r30)
	b doneWithSpringboardShell


.global BounceGoomba
BounceGoomba:
	mr r4, r3
	lhz r3, 8(r3)
	addi r0, r3, -0x1D2
	cntlzw r0, r0
	srwi r3, r0, 5

	cmpwi r3, 0
	beq BounceGoombaReturn

	stb r3, 0x548(r4)

BounceGoombaReturn:
	blr


.global DoCloudBounceAnim
DoCloudBounceAnim:
	lbz r3, 0x548(r27)
	cmpwi r3, 1
	bne noCloudBounceAnim
	li r29, 0
	stb r29, 0x548(r27)
	b actuallyDoCloudBounceAnim

noCloudBounceAnim:
	mr r3, r27 # Restore Instruction
	b doneWithDoCloudBounceAnim


.global NoPlayerGP
NoPlayerGP:
	lis r3, isInItemBowling@h
	ori r3, r3, isInItemBowling@l
	lwz r3, 0(r3)
	cmpwi r3, 0
	mr r3, r31 # Restore Instruction
	bne dontPlayerGP
	b doneWithNoPlayerGP2

dontPlayerGP:
	b doneWithNoPlayerGP


.global NoPlayerFire
NoPlayerFire:
	slwi r6, r3, 2 # Restore Instruction
	lis r3, isInItemBowling@h
	ori r3, r3, isInItemBowling@l
	lwz r3, 0(r3)
	cmpwi r3, 0
	bne NoPlayerFireReturn
	b doneWithNoPlayerFire
	
NoPlayerFireReturn:
	li r3, 0
	blr


.global NoPlayerIce
NoPlayerIce:
	slwi r6, r3, 2 # Restore Instruction
	lis r3, isInItemBowling@h
	ori r3, r3, isInItemBowling@l
	lwz r3, 0(r3)
	cmpwi r3, 0
	bne NoPlayerIceReturn
	b doneWithNoPlayerIce

NoPlayerIceReturn:
	li r3, 0
	blr


.global NoPlayerSpin
NoPlayerSpin:
	lis r3, isInItemBowling@h
	ori r3, r3, isInItemBowling@l
	lwz r3, 0(r3)
	cmpwi r3, 0
	bne NoPlayerSpinReturn

	lwz r0, 0x10D4(r31) # Restore Instruction
	b doneWithNoPlayerSpin

NoPlayerSpinReturn:
	li r3, 0
	b doneWithNoPlayerSpin2


.global NoPlayerSlide
NoPlayerSlide:
	cmpwi r3, 0
	beq NoPlayerSlideReturn

	lis r3, isInItemBowling@h
	ori r3, r3, isInItemBowling@l
	lwz r3, 0(r3)
	cmpwi r3, 0
	bne NoPlayerSlideReturn

	b doneWithNoPlayerSlide

NoPlayerSlideReturn:
	b doneWithNoPlayerSlide2


.global GPSlide
GPSlide:
	lis r6, isInItemBowling@h
	ori r6, r6, isInItemBowling@l
	lwz r6, 0(r6)
	cmpwi r6, 0
	bne doGPSliding

	stwu r1, -0x40(r1) # Restore Instruction
	b doneWithGPSlide

doGPSliding:
	stwu r1, -0x30(r1)
	mflr r0
	stw r0, 0x34(r1)
	addi r11, r1, 48
	bl _savegpr_26

	mr r27, r3
	mr r28, r4
	mr r29, r5
	lwz r30, 4(r5)
	lis r31, unk_80353CD8@h
	ori r31, r31, unk_80353CD8@l

	b doGPSlide


.data
.align 4
.global float5p5
float5p5: .float 8.5

.global isInItemBowling
isInItemBowling: .long 0