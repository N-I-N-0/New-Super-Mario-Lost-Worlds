.text
.align 4

#include <asm_setup.S>

.extern daEnSnakeBlock_c__StateID_Move
.extern instance__10dFlagMgr_c

.global AutoStartSnakeBlock
AutoStartSnakeBlock:
	# Pop stack
	stwu r1, -0x10(r1)
	mflr r0
	stw r0, 0x14(r1)
	stw r31, 0xC(r1)
	stw r30, 0x8(r1)
	
	lwz r3, 4(r4)
	rlwinm. r3, r3, 8, 31, 31 #(r3 >> 24) & 0x1
	beq _checkForTriggeringEventID

	b _doChangeStateToMove

_checkForTriggeringEventID:
	lhz r0, 0x354(r4) #spriteFlagNum
	cmpwi r0, 0
	beq _return

	mr r30, r4

	lis r4, instance__10dFlagMgr_c@h
	ori r4, r4, instance__10dFlagMgr_c@l
	lwz r4, 0(r4)
	lwz r5, 0(r4)
	lwz r6, 4(r4)
	lwz r0, 0x358(r30)
	lwz r4, 0x35C(r30)
	and r6, r6, r4
	and r5, r5, r0
	li r0, 0
	li r4, 0
	xor r0, r6, r0
	xor r31, r5, r4
	or r31, r0, r31
	subic r0, r31, 1
	subfe r31, r0, r31

	cmpwi r31, 0
	beq _return

	mr r4, r30

_doChangeStateToMove:
	lwz r12, 0x60(r4)
	mr r3, r4
	lis r4, daEnSnakeBlock_c__StateID_Move@h
	ori r4, r4, daEnSnakeBlock_c__StateID_Move@l
	lwz r12, 0xD4(r12)
	mtctr r12
	bctrl

_return:
	lwz r0, 0x14(r1)
	lwz r31, 0xC(r1)
	lwz r30, 0x8(r1)
	mtlr r0
	addi r1, r1, 16

	blr


.extern dRail__getRailByID_static
.extern abs
.extern firstLoop
.extern continueFromFirstLoop

.global SnakeBlockLoop1
SnakeBlockLoop1:
	blt _firstLoop

	lhz r5, 6(r28) #lvlRail.unk
	andi. r5, r5, 2
	stw r5, 0x2B40(r30)
	beq _notLooped

	mulli r5, r29, 0x10
	neg r5, r5
	lha       r3, 0(r24)
	#lha       r0, 0x10(r24)
	lhax r0, r5, r24
	srawi     r3, r3, 4
	srawi     r0, r0, 4
	subf      r3, r3, r0
	bl        abs
	lha       r4, 2(r24)
	extsh     r21, r3
	#lha       r0, 0x12(r24)
	mulli r5, r29, 0x10
	addi r5, r5, -2
	neg r5, r5
	lhax r0, r5, r24
	srawi     r3, r4, 4
	srawi     r0, r0, 4
	subf      r3, r3, r0
	bl        abs
	extsh     r3, r3
	add       r0, r25, r21
	add       r25, r3, r0
	addi      r24, r24, 0x10
	addi      r26, r26, 1

_notLooped:
	b continueFromFirstLoop

_firstLoop:
	b firstLoop


.extern loopForNodes
.extern finishNodeLoop
.extern continueFromNodeLoop
.extern cancelNodeLoop

.global SnakeBlockLoop2
SnakeBlockLoop2:
	blt _loopForNodes

	lhz r7, 6(r28) #lvlRail.unk
	andi. r7, r7, 2
	beq _notNodeLooped

	lhz r7, 4(r28) #lvlRail.nodeCount
	cmpw r31, r7
	bge _finishNodeLoop
	addi r7, r7, -1

	mulli r6, r7, 0x10
	neg r6, r6

	lha       r3, 0(r27)
	#lha       r0, 0x10(r27)
	lhax r0, r6, r27
	srawi     r5, r3, 4
	lha       r3, 2(r27)
	srawi     r4, r0, 4
	#lha       r0, 0x12(r27)
	mulli r6, r7, 0x10
	addi r6, r6, -2
	neg r6, r6
	lhax r0, r6, r27
	srawi     r3, r3, 4
	subf      r4, r5, r4
	srawi     r0, r0, 4
	subf      r0, r3, r0
	extsh     r23, r4
	mr        r3, r23
	extsh     r22, r0
	bl        abs
	extsh     r26, r3
	mr        r3, r22
	bl        abs
	cmpwi     r23, 0
	extsh     r6, r3
	li        r7, 0
	li        r8, 0

	b continueFromNodeLoop
