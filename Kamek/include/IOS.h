#ifndef __IOS_H
#define __IOS_H

extern int IOS_Open(char*, int);
extern int IOS_Close(int);
extern int IOS_Ioctl(int, int, void*, size_t, void*, size_t);
extern int IOS_Ioctlv(int, int,int,int, void*);

typedef struct _ioctlv {
	void *data;
	u32 len;
} ioctlv;




#define IPC_HEAP			 -1

#define IPC_OPEN_NONE		  0
#define IPC_OPEN_READ		  1
#define IPC_OPEN_WRITE		  2
#define IPC_OPEN_RW			  (IPC_OPEN_READ|IPC_OPEN_WRITE)

#define IPC_MAXPATH_LEN		 64

#define IPC_OK				  0
#define IPC_EINVAL			 -4
#define IPC_ENOHEAP			 -5
#define IPC_ENOENT			 -6
#define IPC_EQUEUEFULL		 -8
#define IPC_ENOMEM			-22


typedef s32 (*ipccallback)(s32 result,void *usrdata);

s32 IOS_OpenAsync(const char *filepath,u32 mode,ipccallback ipc_cb,void *usrdata);

s32 IOS_CloseAsync(s32 fd,ipccallback ipc_cb,void *usrdata);

s32 IOS_Seek(s32 fd,s32 where,s32 whence);
s32 IOS_SeekAsync(s32 fd,s32 where,s32 whence,ipccallback ipc_cb,void *usrdata);
s32 IOS_Read(s32 fd,void *buf,s32 len);
s32 IOS_ReadAsync(s32 fd,void *buf,s32 len,ipccallback ipc_cb,void *usrdata);
s32 IOS_Write(s32 fd,const void *buf,s32 len);
s32 IOS_WriteAsync(s32 fd,const void *buf,s32 len,ipccallback ipc_cb,void *usrdata);

//s32 IOS_Ioctl(s32 fd,s32 ioctl,void *buffer_in,s32 len_in,void *buffer_io,s32 len_io);
s32 IOS_IoctlAsync(s32 fd,s32 ioctl,void *buffer_in,s32 len_in,void *buffer_io,s32 len_io,ipccallback ipc_cb,void *usrdata);
//s32 IOS_Ioctlv(s32 fd,s32 ioctl,s32 cnt_in,s32 cnt_io,ioctlv *argv);
s32 IOS_IoctlvAsync(s32 fd,s32 ioctl,s32 cnt_in,s32 cnt_io,ioctlv *argv,ipccallback ipc_cb,void *usrdata);

s32 IOS_IoctlvFormat(s32 hId,s32 fd,s32 ioctl,const char *format,...);
s32 IOS_IoctlvFormatAsync(s32 hId,s32 fd,s32 ioctl,ipccallback usr_cb,void *usr_data,const char *format,...);

s32 IOS_IoctlvReboot(s32 fd,s32 ioctl,s32 cnt_in,s32 cnt_io,ioctlv *argv);
s32 IOS_IoctlvRebootBackground(s32 fd,s32 ioctl,s32 cnt_in,s32 cnt_io,ioctlv *argv);


#define _CPU_ISR_Disable( _isr_cookie ) \
	do { \
		u32 _disable_mask = 0; \
		_isr_cookie = 0; \
		__asm__ __volatile__ ( \
			"mfmsr %0\n" \
			"rlwinm %1,%0,0,17,15\n" \
			"mtmsr %1\n" \
			"extrwi %0,%0,1,16" \
			: "=&r" ((_isr_cookie)), "=&r" ((_disable_mask)) \
			: "0" ((_isr_cookie)), "1" ((_disable_mask)) \
			: "memory" \
		); \
	} while (0)

#define _CPU_ISR_Restore( _isr_cookie )  \
	do { \
		u32 _enable_mask = 0; \
		__asm__ __volatile__ ( \
			"cmpwi %0,0\n" \
			"beq 1f\n" \
			"mfmsr %1\n" \
			"ori %1,%1,0x8000\n" \
			"mtmsr %1\n" \
			"1:" \
			: "=r"((_isr_cookie)),"=&r" ((_enable_mask)) \
			: "0"((_isr_cookie)),"1" ((_enable_mask)) \
			: "memory" \
		); \
	} while (0)

#endif


void* IPCGetBufferLo(); //802244b0
int iosCreateHeap(void *heap, u32 length); //80225ec0