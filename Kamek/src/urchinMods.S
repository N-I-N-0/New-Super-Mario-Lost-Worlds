#include <asm_setup.S>

.text
.align 4
.extern GetTexFilenameForR5 #spriteTex

.global TEX_Urchin
TEX_Urchin:
	#cmpwi	r0, 0
	#beq		doUrchinSpriteTex
	#lwzx	r5, r5, r0
	#blr
#doUrchinSpriteTex:						do for both small and big one (Maw-Ray for small, Boo for big)
	lwz		r5, 4(r30)
	srwi	r5, r5, 24
	andi.	r5, r5, 0xF
	b		GetTexFilenameForR5


.extern returnToBigUrchinFireballReal
.extern continueForNormalUrchinFireball
.global Urchin_FireballNone
Urchin_FireballNone:
	bne returnToBigUrchinFireball
	lwz		r4, 4(r29)
	srwi	r4, r4, 24
	andi.	r4, r4, 0xF
	cmpwi	r4, 1
	beq		doMawRayFireball
	b		continueForNormalUrchinFireball

doMawRayFireball:
returnToBigUrchinFireball:
	b		returnToBigUrchinFireballReal


.extern returnToBigUrchinIceballReal
.extern continueForNormalUrchinIceball
.global Urchin_IceballNone
Urchin_IceballNone:
	bne returnToBigUrchinIceball
	lwz		r0, 4(r3)
	srwi	r0, r0, 24
	andi.	r0, r0, 0xF
	cmpwi	r0, 1
	beq		doMawRayIceball
	b		continueForNormalUrchinIceball

doMawRayIceball:
returnToBigUrchinIceball:
	b		returnToBigUrchinIceballReal


.extern returnForMawRayStar
.extern continueForNormalUrchinStar
.global Urchin_StarNone
Urchin_StarNone:
	lwz		r29, 4(r3)
	srwi	r29, r29, 24
	andi.	r29, r29, 0xF
	cmpwi	r29, 1
	mr		r29, r3
	beq		doMawRayStar
	b		continueForNormalUrchinStar

doMawRayStar:
	li		r3, 0
	b		returnForMawRayStar


.extern continueUrchinTurn
.extern continueUrchinNoTurn
.global Urchin_dontTurn
Urchin_dontTurn:
	lwz		r3, 4(r30)
	srwi	r3, r3, 24
	andi.	r3, r3, 0xF
	cmpwi	r3, 1
	mr		r3, r30
	cmpwi	r3, 1
	beq		doMawRayDontTurn
	b		continueUrchinTurn
doMawRayDontTurn:
	b		continueUrchinNoTurn

.extern collisionCat13_Hammer__5dEn_cFP13ActivePhysicsP13ActivePhysics
.extern UrchinHammer__FP5dEn_cP13ActivePhysicsP13ActivePhysics
.global Urchin_MawRayNoHammer
Urchin_MawRayNoHammer:
	stwu	sp, -0x10(sp)
	stw		r3, 0x4(sp)
	
	lwz		r3, 4(r3)
	srwi	r3, r3, 24
	andi.	r3, r3, 0xF
	cmpwi	r3, 1
	
	lwz		r3, 0x4(sp)
	addi	sp, sp, 0x10
	
	beq		noHammerBecauseMawRay
	b		collisionCat13_Hammer__5dEn_cFP13ActivePhysicsP13ActivePhysics
noHammerBecauseMawRay:
	b		UrchinHammer__FP5dEn_cP13ActivePhysicsP13ActivePhysics


.extern continueUrchinOnCreate
.extern setCustomState__12daEnUnizoo_cFv
.global daEnUnizoo_c__setCustomState
daEnUnizoo_c__setCustomState:
	mr r3, r28
	bl setCustomState__12daEnUnizoo_cFv
	
	addi r11, sp, 0x30
	b continueUrchinOnCreate