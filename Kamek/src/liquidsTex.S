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

	li r3, 0					#set nybble 3-4 to 0 to make sure it won't cause unexpected problems later
	stb r3, -0x7B(r29)			#maybe set event mask again using nybble 1-2 as well?

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


.extern returnFromSelectDifferentLavaDeco
.global SelectDifferentLavaDeco
SelectDifferentLavaDeco:
	lis r5, PTR_ARRAY_8031bb30@h
	ori r5, r5, PTR_ARRAY_8031bb30@l
	
	lbz r3, -0x1BF(r24)			#load nybble 3-4 from daBGpolygon_c
	mulli r3, r3, 20
	add r3, r3, r26
	lwzx r5, r5, r3
	b returnFromSelectDifferentLavaDeco



.extern liquidSubTypes #array in .cpp
.extern dWaterManager_c__add
.extern returnFromSaveLiquiSubTypesInNewArray
.global SaveLiquiSubTypesInNewArray
SaveLiquiSubTypesInNewArray:
	bl dWaterManager_c__add			#index i is returned
	lis r4, liquidSubTypes@h		#array
	ori r4, r4, liquidSubTypes@l
	lbz r6, 0x355(r27)				#get subType
	stwx r6, r4, r3					#store it in liquidSubTypes[i]
	b returnFromSaveLiquiSubTypesInNewArray


.global SaveLiquidSubTypeWhenColliding
SaveLiquidSubTypeWhenColliding:
	lis r4, liquidSubTypes@h		#array
	ori r4, r4, liquidSubTypes@l
	lbzx r4, r5, r4					#r5 contains index

	lis r5, nextSplashSubType@h
	ori r5, r5, nextSplashSubType@l
	stb r4, 0x0(r5)					#store next splash sub type
	
	blr


.extern returnFromUseDifferentSplashEffects
.global UseDifferentSplashEffects
UseDifferentSplashEffects:
	lis r4, PTR_aWm_en_waterwav_803118c8@h			#new array
	ori r4, r4, PTR_aWm_en_waterwav_803118c8@l

	lis r5, nextSplashSubType@h						#load last used sub type
	ori r5, r5, nextSplashSubType@l
	lbz r5, 0x0(r5)
	mulli r5, r5, 32
	
	add r4, r4, r5									#select sub type entry address
	
	lwzx r4, r4, r0									#grab effect name
	b returnFromUseDifferentSplashEffects


.extern doneWithLavaParticlesColor
.global LavaParticlesColor
LavaParticlesColor:
    lwz r4, 4(r17)
    rlwinm r4, r4, 24, 28, 31 # (this->settings >> 8) & 0xF
    cmpwi r4, 1
    beq doGreenLavaParticles # We can add as many colors as we want
	cmpwi r4, 2
	beq doBlueLavaParticles
	cmpwi r4, 3
	beq doPinkLavaParticles
	cmpwi r4, 4
	beq doWhiteLavaParticles
	cmpwi r4, 5
	beq doBlackLavaParticles


    # doOrangeLavaParticles:
    li r4, 0xFF # R
    li r5, 0xFF # G
    li r6, 0x00 # B

    li r7, 0xFF # R
    li r8, 0x00 # G
    li r9, 0x00 # B

    b setLavaParticlesColor

doGreenLavaParticles:
    li r4, 0xAF # R
    li r5, 0xD3 # G
    li r6, 0x5F # B

    li r7, 0x18 # R
    li r8, 0x35 # G
    li r9, 0x0D # B

	b setLavaParticlesColor

doBlueLavaParticles:
    li r4, 0x00 # R
    li r5, 0x00 # G
    li r6, 0xFF # B

    li r7, 0x00 # R
    li r8, 0x0B # G
    li r9, 0xFF # B

	b setLavaParticlesColor

doPinkLavaParticles:
    li r4, 0xFF # R
    li r5, 0x00 # G
    li r6, 0xFF # B

    li r7, 0xFF # R
    li r8, 0x14 # G
    li r9, 0x93 # B

	b setLavaParticlesColor

doWhiteLavaParticles:
    li r4, 0xFF # R
    li r5, 0xFF # G
    li r6, 0xFF # B

    li r7, 0xF5 # R
    li r8, 0xF5 # G
    li r9, 0xF5 # B

	b setLavaParticlesColor

doBlackLavaParticles:
    li r4, 0x00 # R
    li r5, 0x00 # G
    li r6, 0x00 # B

    li r7, 0x1F # R
    li r8, 0x1F # G
    li r9, 0x1F # B

	b setLavaParticlesColor

