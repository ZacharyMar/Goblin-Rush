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
  // Write enable
  *(PS2_MOUSE) = 0xF4;
  return;
}

// Initializes the keyboard device
void init_keyboard() {
  // Base addr
  volatile int* PS2_KEYBOARD = (int*)PS2_DUAL_BASE;

  // Write reset
  *(PS2_KEYBOARD) = 0xFF;
  // Write enable
  *(PS2_KEYBOARD) = 0xF4;
  return;
}

// Gets mouse data from PS2
MouseData get_mouse_data() {
  // Create mousedata struct
  MouseData mouse_data = {0, 0, 0};

  // Address to mouse
  volatile int* PS2_MOUSE = (int*)PS2_BASE;

  // Used to keep track of bytes read
  unsigned char count = 0;

  // temps to store bytes read from mouse
  unsigned char byte0 = 0;
  unsigned char byte1 = 0;
  unsigned char byte2 = 0;

  // Used to get data register values from PS2
  int PS2_data, RVALID;

  while (1) {
    // Get data register of PS2
    PS2_data = *(PS2_MOUSE);
    // Get bit 15
    RVALID = PS2_data & 0x8000;

    // No valid data on first byte -> ignore
    if (!RVALID && count == 0) return mouse_data;

    // Load data into correct bytes when valid data
    if (RVALID && count == 0) {
      byte0 = PS2_data & 0xFF;
      count++;
    } else if (RVALID && count == 1) {
      byte1 = PS2_data & 0xFF;
      count++;
    } else if (RVALID && count == 2) {
      byte2 = PS2_data & 0xFF;
      count++;
    }

    // All bytes gathered process data
    if (count == 3) {
      // Get sign bits for movement
      unsigned char x_sign = byte0 & 0x10;
      unsigned char y_sign = byte0 & 0x20;
      // Get LMB press
      mouse_data.LMB = byte0 & 0x1;

      // Change in x
      // Positive -> move right
      if (x_sign == 0) {
        mouse_data.dx = byte1;
      }
      // Negative -> move left
      else {
        // Get two's comp
        int sign_extended = 0xFFFFFF00 + byte1;
        mouse_data.dx = sign_extended;
      }

      // Change in y
      // Positive -> move down
      if (y_sign == 0) {
        mouse_data.dy = byte2;
      } else {
        // Two's comp
        int sign_extended = 0xFFFFFF00 + byte2;
        mouse_data.dy = sign_extended;
      }
      // All data processed return
      return mouse_data;
    }

    // Device was just inserted - allow data to be sent
    if ((byte1 == (char)0xAA) && (byte2 == (char)0x00)) {
      *(PS2_MOUSE) = 0xF4;
    }
  }
}

// Function to get keyboard data
KEYS get_keyboard_data() {
  // Used to keep track of bytes
  unsigned char count = 0;

  // Temps to store bytes sent from keyboard
  unsigned char byte0 = 0;
  unsigned char byte1 = 0;
  unsigned char byte2 = 0;

  // Used to store register values from device
  int PS2_data, RVALID;

  // Address to keyboard
  volatile int* PS2_KEYBOARD = (int*)PS2_DUAL_BASE;

  while (1) {
    // Get data register of PS2
    PS2_data = *(PS2_KEYBOARD);
    // Get bit 15
    RVALID = PS2_data & 0x8000;

    // No valid data if first byte is not valid
    if (!RVALID && count == 0) return OTHER;

    // Load data into correct bytes when valid data
    if (RVALID && count == 0) {
      byte0 = PS2_data & 0xFF;
    } else if (RVALID && count == 1) {
      byte1 = PS2_data & 0xFF;
    } else if (RVALID && count == 2) {
      byte2 = PS2_data & 0xFF;
    }

    // All bytes gathered
    if (count == 2) {
      // decode key interacted with
      switch (byte0) {
        // W key
        case (0x1D):
          return W;
        // A key
        case (0x1C):
          return A;
        // S key
        case (0x1B):
          return S;
        // D key
        case (0x23):
          return D;
        // Space
        case (0x29):
          return SPACE;
        // Other key
        default:
          return OTHER;
      }
    }
    else{
        count++;
    } 
    // Device was just inserted - allow data to be sent
    if ((byte1 == (char)0xAA) && (byte2 == (char)0x00)) {
      *(PS2_KEYBOARD) = 0xF4;
    }
  }
}

// Function used to stop timer
void stop_timer() {
  volatile int* timer_addr = (int*)TIMER_BASE;
  *(timer_addr + 1) = 0x8;
}

// Function used to delay program for specified time in milliseconds
void delay(unsigned int time_delay) {
  // counter = Clock speed * time delay
  unsigned int counter_value = time_delay * 100000;

  // Setup timer
  volatile int* timer_addr = (int*)TIMER_BASE;
  *(timer_addr + 2) = counter_value & 0xFFFF;
  *(timer_addr + 3) = (counter_value >> 16) & 0xFFFF;

  // start timer
  *(timer_addr + 1) = 0x4;

  // Get TO flag
  unsigned char TO = *timer_addr & 0x1;
  // Busy wait
  while (!TO) {
    TO = *timer_addr & 0x1;
  }

  // Reset flag
  *timer_addr = 0x0;
}

// Function processes the data read from the mouse
void process_mouse_data(unsigned char* bytes, Player* player, Cursor* cursor) {
  // Check if LMB clicked
  if (*bytes & 0x1 && player->state != EVASION) {
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
    cursor->x_pos += *(bytes + 1);
    // Prevent going off screen
    if (cursor->x_pos > SCREEN_WIDTH) cursor->x_pos = SCREEN_WIDTH;
  }
  // Negative -> move left
  else {
    // Subtract dx value to cursor
    cursor->x_pos -= *(bytes + 1);
    // Prevent going off screen
    if (cursor->x_pos < 0) cursor->x_pos = 0;
  }

  // Change in y
  // Positive -> move down
  if (y_sign == 0) {
    cursor->y_pos += *(bytes + 2);
    // Prevent off screen
    if (cursor->y_pos > SCREEN_HEIGHT) cursor->y_pos = SCREEN_HEIGHT;
  } else {
    cursor->y_pos -= *(bytes + 2);
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
