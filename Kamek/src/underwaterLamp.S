.text
#include <asm_setup.S>

.global ChangeLampPower
ChangeLampPower:
	lis r6, float266@ha
	lfs f0, float266@l(r6)

	lis r7, multiFloats@h
	ori r7, r7, multiFloats@l

	lwz r6, 4(r3)
	rlwinm r6, r6, 0, 28, 31 # Equivalent of (r6 & 0xF)
	mulli r6, r6, 4
	lfsx f4, r7, r6

	fmuls f0, f0, f4
	blr
	

.data
.align 4

.global float266
float266: .float 266.0

.global multiFloats
multiFloats: .float 1.0, 0.25, 0.5, 0.75, 1.25, 1.5, 1.75, 2.0, 2.5, 3.0, 3.5, 4.0, 5.0, 6.0, 7.0, 8.0

.align 4