setLavaParticlesColor:
    stb r4, 0x574(r17)
    stb r5, 0x575(r17)
    stb r6, 0x576(r17)

    stb r7, 0x578(r17)
    stb r8, 0x579(r17)
    stb r9, 0x57A(r17)

    li r4, 0xFF
    stb r4, 0x57B(r17) # Restore instruction

    b doneWithLavaParticlesColor



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

	.long obj_waterfull		#dummy
	.long black_waterhalf
	.long blue_magma
	.long obj_slimewater
	.long snow_sand
	.long obj_envfog
	.long obj_envfog

	.long obj_waterfull		#dummy
	.long swamp_waterhalf
	.long gold_magma
	.long pink_poisonwater
	.long red_sand
	.long obj_envfog		#dummy
	.long obj_envfog		#dummy

	.long obj_waterfull		#dummy
	.long obj_waterhalf		#dummy
	.long obj_magma			#dummy
	.long obj_poisonwater	#dummy
	.long obj_mudsand
	.long obj_envfog		#dummy
	.long obj_envfog		#dummy

.extern obj_magmawave
.global PTR_ARRAY_8031b988
PTR_ARRAY_8031b988:
	.long obj_waterhalf
	.long obj_magma
	.long obj_poisonwater
	.long obj_magmawave
	.long obj_quicksand

	.long black_waterhalf
	.long blue_magma
	.long obj_slimewater
	.long blue_magmawave
	.long snow_sand

	.long swamp_waterhalf
	.long gold_magma
	.long pink_poisonwater
	.long obj_magmawave			#dummy
	.long red_sand

	.long obj_waterhalf			#dummy
	.long obj_magma				#dummy
	.long obj_poisonwater		#dummy
	.long obj_magmawave			#dummy
	.long obj_mudsand

.extern aWm_en_waterwav, aWm_en_cmnwat_0, aWm_en_waterspl, aWm_en_firevani, aWm_en_cmnmag_1, aWm_en_magmaw_0, aWm_en_poisonin, aWm_en_poisonwa
.global PTR_aWm_en_waterwav_803118c8
PTR_aWm_en_waterwav_803118c8:
	.long aWm_en_waterwav
	.long aWm_en_cmnwat_0
	.long aWm_en_waterspl
	.long aWm_en_firevani
	.long aWm_en_cmnmag_1
	.long aWm_en_magmaw_0
	.long aWm_en_poisonin
	.long aWm_en_poisonwa
	
	.long aWm_en_poisonin
	.long aWm_en_poisonwa
	.long aWm_en_poisonin
	.long aWm_en_poisonwa
	.long aWm_en_poisonin
	.long aWm_en_poisonwa
	.long aWm_en_poisonin
	.long aWm_en_poisonwa


.extern obj_magmadeco
.global PTR_ARRAY_8031bb30
PTR_ARRAY_8031bb30:
	.long obj_magmadeco
	.long obj_magmadeco
	.long obj_poisondeco
	.long obj_magmadeco
	.long obj_magmadeco
	
	.long blue_magmadeco
	.long blue_magmadeco
	.long obj_slimedeco
	.long blue_magmadeco
	.long blue_magmadeco
	
	.long gold_magmadeco
	.long gold_magmadeco
	.long obj_poisondeco
	.long gold_magmadeco
	.long gold_magmadeco



liquidSetting:
	.byte 0x00

nextSplashSubType:
	.byte 0x00

black_waterhalf:
	.string "g3d/black_waterhalf.brres"

swamp_waterhalf:
	.string "g3d/swamp_waterhalf.brres"

blue_magma:
	.string "g3d/blue_magma.brres"

blue_magmadeco:
	.string "g3d/blue_magmadeco.brres"

gold_magma:
	.string "g3d/gold_magma.brres"

gold_magmadeco:
	.string "g3d/gold_magmadeco.brres"

blue_magmawave:
	.string "g3d/blue_magmawave.brres"

obj_slimewater:
	.string "g3d/obj_slimewater.brres"

obj_slimedeco:
	.string "g3d/obj_slimedeco.brres"

pink_poisonwater:
	.string "g3d/pink_poisonwater.brres"

obj_poisondeco:
	.string "g3d/obj_poisondeco.brres"

snow_sand:
	.string "g3d/obj_snowsand.brres"

red_sand:
	.string "g3d/obj_redsand.brres"

obj_mudsand:
	.string "g3d/obj_mudsand.brres"



