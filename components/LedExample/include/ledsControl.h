#ifndef _LEDS_CONROL_H
#define _LEDS_CONROL_H

#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"

#define LOGS_LED_ACTIVE

#define NUMBER_OF_LEDS  3

#define LED1_INI_TAG    "LED1PIN"
#define LED2_INI_TAG    "LED2PIN"
#define LED3_INI_TAG    "LED3PIN"

typedef enum{
    LED1_INI,
    LED2_INI,
    LED3_INI,
}leds_ini_sw;

typedef enum{
    LED_OK,
    LED_CONFIG_PIN_WARNING,
    LED_CHANGE_STAGE,
    LED_INIT_PINS,
    LED_INIT_PIN_NOT_FOUND_IN_FILE,
    LED_NUM_PIN_ERROR,
    LED_CONFIG_TIMERS_ERROR
}led_warnings_log;

typedef enum{
    LED_STAGE_OFF,
    LED_STAGE_ON,
    LED_STAGE_SLOW_FLASHING,
    LED_STAGE_FAST_FLASHING
    //LED_STAGE_WAVING
}led_stage;

typedef struct {
    int32_t led_pin;
    led_stage stage;
    TimerHandle_t timer_handle;
    uint8_t actual_level;
}led_configuration;

led_warnings_log initLedsFromFileIni();

led_warnings_log changeStage(uint8_t num_led);

void warnigsLog(led_warnings_log log, uint8_t num_led);

#endif
