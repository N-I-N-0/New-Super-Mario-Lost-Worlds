# (c) Ninji 2010-2013
# Original disassembly by RootCubed, TW and KR support from NewerSMBW v1.3.0
# First Stage Loader by Asu-chan & CLF78

.macro callSysFunction offset
    lwz r12, \offset(r31)
    mtctr r12
    bctrl
.endm

.macro branchSysFunction offset
    lwz r12, \offset(r31)
    mtctr r12
    bctr
.endm

# offsets in system function table
OSReport =                  0x0
OSFatal =                   0x4
DVDConvertPathToEntrynum =  0x8
DVDFastOpen =               0xC
DVDReadPrio =               0x10
DVDClose =                  0x14
HeapAlloc =                 0x18
HeapFree =                  0x1C
GameHeapArray =             0x20
ArchiveHeap =               0x24
sprintf =                   0x28
BCACheck =                  0x2C
memcpy =                    0x30
memmove =                   0x34
GameInitTable =             0x38
DoneWithFirstLoader =       0x3C

.text
.align 4

# Start of the First Stage Loader

.global _firstStart
_firstStart:
TEST_REGION = 0x800CF6CC
TEST_REGION2 = 0x8000423A
    # Region Check
    li        r6, '1' # Assuming version 1...
    lis       r5, TEST_REGION@h
    ori       r5, r5, TEST_REGION@l
    lwz       r3, 0(r5) # read wherever this shit points to
    clrlwi    r3, r3, 16
    cmpwi     r3, 0x0030
    beq       setEU
    cmpwi     r3, 0x0465
    beq       setUS
    cmpwi     r3, 0x00B4
    beq       setJP
    li        r6, '2' # try testing for version 2
    cmpwi     r3, 0x0038
    beq       setEU
    cmpwi     r3, 0x0000
    beq       setUS
    cmpwi     r3, 0x000C
    beq       setJP
    li        r6, '3'
    cmpwi     r3, 0x0001
    bne       Panic
    lis       r5, TEST_REGION2@h
    ori       r5, r5, TEST_REGION2@l
    lbz       r3, 0(r5)
    cmpwi     r3, 0xC8
    beq       setKR
    li        r6, '4'
    cmpwi     r3, 0xAC
    beq       setTW
    b         Panic # unknown version

setEU:
    lis       r31, EUAddr@h
    ori       r31, r31, EUAddr@l
    b         RegionDetected
setUS:
    lis       r31, USAddr@h
    ori       r31, r31, USAddr@l
    b         RegionDetected
setJP:
    lis       r31, JPAddr@h
    ori       r31, r31, JPAddr@l
    b         RegionDetected
setKR:
    lis       r31, KRAddr@h
    ori       r31, r31, KRAddr@l
    b         RegionDetected
setTW:
    lis       r31, TWAddr@h
    ori       r31, r31, TWAddr@l

RegionDetected:
    # Save Region and OSReport it
    lis       r30, RegionAddr@h
    ori       r30, r30, RegionAddr@l
    stw       r31, 0(r30)
    stw       r6, 4(r30)

    lis       r3, FSLRegion@h
    ori       r3, r3, FSLRegion@l
    li        r4, 14
    bl        InsertRegionIntoFileName

    lis       r3, FSLStart@h
    ori       r3, r3, FSLStart@l
    callSysFunction OSReport
    lis       r3, FSLRegion@h
    ori       r3, r3, FSLRegion@l
    callSysFunction OSReport

    # Remove Anti-piracy
    lis       r3, 0x6000
    lwz       r30, BCACheck(r31)
    stw       r3, 0(r30)

    lis       r3, FSLBCA@h
    ori       r3, r3, FSLBCA@l
    callSysFunction OSReport

    # Rearrange GameInitTable to allow for the Main Loader to be executed
    lwz       r30, GameInitTable(r31)

    lis       r3, memcpybuffer@h       # Destination
    ori       r3, r3, memcpybuffer@l
    addi      r4, r30, 0x3C            # Source
    li        r5, 0xC                  # Size
    callSysFunction memcpy

    addi      r3, r30, 0x10            # Destination
    mr        r4, r30                  # Source
    li        r5, 0x3C                 # Size
    callSysFunction memmove

    mr        r3, r30                  # Destination
    lis       r4, memcpybuffer@h       # Source
    ori       r4, r4, memcpybuffer@l
    li        r5, 0xC                  # Size
    callSysFunction memcpy

    lis       r3, _start@h
    ori       r3, r3, _start@l
    stw       r3, 0xC(r30)
    
    # We're done, let's get outta here.
    lis       r3, FSLFinish@h
    ori       r3, r3, FSLFinish@l
    callSysFunction OSReport

    li        r0, 0
    branchSysFunction DoneWithFirstLoader


