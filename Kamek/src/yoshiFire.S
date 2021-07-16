.text
.align 4

#include <asm_setup.S>

.extern SpawnSprite
.extern Delete__7fBase_cFv
.extern Yoshi_EatNoneState
.extern daYoshi_c_sub_8014EE40
.extern Actor_SearchByID

# TODO: convert this to C (partially in yoshiFire.cpp)
.global addMoreFireBalls
addMoreFireBalls:
    # store some registers to the stack
    mflr   r0
    stw    r29, 0x14(r1)
    stw    r0, 0x10(r1)
    stw    r28, 0xC(r1)
    mr     r29, r3

    # Only do stuff if the animation is done
    # r29 contains this bool
    cmpwi  r29, 0     # patched instruction
    beq    addMoreFireBalls_end

    # get if koopa or paratroopa
    lhz    r0, 8(r30)
    cmplwi r0, 54  # koopa troopa
    beq    addMoreFireBalls_ate_koopa
    cmplwi r0, 55  # koopa paratroopa
    bne    addMoreFireBalls_end

addMoreFireBalls_ate_koopa: #80e3fd50 (NTSC)
    #######################
    # Store Yoshi's Color #
    #######################
    lwz r0, 0x4(r31)
    clrlwi r0, r0, 28   # 0 = light blue; 1 = pink; 2 = yellow; 3 = green; 4 = blue; 5 = red; 6 = orange; 7 = brown; 8 = white; 9 = purple.

    #########################
    # Compare Yoshi's color #
    #########################
    # Remove neutral yoshis
    cmpwi r0, 1
    beq addMoreFireBalls_end # if pink, return
    cmpwi r0, 3
    beq addMoreFireBalls_end # if green, return
    cmpwi r0, 6
    bge addMoreFireBalls_end # if orange, brown, white or purple, return
    # Remove Yoshis that don't have edited projectiles
    cmpwi r0, 2
    beq addMoreFireBalls_shell # if yellow, return
    cmpwi r0, 4
    beq addMoreFireBalls_shell # if blue, return
    # Add Yoshis that have edited projectiles
    cmpwi r0, 0
    beq addMoreFireBalls_make_ice # if light blue, create ice
    cmpwi r0, 5
    beq addMoreFireBalls_make_fire # if light blue, create ice
    # In the case it arrives here for some reason ???
    b addMoreFireBalls_end

addMoreFireBalls_make_ice: #80e3fd94 (NTSC)
    # The main iceball
    lbz    r0, 0x38D(r31)

    extsb  r0, r0
    slwi   r4, r0, 4     # Settings = The current PlayerId

    li     r0, 1         # Set r0 to 1
    or     r4, r4, r0    # OR it in ((this->settings & 0x04000000) >> 26) (so it becomes iceballs)

    b addMoreFireBalls_make_newprojectile

addMoreFireBalls_make_fire:
    # The main fireball
    lbz    r0, 0x38D(r31)

    extsb  r0, r0
    slwi   r4, r0, 4     # Settings = The current PlayerId

addMoreFireBalls_make_newprojectile:
    # Spawn the fireball/iceball
    li     r3, 0x13      # YOSHI_FIRE
    li     r5, 0         # position
    li     r6, 0         # rotation
    li     r7, 0         # layer
    bl     SpawnSprite

    # Store this fireball/iceball's in Yoshi's mouth (get its ID and store it in this->eatenID)
    lwz    r0, 0(r3)
    stw    r0, 0x1520(r31)

    # Save this pointer
    mr     r28, r3

    # FIRST - copy settings from sibling
    li     r3, 0x13      # YOSHI_FIRE
    lwz    r4, 4(r28)    # settings
    li     r5, 0         # position
    li     r6, 0         # rotation
    li     r7, 0         # layer
    bl     SpawnSprite

    # Save the sibling id to the main fire ball
    lwz    r0, 0(r3)
    stw    r0, 0x500(r28)

    # SECOND - copy settings from sibling
    li     r3, 0x13      # YOSHI_FIRE
    lwz    r4, 4(r28)    # settings
    li     r5, 0         # position
    addi   r6, r1, 6     # rotation
    li     r7, 0         # layer
    bl     SpawnSprite

    # Save the sibling id to the main fire ball
    lwz    r0, 0(r3)
    stw    r0, 0x504(r28)

    # Delete the original koopa
    mr     r3, r30
    bl     Delete__7fBase_cFv

    # Set Yoshi's state
    lis    r4, Yoshi_EatNoneState@h
    mr     r3, r31
    addi   r4, r4, Yoshi_EatNoneState@l
    bl     daYoshi_c_sub_8014EE40
    b      addMoreFireBalls_end

