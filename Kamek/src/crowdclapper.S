.text
#include <asm_setup.S>

.extern checkForCounts__12daNiceCoin_cFv

.extern doneWithCheckNewCounts
.extern doneWithCheckNewCounts2
.extern doneWithCheckNewCounts3

.global ParseNewCrowdSettings
ParseNewCrowdSettings:
	stw		r0, 0x3D4(r31) # Restore instruction

	rlwinm	r3, r5, 8, 24, 31	# Equivalent of ((this->settings >> 24) & 0xFF)
	rlwinm	r4, r5, 28, 31, 31	# Equivalent of ((this->settings >> 4) & 1)
	stw		r3, 0x3F0(r31)		# Number of Purple Coins to collect
	stb		r4, 0x3F8(r31)		# Disable SFX
	blr

.global CheckNewCounts
CheckNewCounts:
	bl checkForCounts__12daNiceCoin_cFv
	cmpwi r3, 1
	beq triggerCrowdClapper
	b doneWithCheckNewCounts
triggerCrowdClapper:
	lbz r3, 0x3F8(r31)
	cmpwi r3, 1
	beq noCrowdSFX
	b doneWithCheckNewCounts2
noCrowdSFX:
	b doneWithCheckNewCounts3