memcpybuffer: .long 0, 0, 0
FSLStart: .string "<< CODE_LOADER \trelease build: Jul 18 2023 18:35:00 >>\n"
FSLRegion: .string "-Found Region xx_x\n"
FSLBCA: .string "-Removed BCA Check\n"
FSLFinish: .string "-Patched GameInitTable\n"
.align 4


# Start of the Main Loader

.global _start
_start:
    b main
# offsets to system calls
EUAddr:
    .long 0x8015F870 # OSReport_EU
    .long 0x801AF710 # OSFatal_EU
    .long 0x801CA7C0 # DVDConvertPathToEntrynum_EU
    .long 0x801CAAD0 # DVDFastOpen_EU
    .long 0x801CAC60 # DVDReadPrio_EU
    .long 0x801CAB40 # DVDClose_EU
    .long 0x802B8E00 # HeapAlloc_EU
    .long 0x802B90B0 # HeapFree_EU
    .long 0x80377F48 # GameHeapArray_EU
    .long 0x8042A72C # ArchiveHeap_EU
    .long 0x802E1ACC # sprintf_EU
    .long 0x800CA0B8 # BCACheck_EU
    .long 0x80004364 # memcpy
    .long 0x802DF264 # memmove_EU
    .long 0x80328428 # GameInitTable_EU
    .long 0x80004070 # DoneWithFirstLoader
    .string "EU"
    .byte 0
USAddr:
    .long 0x8015F730 # OSReport_US
    .long 0x801AF5D0 # OSFatal_US
    .long 0x801CA680 # DVDConvertPathToEntrynum_US
    .long 0x801CA990 # DVDFastOpen_US
    .long 0x801CAB20 # DVDReadPrio_US
    .long 0x801CAA00 # DVDClose_US
    .long 0x802B8CC0 # HeapAlloc_US
    .long 0x802B8F70 # HeapFree_US
    .long 0x80377C48 # GameHeapArray_US
    .long 0x8042A44C # ArchiveHeap_US
    .long 0x802E17DC # sprintf_US
    .long 0x800C9FC8 # BCACheck_US
    .long 0x80004364 # memcpy
    .long 0x802DEF74 # memmove_US
    .long 0x803280E0 # GameInitTable_US
    .long 0x80004070 # DoneWithFirstLoader
    .string "US"
    .byte 0
JPAddr:
    .long 0x8015F540 # OSReport_JP
    .long 0x801AF3E0 # OSFatal_JP
    .long 0x801CA490 # DVDConvertPaJP
    .long 0x801CA7A0 # DVDFastOpen_JP
    .long 0x801CA930 # DVDReadPrio_JP
    .long 0x801CA810 # DVDClose_JP
    .long 0x802B8AD0 # HeapAlloc_JP
    .long 0x802B8D80 # HeapFree_JP
    .long 0x803779C8 # GameHeapArraJP
    .long 0x8042A16C # ArchiveHeap_JP
    .long 0x802E15EC # sprintf_JP
    .long 0x800C9F48 # BCACheck_JP
    .long 0x80004364 # memcpy
    .long 0x802DED84 # memmove_JP
    .long 0x80327E48 # GameInitTable_JP
    .long 0x80004070 # DoneWithFirstLoader
    .string "JP"
    .byte 0
KRAddr:
    .long 0x8015FC70 # OSReport_KR
    .long 0x801AFB10 # OSFatal_KR
    .long 0x801CABC0 # DVDConvertPaKR
    .long 0x801CAED0 # DVDFastOpen_KR
    .long 0x801CB060 # DVDReadPrio_KR
    .long 0x801CAF40 # DVDClose_KR
    .long 0x802B9200 # HeapAlloc_KR
    .long 0x802B94B0 # HeapFree_KR
    .long 0x80384948 # GameHeapArraKR
    .long 0x804370EC # ArchiveHeap_KR
    .long 0x802E1D1C # sprintf_KR
    .long 0x800CA0D8 # BCACheck_KR
    .long 0x80004364 # memcpy
    .long 0x802DF4B4 # memmove_KR
    .long 0x80334E10 # GameInitTable_KR
    .long 0x80004070 # DoneWithFirstLoader
    .string "KR"
    .byte 0
