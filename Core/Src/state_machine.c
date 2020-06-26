/*
 * state_machine.c
 *
 *  Created on: 24 июн. 2020 г.
 *      Author: Kostia
 */


#include "state_machine.h"
#include "stm32f4xx.h"
#include "uart.h"
#include <stdio.h>


static enum states current_state = IDLE_STATE;
static enum events current_event = IDLE_EVENT;

void State_Update()
{
	switch (current_state)
	{
		case IDLE_STATE:
			break;

		case PREPARE_TO_CONV_STATE:
			break;

		case START_CONV_STATE:
			break;

		case END_CONV_STATE:
			break;

		case DATA_TRANSFER_STATE:
			break;

		default:
			break;
	}
}
