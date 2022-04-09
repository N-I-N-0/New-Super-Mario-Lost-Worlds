#include <asm_setup.S>

.text
.align 4

.extern returnFromEggAddNewPowerups
.global EggAddNewPowerups
EggAddNewPowerups:
	mr		r4, r3
	slwi	r0, r4, 2
	lis		r3, yoshiEggRewards@h
	ori		r3, r3, yoshiEggRewards@l
	b		returnFromEggAddNewPowerups


.data
.align 4
yoshiEggRewards:
	.long 0x00000000 #Mushroom				when small
	.long 0x00000009 #Fire Flower			when big
	.long 0x00000014 #Gold Flower			when fire
	.long 0x00000000 #Mushroom				when mini
	.long 0x00000012 #Rosalina's Wand		when propeller
	.long 0x00000013 #Frog Suit				when penguin
	.long 0x00000011 #Penguin Suit			when ice
	.long 0x00000010 #Spike Mushroom		when hammer
	.long 0x0000000E #Ice Flower			when gold
	.long 0x0000000F #Boomerang Flower		when spike
	.long 0x00000016 #Cloud Flower			when boomerang
	.long 0x00000006 #Hammer				when frog
	.long 0x00000015 #Propeller Mushroom	when cloud
	.long 0x00000007 #1-UP Mushroom			when wand