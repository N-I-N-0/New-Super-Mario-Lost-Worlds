#ifndef __IOS_H
#define __IOS_H

extern int IOS_Open(char*, int);
extern int IOS_Close(int);
extern int IOS_ioctl(int, int, void*, size_t, void*, size_t, int);
extern int IOS_ioctlv(int, int,int,int, void*);

typedef struct _ioctlv {
	void *data;
	u32 len;
} ioctlv;

#endif