.text
.align 4

#include <asm_setup.S>

.extern returnFromGPSFASixth
.extern daEnItem_c__GetWhetherPlayerCanGetPowerupOrNot
.extern dAcPy_c__ChangePowerupWithAnimation
.extern PlayPlayerSound
.extern CreateActor
.extern PlayerProjectileShooting
.extern Actor_SearchByName
.extern SomeTable_802F5440
.extern SomeTable_802F5580
.extern SomeTable_802F56C0
.extern BlahTable
.extern continuePlumberSetPowerupTexture
.extern doneSettingThePowerupTexture
.extern doneSettingThePowerupTexture2
.extern returnFromPowerupSoundChange

.extern ActiveWiimoteID
.extern it_is_right
.extern not_right
.extern it_is_right2
.extern not_right2
.extern dStockItem__manipulateItemID
.extern tryAndMoveRight
.extern tryAndMoveLeft
.extern cantChange
.extern continueRightLoop
.extern continueLeftLoop

.global DisableHatForHammer
DisableHatForHammer:
	lwz r0, 0x17C(r31)
	clrlwi. r0, r0, 31
	beq DontDrawTheFuckingHat_
	lbz r0, 0x152(r31)	#r0 = this->powerup
	cmpwi r0, 7			#if it's 7 (hammer), don't draw the hat
	beq DontDrawTheFuckingHat_
	cmpwi r0, 9			#if it's 9 (spike), don't draw the hat
	beq DontDrawTheFuckingHat_
	cmpwi r0, 10		#if it's 10 (boomerang), don't draw the hat
	beq DontDrawTheFuckingHat_
	cmpwi r0, 12		#if it's 12 (cloud), don't draw the hat
	beq DontDrawTheFuckingHat_
	b DrawTheFuckingHat	#Else, draw it
DontDrawTheFuckingHat_:
	b DontDrawTheFuckingHat # this is a hack to avoid implementing R_PPC_REL14
.extern DontDrawTheFuckingHat
.extern DrawTheFuckingHat

.global GetInfoFromDumbTable
GetInfoFromDumbTable:
	lis r3, SomeTable@h
	ori r3, r3, SomeTable@l

	slwi r4, r4, 2
	slwi r0, r5, 6

	lwzx r3, r3, r4
	add r3, r3, r0
	blr


.global spikeAShroom
spikeAShroom:
	beq return_1
	cmplwi r0, 7
	bne return_0
return_1:
	li r3, 1
	blr
return_0:
	li r3, 0
	blr


.extern returnFromFrogPenguinMove1
.extern returnFromFrogPenguinMove2
.global frogPenguinMove1
frogPenguinMove1:
	cmpwi r0, 0xe
	bne tryFrogInstead1
	b returnFromFrogPenguinMove1
tryFrogInstead1:
	cmpwi r0, 0x9
	b returnFromFrogPenguinMove1



.global frogPenguinMove2
frogPenguinMove2:
	cmpwi r0, 0xe
	bne tryFrogInstead2
	b returnFromFrogPenguinMove2
tryFrogInstead2:
	cmpwi r0, 0x9
	b returnFromFrogPenguinMove2


.global FukidashiFix
FukidashiFix:
	lis r7, ZeroFloat@h
	ori r7, r7, ZeroFloat@l
	
	lis r8, FukidashiTable2@h
	ori r8, r8, FukidashiTable2@l
	
	mulli r6, r4, 0x20
	slwi r0, r5, 2
	add r0, r0, r6
	
	lfs f0, 0(r7)
	lfsx f1, r8, r0
	
	stfs f0, 0x1DC(r3)
	stfs f1, 0x1E0(r3)
	stfs f0, 0x1E4(r3)
	
	blr




.global PlayerGetHeight
PlayerGetHeight:
	lwz r0, 0x14D4(r3) # r0 = this->characterID
	
	lwz r4, 0x1090(r3) # r4 = this->powerup
	slwi r4, r4, 3
	
	cmpwi r0, 0
	beq useNormal
	cmpwi r0, 1
	bne useDucking
	
useNormal:
	lis r3, PlayerHeightsNormal@h
	ori r3, r3, PlayerHeightsNormal@l
	b gotOffset
	
useDucking:
	lis r3, PlayerHeightsDucking@h
	ori r3, r3, PlayerHeightsDucking@l
	
gotOffset:
	lfsx f1, r3, r4
	blr



.global WtfEver
WtfEver:
	lis r3, WtfEverTable@h
	ori r3, r3, WtfEverTable@l
	lfsx f0, r3, r0
	blr



.global YetAnother
YetAnother:
	lis r3, YetAnotherTable@h
	ori r3, r3, YetAnotherTable@l
	lfsx f0, r3, r0
	blr




.global GetWeirdScaleTable
GetWeirdScaleTable:
	lis r5, WeirdScaleTable@h
	ori r5, r5, WeirdScaleTable@l
	lfsx f1, r5, r0
	blr





.global GetAsdfTable
GetAsdfTable:
	extsb r0, r4
	cmpwi r0, -1
	bne dontGetThisOne
	
	lwz r0, 0x1090(r3)	#r0 = this->powerup
	extsb r4, r0
	
dontGetThisOne:
	extsb r0, r4
	
	lis r3, AsdfTable@h
	ori r3, r3, AsdfTable@l
	lbzx r3, r3, r0
	
	blr





.global GetBlahTableOffset
GetBlahTableOffset:
	lis r5, BlahTableOffsets@h
	ori r5, r5, BlahTableOffsets@l
	lwzx r30, r5, r4
	blr


.global DealWithBlahTable
DealWithBlahTable:
	lis r5, BlahTableOffsets@h
	ori r5, r5, BlahTableOffsets@l
	
	# points to BlahTable
	lis r4, BlahTable@h
	ori r4, r4, BlahTable@l
	
	# get blahtableoffset entry based on powerup
	lwz r0, 0x1090(r3)	#r0 = this->powerup
	slwi r0, r0, 2
	lwzx r0, r5, r0
	
	# now get the entry from the blahtable
	slwi r0, r0, 2
	lwzx r4, r4, r0
	
	# do shit here
	
	lfs f3, 0(r4)
	stfs f3, 0x14E4(r3)
	stfs f3, 0x164(r3)
	
	lfs f1, 0x2A70(r3)
	lfs f0, 0x2A74(r3)
	
	lfs f2, 4(r4)
	fadds f1, f2, f1
	fadds f0, f0, f1
	stfs f0, 0x14E8(r3)
	stfs f0, 0x168(r3)
	
	lfs f0, 0x8(r4)
	lfs f1, 0xC(r4)
	stfs f0, 0x16C(r3)
	stfs f1, 0x170(r3)
	
	# hope I did this right
	blr





.global GetPowerupScaleFloatAddr_r6_trash_r0_valshl2_r4_dest
GetPowerupScaleFloatAddr_r6_trash_r0_valshl2_r4_dest:
	lis r6, PowerupScaleFloats@h
	ori r6, r6, PowerupScaleFloats@l
	add r4, r6, r0
	blr

.global GetPowerupScaleFloatAddr_r6_trash_r0_valshl2_r3_dest
GetPowerupScaleFloatAddr_r6_trash_r0_valshl2_r3_dest:
	lis r6, PowerupScaleFloats@h
	ori r6, r6, PowerupScaleFloats@l
	add r3, r6, r0
	blr

.global GetPowerupScaleFloatAddr_r8_trash_r0_valshl2_r7_dest
GetPowerupScaleFloatAddr_r8_trash_r0_valshl2_r7_dest:
	lis r8, PowerupScaleFloats@h
	ori r8, r8, PowerupScaleFloats@l
	add r7, r8, r0
	blr

.global GetPowerupScaleFloatAddr_fixForSixth
GetPowerupScaleFloatAddr_fixForSixth:
	lis r5, PowerupScaleFloats@h
	ori r5, r5, PowerupScaleFloats@l
	add r4, r5, r4
	b returnFromGPSFASixth



.global PlumberSetPowerupFix
PlumberSetPowerupFix:
	lbz r0, 0x152(r31) # get powerup id
	
	cmpwi r0, 2
	beq doFireTex
	
	cmpwi r0, 6
	beq doIceTex
	
	cmpwi r0, 7
	beq doHammerTex
	
	cmpwi r0, 8
	beq doGoldTex
	
	cmpwi r0, 9
	beq doSpikeTex
	
	cmpwi r0, 10
	beq doBoomerangTex
	
	cmpwi r0, 11
	beq doFrogTex
	
	cmpwi r0, 12
	beq doCloudTex
	
	cmpwi r0, 13
	beq doWandTex
	
	li r4, 0
	b gotTexID
	
doFireTex:
	li r4, 1	#These are Pat0 frame ID
	b gotTexID
	
doIceTex:
	li r4, 2	#These are Pat0 frame ID
	b gotTexID
	
doHammerTex:
	li r4, 3	#These are Pat0 frame ID
	b gotTexID
	
doBoomerangTex:
	li r4, 4	#These are Pat0 frame ID
	b gotTexID
	
doSpikeTex:
	li r4, 5	#These are Pat0 frame ID
	b gotTexID
	
doWandTex:
	li r4, 6	#These are Pat0 frame ID
	b gotTexID
	
doFrogTex:
	li r4, 7	#These are Pat0 frame ID
	b gotTexID
	
doGoldTex:
	li r4, 8	#These are Pat0 frame ID
	b gotTexID
	
doCloudTex:
	li r4, 9	#These are Pat0 frame ID
	
gotTexID:
	lwz r12, 0(r31)
	mr r3, r31
	lwz r12, 0x34(r12)
	mtctr r12
	bctrl
	
	b doneSettingThePowerupTexture



.global PlumberSetPowerupFix2 #same as the first one, like, literally
PlumberSetPowerupFix2:
	cmpwi r31, 2
	beq doFireTex2
	
	cmpwi r31, 6
	beq doIceTex2
	
	cmpwi r31, 7
	beq doHammerTex2

	cmpwi r31, 8
	beq doGoldTex2
	
	cmpwi r31, 9
	beq doSpikeTex2
	
	cmpwi r31, 10
	beq doBoomerangTex2
	
	cmpwi r31, 11
	beq doFrogTex2
	
	cmpwi r31, 12
	beq doCloudTex2
	
	cmpwi r31, 13
	beq doWandTex2
	
	li r4, 0
	b gotTexID2
	
doFireTex2:
	li r4, 1	#These are Pat0 frame ID
	b gotTexID2
	
doIceTex2:
	li r4, 2	#These are Pat0 frame ID
	b gotTexID2
	
doHammerTex2:
	li r4, 3	#These are Pat0 frame ID
	b gotTexID2
	
doBoomerangTex2:
	li r4, 4	#These are Pat0 frame ID
	b gotTexID2
	
doSpikeTex2:
	li r4, 5	#These are Pat0 frame ID
	b gotTexID2
	
doWandTex2:
	li r4, 6	#These are Pat0 frame ID
	b gotTexID2
	
doFrogTex2:
	li r4, 7	#These are Pat0 frame ID
	b gotTexID2
	
doGoldTex2:
	li r4, 8	#These are Pat0 frame ID
	b gotTexID2
	
doCloudTex2:
	li r4, 9	#These are Pat0 frame ID
	
gotTexID2:
	lwz r3, 0x2A64(r30)
	lwz r12, 0(r3)
	lwz r12, 0x34(r12)
	mtctr r12
	bctrl
	
	b doneSettingThePowerupTexture2




.global PlumberSetPowerupTextureFix
PlumberSetPowerupTextureFix:
	stb r4, 0x153(r3)
	
	lis r5, PowerupTextures@h
	ori r5, r5, PowerupTextures@l
	
	slwi r4, r4, 2
	lfsx f31, r5, r4
	
	b continuePlumberSetPowerupTexture



.global SetHammerToEnItemDCA
SetHammerToEnItemDCA:
	bl daEnItem_c__GetWhetherPlayerCanGetPowerupOrNot
	cmpwi r3, 1
	bne DontSetHammer
	
	li r0, 5
	sth r0, 0xDCA(r31) #So basically DCA is the value in EN_ITEM that stores the internal powerup type
	
DontSetHammer:
	lwz r0, 0x14(sp)
	lwz r31, 0xC(sp)
	mtlr r0
	addi sp, sp, 0x10
	blr



.global SetBoomerangToEnItemDCA
SetBoomerangToEnItemDCA:
	bl daEnItem_c__GetWhetherPlayerCanGetPowerupOrNot
	cmpwi r3, 1
	bne DontSetBoomerang
	
	li r0, 0xF
	sth r0, 0xDCA(r31)
	
DontSetBoomerang:
	lwz r0, 0x14(sp)
	lwz r31, 0xC(sp)
	mtlr r0
	addi sp, sp, 0x10
	blr



.global SetSpikeToEnItemDCA
SetSpikeToEnItemDCA:
	bl daEnItem_c__GetWhetherPlayerCanGetPowerupOrNot
	cmpwi r3, 1
	bne DontSetSpike
	
	li r0, 7
	sth r0, 0xDCA(r31)
	
DontSetSpike:
	lwz r0, 0x14(sp)
	lwz r31, 0xC(sp)
	mtlr r0
	addi sp, sp, 0x10
	blr



.global SetWandToEnItemDCA
SetWandToEnItemDCA:
	bl daEnItem_c__GetWhetherPlayerCanGetPowerupOrNot
	cmpwi r3, 1
	bne DontSetWand
	
	li r0, 8
	sth r0, 0xDCA(r31)
	
DontSetWand:
	lwz r0, 0x14(sp)
	lwz r31, 0xC(sp)
	mtlr r0
	addi sp, sp, 0x10
	blr



.global SetFrogToEnItemDCA
SetFrogToEnItemDCA:
	bl daEnItem_c__GetWhetherPlayerCanGetPowerupOrNot
	cmpwi r3, 1
	bne DontSetFrog
	
	li r0, 9
	sth r0, 0xDCA(r31)
	
DontSetFrog:
	lwz r0, 0x14(sp)
	lwz r31, 0xC(sp)
	mtlr r0
	addi sp, sp, 0x10
	blr



