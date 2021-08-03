//OK: Refactor all code: variables to pins int32_t, warnings
//TODO: Activate 2 cores without bug
//OK: ADD de-bouncing with semafore
//TODO: Add coments to functions
//OK: Check all includes files
//TODO: ADD error handling

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "exampleLedButtonTasks.h"
#include "buttonsControl.h"

xQueueHandle queueInteruptButtonsH;
xSemaphoreHandle buttonTaskSemaphoreH;

void app_main(void)
{
  printf("Hello world!\n"); 
  queueInteruptButtonsH = xQueueCreate(10, sizeof(uint8_t));
  buttonTaskSemaphoreH = xSemaphoreCreateBinary();
  xTaskCreate(buttonTask, "Button task", 1000, NULL, 1, NULL );
  xTaskCreate(ledTask, "led task", 10048, NULL, 2, NULL);
}
