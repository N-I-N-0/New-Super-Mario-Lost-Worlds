.text
.align 4

#include <asm_setup.S>
.extern originalHipAttackStage4Continue

.global ewhipAttackStage4__Fv
ewhipAttackStage4__Fv:
	.byte 0x94, 0x21, 0xff, 0xf0			#restores an instruction I don't know how to translate from Ghidra's representation
	b 	originalHipAttackStage4Continue

