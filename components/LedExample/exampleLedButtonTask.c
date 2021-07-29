#include "exampleLedButtonTask.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "ledsControl.h"
#include "buttonsControl.h"

void ledTask(void * param){
  printf("Start task led\n");
  initLedsFromFileIni();
  initButtonsFromFileIni();
  uint8_t button_number;
  while (1){
    if (xQueueReceive(queueInteruptButtons, &button_number, portMAX_DELAY)){
      printf("Dane z buttonu: %d\n", button_number);
      changeStage(button_number);
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}