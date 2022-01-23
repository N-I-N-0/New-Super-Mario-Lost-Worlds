.text
.align 4

#include <asm_setup.S>

.extern setPlayerStateBlueShell__FP7dAcPy_c
.extern returnFromShellTest

.global shellTest
shellTest:
	stwu sp, -0x40(sp)
	stw r0, 0x44(sp)
	mflr r0
	stw r0, 0x3C(sp)
	stw r3, 0x38(sp)
	stw r4, 0x34(sp)
	stw r5, 0x30(sp)
	stw r6, 0x2C(sp)
	stw r7, 0x28(sp)
	stw r8, 0x24(sp)
	stw r9, 0x20(sp)
	stw r10, 0x1C(sp)
	stw r11, 0x18(sp)
	stw r12, 0x14(sp)
	stw r31, 0x10(sp)

	mr		r3, r31
	bl		setPlayerStateBlueShell__FP7dAcPy_c

	lwz r31, 0x10(sp)
	lwz r12, 0x14(sp)
	lwz r11, 0x18(sp)
	lwz r10, 0x1C(sp)
	lwz r9, 0x20(sp)
	lwz r8, 0x24(sp)
	lwz r7, 0x28(sp)
	lwz r6, 0x2C(sp)
	lwz r5, 0x30(sp)
	lwz r4, 0x34(sp)
	lwz r3, 0x38(sp)
	lwz r0, 0x3C(sp)
	mtlr	r0
	lwz r0, 0x44(sp)
	addi	sp, sp, 0x40

	lwz		r0,0x10d4(r31)		#restore instruction
	b		returnFromShellTest