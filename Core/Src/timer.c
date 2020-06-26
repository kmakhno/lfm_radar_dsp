/*
 * timer.c
 *
 *  Created on: Jun 15, 2020
 *      Author: Kostia
 */

#include "stm32f4xx.h"
#include "timer.h"

#define TIM_DEBUG 1

void Timer_Init()
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; //clocking TIM2

#if TIM_DEBUG
	if (!(RCC->AHB1ENR & RCC_AHB1ENR_GPIOAEN))
	{
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	}

	GPIOA->MODER |= GPIO_MODER_MODE1_1; //PA1 as alternate function
	GPIOA->AFR[0] |= 0x10;
#endif /* TIM_DEBUG */

	//TIM2->CR1 |= TIM_CR1_URS; //update request source only counter overflow
	TIM2->CR2 |= TIM_CR2_MMS_1; //The update event is selected as trigger output (TRGO)
	TIM2->CCMR1 |= TIM_CCMR1_OC2M_0 | TIM_CCMR1_OC2M_1; //Toggle - OC1REF toggles when TIMx_CNT=TIMx_CCR1.
#if TIM_DEBUG
	TIM2->CCER |= TIM_CCER_CC2E; //OC2 signal is output on the corresponding output pin
#else
	TIM2->CCER &= ~TIM_CCER_CC2E; //OC2 signal has no effect
#endif /* TIM_DEBUG */
	TIM2->CNT = 0;
	TIM2->PSC = 72-1; //10 microseconds
	TIM2->ARR = 1000-1; //T=1ms

	TIM2->CR1 |= TIM_CR1_CEN; //counter enable
}

void Timer_Start()
{
	TIM2->CR1 |= TIM_CR1_CEN; //counter enable
}