.global SetGoldToEnItemDCA
SetGoldToEnItemDCA:
	bl daEnItem_c__GetWhetherPlayerCanGetPowerupOrNot
	cmpwi r3, 1
	bne DontSetGold
	
	li r0, 0xA
	sth r0, 0xDCA(r31)
	
DontSetGold:
	lwz r0, 0x14(sp)
	lwz r31, 0xC(sp)
	mtlr r0
	addi sp, sp, 0x10
	blr



.global SetCloudToEnItemDCA
SetCloudToEnItemDCA:
	bl daEnItem_c__GetWhetherPlayerCanGetPowerupOrNot
	cmpwi r3, 1
	bne DontSetCloud
	
	li r0, 0xC
	sth r0, 0xDCA(r31)
	
DontSetCloud:
	lwz r0, 0x14(sp)
	lwz r31, 0xC(sp)
	mtlr r0
	addi sp, sp, 0x10
	blr



.global ForceCloudToEnItemDCA
ForceCloudToEnItemDCA:	
	li r0, 0xC
	sth r0, 0xDCA(r31)
	
	lwz r0, 0x14(sp)
	lwz r31, 0xC(sp)
	mtlr r0
	addi sp, sp, 0x10
	blr

	

.global SetHammerToEnItemDCASpecial
SetHammerToEnItemDCASpecial: #sooo this is a big mess so lemme explain it
	#just so you know: 0xD is the mini shroom's DCA internal value, 5 is the hammer's and 0xF is the boomerang's
	#also, 0x19 is the mini shroom's DCB internal value, 6 is the hammer's and 0xF is the boomerang's
	cmplwi r4, 0x19			#if the current DCB isn't a mini shroom
	bne _not19				#then go to _not19
	li r0, 0xD				#else, store 0xD in r0
	sth r0, 0xDCA(r3)		#then store r0 in the DCA
_not19:
	cmplwi r4, 6			#if the current DCB isn't 6 (Hammer) either
	bne notHammerEither		#then got to notHammerEither
	b doHammerBoomerCheck	#Otherwise, go to doHammerBoomerCheck
notHammerEither:
	cmplwi r4, 0xF			#if the current DCB isn't 0xF (Boomerang) either
	bne notBoomerEither		#then got to notBoomerEither
	b doHammerBoomerCheck	#Otherwise, go to doHammerBoomerCheck
notBoomerEither:
	cmplwi r4, 0x10			#if the current DCB isn't 0x10 (Spike) either
	bne notSpikeEither		#then got to notSpikeEither
	b doHammerBoomerCheck	#Otherwise, go to doHammerBoomerCheck
notSpikeEither:
	cmplwi r4, 0x12			#if the current DCB isn't 0x12 (Wand) either
	bne notWandEither		#then got to notWandEither
	b doHammerBoomerCheck	#Otherwise, go to doHammerBoomerCheck
notWandEither:
	cmplwi r4, 0x13			#if the current DCB isn't 0x13 (Frog) either
	bne notFrogEither		#then got to notFrogEither
	b doHammerBoomerCheck	#Otherwise, go to doHammerBoomerCheck
notFrogEither:
	cmplwi r4, 0x14			#if the current DCB isn't 0x14 (Gold) either
	bne notGoldEither	#then got to notGoldEither
	b doHammerBoomerCheck	#Otherwise, go to doHammerBoomerCheck
notGoldEither:
	cmplwi r4, 0x16			#if the current DCB isn't 0x16 (Cloud) either
	bnelr					#then return
	b doHammerBoomerCheck	#Otherwise, go to doHammerBoomerCheck
doHammerBoomerCheck:
	cmplwi r4, 6			#If the current DCB is 6 (Hammer)
	beq setDCAHammer		#Then go to setDCAHammer
	cmplwi r4, 0xF			#If the current DCB is 0xF (Boomerang)
	beq setDCABoomerang		#Then go to setDCABoomerang
	cmplwi r4, 0x10			#If the current DCB is 0x10 (Spike)
	beq setDCASpike			#Then go to setDCASpike
	cmplwi r4, 0x12			#If the current DCB is 0x12 (Wand)
	beq setDCAWand			#Then go to setDCAWand
	cmplwi r4, 0x13			#If the current DCB is 0x13 (Frog)
	beq setDCAFrog			#Then go to setDCAFrog
	cmplwi r4, 0x14			#If the current DCB is 0x14 (Gold)
	beq setDCAGold			#Then go to setDCAGold
	cmplwi r4, 0x16			#If the current DCB is 0x16 (Cloud)
	beq setDCACloud			#Then go to setDCACloud
setDCAHammer:
	li r0, 5				#Store 5 into r0
	sth r0, 0xDCA(r3)		#Store r0 in the DCA
	blr
setDCABoomerang:
	li r0, 0xF				#Store 0xF into r0
	sth r0, 0xDCA(r3)		#Store r0 in the DCA
	blr
setDCASpike:
	li r0, 7				#Store 7 into r0
	sth r0, 0xDCA(r3)		#Store r0 in the DCA
	blr
setDCAWand:
	li r0, 8				#Store 8 into r0
	sth r0, 0xDCA(r3)		#Store r0 in the DCA
	blr
setDCAFrog:
	li r0, 9				#Store 9 into r0
	sth r0, 0xDCA(r3)		#Store r0 in the DCA
	blr
setDCAGold:
	li r0, 0xA				#Store 0xA into r0
	sth r0, 0xDCA(r3)		#Store r0 in the DCA
	blr
setDCACloud:
	li r0, 0xC				#Store 0xC into r0
	sth r0, 0xDCA(r3)		#Store r0 in the DCA
	blr


#.global EnItem_BindAnimation_Fix
#EnItem_BindAnimation_Fix:
#	lhz r0, 0xDCA(r3)
#	cmpwi r0, 5
#	beqlr
#	stwu sp, -0x20(sp)
#	b EnItem_BindAnimation_Continued


.global FixPowerupSound
FixPowerupSound:
	#lis r3, DBString@h
	#ori r3, r3, DBString@l
	#lwz r4, 0x1090(r29)
	#crclr 4*cr1+eq
	#bl OSReport

	lwz r5, 0x14DC(r29) # Original powerup
	lwz r0, 0x1090(r29) # New powerup
	# If NEW is 1 (big) and OLD is not 0 or 3, then play 273 (powerdown)
	cmpwi r0, 1
	bne dontConsiderPowerdown

	cmpwi r5, 0
	beq dontConsiderPowerdown
	cmpwi r5, 3
	beq dontConsiderPowerdown
	# If the player is here, he was downgraded from a higher powerup to Big Mario
	# So play the powerdown sound
	mr r3, r29
	li r4, 273
	li r5, 0
	b returnFromPowerupSoundChange

dontConsiderPowerdown:
	lis r3, PowerupSounds@h
	ori r3, r3, PowerupSounds@l
	slwi r4, r0, 2
	lwzx r4, r3, r4
	mr r3, r29
	li r5, 0

	b returnFromPowerupSoundChange


.extern ContinueFromAlternateQBlock
.extern QBlockTable
.global QBlockVariation
QBlockVariation:
	lwz r0, 4(r27)
	extrwi r0, r0, 4,24 #24 -> nybble 11 | 20 -> nybble 10 | 16 -> nybble 9 | etc...
	cmpwi r0, 0
	beq doFirstTable
	cmpwi r0, 1
	beq doSecondTable
	b ContinueFromAlternateQBlock
doFirstTable:
	lis r3, QBlockTable@h
	ori r3, r3, QBlockTable@l
	b ContinueFromAlternateQBlock
doSecondTable:
	lis r3, AlternateQBlockTable@h
	ori r3, r3, AlternateQBlockTable@l
	b ContinueFromAlternateQBlock


.global WeirdAnimLoadHack #This is to prevent a crash when spawning our brand new EN_ITEM, for some reason
WeirdAnimLoadHack:
	cmplwi r4, 5   #hammersuit
	beqlr
	cmplwi r4, 6   #1up
	beqlr
	cmplwi r4, 0xF #boomerang
	beqlr
	cmplwi r4, 7   #spike
	beqlr
	cmplwi r4, 8   #wand
	beqlr
	cmplwi r4, 9   #frog
	beqlr
	cmplwi r4, 0xA #gold
	beqlr
	cmplwi r4, 0xC #cloud
	beqlr
	# neither of those succeeded
	crclr 4*cr0+eq
	blr


.extern increaseIceCount__11placeholderFv
.extern ret0ice
.extern retice
.global IncreaseIceball
IncreaseIceball:
	cmpwi r3, 0
	beq _ret0ice
	b _retice
_ret0ice:
	b ret0ice
_retice:
	bl increaseIceCount__11placeholderFv
	li r3, 1
	b retice

.extern retice2
.global IncreaseIceballSpin
IncreaseIceballSpin:
	bl PlayerProjectileShooting
	bl increaseIceCount__11placeholderFv
	b retice2


.extern increasePropellerCount__11placeholderFv
.extern ContinueFromPropellerBegin
.global IncreasePropeller
IncreasePropeller:
	bl increasePropellerCount__11placeholderFv
	lha r0, 0x17CC(r31)
	b ContinueFromPropellerBegin


.extern doCleanClouds__11placeholderFv
.global TryToGiveMarioHammerSuit
TryToGiveMarioHammerSuit:
	stwu sp, -0x10(sp)
	mflr r0
	stw r0, 0x14(sp)
	
	# check the obtained item number
	b checkPow	#Sooo let's check the current DCA powerup !

checkPow:
	cmplwi r3, 5				#If it's 5
	beq doCheckForHammer		#Then do the check for the hammer suit
	cmplwi r3, 0xF				#If it's 0xF
	beq doCheckForBoomerang		#Then do the check for the boomerang suit
	cmplwi r3, 7				#If it's 7
	beq doCheckForSpike			#Then do the check for the spike suit
	cmplwi r3, 8				#If it's 8
	beq doCheckForWand			#Then do the check for rosalina's wand
	cmplwi r3, 9				#If it's 9
	beq doCheckForFrog			#Then do the check for the frog suit
	cmplwi r3, 0xA				#If it's 0xA
	beq doCheckForGold			#Then do the check for the gold flower
	cmplwi r3, 0xC				#If it's 0xC
	beq doCheckForCloud			#Then do the check for the cloud suit
	b IsntGettingAnySuit		#Otherwise, No suit getting so go to IsntGettingAnySuit

doCheckForHammer:
	# check the current player powerup
	cmplwi r4, 7				#If the current powerup isn't 7
	bne DontHaveHammerSuit		#then we don't have the hammer yet, so go to DontHaveHammerSuit
	
	li r28, 0					#Otherwise, set some shit and ReturnFromTTGMHS
	li r29, 2
	b ReturnFromTTGMHS

doCheckForGold:
	# check the current player powerup
	cmplwi r4, 8				#If the current powerup isn't 8
	bne DontHaveGold			#then we don't have the thunder yet, so go to DontHaveGold
	
	li r28, 0					#Otherwise, set some shit and ReturnFromTTGMHS
	li r29, 2
	b ReturnFromTTGMHS

doCheckForSpike:
	# check the current player powerup
	cmplwi r4, 9				#If the current powerup isn't 9
	bne DontHaveSpikeSuit		#then we don't have the spike yet, so go to DontHaveSpikeSuit
	
	li r28, 0					#Otherwise, set some shit and ReturnFromTTGMHS
	li r29, 2
	b ReturnFromTTGMHS

doCheckForBoomerang:
	# check the current player powerup
	cmplwi r4, 10				#If the current powerup isn't 10
	bne DontHaveBoomerangSuit	#then we don't have the boomerang yet, so go to DontHaveBoomerangSuit
	
	li r28, 0					#Otherwise, set some shit and ReturnFromTTGMHS
	li r29, 2
	b ReturnFromTTGMHS

doCheckForFrog:
	# check the current player powerup
	cmplwi r4, 11				#If the current powerup isn't 11
	bne DontHaveFrogSuit		#then we don't have the frog yet, so go to DontHaveFrogSuit
	
	li r28, 0					#Otherwise, set some shit and ReturnFromTTGMHS
	li r29, 2
	b ReturnFromTTGMHS

doCheckForCloud:
	# check the current player powerup
	cmplwi r4, 12				#If the current powerup isn't 12
	bne DontHaveCloudSuit		#then we don't have the cloud yet, so go to DontHaveCloudSuit
	
	bl doCleanClouds__11placeholderFv

	li r28, 0					#Otherwise, set some shit and ReturnFromTTGMHS
	li r29, 2
	b ReturnFromTTGMHS

doCheckForWand:
	# check the current player powerup
	cmplwi r4, 13				#If the current powerup isn't 13
	bne DontHaveWand			#then we don't have the bobomb yet, so go to DontHaveWand
	
	li r28, 0					#Otherwise, set some shit and ReturnFromTTGMHS
	li r29, 2
	b ReturnFromTTGMHS
	
DontHaveHammerSuit:
	mr r3, r30								#move this into r3
	li r4, 7								#set the first param to 7 (well the first one is 'this' actually but shhhht)
	bl dAcPy_c__ChangePowerupWithAnimation	#change the current powerup to 7 (with a cute animation owo)
	
	cmpwi r3, 0
	beq ReturnFromTTGMHS
	
	li r29, 1
	b ReturnFromTTGMHS
	
DontHaveGold:
	mr r3, r30								#move this into r3
	li r4, 8								#set the first param to 8 (well the first one is 'this' actually but shhhht)
	bl dAcPy_c__ChangePowerupWithAnimation	#change the current powerup to 8 (with a cute animation owo)
	
	cmpwi r3, 0
	beq ReturnFromTTGMHS
	
	li r29, 1
	b ReturnFromTTGMHS
	
DontHaveSpikeSuit:
	mr r3, r30								#move this into r3
	li r4, 9								#set the first param to 9 (well the first one is 'this' actually but shhhht)
	bl dAcPy_c__ChangePowerupWithAnimation	#change the current powerup to 9 (with a cute animation owo)
	
	cmpwi r3, 0
	beq ReturnFromTTGMHS
	
	li r29, 1
	b ReturnFromTTGMHS
	
