#ifndef _EXAMPLE_LED_CONROL_H
#define _EXAMPLE_LED_CONROL_H

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

extern xQueueHandle queueInteruptButtons;

void ledTask(void * param); 

#endif