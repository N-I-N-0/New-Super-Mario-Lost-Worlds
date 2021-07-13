.text
.align 4

#include <asm_setup.S>
.extern ContinueWithoutCourage
.extern ContinueWithCourage
.extern Actor_SearchByID

.global haveCourageMyCuteYoshi
haveCourageMyCuteYoshi:
	lwz 	r3, 0x4(r28)
    clrlwi	r3, r3, 28					#0 = light blue; 1 = pink; 2 = yellow; 3 = green; 4 = blue; 5 = red; 6 = orange; 7 = brown; 8 = white; 9 = purple.
	cmpwi	r3, 1
	beq		hasCourageColor
	cmpwi	r3, 9
	beq		hasCourageColor
	b		ContinueWithoutCourage

hasCourageColor:
	lwz 	r3, 0x1520(r28)				#eatenID
	bl		Actor_SearchByID
	lhz		r3, 8(r3)
	cmplwi	r3, 54
	beq		hasCourage
	cmplwi	r3, 55
	beq		hasCourage
	b		ContinueWithoutCourage

hasCourage:
	b		ContinueWithCourage