_loopForNodes:
	b loopForNodes
_finishNodeLoop:
	b finishNodeLoop
_notNodeLooped:
	b cancelNodeLoop


.extern continueFromSnakeBlockLoop3

.global SnakeBlockLoop3
SnakeBlockLoop3:
	cmpwi r0, 0
	bne _doUsual
	cmpwi r9, 0 #Is looped
	beq _checkForEndState
	li r0, 1
	stw r0, 0x1AC(r3) #SnakeSubEx._.numberInChain
	b _doUsual

_checkForEndState:
	cmpwi r15, 4 #End State
	bne _doUsual
	cmpwi r17, 1 #Are we the head
	bne _doUsual


	lwz r0, 0x1AC(r3) #SnakeSubEx._.numberInChain
	cmpwi r14, 0
	bne _subTwo
	lwz r17, 0x2B20(r29) #daEnSnakeBlock_c.totalTravelDistance
	mr r16, r17
	addi r16, r16, 3
	lwz r17, 0x2B18 (r29) #daEnSnakeBlock_c.length
	add r0, r16, r17
	b _storeNumberInChain
	_subTwo:
	li r0, 1
	_storeNumberInChain:
	stw  r0, 0x1AC(r3) #SnakeSubEx._.numberInChain


	lwz r0, 0x8A8(r29) #daEnSnakeBlock_c.subex2.numberInChain
	cmpwi r14, 0
	bne _subTwo2
	lwz r17, 0x2B20(r29) #daEnSnakeBlock_c.totalTravelDistance
	mr r16, r17
	addi r0, r16, 3
	b _storeNumberInChain2
	_subTwo2:
	lwz r16, 0x2B18 (r29) #daEnSnakeBlock_c.length
	addi r0, r16, 1
	_storeNumberInChain2:
	stw  r0, 0x8A8(r29) #daEnSnakeBlock_c.subex2.numberInChain

	li r0, 1
	cmpwi r14, 0
	beq _setR14ToOne
	li r14, 0
	li r0, 1
	b _doUsual
	_setR14ToOne:
	li r14, 1
	li r0, 1

_doUsual:
	stfs f5, 0xC(r1) #Restore Instruction
	b continueFromSnakeBlockLoop3


.extern continueFromSnakeBlockBackwards

.global SnakeBlockBackwards
SnakeBlockBackwards:
	cmpwi r14, 0
	beq _doAddi
	addi r0, r8, -1
	b continueFromSnakeBlockBackwards
_doAddi:
	addi r0, r8, 1
	b continueFromSnakeBlockBackwards


.extern doSomethingElseWithTravelInfoBuffer__10SnakeSubExFv
.extern continueFromStoreIsLoopToR9_1
.extern continueFromStoreIsLoopToR9_2
.extern skipFromRemoveSFX

.global StoreIsLoopToR9_1
StoreIsLoopToR9_1:
	lwz r9, 0x2B40(r29) #Is Looped
	lwz r14, 0x2B44(r29) #Going Backwards
	lwz r15, 4(r29) #settings
	rlwinm r15, r15, 4, 28, 31 #(r15 >> 28) & 0xF -> Nybble 5 (Ending State)
	bl doSomethingElseWithTravelInfoBuffer__10SnakeSubExFv
	stw r14, 0x2B44(r29) #Going Backwards
	b continueFromStoreIsLoopToR9_1

