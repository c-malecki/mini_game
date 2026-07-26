#include "device_registry.h"

static SemaphoreHandle_t mutex;

esp_err_t DR_Init(void) {
  mutex = xSemaphoreCreateMutex();
  if (!mutex) {
    return ESP_FAIL;
  }

  Battery_Init();
  Display_Init();
  Sound_Init();

  return ESP_OK;
}

bool DR_SemCanTake(void) {
  return xSemaphoreTake(mutex, pdMS_TO_TICKS(50)) == pdTRUE;
}

void DR_SemGive(void) { xSemaphoreGive(mutex); }

//

void DR_Display_GetPendingCMD(void) {
  xSemaphoreTake(mutex, portMAX_DELAY);
  Display_CMD_Param_t cmd = Display_GetPendingCMD();
  xSemaphoreGive(mutex);
}

void DR_Display_SetPendingCMD(Display_CMD_Param_t cmd) {
  xSemaphoreTake(mutex, portMAX_DELAY);
  Display_SetPendingCMD(cmd);
  xSemaphoreGive(mutex);
}

//

void DR_Battery_Get(Battery_t *battery) {
  xSemaphoreTake(mutex, portMAX_DELAY);
  Battery_Get(battery);
  xSemaphoreGive(mutex);
}

void DR_Battery_Set(float new_voltage, bool is_charging) {
  xSemaphoreTake(mutex, portMAX_DELAY);
  Battery_Set(new_voltage, is_charging);
  xSemaphoreGive(mutex);
}