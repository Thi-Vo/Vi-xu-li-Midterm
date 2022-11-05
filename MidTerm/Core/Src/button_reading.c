
#include "button_reading.h"

//counter using when no button is pressed
//timer interupt is 10ms, so to pass 10 second
//we need to use 1000 tick * 10ms = 10s
static uint16_t counter_for_auto_off = 1000;

//number of button we have
#define N0_OF_BUTTONS 				       3
//timer interrupt duration is 10ms, so to pass 3 second,
//we need to jump to the interrupt service routine 300 time
#define DURATION_FOR_PRESSED	   		   300
#define BUTTON_IS_PRESSED                  GPIO_PIN_RESET
#define BUTTON_IS_RELEASED                 GPIO_PIN_SET

//the buffer that the final result is stored after debouncing
static GPIO_PinState buttonBuffer[N0_OF_BUTTONS];

//we define three buffers for debouncing
static GPIO_PinState debounceButtonBuffer0[N0_OF_BUTTONS];
static GPIO_PinState debounceButtonBuffer1[N0_OF_BUTTONS];
static GPIO_PinState debounceButtonBuffer2[N0_OF_BUTTONS];

//we define flags for button pressed
static uint8_t flagForButtonPress3s[N0_OF_BUTTONS] = {0};
static uint8_t flagForButtonPress[N0_OF_BUTTONS] = {0};

//we define counter for the button is pressed more than 3 second.
static uint16_t counterForButtonPress3s[N0_OF_BUTTONS];

//read 3 button and modify flag buffer when button is pressed
//this function is called in timer interupt
void button_reading(){
	//decrease counter every 10ms
	//if a button is pressed, reset this counter to 1000
	counter_for_auto_off--;

	for(int i = 0; i < N0_OF_BUTTONS; i ++){
		debounceButtonBuffer2[i] = debounceButtonBuffer1[i];
		debounceButtonBuffer1[i] = debounceButtonBuffer0[i];
		//read raw button to buffer0
		if(i == 0){
			debounceButtonBuffer0[i] = HAL_GPIO_ReadPin(RESET_GPIO_Port, RESET_Pin);
		}
		else if(i == 1){
			debounceButtonBuffer0[i] = HAL_GPIO_ReadPin(INC_GPIO_Port, INC_Pin);
		}
		else if(i == 2){
			debounceButtonBuffer0[i] = HAL_GPIO_ReadPin(DEC_GPIO_Port, DEC_Pin);
		}

		//if input is stable: 3 recently input is equal
		if(debounceButtonBuffer0[i] == debounceButtonBuffer1[i] &&
				debounceButtonBuffer1[i] == debounceButtonBuffer2[i]){
			//if input stable at present different from previous
			if(buttonBuffer[i] != debounceButtonBuffer2[i]){
				//update input to buffer
				buttonBuffer[i] = debounceButtonBuffer2[i];
				//if input is pressed
				if(buttonBuffer[i] == BUTTON_IS_PRESSED){
					//turn flag on
					flagForButtonPress[i] = 1;
					//reset counter auto off whenever a button is pressed
					counter_for_auto_off = 1000;
					//set counter long press
					counterForButtonPress3s[i] = DURATION_FOR_PRESSED;
				}
			}
			//if input is stable and no change (present = previous)
			else{
				//decrease counter 3s
				counterForButtonPress3s[i]--;
				//when counter is 0, reset counter
				//and if button is press -> turn flag3s on
				if(counterForButtonPress3s[i] == 0){
					if(buttonBuffer[i] == BUTTON_IS_PRESSED){
						flagForButtonPress3s[i] = 1;

						//reset counter for auto off
						counter_for_auto_off = 1000;
					}
					counterForButtonPress3s[i] = DURATION_FOR_PRESSED;
				}
			}
		}
	}
}

//check if reset_button is pressed and reset its flag
int is_reset_button_pressed()
{
	if(flagForButtonPress[0] == 1){
		flagForButtonPress[0] = 0;
		return 1;
	}
	return 0;
}

//check if INCREASE button is pressed and reset its flag
int is_inc_button_pressed()
{
	if(flagForButtonPress[1] == 1){
		flagForButtonPress[1] = 0;
		return 1;
	}
	return 0;
}

//check if DECREASE button is pressed and reset its flag
int is_dec_button_pressed()
{
	if(flagForButtonPress[2] == 1){
		flagForButtonPress[2] = 0;
		return 1;
	}
	return 0;
}

//check if INC button is pressed more than 3s
//and reset its flag
int is_inc_button_keep_pressed()
{
	if(flagForButtonPress3s[1] == 1){
		flagForButtonPress3s[1] = 0;
		return 1;
	}
	return 0;
}

//check if DEC button is pressed more than 3s
//and reset its flag
int is_dec_button_keep_pressed()
{
	if(flagForButtonPress3s[2] == 1){
		flagForButtonPress3s[2] = 0;
		return 1;
	}
	return 0;
}

//return 1 when INC button is released
int is_inc_button_released()
{
	if(buttonBuffer[1] == GPIO_PIN_SET){
		return 1;
	}
	return 0;
}

//return 1 when DEC button is released
int is_dec_button_released()
{
	if(buttonBuffer[2] == GPIO_PIN_SET){
		return 1;
	}
	return 0;
}
//return 1 when 10s pass and no button is pressed
// 0 otherwise
int auto_decrease()
{
	if(counter_for_auto_off <= 0){
		return 1;
	}
	return 0;
}