.global StoreIsLoopToR9_2
StoreIsLoopToR9_2:
	lwz r9, 0x2B40(r29) #Is Looped
	lwz r14, 0x2B44(r29) #Going Backwards
	lwz r15, 4(r29) #settings
	rlwinm r15, r15, 4, 28, 31 #(r15 >> 28) & 0xF -> Nybble 5 (Ending State)
	li r17, 1
	bl doSomethingElseWithTravelInfoBuffer__10SnakeSubExFv

	lwz r4, 4(r29)
	rlwinm. r4, r4, 8, 30, 30 #(r3 >> 24) & 0x2
	beq _continueFromRemoveSFX

	b skipFromRemoveSFX

_continueFromRemoveSFX:
	b continueFromStoreIsLoopToR9_2


.extern continueFromOtherDirectionsSpawn

.global otherDirectionsSpawn
otherDirectionsSpawn:
	lwz r3, 0x2B1C(r30) #daEnSnakeBlock_c.travelInfoBuffer
	lbz r3, 1(r3)

	cmpwi r3, 1 #Up
	beq _doUp
	cmpwi r3, 2 #Down
	beq _doDown
	cmpwi r3, 3 #Left
	beq _doLeft
	cmpwi r3, 4 #Right
	beq _doRight

	b continueFromOtherDirectionsSpawn

_doRight:
	fsubs f29, f29, f31
	b continueFromOtherDirectionsSpawn
_doLeft:
	fadds f29, f29, f31
	b continueFromOtherDirectionsSpawn
_doUp:
	fsubs f28, f28, f31
	b continueFromOtherDirectionsSpawn
_doDown:
	fadds f28, f28, f31
	b continueFromOtherDirectionsSpawn


.extern continueFromAdjustSpawnPosition

.global adjustSpawnPosition
adjustSpawnPosition:
	lfs f2, 0(r5)

	lwz r7, 0x2B1C(r4) #daEnSnakeBlock_c.travelInfoBuffer
	lbz r7, 1(r7)

	cmpwi r7, 1 #Up
	beq _spawnUp
	cmpwi r7, 2 #Down
	beq _spawnDown
	cmpwi r7, 3 #Left
	beq _spawnLeft
	cmpwi r7, 4 #Right
	beq _spawnRight

	b _returnFromAdjustSpawnPosition

_spawnUp:
_spawnDown:
	lwz r6, 0x2B18(r4) #daEnSnakeBlock_c.length

	xoris r6, r6, 0x8000
	lis r10, floatConverter@h
	ori r10, r10, floatConverter@l
	stw r6, 4(r10)
	lfd f4, 0(r10)

	lis r10, doubleConverter@h
	ori r10, r10, doubleConverter@l
	lfd f3, 0(r10)

	fsub f4, f4, f3

	lis r10, doubleSixteen@h
	ori r10, r10, doubleSixteen@l
	lfd f3, 0(r10)

	fmuls f4, f4, f3

	fsubs f2, f2, f4

	cmpwi r7, 1
	beq _padUp
	fsubs f1, f1, f4
	li r10, 0
	b _returnFromAdjustSpawnPosition
_padUp:
	fadds f1, f1, f4
	li r10, 0
	b _returnFromAdjustSpawnPosition

_spawnLeft:
	lwz r6, 0x2B18(r4) #daEnSnakeBlock_c.length

	xoris r6, r6, 0x8000
	lis r10, floatConverter@h
	ori r10, r10, floatConverter@l
	stw r6, 4(r10)
	lfd f4, 0(r10)

	lis r10, doubleConverter@h
	ori r10, r10, doubleConverter@l
	lfd f3, 0(r10)

	fsub f4, f4, f3

	lis r10, doubleThirtyTwo@h
	ori r10, r10, doubleThirtyTwo@l
	lfd f3, 0(r10)

	fmuls f4, f4, f3

	fsubs f2, f2, f4

	li r10, 0
	b _returnFromAdjustSpawnPosition
_spawnRight:
_returnFromAdjustSpawnPosition:
	cmpwi r0, 0 #Restore previous shit
	b continueFromAdjustSpawnPosition


.extern returnFromDoNotMoveIfEvent
.extern continueFromDoNotMoveIfEvent

