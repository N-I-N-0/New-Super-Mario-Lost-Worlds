.text
.align 4

#include <asm_setup.S>
#include <asm_profiles.S>

.extern CreateParentedObject__FsPvic
.extern CheckForButtonPress__Fv

.extern doneWithSpawnShopAndLevelCredits
.extern doneWithCheckForButtonPress
.extern doneWithCheckForNewCSMBytes

.extern continueCheckForNewCSMBytes

.extern returnCheckForNewCSMBytes

.extern StateID_ShopWait__22dCourseSelectManager_c
.extern StateID_CreatorInfoWait__22dCourseSelectManager_c

.global SpawnShopAndLevelCredits
SpawnShopAndLevelCredits:
	stw r3, 0x538(r31) # Restore instruction

	li r3, WMShop
	mr r4, r29
	li r5, 0
	li r6, 0
	bl CreateParentedObject__FsPvic
	stw r3, 0x570(r31)

	li r3, CreatorInfo
	mr r4, r29
	li r5, 0
	li r6, 0
	bl CreateParentedObject__FsPvic
	stw r3, 0x574(r31)

	li r3, 0
	stb r3, 0x578(r31) # 0'ing them here so I don't have to mess with CSM's constructor
	stb r3, 0x579(r31) # Those two will be used as "is in shop" and "is in level credits" bytes



	li r3, WM_SETTINGS
	mr r4, r29
	li r5, 0
	li r6, 0
	bl CreateParentedObject__FsPvic
	stw r3, 0x57C(r31)

	#li r3, WM_STARSIGN # WM_STARSIGN
	#mr r4, r29
	#li r5, 0
	#li r6, 0
	#bl CreateParentedObject__FsPvic
	#stw r3, 0x580(r31)




	b doneWithSpawnShopAndLevelCredits

.global CheckForButtonPress
CheckForButtonPress:
	bl CheckForButtonPress__Fv

	mr r3, r30
	lwz r0, 0x168(r3)
	b doneWithCheckForButtonPress

.global CheckForNewCSMBytes
CheckForNewCSMBytes:
	beq Byte54D_is_0
	b continueCheckForNewCSMBytes

Byte54D_is_0:
	lbz r0, 0x578(r3)
	cmpwi r0, 0
	beq ShopByte_is_0

	lis r4, StateID_ShopWait__22dCourseSelectManager_c@h
	ori r4, r4, StateID_ShopWait__22dCourseSelectManager_c@l
	lwzu r12, 0x70(r3)
	lwz r12, 0x18(r12)
	mtctr r12
	bctrl
	b returnCheckForNewCSMBytes

ShopByte_is_0:
	lbz r0, 0x579(r3)
	cmpwi r0, 0
	beq CreatorInfoByte_is_0

	lis r4, StateID_CreatorInfoWait__22dCourseSelectManager_c@h
	ori r4, r4, StateID_CreatorInfoWait__22dCourseSelectManager_c@l
	lwzu r12, 0x70(r3)
	lwz r12, 0x18(r12)
	mtctr r12
	bctrl
	b returnCheckForNewCSMBytes

CreatorInfoByte_is_0:
	b doneWithCheckForNewCSMBytes