//Tak jak led control
//Przerwania
//Extern jako kolejka

#ifndef _BUTTONS_CONROL_H
#define _BUTTONS_CONROL_H

#include <stdint.h>

#define NUMBER_OF_BUTTONS  3

#define BUTTON1_INI_TAG    "BUTTON1PIN"
#define BUTTON2_INI_TAG    "BUTTON2PIN"
#define BUTTON3_INI_TAG    "BUTTON3PIN"


void initButtonsFromFileIni();

#endif