TWAddr:
    .long 0x8015FC70 # OSReport_TW
    .long 0x801AFB10 # OSFatal_TW
    .long 0x801CABC0 # DVDConvertPaTW
    .long 0x801CAED0 # DVDFastOpen_TW
    .long 0x801CB060 # DVDReadPrio_TW
    .long 0x801CAF40 # DVDClose_TW
    .long 0x802B9200 # HeapAlloc_TW
    .long 0x802B94B0 # HeapFree_TW
    .long 0x80382D48 # GameHeapArraTW
    .long 0x804354EC # ArchiveHeap_TW
    .long 0x802E1D1C # sprintf_TW
    .long 0x800CA0D8 # BCACheck_TW
    .long 0x80004364 # memcpy
    .long 0x802DF4B4 # memmove_TW
    .long 0x803331C8 # GameInitTable_TW
    .long 0x80004070 # DoneWithFirstLoader
    .string "TW"
    .byte 0
NewerSystem: .string "/Code/Systemxx_x.bin"
NewerDlcode: .string "/Code/DLCodexx_x.bin"
NewerDlrel:  .string "/Code/DLRelocsxx_x.bin"
    .align 4

RegionAddr:
    .long 0
    .long 0

main:
    stwu      r1, -0x60(r1) # reserve 60 bytes on the stack
    mflr      r0 # Backup Link Register
    # Backup GPRs
    stw       r0, 0x64(r1)
    stw       r31, 0x5C(r1)
    stw       r30, 0x58(r1)
    stw       r29, 0x54(r1)

    lis       r31, RegionAddr@h
    ori       r31, r31, RegionAddr@l
    lwz       r6, 4(r31)
    lwz       r31, 0(r31)

    lis       r3, NewerSystem@h
    ori       r3, r3, NewerSystem@l
    li        r4, 0xC
    bl        InsertRegionIntoFileName
    lis       r3, NewerDlcode@h
    ori       r3, r3, NewerDlcode@l
    bl        InsertRegionIntoFileName
    lis       r3, NewerDlrel@h
    ori       r3, r3, NewerDlrel@l
    li        r4, 0xE
    bl        InsertRegionIntoFileName
    #-------------------------
    # Step 1: Load system file
    #-------------------------
    lis       r3, Step1@h
    ori       r3, r3, Step1@l
    lis       r4, NewerSystem@h
    ori       r4, r4, NewerSystem@l
    callSysFunction OSReport
    lis       r3, NewerSystem@h
    ori       r3, r3, NewerSystem@l
    callSysFunction DVDConvertPathToEntrynum
    li        r4, -1
    cmpw      r3, r4
    beq       Error_FileLoad
    addi      r4, r1, 8
    callSysFunction DVDFastOpen
    li        r4, 1
    cmpw      r3, r4
    bne       Error_OpenFile
    #-------------------------
    # Step 2: Load Patches
    #-------------------------
    lis       r3, Step2@h
    ori       r3, r3, Step2@l
    lwz       r4, 0x3C(r1)
    callSysFunction OSReport
    lwz       r3, 0x3C(r1)
    addi      r3, r3, 0x1F
    clrrwi    r3, r3, 5
    li        r4, 0x20
    lwz       r5, 0x24(r31)
    lwz       r5, 0(r5)
    callSysFunction HeapAlloc
    mr        r27, r3
    cmpwi     r27, 0
    beq       Error_AllocMem
    addi      r3, r1, 8
    mr        r4, r27
    lwz       r5, 0x3C(r1)
    addi      r5, r5, 0x1F
    clrrwi    r5, r5, 5
    li        r6, 0
    li        r7, 2
    callSysFunction DVDReadPrio
    addi      r3, r1, 8
    callSysFunction DVDClose
    #-------------------------
    # Step 3: Load dylink code
    #-------------------------
    lis       r3, Step3@h
    ori       r3, r3, Step3@l
    callSysFunction OSReport
    lis       r3, NewerDlcode@h
    ori       r3, r3, NewerDlcode@l
    callSysFunction DVDConvertPathToEntrynum
    addi      r4, r1, 8
    callSysFunction DVDFastOpen
    lwz       r3, 0x3C(r1)
    addi      r3, r3, 0x1F
    clrrwi    r3, r3, 5
    li        r4, 0x20
    lwz       r5, 0x20(r31)
    lwz       r5, 0(r5)
    callSysFunction HeapAlloc
    mr        r29, r3
    addi      r3, r1, 8
    mr        r4, r29
    lwz       r5, 0x3C(r1)
    addi      r5, r5, 0x1F
    clrrwi    r5, r5, 5
    li        r6, 0
    li        r7, 2
    callSysFunction DVDReadPrio
    addi      r3, r1, 8
    callSysFunction DVDClose
    lis       r3, Step3DLCode@h
    ori       r3, r3, Step3DLCode@l
    mr        r4, r29
    callSysFunction OSReport
    lis       r3, NewerDlrel@h
    ori       r3, r3, NewerDlrel@l
    callSysFunction DVDConvertPathToEntrynum
    addi      r4, r1, 8
    callSysFunction DVDFastOpen
    lwz       r3, 0x3C(r1)
    addi      r3, r3, 0x1F
    clrrwi    r3, r3, 5
    li        r4, 0x20
    lwz       r5, 0x24(r31)
    lwz       r5, 0(r5)
    callSysFunction HeapAlloc
    mr        r30, r3
    #-------------------------
    # Step 4: Load relocs
    #-------------------------
    lis       r3, Step4@h
    ori       r3, r3, Step4@l
    callSysFunction OSReport
    addi      r3, r1, 8
    mr        r4, r30
    lwz       r5, 0x3C(r1)
    addi      r5, r5, 0x1F
    clrrwi    r5, r5, 5
    li        r6, 0
    li        r7, 2
    callSysFunction DVDReadPrio
    #-------------------------
    # Step 5: Patch
    #-------------------------
    lis       r3, Step5@h
    ori       r3, r3, Step5@l
    callSysFunction OSReport
    mr        r3, r27
    bl        patch
    #-------------------------
    # Step 6: Link
    #-------------------------
    lis       r3, Step6@h
    ori       r3, r3, Step6@l
    callSysFunction OSReport
    mr        r3, r29
    mr        r4, r30
    bl        dlpatch
    #-------------------------
    # Step 7: Closing Patches & Relocs
    #-------------------------
    lis       r3, Step7@h
    ori       r3, r3, Step7@l
    callSysFunction OSReport
    mr        r3, r27
    lwz       r4, 0x24(r31)
    lwz       r4, 0(r4)
    callSysFunction HeapFree
    mr        r3, r30
    lwz       r4, 0x24(r31)
    lwz       r4, 0(r4)
    callSysFunction HeapFree
    addi      r3, r1, 8
    callSysFunction DVDClose
    #-------------------------
    # Step 8: Return to game
    #-------------------------
    lis       r3, Step8@h
    ori       r3, r3, Step8@l
    callSysFunction OSReport
    b         returnToGame
