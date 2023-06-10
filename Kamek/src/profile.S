.text
.align 4
.set sp, 1

.extern sprites
.extern spriteFiles
.extern customSpriteFiles

#ifndef __MWERKS__
.set r0,0;   .set r1,1;   .set r2,2; .set r3,3;   .set r4,4
.set r5,5;   .set r6,6;   .set r7,7;   .set r8,8;   .set r9,9
.set r10,10; .set r11,11; .set r12,12; .set r13,13; .set r14,14
.set r15,15; .set r16,16; .set r17,17; .set r18,18; .set r19,19
.set r20,20; .set r21,21; .set r22,22; .set r23,23; .set r24,24
.set r25,25; .set r26,26; .set r27,27; .set r28,28; .set r29,29
.set r30,30; .set r31,31; .set f0,0; .set f2,2; .set f3,3
.set f1,1;   .set f30,30; .set f31,31
#endif

.global SpriteList_r3
.global SpriteList_r5
.global SpriteList_r7
.global SpriteList_r9
.global SpriteList_r28
.global SpriteList_r29
.global SpriteList_r30
.global SetSpriteFileList

SpriteList_r3:
    lis       r3, sprites@ha
    addi      r3, r3, sprites@l
    blr

SpriteList_r5:
    lis       r5, sprites@ha
    addi      r5, r5, sprites@l
    blr

SpriteList_r7:
    lis       r7, sprites@ha
    addi      r7, r7, sprites@l
    blr

SpriteList_r9:
    lis       r9, sprites@ha
    addi      r9, r9, sprites@l
    blr

SpriteList_r28:
    lis       r28, sprites@ha
    addi      r28, r28, sprites@l
    blr

SpriteList_r29:
    lis       r29, sprites@ha
    addi      r29, r29, sprites@l
    blr

SpriteList_r30:
    lis       r30, sprites@ha
    addi      r30, r30, sprites@l
    blr

SetSpriteFileList:
    lhz r0, 0(r24)
    cmpwi r0, 483
    blt regularsprite
    mr r25, r0
    subi r25, r25, 483
    mr r0, r25
    lis r25, customSpriteFiles@ha
    addi r25, r25, customSpriteFiles@l
    blr
regularsprite:
    lis r25, spriteFiles@ha
    addi r25, r25, spriteFiles@l
    blr
