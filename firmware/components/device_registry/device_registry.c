#include "device_registry.h"

static SemaphoreHandle_t mutex;

esp_err_t DReg_Init(void) {
  mutex = xSemaphoreCreateMutex();
  if (!mutex) {
    return ESP_FAIL;
  }

  Battery_Init();
  Display_Init();
  Sound_Init();

  return ESP_OK;
}

bool DReg_SemCanTake(void) {
  return xSemaphoreTake(mutex, pdMS_TO_TICKS(50)) == pdTRUE;
}

void DReg_SemGive(void) { xSemaphoreGive(mutex); }

//

void DReg_Display_GetPendingCMD(void) {
  xSemaphoreTake(mutex, portMAX_DELAY);
  Display_CMD_Param_t cmd = Display_GetPendingCMD();
  xSemaphoreGive(mutex);
}

void DReg_Display_SetPendingCMD(Display_CMD_Param_t cmd) {
  xSemaphoreTake(mutex, portMAX_DELAY);
  Display_SetPendingCMD(cmd);
  xSemaphoreGive(mutex);
}

//

void DReg_Battery_Get(Battery_t *battery) {
  xSemaphoreTake(mutex, portMAX_DELAY);
  Battery_Get(battery);
  xSemaphoreGive(mutex);
}

void DReg_Battery_Set(float new_voltage, bool is_charging) {
  xSemaphoreTake(mutex, portMAX_DELAY);
  Battery_Set(new_voltage, is_charging);
  xSemaphoreGive(mutex);
}