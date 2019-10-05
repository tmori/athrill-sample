#ifndef _ACTUATOR_DEVICE_H_
#define _ACTUATOR_DEVICE_H_

#include "types.h"
#include "vdev.h"
#include "device_io.h"

#define DEVICE_ACTUATOR_ADDR_MOTOR_TORQUE           VDEV_TX_DATA(0)
#define DEVICE_ACTUATOR_ADDR_STEARING               VDEV_TX_DATA(1)
#define DEVICE_ACTUATOR_ADDR_BRAKE                  VDEV_TX_DATA(2)
#define DEVICE_ACTUATOR_ADDR_SYNC                   VDEV_TX_FLAG(0)

static inline void actuator_device_write(uint32 addr, uint32 data)
{
    sil_wrw_mem((void*)addr, data);
    return;
}

static inline void actuator_device_sync(uint32 addr)
{
    sil_wrw_mem((void*)DEVICE_ACTUATOR_ADDR_SYNC, 1U);
    return;
}

#endif /* _ACTUATOR_DEVICE_H_ */