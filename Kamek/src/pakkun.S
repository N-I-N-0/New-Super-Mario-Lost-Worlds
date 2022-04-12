#include <asm_setup.S>

.text
.align 4
.extern GetTexFilenameForR5 #spriteTex

.global TEX_Pakkun
TEX_Pakkun:
	lbz r5, 4(r29)
	b GetTexFilenameForR5

.global TEX_PakkunPipe
TEX_PakkunPipe:
.global TEX_PakkunPipeFire
TEX_PakkunPipeFire:
	lbz r5, 4(r3)
	b GetTexFilenameForR5

.extern dEn_c__fireballInvalid #dEn_c::fireballInvalid(dCc_c*,dCc_c*)(param_1,param_2,param_3);
.extern continuePakkunNormal
.extern continuePakkunBony
.global Pakkun_Fireball
Pakkun_Fireball:
	lbz		r28, 4(r3)		#get spriteTex
	cmpwi	r28, 1			#check if bony
	mr		r28, r3			#restore instruction
	beq		isBonyPakkun
	b		continuePakkunNormal
isBonyPakkun:
	bl		dEn_c__fireballInvalid
	b		continuePakkunBony


.extern continuePakkunNormal2
.extern continuePakkunBony2
.global Pakkun_Fireball2
Pakkun_Fireball2:
	lbz		r28, 4(r3)		#get spriteTex
	cmpwi	r28, 1			#check if bony
	mr		r28, r3			#restore instruction
	beq		isBonyPakkun2
	b		continuePakkunNormal2
isBonyPakkun2:
	bl		dEn_c__fireballInvalid
	b		continuePakkunBony2