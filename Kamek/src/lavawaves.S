#include <asm_setup.S>

.global ResetLavaWaves, LavaWavesBitCheck
.global AreLavaWavesEnabled1, AreLavaWavesEnabled2, AreLavaWavesEnabled3
.global AreLavaWavesEnabled4, AreLavaWavesEnabled5

.extern returnFromLavaCheck1True, returnFromLavaCheck1False
.extern returnFromLavaCheck2True, returnFromLavaCheck2False
.extern returnFromLavaCheck3
.extern returnFromLavaCheck4True, returnFromLavaCheck4False
.extern returnFromLavaCheck5True, returnFromLavaCheck5False

.text
.align 4

# This is needed because we need to reset the value between levels (and areas). It probably won't work between zones but i'm not 100% sure.
ResetLavaWaves:
	li r5, 0
	lis r30, LavaWavesEnabled@ha
	stb r5, LavaWavesEnabled@l(r30)
	lwz r30, 4(r3) # og instruction
	blr

# Checks bit 24 of the sprite settings (bit 40 in Reggie), but only if the liquid is lava.
LavaWavesBitCheck:
	lbz r5, 6(r3)
	andi. r5, r5, 1
	lis r28, LavaWavesEnabled@ha
	stb r5, LavaWavesEnabled@l(r28)
	
	lbz r5, 0x355(r3)				#get subType
	lis r28, LavaWavesSubType@ha
	stb r5, LavaWavesSubType@l(r28)

	li r28, 0xFF # og instruction
	blr

# Check 1
AreLavaWavesEnabled1:
	lis r3, LavaWavesEnabled@ha
	lbz r3, LavaWavesEnabled@l(r3)
	cmpwi r3, 1
	beq Back1
	b returnFromLavaCheck1False

Back1:
	b returnFromLavaCheck1True

# Check 2
AreLavaWavesEnabled2:
	lis r5, LavaWavesEnabled@ha
	lbz r0, LavaWavesEnabled@l(r5)
	cmpwi r0, 1
	beq Back2
	b returnFromLavaCheck2False

Back2:
	b returnFromLavaCheck2True

# Check 3
AreLavaWavesEnabled3:
	lis r5, LavaWavesEnabled@ha
	lbz r5, LavaWavesEnabled@l(r5)
	b returnFromLavaCheck3

# Check 4
AreLavaWavesEnabled4:
	lis r4, LavaWavesEnabled@ha
	lbz r4, LavaWavesEnabled@l(r4)
	cmpwi r4, 1
	bne Back4
	li r25, 3
	b returnFromLavaCheck4True

Back4:
	b returnFromLavaCheck4False

# Check 5
AreLavaWavesEnabled5:
	lis r4, LavaWavesEnabled@ha
	lbz r4, LavaWavesEnabled@l(r4)
	cmpwi r4, 1
	beq Back5
	b returnFromLavaCheck5False

Back5:
	b returnFromLavaCheck5True

.data
.align 4
LavaWavesEnabled: .byte 0
.global LavaWavesSubType
LavaWavesSubType: .byte 0