.text
#include <asm_setup.S>

.align 4
.extern isCSMgrStockItem__Fv
.extern returnFromSelectCorrectPlayerAnimation1
.extern returnFromSelectCorrectPlayerAnimation2
.extern returnFromSelectCorrectPlayerAnimation3

.global selectCorrectPlayerAnimation1
selectCorrectPlayerAnimation1:
	mr r4, r0
	stwu sp, -0x80 (sp) #Push stack, make space for 29 registers
	stmw r3, 0x8 (sp)

	bl isCSMgrStockItem__Fv
	cmpwi r3, 0

	lmw r3, 0x8 (sp)
	addi sp, sp, 0x80 #Pop stack
	mr r0, r4

	beq doForAddDropPlayers1

doForStockItem1:
	li r4, 0
	b returnFromSelectCorrectPlayerAnimation1

doForAddDropPlayers1:
	li r4, 169
	b returnFromSelectCorrectPlayerAnimation1


.global selectCorrectPlayerAnimation2
selectCorrectPlayerAnimation2:
	mflr r4
	stwu sp, -0x80 (sp) #Push stack, make space for 29 registers
	stmw r3, 0x8 (sp)

	bl isCSMgrStockItem__Fv
	cmpwi r3, 0

	lmw r3, 0x8 (sp)
	addi sp, sp, 0x80 #Pop stack
	mtlr r4

	beq doForAddDropPlayers2

doForStockItem2:
	li r4, 0
	b returnFromSelectCorrectPlayerAnimation2

doForAddDropPlayers2:
	li r4, 169
	b returnFromSelectCorrectPlayerAnimation2


.global selectCorrectPlayerAnimation3
selectCorrectPlayerAnimation3:
	mflr r4
	stwu sp, -0x80 (sp) #Push stack, make space for 29 registers
	stmw r3, 0x8 (sp)

	bl isCSMgrStockItem__Fv
	cmpwi r3, 0

	lmw r3, 0x8 (sp)
	addi sp, sp, 0x80 #Pop stack
	mtlr r4

	li r0, 0
	beq doForAddDropPlayers3

doForStockItem3:
	li r4, 0
	b returnFromSelectCorrectPlayerAnimation3

doForAddDropPlayers3:
	li r4, 169
	b returnFromSelectCorrectPlayerAnimation3
