#define GEKKO

#include <common.h>
#include "rvl/mtx.h"
#include "rvl/GXEnum.h"
#include "rvl/GXStruct.h"
#include "rvl/GXTransform.h"
#include "rvl/GXGeometry.h"
#include "rvl/GXDispList.h"
#include "rvl/GXLighting.h"
#include "rvl/GXTev.h"
#include "rvl/GXTexture.h"
#include "rvl/GXCull.h"
#include "rvl/GXPixel.h"
#include "rvl/GXBump.h"
//#include "rvl/GXVert.h"
#include "rvl/vifuncs.h"

extern u32 Global5758;
extern u8 GlobalEnableFlag;

#define GXPosition3f32(x,y,z) \
	*((volatile float*)0xCC008000) = (x); \
	*((volatile float*)0xCC008000) = (y); \
	*((volatile float*)0xCC008000) = (z);

#define GXColor1u32(x) \
	*((volatile u32*)0xCC008000) = (x);


extern "C" {


void cppGXStart() {
	Mtx44 ortho;
	
	MTXOrtho(ortho, 0, 456, 0, 686, 0.0F, 1.0F);
	GXSetProjection(ortho, GX_ORTHOGRAPHIC);
	
	Mtx identity;
	MTXIdentity(identity);
	
	GXLoadPosMtxImm(identity, GX_PNMTX0);
	
	GXSetNumTevStages(1);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
	GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
	GXSetTevDirect(GX_TEVSTAGE0);
	
	GXSetNumChans(1);
	GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
	
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBX8, 0);
	
	GXSetZMode(GX_TRUE, GX_ALWAYS, GX_TRUE);
}

void cppGXEnd() {
}



float y_positions[] = {
	-1.0f,
	0.0f,
	16.0f,
	32.0f,
};

#define END_AT 80.0f

u32 colours[] = {
	0x00FF00FF,
	0x0000ffff,
	0xff0000ff,
	0xffff00ff,
};

u32 col_5758[] = {
	0x00ff00ff,
	0xffff00ff,
	0x00ffffff,
	0xff00ffff
};

void cppProcessHeap(u32 freeSize, u32 heapSize, int heapID, char *name) {
	// get the info needed
	float y_pos = y_positions[heapID];
	u32 colour = colours[heapID];
	
	// width: 686
	
	
	float left = 0;
	float right = ((heapSize - freeSize) * 686.0f) / heapSize;
	float top = y_pos;
	float bottom = y_pos+16;
	
	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	
	GXPosition3f32(left, top, 0.0);
	GXColor1u32(colour);
	GXPosition3f32(right, top, 0.0);
	GXColor1u32(colour);
	GXPosition3f32(right, bottom, 0.0);
	GXColor1u32(colour);
	GXPosition3f32(left, bottom, 0.0);
	GXColor1u32(colour);
	
	GXEnd();
	
	
	GXBegin(GX_QUADS, GX_VTXFMT0, 16);
	
	for (int i = 0; i < 4; i++) {
		GXPosition3f32(16*i, END_AT, 0.0);
		GXColor1u32(col_5758[i]);
		GXPosition3f32(16*i+16, END_AT, 0.0);
		GXColor1u32(col_5758[i]);
		GXPosition3f32(16*i+16, END_AT+16.0f, 0.0);
		GXColor1u32(col_5758[i]);
		GXPosition3f32(16*i, END_AT+16.0f, 0.0);
		GXColor1u32(col_5758[i]);
	}
	
	GXEnd();
	
	for (int i = 0; i < 4; i++) {
		if (Global5758 & (1 << i)) {
			GXBegin(GX_QUADS, GX_VTXFMT0, 4);
			
			GXPosition3f32(16*i, END_AT+16.0f, 0.0);
			GXColor1u32(0xffffffff);
			GXPosition3f32(16*i+16, END_AT+16.0f, 0.0);
			GXColor1u32(0xffffffff);
			GXPosition3f32(16*i+16, END_AT+24.0f, 0.0);
			GXColor1u32(0xffffffff);
			GXPosition3f32(16*i, END_AT+24.0f, 0.0);
			GXColor1u32(0xffffffff);
			
			GXEnd();
		}
	}
	
	for (int i = 0; i < 8; i++) {
		if (GlobalEnableFlag & (1 << i)) {
			GXBegin(GX_QUADS, GX_VTXFMT0, 4);
			
			GXPosition3f32(16*i, END_AT+32.0f, 0.0);
			GXColor1u32(0xffffffff);
			GXPosition3f32(16*i+16, END_AT+32.0f, 0.0);
			GXColor1u32(0xffffffff);
			GXPosition3f32(16*i+16, END_AT+40.0f, 0.0);
			GXColor1u32(0xffffffff);
			GXPosition3f32(16*i, END_AT+40.0f, 0.0);
			GXColor1u32(0xffffffff);
			
			GXEnd();
		}
	}
}


};