DontHaveBoomerangSuit:
	mr r3, r30								#move this into r3
	li r4, 10								#set the first param to 10 (well the first one is 'this' actually but shhhht)
	bl dAcPy_c__ChangePowerupWithAnimation	#change the current powerup to 10 (with a cute animation owo)
	
	cmpwi r3, 0
	beq ReturnFromTTGMHS
	
	li r29, 1
	b ReturnFromTTGMHS
	
DontHaveFrogSuit:
	mr r3, r30								#move this into r3
	li r4, 11								#set the first param to 11 (well the first one is 'this' actually but shhhht)
	bl dAcPy_c__ChangePowerupWithAnimation	#change the current powerup to 11 (with a cute animation owo)
	
	cmpwi r3, 0
	beq ReturnFromTTGMHS
	
	li r29, 1
	b ReturnFromTTGMHS
	
DontHaveCloudSuit:
	bl doCleanClouds__11placeholderFv
	mr r3, r30								#move this into r3
	li r4, 12								#set the first param to 12 (well the first one is 'this' actually but shhhht)
	bl dAcPy_c__ChangePowerupWithAnimation	#change the current powerup to 12 (with a cute animation owo)

	cmpwi r3, 0
	beq ReturnFromTTGMHS
	
	li r29, 1
	b ReturnFromTTGMHS
	
DontHaveWand:
	mr r3, r30								#move this into r3
	li r4, 13								#set the first param to 13 (well the first one is 'this' actually but shhhht)
	bl dAcPy_c__ChangePowerupWithAnimation	#change the current powerup to 13 (with a cute animation owo)
	
	cmpwi r3, 0
	beq ReturnFromTTGMHS
	
	li r29, 1
	
IsntGettingAnySuit:
ReturnFromTTGMHS:
	# end of function
	lwz r0, 0x14(sp)
	mtlr r0
	addi sp, sp, 0x10
	
	# leftover instruction from what we replaced to add the bl
	cmpwi r29, 0
	blr



.global ThrowHammer #More like "ThrowProjectile" but renaming is way too hard for my poor hands
ThrowHammer:
	# continues on from PlayerProjectileShooting
	
	lwz r3, 0x1090(r30)			#r3 = this->powerup

	cmplwi r3, 7				#If the current powerup is 7, DoHammer
	beq DoHammer

	cmplwi r3, 8				#If the current powerup is 8, DoGold
	beq DoGold

	cmplwi r3, 9				#If the current powerup is 9, DoSpike
	beq DoSpike

	cmplwi r3, 10				#If the current powerup is 10, DoBoomerang
	beq DoBoomerang

	cmplwi r3, 11				#If the current powerup is 11, DoFrog
	beq DoFrog

	#No need of the cloud

	#cmplwi r3, 13				#If the current powerup is 13, DoWand
	#beq DoWand

	b ReturnFromThrowHammer		#Otherwise, return

DoHammer:	#Basically, spawning a hammer
	mr r3, r30
	li r4, 498
	li r5, 0
	bl PlayPlayerSound
	
	li r3, 0x73			# Hammer
	li r4, 0				# Settings
	addi r5, sp, 0x2C		# Position
	li r6, 0				# Rotation
	lbz r7, 0x38F(r30)	# Layer
	bl CreateActor
	
	# Store player ID
	lwz r4, 0(r30)
	stw r4, 0x450(r3)
	
	# store player number, but add 0x100 to differentiate it from 0
	lbz r4, 0x38D(r30)
	addi r4, r4, 0x100
	stw r4, 0x458(r3)
	
	# Fix scale
	lis r4, OneFloat@h
	ori r4, r4, OneFloat@l
	lfs f0, 0(r4)
	stfs f0, 0xDC(r3)
	stfs f0, 0xE0(r3)
	stfs f0, 0xE4(r3)
	
	mr r4, r30
	lwz r12, 0x60(r3)
	lwz r12, 0x8C(r12)
	mtctr r12
	bctrl
	b ReturnFromThrowHammer

#include <asm_profiles.S>
DoBoomerang:	#Basically, spawning a boomerang
	mr r3, r30
	li r4, 498
	li r5, 0
	bl PlayPlayerSound


	li r3, BoomerangHax			# Custom Boomerang

	lwz r4, 0x38D(r30)		# Settings => dStageActor_c->associatedPlayerID (in nybble 6 for some reasons) + 2 (in nybble 12 because yes)
	addi r4, r4, 2

	addi r5, sp, 0x2C		# Position
	li r6, 0				# Rotation
	lbz r7, 0x38F(r30)		# Layer

	bl CreateActor

	#mr r4, r30
	#lwz r12, 0x60(r3)
	#lwz r12, 0x8C(r12)
	#mtctr r12
	#bctrl
	b ReturnFromThrowHammer

DoSpike:	#Basically, spawning a spikeball
	mr r3, r30
	li r4, 498
	li r5, 0
	bl PlayPlayerSound

	
	li r3, BoomerangHax			# Custom Spikeball

	lwz r4, 0x38D(r30)		# Settings => dStageActor_c->associatedPlayerID (in nybble 6 for some reasons) + 2 (in nybble 12 because yes) + 1 (in nybble 11)
	addi r4, r4, 18

	addi r5, sp, 0x2C		# Position
	li r6, 0				# Rotation
	lbz r7, 0x38F(r30)		# Layer
	bl CreateActor
	
	#mr r4, r30
	#lwz r12, 0x60(r3)
	#lwz r12, 0x8C(r12)
	#mtctr r12
	#bctrl
	b ReturnFromThrowHammer

DoWand:	#Basically, i didn't make it yet	
	mr r3, r30
	li r4, 498
	li r5, 0
	bl PlayPlayerSound
	

	li r3, BoomerangHax			# Custom Shit

	lwz r4, 0x38D(r30)		# Settings => dStageActor_c->associatedPlayerID (in nybble 6 for some reasons) + 2 (in nybble 12 because yes) + 2 (in nybble 11)
	addi r4, r4, 34

	addi r5, sp, 0x2C		# Position
	li r6, 0				# Rotation
	lbz r7, 0x38F(r30)		# Layer
	bl CreateActor
	
	mr r4, r30
	lwz r12, 0x60(r3)
	lwz r12, 0x8C(r12)
	mtctr r12
	bctrl
	b ReturnFromThrowHammer

DoFrog:	#Basically, spawning a bubble
	mr r3, r30
	li r4, 498
	li r5, 0
	bl PlayPlayerSound
	

	li r3, BoomerangHax			# Custom Bubble

	lwz r4, 0x38D(r30)		# Settings => dStageActor_c->associatedPlayerID (in nybble 6 for some reasons) + 2 (in nybble 12 because yes) + 3 (in nybble 11)
	addi r4, r4, 50

	addi r5, sp, 0x2C		# Position
	li r6, 0				# Rotation
	lbz r7, 0x38F(r30)		# Layer
	bl CreateActor
	
	#mr r4, r30
	#lwz r12, 0x60(r3)
	#lwz r12, 0x8C(r12)
	#mtctr r12
	#bctrl
	b ReturnFromThrowHammer

DoGold:	#Basically, whatever
	mr r3, r30
	li r4, 2003
	li r5, 0
	bl PlayPlayerSound
	
	frsp f1, f31
	lfs f2, 0x20(r1) #0xC0+another_x
	frsp f0, f30
	stfs f2, 0x2C(r1) #0xC0+VEC3_x
	slwi r4, r31, 16
	addi r5, r1, 0x2C #0xC0+VEC3_x			# pos
	stfs f1, 0x30(r1) #0xC0+VEC3_y

	li r3, 327								# classID					#old is 0x69
	li r6, 0								# rot
	li r7, 0								# layer

	stfs f0, 0x34(r1) #0xC0+VEC3_z
	lbz r0, 0x1035(r30)
	lbz r10, 0x38D(r30) #dAcPy_c._._.associatedPlayerID
	lbz r9, 0x27CC(r30)
	slwi r0, r0, 12
	lbz r8, 0x38F(r30)
	or r0, r4, r0
	extsb r10, r10
	slwi r9, r9, 4
	slwi r4, r8, 8
	or r8, r10, r9
	or r0, r4, r0

	or r4, r8, r0							# settings
	bl CreateActor							# 105 PL_FIREBALL
	
ReturnFromThrowHammer:
	lwz r0, 0xC4(sp)
	psq_l f31, 0xB8(sp), 1, 0
	lfd f31, 0xB0(sp)
	psq_l f30, 0xA8(sp), 1, 0
	lfd f30, 0xA0(sp)
	lwz r31, 0x9C(sp)
	lwz r30, 0x98(sp)
	mtlr r0
	addi sp, sp, 0xC0
	blr



.global MoreProjectileStuff_Fix
MoreProjectileStuff_Fix:
	lwz r3, 0x1090(r31)		#r3 = this->powerup
	cmplwi r3, 7			#if it's <7
	blt ReturnFromMPSFix	#then ReturnFromMPSFix
	cmplwi r3, 12			#if it's 12 (Cloud)
	beq ReturnFromMPSFix
	
	mr r3, r31				#Otherwise, move and set some things, then PlayerProjectileShooting
	li r4, 1
	bl PlayerProjectileShooting
	
ReturnFromMPSFix:
	lwz r0, 0x14(sp)
	lwz r31, 0xC(sp)
	mtlr r0
	addi sp, sp, 0x10
	blr


.extern DoGoldFromProjectileShootCheck_Fix

.global ProjectileShootCheck_Fix #this is what limits the amount of projectiles
ProjectileShootCheck_Fix:
	lwz r3, 0x1090(r31)		#r3 = this->powerup
	cmplwi r3, 7			#if it's less than 7
	blt Return0fromPSCFix	#then Return0fromPSCFix
	cmplwi r3, 12			#if it 12 (cloud)
	beq Return0fromPSCFix	#then Return0fromPSCFix
	
	cmplwi r3, 7			#If it's 7 (hammer), then do the hammer checking
	beq DoHammerFromProjectileShootCheck_Fix

	cmplwi r3, 8			#If it's 8 (gold), then do the gold checking
	beq _DoGoldFromProjectileShootCheck_Fix

	cmplwi r3, 9			#If it's 9 (spike), then do the spike checking
	beq DoBoomerangFromProjectileShootCheck_Fix

	cmplwi r3, 10			#If it's 10 (boomerang), then do the boomerang checking
	beq DoBoomerangFromProjectileShootCheck_Fix

	cmplwi r3, 11			#If it's 11 (frog), then do the frog checking
	beq DoBoomerangFromProjectileShootCheck_Fix

	#No need of the cloud

	cmplwi r3, 13			#If it's 13 (wand), then do the wand checking
	beq DoBoomerangFromProjectileShootCheck_Fix

	blr

DoHammerFromProjectileShootCheck_Fix:
	lbz r3, 0x38D(r31)
	li r4, 0
	extsb r3, r3
	bl CheckHammerLimit
	b FinishFromProjectileShootCheck_Fix

DoBoomerangFromProjectileShootCheck_Fix:
	lbz r3, 0x38D(r31)
	li r4, 0
	extsb r3, r3
	bl CheckBoomerangLimit
	b FinishFromProjectileShootCheck_Fix

_DoGoldFromProjectileShootCheck_Fix:
	b DoGoldFromProjectileShootCheck_Fix

Return0fromPSCFix:
	li r3, 0
	b ReturnFromPSCFix
	
Return1fromPSCFix:
	li r3, 1
	
ReturnFromPSCFix:
	lwz r0, 0x14(sp)
	lwz r31, 0xC(sp)
	mtlr r0
	addi sp, sp, 0x10
	blr

FinishFromProjectileShootCheck_Fix:
	cmpwi r3, 0
	beq Return0fromPSCFix
	b Return1fromPSCFix


.global CheckHammerLimit
CheckHammerLimit:
	stwu sp, -0x10(sp)
	mflr r0
	stw r0, 0x14(sp)
	stw r31, 0xC(sp)
	stw r30, 0x8(sp)
	
	# r30 = player number
	mr r30, r3
	addi r30, r30, 0x100
	
	# r31 = hammer count so far
	li r31, 0
	
	# search for every Hammer
	
	li r3, 0
	b startOrContinueLoop
loop:
	
	lwz r4, 0x458(r3)
	cmpw r4, r30
	bne startOrContinueLoop
	
	addi r31, r31, 1
	
startOrContinueLoop:
	mr r4, r3
	li r3, 0x73
	bl Actor_SearchByName
	
	cmpwi r3, 0
	bne loop
	
	# now if the current count is >= 2, return 0
	# otherwise return 1
	cmpwi r31, 2
	bge tooManyHammers
	
	li r3, 1
	b gotHammerStatus
	
tooManyHammers:
	li r3, 0
	
gotHammerStatus:
	lwz r0, 0x14(sp)
	lwz r31, 0xC(sp)
	lwz r30, 0x8(sp)
	mtlr r0
	addi sp, sp, 0x10
	blr


.data
.align 4
.global doWait
doWait: .byte 0x0, 0x0, 0x0, 0x0

.text
.align 4
.global CheckBoomerangLimit
CheckBoomerangLimit:
	stwu sp, -0x10(sp)
	mflr r0
	stw r0, 0x14(sp)
	stw r31, 0xC(sp)
	stw r30, 0x8(sp)
	
	# r3 = player number

	lis r4, doWait@h
	ori r4, r4, doWait@l
	add r4, r4, r3
	lbz r4, 0(r4) #store doWait[player number] in r4
	
	# now if the frames to wait are > 0, return 0
	# otherwise return 1
	cmpwi r4, 0
	bgt tooManyBoomerangs
	
	li r3, 1
	b gotBoomerangStatus
	
tooManyBoomerangs:
	li r3, 0
	
gotBoomerangStatus:
	lwz r0, 0x14(sp)
	lwz r31, 0xC(sp)
	lwz r30, 0x8(sp)
	mtlr r0
	addi sp, sp, 0x10
	blr



.extern ContinueFromTinyTable
.global FixThisTinyTable
FixThisTinyTable:
	lis r5, NewTinyTable@h
	ori r5, r5, NewTinyTable@l
	b ContinueFromTinyTable




