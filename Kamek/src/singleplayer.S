.text
#include <asm_setup.S>

.extern doneWithChangeRemoconID
.extern doneWithChangeRemoconID2
.extern realPlayer
.extern playerFuckeryEnabled

.global ChangeRemoconID
ChangeRemoconID:
	lis r3, playerFuckeryEnabled@h
	ori r3, r3, playerFuckeryEnabled@l
	lbz r3, 0(r3)
	cmpwi r3, 1
	beq DoRemoconChange

	lbz r0, 0x38D(r31)
	b doneWithChangeRemoconID

DoRemoconChange:
	lis r3, realPlayer@h
	ori r3, r3, realPlayer@l
	lwz r0, 0(r3)
	b doneWithChangeRemoconID

.global ChangeRemoconID2
ChangeRemoconID2:
	lis r4, playerFuckeryEnabled@h
	ori r4, r4, playerFuckeryEnabled@l
	lbz r4, 0(r4)
	cmpwi r4, 1
	beq DoRemoconChange2

	lbz r0, 0x38D(r31)
	b doneWithChangeRemoconID2

DoRemoconChange2:
	lis r4, realPlayer@h
	ori r4, r4, realPlayer@l
	lwz r0, 0(r4)
	b doneWithChangeRemoconID2
