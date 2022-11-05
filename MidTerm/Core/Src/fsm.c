
#include "fsm.h"
#include "software_timer.h"
#include "button_reading.h"

//state variable of fsm
static enum status state = INIT;

//variable to display on 7segment led
static int counter = 0;

//increase counter by 1, if counter = 9 -> come back 0
void count_up()
{
	counter = (counter+1)%10;
}

//decrease counter by 1, if counter = 0, come to 9
void count_down()
{
	counter = (counter+9)%10;
}

//Three timer is use
//1. blink_led() every second
//2. countdown when long press happen
//3. countdown when turn off
void fsm_simple_buttons_run()
{
	switch(state)
	{
	case INIT:
		//initiate 3 timer for further use
		setTimer1(100);
		setTimer2(100);
		setTimer3(100);

		state = RES;
		break;
	case RES:
		//RESET state:
		//counter = 0 until INC or DEC button is pressed
		counter = 0;
		led_blink();
		display7SEG(counter);

		//turn to PRESS state and count down when DEC is press
		if(is_dec_button_pressed()){
			count_down();
			state = PRESS;
		}

		//turn to PRESS state and count up when INC is pressed
		if(is_inc_button_pressed()){
			count_up();
			state = PRESS;
		}

		//this part is added from step 3 (section 2.4)
		//when no button is pressed -> turn to COUNT_DOWN state
		if(auto_decrease()){
			state = COUNT_DOWN;
		}
		break;
	case PRESS:
		led_blink();
		display7SEG(counter);

		//when a button is pressed, do the function
		//corresponding with this button
		if(is_inc_button_pressed()){
			count_up();
		}
		if(is_dec_button_pressed()){
			count_down();
		}
		if(is_reset_button_pressed()){
			state = RES;
		}

		//this part is added from step 2 (section 2.3)
		//when DEC button is keep pressing more than 3s,
		//count down and turn to LONG_PRESS_DOWN state
		if(is_dec_button_keep_pressed()){
			count_down();
			setTimer2(1000);
			state = LONG_PRESS_DOWN;
		}
		//when INC button is keep pressing more than 3s,
		//count up and turn to LONG_PRESS_UP state
		if(is_inc_button_keep_pressed()){
			count_up();
			setTimer2(1000);
			state = LONG_PRESS_UP;
		}

		//this part is added from step 3 (section 2.4)
		//when no button is pressed -> turn to COUNT_DOWN state
		if(auto_decrease()){
			state = COUNT_DOWN;
		}
		break;
	default:
		break;
	}
}


void fsm_long_press_buttons_run()
{
	switch(state)
	{
	case LONG_PRESS_DOWN:
		led_blink();
		display7SEG(counter);

		//count down every second
		if(timer2_flag == 1){
			setTimer2(1000);
			count_down();
		}

		//when DEC button is release, come back to PRESS state
		if(is_dec_button_released()) state = PRESS;

		break;
	case LONG_PRESS_UP:
		led_blink();
		display7SEG(counter);

		//count up every second
		if(timer2_flag == 1){
			setTimer2(1000);
			count_up();
		}

		//when INC button is released, turn back to PRESS state
		if(is_inc_button_released()) state = PRESS;

	default:
		break;
	}
}


void fsm_time_out()
{
	switch(state)
	{
	case COUNT_DOWN:
		led_blink();
		display7SEG(counter);

		//auto countdown every second
		//when count to 0, turn OFF
		if(counter <= 0) state = OFF;
		if(timer3_flag == 1){
			counter--;
			setTimer3(1000);
		}

		//when counting down, if a button is pressed,
		//come back to its active state
		if(is_dec_button_pressed()){
			count_down();
			state = PRESS;
		}
		if(is_inc_button_pressed()){
			count_up();
			state = PRESS;
		}
		if(is_reset_button_pressed()){
			state = RES;
		}
		break;
	case OFF:
		//turn led off
		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
		//turn 7seg led off
		display7SEG(-1);

		//if either DEC or INC button is pressed,
		//turn to RESET state and active again
		if(is_dec_button_pressed() || is_inc_button_pressed()){
			state = RES;
		}
		break;
	default:
		break;
	}
}





//display num on 7seg led
//a to g equal to SEG0 to SEG6
void display7SEG(int num)
{
	if(num==0 || num==2 || num==3 || num==5 || num==6
			|| num==7 || num==8 || num==9){
		HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, GPIO_PIN_RESET);
	}
	else HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, GPIO_PIN_SET);

	if(num==0 || num==1 || num==2 || num==3 || num==4
		|| num==7 || num==8 || num==9){
		HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, GPIO_PIN_RESET);
	}
	else HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, GPIO_PIN_SET);

	if(num==0 || num==1 || num==3 || num==4 || num==5 || num==6
			|| num==7 || num==8 || num==9){
		HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, GPIO_PIN_RESET);
	}
	else HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, GPIO_PIN_SET);

	if(num==0 || num==2 || num==3 || num==5 || num==6 || num==8 ||num==9){
		HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, GPIO_PIN_RESET);
	}
	else HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, GPIO_PIN_SET);

	if(num==0 || num==2 || num==6 || num==8){
		HAL_GPIO_WritePin(e_GPIO_Port, e_Pin, GPIO_PIN_RESET);
	}
	else HAL_GPIO_WritePin(e_GPIO_Port, e_Pin, GPIO_PIN_SET);

	if(num==0 || num==4 || num==5 || num==6 || num==8 ||num==9){
		HAL_GPIO_WritePin(f_GPIO_Port, f_Pin, GPIO_PIN_RESET);
	}
	else HAL_GPIO_WritePin(f_GPIO_Port, f_Pin, GPIO_PIN_SET);

	if(num==2 || num==3 || num==4 || num==5 || num==6 || num==8 || num==9){
		HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, GPIO_PIN_RESET);
	}
	else HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, GPIO_PIN_SET);

}

//using timer1 to blink single led every second
void led_blink()
{
	if(timer1_flag == 1){
		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
		setTimer1(1000);
	}
}
