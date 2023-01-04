#ifndef __GL_POLE_H
#define __GL_POLE_H

#include <game.h>

class daEnGlpole_c : public dEn_c {

public:
    char data_1[8];
    mHeapAllocator_c allocator;
    m3d::mdl_c models[3];
    m3d::anmChr_c animations[2];
    char data_2[313];
    Physics physics;
    ActivePhysics wall_physics;
    char field_0x973;
    ActivePhysics ap2;
    char field_0xa17;
    char data_3[136];

    uint theme_type;
    uint secret_exit_type;
    char field_0xaa8;
    char field_0xaa9;
    char field_0xaaa;
    char field_0xaab;
    char field_0xaac;
    char field_0xaad;
    char field_0xaae;
    char field_0xaaf;
};
#endif