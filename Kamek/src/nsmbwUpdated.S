.text
.align 4

#include <asm_setup.S>

.global turnIcicleIntoIceballYoshiEat
turnIcicleIntoIceballYoshiEat:
    stfs f0, 0x31c(r28)
    li r4, 5
    stb r4, 0x36d(r28)
    blr


.extern FUN_8085f5c0	#0x8085f5c0
.extern fixJumbo1return	#0x80860074
.extern fixJumbo2return	#0x808600d8
.global fixJumbo1
fixJumbo1:
	bl FUN_8085f5c0
	b fixJumbo1return

.global fixJumbo2
fixJumbo2:
	bl FUN_8085f5c0
	b fixJumbo2return


.global fixLightmapsForDolphin
fixLightmapsForDolphin:
	li r4, 5
    stb r4, 0xc(r3)
    li r4, 1
    stb r4, 0xd(r3)
    li r4, 0
    blr


.global fixEventControlledCameras
fixEventControlledCameras:
    li r31, 0xff
    stb r31, 0x20(r6)
    li r31, 0
    blr