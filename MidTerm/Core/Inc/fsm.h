
#ifndef INC_FSM_H_
#define INC_FSM_H_

#include "main.h"

enum status{
	INIT = 0,
	RES = 1,
	PRESS = 2,
	LONG_PRESS_UP = 3,
	LONG_PRESS_DOWN = 4,
	COUNT_DOWN = 5,
	OFF = 6
};

void fsm_simple_buttons_run();
void fsm_long_press_buttons_run();
void fsm_time_out();

void display7SEG(int);
void led_blink();

#endif /* INC_FSM_H_ */
