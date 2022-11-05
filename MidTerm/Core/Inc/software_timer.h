
#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

#include "main.h"
#include "button_reading.h"

extern int timer1_flag;
extern int timer2_flag;
extern int timer3_flag;


void timerRun();

void setTimer1(uint32_t duration);
void setTimer2(uint32_t duration);
void setTimer3(uint32_t duration);




#endif /* INC_SOFTWARE_TIMER_H_ */
