char* custom_map_animation_strings[] = {
	"\x93\xb9",
	"\x8d\xbb",
	"\x96\xd8",
	"\x83W\x83\x83\x83\x93\x83v",
	"\x82\xcd\x82\xb5\x82\xb2",
	"\x83\x63\x83^",
	"\x8d\xe2",
	"\x95X\x8d\xe2",
	"\x83X\x83\x43\x83\x62\x83`\x83u\x83\x8d\x83\x62\x83N",
	"\x97\xac\x8d\xbb",
	"\x90\xe1",
	"\x95X",
	"\x89_",
	"\x90\x85",
	"\x82\xcd\x82\xb5\x82\xb2\x89\x45",
	"\x82\xcd\x82\xb5\x82\xb2\x8d\xb6",
	"\x82\xcd\x82\xb5\x82\xb2\x8a\xe2",
	"\x82\xcd\x82\xb5\x82\xb2\x93\xea",
	"\x93y",
	//custom animations:
	"swim"
};

u32 custom_map_animation_ids[] = {
    0x02,
	0x0f,
	0x1a,
	0x04,
	0x03,
	0x06,
	0x07,
	0x08,
	0x0e,
	0x10,
	0x11,
	0x12,
	0x13,
	0x14,
	0x16,
	0x15,
	0x18,
	0x19,
	0x17,
	//custom animations:
	0x20,
};

#include <playerAnim.h>
PlayerAnim custom_map_animation_conversions[] = {
	swim, //0x20
};