#include "device_system.h"
#include "battery.h"
#include "include/device_system.h"
#include <stdint.h>

void DSys_Init() { Battery_Init(); }

float DSys_Battery_Read() { return Battery_ReadV(); }