.extern continueToadPowerupSet
.global ToadPowerupFix
ToadPowerupFix:
	cmpwi r4, 3
	beq isHammerForToad
	cmpwi r4, 4
	beq isBoomerangForToad
	cmpwi r4, 5
	beq isSpikeForToad
	cmpwi r4, 6
	beq isBobombForToad
	cmpwi r4, 7
	beq isFrogForToad
	cmpwi r4, 8
	beq isThunderForToad
	cmpwi r4, 9
	beq isCloudForToad

	lis r8, PowerupTextures@h
	ori r8, r8, PowerupTextures@l
	slwi r9, r4, 2
	lfsx f1, r8, r9
	fadds f31, f31, f1
	b continueToadPowerupSet

isHammerForToad:
	lis r8, ToadTextures@h
	ori r8, r8, ToadTextures@l
	slwi r9, r0, 2
	lfsx f31, r8, r9
	b continueToadPowerupSet

isBoomerangForToad:
	lis r8, ToadBoomerTextures@h
	ori r8, r8, ToadBoomerTextures@l
	slwi r9, r0, 2
	lfsx f31, r8, r9
	b continueToadPowerupSet

isSpikeForToad:
	lis r8, ToadSpikeTextures@h
	ori r8, r8, ToadSpikeTextures@l
	slwi r9, r0, 2
	lfsx f31, r8, r9
	b continueToadPowerupSet

isBobombForToad:
	lis r8, ToadBobombTextures@h
	ori r8, r8, ToadBobombTextures@l
	slwi r9, r0, 2
	lfsx f31, r8, r9
	b continueToadPowerupSet

isFrogForToad:
	lis r8, ToadFrogTextures@h
	ori r8, r8, ToadFrogTextures@l
	slwi r9, r0, 2
	lfsx f31, r8, r9
	b continueToadPowerupSet

isThunderForToad:
	lis r8, ToadBobombTextures@h
	ori r8, r8, ToadBobombTextures@l
	slwi r9, r0, 2
	lfsx f31, r8, r9
	b continueToadPowerupSet

isCloudForToad:
	lis r8, ToadCloudTextures@h
	ori r8, r8, ToadCloudTextures@l
	slwi r9, r0, 2
	lfsx f31, r8, r9
	b continueToadPowerupSet






.global CheckExistingPowerup
CheckExistingPowerup:
	cmpwi r3, 0
	beq noPlayerForCheckExistingPowerup
	lwz r3, 0x14E0(r3)
	blr
noPlayerForCheckExistingPowerup:
	li r3, -0x1
	blr





.global AcPyCtorHS
AcPyCtorHS:
	stwu r1, -0x10(r1)
	mflr r0
	stw r0, 0x14(r1)
	stw r31, 0xC(r1)
	mr r31, r3

	bl build__21dHammerSuitRenderer_cFv
	stw r3, 0x2D08(r31)

	mr r3, r31
	lwz r31, 0xC(r1)
	lwz r0, 0x14(r1)
	mtlr r0
	addi r1, r1, 0x10
	blr

.global AcPyDtorHS
AcPyDtorHS:
	stwu r1, -0x10(r1)
	mflr r0
	stw r0, 0x14(r1)
	stw r31, 0xC(r1)
	stw r30, 8(r1)

	mr r31, r4
	mr r30, r3

	cmpwi r3, 0
	beq oops2
	lwz r3, 0x2D08(r30)
	li r4, 1
	bl __dt__21dHammerSuitRenderer_cFv
oops2:
	
	mr r3, r30
	mr r4, r31
	bl RealAcPyDtor

	lwz r30, 8(r1)
	lwz r31, 0xC(r1)
	lwz r0, 0x14(r1)
	mtlr r0
	addi r1, r1, 0x10
	blr

.global AcPySetMeUpHS
AcPySetMeUpHS:
	# partway in: r31 = this
	lwz r3, 0x2D08(r31)
	addi r4, r31, 0x2A60
	bl setup__21dHammerSuitRenderer_cFP21dPlayerModelHandler_c

	# epilogue
	li r3, 1
	lwz r31, 0xC(r1)
	lwz r0, 0x14(r1)
	mtlr r0
	addi r1, r1, 0x10
	blr

.global AcPyDrawHS
AcPyDrawHS:
	# partway in: r31 = this
	lwz r3, 0x2D08(r31)
	bl draw__21dHammerSuitRenderer_cFv

	# epilogue
	li r3, 1
	lwz r0, 0x14(r1)
	lwz r31, 0xC(r1)
	mtlr r0
	addi r1, r1, 0x10
	blr



# DO IT AGAIN
.global Wm2dPlayerCtorHS
Wm2dPlayerCtorHS:
	stwu r1, -0x10(r1)
	mflr r0
	stw r0, 0x14(r1)
	stw r31, 0xC(r1)
	mr r31, r3

	bl build__21dHammerSuitRenderer_cFv
	stw r3, 0x384(r31)

	mr r3, r31
	lwz r31, 0xC(r1)
	lwz r0, 0x14(r1)
	mtlr r0
	addi r1, r1, 0x10
	blr

.global Wm2dPlayerDtorHS
Wm2dPlayerDtorHS:
	stwu r1, -0x10(r1)
	mflr r0
	stw r0, 0x14(r1)
	stw r31, 0xC(r1)
	stw r30, 8(r1)

	mr r31, r4
	mr r30, r3

	cmpwi r3, 0
	beq oops
	lwz r3, 0x384(r30)
	li r4, 1
	bl __dt__21dHammerSuitRenderer_cFv

oops:
	mr r3, r30
	mr r4, r31
	bl RealWm2dPlayerDtor

	lwz r30, 8(r1)
	lwz r31, 0xC(r1)
	lwz r0, 0x14(r1)
	mtlr r0
	addi r1, r1, 0x10
	blr

.global Wm2dPlayerSetMeUpHS
Wm2dPlayerSetMeUpHS:
	# partway in: r27 = this
	lwz r3, 0x384(r27)
	lwz r4, 0x128(r27)
	li r5, 1
	bl setup__21dHammerSuitRenderer_cFP21dPlayerModelHandler_ci

	# epilogue
	addi r11, r1, 0x20
	bl _restgpr_27
	lwz r0, 0x24(r1)
	mtlr r0
	addi r1, r1, 0x20
	blr

.global Wm2dPlayerDrawHS
Wm2dPlayerDrawHS:
	# dicks (a very insightful comment)
	lbz r0, 0x261(r3)
	cmpwi r0, 0
	beqlr

	stwu r1, -0x10(r1)
	mflr r0
	stw r0, 0x14(r1)
	stw r31, 0xC(r1)
	mr r31, r3

	lwz r3, 0x384(r31)
	bl draw__21dHammerSuitRenderer_cFv

	lwz r3, 0x128(r31)
	bl draw__21dPlayerModelHandler_cFv

	lwz r0, 0x14(r1)
	lwz r31, 0xC(r1)
	mtlr r0
	addi r1, r1, 0x10
	blr

.extern _restgpr_27
.extern RealAcPyDtor
.extern RealWm2dPlayerDtor
.extern build__21dHammerSuitRenderer_cFv
.extern __dt__21dHammerSuitRenderer_cFv
.extern setup__21dHammerSuitRenderer_cFP21dPlayerModelHandler_ci
.extern setup__21dHammerSuitRenderer_cFP21dPlayerModelHandler_c
.extern draw__21dHammerSuitRenderer_cFv
.extern draw__21dPlayerModelHandler_cFv

.extern GabonRock_DropOutState
.extern GabonRock_CrushState


.global HammerGabonRock
HammerGabonRock:
	# r27 = apOther
	# r28 = apThis
	# r29 = objOther
	# r31 = objThis
	lbz r0, 0x2D(r27)
	cmpwi r0, 19
	bne hgrExit

	addi r3, r31, 0x394
	lis r4, GabonRock_CrushState@h
	ori r4, r4, GabonRock_CrushState@l
	lwz r12, 0(r3)
	lwz r12, 0x18(r12)
	mtctr r12
	bctrl

	lbz r0, 0xA1(r28)
	ori r0, r0, 2
	stb r0, 0xA1(r28)
hgrExit:
	lwz r27, 0x2C(r1)
	lwz r28, 0x30(r1)
	lwz r29, 0x34(r1)
	lwz r30, 0x38(r1)
	lwz r31, 0x3C(r1)
	lwz r0, 0x44(r1)
	mtlr r0
	addi r1, r1, 0x40
	blr

.global HammerCallbackGabonRockOverride
.extern forceHCGROPushBack
.extern continueFromHCGRO
HammerCallbackGabonRockOverride:
	lhz r3, 8(r30) #objThis
	cmpwi r3, 115 #HAMMER
	bne continueHCGRO

	lhz r3, 8(r30) #objOther
	cmpwi r3, BoomerangHax
	bne continueHCGRO

	b forceHCGROPushBack

continueHCGRO:
	lbz r0, 0x2D(r4) #to fix the branch we nuked
	b continueFromHCGRO


.data

PowerupSounds:
	.long 273, 274, 274, 275, 276, 277, 274, 274, 2002, 274, 274, 274, 274, 274


SomeTable:
	.long SomeTable_802F5580
	.long SomeTable_802F56C0
	.long SomeTable_802F56C0
	.long SomeTable_802F5440
	.long SomeTable_802F56C0
	.long SomeTable_802F56C0
	.long SomeTable_802F56C0
	.long SomeTable_802F56C0
	.long SomeTable_802F56C0
	.long SomeTable_802F56C0
	.long SomeTable_802F56C0
	.long SomeTable_802F56C0
	.long SomeTable_802F56C0
	.long SomeTable_802F56C0

ZeroFloat:
	.float 0.0

OneFloat:
	.float 1.0

FukidashiTable2:
	.float 20.0, 32.0, 32.0, 12.0, 38.0, 33.0, 32.0, 32.0, 32.0, 32.0, 32.0, 33.0, 32.0, 32.0
	.float 20.0, 34.0, 34.0, 12.0, 39.0, 35.0, 34.0, 34.0, 34.0, 34.0, 34.0, 35.0, 34.0, 34.0
	.float 20.0, 29.0, 29.0, 11.0, 34.0, 29.0, 29.0, 29.0, 29.0, 29.0, 29.0, 29.0, 29.0, 29.0
	.float 20.0, 29.0, 29.0, 11.0, 34.0, 29.0, 29.0, 29.0, 29.0, 29.0, 29.0, 29.0, 29.0, 29.0

PlayerHeightsNormal:
	.float 16.0, 27.0, 27.0, 12.0, 27.0, 27.0, 27.0, 27.0, 27.0, 27.0, 27.0, 27.0, 27.0, 27.0

PlayerHeightsDucking:
	.float 13.0, 21.0, 21.0, 9.0, 21.0, 21.0, 21.0, 21.0, 21.0, 21.0, 21.0, 21.0, 21.0, 21.0

WtfEverTable:
	.float 4.5, 10.0, 10.0, 1.0, 10.0, 9.0, 10.0, 10.0, 10.0, 10.0, 10.0, 9.0, 10.0, 10.0

YetAnotherTable:
	.float 0.0, -4.0, -4.0, 4.0, -6.0, -2.0, -4.0, -4.0, -4.0, -4.0, -4.0, -2.0, -4.0, -4.0

WeirdScaleTable:
	.float 16.0, 31.0, 31.0, 6.0, 34.0, 31.0, 31.0, 31.0, 31.0, 31.0, 31.0, 31.0, 31.0, 31.0

.align 4
AsdfTable:
	.byte 1, 2, 2, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0

.align 4
BlahTableOffsets:
	.long 1, 2, 2, 0, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0

NewTinyTable:
	.long 1, 1, 1, 1
	.long 1, 1, 1, 1, 1, 1, 1, 1, 1, 1

PowerupScaleFloats:
	.long 0xDEADBEEF, 0xDEADBEEF # padding
	.float 1.0
	.float 1.0
	.float 1.0
	.float 0.6
	.float 1.0
	.float 1.0
	.float 1.0
	.float 1.0
	.float 1.0
	.float 1.0
	.float 1.0
	.float 1.0
	.float 1.0
	.float 1.0

PowerupTextures:
	.float 0.0
	.float 1.0
	.float 2.0
	.float 3.0
	.float 4.0
	.float 5.0
	.float 6.0
	.float 7.0
	.float 8.0
	.float 9.0
ToadTextures:
	.long 0xDEADBEEF, 0xDEADBEEF
	.float 7.0, 8.0, 6.0
ToadBoomerTextures:
	.long 0xDEADBEEF, 0xDEADBEEF
	.float 9.0, 10.0, 6.0
ToadSpikeTextures:
	.long 0xDEADBEEF, 0xDEADBEEF
	.float 11.0, 12.0, 6.0
ToadBobombTextures:
	.long 0xDEADBEEF, 0xDEADBEEF
	.float 13.0, 14.0, 6.0
ToadFrogTextures:
	.long 0xDEADBEEF, 0xDEADBEEF
	.float 15.0, 16.0, 6.0
ToadThunderTextures:
	.long 0xDEADBEEF, 0xDEADBEEF
	.float 17.0, 18.0, 6.0
ToadCloudTextures:
	.long 0xDEADBEEF, 0xDEADBEEF
	.float 19.0, 20.0, 6.0

AlternateQBlockTable:
	.long 0x10
	.long 0x12
	.long 0x13
	.long 0x14
	.long 0x16
	.long 0x11
	.long 0x19
	.long 1
	.long 0x1B
	.long 0xC
	.long 4
	.long 7
	.long 0xA
	.long 0xD
	.long 8
	.long 0xE
	.long 5
	.long 6

.global BrosArcFileName
BrosArcFileName:
	.string "hammerM"

.global I_hammerResFile
I_hammerResFile:
#	.string "g3d/I_hammer.brres"
	.string "g3d/I_fireflower.brres"

.global I_hammerModelName
I_hammerModelName:
#	.string "I_hammer"
	.string "I_fireflower"

.global g3d_I_kinoko_brres
g3d_I_kinoko_brres: .string "g3d/I_kinoko.brres"

.global I_propeller_model
I_propeller_model: .string "I_propeller_model"

.global SI_kinoko
SI_kinoko: .string "SI_kinoko"

.global SI_hammer
SI_hammer: .string "SI_hammer"

