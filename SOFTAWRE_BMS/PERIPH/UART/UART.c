#include "UART.h"

/**
 * @brief Initialize the USART2 UART.
 *
 * This function initializes the USART2 peripheral for UART communication.
 * It sets up the data format, parity, stop bits, baud rate, enables the receiver and transmitter,
 * enables the interrupt for data reception, and finally enables the USART2 peripheral.
 */
void USART2_UART_Init(void)
{

	/************************************************
	 *  ... USART2 is configured as follow:
	 *
	 *		-Enable USART2 Clock
	 *
	 *		-Clear CR1 (Control Register 1) for USART2
	 *		-M == 1 			--> 9 data bits
	 *		-PCE == 1 			--> Parity Enabled
	 *		-PS == 1 			--> Odd Parity
	 *		-Stop Bit == 10 	--> 2 bit Stop
	 *		-Baud rate == 115200, PCLK1 at 45MHz
	 *		-RE = 1 			--> Enable the receiver
	 *		-TE = 1 			--> Enable the transmitter
	 *		-Enable Interruption on data reception
	 *
	 *		-Enable IRQ Handler
	 *
	 *		-UE == 1 			--> Enable USART1
	 *
	 ***********************************************/
    RCC->APB1ENR|=RCC_APB1ENR_USART2EN;
    
    USART2->CR1  = 0x00;
    USART2->CR1 |= (1<<12);
    USART2->CR1 |= (1<<10);
    USART2->CR1 |= (1<<9);
    USART2->CR2 |= (2<<12);
    USART2->BRR |= (11<<0) | (8<<4);
    USART2->CR1 |= (1<<2);
    USART2->CR1 |= (1<<3);
    USART2->CR1 |= USART_CR1_RXNEIE;
    
    NVIC_EnableIRQ(USART2_IRQn);
    
    USART2->CR1 |= (1<<13);
}

/**
 * @brief Standard C library compatible output function for USART2.
 *
 * This function sends data through USART2 for UART communication.
 *
 * @param fd File descriptor (not used in this function).
 * @param s Pointer to the data to be sent.
 * @param len Length of the data to be sent.
 */
void _write(int fd, const char *s, int len)
{
    (void)fd;
    while (len--) {
        USART2->DR = *s++;
        while (!(USART2->SR & (1<<6)));
    }
}
