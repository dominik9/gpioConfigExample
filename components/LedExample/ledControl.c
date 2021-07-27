#include "ledsControl.h"
#include "buttonsControl.h"
#include "configiniread.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOSConfig.h"


#define DEFAULT_LED1_PIN    4
#define DEFAULT_LED2_PIN    16
#define DEFAULT_LED3_PIN    17

#define SLOW_FLASHING_TIMER pdMS_TO_TICKS(1000)
#define FAST_FLASHING_TIMER pdMS_TO_TICKS(400)

led_configuration led1;
led_configuration led2;
led_configuration led3;
led_configuration * led_configuration_tab[3] = {&led1, &led2, &led3};

// LOCAL FUNS

led_warnings_log setupLedPinsFromFile(void);
led_warnings_log setupLed(leds_ini_sw led_num);
led_warnings_log createAllLedTimers(void);
void setupLedGPIO(void);
void ledOff(uint8_t num_led);
void ledOn(uint8_t num_led);
void ledSlowFlashing(uint8_t num_led);
void ledFastFlashing(uint8_t num_led); 
void ledTimerStop(uint8_t num_led);

void ledsTimerCallback(TimerHandle_t xTimer);

led_warnings_log setupLedPinsFromFile(){
    for(uint8_t i = 0; i < NUMBER_OF_LEDS;i++){
        warnigsLog(setupLed(i), i);
    }
    return LED_OK;
}

led_warnings_log setupLed(leds_ini_sw led_num){
    led_warnings_log status = LED_OK;
    switch(led_num){
        case LED1_INI:
            if(getIntConfigFromFile(LED1_INI_TAG, &led1.led_pin)){
                led1.led_pin = DEFAULT_LED1_PIN;
                status = LED_INIT_PIN_NOT_FOUND_IN_FILE;
            }
            //printf("Led 1 pin = %d\n", led1.led_pin);
            led1.stage = 0;
            led1.actual_level = 0;
            break;
        case LED2_INI:
            if(getIntConfigFromFile(LED2_INI_TAG, &led2.led_pin)){
                led2.led_pin = DEFAULT_LED2_PIN;
                status = LED_INIT_PIN_NOT_FOUND_IN_FILE;
            }
            led2.stage = 0;
            led2.actual_level = 0;
            break;
        case LED3_INI:
            if(getIntConfigFromFile(LED3_INI_TAG, &led3.led_pin)){
                led3.led_pin = DEFAULT_LED3_PIN;
                status = LED_INIT_PIN_NOT_FOUND_IN_FILE;
            }
            led3.stage = 0;
            led3.actual_level = 0;
            break;
    }
    
    return status;
}

led_warnings_log createAllLedTimers(void){
    led1.timer_handle = xTimerCreate("timLed1", SLOW_FLASHING_TIMER, true, NULL, ledsTimerCallback);
    led2.timer_handle = xTimerCreate("timLed2", SLOW_FLASHING_TIMER, true, NULL, ledsTimerCallback);
    led3.timer_handle = xTimerCreate("timLed3", SLOW_FLASHING_TIMER, true, NULL, ledsTimerCallback);
    if(led1.timer_handle == NULL || led2.timer_handle == NULL || led3.timer_handle == NULL){
        return LED_CONFIG_TIMERS_ERROR;
    }
    return LED_OK;
}

void setupLedGPIO(void){
    for (uint8_t i = 0; i < NUMBER_OF_LEDS; i++){
        gpio_pad_select_gpio(led_configuration_tab[i]->led_pin);
        gpio_set_direction(led_configuration_tab[i]->led_pin, GPIO_MODE_OUTPUT);
        gpio_set_level(led_configuration_tab[i]->led_pin, 0);
    }
}

void ledOff(uint8_t num_led){
    gpio_set_level(led_configuration_tab[num_led]->led_pin, 0);
}

void ledOn(uint8_t num_led){
    gpio_set_level(led_configuration_tab[num_led]->led_pin, 1);
}

void ledTimerStop( uint8_t num_led){
    xTimerStop(led_configuration_tab[num_led]->timer_handle, 0);
}

void ledsTimerCallback(TimerHandle_t xTimer){
    //printf("Callback led1 state before: %d\n", led1.actual_level);
    if (xTimer == led1.timer_handle){
        if (led1.actual_level){
            gpio_set_level(led1.led_pin, 0); 
            led1.actual_level = 0;
        }else{
            gpio_set_level(led1.led_pin, 1);
            led1.actual_level = 1;
        }
        //printf("Callback led1 state after: %d\n", led1.actual_level);
    }else if(xTimer == led2.timer_handle){
        if (led2.actual_level){
            gpio_set_level(led2.led_pin, 0); 
            led2.actual_level = 0;
        }else{
            gpio_set_level(led2.led_pin, 1);
            led2.actual_level = 1;
        }
    }else if(xTimer == led3.timer_handle){
        if (led3.actual_level){
            gpio_set_level(led3.led_pin, 0); 
            led3.actual_level = 0;
        }else{
            gpio_set_level(led3.led_pin, 1);
            led3.actual_level = 1;
        } 
    }
}

void ledSlowFlashing(uint8_t num_led){
    //Timers start
    xTimerStop(led_configuration_tab[num_led]->timer_handle, 0);
    xTimerChangePeriod(led_configuration_tab[num_led]->timer_handle, SLOW_FLASHING_TIMER, 0);
    xTimerStart(led_configuration_tab[num_led]->timer_handle, 0);
}

void ledFastFlashing(uint8_t num_led){
    xTimerStop(led_configuration_tab[num_led]->timer_handle, 0);
    xTimerChangePeriod(led_configuration_tab[num_led]->timer_handle, FAST_FLASHING_TIMER, 0);
    xTimerStart(led_configuration_tab[num_led]->timer_handle, 0);
} 

led_warnings_log initLedsFromFileIni(){
    setupLedPinsFromFile();
    setupLedGPIO();
    createAllLedTimers();
    return LED_OK;
}


led_warnings_log changeStage(uint8_t num_led){
    if(num_led >= NUMBER_OF_LEDS){
        return LED_NUM_PIN_ERROR;
    }
    switch(led_configuration_tab[num_led]->stage){
        case LED_STAGE_OFF:
            printf("Stage OFF\n");
            ledTimerStop(num_led);
            ledOff(num_led);
            led_configuration_tab[num_led]->stage = LED_STAGE_ON;
            break;
        case LED_STAGE_ON:
            printf("Stage ON\n");
            ledOn(num_led);
            led_configuration_tab[num_led]->stage = LED_STAGE_SLOW_FLASHING;
            break;
        case LED_STAGE_SLOW_FLASHING:
            printf("Stage SLOW\n");
            ledSlowFlashing(num_led);
            led_configuration_tab[num_led]->stage = LED_STAGE_FAST_FLASHING;
            break;
        case LED_STAGE_FAST_FLASHING:
            ledFastFlashing(num_led);
            led_configuration_tab[num_led]->stage = LED_STAGE_OFF;
            break;
        default:
            led_configuration_tab[num_led]->stage = 0;
            break;
    }
    return LED_OK;
}

void warnigsLog(led_warnings_log log, uint8_t num_led){

}
