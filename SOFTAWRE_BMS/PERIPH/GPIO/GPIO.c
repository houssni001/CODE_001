#include "GPIO.h"

/**
 * @brief Configure the GPIO ports.
 *
 * This function initializes the GPIO ports that are used in the project. It sets up the clock for the GPIO ports,
 * configures the GPIO pins such as LD2_Pin, button on PC13, PA2/3 for RS232, and SCK, MISO, MOSI, and CS for SPI communication.
 */
void GPIO_Config(void)
{
	/************************************************
	 *  ... GPIO Ports Clock Enable  :
	 *
	 *  	- Enable GPIOA clock
	 *  	- Enable GPIOB clock
	 ***********************************************/

		RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
		RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
  

	/************************************************
	 *  ... GPIO Configuration For button  :
	 *
	 *		- PA5 as an output.
	 *		- Bit 5 = 0 --> Push-Pull Mode
	 *		- Fast Speed
	 *		- No Pull-up or Pull Down
	 *
	 ***********************************************/

  	  //GPIOA->MODER   |= (1<<10);
  	  //GPIOA->OTYPER  &= ~(1<<5);
  	  //GPIOA->OSPEEDR |= (1<<11);
  	  //GPIOA->PUPDR   &= ~((1<<10) | (1<<11));


	/************************************************
	 *  ... GPIO Configuration For button  :
	 *
	 *		- PA1 as an input
	 *		- PA1 in pull up mode
	 *
	 ***********************************************/

		GPIOC->MODER &= ~(3<<2);
		GPIOC-> PUPDR |= (1<<2);


	/************************************************
	 *  ... GPIO Configuration For RS232 :
	 *
	 *		- Bits (5:4)= 1:0 					--> Alternate Function for Pin PA2
	 *		- Bits (7:6)= 1:0					--> Alternate Function for Pin PA3
	 *		- Bites (11:10:9:8) = 0:1:1:1  		--> AF7 Alternate function for USART2 at Pin PA2
	 *		- Bites (15:14:13:12) = 0:1:1:1 	--> AF7 Alternate function for USART2 at Pin PA3
	 *
	 ***********************************************/

		GPIOA->MODER 	 |=  (2<<4);
		GPIOA->MODER 	 |=  (2<<6);
		GPIOA->AFR[0]  	 |=  (7<<8);
		GPIOA->AFR[0]    |=  (7<<12);


	/************************************************
	 *  ... GPIO Configuration For SPI1 :
	 *
	 *	   	- SCK: 		PA5, push-pull, high speed, no pull, alternate function mode
	 *	   	- MISO: 	PA6, push-pull, high speed, no pull, alternate function mode
	 *	   	- MOSI: 	PA7, push-pull, high speed, no pull, alternate function mode
	 *	   	- CS: 		PA9, push-pull, high speed, pull-up, output mode
	 *
	 ***********************************************/

		GPIOA->MODER |= (2<<10)|(2<<12)|(2<<14)|(1<<18);
		GPIOA->OSPEEDR |= (3<<10)|(3<<12)|(3<<14)|(3<<18);
		GPIOA->PUPDR |= (0<<10)|(0<<12)|(0<<14)|(1<<18);
		GPIOA->AFR[0] |= (5<<20)|(5<<24)|(5<<28);
}
