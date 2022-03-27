.text
.align 4

#include <asm_setup.S>
.extern ContinueWithoutCourage
.extern ContinueWithCourage
.extern Actor_SearchByID
.extern doSwallow__9daYoshi_cFv
.extern daYoshi_c__endState_EatNone
.extern Yoshi_EatNoneState
.extern daYoshi_c_sub_8014EE40

.global haveCourageMyCuteYoshi
haveCourageMyCuteYoshi:
	lwz 	r3, 0x4(r28)
	clrlwi	r3, r3, 28					#0 = light blue; 1 = pink; 2 = yellow; 3 = green; 4 = blue; 5 = red; 6 = orange; 7 = brown; 8 = white; 9 = purple.
	cmpwi	r3, 1
	beq		hasCourageColor
	cmpwi	r3, 9
	beq		hasCourageColor
	b		ContinueWithoutCourage

hasCourageColor:
	lwz 	r3, 0x1520(r28)				#eatenID
	li		r4, 0
	bl		Actor_SearchByID
	cmpwi	r3, 0
	beq noCourage
	lhz		r3, 8(r3)
	cmplwi	r3, 54
	beq		hasCourage
	cmplwi	r3, 55
	beq		hasCourage
noCourage:
	b		ContinueWithoutCourage

.extern yoshiEatCheck
.extern yoshiSetEatAction
.extern yoshiFinalizeEat
hasCourage:
	lis		r3, 0
	stw		r3, 0x1520(r28)

	#mr		r3, r28
	#bl		doSwallow__9daYoshi_cFv

	#mr		r3, r28
	#bl		yoshiFinalizeEat

	lis		r4, Yoshi_EatNoneState@h
	mr		r3, r28
	addi	r4, r4, Yoshi_EatNoneState@l
	bl		daYoshi_c_sub_8014EE40

	#mr		r3, r28
	#bl		yoshiEatCheck

	#mr		r3, r28
	#bl		yoshiSetEatAction

	b		ContinueWithCourage


.extern doStarEffect__FP9daYoshi_c
.extern ContinueEatNoneExecute

.global haveStarAnimationMyCuteYoshi
haveStarAnimationMyCuteYoshi:
	mr		r31, r3
	stwu sp, -0x40(sp)
	stw r0, 0x44(sp)
	mflr r0
	stw r0, 0x3C(sp)
	stw r4, 0x38(sp)
	stw r5, 0x34(sp)
	stw r6, 0x30(sp)
	stw r24, 0x2C(sp)
	stw r25, 0x28(sp)
	stw r26, 0x24(sp)
	stw r27, 0x20(sp)
	stw r28, 0x1C(sp)
	stw r29, 0x18(sp)
	stw r30, 0x14(sp)
	stw r31, 0x10(sp)


	lwz 	r3, 0x4(r31)
	clrlwi	r3, r3, 28					#0 = light blue; 1 = pink; 2 = yellow; 3 = green; 4 = blue; 5 = red; 6 = orange; 7 = brown; 8 = white; 9 = purple.
	cmpwi	r3, 1
	beq		hasCourageColor2
	cmpwi	r3, 9
	beq		hasCourageColor2

	mr		r3, r31
	b		EatNoneExecuteStackRestore

hasCourageColor2:
	lwz 	r3, 0x1520(r31)				#eatenID
	li		r4, 0
	bl		Actor_SearchByID
	cmpwi	r3, 0
	beq		noCourage2
	lhz		r3, 8(r3)
	cmplwi	r3, 54
	beq		hasCourage2
	cmplwi	r3, 55
	beq		hasCourage2

noCourage2:
	b		EatNoneExecuteStackRestore

hasCourage2:
	#lbz		r3, 0x1541(r31)				#somePlayerID
	mr		r3, r31
	bl		doStarEffect__FP9daYoshi_c
	
EatNoneExecuteStackRestore:
	lwz r31, 0x10(sp)
	lwz r30, 0x14(sp)
	lwz r29, 0x18(sp)
	lwz r28, 0x1C(sp)
	lwz r27, 0x20(sp)
	lwz r26, 0x24(sp)
	lwz r25, 0x28(sp)
	lwz r24, 0x2C(sp)
	lwz r6, 0x30(sp)
	lwz r5, 0x34(sp)
	lwz r4, 0x38(sp)
	lwz r0, 0x3C(sp)
	mtlr	r0
	lwz r0, 0x44(sp)
	addi	sp, sp, 0x40
	mr		r3, r31
	b		ContinueEatNoneExecute