.global SI_boomerang
SI_boomerang: .string "SI_boomerang"

.global SI_spike
SI_spike: .string "SI_spike"

.global SI_wand
SI_wand: .string "SI_wand"

.global SI_frog
SI_frog: .string "SI_frog"

.global SI_gold
SI_gold: .string "SI_gold"

.global SI_cloud
SI_cloud: .string "SI_cloud"

.global I_hammerArcFilename
I_hammerArcFilename:
	.string "I_hammer"

.global I_boomerangArcFilename
I_boomerangArcFilename:
	.string "I_boomerang"

.global I_spikeArcFilename
I_spikeArcFilename:
	.string "I_spike"

.global I_wandArcFilename
I_wandArcFilename:
	.string "I_wand"

.global I_frogArcFilename
I_frogArcFilename:
	.string "I_frog"

.global I_goldArcFilename
I_goldArcFilename:
	.string "I_gold"

.global I_cloudArcFilename
I_cloudArcFilename:
	.string "I_cloud"

.global OSShit
OSShit: .string "yay %d\n"

.global OSShitTwo
OSShitTwo: .string "that's a boomerang\n"

g3d: .string "g3d/"

brres: .string ".brres"

g3dmodel: .string "g3d/model.brres"

.text
.global AddHSToBattleItem_FixWait2Anim
AddHSToBattleItem_FixWait2Anim:
	lis r4, wait2@h
	ori r4, r4, wait2@l
	blr

.global InsertLostG3D
InsertLostG3D:
	lis r4, g3d@h
	ori r4, r4, g3d@l
	blr

.global InsertLostBRRES
InsertLostBRRES:
	lis r4, brres@h
	ori r4, r4, brres@l
	blr

.global InsertLostG3DModel
InsertLostG3DModel:
	lis r5, g3dmodel@h
	ori r5, r5, g3dmodel@l
	blr
.data
wait2: .string "wait2"

#PowerupTexDebugStr:
#	.string "Plumber::SetPowerupTexture() called on class %p with texnum %d lr=%p\n"

#DBString:
#	.string "PW:%d\n"

#.global BrosModelFilename
#BrosModelFilename:
#	.string "g3d/bros.brres"

#.global HammerModelName
#HammerModelName:
#	.string "bros_hammer"

.text
.extern continueFromStrongBox
.global StrongboxInit
StrongboxInit:
	# MODE
	li r4, 2
	stw r4, 0x8E0(r30)
	# ITEM COUNT (nybble 8)
	extrwi r4, r7, 4, 12
	stw r4, 0x8E8(r30)
	b continueFromStrongBox

.extern continueFromStrongBox2
.extern AddStockPowerup
.extern create__13dStageActor_cF6ActorsUiP7Point3dP6S16VecUc
.global StrongboxGiveBattleItem
StrongboxGiveBattleItem:
	# save a reg we can use
	stw r29, 0x24(r1)

	# get number of current item
	lhz r6, 0x8FA(r30)
	# create a shift amount...
	slwi r6, r6, 2
	lwz r7, 4(r30)
	# ..and use it to get the item
	srw r7, r7, r6
	clrlwi r29, r7, 28

	# now give it
	lis r4, IndexItemsForStockItem@h
	ori r4, r4, IndexItemsForStockItem@l
	lbzx r4, r4, r29
	bl AddStockPowerup

	# and then copy the position
	lfs f1, 0xAC(r30)
	stfs f1, 0x10(r1)
	lfs f1, 0xB0(r30)
	lfs f2, 0x8D8(r30)
	fsubs f1, f1, f2
	stfs f1, 0x14(r1)
	lfs f1, 0xB4(r30)
	stfs f1, 0x18(r1)

	li r3, 0x25
	slwi r4, r31, 4
	or r4, r4, r29
	addi r5, r1, 0x10
	li r6, 0
	li r7, 0
	bl create__13dStageActor_cF6ActorsUiP7Point3dP6S16VecUc

	# restore our reg
	lwz r29, 0x24(r1)
	b continueFromStrongBox2

.data
IndexItemsForStockItem:
.byte 0,1,2,3,5,6,4,7,8,9,10,11,12,13


SLGWindow: .string "A00_Window"
SLGBtn0: .string "B00_itemButton"
SLGBtn1: .string "B01_itemButton"
SLGBtn2: .string "B02_itemButton"
SLGBtn3: .string "B03_itemButton"
SLGBtn4: .string "B04_itemButton"
SLGBtn5: .string "B05_itemButton"
SLGBtn6: .string "B06_itemButton"
SLGBtn7: .string "HammerGroup"
SLGBtn8: .string "GoldenGroup"
SLGBtn9: .string "SpikyGroup"
SLGBtn10: .string "BoomerGroup"
SLGBtn11: .string "FroggyGroup"
SLGBtn12: .string "CloudyGroup"
SLGBtn13: .string "WandGroup"
SLButtonBase00: .string "P_buttonBase_00"
SLButtonBase01: .string "P_buttonBase_01"
SLButtonBase02: .string "P_buttonBase_02"
SLButtonBase03: .string "P_buttonBase_03"
SLButtonBase04: .string "P_buttonBase_04"
SLButtonBase05: .string "P_buttonBase_05"
SLButtonBase06: .string "P_buttonBase_06"
SLButtonBase07: .string "P_buttonBase_07"
SLButtonBase08: .string "P_buttonBase_08"
SLButtonBase09: .string "P_buttonBase_09"
SLButtonBase10: .string "P_buttonBase_10"
SLButtonBase11: .string "P_buttonBase_11"
SLButtonBase12: .string "P_buttonBase_12"
SLButtonBase13: .string "P_buttonBase_13"
SLIcon0: .string "P_iconKinoko_00"
SLIcon1: .string "P_iconFlower_00"
SLIcon2: .string "P_iconPro_00"
SLIcon3: .string "P_iconIce_00"
SLIcon4: .string "P_iconPen_00"
SLIcon5: .string "P_mameKinoko_00"
SLIcon6: .string "P_iconStar_00"
SLIcon7: .string "P_iconHammer_00"
SLIcon8: .string "P_iconGold_00"
SLIcon9: .string "P_iconSpike_00"
SLIcon10: .string "P_iconBoomer_00"
SLIcon11: .string "P_iconFrog_00"
SLIcon12: .string "P_iconCloud_00"
SLIcon13: .string "P_iconWand_00"
SLIconPane0: .string "N_iconKinoko_00"
SLIconPane1: .string "N_iconFlower_00"
SLIconPane2: .string "N_iconPro_00"
SLIconPane3: .string "N_iconIce_00"
SLIconPane4: .string "N_iconPen_00"
SLIconPane5: .string "N_mameKinoko_00"
SLIconPane6: .string "N_iconStar_00"
SLIconPane7: .string "N_iconHammer_00"
SLIconPane8: .string "N_iconGold_00"
SLIconPane9: .string "N_iconSpike_00"
SLIconPane10: .string "N_iconBoomer_00"
SLIconPane11: .string "N_iconFrog_00"
SLIconPane12: .string "N_iconCloud_00"
SLIconPane13: .string "N_iconWand_00"
.align 4
SILayoutGroupNames:
.long SLGWindow
.long SLGBtn0,SLGBtn1,SLGBtn2,SLGBtn6,SLGBtn4,SLGBtn5,SLGBtn3
.long SLGBtn0,SLGBtn1,SLGBtn2,SLGBtn6,SLGBtn4,SLGBtn5,SLGBtn3
.long SLGBtn0,SLGBtn1,SLGBtn2,SLGBtn6,SLGBtn4,SLGBtn5,SLGBtn3
.long SLGBtn0,SLGBtn1,SLGBtn2,SLGBtn6,SLGBtn4,SLGBtn5,SLGBtn3
.long SLGWindow
.long SLGBtn7,SLGBtn7,SLGBtn7
.long SLGBtn8,SLGBtn8,SLGBtn8
.long SLGBtn9,SLGBtn9,SLGBtn9
.long SLGBtn10,SLGBtn10,SLGBtn10
.long SLGBtn11,SLGBtn11,SLGBtn11
.long SLGBtn12,SLGBtn12,SLGBtn12
.long SLGBtn13,SLGBtn13,SLGBtn13
SILayoutGroupIDs:
.long 0
.long 1,1,1,1,1,1,1
.long 2,2,2,2,2,2,2
.long 3,3,3,3,3,3,3
.long 4,4,4,4,4,4,4
.long 5
.long 1,3,4
.long 1,3,4
.long 1,3,4
.long 1,3,4
.long 1,3,4
.long 1,3,4
.long 1,3,4

SILayoutButtonBases:
.long SLButtonBase00,SLButtonBase01,SLButtonBase02,SLButtonBase06
.long SLButtonBase04,SLButtonBase05,SLButtonBase03,SLButtonBase07,SLButtonBase08
.long SLButtonBase09,SLButtonBase10,SLButtonBase11,SLButtonBase12,SLButtonBase13
SILayoutIconPictures:
.long SLIcon0,SLIcon1,SLIcon2,SLIcon3
.long SLIcon4,SLIcon5,SLIcon6,SLIcon7,SLIcon8
.long SLIcon9,SLIcon10,SLIcon11,SLIcon12,SLIcon13
SILayoutIconPanes:
.long SLIconPane0,SLIconPane1,SLIconPane2,SLIconPane3
.long SLIconPane4,SLIconPane5,SLIconPane6,SLIconPane7,SLIconPane8
.long SLIconPane9,SLIconPane10,SLIconPane11,SLIconPane12,SLIconPane13

.text
.global SILayoutGroupParams
SILayoutGroupParams:
	lis r4, SILayoutGroupNames@h
	ori r4, r4, SILayoutGroupNames@l
	lis r5, SILayoutGroupIDs@h
	ori r5, r5, SILayoutGroupIDs@l
	li r6, 0x33
	blr

# - 0x950 : Button Bases 	#Old: 0x928
# - 0x988 : Icon Panes 		#Old: 0x94C
# - 0x9C0 : Icon Pictures 	#Old: 0x970
.extern getPanes__Q23m2d13EmbedLayout_cCFPPCcPPQ34nw4r3lyt4Panei
.extern getPictures__Q23m2d13EmbedLayout_cCFPPCcPPQ34nw4r3lyt7Picturei
.extern ContinueFromSILayoutExtraStuffs
.extern IsWideScreen__Fv
.global SILayoutExtraStuffs
SILayoutExtraStuffs:
	addi r3, r30, 0x74
	lis r4, SILayoutButtonBases@h
	ori r4, r4, SILayoutButtonBases@l
	addi r5, r30, 0x950
	li r6, 14
	bl getPictures__Q23m2d13EmbedLayout_cCFPPCcPPQ34nw4r3lyt7Picturei

	addi r3, r30, 0x74
	lis r4, SILayoutIconPanes@h
	ori r4, r4, SILayoutIconPanes@l
	addi r5, r30, 0x988
	li r6, 14
	bl getPanes__Q23m2d13EmbedLayout_cCFPPCcPPQ34nw4r3lyt4Panei

	addi r3, r30, 0x74
	lis r4, SILayoutIconPictures@h
	ori r4, r4, SILayoutIconPictures@l
	addi r5, r30, 0x9C0
	li r6, 14
	bl getPictures__Q23m2d13EmbedLayout_cCFPPCcPPQ34nw4r3lyt7Picturei

	bl IsWideScreen__Fv
	cmpwi r3, 1
	beq dontScaleDownSI
	lis r3, float0_85@h
	ori r3, r3, float0_85@l
	lfs f1, 0(r3)
	lwz r3, 0x248(r30)
	stfs f1, 0x44(r3)
	stfs f1, 0x48(r3)
dontScaleDownSI:

	li r3, 1
	b ContinueFromSILayoutExtraStuffs

.global HideItemsByDefault
.extern ContinueFromHideItemsByDefault
HideItemsByDefault:
	li r3, 14
	mtctr r3
	mr r4, r31
meNext:
	lwz r9, 0x9C0(r4)
	lbz r5, 0xBB(r9)
	rlwinm r5, r5, 0,24,30
	stb r5, 0xBB(r9)
	addi r4, r4, 4
	bdnz meNext

	li r8, 0
	li r7, 1
	li r6, 4
	li r5, 0xE
	li r0, 0xF
	li r3, 1
	b ContinueFromHideItemsByDefault

.global AddHammerPowerupFinalValue
AddHammerPowerupFinalValue:
	li r24, 0
	lis r22, OutputPowerupsByItemID@h
	ori r22, r22, OutputPowerupsByItemID@l
	blr

.data
OutputPowerupsByItemID:
.long 1, 2, 4, 6, 5, 3, -1, 7, 8, 9, 10, 11, 12, 13

.text
.global FixStockItemEffectPlayer
FixStockItemEffectPlayer:
	lwz r0, 0x8A0(r30)
	slwi r0, r0, 2
	add r5, r30, r0
	lwz r28, 0x988(r5)
	blr

.global FixStockItemEffectPlayer2
FixStockItemEffectPlayer2:
	lwz r0, 0x8A0(r30)
	stw r0, 0x8D0(r30)
	blr

.global FixMoreSIEffectRelatedCrap
FixMoreSIEffectRelatedCrap:
	lwz r0, 0x8D0(r31)
	slwi r0, r0, 2
	add r3, r31, r0
	lwz r29, 0x988(r3)
	blr


.global AddHammerSuitAddSound
.extern MakeSFISound
.extern ContinueFromSIAddSound
AddHammerSuitAddSound:
	cmpwi r0, 7
	beq _MakeSFISound
	cmpwi r0, 8
	beq _MakeSFISound
	cmpwi r0, 9
	beq _MakeSFISound
	cmpwi r0, 10
	beq _MakeSFISound
	cmpwi r0, 11
	beq _MakeSFISound
	cmpwi r0, 12
	beq _MakeSFISound
	cmpwi r0, 13
	beq _MakeSFISound
	b ContinueFromSIAddSound
	_MakeSFISound:
	b MakeSFISound

