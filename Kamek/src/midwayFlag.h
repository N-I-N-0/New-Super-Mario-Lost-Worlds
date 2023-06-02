#include <common.h>
#include <game.h>
#include <dCourse.h>
#include <g3dhax.h>
#include <sfx.h>
#include <stage.h>
#include "boss.h"

class daChukanPoint_c : public dActorState_c {
    private:
        mHeapAllocator_c allocator;
        // u8 field2_0x3ec;
        // u8 field3_0x3ed;
        // u8 field4_0x3ee;
        // u8 field5_0x3ef;
        u8 unk1[4];
        m3d::mdl_c model;
        m3d::anmChr_c animation;
        // u8 field8_0x468;
        // u8 field9_0x469;
        // u8 field10_0x46a;
        // u8 field11_0x46b;
        // u8 field12_0x46c;
        // u8 field13_0x46d;
        // u8 field14_0x46e;
        // u8 field15_0x46f;
        // u8 field16_0x470;
        // u8 field17_0x471;
        // u8 field18_0x472;
        // u8 field19_0x473;
        // u8 field20_0x474;
        // u8 field21_0x475;
        // u8 field22_0x476;
        // u8 field23_0x477;
        u8 unk2[16];
        m3d::anmTexPat_c texpat;
        // u8 field25_0x4a4;
        // u8 field26_0x4a5;
        // u8 field27_0x4a6;
        // u8 field28_0x4a7;
        u8 unk3[4];
        m3d::anmTexSrt_c texsrt;
        int field30_0x4d4;
        float field31_0x4d8;
        // u8 field32_0x4dc;
        // u8 field33_0x4dd;
        // u8 field34_0x4de;
        // u8 field35_0x4df;
        u8 unk4[4];

    public:
        bool isBouncing, enableGravity;
        u8 liquid;
        ActivePhysics::Callback activePhysicsCallback;

        virtual void beginState_Wait();
        virtual void executeState_Wait();
        virtual void endState_Wait();
        virtual void beginState_SeaWait();
        virtual void executeState_SeaWait();
        virtual void endState_SeaWait();

        bool isInLiquid();

        USING_STATES(daChukanPoint_c);
        REF_NINTENDO_STATE(Wait);
        REF_NINTENDO_STATE(SeaWait);
};