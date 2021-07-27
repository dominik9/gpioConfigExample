#include <stdio.h>
#include "ledsControl.h"

//Działanie aplikacji:
//Przyciski i ledy są przypisane do odpowiednich numerów
//Jeżeli został przyciśniety przycisk pod numerem jakimś to led zmienia tryb działania
//Tryby działania: Stały, pulsacja, miganie, wyłączenie


void app_main(void)
{
  printf("Hello world!\n");
  initLedsFromFileIni();
  changeStage(3);
}
