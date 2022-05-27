.text
.align 4

#include <asm_setup.S>

.extern checkExplosion__12daEnTeresa_cFv
.extern returnFromBooExplosionCheck

.global insertBooExplosionCheck
insertBooExplosionCheck:
	bl		checkExplosion__12daEnTeresa_cFv
	stwu	sp,-0x30(sp)
	b       returnFromBooExplosionCheck