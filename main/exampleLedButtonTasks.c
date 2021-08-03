#include "exampleLedButtonTasks.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "ledsControl.h"
#include "buttonsControl.h"


void buttonTask(void * param){
  while(1){
    xSemaphoreTake(buttonTaskSemaphoreH, portMAX_DELAY);
    vTaskDelay(40 / portTICK_PERIOD_MS);
    //ets_printf("ELO %d\n", pinNumber);
    sendButtonNumber();
  }
}

void ledTask(void * param){
  printf("Start task led\n");
  initLedsFromFileIni();
  initButtonsFromFileIni();
  uint8_t button_number;
  while (1){
    if (xQueueReceive(queueInteruptButtonsH, &button_number, portMAX_DELAY)){
      printf("Dane z buttonu: %d\n", button_number);
      changeStage(button_number);
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}