#include <game.h>
// #include <sdk/gx/GXEnum.h>
// #include <sdk/gx/GXGeometry.h>
// #include <sdk/gx/GXLighting.h>
// #include <sdk/gx/GXStruct.h>
// #include <sdk/gx/GXTev.h>
// #include <sdk/gx/GXVert.h>

class APDebugDrawer : public m3d::proc_c {
	public:
		APDebugDrawer();

		// bool amISetUp;
		// mHeapAllocator_c allocator;

		// void setMeUp();

		// void drawMe();

		void drawOpa();
		void drawXlu();
};


// Drawing helper functions
void DrawQuad(float tlX, float tlY, float trX, float trY, float blX, float blY, float brX, float brY, float z, u8 r, u8 g, u8 b, u8 a, u8 addDiagonal) {

    // Setup drawing
    GXBegin(GX_LINES, GX_VTXFMT0, 8 + (addDiagonal > 0) * 2);

    // Draw top
    GXPosition3f32(tlX, tlY, z);
    GXColor4u8(r,g,b,a);
    GXPosition3f32(trX, trY, z);
    GXColor4u8(r,g,b,a);

    // Draw left
    GXPosition3f32(tlX, tlY, z);
    GXColor4u8(r,g,b,a);
    GXPosition3f32(blX, blY, z);
    GXColor4u8(r,g,b,a);

    // Draw right
    GXPosition3f32(trX, trY, z);
    GXColor4u8(r,g,b,a);
    GXPosition3f32(brX, brY, z);
    GXColor4u8(r,g,b,a);

    // Draw bottom
    GXPosition3f32(blX, blY, z);
    GXColor4u8(r,g,b,a);
    GXPosition3f32(brX, brY, z);
    GXColor4u8(r,g,b,a);

    // Draw diagonal if enabled
    if (addDiagonal == 1) {
        GXPosition3f32(trX, trY, z);
        GXColor4u8(r,g,b,a);
        GXPosition3f32(blX, blY, z);
        GXColor4u8(r,g,b,a);
    }
    else if (addDiagonal == 2) {
        GXPosition3f32(tlX, tlY, z);
        GXColor4u8(r,g,b,a);
        GXPosition3f32(brX, brY, z);
        GXColor4u8(r,g,b,a);
    }

    GXEnd();
}

// Credits to Ismy for helping me with this one
void DrawCircle(float centreX, float centreY, float radiusX, float radiusY, float z, u8 r, u8 g, u8 b, u8 a) {

    // Define a few variables
    const int numVert = 64;
    const float step = 256.0f/numVert;
    float sin, cos, xDist, yDist;

    // Initialize the prev variables
    float prevSin = 0.0f;
    float prevCos = 1.0f;
    float prevXDist = radiusX;
    float prevYDist = 0.0f;

    // Begin drawing
    GXBegin(GX_LINES, GX_VTXFMT0, numVert * 2);

    // Draw each line
    for (int i = 1; i <= numVert / 4; i++) {

        // Calculate sin and cos for the current angle
        nw4r::math::SinCosFIdx(&sin, &cos, step * i);

        // Calculate the distances from the center
        xDist = radiusX * cos;
        yDist = radiusY * sin;

        // Draw on the first quadrant
        GXPosition3f32(centreX + prevXDist, centreY + prevYDist, z);
        GXColor4u8(r,g,b,a);
        GXPosition3f32(centreX + xDist, centreY + yDist, z);
        GXColor4u8(r,g,b,a);

        // Draw on the second quadrant
        GXPosition3f32(centreX - prevYDist, centreY + prevXDist, z);
        GXColor4u8(r,g,b,a);
        GXPosition3f32(centreX - yDist, centreY + xDist, z);
        GXColor4u8(r,g,b,a);

        // Draw on the third quadrant
        GXPosition3f32(centreX - prevXDist, centreY - prevYDist, z);
        GXColor4u8(r,g,b,a);
        GXPosition3f32(centreX - xDist, centreY - yDist, z);
        GXColor4u8(r,g,b,a);

        // Draw on the fourth quadrant
        GXPosition3f32(centreX + prevYDist, centreY - prevXDist, z);
        GXColor4u8(r,g,b,a);
        GXPosition3f32(centreX + yDist, centreY - xDist, z);
        GXColor4u8(r,g,b,a);

        // Override the "previous" values
        prevSin = sin;
        prevCos = cos;
        prevXDist = xDist;
        prevYDist = yDist;
    }

    // End drawing
    GXEnd();
}


static APDebugDrawer defaultInstance;
bool enableDebugMode = false;

int APDebugDraw() {
	if (enableDebugMode)
		defaultInstance.scheduleForDrawing();
	return 1;
}