.extern resetAnim__Q23m2d13EmbedLayout_cFib
.extern ContinueFromResetHammerAnimOnOpen
.global ResetHammerAnimOnOpen
ResetHammerAnimOnOpen:
	addi r3, r28, 0x74
	li r4, 0x1E
	li r5, 0
	bl resetAnim__Q23m2d13EmbedLayout_cFib
	addi r3, r28, 0x74
	li r4, 0x21
	li r5, 0
	bl resetAnim__Q23m2d13EmbedLayout_cFib
	addi r3, r28, 0x74
	li r4, 0x24
	li r5, 0
	bl resetAnim__Q23m2d13EmbedLayout_cFib
	addi r3, r28, 0x74
	li r4, 0x27
	li r5, 0
	bl resetAnim__Q23m2d13EmbedLayout_cFib
	addi r3, r28, 0x74
	li r4, 0x2A
	li r5, 0
	bl resetAnim__Q23m2d13EmbedLayout_cFib
	addi r3, r28, 0x74
	li r4, 0x2D
	li r5, 0
	bl resetAnim__Q23m2d13EmbedLayout_cFib
	addi r3, r28, 0x74
	li r4, 0x30
	li r5, 0
	bl resetAnim__Q23m2d13EmbedLayout_cFib
	addi r3, r28, 0x74
	b ContinueFromResetHammerAnimOnOpen

.extern ChosenStartChoice
.global PickHammerAsPossibleStartChoice
PickHammerAsPossibleStartChoice:
	lwz r0, 0x930(r30)
	li r3, 6
	cmpwi r0, 0
	beq notGoodEnough
	stw r3, 0x8A0(r30)
	b ChosenStartChoice
notGoodEnough:
	lwz r0, 0x934(r30)
	li r3, 7
	cmpwi r0, 0
	beq stillNotGoodEnough
	stw r3, 0x8A0(r30)
	b ChosenStartChoice
stillNotGoodEnough:
	lwz r0, 0x938(r30)
	li r3, 8
	cmpwi r0, 0
	beq alwaysNotGoodEnough
	stw r3, 0x8A0(r30)
	b ChosenStartChoice
alwaysNotGoodEnough:
	lwz r0, 0x93C(r30)
	li r3, 9
	cmpwi r0, 0
	beq ewNotGoodEnough
	stw r3, 0x8A0(r30)
	b ChosenStartChoice
ewNotGoodEnough:
	lwz r0, 0x940(r30)
	li r3, 10
	cmpwi r0, 0
	beq grrNotGoodEnough
	stw r3, 0x8A0(r30)
	b ChosenStartChoice
grrNotGoodEnough:
	lwz r0, 0x944(r30)
	li r3, 11
	cmpwi r0, 0
	beq stupidNotGoodEnough
	stw r3, 0x8A0(r30)
	b ChosenStartChoice
stupidNotGoodEnough:
	lwz r0, 0x948(r30)
	li r3, 12
	cmpwi r0, 0
	beq everytimeNotGoodEnough
	stw r3, 0x8A0(r30)
	b ChosenStartChoice
everytimeNotGoodEnough:
	lwz r0, 0x94C(r30)
	li r3, 13
	cmpwi r0, 0
	beq _ChosenStartChoice
	stw r3, 0x8A0(r30)
	_ChosenStartChoice:
	b ChosenStartChoice


.global ChooseOldButtonOffAnim
ChooseOldButtonOffAnim:
	cmpwi r4, 7
	beq useHammerOffAnim
	cmpwi r4, 8
	beq useGoldOffAnim
	cmpwi r4, 9
	beq useSpikeOffAnim
	cmpwi r4, 10
	beq useBoomerangOffAnim
	cmpwi r4, 11
	beq useFrogOffAnim
	cmpwi r4, 12
	beq useCloudOffAnim
	cmpwi r4, 13
	beq useWandOffAnim
	addi r4, r4, 0x16
	blr
useHammerOffAnim:
	li r4, 0x20
	blr
useGoldOffAnim:
	li r4, 0x23
	blr
useSpikeOffAnim:
	li r4, 0x26
	blr
useBoomerangOffAnim:
	li r4, 0x29
	blr
useFrogOffAnim:
	li r4, 0x2C
	blr
useCloudOffAnim:
	li r4, 0x2F
	blr
useWandOffAnim:
	li r4, 0x32
	blr

.global ChooseOldButtonOffAnim2
ChooseOldButtonOffAnim2:
	cmpwi r3, 7
	beq useHammerOffAnim2
	cmpwi r3, 8
	beq useGoldOffAnim2
	cmpwi r4, 9
	beq useSpikeOffAnim2
	cmpwi r4, 10
	beq useBoomerangOffAnim2
	cmpwi r4, 11
	beq useFrogOffAnim2
	cmpwi r4, 12
	beq useCloudOffAnim2
	cmpwi r4, 13
	beq useWandOffAnim2
	addi r0, r3, 0x16
	blr
useHammerOffAnim2:
	li r0, 0x20
	blr
useGoldOffAnim2:
	li r0, 0x23
	blr
useSpikeOffAnim2:
	li r0, 0x26
	blr
useBoomerangOffAnim2:
	li r0, 0x29
	blr
useFrogOffAnim2:
	li r0, 0x2C
	blr
useCloudOffAnim2:
	li r0, 0x2F
	blr
useWandOffAnim2:
	li r0, 0x32
	blr

.global ChooseButtonOnAnim
ChooseButtonOnAnim:
	cmpwi r4, 7
	beq useHammerOnAnim
	cmpwi r4, 8
	beq useGoldOnAnim
	cmpwi r4, 9
	beq useSpikeOnAnim
	cmpwi r4, 10
	beq useBoomerangOnAnim
	cmpwi r4, 11
	beq useFrogOnAnim
	cmpwi r4, 12
	beq useCloudOnAnim
	cmpwi r4, 13
	beq useWandOnAnim
	addi r4, r4, 1
	blr
useHammerOnAnim:
	li r4, 0x1E
	blr
useGoldOnAnim:
	li r4, 0x21
	blr
useSpikeOnAnim:
	li r4, 0x24
	blr
useBoomerangOnAnim:
	li r4, 0x27
	blr
useFrogOnAnim:
	li r4, 0x2A
	blr
useCloudOnAnim:
	li r4, 0x2D
	blr
useWandOnAnim:
	li r4, 0x30
	blr

.global DoSISelectCursor
.extern UpdateSelectCursor
DoSISelectCursor:
	slwi r0, r0, 2
	add r3, r3, r0
	lwz r3, 0x950(r3)
	li r4, 0
	li r5, 0
	b UpdateSelectCursor


.global ChooseHitButtonAnim
ChooseHitButtonAnim:
	cmpwi r4, 7
	beq useHammerHitAnim
	cmpwi r4, 8
	beq useGoldHitAnim
	cmpwi r4, 9
	beq useSpikeHitAnim
	cmpwi r4, 10
	beq useBoomerangHitAnim
	cmpwi r4, 11
	beq useFrogHitAnim
	cmpwi r4, 12
	beq useCloudHitAnim
	cmpwi r4, 13
	beq useWandHitAnim
	addi r4, r4, 0xF
	blr
useHammerHitAnim:
	li r4, 0x1F
	blr
useGoldHitAnim:
	li r4, 0x22
	blr
useSpikeHitAnim:
	li r4, 0x25
	blr
useBoomerangHitAnim:
	li r4, 0x28
	blr
useFrogHitAnim:
	li r4, 0x2B
	blr
useCloudHitAnim:
	li r4, 0x2E
	blr
useWandHitAnim:
	li r4, 0x31
	blr



.global UpdateCalcPowerupResult
.extern ContinueFromCalcPowerupResult
UpdateCalcPowerupResult:
	cmpwi r3, 7
	beq cprIsDoneHammer
	cmpwi r3, 8
	beq cprIsDoneGold
	cmpwi r3, 9
	beq cprIsDoneSpike
	cmpwi r3, 10
	beq cprIsDoneBoomerang
	cmpwi r3, 11
	beq cprIsDoneFrog
	cmpwi r3, 12
	beq cprIsDoneCloud
	cmpwi r3, 13
	beq cprIsDoneWand
	b ContinueFromCalcPowerupResult
cprIsDoneHammer:
	li r6, 7
	stb r0, 0xF(r1)
	b ContinueFromCalcPowerupResult
cprIsDoneGold:
	li r6, 8
	stb r0, 0xF(r1)
	b ContinueFromCalcPowerupResult
cprIsDoneSpike:
	li r6, 9
	stb r0, 0xF(r1)
	b ContinueFromCalcPowerupResult
cprIsDoneBoomerang:
	li r6, 10
	stb r0, 0xF(r1)
	b ContinueFromCalcPowerupResult
cprIsDoneFrog:
	li r6, 11
	stb r0, 0xF(r1)
	b ContinueFromCalcPowerupResult
cprIsDoneCloud:
	li r6, 12
	stb r0, 0xF(r1)
	b ContinueFromCalcPowerupResult
cprIsDoneWand:
	li r6, 13
	stb r0, 0xF(r1)
	b ContinueFromCalcPowerupResult

.global FixCalcPowerupResultForMushroom
.extern ContinueFromFixCalcPowerupResultForMushroom
FixCalcPowerupResultForMushroom:
	stb r0, 0xF(r1)
	li r6, 1
	b ContinueFromFixCalcPowerupResultForMushroom



# - 0x29C: Hammer value textbox 	#Old: 0x288
# - 0x2A0: Hammer X textbox 		#Old: 0x28C
# - 0x2A4: Hammer shadow 			#Old: 0x290
# - 0x2A8: Gold value textbox 	#Old: 0x294
# - 0x2AC: Gold X textbox 		#Old: 0x298
# - 0x2B0: Gold shadow 		#Old: 0x29C
# - 0x2B4: Spike value textbox
# - 0x2B8: Spike X textbox
# - 0x2BC: Spike shadow
# - 0x2C0: Boomerang value textbox
# - 0x2C4: Boomerang X textbox
# - 0x2C8: Boomerang shadow
# - 0x2CC: Frog value textbox
# - 0x2D0: Frog X textbox
# - 0x2D4: Frog shadow
# - 0x2D8: Cloud value textbox
# - 0x2DC: Cloud X textbox
# - 0x2E0: Cloud shadow
# - 0x2E4: Wand value textbox
# - 0x2E8: Wand X textbox
# - 0x2EC: Wand shadow
.global SISLayoutExtraStuffs
.extern ContinueFromSISLayoutExtraStuffs
.extern findTextBoxByName__Q23m2d17EmbedLayoutBase_cCFPCc
.extern findPictureByName__Q23m2d17EmbedLayoutBase_cCFPCc
SISLayoutExtraStuffs:
	addi r3, r30, 0x70
	lis r4, t_itemRest_15@h
	ori r4, r4, t_itemRest_15@l
	bl findTextBoxByName__Q23m2d17EmbedLayoutBase_cCFPCc	#Hammer Value Textbox
	stw r3, 0x29C(r30)

	addi r3, r30, 0x70
	lis r4, t_itemRest_14@h
	ori r4, r4, t_itemRest_14@l
	bl findTextBoxByName__Q23m2d17EmbedLayoutBase_cCFPCc	#Hammer X Textbox
	stw r3, 0x2A0(r30)

	addi r3, r30, 0x70
	lis r4, p_buttonBaseS_07@h
	ori r4, r4, p_buttonBaseS_07@l
	bl findPictureByName__Q23m2d17EmbedLayoutBase_cCFPCc	#Hammer Shadow
	stw r3, 0x2A4(r30)

	addi r3, r30, 0x70
	lis r4, t_itemRest_16@h
	ori r4, r4, t_itemRest_16@l
	bl findTextBoxByName__Q23m2d17EmbedLayoutBase_cCFPCc	#Gold Value Textbox
	stw r3, 0x2A8(r30)

	addi r3, r30, 0x70
	lis r4, t_itemRest_17@h
	ori r4, r4, t_itemRest_17@l
	bl findTextBoxByName__Q23m2d17EmbedLayoutBase_cCFPCc	#Gold X Textbox
	stw r3, 0x2AC(r30)

	addi r3, r30, 0x70
	lis r4, p_buttonBaseS_08@h
	ori r4, r4, p_buttonBaseS_08@l
	bl findPictureByName__Q23m2d17EmbedLayoutBase_cCFPCc	#Gold Shadow
	stw r3, 0x2B0(r30)

	addi r3, r30, 0x70
	lis r4, t_itemRest_18@h
	ori r4, r4, t_itemRest_18@l
	bl findTextBoxByName__Q23m2d17EmbedLayoutBase_cCFPCc	#Spike Value Textbox
	stw r3, 0x2B4(r30)

	addi r3, r30, 0x70
	lis r4, t_itemRest_19@h
	ori r4, r4, t_itemRest_19@l
	bl findTextBoxByName__Q23m2d17EmbedLayoutBase_cCFPCc	#Spike X Textbox
	stw r3, 0x2B8(r30)

	addi r3, r30, 0x70
	lis r4, p_buttonBaseS_09@h
	ori r4, r4, p_buttonBaseS_09@l
	bl findPictureByName__Q23m2d17EmbedLayoutBase_cCFPCc	#Spike Shadow
	stw r3, 0x2BC(r30)

	addi r3, r30, 0x70
	lis r4, t_itemRest_20@h
	ori r4, r4, t_itemRest_20@l
	bl findTextBoxByName__Q23m2d17EmbedLayoutBase_cCFPCc	#Boomerang Value Textbox
	stw r3, 0x2C0(r30)

	addi r3, r30, 0x70
	lis r4, t_itemRest_21@h
	ori r4, r4, t_itemRest_21@l
	bl findTextBoxByName__Q23m2d17EmbedLayoutBase_cCFPCc	#Boomerang X Textbox
	stw r3, 0x2C4(r30)

	addi r3, r30, 0x70
	lis r4, p_buttonBaseS_10@h
	ori r4, r4, p_buttonBaseS_10@l
	bl findPictureByName__Q23m2d17EmbedLayoutBase_cCFPCc	#Boomerang Shadow
	stw r3, 0x2C8(r30)

	addi r3, r30, 0x70
	lis r4, t_itemRest_22@h
	ori r4, r4, t_itemRest_22@l
	bl findTextBoxByName__Q23m2d17EmbedLayoutBase_cCFPCc	#Frog Value Textbox
	stw r3, 0x2CC(r30)

	addi r3, r30, 0x70
	lis r4, t_itemRest_23@h
	ori r4, r4, t_itemRest_23@l
	bl findTextBoxByName__Q23m2d17EmbedLayoutBase_cCFPCc	#Frog X Textbox
	stw r3, 0x2D0(r30)

	addi r3, r30, 0x70
	lis r4, p_buttonBaseS_11@h
	ori r4, r4, p_buttonBaseS_11@l
	bl findPictureByName__Q23m2d17EmbedLayoutBase_cCFPCc	#Frog Shadow
	stw r3, 0x2D4(r30)

	addi r3, r30, 0x70
	lis r4, t_itemRest_24@h
	ori r4, r4, t_itemRest_24@l
	bl findTextBoxByName__Q23m2d17EmbedLayoutBase_cCFPCc	#Cloud Value Textbox
	stw r3, 0x2D8(r30)

	addi r3, r30, 0x70
	lis r4, t_itemRest_25@h
	ori r4, r4, t_itemRest_25@l
	bl findTextBoxByName__Q23m2d17EmbedLayoutBase_cCFPCc	#Cloud X Textbox
	stw r3, 0x2DC(r30)

	addi r3, r30, 0x70
	lis r4, p_buttonBaseS_12@h
	ori r4, r4, p_buttonBaseS_12@l
	bl findPictureByName__Q23m2d17EmbedLayoutBase_cCFPCc	#Cloud Shadow
	stw r3, 0x2E0(r30)

	addi r3, r30, 0x70
	lis r4, t_itemRest_26@h
	ori r4, r4, t_itemRest_26@l
	bl findTextBoxByName__Q23m2d17EmbedLayoutBase_cCFPCc	#Wand Value Textbox
	stw r3, 0x2E4(r30)

	addi r3, r30, 0x70
	lis r4, t_itemRest_27@h
	ori r4, r4, t_itemRest_27@l
	bl findTextBoxByName__Q23m2d17EmbedLayoutBase_cCFPCc	#Wand X Textbox
	stw r3, 0x2E8(r30)

	addi r3, r30, 0x70
	lis r4, p_buttonBaseS_13@h
	ori r4, r4, p_buttonBaseS_13@l
	bl findPictureByName__Q23m2d17EmbedLayoutBase_cCFPCc	#Wand Shadow
	stw r3, 0x2EC(r30)
	
	li r3, 1
	b ContinueFromSISLayoutExtraStuffs

