/*
 * uart.c
 *
 *  Created on: Jun 17, 2020
 *      Author: Kostia
 */


/* Will use USART2 as logger */
/*
 * PA2 -- USART_TX (will use only TX)
 * PA3 -- USART_RX
 *
 * */

#include "stm32f4xx.h"
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

void UART_Init()
{
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN; //enable clocking USART2
	/* Configure pins for USART2 */
	if (!(RCC->AHB1ENR & RCC_AHB1ENR_GPIOAEN))
	{
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	}

	GPIOA->MODER |= GPIO_MODER_MODE2_1; //PA2 as alternate function
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL2_0 | GPIO_AFRL_AFSEL2_1 | GPIO_AFRL_AFSEL2_2;

	/* USART configure */
	USART2->BRR = 0xEA6; //9600 bps
	USART2->CR1 |= USART_CR1_TE; //Transmitter enable
	USART2->CR1 |= USART_CR1_UE; //enable USART2
}


void UART_SendByte(uint8_t b)
{
	USART2->DR = b;
	while (!(USART2->SR & USART_SR_TXE));
}