.global doNotMoveIfEvent
doNotMoveIfEvent:
	beq _returnFromDoNotMoveIfEvent
	lhz r0, 0x354(r31) #spriteFlagNum
	cmpwi r0, 0
	beq _continueFromDoNotMoveIfEvent

_returnFromDoNotMoveIfEvent:
	b returnFromDoNotMoveIfEvent

_continueFromDoNotMoveIfEvent:
	b continueFromDoNotMoveIfEvent


.extern continueFromMakeTIBTwiceAsBig

.global MakeTIBTwiceAsBig
MakeTIBTwiceAsBig:
	addi r3, r25, 2
	slwi r3, r3, 1 #r3 *= 2

	b continueFromMakeTIBTwiceAsBig


.extern continueFromCreateBackwardsData

.global CreateBackwardsData #r3 is travelInfoBuffer; r25 is totalTravelDistance
CreateBackwardsData:
	stb r4, 1(r3) #Restore instruction

	mr r3, r0

	stwu sp, -0x100(sp)
	mflr r0
	stw r0, 0x104(sp)
	stw r31, 0xFC(sp)
	stw r30, 0xF8(sp)
	stw r29, 0xF4(sp)
	stw r28, 0xF0(sp)
	stw r27, 0xEC(sp)
	stw r26, 0xE8(sp)
	stw r25, 0xE4(sp)
	stw r24, 0xE0(sp)
	stw r23, 0xDC(sp)
	stw r22, 0xD8(sp)
	stw r21, 0xD4(sp)
	stw r20, 0xD0(sp)
	stw r19, 0xCC(sp)
	stw r18, 0xC8(sp)
	stw r17, 0xC4(sp)
	stw r16, 0xC0(sp)
	stw r15, 0xBC(sp)
	stw r14, 0xB8(sp)

	li r0, 0 #r0 will be our index to the beginning
	mr r4, r25
	addi r4, r4, 2
	slwi r4, r4, 1 #r4 will be out index to the end
	addi r4, r4, -1

	addi r6, r25, 1

_startLoop:
	lbzx r16, r3, r0 #Load r0(travelInfoBuffer)
	cmpw r0, r6
	bgt _endLoop

_doLoop:
	cmpwi r16, 1
	beq _setToTwo
	cmpwi r16, 2
	beq _setToOne
	cmpwi r16, 3
	beq _setToFour
	cmpwi r16, 4
	beq _setToThree

	b _storeNewInfo

	_setToOne:
	li r16, 1
	b _storeNewInfo
	_setToTwo:
	li r16, 2
	b _storeNewInfo
	_setToThree:
	li r16, 3
	b _storeNewInfo
	_setToFour:
	li r16, 4

	_storeNewInfo:
	stbx r16, r3, r4
	mr r16, r0
	addi r0, r16, 1 #That's to avoid it addi r0, r0 to compile as li r0
	addi r4, r4, -1
	b _startLoop

_endLoop:
	#b continueFromCreateBackwardsData
	lwz r0, 0x104(sp)
	lwz r31, 0xFC(sp)
	lwz r30, 0xF8(sp)
	lwz r29, 0xF4(sp)
	lwz r28, 0xF0(sp)
	lwz r27, 0xEC(sp)
	lwz r26, 0xE8(sp)
	lwz r25, 0xE4(sp)
	lwz r24, 0xE0(sp)
	lwz r23, 0xDC(sp)
	lwz r22, 0xD8(sp)
	lwz r21, 0xD4(sp)
	lwz r20, 0xD0(sp)
	lwz r19, 0xCC(sp)
	lwz r18, 0xC8(sp)
	lwz r17, 0xC4(sp)
	lwz r16, 0xC0(sp)
	lwz r15, 0xBC(sp)
	lwz r14, 0xB8(sp)
	mtlr r0
	addi sp, sp, 0x100
	blr



.data
.global floatConverter
floatConverter:
	.long 0x43300000
	.long 0

.global doubleConverter
doubleConverter:
	.long 0x43300000
	.long 0x80000000

.global doubleThirtyTwo
doubleThirtyTwo:
	.double 32.0

.global doubleSixteen
doubleSixteen:
	.double 16.0

.global doubleTwo
doubleTwo:
	.double 2.0