.text
#include <asm_setup.S>
.extern sprintf
.align 4


##########################
# Name getting functions #
##########################

.global getNameForBRRES
getNameForBRRES:
	# r5 has to be the OG filename (without g3d/ nor .brres)
	# r6 has to be the model ID
	# r5 will be returned and will contain the pointer to whatever text is outputed

	stwu	sp, -0x30(sp)
	mflr	r0
	stw		r0, 0x24(sp)
	stw		r3, 0x1C(sp)
	stw		r4, 0x18(sp)
	#stw		r5, 0x14(sp)
	#stw		r6, 0x10(sp)
	stw		r7, 0x0C(sp)
	stw		r8, 0x08(sp)
	stw		r9, 0x04(sp)

	lis		r3, TexFilenameBuffer@h
	ori		r3, r3, TexFilenameBuffer@l

	cmpwi	r6, 0
	beq		spriteTexBRRESOrig

	lis		r4, TexFormatStringBRRES@h
	ori		r4, r4, TexFormatStringBRRES@l
	b		doBRRESsprintf

spriteTexBRRESOrig:
	lis		r4, TexFormatStringBRRES_og@h
	ori		r4, r4, TexFormatStringBRRES_og@l

doBRRESsprintf:
	crclr	4*cr1+eq
	bl		sprintf

	lis		r5, TexFilenameBuffer@h
	ori		r5, r5, TexFilenameBuffer@l

spriteTexBRRESReturn:
	lwz		r0, 0x24(sp)
	lwz		r3, 0x1C(sp)
	lwz		r4, 0x18(sp)
	#lwz		r5, 0x14(sp)
	#lwz		r6, 0x10(sp)
	lwz		r7, 0x0C(sp)
	lwz		r8, 0x08(sp)
	lwz		r9, 0x04(sp)
	mtlr	r0
	addi	sp, sp, 0x30
	blr

.global getNameForMDL0
getNameForMDL0:
	# r4 has to be the OG filename
	# r5 has to be the model ID
	# r4 will be returned and will contain the pointer to whatever text is outputed

	stwu	sp, -0x30(sp)
	mflr	r0
	stw		r0, 0x24(sp)
	stw		r3, 0x1C(sp)
	#stw		r4, 0x18(sp)
	#stw		r5, 0x14(sp)
	stw		r6, 0x10(sp)
	stw		r7, 0x0C(sp)
	stw		r8, 0x08(sp)
	stw		r9, 0x04(sp)

	cmpwi	r5, 0
	beq		spriteTexMDL0Return

	mr		r6, r5
	mr		r5, r4

	lis		r3, TexFilenameBuffer@h
	ori		r3, r3, TexFilenameBuffer@l

	lis		r4, TexFormatStringMDL0@h
	ori		r4, r4, TexFormatStringMDL0@l

	crclr	4*cr1+eq
	bl		sprintf

	lis		r4, TexFilenameBuffer@h
	ori		r4, r4, TexFilenameBuffer@l

spriteTexMDL0Return:
	lwz		r0, 0x24(sp)
	lwz		r3, 0x1C(sp)
	#lwz		r4, 0x18(sp)
	#lwz		r5, 0x14(sp)
	lwz		r6, 0x10(sp)
	lwz		r7, 0x0C(sp)
	lwz		r8, 0x08(sp)
	lwz		r9, 0x04(sp)
	mtlr	r0
	addi	sp, sp, 0x30
	blr


#################################
# Macros for easy sprite adding #
#################################

doSpriteTexBRRES: .macro name, this, sh, mb, me
	mflr	r7

	lwz		r5, 4(this)
	rlwinm	r6, r5, sh, mb, me

	lis		r5, name@h
	ori		r5, r5, name@l

	bl		getNameForBRRES

	mtlr	r7
	blr
.endm

doSpriteTexMDL0: .macro name, this, sh, mb, me
	mflr	r6

	lwz		r4, 4(this)
	rlwinm	r5, r4, sh, mb, me

	lis		r4, name@h
	ori		r4, r4, name@l

	bl		getNameForMDL0

	mtlr	r6
	blr