addMoreFireBalls_shell:
    # Trigger nybble 5 (is there a shell is yoshi's mouth)
    lwz    r0, 0x4(r31)
    oris   r0, r0, 0x1000
    stw    r0, 0x4(r31)

addMoreFireBalls_end:
    # set the EQ bit so we don't set the state again
    cmpwi  r29, 0
    lwz    r28, 0xC(r1)
    lwz    r29, 0x14(r1)
    lwz    r0, 0x10(r1)
    mtlr   r0
    blr

################################################################################
################################################################################
################################################################################

.global disableShellTrigger
disableShellTrigger:
    # Untrigger nybble 5 (is there a shell is yoshi's mouth) if it's enabled
    lwz    r0, 0x4(r31)     # Get the current settings
    oris   r0, r0, 0x1000   # Trigger nybble 5 if it's not already (this avoids to tigger the nybble 5 if it wasn't triggered yet in the next line)
    xoris  r0, r0, 0x1000   # Untrigger it
    stw    r0, 0x4(r31)     # Store the result in the current settings
    mr     r0, r20          # More r20 into r0
    neg    r0, r5           # Patched Instruction
    blr

################################################################################
################################################################################
################################################################################

.global GetThoseSiblingsMoving

# TODO: convert this to C
GetThoseSiblingsMoving:
    # Patches part of YOSHI_FIRE->vf8C()
    mflr    r0
    stw     r0, 0x10(r1)
    stw     r28, 0xC(r1)

    li      r0, 1
    stw     r0, 0x3D0(r29)    # Patched instruction

    # Load first sibling
    lwz     r3, 0x500(r29)
    cmpwi   r3, 0
    beq     GetThoseSiblingsMoving_end

    # Search for it
    bl      Actor_SearchByID
    mr.     r28, r3
    beq     GetThoseSiblingsMoving_end

    # Call actor->vf8C to activate it
    lwz     r12, 0x60(r3)
    lwz     r12, 0x8C(r12)
    mtctr   r12
    mr      r4, r30
    bctrl

    # Make it move up a little bit as well
    # f0 still contains the horizontal speed
    lis     r4, straightToSlanted@ha
    lfs     f1, straightToSlanted@l(r4)
    fmuls   f0, f0, f1
    stfs    f0, 0xE8(r28)

    # Now make it move up
    lis     r4, horizontalToVertical@ha
    lfs     f1, horizontalToVertical@l(r4)
    fmuls   f0, f0, f1
    stfs    f0, 0xEC(r28)

    # Let it show, let it show, can't hold it back any more
    li      r0, 1
    stw     r0, 0x3D0(r28)

    # Load second sibling
    lwz     r3, 0x504(r29)
    cmpwi   r3, 0
    beq     GetThoseSiblingsMoving_end

    # Search for it
    bl      Actor_SearchByID
    mr.     r28, r3
    beq     GetThoseSiblingsMoving_end

    # Call actor->vf8C to activate it
    lwz     r12, 0x60(r3)
    lwz     r12, 0x8C(r12)
    mtctr   r12
    mr      r4, r30
    bctrl

    # Make it move down a little bit as well
    # f0 still contains the horizontal speed
    lis     r4, straightToSlanted@ha
    lfs     f1, straightToSlanted@l(r4)
    fmuls   f0, f0, f1
    stfs    f0, 0xE8(r28)

    # Now make it move down
    lis     r4, horizontalToVertical@ha
    lfs     f1, horizontalToVertical@l(r4)
    fneg    f1, f1
    fmuls   f0, f0, f1
    stfs    f0, 0xEC(r28)

    # Let it show, let it show, can't hold it back any more
    li      r0, 1
    stw     r0, 0x3D0(r28)

GetThoseSiblingsMoving_end:
    mr      r3, r29
    lwz     r0, 0x10(r1)
    lwz     r28, 0xC(r1)
    mtlr    r0
    blr

.data
.align 4
straightToSlanted:
    .float 0.9

.align 4
horizontalToVertical:
    .float 0.222222

.align 4
keepHorizontal:
    .float 0.0