APDebugDrawer::APDebugDrawer() {
	// amISetUp = false;
	this->setup(NULL, NULL);
}

// void APDebugDrawer::setMeUp() {
// 	allocator.setup(GameHeaps[0], 0x20);
// 	setup(&allocator);
// }

// void APDebugDrawer::drawMe() {
// 	if (!amISetUp) {
// 		setMeUp();
// 		amISetUp = true;
// 	}

// 	scheduleForDrawing();
// }

void APDebugDrawer::drawOpa() {
	// drawXlu();
}
void APDebugDrawer::drawXlu() {
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

    ActivePhysics* currCc = ActivePhysics::globalListHead;
    while (currCc) {

        // Make sure the actor isn't dead and that its owner exists
        if (currCc->isDead != 2 && currCc->owner != NULL) {

            u32 uptr = (u32)currCc;
            u8 r = (uptr >> 16) & 0xFF;
            u8 g = (uptr >> 8) & 0xFF;
            u8 b = uptr & 0xFF;
            u8 a = 0xFF;

            float centreX = currCc->getCenterPosX();
            float centreY = currCc->getCenterPosY();
            float edgeDistX = currCc->info.xDistToEdge;
            float edgeDistY = currCc->info.yDistToEdge;
            u8 collType = currCc->collisionType;

            // Call DrawCircle for circles
            if (collType == ccCollType::Circle)
                DrawCircle(centreX, centreY, edgeDistX, edgeDistY, 9000.0f, r, g, b, a);

            // Else call DrawQuad
            else {
                float tlX, tlY, trX, trY, blX, blY, brX, brY;
                bool addDiagonal = true;

                // Use trapezoidDist for Y coordinates if collType is 2
                // Else edge distance
                if (collType == ccCollType::TrapezoidUD) {
                    tlY = centreY + currCc->trapezoidDist1;
                    trY = centreY + currCc->trapezoidDist3;
                    blY = centreY + currCc->trapezoidDist2;
                    brY = centreY + currCc->trapezoidDist4;
                    addDiagonal = false;
                } else {
                    tlY = centreY + edgeDistY;
                    trY = centreY + edgeDistY;
                    blY = centreY - edgeDistY;
                    brY = centreY - edgeDistY;
                }

                // Use trapezoidDist for X coordinates if collType is 3
                // Else edge distance
                if (collType == ccCollType::TrapezoidLR) {
                    tlX = centreX + currCc->trapezoidDist1;
                    trX = centreX + currCc->trapezoidDist2;
                    blX = centreX + currCc->trapezoidDist3;
                    brX = centreX + currCc->trapezoidDist4;
                    addDiagonal = false;
                } else {
                    tlX = centreX - edgeDistX;
                    trX = centreX + edgeDistX;
                    blX = centreX - edgeDistX;
                    brX = centreX + edgeDistX;
                }

                // Draw the quad
                DrawQuad(tlX, tlY, trX, trY, blX, blY, brX, brY, 9000.0f, r, g, b, a, addDiagonal);
            }
        }

        currCc = currCc->listPrev;
    }

	Physics *currBgCtr = Physics::globalListHead;

	while (currBgCtr) {
		u32 uptr = (u32)currBgCtr;
		u8 r = (uptr>>16)&0xFF;
		u8 g = (uptr>>8)&0xFF;
		u8 b = uptr&0xFF;
		u8 a = 0xFF;

        // If round, draw a circle
        if (currBgCtr->isRound)
            DrawCircle(currBgCtr->lastPos.x, currBgCtr->lastPos.y, currBgCtr->radius, currBgCtr->radius, 9000.0f, r, g, b, a);

        // Else draw a quad
        else {
            float tlX = currBgCtr->calcedPos[0].x;
            float tlY = currBgCtr->calcedPos[0].y;
            float trX = currBgCtr->calcedPos[3].x;
            float trY = currBgCtr->calcedPos[3].y;
            float blX = currBgCtr->calcedPos[1].x;
            float blY = currBgCtr->calcedPos[1].y;
            float brX = currBgCtr->calcedPos[2].x;
            float brY = currBgCtr->calcedPos[2].y;
            DrawQuad(tlX, tlY, trX, trY, blX, blY, brX, brY, 9000.0f, r, g, b, a, 2);
        }

		currBgCtr = currBgCtr->next;
	}


	// Basic Colliders
	BasicCollider *bc = BasicCollider::globalListHead;
	while (bc) {
		u32 uptr = (u32)bc;
		u8 r = (uptr>>16)&0xFF;
		u8 g = (uptr>>8)&0xFF;
		u8 b = uptr&0xFF;
		u8 a = 0xFF;

		switch (bc->type) {
			case 0: case 2:
    			GXBegin(GX_LINES, GX_VTXFMT0, 2);
				GXPosition3f32(bc->leftX, bc->leftY, 9000.0f);
				GXColor4u8(r,g,b,a);
				GXPosition3f32(bc->rightX, bc->rightY, 9000.0f);
				GXColor4u8(r,g,b,a);
				GXEnd();
				break;
		}

		bc = bc->next;
	}


	// Now, the hardest one.. CollisionMgr_c!
	fBase_c *fb = 0;
	while ((fb = fBase_c::searchByBaseType(2, fb))) {
		u8 *testMe = ((u8*)fb) + 0x1EC;
		if (*((u32*)testMe) != 0x8030F6D0)
			continue;

		u32 uptr = (u32)fb;
		u8 r = u8((uptr>>16)&0xFF)+0x20;
		u8 g = u8((uptr>>8)&0xFF)-0x30;
		u8 b = u8(uptr&0xFF)+0x80;
		u8 a = 0xFF;

		dStageActor_c *ac = (dStageActor_c*)fb;

		sensorBase_s *sensors[4] = {
			ac->collMgr.pBelowInfo, ac->collMgr.pAboveInfo,
			ac->collMgr.pAdjacentInfo, ac->collMgr.pAdjacentInfo};

		for (int i = 0; i < 4; i++) {
			sensorBase_s *s = sensors[i];
			if (!s)
				continue;

			float mult = (i == 3) ? -1.0f : 1.0f;

			switch (s->flags & SENSOR_TYPE_MASK) {
				case SENSOR_POINT:
    				GXBegin(GX_POINTS, GX_VTXFMT0, 1);
					GXPosition3f32(
							ac->pos.x + (mult * (s->asPoint()->x / 4096.0f)),
							ac->pos.y + (s->asPoint()->y / 4096.0f),
							8005.0f);
					GXColor4u8(r,g,b,a);
					GXEnd();
					break;
				case SENSOR_LINE:
    				GXBegin(GX_LINES, GX_VTXFMT0, 2);
					if (i < 2) {
						GXPosition3f32(
								ac->pos.x + (s->asLine()->lineA / 4096.0f),
								ac->pos.y + (s->asLine()->distanceFromCenter / 4096.0f),
								8005.0f);
						GXColor4u8(r,g,b,a);
						GXPosition3f32(
								ac->pos.x + (s->asLine()->lineB / 4096.0f),
								ac->pos.y + (s->asLine()->distanceFromCenter / 4096.0f),
								8005.0f);
						GXColor4u8(r,g,b,a);
					} else {
						GXPosition3f32(
								ac->pos.x + (mult * (s->asLine()->distanceFromCenter / 4096.0f)),
								ac->pos.y + (s->asLine()->lineA / 4096.0f),
								8005.0f);
						GXColor4u8(r,g,b,a);
						GXPosition3f32(
								ac->pos.x + (mult * (s->asLine()->distanceFromCenter / 4096.0f)),
								ac->pos.y + (s->asLine()->lineB / 4096.0f),
								8005.0f);
						GXColor4u8(r,g,b,a);
					}
					GXEnd();
					break;
			}
		}
	}
}

