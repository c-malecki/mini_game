#ifndef __DEVICE_SYSTEM_H_
#define __DEVICE_SYSTEM_H_

#include <stdint.h>

typedef struct
{
} Device_System_t;

void Device_System_Init(Device_System_t *dsys);
uint32_t Device_System_ReadBat(Device_System_t *dsys);

#endif // __DEVICE_SYSTEM_H_