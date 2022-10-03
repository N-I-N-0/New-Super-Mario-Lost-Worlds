.text
.extern GetTexFilenameForR5
.global TEX_Goomba
TEX_Goomba:
	lwz r5, 4(r3)
	srwi r5, r5, 24
	andi. r5, r5, 0xF
	b GetTexFilenameForR5

.global TEX_MiniGoomba
TEX_MiniGoomba:
	lwz r5, 4(r29)
	srwi r5, r5, 24
	andi. r5, r5, 0xF
	b GetTexFilenameForR5

.global TEX_BigGoomba
TEX_BigGoomba:
	lwz r5, 4(r30)
	srwi r5, r5, 24
	andi. r5, r5, 0xF
	b GetTexFilenameForR5



















