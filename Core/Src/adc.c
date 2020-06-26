/*
 * adc.c
 *
 *  Created on: 15 июн. 2020 г.
 *      Author: Kostia
 */

#include "adc.h"


void ADC_Init()
{
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; //enable clocking ADC1

	if (!(RCC->AHB1ENR & RCC_AHB1ENR_GPIOAEN))
	{
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; //enable clocking PORTA
	}

	GPIOA->MODER |= GPIO_MODER_MODE0_0 | GPIO_MODER_MODE0_1; //analog mode

	/*Configure external trigger PC11*/
	if (!(RCC->APB2ENR & RCC_APB2ENR_SYSCFGEN))
	{
		RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	}

	if (!(RCC->AHB1ENR & RCC_AHB1ENR_GPIOCEN))
	{
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	}
#if DEBUG_EXTI
	if (!(RCC->AHB1ENR & RCC_AHB1ENR_GPIODEN))
	{
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	}
	GPIOD->MODER |= GPIO_MODER_MODE14_0;
#endif
	GPIOC->MODER &= ~(GPIO_MODER_MODE11_0 | GPIO_MODER_MODE11_1); //PA11 as input
	SYSCFG->EXTICR[2] = SYSCFG_EXTICR3_EXTI11_PC; //PC11 is source input for EXTI11
	EXTI->FTSR |= EXTI_FTSR_TR11; //Falling trigger enabled for input line.
	EXTI->PR = EXTI_PR_PR11; //clear interrupt flag
	EXTI->IMR |= EXTI_IMR_IM11; //Interrupt request from line 11 is not masked !!!!
	NVIC_EnableIRQ(EXTI15_10_IRQn);

	/* ADC configure */
	ADC1->CR1 |= ADC_RES_8b; //8-bit (11 ADCCLK cycles)
	ADC1->CR2 |= ADC_CR2_EXTEN_1; //Trigger detection on the falling edge
	//ADC1->CR2 |= ADC_CR2_EXTSEL_3 | ADC_CR2_EXTSEL_2 | ADC_CR2_EXTSEL_1 | ADC_CR2_EXTSEL_0; //EXTI line11 trigger
	ADC1->SQR3 = 0; //channel 0
	//ADC1->CR2 |= ADC_CR2_EOCS; //The EOC bit is set at the end of each regular conversion. Overrun detection is enabled.
	ADC1->CR2 |= ADC_CR2_DDS; //DMA requests are issued as long as data are converted and DMA=1
	ADC1->CR2 |= ADC_CR2_DMA; //DMA mode enabled
	ADC1->CR2 |= ADC_CR2_CONT; //Continuous conversion mode
	ADC1->CR2 |= ADC_CR2_ADON; //enable ADC
}

void ADC_DMA_Init(uint8_t *buff, uint32_t len)
{
	DMA2_Stream4->CR &= ~DMA_SxCR_EN; //disable DMA

	/* Configure DMA */
	DMA2_Stream4->CR |= DMA_SxCR_PL_1; //priority level - high
	DMA2_Stream4->CR |= DMA_SxCR_MINC; //Memory address pointer is incremented after each data transfer
	DMA2_Stream4->CR |= DMA_SxCR_CIRC; //circular mode enable
	DMA2_Stream4->NDTR = len;
	DMA2_Stream4->PAR  = (uint32_t)&ADC1->DR;
	DMA2_Stream4->M0AR = (uint32_t)buff;
	DMA2->HIFCR |= DMA_HIFCR_CTCIF4; //clear interrupt flag
	NVIC_EnableIRQ(DMA2_Stream4_IRQn);
}

void ADC_DMA_Enable()
{
	DMA2_Stream4->CR |= DMA_SxCR_TCIE; //enable transfer complete interrupt
	DMA2_Stream4->CR |= DMA_SxCR_EN; //enable DMA
}


void ADC_DMA_Disable()
{
	DMA2_Stream4->CR &= ~DMA_SxCR_TCIE; //disable transfer complete interrupt
	DMA2_Stream4->CR &= ~DMA_SxCR_EN; //disable DMA
}


