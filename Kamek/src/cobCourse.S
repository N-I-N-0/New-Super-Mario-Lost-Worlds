.text
#include <asm_setup.S>

.extern doneWithPatchAnimNameArray
.extern doneWithPatchAnimByte34Array

.global PatchAnimNameArray
PatchAnimNameArray:
	lis r4, AnimNameArray@h
	ori r4, r4, AnimNameArray@l
	lwzx r4, r28, r4
	b doneWithPatchAnimNameArray

.global PatchAnimByte34Array
PatchAnimByte34Array:
	lis r4, AnimByte34Array@h
	ori r4, r4, AnimByte34Array@l
	lwzx r4, r26, r4
	b doneWithPatchAnimByte34Array


.data
.global AnimNameArray
AnimNameArray:
	.long aCobcourseclear
	.long aCobcoursehelp
	.long aCobcourseopen
	.long aCobcoursehalf
	.long 0

.global AnimByte34Array
AnimByte34Array:
	.long 1, 0, 0, 1

aCobcourseclear:
	.string "cobCourseClear"

aCobcoursehelp:
	.string "cobCourseHelp"

aCobcourseopen:
	.string "cobCourseOpen"

aCobcoursehalf:
	.string "cobCourseHalf"