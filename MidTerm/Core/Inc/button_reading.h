
#ifndef INC_BUTTON_READING_H_
#define INC_BUTTON_READING_H_

#include "main.h"

void button_reading();

int is_reset_button_pressed();
int is_inc_button_pressed();
int is_dec_button_pressed();

int is_inc_button_keep_pressed();
int is_dec_button_keep_pressed();
int is_inc_button_released();
int is_dec_button_released();

int auto_decrease();
#endif /* INC_BUTTON_READING_H_ */
