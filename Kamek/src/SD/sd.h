void SDSeek(u32 new_clock_pos);
#define SDReadWriteFT(write, buffer_from, buffer_to) SDReadWrite(write, (void*)buffer_from, (((unsigned int)buffer_to)-((unsigned int)buffer_from)))
void SDReadWrite(bool write, void* buffer, int size);
void SD_SendCMD(u32 sd_fd, u32 cmd, u32 cmd_type, u32 resp_type, u32 arg, u32 buffer, u32 block_count, u32 sector_size);
u32 SD_GetHCRegister(u32 sd_fd, u32 register_offset);
void SD_SetHCRegister(u32 sd_fd, u32 register_offset, u32 value);