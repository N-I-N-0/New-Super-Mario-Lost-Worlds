#includes <asm_setup.S>

.text
.align 4
.global TitleScreenLevel
.extern RESTART_CRSIN_LevelStartStruct
.extern SaveFileBuffer
.extern enableHardMode
TitleScreenLevel:
	lis r3, RESTART_CRSIN_LevelStartStruct@h
	ori r3, r3, RESTART_CRSIN_LevelStartStruct@l
	li r4, 0
	stb r4, 7(r3)

	# now get the FUCKING WORLD and FUCKING LEVEL
	lis r4, SaveFileBuffer@h
	ori r4, r4, SaveFileBuffer@l
	#lwz r4, 0(r4)
	lbz r5, 6(r4)
	extsb r5, r5
	mulli r5, r5, 0x980
	addi r4, r4, 0x6A0
	add r4, r4, r5

	# do we need to use Quick Save? get the QS block
	addi r4, r4, 0x1C80
	lbz r6, 2(r4)
	clrlwi. r6, r6, 31
	beq quickSaveExists
	subi r4, r4, 0x1C80
quickSaveExists:

	# now we have the save block in r4
	lbz r6, 2(r4)
	clrlwi. r6, r6, 31
	beq saveIsNotNew
	li r5, 0
	li r6, 9
	b chosenTitle
saveIsNotNew:
	li r5, 9			#world is always 10
	lbz r6, 0x723(r4)	#level saved in save file, converted from world number
chosenTitle:
	cmpwi r5, 0
	bne isntWorld1_1
	cmpwi r6, 0
	bne isntWorld1_1
	li r5, 0
	li r6, 9
isntWorld1_1:
	stb r5, 0xC(r3)
	stb r6, 0xD(r3)
	blr