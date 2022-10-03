.text

.set sp, 1

.align 4
.extern OSReport
.extern profileNames
.extern customProfileNames

#if defined(REGION_K) || defined(REGION_W) || defined(REGION_C)
	#define ORIGINAL_PROFILES 752
#else
	#define ORIGINAL_PROFILES 750
#endif

.global ActorLogHook
ActorLogHook:
	stwu sp, -0x20(sp)
	mflr r0
	stw r0, 0x24(sp)
	stw r3, 0x1C(sp)
	stw r4, 0x18(sp)
	stw r5, 0x14(sp)
	stw r6, 0x10(sp)

	cmpwi r3, ORIGINAL_PROFILES
	blt useProfileNames
	
	subi r3, r3, ORIGINAL_PROFILES
	lis r8, customProfileNames@h
	ori r8, r8, customProfileNames@l
	b gotRightProfileNamesList

useProfileNames:
	lis r8, profileNames@h
	ori r8, r8, profileNames@l

gotRightProfileNamesList:
	slwi r7, r3, 2
	lwzx r7, r8, r7

	lis r3, M@h
	ori r3, r3, M@l
	crclr 4*cr1+eq
	bl OSReport

	lwz r6, 0x10(sp)
	lwz r5, 0x14(sp)
	lwz r4, 0x18(sp)
	lwz r3, 0x1C(sp)
	lwz r0, 0x24(sp)
	mtlr r0
	addi sp, sp, 0x20

	# Return to __objCreate
	clrlslwi r31, r3, 16, 2
	blr

.data
M: .string "P:%p S:%08x T:%d N:%s\n"
