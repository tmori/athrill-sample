#ifndef _SENSOR_DEVICE_H_
#define _SENSOR_DEVICE_H_

#include "types.h"
#include "vdev.h"
#include "device_io.h"

#define DEVICE_SENSOR_ADDR_POSE_X               VDEV_RX_DATA(0)
#define DEVICE_SENSOR_ADDR_POSE_Y               VDEV_RX_DATA(1)
#define DEVICE_SENSOR_ADDR_STEARING             VDEV_RX_DATA(2)
#define DEVICE_SENSOR_ADDR_SPEED                VDEV_RX_DATA(3)
#define DEVICE_SENSOR_ADDR_OBSTACLE_DISTANCE    VDEV_RX_DATA(4)
#define DEVICE_SENSOR_ADDR_OBSTACLE_ANGLE       VDEV_RX_DATA(5)
#define DEVICE_SENSOR_ADDR_LINE_SENSOR          VDEV_RX_DATA(6)

static inline void sensor_device_read(uint32 addr, uint32 *data)
{
    *data = sil_rew_mem((void*)addr);
}


#endif /* _SENSOR_DEVICE_H_ */