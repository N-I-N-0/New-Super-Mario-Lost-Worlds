.text
#include <asm_setup.S>

.align 4
.global useNewBrres #80b48f34
useNewBrres:
	lis r5, bowsetteBrres@h
	ori r5, r5, bowsetteBrres@l
	blr

.global useNewArcAndMdl #80b48f3c and 80b48f50
useNewArcAndMdl:
	lis r4, bowsette@h
	ori r4, r4, bowsette@l
	blr

.data
.align 4
.global bowsetteBrres
bowsetteBrres:
	.string	"g3d/bowsette.brres"
	.byte	0
	.byte	0

.global bowsette
bowsette:
	.string	"bowsette"
	.byte	0
	.byte	0