.endm


###########
# Sprites #
###########

.global SpriteTex_GHDoor # Sprite 276 / Actor 230
SpriteTex_GHDoor:
	doSpriteTexMDL0 obj_obake_door, r29, 8, 28, 31 # Nybble 6

.global SpriteTex_UnderwaterLamp # Sprite 447 / Actor 618
SpriteTex_UnderwaterLamp:
	doSpriteTexMDL0 shinkai_light, r28, 28, 28, 31 # Nybble 11
	
.global SpriteTex_FrontBlack # Sprite 482 / Actor 276
SpriteTex_FrontBlack:
	doSpriteTexMDL0 FrontBlack, r30, 8, 28, 31 # Nybble 6

.global SpriteTex_Pipes # Sprites 450, 377, 378, 379, 380 / Actors 581, 580, 582, 583, 584
SpriteTex_Pipes:
	doSpriteTexBRRES obj_dokan, r28, 8, 28, 31 # Nybble 6

.global SpriteTex_Rolling1HillPipe # Sprite 355 / Actor 610
SpriteTex_Rolling1HillPipe:
	doSpriteTexBRRES obj_dokan, r30, 4, 28, 31 # Nybble 5

.global SpriteTex_Rolling8HillPipe # Sprite 360 / Actor 611
SpriteTex_Rolling8HillPipe:
	doSpriteTexBRRES obj_dokan, r28, 4, 28, 31 # Nybble 5

.global SpriteTex_4WayCannonPipe # Sprite 301 / Actor 548
SpriteTex_4WayCannonPipe:
	doSpriteTexBRRES obj_dokan, r30, 4, 28, 31 # Nybble 5

.global SpriteTex_ArrowSignboard # Sprite 301 / Actor 548
SpriteTex_ArrowSignboard:
	doSpriteTexMDL0 kanban_yajirushi, r29, 8, 28, 31 # Nybble 6



.extern getBubbleColor__Fv
.global SpriteTex_Bubble
SpriteTex_Bubble:
	stwu sp, -0x40(sp)
	stw r0, 0x44(sp)
	mflr r0
	stw r0, 0x3C(sp)
	stw r3, 0x38(sp)
	stw r6, 0x2C(sp)
	stw r7, 0x28(sp)
	stw r8, 0x24(sp)
	stw r9, 0x20(sp)
	stw r10, 0x1C(sp)
	stw r11, 0x18(sp)
	stw r12, 0x14(sp)
	stw r31, 0x10(sp)

	bl	getBubbleColor__Fv
	mr	r5, r3
	lis		r4, balloon@h
	ori		r4, r4, balloon@l	
	bl getNameForMDL0


	lwz r31, 0x10(sp)
	lwz r12, 0x14(sp)
	lwz r11, 0x18(sp)
	lwz r10, 0x1C(sp)
	lwz r9, 0x20(sp)
	lwz r8, 0x24(sp)
	lwz r7, 0x28(sp)
	lwz r6, 0x2C(sp)
	lwz r3, 0x38(sp)
	lwz r0, 0x3C(sp)	
	mtlr	r0
	lwz r0, 0x44(sp)
	addi	sp, sp, 0x40
	blr	


###################
# Strings & stuff #
###################

.data
.align 4

TexFilenameBuffer:
	.long 0,0,0,0,0,0,0,0,0,0

TexFormatStringBRRES:
	.string "g3d/%s_%d.brres"
TexFormatStringBRRES_og:
	.string "g3d/%s.brres"
TexFormatStringMDL0:
	.string "%s_%d"
.align 4

# Original names

obj_obake_door: .string "obj_obake_door"
shinkai_light: .string "shinkai_light"
FrontBlack: .string "FrontBlack"
obj_dokan: .string "obj_dokan"
kanban_yajirushi: .string "kanban_yajirushi"
balloon: .string "balloon"

.align 4