#include <game.h>
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
#include "rvl/GXVert.h"
#include "rvl/vifuncs.h"


class WandDot {
	public:
		WandDot();
	
		float x, y;
		u8 size, initialSize, colourIndex;
		int timer;
		bool drawMe, amISetUp, colourMoveRight, visible;

		void setMeUp(float, float, bool, u8, u8);
		void sizeUpdate();
		void colourUpdate();
};

WandDot::WandDot() {
	amISetUp = false;
}

void WandDot::setMeUp(float pX, float pY, bool pDrawMe = false, u8 pColourIndex = 0, u8 pSize = 16) {
	this->x = pX;
	this->y = pY;
	this->drawMe = pDrawMe;
	this->size = pSize;
	this->initialSize = pSize;
	this->colourIndex = pColourIndex % 72;
	this->amISetUp = true;
}

inline void WandDot::sizeUpdate() {
	if(this->timer >= 60) {
		if(this->timer % 5 == 0) this->size -= 1;
		if(this->timer >= 120) {
			this->timer = 0;
			this->size = this->initialSize;
			return;
		}
	} else {
		if(this->timer % 5 == 0) this->size += 1;
	}
	this->timer++;
}

struct colour {
	union {
		u32 all;
		struct { u8 r, g, b, a; };
	};
};

colour rainbowColours[] = {
	0xFF0000FF,
	0xFF1500FF,
	0xFF2A00FF,
	
	0xFF4000FF,
	0xFF5400FF,
	0xFF6A00FF,
	
	0xFF8000FF,
	0xFF9400FF,
	0xFFAA00FF,
	
	0xFFBF00FF,
	0xFFD400FF,
	0xFFE900FF,
	
	0xFFFF00FF,
	0xE9FF00FF,
	0xD4FF00FF,
	
	0xBFFF00FF,
	0xAAFF00FF,
	0x94FF00FF,
	
	0x80FF00FF,
	0x6AFF00FF,
	0x54FF00FF,
	
	0x40FF00FF,
	0x2AFF00FF,
	0x15FF00FF,
	
	0x00FF00FF,
	0x00FF15FF,
	0x00FF2AFF,
	
	0x00FF40FF,
	0x00FF54FF,
	0x00FF6AFF,
	
	0x00FF80FF,
	0x00FF94FF,
	0x00FFAAFF,
	
	0x00FFBFFF,
	0x00FFD4FF,
	0x00FFE9FF,
	
	0x00FFFFFF,
	0x00E9FFFF,
	0x00D4FFFF,
	
	0x00BFFFFF,
	0x00AAFFFF,
	0x0094FFFF,
	
	0x0080FFFF,
	0x006AFFFF,
	0x0054FFFF,
	
	0x0040FFFF,
	0x002AFFFF,
	0x0015FFFF,
	
	0x0000FFFF,
	0x1500FFFF,
	0x2A00FFFF,
	
	0x4000FFFF,
	0x5400FFFF,
	0x6A00FFFF,
	
	0x8000FFFF,
	0x9400FFFF,
	0xAA00FFFF,
	
	0xBF00FFFF,
	0xD400FFFF,
	0xE900FFFF,
	
	0xFF00FFFF,
	0xFF00E9FF,
	0xFF00D4FF,
	
	0xFF00BFFF,
	0xFF00AAFF,
	0xFF0094FF,
	
	0xFF0080FF,
	0xFF006AFF,
	0xFF0054FF,
	
	0xFF0040FF,
	0xFF002AFF,
	0xFF0015FF


/*	0xFF0000FF,
	0xFF4000FF,
	0xFF8000FF,
	0xFFBF00FF,
	0xFFFF00FF,
	0xBFFF00FF,
	0x80FF00FF,
	0x40FF00FF,
	0x00FF00FF,
	0x00FF40FF,
	0x00FF80FF,
	0x00FFBFFF,
	0x00FFFFFF,
	0x00BFFFFF,
	0x0080FFFF,
	0x0040FFFF,
	0x0000FFFF,
	0x4000FFFF,
	0x8000FFFF,
	0xBF00FFFF,
	0xFF00FFFF,
	0xFF00BFFF,
	0xFF0080FF,
	0xFF0040FF*/
};

inline void WandDot::colourUpdate() {
	if(this->timer % 5 == 0) {
		this->colourIndex +=1;
		this->colourIndex %= 72;
	}
}

