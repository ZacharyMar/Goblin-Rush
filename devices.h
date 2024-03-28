#ifndef DEVICES_H
#define DEVICES_H

#include "structs.h"

// Function used to initialize the mouse device
void init_mouse();
// Function used to initalize the keyboard device
void init_keyboard();
// Function used to get mouse data
MouseData get_mouse_data();
// Function to get keyboard data
KEYS get_keyboard_data();

// Function used to process mouse data
void process_mouse_data(unsigned char*, Player*, Cursor*);
// Function used to process keyboard data
void process_keyboard_data(unsigned char*, Player*);

// Function used to stop timer
void stop_timer();
// Function used to delay program for specified time in milliseconds
void delay(unsigned int time_delay);

#endif