Panic:
    b Panic

Error_FileLoad:
    lis       r3, sprinf_buffer@h
    ori       r3, r3, sprinf_buffer@l
    lis       r4, ErrorFileMissing@h
    ori       r4, r4, ErrorFileMissing@l
    lis       r5, NewerSystem@h
    ori       r5, r5, NewerSystem@l
    lis       r6, 0x8000
    lbz       r7, 3(r6)
    lbz       r6, 7(r6)
    callSysFunction sprintf
    lis       r3, sprinf_buffer@h
    ori       r3, r3, sprinf_buffer@l
    b         PanicWithMessage

Error_OpenFile:
    lis       r3, ErrorOpenSys@h
    ori       r3, r3, ErrorOpenSys@l
    b         PanicWithMessage

Error_AllocMem:
    lis       r3, ErrorAlloc@h
    ori       r3, r3, ErrorAlloc@l

PanicWithMessage:
    mr r5, r3
    lis r3, OSFatalData@h
    ori r3, r3, OSFatalData@l
    lis r4, OSFatalData@h + 4
    ori r4, r4, OSFatalData@l + 4
    callSysFunction OSFatal

returnToGame:
    li        r3, 1
    lwz       r31, 0x5C(r1)
    lwz       r30, 0x58(r1)
    lwz       r29, 0x54(r1)
    lwz       r0, 0x64(r1)
    mtlr      r0
    addi      r1, r1, 0x60
    blr

InsertRegionIntoFileName: # inserts the region and version into NewerRes/Systemxx_x.bin
    add       r3, r3, r4
    lbz       r5, 0x40(r31)
    stb       r5, 0(r3)
    lbz       r5, 0x41(r31)
    stb       r5, 1(r3)
    stb       r6, 3(r3)
    blr

OSFatalData:
    .long 0xFFFFFFFF
    .long 0x00000000
