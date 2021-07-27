#include <asm_setup.S>

.extern flipAllPanels__Fv
.extern continueAt8005e724
.extern continueAt80147214

.text
.align 4

.global playerJumped
playerJumped:
	stwu sp, -0x40(sp)
	stw r0, 0x44(sp)
	mflr r0
	stw r0, 0x3C(sp)
	stw r3, 0x38(sp)
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

	bl flipAllPanels__Fv
	
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
	lwz r3, 0x38(sp)
	lwz r0, 0x3C(sp)
	mtlr	r0
	lwz r0, 0x44(sp)
	addi	sp, sp, 0x40
	b       continueAt8005e724

.global playerSpinned
playerSpinned:
	stwu sp, -0x40(sp)
	stw r0, 0x44(sp)
	mflr r0
	stw r0, 0x3C(sp)
	stw r3, 0x38(sp)
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

	bl flipAllPanels__Fv
	
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
	lwz r3, 0x38(sp)
	lwz r0, 0x3C(sp)
	mtlr	r0
	lwz r0, 0x44(sp)
	addi	sp, sp, 0x40
	mr		r3, r31
	b       continueAt80147214