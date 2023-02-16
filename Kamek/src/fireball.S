.text
.align 4

#include <asm_setup.S>

.extern onCreate__17daFireBall_Base_cFv
.extern beforeCreate__13dStageActor_cFv
.extern afterCreate__13dStageActor_cFi
.extern onDelete__16daGoldFireBall_cFv
.extern beforeDelete__13dStageActor_cFv
.extern afterDelete__13dStageActor_cFi
.extern onExecute__17daFireBall_Base_cFv
.extern beforeExecute__17daFireBall_Base_cFv
.extern afterExecute__13dStageActor_cFi
.extern onDraw__17daFireBall_Base_cFv
.extern beforeDraw__13dStageActor_cFv
.extern afterDraw__13dStageActor_cFi
.extern willBeDeleted__17daFireBall_Base_cFv
.extern moreHeapShit__7fBase_cFUiPv
.extern createHeap__7fBase_cFUiPv
.extern heapCreated__7fBase_cFv
.extern __dt__16daGoldFireBall_cFv
.extern GetExplanationString__13dStageActor_cFv
.extern specialDraw1__8dActor_cFv
.extern specialDraw2__8dActor_cFv
.extern _vf58__8dActor_cFv
.extern _vf5C__8dActor_cFv
.extern isOutOfView__13dStageActor_cFv
.extern kill__13dStageActor_cFv
.extern _vf68__13dStageActor_cFv
.extern getPlrNo__13dStageActor_cFv
.extern _vf70__13dStageActor_cFv
.extern _vf74__13dStageActor_cFv
.extern itemPickedUp__13dStageActor_cFv
.extern _vf7C__13dStageActor_cFv
.extern eatIn__17daFireBall_Base_cFv
.extern disableEatIn__17daFireBall_Base_cFv
.extern _vf88__13dStageActor_cFv
.extern spitOut__13dStageActor_cFPv
.extern _vf90__13dStageActor_cFP13dStageActor_c
.extern _vf94__13dStageActor_cFPv
.extern removeMyActivePhysics__13dStageActor_cFv
.extern addMyActivePhysics__13dStageActor_cFv
.extern returnRegularScale__13dStageActor_cFv
.extern _vfA4__13dStageActor_cFPv
.extern _vfA8__13dStageActor_cFPv
.extern _vfAC__13dStageActor_cFPv
.extern killedByLevelClear__13dStageActor_cFv
.extern _vfB4__13dStageActor_cFv
.extern _vfB8__13dStageActor_cFv
.extern _vfBC__13dStageActor_cFv
.extern _vfC0__13dStageActor_cFv
.extern _vfC4__13dStageActor_cFv
.extern _vfC8__13dStageActor_cFP7Point2df
.extern _vfCC__13dStageActor_cFP7Point2df
.extern _vfD0__13dStageActor_cFP7Point2df
.extern beginState_Gegneric__13dActorState_cFv
.extern endState_Gegneric__13dActorState_cFv
.extern executeState_Gegneric__13dActorState_cFv
.extern beginState_Move_orig__16daGoldFireBall_cFv
.extern executeState_Move_orig__16daGoldFireBall_cFv
.extern endState_Move_orig__16daGoldFireBall_cFv
.extern Kill_Begin__17daFireBall_Base_cFv
.extern Kill_Execute__17daFireBall_Base_cFv
.extern Kill_End__17daFireBall_Base_cFv
.extern eatIn_Begin__17daFireBall_Base_cFv
.extern eatIn_Execute__17daFireBall_Base_cFv
.extern eatIn_End__17daFireBall_Base_cFv
.extern eatNow_Begin__17daFireBall_Base_cFv
.extern eatNow_Execute__17daFireBall_Base_cFv
.extern eatNow_End__17daFireBall_Base_cFv
.extern calledInOnCreate_b__16daGoldFireBall_cFv
.extern updateSpawnLimits__16daGoldFireBall_cFv
.extern calledInOnCreate_c__16daGoldFireBall_cFv
.extern setUpCollSensor__17daFireBall_Base_cFv
.extern setZOrder_maybe__16daGoldFireBall_cFv
.extern playFireballEffect__17daFireBall_Base_cFv
.extern doFireSplash__16daGoldFireBall_cFv
.extern doLavaSplash__17daFireBall_Base_cFv
.extern doPoisonSplash__17daFireBall_Base_cFv
.extern getSizeOfLight__17daFireBall_Base_cFv
.extern vf138__16daGoldFireBall_cFv
.extern vf13C__16daGoldFireBall_cFv
.extern collisionCallback__16daGoldFireBall_cFP13ActivePhysicsP13ActivePhysics