.data
t_itemRest_14: .string "T_itemRest_14"			#Hammer
t_itemRest_15: .string "T_itemRest_15"
p_buttonBaseS_07: .string "P_buttonBaseS_07"

t_itemRest_17: .string "T_itemRest_17"			#Gold
t_itemRest_16: .string "T_itemRest_16"
p_buttonBaseS_08: .string "P_buttonBaseS_08"

t_itemRest_19: .string "T_itemRest_19"			#Spike
t_itemRest_18: .string "T_itemRest_18"
p_buttonBaseS_09: .string "P_buttonBaseS_09"

t_itemRest_21: .string "T_itemRest_21"			#Boomerang
t_itemRest_20: .string "T_itemRest_20"
p_buttonBaseS_10: .string "P_buttonBaseS_10"

t_itemRest_23: .string "T_itemRest_23"			#Frog
t_itemRest_22: .string "T_itemRest_22"
p_buttonBaseS_11: .string "P_buttonBaseS_11"

t_itemRest_25: .string "T_itemRest_25"			#Cloud
t_itemRest_24: .string "T_itemRest_24"
p_buttonBaseS_12: .string "P_buttonBaseS_12"

t_itemRest_27: .string "T_itemRest_27"			#Wand
t_itemRest_26: .string "T_itemRest_26"
p_buttonBaseS_13: .string "P_buttonBaseS_13"

.text
.global UpdateSIS
.extern ContinueFromUpdateSIS
.extern WriteNumberToTextBox__FPiPCiPQ34nw4r3lyt7TextBoxb
UpdateSIS:
	cmpwi r0, 0
	beq DontUpdateSIS
	subi r25, r25, 0x1C

	# replicate this shit
	lwz r0, 0x280(r25)
	cmpwi r0, 0
	lwz r3, 0x2A4(r25)
	lbz r0, 0xBB(r3)
	beq theresNoHammer

	li r4, 1
	rlwinm r0, r0, 0,24,30
	b changedBase1
theresNoHammer:
	li r4, 0
	ori r0, r0, 1
changedBase1:
	stb r0, 0xBB(r3)
	slwi r29, r4, 2
	addi r5, r1, 0x14

	lwzx r27, r26, r29
	li r4, 0
	stw r27, 0x14(r1)
	lwz r3, 0x29C(r25)
	lwz r12, 0(r3)
	lwz r12, 0x28(r12)
	mtctr r12
	bctrl

	lwzx r29, r28, r29
	addi r5, r1, 0x10
	stw r29, 0x10(r1)
	li r4, 2
	lwz r3, 0x29C(r25)
	lwz r12, 0(r3)
	lwz r12, 0x28(r12)
	mtctr r12
	bctrl

	addi r5, r1, 0x14
	li r4, 0
	lwz r3, 0x2A0(r25)
	lwz r12, 0(r3)
	lwz r12, 0x28(r12)
	mtctr r12
	bctrl

	addi r5, r1, 0x10
	li r4, 2
	lwz r3, 0x2A0(r25)
	lwz r12, 0(r3)
	lwz r12, 0x28(r12)
	mtctr r12
	bctrl

	lwz r5, 0x29C(r25)
	addi r3, r25, 0x280
	lis r4, ptrToTwo@h
	ori r4, r4, ptrToTwo@l
	li r6, 1
	bl WriteNumberToTextBox__FPiPCiPQ34nw4r3lyt7TextBoxb



	# replicate this shit another time
	lwz r0, 0x284(r25)
	cmpwi r0, 0
	lwz r3, 0x2B0(r25)
	lbz r0, 0xBB(r3)
	beq theresNoGold

	li r4, 1
	rlwinm r0, r0, 0,24,30
	b changedBase2
theresNoGold:
	li r4, 0
	ori r0, r0, 1
changedBase2:
	#here, redo it, bastard
	stb r0, 0xBB(r3)
	slwi r29, r4, 2
	addi r5, r1, 0x14

	lwzx r27, r26, r29
	li r4, 0
	stw r27, 0x14(r1)
	lwz r3, 0x2A8(r25)
	lwz r12, 0(r3)
	lwz r12, 0x28(r12)
	mtctr r12
	bctrl

	lwzx r29, r28, r29
	addi r5, r1, 0x10
	stw r29, 0x10(r1)
	li r4, 2
	lwz r3, 0x2A8(r25)
	lwz r12, 0(r3)
	lwz r12, 0x28(r12)
	mtctr r12
	bctrl

	addi r5, r1, 0x14
	li r4, 0
	lwz r3, 0x2AC(r25)
	lwz r12, 0(r3)
	lwz r12, 0x28(r12)
	mtctr r12
	bctrl

	addi r5, r1, 0x10
	li r4, 2
	lwz r3, 0x2AC(r25)
	lwz r12, 0(r3)
	lwz r12, 0x28(r12)
	mtctr r12
	bctrl

	lwz r5, 0x2A8(r25)
	addi r3, r25, 0x284
	lis r4, ptrToTwo@h
	ori r4, r4, ptrToTwo@l
	li r6, 1
	bl WriteNumberToTextBox__FPiPCiPQ34nw4r3lyt7TextBoxb



	# replicate this shit another time
	lwz r0, 0x288(r25)
	cmpwi r0, 0
	lwz r3, 0x2BC(r25)
	lbz r0, 0xBB(r3)
	beq theresNoSpike

	li r4, 1
	rlwinm r0, r0, 0,24,30
	b changedBase3
theresNoSpike:
	li r4, 0
	ori r0, r0, 1
changedBase3:
	#here, redo it yet another time, bastard
	stb r0, 0xBB(r3)
	slwi r29, r4, 2
	addi r5, r1, 0x14

	lwzx r27, r26, r29
	li r4, 0
	stw r27, 0x14(r1)
	lwz r3, 0x2B4(r25)
	lwz r12, 0(r3)
	lwz r12, 0x28(r12)
	mtctr r12
	bctrl

	lwzx r29, r28, r29
	addi r5, r1, 0x10
	stw r29, 0x10(r1)
	li r4, 2
	lwz r3, 0x2B4(r25)
	lwz r12, 0(r3)
	lwz r12, 0x28(r12)
	mtctr r12
	bctrl

	addi r5, r1, 0x14
	li r4, 0
	lwz r3, 0x2B8(r25)
	lwz r12, 0(r3)
	lwz r12, 0x28(r12)
	mtctr r12
	bctrl

	addi r5, r1, 0x10
	li r4, 2
	lwz r3, 0x2B8(r25)
	lwz r12, 0(r3)
	lwz r12, 0x28(r12)
	mtctr r12
	bctrl

	lwz r5, 0x2B4(r25)
	addi r3, r25, 0x288
	lis r4, ptrToTwo@h
	ori r4, r4, ptrToTwo@l
	li r6, 1
	bl WriteNumberToTextBox__FPiPCiPQ34nw4r3lyt7TextBoxb



	# replicate this shit another time
	lwz r0, 0x28C(r25)
	cmpwi r0, 0
	lwz r3, 0x2C8(r25)
	lbz r0, 0xBB(r3)
	beq theresNoBoomerang

	li r4, 1
	rlwinm r0, r0, 0,24,30
	b changedBase4
theresNoBoomerang:
	li r4, 0
	ori r0, r0, 1
changedBase4:
	#here, redo it yet another time, bastard
	stb r0, 0xBB(r3)
	slwi r29, r4, 2
	addi r5, r1, 0x14

	lwzx r27, r26, r29
	li r4, 0
	stw r27, 0x14(r1)
	lwz r3, 0x2C0(r25)
	lwz r12, 0(r3)
	lwz r12, 0x28(r12)
	mtctr r12
	bctrl

	lwzx r29, r28, r29
	addi r5, r1, 0x10
	stw r29, 0x10(r1)
	li r4, 2
	lwz r3, 0x2C0(r25)
	lwz r12, 0(r3)
	lwz r12, 0x28(r12)
	mtctr r12
	bctrl

	addi r5, r1, 0x14
	li r4, 0
	lwz r3, 0x2C4(r25)
	lwz r12, 0(r3)
	lwz r12, 0x28(r12)
	mtctr r12
	bctrl

	addi r5, r1, 0x10
	li r4, 2
	lwz r3, 0x2C4(r25)
	lwz r12, 0(r3)
	lwz r12, 0x28(r12)
	mtctr r12
	bctrl

	lwz r5, 0x2C0(r25)
	addi r3, r25, 0x28C
	lis r4, ptrToTwo@h
	ori r4, r4, ptrToTwo@l
	li r6, 1
	bl WriteNumberToTextBox__FPiPCiPQ34nw4r3lyt7TextBoxb



	# replicate this shit another time
	lwz r0, 0x290(r25)
	cmpwi r0, 0
	lwz r3, 0x2D4(r25)
	lbz r0, 0xBB(r3)
	beq theresNoFrog

	li r4, 1
	rlwinm r0, r0, 0,24,30
	b changedBase5
theresNoFrog:
	li r4, 0
	ori r0, r0, 1
changedBase5:
	#here, redo it yet another time, bastard
	stb r0, 0xBB(r3)
	slwi r29, r4, 2
	addi r5, r1, 0x14

	lwzx r27, r26, r29
	li r4, 0
	stw r27, 0x14(r1)
	lwz r3, 0x2CC(r25)
	lwz r12, 0(r3)
	lwz r12, 0x28(r12)
	mtctr r12
	bctrl

	lwzx r29, r28, r29
	addi r5, r1, 0x10
	stw r29, 0x10(r1)
	li r4, 2
	lwz r3, 0x2CC(r25)
	lwz r12, 0(r3)
	lwz r12, 0x28(r12)
	mtctr r12
	bctrl

	addi r5, r1, 0x14
	li r4, 0
	lwz r3, 0x2D0(r25)
	lwz r12, 0(r3)
	lwz r12, 0x28(r12)
	mtctr r12
	bctrl

	addi r5, r1, 0x10
	li r4, 2
	lwz r3, 0x2D0(r25)
	lwz r12, 0(r3)
	lwz r12, 0x28(r12)
	mtctr r12
	bctrl

	lwz r5, 0x2CC(r25)
	addi r3, r25, 0x290
	lis r4, ptrToTwo@h
	ori r4, r4, ptrToTwo@l
	li r6, 1
	bl WriteNumberToTextBox__FPiPCiPQ34nw4r3lyt7TextBoxb



	# replicate this shit another time
	lwz r0, 0x294(r25)
	cmpwi r0, 0
	lwz r3, 0x2E0(r25)
	lbz r0, 0xBB(r3)
	beq theresNoCloud

	li r4, 1
	rlwinm r0, r0, 0,24,30
	b changedBase6
theresNoCloud:
	li r4, 0
	ori r0, r0, 1
changedBase6:
	#here, redo it yet another time, bastard
	stb r0, 0xBB(r3)
	slwi r29, r4, 2
	addi r5, r1, 0x14

	lwzx r27, r26, r29
	li r4, 0
	stw r27, 0x14(r1)
	lwz r3, 0x2D8(r25)
	lwz r12, 0(r3)
	lwz r12, 0x28(r12)
	mtctr r12
	bctrl

	lwzx r29, r28, r29
	addi r5, r1, 0x10
	stw r29, 0x10(r1)
	li r4, 2
	lwz r3, 0x2D8(r25)
	lwz r12, 0(r3)
	lwz r12, 0x28(r12)
	mtctr r12
	bctrl

	addi r5, r1, 0x14
	li r4, 0
	lwz r3, 0x2DC(r25)
	lwz r12, 0(r3)
	lwz r12, 0x28(r12)
	mtctr r12
	bctrl

	addi r5, r1, 0x10
	li r4, 2
	lwz r3, 0x2DC(r25)
	lwz r12, 0(r3)
	lwz r12, 0x28(r12)
	mtctr r12
	bctrl

	lwz r5, 0x2D8(r25)
	addi r3, r25, 0x294
	lis r4, ptrToTwo@h
	ori r4, r4, ptrToTwo@l
	li r6, 1
	bl WriteNumberToTextBox__FPiPCiPQ34nw4r3lyt7TextBoxb



	# replicate this shit another time
	lwz r0, 0x298(r25)
	cmpwi r0, 0
	lwz r3, 0x2EC(r25)
	lbz r0, 0xBB(r3)
	beq theresNoWand

	li r4, 1
	rlwinm r0, r0, 0,24,30
	b changedBase7
