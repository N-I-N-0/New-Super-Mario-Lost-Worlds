.text
#include <asm_setup.S>
.align 4

.global BooCircleZAdjustment
BooCircleZAdjustment:
	mr		r6, r4 # Restore instruction

	lwz		r4, 4(r3) # Obtain this->settings
	rlwinm	r4, r4, 4, 31, 31 # Equivalent of ((r4 >> 28) & 1)
	cmpwi	r4, 1
	bne		dontSetBooCircleZ

	lis		r4, float300@ha
	lfs		f0, float300@l(r4)
	lfs		f1, 8(r6)
	fadds	f1, f1, f0
	stfs	f1, 8(r6)

dontSetBooCircleZ:
	blr


.global BooZAdjustment
BooZAdjustment:
	lfs		f0, 0xB4(r30) # Restore instruction

	lwz		r6, 4(r30) # Obtain this->settings
	rlwinm	r6, r6, 24, 31, 31 # Equivalent of ((r6 >> 8) & 1)
	cmpwi	r6, 1
	bne		dontSetBooZ

	lis		r6, float300@ha
	lfs		f3, float300@l(r6)
	fadds	f0, f0, f3
	stfs	f0, 0xB4(r30)

dontSetBooZ:
	blr






.data
.align 4

float300: .float 300.0