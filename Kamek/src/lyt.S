.text
.align 4
#include <asm_setup.S>

.global StoreStringLength
StoreStringLength:
	fsubs f28, f1, f0 # Restore Instruction

	stfs f28, 0x104(r29)
	blr