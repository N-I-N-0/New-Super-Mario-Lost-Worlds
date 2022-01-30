.text
#include <asm_setup.S>

.align 4
.extern check_GROUP_CASTLE
.extern return_after_GROUP_BOSS_script
.extern not_GROUP_BOSS_script
.extern FUN_801017c0

.global check_GROUP_BOSS
check_GROUP_BOSS:
	bge	not_GROUP_TOWER
	li		r3, 0x2
	blr
	
not_GROUP_TOWER:
	cmpwi	r3, 0x18
	bge		go_check_GROUP_CASTLE
	li		r3, 0xb
	blr

go_check_GROUP_CASTLE:
	b		check_GROUP_CASTLE

.global GROUP_BOSS_script
GROUP_BOSS_script:
	cmpwi	r30, 0xb 
	beq		do_GROUP_BOSS_script
	cmplwi	r30, 0xa	#restore with 0xa -> if 0xb jump won't be reached
	b		not_GROUP_BOSS_script
do_GROUP_BOSS_script:
	lwz		r3, -0x5b74(r4)
	li		r4, 0x36
	li		r5, 0
	li		r6, 0
	li		r7, 0x80
	bl		FUN_801017c0
	b		return_after_GROUP_BOSS_script
	