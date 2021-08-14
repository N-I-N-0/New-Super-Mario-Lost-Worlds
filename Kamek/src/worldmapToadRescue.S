.text
.align 4

#include <asm_setup.S>

.extern CurrentWorldNumForWorldMapSub
.extern continueAt80e3c274

.global ReturnByteFromArrayAt0x10InWorldData
ReturnByteFromArrayAt0x10InWorldData:
	add		r3, r3, r4
	lis		r4, CurrentWorldNumForWorldMapSub@h
	ori		r4, r4, CurrentWorldNumForWorldMapSub@l
	lbz		r4, 0x0(r4)
	mulli	r4, r4, 0x8
	add		r3, r3, r4
	lbz		r3, 0x70B(r3)
	blr

.global StoreByteIntoArrayAt0x10InWorldData
StoreByteIntoArrayAt0x10InWorldData:
	add		r3, r3, r4
	lis		r4, CurrentWorldNumForWorldMapSub@h
	ori		r4, r4, CurrentWorldNumForWorldMapSub@l
	lbz		r4, 0x0(r4)
	mulli	r4, r4, 0x8
	add		r3, r3, r4
	stb		r3, 0x70B(r3)
	blr