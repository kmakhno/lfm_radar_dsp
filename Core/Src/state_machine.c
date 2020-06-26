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


const struct transition state_table[MAX_STATES][MAX_SIGNALS] =
{
	[IDLE_STATE][TO_IDLE_SIG] = {IDLE_STATE, NULL},
	[IDLE_STATE][START_CONVERSION_SIG] = {START_CONVERSION_STATE, NULL},
	[IDLE_STATE][START_SENDING_SIG] = {IDLE_STATE, NULL},
	[IDLE_STATE][STOP_SENDING_SIG] = {IDLE_STATE, NULL},

	[START_CONVERSION_STATE][TO_IDLE_SIG] = {START_CONVERSION_STATE, NULL},
	[START_CONVERSION_STATE][START_CONVERSION_SIG] = {START_CONVERSION_STATE, NULL},
	[START_CONVERSION_STATE][START_SENDING_SIG] = {START_SENDING_SIG, NULL},
	[START_CONVERSION_STATE][STOP_SENDING_SIG] = {START_CONVERSION_STATE, NULL},

	[START_SENDING_STATE][TO_IDLE_SIG] = {START_SENDING_STATE, NULL},
	[START_SENDING_STATE][START_CONVERSION_SIG] = {START_SENDING_STATE, NULL},
	[START_SENDING_STATE][START_SENDING_SIG] = {START_SENDING_STATE, NULL},
	[START_SENDING_STATE][STOP_SENDING_SIG] = {IDLE_STATE, NULL},
};

enum signals current_signal = TO_IDLE_SIG;
enum states current_state = IDLE_STATE;


void State_Machine()
{
	enum signals new_signal = Get_Signal();
	enum states new_state = state_table[current_state][new_signal].new_state;
	transition_callback worker = state_table[current_state][new_signal].worker;
	if (worker != NULL)
	{
		worker(new_state, new_signal);
	}

	if (current_state != new_state)
	{
		printf("Current state %d -> New state %d | event %d \r\n", current_state, new_state, new_signal);
	}
	current_state = new_state;
}

void Set_Signal(enum signals signal)
{
	current_signal = signal;
}


enum signals Get_Signal()
{
	return current_signal;
}

enum states Get_CurrentState()
{
	return current_state;
}
