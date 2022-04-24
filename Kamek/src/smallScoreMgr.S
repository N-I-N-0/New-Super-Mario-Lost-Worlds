.text
.align 4
#include <asm_setup.S>

.extern continueFromChangeSmallScore100Color
.extern continueFromChangeSmallScore1000Color1
.extern continueFromChangeSmallScore1000Color2
.extern continueFromChangeSmallScore1UPColor1
.extern continueFromChangeSmallScore1UPColor2

#100

.global ChangeSmallScore100Color
ChangeSmallScore100Color:
	lis r5, SmallScore100Color@h
	ori r5, r5, SmallScore100Color@l
	b continueFromChangeSmallScore100Color

#1000

.global ChangeSmallScore1000Color1
ChangeSmallScore1000Color1:
	lis r6, SmallScore1000Color1@h
	ori r6, r6, SmallScore1000Color1@l
	b continueFromChangeSmallScore1000Color1

.global ChangeSmallScore1000Color2
ChangeSmallScore1000Color2:
	lis r3, SmallScore1000Color2@h
	ori r3, r3, SmallScore1000Color2@l
	b continueFromChangeSmallScore1000Color2

#1UP

.global ChangeSmallScore1UPColor1
ChangeSmallScore1UPColor1:
	lis r5, SmallScore1UPColor1@h
	ori r5, r5, SmallScore1UPColor1@l
	b continueFromChangeSmallScore1UPColor1

.global ChangeSmallScore1UPColor2
ChangeSmallScore1UPColor2:
	lis r4, SmallScore1UPColor2@h
	ori r4, r4, SmallScore1UPColor2@l
	b continueFromChangeSmallScore1UPColor2

.data
.align 4

#100

.global SmallScore100Color
SmallScore100Color:
	.long 0x46FA46FF	#Baby Luigi
	.long 0x46FA46FF	#Luigi
	.long 0xFFB43BFF	#Daisy
	.long 0xFF52F3FF	#Peach
	.long 0xFAFFFFFF

#1000

.global SmallScore1000Color1
SmallScore1000Color1:
	.long 0x32FA32FF	#Baby Luigi
	.long 0x32FA32FF	#Luigi
	.long 0xFF9500FF	#Daisy
	.long 0xFF4AFFFF	#Peach
	.long 0xFFFFFFFF

.global SmallScore1000Color2
SmallScore1000Color2:
	.long 0xD5FF7AFF	#Baby Luigi
	.long 0xD5FF7AFF	#Luigi
	.long 0xFFCF6EFF	#Daisy
	.long 0xFAC3F2FF	#Peach
	.long 0xFFFFFFFF

#1UP

.global SmallScore1UPColor1
SmallScore1UPColor1:
	.long 0xD2FF00FF	#Baby Luigi
	.long 0xD2FF00FF	#Luigi
	.long 0xFFB700FF	#Daisy
	.long 0xFF70FFFF	#Peach
	.long 0xFFFFFFFF

.global SmallScore1UPColor2
SmallScore1UPColor2:
	.long 0x197D0AFF	#Baby Luigi
	.long 0x197D0AFF	#Luigi
	.long 0xFFB03BFF	#Daisy
	.long 0x8F1D8FFF	#Peach
	.long 0xFFBEFFFF