.text
.align 4

#include <asm_setup.S>

.extern windColor
.extern returnFromWindColor

.global setWindColor
setWindColor:
	lis r3, windColor@h
	ori r3, r3, windColor@l
	lbz		r6, 0(r3)
	lbz		r0, 2(r3)
	lbz		r3, 1(r3)
	
	b		returnFromWindColor