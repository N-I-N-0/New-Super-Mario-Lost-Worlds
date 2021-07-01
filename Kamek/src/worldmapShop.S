.text
.align 4

#include <asm_setup.S>

.extern returnForCalledFromExecute
.extern continueInCalledFromExecute
.extern FUN_808fbd00

.global checkIfCommandOpenShop
checkIfCommandOpenShop:
	beq     return
	cmpwi   r4, 0xA9					#new command 169
	beq     spawnThisShop
	b       continueInCalledFromExecute

return:
	b       returnForCalledFromExecute

spawnThisShop:
	bl      FUN_808fbd00
	b       continueInCalledFromExecute


.extern isFrozen
.global setFrozen
setFrozen:
	stwu    sp, -0x10(sp)
	#mflr    r0
	stw     r3, 0xC(sp)
	
	li      r0, 0x1
	lis     r3, isFrozen@h
	stb     r0, isFrozen@l(r3)
	
	lwz     r3, 0xC(sp)
	#mtlr    r0
	addi    sp, sp, 0x10
	
	stb     r0, 0x1a8(r3)
    stw     r0, 0x1ac(r3)

	blr

.extern PtrToWM_CS_SEQ_MNG
.global unsetFrozen
unsetFrozen:
	stwu    sp, -0x10(sp)
	#mflr    r0
	stw     r3, 0xC(sp)

	li      r0, 0x0
	lis     r3, isFrozen@h
	stb     r0, isFrozen@l(r3)
	
	lwz     r3, 0xC(sp)
	#mtlr    r0
	addi    sp, sp, 0x10
	
	stb     r0, 0x1a8(r3)
    lis     r3, PtrToWM_CS_SEQ_MNG@h
    lwz     r3, PtrToWM_CS_SEQ_MNG@l(r3)
    stb     r0, 0x394(r3)

	
	blr