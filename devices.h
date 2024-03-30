#ifndef DEVICES_H
#define DEVICES_H

#include "structs.h"
#include <stdbool.h>

/*************************
 * Functions used to interface with I/O
*/

/****** MOUSE **************/
// Function used to initialize the mouse device
void init_mouse();

// Function used to get mouse data
MouseData get_mouse_data();

/******** KEYBOARD *******************/
// Function used to initalize the keyboard device
void init_keyboard();

// Function to get keyboard data
KEYS get_keyboard_data();

/******** TIMER ***************/
// Function used to stop both timers
void stop_timer();

// Function used to delay program for specified time in milliseconds
void delay(unsigned int time_delay);

// Sets the timer for specified time (ms) and starts it
void set_timer(unsigned int time);

// Polls timer if it is done counting down
bool timer_done();

#endif