#==========C++ replacements==========#

.extern onCreate__16daGoldFireBall_cFv
.extern onExecute__16daGoldFireBall_cFv

#==========constructor replace==========#

.extern AllocFromGameHeap1
.extern __ct__13dActorState_cFv
.extern __ct__16mHeapAllocator_cFv
.extern __ct__Q23EGG6EffectFv
.extern __vt__15StageActorLight
.extern __vt__17daFireBall_Base_c
.extern __vt__Q23mEf3es2

.global __ct__16daGoldFireBall_cFv
__ct__16daGoldFireBall_cFv:
	stwu r1, -0x20(r1)
	mflr r0
	li r3, 0x57C #Class size
	stw r0, 0x24(r1)
	stw r31, 0x1C(r1)
	stw r30, 0x18(r1)
	stw r29, 0x14(r1)

	bl AllocFromGameHeap1

	cmpwi r3, 0
	mr r30, r3
	beq returnct

	bl __ct__13dActorState_cFv

	lis r4, __vt__17daFireBall_Base_c@h
	addi r3, r30, 0x3D0
	addi r4, r4, __vt__17daFireBall_Base_c@l
	stw r4, 0x60(r30)
	bl __ct__16mHeapAllocator_cFv

	lis r3, __vt__15StageActorLight@h
	lfs f0, -0x5BA0(r2) #0x8042D7C0 = 0.0f
	addi r3, r3, __vt__15StageActorLight@l
	stw r3, 0x3EC(r30)
	addi r29, r30, 0x42C
	li r31, 0
	stfs f0, 0x3FC(r30)
	li r0, 1
	mr r3, r29

	stw r31, 0x400(r30)
	stw r31, 0x404(r30)
	stw r31, 0x408(r30)
	stw r31, 0x410(r30)
	stw r0, 0x414(r30)
	stw r31, 0x428(r30)
	bl __ct__Q23EGG6EffectFv

	lis r4, __vt__Q23mEf3es2@h
	lis r3, newgoldvtable@h
	addi r4, r4, __vt__Q23mEf3es2@l
	stw r4, 0(r29)
	ori r3, r3, newgoldvtable@l
	stw r31, 0x114(r29)
	stw r31, 0x118(r29)
	stb r31, 0x11C(r29)
	stb r31, 0x11D(r29)
	stw r31, 0x120(r29)
	stw r31, 0x124(r29)
	stw r3, 0x60(r30)
returnct:
	mr r3, r30
	lwz r31, 0x1C(r1)
	lwz r30, 0x18(r1)
	lwz r29, 0x14(r1)
	lwz r0, 0x24(r1)
	mtlr r0
	addi r1, r1, 0x20
	blr

#==========playFireballEffect replace==========#

.extern dword_80357158
.extern dword_803572A0
.extern EffectRelated

.global newPlayFireballEffect
newPlayFireballEffect:
	addi r3, r3, 0xAC
	stwu r1, -0x10(r1)
	mflr r0
	lis r4, Wm_mr_firegold_b@h
	li r5, 0
	stw r0, 0x14(r1)
	ori r4, r4, Wm_mr_firegold_b@l
	li r7, 0
	li r8, 0
	stw r31, 0xC(r1)
	mr r31, r3
	lis r3, dword_80357158@h
	mr r6, r31
	addi r3, r3, dword_80357158@l
	bl EffectRelated
	lis r3, dword_803572A0@h
	lis r4, Wm_mr_firegold_a@h
	mr r6, r31
	li r5, 0
	addi r3, r3, dword_803572A0@l
	ori r4, r4, Wm_mr_firegold_a@l
	li r7, 0
	li r8, 0
	bl EffectRelated
	lwz r0, 0x14(r1)
	lwz r31, 0xc(r1)
	mtlr r0
	addi r1, r1, 0x10
	blr


#==========calledInOnCreate_c replace==========#

.extern ActivePhysics_InitWithStruct
.extern ActivePhysics_AddToList