theresNoWand:
	li r4, 0
	ori r0, r0, 1
changedBase7:
	#here, redo it yet another time, bastard
	stb r0, 0xBB(r3)
	slwi r29, r4, 2
	addi r5, r1, 0x14

	lwzx r27, r26, r29
	li r4, 0
	stw r27, 0x14(r1)
	lwz r3, 0x2E4(r25)
	lwz r12, 0(r3)
	lwz r12, 0x28(r12)
	mtctr r12
	bctrl

	lwzx r29, r28, r29
	addi r5, r1, 0x10
	stw r29, 0x10(r1)
	li r4, 2
	lwz r3, 0x2E4(r25)
	lwz r12, 0(r3)
	lwz r12, 0x28(r12)
	mtctr r12
	bctrl

	addi r5, r1, 0x14
	li r4, 0
	lwz r3, 0x2E8(r25)
	lwz r12, 0(r3)
	lwz r12, 0x28(r12)
	mtctr r12
	bctrl

	addi r5, r1, 0x10
	li r4, 2
	lwz r3, 0x2E8(r25)
	lwz r12, 0(r3)
	lwz r12, 0x28(r12)
	mtctr r12
	bctrl

	lwz r5, 0x2E4(r25)
	addi r3, r25, 0x298
	lis r4, ptrToTwo@h
	ori r4, r4, ptrToTwo@l
	li r6, 1
	bl WriteNumberToTextBox__FPiPCiPQ34nw4r3lyt7TextBoxb

DontUpdateSIS:
	addi r11, r1, 0x40
	b ContinueFromUpdateSIS


.extern ContinueFromThatFix
.global TryToFixThis
TryToFixThis:
	lbz r0, 0x152(r31)		#r0 = this->powerupId
	cmpwi r0, 8				#if(r0 == 8)
	bge StopBeingAnAss		# { StopBeingAnAss() }
	lwz r0, 0x770(r31)		#else { r0 = this->current_player_model_id
	b ContinueFromThatFix	# ContinueFromThatFix() }
StopBeingAnAss:
	li r0, 0				#r0 = 0
	stw r0, 0x770(r31)		#this->current_player_model_id = r0
	b ContinueFromThatFix	#ContinueFromThatFix()


.global NewPowerupArcTable
NewPowerupArcTable:
	lis r27, PowerupArcTable@h
	ori r27, r27, PowerupArcTable@l
	blr


.global NewPowerupBrresTable
NewPowerupBrresTable:
	lis r5, PowerupBRRESTable@h
	ori r5, r5, PowerupBRRESTable@l
	blr


.global NewPowerupModelTable
NewPowerupModelTable:
	lis r4, PowerupModelTable@h
	ori r4, r4, PowerupModelTable@l
	blr


.global NewENITEMPowerupTable
NewENITEMPowerupTable:
	lis r4, ENITEMPowerupTable@h
	ori r4, r4, ENITEMPowerupTable@l
	blr


#=========================up down select for SI menu=========================#

.global is_it_right
is_it_right:
	beq        new_not_right
	b          it_is_right

.global new_not_right
new_not_right:
	lis        r3, ActiveWiimoteID@h
	ori        r3, r3, ActiveWiimoteID@l
    rlwinm     r0,r0,0x2,0x0,0x1d
    add        r3,r29,r0
	lwz        r3,0x4(r3)
    lwz        r0,0x1c(r3)
    rlwinm.    r0,r0,0x0,0x1e,0x1e                   #r0 = r0 & 2
	beq        not_up

    li         r0,0x3
    stw        r0,0x314(r30)
    or         r3,r30,r30
    addi       r4,r1,0x8
    bl         dStockItem__manipulateItemID
	b          not_right_it_was

.global not_up
not_up:
	lis        r3, ActiveWiimoteID@h
	ori        r3, r3, ActiveWiimoteID@l
    rlwinm     r0,r0,0x2,0x0,0x1d
    add        r3,r29,r0
	lwz        r3,0x4(r3)
    lwz        r0,0x1c(r3)
    rlwinm.    r0,r0,0x0,0x1f,0x1f                   #r0 = r0 & 1
	beq        not_right_it_was

    li         r0,0x4
    stw        r0,0x314(r30)
    or         r3,r30,r30
    addi       r4,r1,0x8
    bl         dStockItem__manipulateItemID

.global not_right_it_was
not_right_it_was:
	b          not_right

.global is_it_right2
is_it_right2:
	beq        new_not_right2
	b          it_is_right2

.global new_not_right2
new_not_right2:
	lis        r3, ActiveWiimoteID@h
	ori        r3, r3, ActiveWiimoteID@l
    rlwinm     r0,r0,0x2,0x0,0x1d
    add        r3,r29,r0
	lwz        r3,0x4(r3)
    lwz        r0,0x1c(r3)
    rlwinm.    r0,r0,0x0,0x1e,0x1e                   #r0 = r0 & 2
	beq        not_up2

    li         r0,0x3
    stw        r0,0x314(r30)
    or         r3,r30,r30
    addi       r4,r1,0x8
    bl         dStockItem__manipulateItemID
	b          not_right_it_was2

.global not_up2
not_up2:
	lis        r3, ActiveWiimoteID@h
	ori        r3, r3, ActiveWiimoteID@l
    rlwinm     r0,r0,0x2,0x0,0x1d
    add        r3,r29,r0
	lwz        r3,0x4(r3)
    lwz        r0,0x1c(r3)
    rlwinm.    r0,r0,0x0,0x1f,0x1f                   #r0 = r0 & 1
	beq        not_right_it_was2

    li         r0,0x4
    stw        r0,0x314(r30)
    or         r3,r30,r30
    addi       r4,r1,0x8
    bl         dStockItem__manipulateItemID

.global not_right_it_was2
not_right_it_was2:
	b          not_right2

.global selectNewActionsForButtons
selectNewActionsForButtons:
	beq        tryAndMoveLeft_
	cmpwi      r0,0x3
	blt        tryAndMoveRight_
	beq        tryAndMoveUp
	b          tryAndMoveDown

.global tryAndMoveRight_
tryAndMoveRight_:
	b          tryAndMoveRight

.global tryAndMoveLeft_
tryAndMoveLeft_:
	b          tryAndMoveLeft

.global upLoop
upLoop:
    lwz        r5,0x0(r4)
    subic.     r0,r5,0x7
    stw        r0,0x0(r4)
    blt        tryAndMoveUp
    rlwinm     r0,r0,0x2,0x0,0x1d
    add        r5,r3,r0
    lwz        r0,0x918(r5)
    cmpwi      r0,0x0
    bnelr

.global tryAndMoveUp
tryAndMoveUp:
	li         r0,0x1
    stw        r0,0x314(r3)
	lwz        r0,0x0(r4)
	cmpwi      r0,0x0
	bge        upLoop
	b          cantChange

.global downLoop
downLoop:
    lwz        r5,0x0(r4)
    addi       r0,r5,0x7
    stw        r0,0x0(r4)
    cmpwi      r0,0xe
	bgt        tryAndMoveDown
    rlwinm     r0,r0,0x2,0x0,0x1d
    add        r5,r3,r0
    lwz        r0,0x918(r5)
    cmpwi      r0,0x0
    bnelr

.global tryAndMoveDown
tryAndMoveDown:
	li         r0,0x1
    stw        r0,0x314(r3)
	lwz        r0,0x0(r4)
	cmpwi      r0,0x7
	blt        downLoop
	b          cantChange

.global goBackTo0
goBackTo0:
	bge		   setTo0
	b          continueRightLoop

.global setTo0
setTo0:
	li         r0, 0x0
	stw        r0,0x0(r4)
	b          continueRightLoop

.global goForthToD
goForthToD:
	blt		   setToD
	b          continueLeftLoop

.global setToD
setToD:
	li         r0, 0xD
	stw        r0,0x0(r4)
	b          continueLeftLoop



#=========================end of up down select for SI menu=========================#


#=========================GoldFlowerAppearSound=========================#

.global GoldFlowerAppearSound
GoldFlowerAppearSound:
	cmpwi r29, 0x14
	beq _setGoldSFX
	li r4, 599 # SE_OBJ_ITEM_APPEAR
	blr
_setGoldSFX:
	li r4, 2001
	blr

#=========================end of GoldFlowerAppearSound=========================#




.data
.align 4
ptrToTwo: .long 2
float0_85: .float 0.85

PowerupBRRESTable:
	.long aI_kinokoBRRES_6
	.long aI_fireflowerBRRES_1
	.long aI_propellerBRRES_4
	.long aI_iceflowerBRRES_4
	.long aI_kinokoBRRES_6
	.long aI_starBRRES_4
	.long aI_penguinBRRES_4
	.long aI_fireflowerBRRES_1
	.long aI_fireflowerBRRES_1
	.long aI_fireflowerBRRES_1
	.long aI_fireflowerBRRES_1
	.long aI_fireflowerBRRES_1
	.long aI_fireflowerBRRES_1
	.long aI_fireflowerBRRES_1

PowerupModelTable:
	.long aI_kinoko_6
	.long aI_fireflower_1
	.long aI_propeller_4
	.long aI_iceflower_4
	.long aI_kinoko_6
	.long aI_star_4
	.long aI_penguin_4
	.long aI_iceflower_4
	.long aI_iceflower_4
	.long aI_iceflower_4
	.long aI_iceflower_4
	.long aI_iceflower_4
	.long aI_iceflower_4
	.long aI_iceflower_4

PowerupArcTable:
	.long aI_kinoko_6
	.long aI_fireflower_1
	.long aI_propeller_4
	.long aI_iceflower_4
	.long aI_penguin_4
	.long aI_kinoko_6
	.long aI_star_4
	.long I_hammerArcFilename2
	.long I_goldArcFilename2
	.long I_spikeArcFilename2
	.long I_boomerangArcFilename2
	.long I_frogArcFilename2
	.long I_cloudArcFilename2
	.long I_wandArcFilename2

ENITEMPowerupTable:
	.long aI_kinoko_6
	.long aI_fireflower_1
	.long aI_star_4
	.long aObj_coin_5
	.long aI_iceflower_4
	.long I_hammerArcFilename2
	.long aI_kinoko_6
	.long aI_kinoko_6
	.long I_spikeArcFilename2
	.long I_wandArcFilename2
	.long I_frogArcFilename2
	.long aI_propeller_4
	.long I_goldArcFilename2
	.long aI_kinoko_6
	.long aI_penguin_4
	.long I_boomerangArcFilename2
	.long aObj_coin_5
	.long I_cloudArcFilename2

aI_kinokoBRRES_6:
	.string "g3d/I_kinoko.brres"

aI_fireflowerBRRES_1:
	.string "g3d/I_fireflower.brres"

aI_propellerBRRES_4:
	.string "g3d/I_propeller.brres"

aI_iceflowerBRRES_4:
	.string "g3d/I_iceflower.brres"

aI_penguinBRRES_4:
	.string "g3d/I_penguin.brres"

aI_starBRRES_4:
	.string "g3d/I_star.brres"

aI_kinoko_6:
	.string "I_kinoko"

aI_fireflower_1:
	.string "I_fireflower"

aI_propeller_4:
	.string "I_propeller"

aI_iceflower_4:
	.string "I_iceflower"

aI_penguin_4:
	.string "I_penguin"

aI_star_4:
	.string "I_star"

I_hammerArcFilename2:
	.string "I_hammer"

I_goldArcFilename2:
	.string "I_gold"

I_spikeArcFilename2:
	.string "I_spike"

I_boomerangArcFilename2:
	.string "I_boomerang"

I_frogArcFilename2:
	.string "I_frog"

I_cloudArcFilename2:
	.string "I_cloud"

I_wandArcFilename2:
	.string "I_wand"

aObj_coin_5:
	.string "obj_coin"

.text
.global WMItem_AddBrres, WMItem_AddMdl0

WMItem_AddBrres:
	lwzx r4, r27, r0
	cmpwi r0, 28
	beq doFireFlowerBrres
	cmpwi r0, 32
	beq doFireFlowerBrres	
	cmpwi r0, 36
	beq doFireFlowerBrres	
	cmpwi r0, 40
	beq doFireFlowerBrres	
	cmpwi r0, 44
	beq doFireFlowerBrres	
	cmpwi r0, 48
	beq doFireFlowerBrres	
	cmpwi r0, 52
	beq doFireFlowerBrres	
	blr
doFireFlowerBrres:
	lis r5, FireflowerFileName@h
	ori r5, r5, FireflowerFileName@l
	blr
WMItem_AddMdl0:
	lwzx r4, r4, r0
	cmpwi r0, 28
	beq doFireFlowerMdl0
	cmpwi r0, 32
	beq doFireFlowerMdl0
	cmpwi r0, 36
	beq doFireFlowerMdl0
	cmpwi r0, 40
	beq doFireFlowerMdl0
	cmpwi r0, 44
	beq doFireFlowerMdl0
	cmpwi r0, 48
	beq doFireFlowerMdl0
	cmpwi r0, 52
	beq doFireFlowerMdl0
	blr
doFireFlowerMdl0:
	lis r4, FireflowerModelName@h
	ori r4, r4, FireflowerModelName@l
	blr

.extern callFooDeath
.global HammerFoo
HammerFoo:
	b callFooDeath

.extern dPlayerMdl_c__createPlayerModel
.global createPlayerModelOrig
createPlayerModelOrig:
	stwu r1, -0x70(r1)
	b dPlayerMdl_c__createPlayerModel

.data
FireflowerFileName: .string "g3d/I_fireflower.brres"
FireflowerModelName: .string "I_fireflower"
.align 4

