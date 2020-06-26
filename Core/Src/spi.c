/*
 * spi.c
 *
 *  Created on: Jun 11, 2020
 *      Author: Kostiantyn
 */


#include "stm32f4xx.h"
#include "string.h"
#include "spi.h"

#define BUFF_LENGTH 1024

static uint8_t transferComplete = 0;

void SPI_Init()
{

	/* Enable clocking SPI1 and GPIOA*/
	if (!(RCC->APB2ENR & RCC_APB2ENR_SPI1EN))
	{
		RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	}

	if (!(RCC->AHB1ENR & RCC_AHB1ENR_GPIOAEN))
	{
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	}

	/* Configure GPIOA*/
	GPIOA->MODER |= GPIO_MODER_MODE5_1; //PA5 as alternate function
	GPIOA->MODER |= GPIO_MODER_MODE6_1; //PA6 as alternate function
	GPIOA->MODER |= GPIO_MODER_MODE7_1; //PA7 as alternate function
	GPIOA->MODER &= ~(GPIO_MODER_MODE4_1 | GPIO_MODER_MODE4_0); //PA4 as input
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR5_1; //set output speed to high
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR6_1;
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR7_1;
	GPIOA->AFR[0] |= 0x55500000;

	/* Configure SPI1 as slave */
	SPI1->CR1 &= ~SPI_CR1_DFF; //8 bit data size
	SPI1->CR1 &= ~SPI_CR1_SSM; //software slave management disable, hardware CS
	SPI1->CR1 &= ~SPI_CR1_SSI; //internal salve select disable
	SPI1->CR2 |= SPI_CR2_TXDMAEN;
	SPI1->CR2 |= SPI_CR2_RXDMAEN;
	SPI1->CR1 &= ~SPI_CR1_MSTR; //slave mode
	SPI1->CR1 |= SPI_CR1_SPE; //enable SPI
}


void SPI_Write(uint8_t *buff, uint32_t len)
{
	static uint8_t _rx_dummy;

	transferComplete = 1;
	DMA2_Stream3->CR &= ~DMA_SxCR_EN; //disable DMA SPI_TX
	DMA2_Stream0->CR &= ~DMA_SxCR_EN; //disable DMA SPI_RX

	DMA2_Stream3->CR |= DMA_SxCR_MINC; //memory increment mode
	DMA2_Stream3->CR |= DMA_SxCR_DIR_0; //memory to peripheral
	/* Disable interrupt flags */
	DMA2->LIFCR |= DMA_LIFCR_CTCIF3;
	DMA2_Stream3->NDTR = len; //number of bytes
	DMA2_Stream3->PAR = (uint32_t)&SPI1->DR; //peripheral address(destination)
	DMA2_Stream3->M0AR = (uint32_t)buff; //memory address(source)
	NVIC_EnableIRQ(DMA2_Stream3_IRQn);
	DMA2_Stream3->CR |= DMA_SxCR_TCIE; //enable transfer complete interrupt

	/*Configure dummy receiving*/
	DMA2_Stream0->NDTR = len; //number of bytes
	DMA2_Stream0->PAR = (uint32_t)&SPI1->DR; //peripheral address(destination)
	DMA2_Stream0->M0AR = (uint32_t)&_rx_dummy; //memory address(source)

	DMA2_Stream3->CR |= DMA_SxCR_EN; //enable DMA SPI_TX
	DMA2_Stream0->CR |= DMA_SxCR_EN; //enable DMA SPI_RX
}

void SPI_ReadWrite(uint8_t *o_buff, uint8_t *i_buff, uint32_t len)
{
	transferComplete = 1;
	DMA2_Stream0->CR &= ~DMA_SxCR_EN; //disable DMA SPI_RX
	DMA2_Stream3->CR &= ~DMA_SxCR_EN; //disable DMA SPI_TX
	DMA2_Stream0->CR |= DMA_SxCR_MINC; //memory increment mode
	/* Disable interrupt flags */
	DMA2->LIFCR |= DMA_LIFCR_CTCIF0;
	DMA2_Stream0->NDTR = len; //number of bytes
	DMA2_Stream0->PAR = (uint32_t)&SPI1->DR; //peripheral address(destination)
	DMA2_Stream0->M0AR = (uint32_t)i_buff; //memory address(source)
	NVIC_EnableIRQ(DMA2_Stream0_IRQn);
	DMA2_Stream0->CR |= DMA_SxCR_TCIE; //enable transfer complete interrupt

	/* Configure transmitting data*/
	DMA2_Stream3->CR |= DMA_SxCR_MINC; //memory increment mode
	DMA2_Stream3->CR |= DMA_SxCR_DIR_0; //memory to peripheral
	DMA2_Stream3->NDTR = len; //number of bytes
	DMA2_Stream3->PAR = (uint32_t)&SPI1->DR; //peripheral address(destination)
	DMA2_Stream3->M0AR = (uint32_t)o_buff; //memory address(source)

	DMA2_Stream0->CR |= DMA_SxCR_EN; //enable DMA SPI_RX
	DMA2_Stream3->CR |= DMA_SxCR_EN; //enable DMA SPI_TX
}

void SPI_Read(uint8_t *buff, uint32_t len)
{
	static uint8_t _tx_dummy = 0xAA;

	transferComplete = 1;
	DMA2_Stream0->CR &= ~DMA_SxCR_EN; //disable DMA SPI_RX
	DMA2_Stream3->CR &= ~DMA_SxCR_EN; //disable DMA SPI_TX
	DMA2_Stream0->CR |= DMA_SxCR_MINC; //memory increment mode
	/* Disable interrupt flags */
	DMA2->LIFCR |= DMA_LIFCR_CTCIF0;
	DMA2_Stream0->NDTR = len; //number of bytes
	DMA2_Stream0->PAR = (uint32_t)&SPI1->DR; //peripheral address(destination)
	DMA2_Stream0->M0AR = (uint32_t)buff; //memory address(source)
	DMA2_Stream0->CR |= DMA_SxCR_TCIE; //enable transfer complete interrupt
	NVIC_EnableIRQ(DMA2_Stream0_IRQn);

	/* Configure transmitting data*/
	DMA2_Stream3->CR |= DMA_SxCR_DIR_0; //memory to peripheral
	DMA2_Stream3->NDTR = len; //number of bytes
	DMA2_Stream3->PAR = (uint32_t)&SPI1->DR; //peripheral address(destination)
	DMA2_Stream3->M0AR = (uint32_t)&_tx_dummy; //memory address(source)

	DMA2_Stream0->CR |= DMA_SxCR_EN; //enable DMA SPI_RX
	DMA2_Stream3->CR |= DMA_SxCR_EN; //enable DMA SPI_TX
}

/*void DMA2_Stream0_IRQHandler()
{
	if (DMA2->LISR & DMA_LISR_TCIF0)
	{
		transferComplete = 0;
		DMA2->LIFCR |= DMA_LIFCR_CTCIF0;
		DMA2_Stream0->CR &= ~DMA_SxCR_EN; //disable DMA
	}
}

void DMA2_Stream3_IRQHandler()
{
	if (DMA2->LISR & DMA_LISR_TCIF3)
	{
		transferComplete = 0;
		SPI1->CR2 &= ~SPI_CR2_TXDMAEN;
		DMA2_Stream3->CR &= ~DMA_SxCR_EN; //disable DMA
	}
}*/

void SPI_Wait()
{
	while(transferComplete);
}
