#include "device_system.h"
#include "battery.h"
#include "include/device_system.h"
#include <stdint.h>

void Device_System_Init(Device_System_t *dsys) { Battery_Init(); }

uint32_t Device_System_ReadBat(Device_System_t *dsys) {
  return Battery_ReadV();
}