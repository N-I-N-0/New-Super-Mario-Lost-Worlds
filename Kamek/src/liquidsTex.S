#include <asm_setup.S>


.text
.extern continueBGpolyCreate
.global CheckRanOnce
CheckRanOnce:
	#save
    stwu sp, -0x80 (sp)
    mflr r0
    stw r0, 0x4(sp)
    stmw r2, 0x8 (sp)
	
	addis r6, r3, 0x2 #adding 0x20000 so we can access the stuff at the end of this gigantic class by substracting from the value next
	lbz r4, -0x5AFD(r6)
	
	cmpwi r4, 0
	beq notRanOnceYet
	
	#save liquidSettings from nybble 3-4
	lbz r4, 0x355(r3)
	lis r5, liquidSetting@h
	ori r5, r5, liquidSetting@l
	stb r4, 0x0(r5)
	
	#restore
    lmw r2, 0x8 (sp)
    lwz r0, 0x4(sp)
    mtlr r0
    addi sp, sp, 0x80
	
	stwu r1, -0x60(r1)
	b continueBGpolyCreate

notRanOnceYet:
    li r4, 1			#now it ran once so set it to true
	stb r4, -0x5AFD(r6)
	
	lmw r2, 0x8 (sp)
    lwz r0, 0x4(sp)
    mtlr r0
    addi sp, sp, 0x80
	li r3, 0
	blr


.extern returnFromSelectDifferentLiquidsSet
.global SelectDifferentLiquidsSet
SelectDifferentLiquidsSet:
	lis r5, PTR_ARRAY_8031b7d0@h
	ori r5, r5, PTR_ARRAY_8031b7d0@l
	
	lbz r3, -0x7B(r29)			#load nybble 3-4 from daBGpolygon_c
	mulli r3, r3, 28
	add r3, r3, r28
	lwzx r5, r5, r3
	addi r3, r6, 0x4
	b returnFromSelectDifferentLiquidsSet


.extern returnFromSelectDifferentLiquidsSet2
.global SelectDifferentLiquidsSet2
SelectDifferentLiquidsSet2:
	lis r5, PTR_ARRAY_8031b988@h
	ori r5, r5, PTR_ARRAY_8031b988@l
	
	lbz r3, -0x1BF(r24)			#load nybble 3-4 from daBGpolygon_c
	mulli r3, r3, 20
	add r3, r3, r26
	lwzx r5, r5, r3
	addi r3, r6, 0x4
	b returnFromSelectDifferentLiquidsSet2
	

.extern obj_waterfull, obj_waterhalf, obj_magma, obj_poisonwater, obj_quicksand, obj_envfog
.data
.align 4
.global PTR_ARRAY_8031b7d0
PTR_ARRAY_8031b7d0:
	.long obj_waterfull
	.long obj_waterhalf
	.long obj_magma
	.long obj_poisonwater
	.long obj_quicksand
	.long obj_envfog
	.long obj_envfog

	.long black_waterfull
	.long black_waterhalf
	.long blue_magma
	.long shroob_slimewater
	.long snow_sand
	.long obj_envfog
	.long obj_envfog


.global PTR_ARRAY_8031b988
PTR_ARRAY_8031b988:
	.long obj_waterfull
	.long obj_waterhalf
	.long obj_magma
	.long obj_poisonwater
	.long obj_quicksand

	.long black_waterfull
	.long black_waterhalf
	.long blue_magma
	.long shroob_slimewater
	.long snow_sand

liquidSetting:
	.byte 0x00


black_waterfull:
	.string "g3d/black_waterfull.brres"

black_waterhalf:
	.string "g3d/black_waterhalf.brres"

blue_magma:
	.string "g3d/blue_magma.brres"

shroob_slimewater:
	.string "g3d/shroob_slimewater.brres"

snow_sand:
	.string "g3d/obj_snowsand.brres"



