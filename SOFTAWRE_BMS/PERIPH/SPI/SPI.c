#include "SPI.h"


void SPIConfig(void)
{
	/************************************************
	 *  ... SPI1 is configured as follow:
	 *
	 *  	- Enable SPI1 Clock
	 *
	 *  	- BaudRate BR[2:0] = fPCLK/256 = 16MHz/256 = 62500 Hz oversampling by 256
	 *  	- Clock phase = 1 	--> Rising edge
	 *  	- Clock Polarity = 1 --> High
	 *  	- DFF = 0 			--> 8 bit data
	 *  	- LSB first
	 *  	-  	--> Hardware slave management
	 *  	- RXONLY = 0 		--> FULL Duplex mode
	 *  	- Master Mode
	 *
	 *  	- Enable RXNE interrupt
	 *
	 *  	- SET SPI interrupt priority
	 *  	- Enable SPI interrupt
	 *
	 ***********************************************/
	
	RCC->APB2ENR = RCC_APB2ENR_SPI1EN;
	
	SPI1->CR1 |= (7<<3);
	SPI1->CR1 |= (1<<1);
	SPI1->CR1 |= (1<<0);
	SPI1->CR1 |= (0<<11);
	SPI1->CR1 |= (1<<7);
	SPI1->CR1 &= ~(1<<8);
	SPI1->CR1 &= ~(1<<2); // Set to slave mode

	SPI1->CR2 |= (1<<6);

	NVIC_SetPriority(SPI1_IRQn,0);
	NVIC_EnableIRQ(SPI1_IRQn);

} 







	

