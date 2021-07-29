#include "buttonsControl.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "exampleLedButtonTask.h"
#include "configiniread.h"
#include "driver/gpio.h"
#include "esp_intr_alloc.h"
#include "esp_log.h"

#define DEFAULT_BUTTON1_PIN    26
#define DEFAULT_BUTTON2_PIN    25
#define DEFAULT_BUTTON3_PIN    14

#define NUM_BUTTON1     0
#define NUM_BUTTON2     1
#define NUM_BUTTON3     2

uint8_t button1_pin;
uint8_t button2_pin;
uint8_t button3_pin;


void setupButtonsPinsfromFile();
void setupPin(uint8_t pin);


static void IRAM_ATTR gpio_isr_handler(void *args){
    int pinNumber = (int)args;
    ets_printf("ELO %d\n", pinNumber);
    uint8_t num = 0;
    if(pinNumber == button1_pin){
        num = NUM_BUTTON1;
        xQueueSendFromISR(queueInteruptButtons, &num, NULL);
    }else if(pinNumber == button2_pin){
        num = NUM_BUTTON2;
        xQueueSendFromISR(queueInteruptButtons, &num, NULL);
    }else if(pinNumber == button3_pin){
        num = NUM_BUTTON3;
        xQueueSendFromISR(queueInteruptButtons, &num, NULL);
    } 
}

void setupButtonsPinsfromFile(){
    if(getIntConfigFromFile(BUTTON1_INI_TAG, &button1_pin)){
        button1_pin = DEFAULT_BUTTON1_PIN;
        //printf("PIN 1 Config\n");
    }
    if(getIntConfigFromFile(BUTTON2_INI_TAG, &button2_pin)){
        button2_pin = DEFAULT_BUTTON2_PIN;
    }
    if(getIntConfigFromFile(BUTTON3_INI_TAG, &button3_pin)){
        button3_pin = DEFAULT_BUTTON3_PIN;
    }
    printf("But 1 pin: %d, But 2 pin: %d, But 3 pin: %d\n", button1_pin, button2_pin, button3_pin);
}

void setupPin(uint8_t pin){
    gpio_pad_select_gpio(pin);
    gpio_set_direction(pin, GPIO_MODE_INPUT);
    gpio_pulldown_en(pin);
    gpio_pullup_dis(pin);
    gpio_set_intr_type(pin, GPIO_INTR_POSEDGE);
    
    gpio_isr_handler_add(pin, gpio_isr_handler, (void*)pin);
}

void setupInputsAndISR(){
    gpio_install_isr_service(0);
    setupPin(button1_pin);
    setupPin(button2_pin);
    setupPin(button3_pin);
}

void initButtonsFromFileIni(){
    setupButtonsPinsfromFile();
    setupInputsAndISR();
}