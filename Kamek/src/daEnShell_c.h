#ifndef __ENSHELL_H
#define __ENSHELL_H

#include <game.h>

class daEnShell_c : public dEn_c {
public:
    u32 _524;
    mAllocator_c m_alloc_subclass;
    nw4r::g3d::ResMdl* resfile;
    m3d::mdl_c shellModel;
    m3d::anmChr_c shellAnm; // maybe not correct but :shrug:
    m3d::anmTexPat_c* texpatPtr;
    m3d::anmTexPat_c texpat;
    ActivePhysics::Info collisionInfo;
    u32 aboveInfo;
    u32 _618;
    u32 _61C;
    u32 belowInfo2;
    u32 _624;
    u32 _628;
    u32 _62C;
    u32 adjacentInfo;
    u32 _634;
    u32 _638;
    u32 _63C;
    u32 belowInfo;
    u32 _644;
    u32 _648;
    u32 _64C;
    mEf::es2 effect;
    u32 _778;
    u32 shellTypeMaybe;
    u32 _780;
    float _784;
    u32 _788;
    float _78C;
    float _790;
    float _794;
    u32 isInTheAir;
    u32 _79C;
    u32 _7A0;
    u32 _7A4;
    u32 _7A8;
    u32 _7AC;
    u32 playerIDHoldingShell;
    u32 _7B4;
    u16 _7B8;
    u16 _7BA;
    u16 _7BC;
    u16 _7BE;
    u16 _7C0;
    u16 _7C2;
    u8 _7C4;
    u8 _7C5;
    u8 _7C6;
    u8 _7C7;
    u32 carriedByPlayerID;
    u32 _7CC;
    float _7D0;
    float _7D4;
    u32 _7D8;

    // dActor_c
    int afterExecute(int);  // 80038670
        ~daEnShell_c();     // 80038080

    // dStageActor_c
    bool isOutOfView();     // 8003A010
    void kill();            // 8003A0C0
    int  _vf74();           // 8003A010
    void itemPickedUp();    // 8003AB80
    void _vf7C();           // 80039610
    void eatIn();           // 8003A570
    bool spitOut(void* yoshi); // 8003A5D0, formerly _vf8C, actually daYoshi_c*

    // dEn_c
    bool stageActorCollision(ActivePhysics *apThis, ActivePhysics *apOther);    // 80038800
    void spriteCollision(ActivePhysics *apThis, ActivePhysics *apOther);        // 80038830
    void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther);        // 800389E0
    void yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther);         // 80039010

    void collidedWithCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther); // 80039B40
    void collidedWithCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther); // 80039BF0
    void collidedWithCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther); // 80039D70
    void collidedWithCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther); // 800393A0
    void collidedWithCat2_Iceball_or_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther); // 8003A7B0

    void dieFall_Begin();   // 8003BE00
    void dieFall_Execute(); // 8003BE80
    void dieFall_End();     // 8003BE70
    void _vf220(void* player); // 80039680, hurtPlayer
    void _vf224(); // 8003BFF0
    void _vf238(); // 8003A8C0, setsAfterIceState
    int _vf248(int something); // 800392C0
    void bouncePlayerWhenJumpedOn(void* player); // 800392F0
    void addScoreWhenHit(void* player); // 80039590
    void _vf25C(void* player); // 8003BF80

    // state is created by daEnCarry_c
    virtual void beginState_Carry(); // 0x8003B020
    virtual void executeState_Carry(); // 0x8003B200
    virtual void endState_Carry(); // 0x8003B140

    // daEnShell_c
    virtual void beginState_Sleep(); // 0x8003AE40
    virtual void executeState_Sleep(); // 0x8003AEE0
    virtual void endState_Sleep(); // 0x8003AEB0
    virtual void beginState_Slide(); // 0x8003B3E0
    virtual void executeState_Slide(); // 0x8003B830
    virtual void endState_Slide(); // 0x8003B6F0
    virtual void beginState_Wakeup(); // 0x8003BB40
    virtual void executeState_Wakeup(); // 0x8003BBB0
    virtual void endState_Wakeup(); // 0x8003BBA0
    virtual void beginState_WakeupReverse(); // 0x8003BD30
    virtual void executeState_WakeupReverse(); // 0x8003BD60
    virtual void endState_WakeupReverse(); // 0x8003BD50
    virtual void beginState_WakeupTurn(); // 0x8003BC30
    virtual void executeState_WakeupTurn(); // 0x8003BC80
    virtual void endState_WakeupTurn(); // 0x8003BC70
    virtual void _vf2C8(); // 0x80039690
    virtual void _vf2CC(); // 0x8003AA60
    virtual void relatedToSpriteCollision(); // 0x800389D0
    virtual void _vf2D4(); // 0x80039000
    virtual void isNotSliding(); // 0x800392B0
    virtual void relatedToYoshiOrPlayerCollision(); // 0x80038FF0
    virtual void _vf2E0(); // 0x8003BD20
    virtual void turnSlightly(); // 0x8003BD10
    virtual void _vf2E8(); // 0x8003A410
    virtual void spawnKickHitEffect(); // 0x80039790
    virtual void _vf2F0(); // 0x8003BE60
    virtual void _vf2F4(); // 0x80038660
    virtual void spawnTailEffect(); // 0x8003BB30
    virtual void _vf2FC(); // 0x8003A210

    void rotateShell(); // 8003A3A0
    void doMovement(); // ???

    USING_STATES(daEnShell_c);
    DECLARE_STATE(ThrowSMW);
    REF_NINTENDO_STATE(Slide);
};

#endif
