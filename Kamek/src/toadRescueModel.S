.text
.align 4

#include <asm_setup.S>

.extern aKinopio
.extern aK_rcha
.extern aKb_model
.extern aSkb_model
.extern aPlkb_model
.extern aPkb_model
.extern aKh_model
.extern aSkh_model
.extern aPlkh_model
.extern aPkh_model
.extern ToadModelInfo

.global changeToadModelInfo
changeToadModelInfo:
	lbz r0, 0x151(r28)		#playerID, 4 = Rescue Toad
	cmpwi r0, 4
	beq isRescueToad
	lis r4, ToadModelInfo@h
	ori r4, r4, ToadModelInfo@l
	b rescueToadReturn

isRescueToad:
	lis r4, RescueToadModelInfo@h
	ori r4, r4, RescueToadModelInfo@l

rescueToadReturn:
	stw r4, 0x778(r28)
	li r0, 0xF
	blr




.data
.align 4

.global RescueToadModelInfo
RescueToadModelInfo:
	.long aRescueKinopio
	.long aRescueK_rcha
	.long aKb_model
	.long aSkb_model
	.long aPlkb_model
	.long aPkb_model
	.long aKh_model
	.long aSkh_model
	.long aPlkh_model
	.long aPkh_model
	.float 18.0
	.float 13.0
	.float 18.0
	.float 18.0

aRescueKinopio:
	.string "Kinopio2"

aRescueK_rcha:
	.string "K_rcha2"