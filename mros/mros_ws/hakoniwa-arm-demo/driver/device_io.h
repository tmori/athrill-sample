#ifndef _DEVICE_IO_H_
#define _DEVICE_IO_H_

#include "types.h"

static inline void sil_wrb_mem(void *addr, uint8 data)
{
	(*(uint8*)addr) = data;
	return;
}
static inline uint8 sil_reb_mem(void *addr)
{
	return (*(uint8*)addr);
}

static inline void sil_wrh_mem(void *addr, uint16 data)
{
	(*(uint16*)addr) = data;
	return;
}
static inline uint16 sil_reh_mem(void *addr)
{
	return (*(uint16*)addr);
}

static inline void sil_wrw_mem(void *addr, uint32 data)
{
	(*(uint32*)addr) = data;
	return;
}
static inline uint32 sil_rew_mem(void *addr)
{
	return (*(uint32*)addr);
}

#endif /* _DEVICE_IO_H_ */