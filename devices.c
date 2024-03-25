#include "devices.h"

#include "address_map_nios2.h"
#include "constants.h"
#include "structs.h"

// Initalizes the mouse device
void init_mouse() {
  // Base addr
  volatile int* PS2_MOUSE = (int*)PS2_BASE;

  // Write reset
  *(PS2_MOUSE) = 0xFF;
  return;
}

// Initializes the keyboard device
void init_keyboard() {
  // Base addr
  volatile int* PS2_KEYBOARD = (int*)PS2_DUAL_BASE;

  // Write reset
  *(PS2_KEYBOARD) = 0xFF;
  return;
}

// Called when there is data available to read
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
void process_mouse_data(unsigned char* bytes, Player* player, Cursor* cursor) {
  // Check if LMB clicked
  if (*bytes & 0x1) {
    // Put player into shoot state
    player->state = SHOOTING;
  }

  // Get sign bits for movement
  unsigned char x_sign = *bytes & 0x10;
  unsigned char y_sign = *bytes & 0x20;

  // Get change in mouse position

  // Change in x
  // Positive -> move right
  if (x_sign == 0) {
    // Add dx value to cursor position
    cursor->x_pos += *(bytes+1);
    // Prevent going off screen
    if (cursor->x_pos > SCREEN_WIDTH) cursor->x_pos = SCREEN_WIDTH;
  }
  // Negative -> move left
  else{
    // Subtract dx value to cursor
    cursor->x_pos -= *(bytes+1);
    // Prevent going off screen
    if (cursor->x_pos < 0) cursor->x_pos = 0;
  }

  // Change in y
  // Positive -> move down
  if (y_sign == 0){
    cursor->y_pos += *(bytes+2);
    // Prevent off screen
    if (cursor->y_pos > SCREEN_HEIGHT) cursor->y_pos = SCREEN_HEIGHT;
  }
  else{
    cursor->y_pos -= *(bytes+2);
    // Prevent off screen
    if (cursor->y_pos < 0) cursor->y_pos = 0;
  }
}

// Processes data read from keyboard
void process_keyboard_data(unsigned char* bytes, Player* player) {
  /*********
   * Codes for keys:
   * W -> make: 1D, break: F0, 1D
   * A -> make: 1C, break: F0, 1C
   * S -> make: 1B, break: F0, 1B
   * D -> make: 23, break: F0, 23
   * SPACE -> make: 29, break: F0, 29
   */

  // Check byte 0 for key identfier
  // Update movement of player accordingly
  switch (*bytes) {
    // W key
    case (0x1D):
      // Release W key
      if (*(bytes + 1) == 0xF0) player->up = 0;
      // Press W key
      else
        player->up = 1;
      break;
    // A key
    case (0x1C):
      // Release
      if (*(bytes + 1) == 0xF0) player->left = 0;
      // press
      else
        player->left = 1;
      break;
    // S key
    case (0x1B):
      // Release
      if (*(bytes + 1) == 0xF0) player->down = 0;
      // Press
      else
        player->down = 1;
      break;
    // D key
    case (0x23):
      // Release
      if (*(bytes + 1) == 0xF0) player->right = 0;
      // Press
      else
        player->right = 1;
      break;
    // Space
    case (0x29):
      // Put player into evasion mode if cooldown is 0 and player is alive
      if (*(bytes + 1) != 0xF0 && player->ability_cooldown == 0 &&
          player->health > 0) {
        player->state = EVASION;
        player->ability_cooldown = ABILITY_COOLDOWN;
      }
      break;
    // Other key
    default:
      break;
  }
}
