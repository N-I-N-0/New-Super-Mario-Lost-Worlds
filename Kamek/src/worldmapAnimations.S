.text
.align 4

#include <asm_setup.S>

.extern custom_map_animation_ids
.extern custom_map_animation_strings
.extern returnToOriginalAnimationCode1			#0x800f51f0
.extern returnToOriginalAnimationCode2			#0x800f51cc

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