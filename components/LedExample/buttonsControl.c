#include "buttonsControl.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
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

int32_t button1_pin;
int32_t button2_pin;
int32_t button3_pin;


void setupButtonsPinsfromFile();
void setupPin(uint8_t pin);


static void IRAM_ATTR gpio_isr_handler(void *args){
    BaseType_t xHigherPriorityTaskWoken;
    xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR( buttonTaskSemaphoreH, &xHigherPriorityTaskWoken );
    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

void sendButtonNumber(){
    int32_t num = 0;
    if(gpio_get_level(button1_pin)){
        num = NUM_BUTTON1;
        xQueueSendFromISR(queueInteruptButtonsH, &num, NULL);
    }else if(gpio_get_level(button2_pin)){
        num = NUM_BUTTON2;
        xQueueSendFromISR(queueInteruptButtonsH, &num, NULL);
    }else if(gpio_get_level(button3_pin)){
        num = NUM_BUTTON3;
        xQueueSendFromISR(queueInteruptButtonsH, &num, NULL);
    } 
}


///////////////////////////////////
// Init funs
///////////////////////////////////
void setupButtonsPinsfromFile(){
    printf("But 1 pin: %d, But 2 pin: %d, But 3 pin: %d\n", button1_pin, button2_pin, button3_pin);
    if(getIntConfigFromFile(BUTTON1_INI_TAG, &button1_pin)){
        button1_pin = DEFAULT_BUTTON1_PIN;
    }
    if(getIntConfigFromFile(BUTTON2_INI_TAG, &button2_pin)){
        button2_pin = DEFAULT_BUTTON2_PIN;
    }
    if(getIntConfigFromFile(BUTTON3_INI_TAG, &button3_pin)){
        button3_pin = DEFAULT_BUTTON3_PIN;
        printf("But 3 pin: %d", button3_pin);
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