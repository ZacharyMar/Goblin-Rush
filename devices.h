#ifndef DEVICES_H
#define DEVICES_H

#include "structs.h"

// Function used to initialize the mouse device
void init_mouse();
// Function used to initalize the keyboard device
void init_keyboard();
// Function used to get data from PS2 devices
void get_PS2_data(volatile int*, unsigned char*);
// Function used to process mouse data
void process_mouse_data(unsigned char*, Player*, Cursor*);
// Function used to process keyboard data
void process_keyboard_data(unsigned char*, Player*);

#endif