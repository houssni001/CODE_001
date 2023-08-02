#include "TIMER.h"

/**
 * @brief Configure Timer 2 (TIM2).
 *
 * This function enables TIM2 clock, sets the prescaler and auto-reload register values, and enables the counter.
 * It also waits until the Update Interrupt Flag (UIF) is set, indicating that the registers are updated.
 */
void TIMER_Config (void)
{
	/************************************************
	 *  ... TIMER2 is configured as followed :
	 *
	 *		- Enable TIM2 clock
	 *		- 32MHz/32 = 1MHz ~1us delay
	 *		- MAX ARR VALUE
	 *		- Enable the counter
	 *		- UIF: Update Interrupt Flag - this bit is set by hardware when the registers are updated
	 *
	 *
	 ***********************************************/
			RCC->APB1ENR |= (1<<0);
			TIM2->PSC = 32-1;
			TIM2->ARR = 0xffff;
			TIM2->CR1 |= (1<<0);
			while (!(TIM2->SR & (1<<0)));
}
