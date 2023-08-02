#include "../LIBRARY_EXT/Device/Include/stm32l152xe.h"
#include "../PERIPH/GPIO/GPIO.h"
#include "../PERIPH/RCC/RCC.h"
#include "../PERIPH/TIMER/TIMER.h"
#include "../PERIPH/UART/UART.h"
#include "../PERIPH/SPI/SPI.h"

/* Déclaration des variables*/
uint16_t i;
char c='A';

const char greeting[] = {
	"\r\n************************************************"
	"\r\n** Limatech build 0.0.1."__DATE__" "__TIME__" **"
	"\r\n********************************************\r\n"
};