.global newCalledInOnCreate_c
newCalledInOnCreate_c:
	stwu r1, -0x10(r1)
	mflr r0
	lis r5, goldfireCollision@h
	stw r0, 0x14(r1)
	ori r5, r5, goldfireCollision@l
	stw r31, 0xC(r1)
	mr r31, r3
	mr r4, r31
	addi r3, r3, 0x148 #328
	bl ActivePhysics_InitWithStruct
	addi r3, r31, 0x148
	bl ActivePhysics_AddToList
	lwz r0, 0x14(r1)
	lwz r31, 0xC(r1)
	mtlr r0
	addi r1, r1, 16
	blr


#==========newSetUpCollSensor replace==========#

.extern init__14collisionMgr_cFP13dStageActor_cPC12sensorBase_sPC12sensorBase_sPC12sensorBase_s

.global newSetUpCollSensor
newSetUpCollSensor:
	stwu r1, -0x10(r1)
	mflr r0
	lis r7, firstColliderThing@h
	stw r0, 0x14(r1)
	ori r7, r7, firstColliderThing@l
	addi r5, r7, 0
	stw r31, 0xC(r1)
	mr r31, r3
	lis r6, secondColliderThing@h
	ori r6, r6, secondColliderThing@l
	lis r7, thirdColliderThing@h
	ori r7, r7, thirdColliderThing@l
	mr r4, r31
	addi r3, r3, 0x1EC #492
	bl init__14collisionMgr_cFP13dStageActor_cPC12sensorBase_sPC12sensorBase_sPC12sensorBase_s
	addi r0, r31, 0x2DC #daFireBall_Base_c._._.rCollDetector
	stw r0, 0x23C(r31) #daFireBall_Base_c._._.cSensor.ptrToClass2DC
	lwz r31, 0xC(r1)
	lwz r0, 0x14(r1)
	mtlr r0
	addi r1, r1, 16
	blr


#==========newPlayFireballHitEffect replace==========#

.extern EffectSpawnerThatTakesPlayerCountIntoAccount
.extern _doGoldTileChange
#.extern doGoldTileChange__16daGoldFireBall_cFv
.extern returnFromNewDirectWallHit
.extern Delete__7fBase_cFv
.extern continueFromDontKillYourself

.global newPlayFireballHitEffect
newPlayFireballHitEffect:
	stwu r1, -0x10(r1)
	mflr r0
	li r4, 0
	li r6, 0
	stw r0, 0x14(r1)
	li r7, 0
	stw r31, 0xC(r1)

	mr r31, r3
	lis r3, Wm_mr_fireball_hit@h
	ori r3, r3, Wm_mr_fireball_hit@l
	addi r5, r31, 0xAC
	bl EffectSpawnerThatTakesPlayerCountIntoAccount
	li r0, 1
	stw r0, 0x408(r31) #daFireBall_Base_c.didSplash

	lwz r0, 0x560(r31) #daGoldFireBall_c.isGold
	cmpwi r0, 1
	bne _dontDoGoldTileChange
#	mr r3, r31
#	bl doGoldTileChange__16daGoldFireBall_cFv
	li r3, 1
	stw r3, 0x564(r31)

_dontDoGoldTileChange:
	lwz r31, 0xC(r1)
	lwz r0, 0x14(r1)
	mtlr r0
	addi r1, r1, 16
	blr

.global newDirectWallHit
newDirectWallHit:
#	mr r3, r30
#	bl doGoldTileChange__16daGoldFireBall_cFv
	li r3, 1
	stw r3, 0x564(r30)
	li r3, 0
	b returnFromNewDirectWallHit

.global dontDieUntilITellYouTo
dontDieUntilITellYouTo:
	lwz r3, 0x560(r3) #daGoldFireBall_c.isGold
	cmpwi r3, 1
	beq dontKillYourself
	mr r3, r31
	bl Delete__7fBase_cFv
dontKillYourself:
	b continueFromDontKillYourself

.global returnByteAtPointer__FUiUi
returnByteAtPointer__FUiUi:
	lbzx r3, r4, r3
	blr


#==========Trigger death in case of bouncing on a brick==========#

.extern continueFromCheckBouncingBlock
.extern checkForTriggerableBlock__16daGoldFireBall_cFv
.extern goldFireHitAdjacent

