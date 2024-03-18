#include "devices.h"

#include "address_map_nios2.h"

// Initalizes the mouse device
void init_keyboard() {
  // Base addr
  volatile int* PS2_MOUSE = (int*)PS2_BASE;
  // Enable interrupt
  *(PS2_MOUSE + 1) = 0x1;

  // Write reset
  *(PS2_MOUSE) = 0xFF;
  return;
}

// Initializes the keyboard device
void init_mouse() {
  // Base addr
  volatile int* PS2_KEYBOARD = (int*)PS2_DUAL_BASE;
  // Enable interrupt
  *(PS2_KEYBOARD + 1) = 0x1;

  // Write reset
  *(PS2_KEYBOARD) = 0xFF;
  return;
}

// Called when interrupt for mouse enabled to get data
// Pass in pointers for device to read and array of bytes
void get_PS2_data(volatile int* PS2_device, unsigned char* byte_arr) {
  // Used to store register values from device
  int PS2_data, RVALID;

  // Set bytes to zero
  *(byte_arr) = 0;
  *(byte_arr + 1) = 0;
  *(byte_arr + 2) = 0;

  // Read 3 bytes from device
  for (int i = 0; i < 3;) {
    // Get data
    PS2_data = *(PS2_device);
    RVALID = *(PS2_device) & 0x8000;

    // If the data read is valid parse it
    if (RVALID) {
      // Shift data into bytes
      *(byte_arr) = *(byte_arr + 1);
      *(byte_arr + 1) = *(byte_arr + 2);
      *(byte_arr + 2) = PS2_data & 0xFF;
      // Increment on valid data read
      i++;
    }
  }

  // Device was just inserted - allow data to be sent
  if ((*(byte_arr + 1) == (char)0xAA) && (*(byte_arr + 2) == (char)0x00)) {
    *(PS2_device) = 0xF4;
  }
}

// Function processes the data read from the mouse
void process_mouse_data(unsigned char* bytes, struct Player* player, struct Cursor* cursor){
    // Check if LMB clicked
    
}
