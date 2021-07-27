#include <stdio.h>
#include "ledsControl.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//Działanie aplikacji:
//Przyciski i ledy są przypisane do odpowiednich numerów
//Jeżeli został przyciśniety przycisk pod numerem jakimś to led zmienia tryb działania
//Tryby działania: Stały, pulsacja, miganie, wyłączenie
void ledTask(void * param){
  printf("Start task led\n");
  initLedsFromFileIni();
  changeStage(2);
  while (1){
    changeStage(0);
    changeStage(1);
    changeStage(2);
    vTaskDelay(10000 / portTICK_PERIOD_MS);
  }
}

void app_main(void)
{
  printf("Hello world!\n");
  xTaskCreate(&ledTask, "led task", 40048, "ledTask", 2, NULL);
  
}