.global checkBouncingBlock
checkBouncingBlock:
	lwz r3, 0x560(r30) #daGoldFireBall_c.isGold
	cmpwi r3, 1
	bne _notGoldFire

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
	mr r3, r30
	bl checkForTriggerableBlock__16daGoldFireBall_cFv
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

	cmpwi r3, 1
	li r3, 0
	beq _goldFireHitAdjacent
_notGoldFire:
	addi r3, r30, 0x1EC
	b continueFromCheckBouncingBlock
_goldFireHitAdjacent:
	li r4, 0
	li r5, 0
	b goldFireHitAdjacent


#==========New SFXs==========#

.extern continueFromReplaceGoldHitSFXEnemy
.extern ConvertStagePositionIntoScreenPosition__Maybe
.extern PlaySoundAtPosition__17SoundPlayingClassFiP7Point2dUi

.global replaceGoldHitSFX
replaceGoldHitSFX:
	lwz r4, 0x560(r30)
	cmpwi r4, 1
	beq _useGoldHitSFX
	li r4, 0x266 #SE_OBJ_FIREBALL_DISAPP
	blr
_useGoldHitSFX:
	li r4, 2000
	blr

.global replaceGoldHitSFXEnemy
replaceGoldHitSFXEnemy:
	lwz r4, 0x560(r30)
	cmpwi r4, 1
	bne _dontUseGoldHitSFXEnemy
	lwz r31, -0x5940(r13)
	addi r3, r1, 8
	addi r4, r30, 0xAC
	bl ConvertStagePositionIntoScreenPosition__Maybe
	mr r3, r31
	addi r5, r1, 8
	li r4, 2000
	li r6, 0
	bl PlaySoundAtPosition__17SoundPlayingClassFiP7Point2dUi
_dontUseGoldHitSFXEnemy:
	b continueFromReplaceGoldHitSFXEnemy


#==========New vtable==========#

.data
.align 4
.global newgoldvtable
newgoldvtable:
	.long 0
	.long 0
	.long onCreate__16daGoldFireBall_cFv #onCreate__17daFireBall_Base_cFv
	.long beforeCreate__13dStageActor_cFv
	.long afterCreate__13dStageActor_cFi
	.long onDelete__16daGoldFireBall_cFv
	.long beforeDelete__13dStageActor_cFv
	.long afterDelete__13dStageActor_cFi
	.long onExecute__16daGoldFireBall_cFv #onExecute__17daFireBall_Base_cFv
	.long beforeExecute__17daFireBall_Base_cFv
	.long afterExecute__13dStageActor_cFi
	.long onDraw__17daFireBall_Base_cFv
	.long beforeDraw__13dStageActor_cFv
	.long afterDraw__13dStageActor_cFi
	.long willBeDeleted__17daFireBall_Base_cFv
	.long moreHeapShit__7fBase_cFUiPv
	.long createHeap__7fBase_cFUiPv
	.long heapCreated__7fBase_cFv
	.long __dt__16daGoldFireBall_cFv
	.long GetExplanationString__13dStageActor_cFv
	.long specialDraw1__8dActor_cFv
	.long specialDraw2__8dActor_cFv
	.long _vf58__8dActor_cFv
	.long _vf5C__8dActor_cFv
	.long isOutOfView__13dStageActor_cFv
	.long kill__13dStageActor_cFv
	.long _vf68__13dStageActor_cFv
	.long getPlrNo__13dStageActor_cFv
	.long _vf70__13dStageActor_cFv
	.long _vf74__13dStageActor_cFv
	.long itemPickedUp__13dStageActor_cFv
	.long _vf7C__13dStageActor_cFv
	.long eatIn__17daFireBall_Base_cFv
	.long disableEatIn__17daFireBall_Base_cFv
	.long _vf88__13dStageActor_cFv
	.long spitOut__13dStageActor_cFPv
	.long _vf90__13dStageActor_cFP13dStageActor_c
	.long _vf94__13dStageActor_cFPv
	.long removeMyActivePhysics__13dStageActor_cFv
	.long addMyActivePhysics__13dStageActor_cFv
	.long returnRegularScale__13dStageActor_cFv
	.long _vfA4__13dStageActor_cFPv
	.long _vfA8__13dStageActor_cFPv
	.long _vfAC__13dStageActor_cFPv
	.long killedByLevelClear__13dStageActor_cFv
	.long _vfB4__13dStageActor_cFv
	.long _vfB8__13dStageActor_cFv
	.long _vfBC__13dStageActor_cFv
	.long _vfC0__13dStageActor_cFv
	.long _vfC4__13dStageActor_cFv
	.long _vfC8__13dStageActor_cFP7Point2df
	.long _vfCC__13dStageActor_cFP7Point2df
	.long _vfD0__13dStageActor_cFP7Point2df
	.long beginState_Gegneric__13dActorState_cFv
	.long endState_Gegneric__13dActorState_cFv
	.long executeState_Gegneric__13dActorState_cFv
	.long beginState_Move_orig__16daGoldFireBall_cFv
	.long executeState_Move_orig__16daGoldFireBall_cFv
	.long endState_Move_orig__16daGoldFireBall_cFv
	.long Kill_Begin__17daFireBall_Base_cFv
	.long Kill_Execute__17daFireBall_Base_cFv
	.long Kill_End__17daFireBall_Base_cFv
	.long eatIn_Begin__17daFireBall_Base_cFv
	.long eatIn_Execute__17daFireBall_Base_cFv
	.long eatIn_End__17daFireBall_Base_cFv
	.long eatNow_Begin__17daFireBall_Base_cFv
	.long eatNow_Execute__17daFireBall_Base_cFv
	.long eatNow_End__17daFireBall_Base_cFv
	.long calledInOnCreate_b__16daGoldFireBall_cFv
	.long updateSpawnLimits__16daGoldFireBall_cFv
	.long newCalledInOnCreate_c #calledInOnCreate_c__16daGoldFireBall_cFv
	.long newSetUpCollSensor #setUpCollSensor__17daFireBall_Base_cFv
	.long setZOrder_maybe__16daGoldFireBall_cFv
	.long newPlayFireballEffect #playFireballEffect__17daFireBall_Base_cFv
	.long doFireSplash__16daGoldFireBall_cFv
	.long doLavaSplash__17daFireBall_Base_cFv
	.long doPoisonSplash__17daFireBall_Base_cFv
	.long getSizeOfLight__17daFireBall_Base_cFv
	.long vf138__16daGoldFireBall_cFv
	.long vf13C__16daGoldFireBall_cFv
	.long 0, 0, 0, 0, 0

