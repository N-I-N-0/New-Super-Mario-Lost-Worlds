#ifndef __COLOUR_H
#define __COLOUR_H

struct colour {
	union {
		u32 rgba;
		struct { u8 r, g, b, a; };
	};
};

#endif