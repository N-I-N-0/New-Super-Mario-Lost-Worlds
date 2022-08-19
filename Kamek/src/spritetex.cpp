struct ResColor {
    union {
        struct {
            u8 tevColor[3][20];
            u8 padding1[4];
            u8 tevConstantColor[4][10];
            u8 padding2[24];
        } dl;
        u8 data[128];
    };
};

struct ResMatDLData {
    u8 _0[32];
    ResColor resColor;
    u8 _1[64];
    u8 _2[160];
};

struct ResMat {
    u32 size;
    u32 mdlOffs;
    u32 nameOffs;
    u32 index;
    u32 flags;
    u8 tcgencount;
    u8 chancount;
    u8 tevcount;
    u8 indcount;
    u32 cullmode;
    u8 zcomploc;
    u8 lightsetid;
    u8 fogid;
    u8 padding;
    u32 indtexmtxcalcmethod1;
    u32 indtexmtxcalcmethod2;
    u32 shaderoffs;
    u32 texinfocount;
    u32 texinfooffs;
    u32 furoffs;
    u32 unkoffs;
    ResMatDLData** dloffs;
};

/*typedef struct {
    u8 r, g, b, a;
} GXColor;*/

int getBubbleColor() {
	ResMat* resMat; //use "void * /*ResMat*/ GetResMat(const char *str) const;" here
	ResMatDLData* resMatDLData = (ResMatDLData*)(((u32)(resMat))+((u32)resMat->dloffs));
	GXColor* color = (GXColor*)&resMatDLData[1].resColor.dl.tevColor;
	
	//reference: PALv1@8000d330
	//color->nw4r::g3d::ResMatTevColor::GXGetTevColor(param_2,&local_48);
		//nw4r::g3d::ScnMdl::CopiedMatAccess::__ct((nw4r::g3d::ScnMdl*,unsignedlong))(auStack68,piVar6,uVar5);
		//*color = nw4r::g3d::ScnMdl::CopiedMatAccess::GetResMatTevColor((bool))(auStack68,param_3);
		//local_54 = local_48;
	//color->nw4r::g3d::ResMatTevColor::GXSetTevColor(param_2,&local_54);
		//nw4r::g3d::ResMatTevColor::DCStore((bool))(color,0);
	
	
	dCourse_c *course = dCourseFull_c::instance->get(GetAreaNum());
	int zoneNum = GetZoneNum();
	dCourse_c::zone_s* zone = course->getZoneByID(zoneNum);
	u16 color2 = zone->modelShading;
	
	OSReport("Color: %d\n", color2);
	return 1;
}


