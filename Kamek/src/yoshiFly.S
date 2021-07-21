.text
.align 4

#include <asm_setup.S>
.extern ContinueFromFlyingYoshi
.extern getHeldTwo__14dPlayerInput_c

.global flyNowMyCuteYoshi
flyNowMyCuteYoshi:
	####################################################################
	#This was made using a Gecko code made by Bully@Wiiplaza and mdmwii#
	####################################################################

	# getting whether 2 is pressed
	addi	r3, r29, 0xEA4
	bl		getHeldTwo__14dPlayerInput_c

	# check that 2 is pressed
	cmpwi	r3, 0
	beq		ignoreFly

	# check that Yoshi's colour is blue
	lwz		r11, 4(r29)
	clrlwi	r3, r11, 28
	cmpwi	r3, 4			#blue
	bne		notBlue
	b		canFly

notBlue:
	cmpwi	r3, 7			#black
	bne		ignoreFly
	
canFly:
	# check that Yoshi has a shell in his mouth
	srwi.	r11, r11, 28
	beq		ignoreFly

	# set Yoshi's Y-speed
	lis		r11, YoshiUpSpeed@ha
	lfs		f1, YoshiUpSpeed@l(r11)
	stfs	f1, 0xEC(r29)

	# return
	b		ContinueFromFlyingYoshi

ignoreFly:
	lfs		f1, 0xEC(r29)
	b		ContinueFromFlyingYoshi

########################################################
########################################################
########################################################

# .extern RealYoshiDtor
# .extern _ZN21dYoshiWingsRenderer_c5buildEv
# .extern _ZN21dYoshiWingsRenderer_cD1Ev
# .extern _ZN21dYoshiWingsRenderer_c5setupEP21dPlayerModelHandler_c
# .extern _ZN21dYoshiWingsRenderer_c5setupEP21dPlayerModelHandler_ci
# .extern _ZN21dYoshiWingsRenderer_c4drawEv

# .global YoshiCtorYW #80e40370
# YoshiCtorYW:
# 	stwu r1, -0x10(r1)
# 	mflr r0
# 	stw r0, 0x14(r1)
# 	stw r27, 0xC(r1)
# 	mr r27, r3

# 	bl _ZN21dYoshiWingsRenderer_c5buildEv
# 	stw r3, 0x17F0(r27)

# 	mr r3, r27
# 	lwz r27, 0xC(r1)
# 	lwz r0, 0x14(r1)
# 	mtlr r0
# 	addi r1, r1, 0x10
# 	blr

# .global YoshiDtorYW #80e403a4
# YoshiDtorYW:
# 	stwu r1, -0x10(r1)
# 	mflr r0
# 	stw r0, 0x14(r1)
# 	stw r31, 0xC(r1)
# 	stw r30, 8(r1)

# 	mr r30, r4
# 	mr r29, r3

# 	cmpwi r3, 0
# 	beq oops3
# 	lwz r3, 0x17F0(r29)
# 	li r4, 1
# 	bl _ZN21dYoshiWingsRenderer_cD1Ev
# oops3:
	
# 	mr r3, r30
# 	mr r4, r31
# 	bl RealYoshiDtor

# 	lwz r30, 8(r1)
# 	lwz r31, 0xC(r1)
# 	lwz r0, 0x14(r1)
# 	mtlr r0
# 	addi r1, r1, 0x10
# 	blr

# .global YoshiSetMeUpYW #80e403f8
# YoshiSetMeUpYW:
# 	# partway in: r30 = this
# 	lwz r3, 0x17F0(r30)
# 	addi r4, r30, 0x14D4
# 	bl _ZN21dYoshiWingsRenderer_c5setupEP21dPlayerModelHandler_c

# 	# epilogue
# 	li r3, 1
# 	lwz r31, 0xC(r1)
# 	lwz r0, 0x14(r1)
# 	mtlr r0
# 	addi r1, r1, 0x10
# 	blr

# .global YoshiDrawYW #80e4041c
# YoshiDrawYW:
# 	# partway in: r31 = this
# 	lwz r3, 0x17F0(r31)
# 	bl _ZN21dYoshiWingsRenderer_c4drawEv

# 	# epilogue
# 	li r3, 1
# 	lwz r0, 0x14(r1)
# 	lwz r31, 0xC(r1)
# 	mtlr r0
# 	addi r1, r1, 0x10
# 	blr

########################################################
########################################################
########################################################

.data
YoshiUpSpeed:
	.float  3.0