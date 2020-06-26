/*
 * state_machine.h
 *
 *  Created on: 24 июн. 2020 г.
 *      Author: Kostia
 */

#ifndef INC_STATE_MACHINE_H_
#define INC_STATE_MACHINE_H_

enum states
{
	IDLE_STATE = 0,
	START_CONVERSION_STATE,
	START_SENDING_STATE,

	MAX_STATES
};


enum signals
{
	TO_IDLE_SIG = 0,
	START_CONVERSION_SIG,
	START_SENDING_SIG,
	STOP_SENDING_SIG,

	MAX_SIGNALS
};

typedef void (*transition_callback)(enum states state, enum signals signal);

struct transition
{
	enum states new_state;
	transition_callback worker;
};


enum signals Get_Signal(void);
void Set_Signal(enum signals signal);
void State_Machine(void);
enum states Get_CurrentState(void);

#endif /* INC_STATE_MACHINE_H_ */