#define wandDotAmount 10
WandDot lotsOfDots[4][wandDotAmount];


class LevelDrawer : public m3d::proc_c {
	public:
		LevelDrawer();

		bool amISetUp;
		mHeapAllocator_c allocator;

		void setMeUp();
		void drawMe();
		void drawOpa();
		void drawXlu();
};




static LevelDrawer levelDrawerInstance;

int LevelDraw() {
	levelDrawerInstance.drawMe();
	
	APDebugDraw();
	
	return 1;
}


LevelDrawer::LevelDrawer() {
	amISetUp = false;
}

void LevelDrawer::setMeUp() {
	allocator.setup(GameHeaps[0], 0x20);
	setup(&allocator);
	
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < wandDotAmount; j++) {
			lotsOfDots[i][j].setMeUp(1024+4*j, -432+4*i, false, j); 
		}
	}
	
}

void LevelDrawer::drawMe() {
	if (!amISetUp) {
		setMeUp();
		amISetUp = true;
	}
	
	scheduleForDrawing();
}

void LevelDrawer::drawOpa() {
	drawXlu();
}

void LevelDrawer::drawXlu() {
	GXClearVtxDesc();
	
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
	
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
	
	GXSetNumIndStages(0);
	for (int i = 0; i < 0x10; i++)
		GXSetTevDirect((GXTevStageID)i);
	
	GXSetNumChans(1);
	GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
	GXSetChanAmbColor(GX_COLOR0A0, (GXColor){255,255,255,255});
	GXSetChanMatColor(GX_COLOR0A0, (GXColor){255,255,255,255});
	GXSetNumTexGens(0);
	
	GXSetNumTevStages(1);
	GXSetNumIndStages(0);
	
	GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
	
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
	GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
//	GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C1, GX_CC_C0, GX_CC_RASC, GX_CC_ZERO);
//	GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
//	GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_RASA, GX_CA_ZERO);
//	GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
	
	GXSetZCompLoc(GX_FALSE);
	GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
	GXSetZMode(GX_TRUE, GX_ALWAYS, GX_FALSE);
	GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
	
	GXSetFog(GX_FOG_NONE, 0, 0, 0, 0, (GXColor){0,0,0,0});
	GXSetFogRangeAdj(GX_FALSE, 0, 0);
	
	GXSetCullMode(GX_CULL_NONE);
	
	GXSetDither(GX_TRUE);
	//GXSetLineWidth(8, GX_TO_ZERO);
	
	
	GXSetTevColor(GX_TEVREG0, (GXColor){255,255,255,255});
	GXSetTevColor(GX_TEVREG1, (GXColor){0,0,0,255});
	
	nw4r::g3d::Camera cam(GetCameraByID(GetCurrentCameraID()));
	Mtx matrix;
	cam.GetCameraMtx(&matrix);
	GXLoadPosMtxImm(matrix, 0);
	GXSetCurrentMtx(0);
	
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < wandDotAmount; j++) {

			if(!lotsOfDots[i][j].amISetUp || !lotsOfDots[i][j].drawMe) continue;

			GXSetPointSize(lotsOfDots[i][j].size, GX_TO_ZERO);

			lotsOfDots[i][j].sizeUpdate();
			lotsOfDots[i][j].colourUpdate();

			GXBegin(GX_POINTS, GX_VTXFMT0, 1);
			
			GXPosition3f32(lotsOfDots[i][j].x, lotsOfDots[i][j].y, 9000.0f);
			u8 r, g, b, a;
			if(lotsOfDots[i][j].colourMoveRight) {
				r = rainbowColours[72-lotsOfDots[i][j].colourIndex].r;
				g = rainbowColours[72-lotsOfDots[i][j].colourIndex].g;
				b = rainbowColours[72-lotsOfDots[i][j].colourIndex].b;
				a = rainbowColours[72-lotsOfDots[i][j].colourIndex].a;
			} else {
				r = rainbowColours[lotsOfDots[i][j].colourIndex].r;
				g = rainbowColours[lotsOfDots[i][j].colourIndex].g;
				b = rainbowColours[lotsOfDots[i][j].colourIndex].b;
				a = rainbowColours[lotsOfDots[i][j].colourIndex].a;
			}
			
			GXColor4u8(r,g,b,a);

			GXEnd();
		}
	}
	
	GXSetPointSize(16, GX_TO_ZERO);
}