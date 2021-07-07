.text
.align 4

#include <asm_setup.S>

.extern daEnSnakeBlock_c__StateID_Move

.global AutoStartSnakeBlock
AutoStartSnakeBlock:
	lwz r3, 4(r4)
	rlwinm. r3, r3, 8, 28, 31 #(r3 >> 24) & 0xF
	beq _return

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
	cmpwi r9, 0
	beq _doUsual
	li r0, 1
	stw r0, 0x1AC(r3) #SnakeSubEx._.numberInChain
_doUsual:
	stfs f5, 0xC(r1) #Restore Instruction
	b continueFromSnakeBlockLoop3


.extern doSomethingElseWithTravelInfoBuffer__SnakeSubEx
.extern continueFromStoreIsLoopToR9_1
.extern continueFromStoreIsLoopToR9_2

.global StoreIsLoopToR9_1
StoreIsLoopToR9_1:
	lwz r9, 0x2B40(r29)
	bl doSomethingElseWithTravelInfoBuffer__SnakeSubEx
	b continueFromStoreIsLoopToR9_1

.global StoreIsLoopToR9_2
StoreIsLoopToR9_2:
	lwz r9, 0x2B40(r29)
	bl doSomethingElseWithTravelInfoBuffer__SnakeSubEx
	b continueFromStoreIsLoopToR9_2