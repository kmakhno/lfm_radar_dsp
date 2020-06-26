/*
 * dma.c
 *
 *  Created on: 13 июн. 2020 г.
 *      Author: Kostia
 */

#include "stm32f4xx.h"


void DMA_Init()
{
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;

	/* Configure SPI1_RX */
	DMA2_Stream0->CR |= DMA_SxCR_CHSEL_0 | DMA_SxCR_CHSEL_1; //channel 3
	DMA2_Stream0->CR |= DMA_SxCR_PL_0; //medium priority level

	/* Configure SPI1_TX*/
	DMA2_Stream3->CR |= DMA_SxCR_CHSEL_0 | DMA_SxCR_CHSEL_1;
	DMA2_Stream3->CR |= DMA_SxCR_PL_0;

	/* Disable interrupt flags */
	DMA2->LIFCR |= DMA_LIFCR_CTCIF0;
	DMA2->LIFCR |= DMA_LIFCR_CTCIF3;
}
