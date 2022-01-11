#include <asm_setup.S>

.extern returnFromCallBackWEdit
.extern returnFromCallBackWEdit2
.extern FUN_80020ed0

.text
.align 4

.global callBackWEdit
callBackWEdit:
	lhz		r3, 8(r29)
	cmpwi	r3, 749
	or		r3, r29, r29							#restore instruction
	bgt		noStrangeFunctionCallPlease
	
strangeFunctionCall:
	bl		FUN_80020ed0
	b returnFromCallBackWEdit

noStrangeFunctionCallPlease:
	b returnFromCallBackWEdit2