#==========Effects name==========#
.align 4

.global Wm_mr_firegold
Wm_mr_firegold:
	.string "Wm_mr_firegold"

.global Wm_mr_firegold_a
Wm_mr_firegold_a:
	.string "Wm_mr_firegold_a"

.global Wm_mr_firegold_b
Wm_mr_firegold_b:
	.string "Wm_ob_startail_kira"

.global Wm_mr_firegold_hit
Wm_mr_firegold_hit:
	.string "Wm_mr_firegold_hit"

.global Wm_mr_firegold_hit01
Wm_mr_firegold_hit01:
	.string "Wm_mr_firegold_hit01"

.global Wm_mr_fireball_hit
Wm_mr_fireball_hit:
	.string "Wm_mr_fireball_hit"

#==========New Collisions==========#
.align 4

.global goldfireCollision
goldfireCollision:
	.float 0.0                                    # x_dist_to_center
	.float 0.0                                    # y_dist_to_center
	.float 6.0                                    # x_dist_to_edge - Old -> 3.0
	.float 6.0                                    # y_dist_to_edge - Old -> 3.0
	.byte 6                                       # category1 - Old -> 6
	.byte 1                                       # category2 - Old -> 1
	.byte 0, 0
	.long 0xFA                                    # bitfield1 - Old -> 0xFA
	.long 0x8040                                  # bitfield2 - Old -> 0x8040
	.short 0                                      # unkShort1C
	.byte 0, 0
	.long collisionCallback__16daGoldFireBall_cFP13ActivePhysicsP13ActivePhysics  # callback
	.long 0

.align 4

.global firstColliderThing
firstColliderThing:
	.long 0
	.long 0
	.long -0x6000 #Old -> 0xFFFFD000 = -0x3000

.global secondColliderThing
secondColliderThing:
	.long 0
	.long 0
	.long 0x6000 #Old -> 0x3000

.global thirdColliderThing
thirdColliderThing:
	.long 0
	.long 0x6000 #Old -> 0x3000
	.long 0