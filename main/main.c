#include <stdio.h>
#include "ledsControl.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "exampleLedButtonTask.h"
#include "buttonsControl.h"
#include "driver/gpio.h"
//Działanie aplikacji:
//Przyciski i ledy są przypisane do odpowiednich numerów
//Jeżeli został przyciśniety przycisk pod numerem jakimś to led zmienia tryb działania
//Tryby działania: Stały, pulsacja, miganie, wyłączenie
xQueueHandle queueInteruptButtons;

void app_main(void)
{
  printf("Hello world!\n");
  queueInteruptButtons = xQueueCreate(10, sizeof(uint8_t));
  xTaskCreate(&ledTask, "led task", 10048, NULL, 2, NULL);
}
