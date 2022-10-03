#include <asm_setup.S>

#.extern ClearOldPathAvailabilityData__Fv
.extern enableNonLoopAnim__Q23m2d13EmbedLayout_cFib
.extern findTextBoxByName__Q23m2d17EmbedLayoutBase_cCFPCc
.extern findPictureByName__Q23m2d17EmbedLayoutBase_cCFPCc
.extern GrabExitInfoForFileSelect
.extern InsertIntIntoTextBox1
.extern SetWorldNameForSaveFile
#.extern ResetAllCompletionCandidates__Fv

.global A00_window
.global CDHolderGroup
.global DefaultSavefileInfo
.global DFNiceTitle
.global DFNiceWorldName
.global FSDummyBrlanName, FSDummyGroup, FSDummyPane
.global FSHideCD
.global FSShowCD_OpenWait
.global Out_Stuff
.global ShowEraseCopy, HideEraseCopy

.text
FSHideCD:
	addi r3, r3, 0x74
	li r4, 0x2B
	b enableNonLoopAnim__Q23m2d13EmbedLayout_cFib

FSShowCD_OpenWait:
	addi r3, r31, 0x74
	li r4, 0x2A
	bl enableNonLoopAnim__Q23m2d13EmbedLayout_cFib
	lwz r0, 0x14(r1)
	lwz r31, 0xC(r1)
	mtlr r0
	addi r1, r1, 0x10
	blr

DFNiceTitle:
	mr r3, r4
	lis r4, DFTitle@h
	ori r4, r4, DFTitle@l
	lwz r6, 0xE44(r30)
	addi r6, r6, '1'
	sth r6, 0xA(r4)
	li r5, 0
	li r6, 6
	lwz r12, 0(r3)
	lwz r12, 0x80(r12)
	mtctr r12
	bctr

DFNiceWorldName:
	# r1+0x20 is reserved for the stupid ints in textboxes
	stwu r1, -0x30(r1)
	mflr r0
	stw r0, 0x34(r1)
	stw r20, 0x2C(r1)
	stw r21, 0x28(r1)
	stw r22, 0x24(r1)
	stw r31, 0x20(r1)

	# Savefile is in r31


	lbz r3, 0x3(r31)	#r3 is the last played world number - save->current_world
	bl SetWorldNameForSaveFile

	# World Name field is in r20
	lwz r20, 0x2C(r1)
	lwz r31, 0x20(r1)

	# Load hint colour
	lwz r3, 0x71C(r31)
	stw r3, 0xDC(r20)
	lwz r3, 0x720(r31)
	stw r3, 0xE0(r20)

	# Set hint color
	#addi r3, r30, 0x74
	#lis r4, Picture_00@h
	#ori r4, r4, Picture_00@l
	#bl findPictureByName__Q23m2d17EmbedLayoutBase_cCFPCc
	
	# Load text color
	#lwz r4, 0x724(r31)
	#stw r4, 0xD8(r3)
	#stw r4, 0xDC(r3)
	#lwz r4, 0x728(r31)
	#stw r4, 0xE0(r3)
	#stw r4, 0xE4(r3)

	# Grab exits
	lis r3, Out_Stuff@h
	ori r3, r3, Out_Stuff@l
	mr r4, r31
	bl GrabExitInfoForFileSelect
	lwz r21, 0(r3)
	lwz r22, 4(r3)

	# Write exits to text box
	addi r3, r30, 0x74
	lis r4, ExitCount@h
	ori r4, r4, ExitCount@l
	bl findTextBoxByName__Q23m2d17EmbedLayoutBase_cCFPCc

	stw r22, 0x20(r1)
	mr r4, r3
	addi r3, r1, 0x20
	li r5, 1
	bl InsertIntIntoTextBox1

	# Find StarCoinCount pane
	addi r3, r30, 0x74
	lis r4, StarCoinCount@h
	ori r4, r4, StarCoinCount@l
	bl findTextBoxByName__Q23m2d17EmbedLayoutBase_cCFPCc

	# Update star coin count width
	lis r4, NewSCCWidth@ha
	lfs f1, NewSCCWidth@l(r4)
	stfs f1, 0x4C(r3)

	# Write star coins to text box
	stw r21, 0x20(r1)
	mr r4, r3
	addi r3, r1, 0x20
	li r5, 0
	bl InsertIntIntoTextBox1

	# Finish it
	#bl ResetAllCompletionCandidates__Fv
	#bl ClearOldPathAvailabilityData__Fv

	lwz r22, 0x24(r1)
	lwz r21, 0x28(r1)
	lwz r20, 0x2C(r1)
	lwz r0, 0x34(r1)
	mtlr r0
	addi r1, r1, 0x30
	blr

DefaultSavefileInfo:
	addi r4, r3, 0x6FC
	lis r5, DefaultSavefileInfoData@h
	ori r5, r5, DefaultSavefileInfoData@l
	lis r6, DefaultSavefileInfoDataEnd@h
	ori r6, r6, DefaultSavefileInfoDataEnd@l

DSFICopyLoop:
	lwz r7, 0(r5)
	stw r7, 0(r4)
	addi r4, r4, 4
	addi r5, r5, 4
	cmpw r5, r6
	blt DSFICopyLoop
	blr

.data
FSDummyBrlanName: .string "fileSelectBase_18_DummyAnim.brlan"
FSDummyGroup: .string "DummyGroup"
FSDummyPane: .string "DummyPane"

A00_window: .string "A00_window"

ShowEraseCopy: .string "fileSelectBase_18_ShowEraseCopy.brlan"
HideEraseCopy: .string "fileSelectBase_18_HideEraseCopy.brlan"

CDHolderGroup: .string "CDHolderGroup"

StarCoinCount: .string "StarCoinCount"
ExitCount: .string "ExitCount"
Picture_00: .string "Picture_00"

.align 4
NewSCCWidth: .float 100.0

Out_Stuff:
.long 0, 0

.align 4
DFTitle:
.short 'F','i','l','e',' ','X',0

ConvertedWorldName:
.long 0, 0, 0, 0, 0, 0, 0, 0
.long 0, 0, 0, 0, 0, 0, 0, 0
.long 0, 0, 0, 0, 0, 0, 0, 0
.long 0, 0, 0, 0, 0, 0, 0, 0

.align 4
DefaultSavefileInfoData:
.string "Choose a Game" # Game name (14 bytes)
.byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 # Padding to 32
.long 0xFFFFFFFF,0xFFFFFFFF  # File Text
.long 0xFF6A00FF,0xCC5100FF  # File Background
.long 0xFFFFFFFF,0xFFFFFFFF  # Map Text
.short 0x18  # Map Hue
.byte 0x3C,0x1E  # Map Saturation / Lightness
.byte 1,1  # Music ID / World ID
.byte 0,46  # Titlescreen World/Level (both -1)

DefaultSavefileInfoDataEnd:
.long 0
