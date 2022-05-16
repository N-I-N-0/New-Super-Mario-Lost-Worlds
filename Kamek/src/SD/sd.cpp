#include "game.h"
#include "SD/sd.h"
extern int IOS_Open(char*, int);
extern void IOS_Close(int);
extern void IOS_ioctl(int, int, void*, size_t, void*, size_t, int);
extern void IOS_ioctlv(int, int,int,int, void*);

typedef struct _ioctlv
{
	void *data;
	u32 len;
} ioctlv;
u32 SD_BLOCK_POS = 0;
void SDSeek(u32 new_clock_pos){
	SD_BLOCK_POS = new_clock_pos;
}
void SDReadWrite(bool write, void* buffer, int size){
	int fd = IOS_Open("/dev/sdio/slot0", 0);
	u32 res_buffer;
	IOS_ioctl( fd, 0x0B, 0, 0, &res_buffer, 4, 0);
	//OSReport("SD ready? %08X\n", buffer);
	if ((res_buffer & 0x00010001) != 0x00010001){
		OSReport("SD not ready\n");
		IOS_Close(fd);
		return;
	}
	OSReport("SD ready\n");
	IOS_ioctl( fd, 4, 0, 0, &res_buffer, 4, 0);
	OSReport("SD reset; RCA and stuff: %08X\n", res_buffer);
	SD_SendCMD(fd, 7, 3, 2, res_buffer&0xFFFF0000, 0, 0, 0);
	OSReport("SD selected\n");
	SD_SendCMD(fd, 0x10, 3, 1, 0x200, 0, 0, 0);
	OSReport("SD block set to 0x200\n");
	SD_SendCMD(fd, 0x37, 3, 1, res_buffer&0xFFFF0000, 0, 0, 0);
	SD_SendCMD(fd, 6, 3, 1, 2, 0, 0, 0);
	SD_SetHCRegister(fd, 0x28, (SD_GetHCRegister(fd, 0x28)&2)|2);
	OSReport("SD bus width to 4\n");
	u32 clock = 1;
	IOS_ioctl(fd, 6, &clock, 4, 0, 0, 0);
	OSReport("SD clock set\n");
	u32 bufferPTR = (u32)buffer;
	while (size > 0){
		ioctlv data[3];
		u32 replyBuff[4];
		{
			u32 InputBuffer[] = {write?0x19:0x12, 3, 1, (SD_BLOCK_POS++) * 0x200, 1, 0x200, bufferPTR, 1, 0};
			data[0].data = InputBuffer;
			data[0].len = 0x24;
			
			data[1].data = (void* )bufferPTR;
			data[1].len = 0x200;
			
			data[2].data = replyBuff;
			data[2].len = 0x10;
			
			bufferPTR += 0x200;
			size -= 0x200;
		}
		IOS_ioctlv(fd, 7, 2, 1, data);
	}
	IOS_Close(fd);
}
u32 SD_GetHCRegister(u32 sd_fd, u32 register_offset){
	
	u32 inbuf[5];
	inbuf[0] = register_offset;
	inbuf[3] = 1;
	inbuf[4] = 0;
	u32 reg;
	IOS_ioctl( sd_fd, 2, inbuf, 0x18, &reg, 4, 0);
	return reg;
}
void SD_SetHCRegister(u32 sd_fd, u32 register_offset, u32 value){
	
	u32 inbuf[5];
	inbuf[0] = register_offset;
	inbuf[3] = 1;
	inbuf[4] = value;
	IOS_ioctl( sd_fd, 1, inbuf, 0x18, 0, 0, 0);
}

void SD_SendCMD(u32 sd_fd, u32 cmd, u32 cmd_type, u32 resp_type, u32 arg, u32 buffer, u32 block_count, u32 sector_size){

	u32 inbuf[9];

	inbuf[0]=cmd;
	inbuf[1]=cmd_type;
	inbuf[2]=resp_type;
	inbuf[3]=arg;
	inbuf[4]=block_count;
	inbuf[5]=sector_size;
	inbuf[6]=buffer;
	inbuf[7]=0; // ?
	inbuf[8]=0; // ?

	u32 outbuf[4];
	IOS_ioctl( sd_fd, 7, inbuf, 0x24, outbuf, 0x10, 0);
}