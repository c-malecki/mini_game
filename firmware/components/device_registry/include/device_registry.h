#ifndef __DEVICE_REGISTRY_H_
#define __DEVICE_REGISTRY_H_

#include "battery.h"
#include "display.h"
#include "sound.h"
#include "esp_err.h"

esp_err_t DReg_Init(void);

bool DReg_SemCanTake(void);
void DReg_SemGive(void);

void DReg_Display_GetPendingCMD(void);
void DReg_Display_SetPendingCMD(Display_CMD_Param_t cmd);

void DReg_Battery_Get(Battery_t *battery);
void DReg_Battery_Set(float new_voltage, bool is_charging);

#endif // __DEVICE_REGISTRY_H_