/*
// Patches to allow tracking instances of dBc_c
static collisionMgr_c* firstBc = NULL;
static collisionMgr_c* lastBc = NULL;

extern "C" {
void addBcToList(collisionMgr_c* self);
void removeBcFromList(collisionMgr_c* self);
}

void addBcToList(collisionMgr_c* self) {

    // If the bc was already initialized, bail
    if (self->next || lastBc == self)
        return;

    // If first is NULL, set this bc as both first and last
    if (firstBc == NULL) {
        firstBc = self;
        lastBc = self;

    // Else set the next field of the previous one, and set this one as last
    } else {
        lastBc->next = self;
        lastBc = self;
    }
}

void removeBcFromList(collisionMgr_c* self) {

    // If no bcs are initialized, bail
    if (!firstBc)
        return;

    // If this bc has no "next" and isn't the last bc it was never initialized, so bail
    if (!self->next && lastBc != self)
        return;

    // If this bc is the only initialized one, set both first and last to NULL
    if (firstBc == lastBc) {
        firstBc = NULL;
        lastBc = NULL;
        return;
    }

    // If this bc is the first one, set the next one as the first one
    if (firstBc == self) {
        firstBc = self->next;
        return;
    }

    // Find the previous bc
    collisionMgr_c* prevBc = firstBc;
    while (prevBc->next != self)
        prevBc = prevBc->next;

    // Remove this bc from the list
    prevBc->next = self->next;

    // If this was the last bc, set the previous one as the new last
    if (lastBc == self)
        lastBc = prevBc;
}
*/