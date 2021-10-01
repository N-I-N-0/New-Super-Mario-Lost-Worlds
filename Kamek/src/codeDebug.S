#include <asm_setup.S>

.extern actuallyPrintLine__Fi

.text
.global CDPrintCurrentAddress
CDPrintCurrentAddress:
	mflr	r3
	bl		actuallyPrintLine__Fi
	mtlr	r3
	blr