/**
 * @file main.c
 * @author
 * @version 1.0
 * @date 18/07/2023
 */


#include "main.h"
#include <stdint.h>
#include <stdio.h>

#define US_TO_MS_FACTOR 1000
volatile uint8_t newDataReceived = 0;
volatile uint8_t receivedData[33];
/**
 * \brief Delay for a specified number of microseconds.
 * \param us Number of microseconds to delay.
 */
void Delay_us (uint16_t us);

/**
 * \brief Delay for a specified number of milliseconds.
 * \param ms Number of milliseconds to delay.
 */
void Delay_ms (uint16_t ms);

/**
 * @brief This function Transmits data via SPI.
 * @param data Pointer to data to be transmitted.
 * @param size Size of the data to be transmitted.
 * @return none
 */
//void SPI_Transmit (uint8_t *data, int size);

/**
 * @brief This function receive data via SPI .
 * @param data Pointer to data to be received.
 * @param size Size of the data to be received.
 * @return none
 */
//void SPI_Receive (uint8_t *data, int size);

/**
 * @brief SPI1 interruption handler : SPI1 routine after interrupt on received data on SPI1.
 * @param none.
 */
void SPI1_IRQHandler();

/**
 * @brief Enable CS : Chip Select.
 * @param none.
 */
void CS_Enable (void);

/**
 * @brief Disable CS : Chip Select.
 * @param none.
 */
void CS_Disable (void);

/**
 * @brief Initialize all necessary peripherals.
 */
void Initialise ();



void Delay_us (uint16_t us)
{
	TIM2->CNT = 0;
	while (TIM2->CNT < us);
}

void Delay_ms (uint16_t ms)
{
	for (uint16_t i=0; i<ms; i++)
	{
		Delay_us(500); //DELAY OF 1/2 MS
	}
}

void SPI_Enable (void)
{
	SPI1->CR1 |= (1<<6);   // SPE=1, Peripheral enabled
}

void SPI_Disable (void)
{
	SPI1->CR1 &= ~(1<<6);   // SPE=0, Peripheral Disabled
}


void CS_Enable (void)
{
	GPIOA->BSRR |= (1<<9)<<16; //Slave state low : data are being transmitted
}

void CS_Disable (void)
{
	GPIOA->BSRR |= (1<<9);	//Slave state high : data has been transmitted
}


void Initialise ()
{
   RCC_Config();
   GPIO_Config();
   USART2_UART_Init();
   TIMER_Config();
   SPIConfig();
}


void SPI_Transmit (uint8_t *data, int size)
{
	/************************************************
	 *  ... SPI_Transmit Function is configured as follow:
	 *
	 *  	-Enable SPI1 slave
	 *
	 *  	-While i<size of the buffer to be transmitted :
	 *  		- Wait for TXE bit to be set --> This will indicate that the buffer is empty
	 *  		- Load the data into DR (Data Register)
	 *  		- Increment i;
	 *
	 *  	- Wait for TXE bit to be set --> This will indicate that the buffer is empty
	 *  	- Wait for BSY bit to reset  --> This will indicate that SPI is not busy in communication
	 *
	 *  	- Disable SPI slave
	 *
	 ***********************************************/
		CS_Enable();

		int i=0;

		while (i<size)
		{
			while (!((SPI1->SR)&(1<<1))) {};
			SPI1->DR = data[i];
			i++;
		}

		while (!((SPI1->SR)&(1<<1))) {};
		while (((SPI1->SR)&(1<<7))) {};


	    CS_Disable();
}

void SPI1_IRQHandler(void)
{
	/************************************************
	 *  ... SPI1_IRQHandler Function is configured as follow:
	 *
	 *		- Check if RXNE interrupt has occurred
	 *		- if :
	 *			- Buffer to store received data
	 *			- Call SPI receive function
	 *			- Clear RXNE interrupt flag
	 *			- Set new data received flag
	 *
	 ***********************************************/

	   if ((SPI1->SR & (1 << 0)) != 0)  // Check if RXNE interrupt has occurred
	    {
	        // Read the data from DR (Data Register)
	        receivedData[32] = SPI1->DR;

	        // Set new data received flag
	        newDataReceived = 1;
	    }
}


/**
 * @brief Main program.
 * @return The return will be 0
 */
int main(void) 
{

	/************************************************
	 *  ... main Function is configured as follow:
	 *
	 *  	- Initialize functions(GPIO,RCC,USART2,TIMER)
	 *  	- Write greetings
	 *  	- Enable SPI1
	 *  	- Data to send
	 *  	- Buffer to store received data
	 *  	- While loop :
	 *  		- Transmit Data
	 *  		- check if new data has been received :
	 *  			- Reset new data received flag
	 *  			- write data that has been received using RS232
	 *
	 ***********************************************/
    Initialise ();

    _write(0, greeting, sizeof(greeting));

    SPI_Enable ();

    //uint8_t dataToSend[] = "A";
    uint8_t receivedData[32];

    	while (1)
    	{
    	  //SPI_Transmit (dataToSend, sizeof(dataToSend));

    	  if (newDataReceived)
    	  {

    		  newDataReceived = 0;
    		  _write(1, (char *)receivedData, sizeof(receivedData));
    	  }

    	  //Delay_ms (500);
    	}

        return 0;
}
