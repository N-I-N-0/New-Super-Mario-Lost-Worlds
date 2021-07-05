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
	
		float x, y, size;
		int timer;
		bool drawMe, amISetUp;

		void setMeUp(float, float, bool, float);
		void sizeUpdate();
};

WandDot::WandDot() {
	amISetUp = false;
}

void WandDot::setMeUp(float pX, float pY, bool pDrawMe, float pSize = 1.0f) {
	this->x = pX;
	this->y = pY;
	this->drawMe = pDrawMe;
	this->size = pSize;
	amISetUp = true;
}

void WandDot::sizeUpdate() {
	if(timer >= 60) {
		size -= 0.01;
		if(timer == 120) {
			timer = 0;
			return;
		}
	} else {
		size += 0.01;
		timer++;
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
			lotsOfDots[i][j].setMeUp(1024+4*j, -432+4*i, true); 
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
	GXSetLineWidth(18, GX_TO_ZERO);
	
	GXSetTevColor(GX_TEVREG0, (GXColor){255,255,255,255});
	GXSetTevColor(GX_TEVREG1, (GXColor){0,0,0,255});
	
	nw4r::g3d::Camera cam(GetCameraByID(GetCurrentCameraID()));
	Mtx matrix;
	cam.GetCameraMtx(&matrix);
	GXLoadPosMtxImm(matrix, 0);
	GXSetCurrentMtx(0);
	
	
	u8 r = 0x00;
	u8 g = 0x00;
	u8 b = 0x00;
	u8 a = 0xFF;

	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < wandDotAmount; j++) {

			if(!lotsOfDots[i][j].amISetUp || !lotsOfDots[i][j].drawMe) continue;


			GXBegin(GX_LINES, GX_VTXFMT0, 8);
			float size = lotsOfDots[i][j].size;

			float tlX = lotsOfDots[i][j].x - size / 2;
			float tlY = lotsOfDots[i][j].y + size / 2;
			float trX = lotsOfDots[i][j].x + size / 2;
			float trY = lotsOfDots[i][j].y + size / 2;
			float blX = lotsOfDots[i][j].x - size / 2;
			float blY = lotsOfDots[i][j].y - size / 2;
			float brX = lotsOfDots[i][j].x + size / 2;
			float brY = lotsOfDots[i][j].y - size / 2;
			
			lotsOfDots[i][j].sizeUpdate();

			// Top
			GXPosition3f32(tlX, tlY, 9000.0f);
			GXColor4u8(r,g,b,a);
			GXPosition3f32(trX, trY, 9000.0f);
			GXColor4u8(r,g,b,a);

			// Left
			GXPosition3f32(tlX, tlY, 9000.0f);
			GXColor4u8(r,g,b,a);
			GXPosition3f32(blX, blY, 9000.0f);
			GXColor4u8(r,g,b,a);

			// Right
			GXPosition3f32(trX, trY, 9000.0f);
			GXColor4u8(r,g,b,a);
			GXPosition3f32(brX, brY, 9000.0f);
			GXColor4u8(r,g,b,a);

			// Bottom
			GXPosition3f32(blX, blY, 9000.0f);
			GXColor4u8(r,g,b,a);
			GXPosition3f32(brX, brY, 9000.0f);
			GXColor4u8(r,g,b,a);

			GXEnd();
		}
	}
}