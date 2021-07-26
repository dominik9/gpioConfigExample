#include "ledsControl.h"
#include "buttonsControl.h"
#include "configiniread.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"

#define DEFAULT_LED1_PIN    4
#define DEFAULT_LED2_PIN    16
#define DEFAULT_LED3_PIN    17


led_configuration led1;
led_configuration led2;
led_configuration led3;
led_configuration * led_configuration_tab[3] = {&led1, &led2, &led3};

// LOCAL FUNS

led_warnings_log setupLedPinsFromFile(void);
led_warnings_log setupLed(leds_ini_sw led_num);
void createAllLedTimers(void);
void setupLedGPIO(void);
void ledOff(uint8_t num_led);
void ledOn(uint8_t num_led);
void ledSlowFlashing(uint8_t num_led);
void ledFastFlashing(uint8_t num_led); 

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
            break;
        case LED2_INI:
            if(getIntConfigFromFile(LED2_INI_TAG, &led2.led_pin)){
                led2.led_pin = DEFAULT_LED2_PIN;
                status = LED_INIT_PIN_NOT_FOUND_IN_FILE;
            }
            led2.stage = 0;
            break;
        case LED3_INI:
            if(getIntConfigFromFile(LED3_INI_TAG, &led3.led_pin)){
                led3.led_pin = DEFAULT_LED3_PIN;
                status = LED_INIT_PIN_NOT_FOUND_IN_FILE;
            }
            led3.stage = 0;
            break;
    }
    
    return status;
}

void createAllLedTimers(void){

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

void ledsTimerCallback(TimerHandle_t xTimer){

}


void ledSlowFlashing(uint8_t num_led){
    //Timers start
}

void ledFastFlashing(uint8_t num_led){
    //Timers start
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
            ledOff(num_led);
            led_configuration_tab[num_led]->stage = LED_STAGE_ON;
            break;
        case LED_STAGE_ON:
            ledOn(num_led);
            led_configuration_tab[num_led]->stage = LED_STAGE_SLOW_FLASHING;
            break;
        case LED_STAGE_SLOW_FLASHING:
            ledSlowFlashing(num_led);
            led_configuration_tab[num_led]->stage = LED_STAGE_SLOW_FLASHING;
            break;
        case LED_STAGE_FAST_FLASHING:
            ledFastFlashing(num_led);
            break;
        default:
            led_configuration_tab[num_led]->stage = 0;
            break;
    }
    return LED_OK;
}

void warnigsLog(led_warnings_log log, uint8_t num_led){

}
