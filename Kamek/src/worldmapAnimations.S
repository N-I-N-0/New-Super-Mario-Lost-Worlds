.text
.align 4

#include <asm_setup.S>

.extern custom_map_animation_ids
.extern custom_map_animation_strings
.extern custom_map_animation_conversions
.extern returnToOriginalAnimationCode1			#0x800f51f0
.extern returnToOriginalAnimationCode2			#0x800f51cc
.extern returnToOriginalAnimationCode3			#0x809065d0
.extern returnToOriginalAnimationCode4			#0x809065cc

.global insert_animation_ids
insert_animation_ids:
    lis		r4, custom_map_animation_ids@h
    ori		r4, r4, custom_map_animation_ids@l
	b		returnToOriginalAnimationCode1


.global insert_animation_strings
insert_animation_strings:
    lis		r31, custom_map_animation_strings@h
    ori		r31, r31, custom_map_animation_strings@l
	b		returnToOriginalAnimationCode2


.global insert_animation_id_conversion
insert_animation_id_conversion:
    cmpwi	r3, 0x20
	blt-	return
	subi	r3, r3, 0x20
	mulli	r0, r3, 0x4
	mr		r31, r3
	lis		r4, custom_map_animation_conversions@h
    ori		r4, r4, custom_map_animation_conversions@l
	add		r4, r4, r0
	lwz		r28, 0x0(r4)
	b		returnToOriginalAnimationCode3
return:
	mr		r31, r3
	b		returnToOriginalAnimationCode4