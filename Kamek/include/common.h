#ifndef __KAMEK_COMMON_H
#define __KAMEK_COMMON_H


#define GEKKO

/* Common Number Types */
typedef signed long long s64;
typedef signed int s32;
typedef signed short s16;
typedef signed char s8;

typedef unsigned long long u64;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

typedef float f32;
typedef double f64;

typedef unsigned long ulong;
typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char uchar;

typedef unsigned long size_t;

typedef char* Ptr; // DUMB

typedef int BOOL;
#define TRUE 1
#define FALSE 0

#define NULL 0

/* Structures */
typedef f32 Mtx[3][4];
typedef f32 (*MtxPtr)[4];
typedef f32 Mtx44[4][4];
typedef f32 (*Mtx44Ptr)[4];
typedef f32 ROMtx[4][3];
typedef f32 (*ROMtxPtr)[3];

typedef struct { f32 x, y; } VEC2, Vec2, *Vec2Ptr, Point2d, *Point2dPtr;
typedef struct { f32 x, y, z; } VEC3, Vec, Vec3, *VecPtr, Point3d, *Point3dPtr;
typedef struct { s16 x; s16 y; s16 z; }S16Vec, *S16VecPtr;
typedef struct { f32 x, y, z, w; } Quaternion, *QuaternionPtr, Qtrn, *QtrnPtr;

typedef struct { f32 frame, value, slope; } HermiteKey;

extern "C" const char * strrchr ( const char * str, int character );
extern "C" int strcmp ( const char * str1, const char * str2 );

#include "rvl/mtx.h"


// Stop the auto completion from whining
#ifdef __CLANG
inline void *operator new(unsigned int size, void *ptr) { return ptr; }
float abs(float value);
double abs(double value);
#endif
#ifndef __CLANG
inline void *operator new(size_t size, void *ptr) { return ptr; }

inline float abs(float value) {
	return __fabs(value);
}
inline double abs(double value) {
	return __fabs(value);
}
#endif


struct tree_node {
	tree_node *parent, *child, *prev_sibling, *next_sibling;
	void *obj;
};

/* Virtual Function Helpers */
//#define VF_BEGIN(type, obj, id, vtable_offset) \
//	{ type __VFUNC = (((u32)(obj))+(vtable_offset));
#define VF_BEGIN(type, obj, id, vtable_offset) \
	{ type __VFUNC = ((type*)(*((void**)(((u32)(obj))+(vtable_offset)))))[(id)];

#define VF_CALL __VFUNC

#define VF_END }




/* Common Functions */

void OSReport(const char *format, ...);
int sprintf(char *buffer, const char *format, ...);
int snprintf(char *buffer, size_t buff_size, const char *format, ...);
char *strcat(char *destination, const char *source);
extern "C" void *memcpy(void *dest, const void *src, size_t count);
void *memset(void *ptr, int value, size_t num);
int memcmp(const void *ptr1, const void *ptr2, size_t num);

void *AllocFromGameHeap1(u32 size);
void FreeFromGameHeap1(void *block);

float GetHermiteCurveValue(float current_frame, HermiteKey* keys, unsigned int key_count);

extern "C" void mbstowcs(wchar_t *destination, const char *source, size_t count);

/* Archive */
/*#ifdef REGION_PAL
	#define ARC_TABLE ((*((void**)0x8042A318))+4)
	#define RAW_ARC_TABLE (*((void**)0x8042A318))
#endif

#ifdef REGION_NTSC
	#define ARC_TABLE ((*((void**)0x8042A038))+4)
	#define RAW_ARC_TABLE (*((void**)0x8042A038))
#endif

char *RetrieveFileFromArc(void *table, char *name, char *path);
char *RetrieveFileFromArcAlt(void *table, char *name, char *path);*/

extern void *ArchiveHeap; // PAL 0x8042A72C, NTSC 0x8042A44C

namespace nw4r { namespace math { float FrSqrt(float); }}
float sqrtf(float x) {
    return (x <= 0) ? 0.0f : x * nw4r::math::FrSqrt(x);
}

#ifdef __MWERKS__
	#define InfiniteLoop for (;;) { asm { nop } }
#else
	#define InfiniteLoop for (;;) { asm("nop"); }
#endif

typedef struct
{
    u8   *destp;                         // Write-destination pointer:                     4B
    s32  destCount;                      // Remaining size to write:                     4B
    s32  forceDestCount;                 // Forcibly set the decompression size             4B
    u16  huffTable9 [ 1 << (9 + 1) ];    // Huffman encoding table: 2048B
    u16  huffTable12[ 1 << (5 + 1) ];    // Huffman encoding table: 128B
    u16  *nodep;                         // Node during a Huffman table search: 4B
    s32  tableSize9;                     // Table size during a load: 4B
    s32  tableSize12;                    // Table size during a load: 4B
    u32  tableIdx;                       // Index for the table load position: 4B
    u32  stream;                         // Bit stream for loading: 4B
    u32  stream_len;                     // Number of valid stream bits for loading: 4B
    u16  length;                         // Read length for LZ compression: 2B
    s8   offset_bits;                    // Bit length for offset information: 1B
    u8   headerSize;                     // Size of header being read:             1B
}                                        //                             Total is 2216B
CXUncompContextLH;

extern "C" void CXInitUncompContextLH( CXUncompContextLH * context, void* dest );
extern "C" s32 CXReadUncompLH( CXUncompContextLH *context, const void* data, u32 len );
extern "C" u32 CXGetUncompressedSize( const void *srcp );
extern "C" void CXUncompressLZ( const void *srcp, void *destp );

static inline BOOL CXIsFinishedUncompLH( const CXUncompContextLH *context )
{
    return (context->destCount > 0 || context->headerSize > 0)? FALSE : TRUE;
}

namespace nw4r { namespace db {
    void Exception_Printf_(const char *msg, ...);
	void* sException();
}}

#endif
