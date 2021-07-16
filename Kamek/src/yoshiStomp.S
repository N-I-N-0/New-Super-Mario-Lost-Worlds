.text
.align 4

#include <asm_setup.S>
.extern originalHipAttackStage4Continue

.global ewhipAttackStage4__Fv
ewhipAttackStage4__Fv:
	stwu sp, -0x10(sp)
	b 	originalHipAttackStage4Continue