Step1: .string "1: Loading System file: %s\n"
Step2: .string "2: File successfully opened. %d bytes. Allocating...\n"
Step3: .string "3: Done. Loading dylink code...\n"
Step3DLCode: .string "DLCode @ %p\n"
Step4: .string "4: Done. Loading relocs...\n"
Step5: .string "5: Done. Patching...\n"
Step6: .string "6: Done. Linking...\n"
Step7: .string "7: Done. Closing Relocs & System files...\n"
Step8: .string "8: Done! Hacks loaded!\nWelcome to the world of custom code!\n"
Secret: .string "Hmm... what are you doing here?\nLooking for secrets, huh?\nWell then, congrats for finding this message!\n"
ErrorOpenSys: .string "FATAL ERROR: System file could not be opened."
ErrorAlloc: .string "FATAL ERROR: Failed to allocate memory (%d)."
ErrorFileMissing:
    .ascii "NSMLW\n"
    .ascii "\n"
    .ascii "Something went wrong while installing.\n"
    .ascii "The following file is required, but can't be found on the SD card or USB drive:\n"
    .ascii "/NSMLW%s\n"
    .ascii "\n"
    .ascii "For troubleshooting and installation\n"
    .ascii "instructions, join our discord:\n"
    .ascii "    https://discord.gg/4s72Nnm\n"
    .ascii "\n"
    .ascii "Disc version: %d\n"
    .ascii "Game region: %c"
    .byte 0
sprinf_buffer:
    # ~0x80 bytes

.org 0xC00
patch:
    stwu      r1, -0x20(r1)
    mflr      r0
    stw       r0, 0x24(r1)
    stw       r31, 0x1C(r1)
    stw       r30, 0x18(r1)
    stw       r29, 0x14(r1)
    addi      r31, r3, -4

patch_loop:
    li        r29, 0
    addi      r29, r29, -1
    lwzu      r30, 4(r31)
    cmpw      r30, r29 # eof is marked with 0xFFFFFFFF
    beq       patch_done
    lis       r29, 0x8000
    and.      r29, r29, r30 # address or number (length of patch)?
    beq       set_length_of_patch
    # else, patch a single address
    lwzu      r29, 4(r31)
    stw       r29, 0(r30)
    clrrwi    r3, r30, 5
    dcbst     0, r3
    sync
    icbi      0, r3
    b         patch_next
set_length_of_patch:
    mtctr     r30
    lwzu      r30, 4(r31)
    addi      r30, r30, -4

patch_address_loop:
    lwzu      r29, 4(r31)
    stwu      r29, 4(r30)
    clrrwi    r3, r30, 5
    dcbst     0, r3
    sync
    icbi      0, r3
    bdnz      patch_address_loop

patch_next:
    b         patch_loop

patch_done:
    sync
    isync
    lwz       r29, 0x14(r1)
    lwz       r30, 0x18(r1)
    lwz       r31, 0x1C(r1)
    lwz       r0, 0x24(r1)
    mtlr      r0
    addi      r1, r1, 0x20
    blr

dlpatch: # disassembling this requires knowledge about linking and stuff,
         # which I do not have...
    lwz       r11, 8(r4)
    add       r11, r4, r11
    addi      r4, r4, 0xC
    b         relStartLoop
relLoop:
    lwz       r5, 0(r4)
    lwz       r7, 4(r4)
    addi      r4, r4, 8
    clrlslwi  r6, r5, 8,2
    srwi      r5, r5, 24
    lwzx      r6, r11, r6
    srwi.     r8, r6, 31
    bne       relTargetIsAbsolute
    add       r6, r3, r6

relTargetIsAbsolute:
    srwi.     r8, r7, 31
    bne       relAddrIsAbsolute
    add       r7, r3, r7

relAddrIsAbsolute:
    cmpwi     r5, 1
    beq       p_ADDR32
    cmpwi     r5, 4
    beq       p_ADDR16_LO
    cmpwi     r5, 5
    beq       p_ADDR16_HI
    cmpwi     r5, 6
    beq       p_ADDR16_HA
    cmpwi     r5, 0xA
    beq       p_REL24
    b         relContinueLoop
p_ADDR32:
    stw       r6, 0(r7)
    b         relContinueLoop
p_ADDR16_LO:
    sth       r6, 0(r7)
    b         relContinueLoop
p_ADDR16_HI:
    srwi      r6, r6, 16
    sth       r6, 0(r7)
    b         relContinueLoop
p_ADDR16_HA:
    extrwi    r8, r6, 1,16
    srwi      r6, r6, 16
    add       r6, r6, r8
    sth       r6, 0(r7)
    b         relContinueLoop
p_REL24:
    subf      r9, r7, r6
    clrlwi    r9, r9, 6
    lwz       r8, 0(r7)
    or        r8, r8, r9
    stw       r8, 0(r7)

relContinueLoop:
    dcbst     0, r7
    sync
    icbi      0, r7

relStartLoop:
    cmpw      r4, r11
    blt       relLoop
    sync
    isync
    blr
