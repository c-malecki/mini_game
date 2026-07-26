#ifndef __DEVICE_REGISTRY_H_
#define __DEVICE_REGISTRY_H_

#include "battery.h"
#include "display.h"
#include "sound.h"
#include "esp_err.h"

esp_err_t DR_Init(void);

bool DR_SemCanTake(void);
void DR_SemGive(void);

void DR_Display_GetPendingCMD(void);
void DR_Display_SetPendingCMD(Display_CMD_Param_t cmd);

void DR_Battery_Get(Battery_t *battery);
void DR_Battery_Set(float new_voltage, bool is_charging);

#endif // __DEVICE_REGISTRY_H_