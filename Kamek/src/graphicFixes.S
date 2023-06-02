.text

.set sp, 1

.align 4
.extern returnFromPatchWidth #801BD590
.global patchWidth
patchWidth:
	subfic r6, r0, 720
	li r0, 0x2
	divwu r0, r6, r0
	b returnFromPatchWidth

.extern returnFromfix480p #801c01b8
.global fix480p
fix480p:
	li r5, 0x3
	stb r5, 29(r1)
	b returnFromfix480p