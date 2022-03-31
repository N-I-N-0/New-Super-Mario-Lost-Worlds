#include <asm_setup.S>

.extern LoadBreffKoopa
.extern LoadBreffKoopaJr
.extern returnFromBreffLoadPatch

.global BreffLoadPatch

.text
.align 4

BreffLoadPatch:
	# Original instruction
	lhz r0, 0(r27)# get sprite id
	
	cmpwi r0, 449 # Bowser Fireballs will load Bowser's effect files
	beq Koopa
	cmpwi r0, 563 # Thunder Cloud will load thunder effects files
	beq KoopaJr
	
	# Otherwise keep doing the game's original checks
	b returnFromBreffLoadPatch

Koopa:
	b LoadBreffKoopa

KoopaJr:
	b LoadBreffKoopaJr
