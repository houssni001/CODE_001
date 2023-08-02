#include "RCC.h"

/**
 * @brief Configure the Reset and clock control (RCC).
 *
 * This function configures the main internal regulator output voltage and initializes the RCC oscillators.
 * It also initializes the CPU, AHB and APB buses clocks.
 */
void RCC_Config(void)
{

	/************************************************
	 *  ... RCC is configured as followed :
	 *
	 *		- Configure the main integral regulator output voltage
	 *
	 *		-- Initializes the RCC Oscillators according to the specified parameters in the RCC_OscInitTypeDef structure --
	 *			- Enable the HSI
	 *			- Wait until the HSI is ready
	 *			- Select the HSI as system clock
	 *			- Calibrate the HSI
	 *
	 *
	 *		 -- Initializes the CPU, AHB and APB buses clocks --
	 *		 	- SYSCLK is not divided
	 *		 	- HCLK (high speed clock) is not divided
	 *		 	- HCLK (low speed clock) is not divided
	 *		 	- Enable the SYSCLK
	 *		 	- Enable power interface clock
	 *		 	- Enable system configuration clock
	 *		 	- Flash Latency
	 *
	 ***********************************************/

    PWR->CR |= PWR_CR_VOS_1;


    RCC->CR      |= RCC_CR_HSION;
    while(!(RCC->CR & RCC_CR_HSIRDY));
    RCC->CFGR    |= RCC_CFGR_SW_HSI;
    RCC->ICSCR   |= RCC_ICSCR_HSICAL;


    RCC->CFGR    |=RCC_CFGR_HPRE_DIV1;
    RCC->CFGR    |=RCC_CFGR_PPRE1_DIV1;
    RCC->CFGR    |=RCC_CFGR_PPRE2_DIV1;
    RCC->CFGR    |=RCC_CFGR_MCOSEL_HSI;
    RCC->APB1ENR |=RCC_APB1ENR_PWREN;
    RCC->APB2ENR |=RCC_APB2ENR_SYSCFGEN;
    FLASH->ACR   |= FLASH_ACR_LATENCY;
}
