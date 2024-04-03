#include "devices.h"

#include "address_map_nios2.h"
#include "constants.h"
#include "structs.h"

/************** MOUSE + KEYBOARD **********************/

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
  // Add busy wait loop before getting mouse data

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
    } else {
      count++;
    }
    // Device was just inserted - allow data to be sent
    if ((byte1 == (char)0xAA) && (byte2 == (char)0x00)) {
      *(PS2_KEYBOARD) = 0xF4;
    }
  }
}

/***************** TIMER *******************/

// Function used to stop timers
void stop_timer() {
  volatile int* timer_addr = (int*)TIMER_BASE;
  *(timer_addr + 1) = 0x8;
  timer_addr = (int*)TIMER_2_BASE;
  *(timer_addr + 1) = 0x8;
}

// Function used to delay program for specified time in milliseconds
void delay(unsigned int time_delay) {
  // counter = Clock speed * time delay
  unsigned int counter_value = time_delay * CLOCK_SPEED_DIV;

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

// Separate timer used for setting counts
void set_timer(unsigned int time, int timer_addr, bool cont) {
  // Counter = clock speed * time
  unsigned int count = time * CLOCK_SPEED_DIV;

  // Setip timer
  volatile int* addr = (int*)timer_addr;
  *(addr + 1) = 0x8;
  *(addr + 2) = count & 0xFFFF;
  *(addr + 3) = (count >> 16) & 0xFFFF;

  // Start timer
  if (cont)
    *(addr + 1) = 0x6;
  else
    *(addr + 1) = 0x4;
}

// Polls second timer to see if it is done
bool timer_done(int timer_addr) {
  // Check TO flag
  volatile int* addr = (int*)timer_addr;
  // TO flag raised --> timer done
  if (*addr & 0x1) {
    // reset flag
    *addr = 0x0;
    return true;
  }
  // TO flag not raised
  return false;
}

/*********** HEX DISPLAY ***********/
// Sets the hex display to all zeros
void init_hex() {
  volatile int* hex0_3 = (int*)HEX3_HEX0_BASE;
  volatile int* hex4_5 = (int*)HEX5_HEX4_BASE;

  // Set to zero
  *hex0_3 = 0x0;
  *hex4_5 = 0x0;
}

// // Displays number to hex as BCD
void set_hex(int v) {
  volatile int* hex0_3 = (int*)HEX3_HEX0_BASE;
  volatile int* hex4_5 = (int*)HEX5_HEX4_BASE;

  /** Address mapping
   * HEX0-3:
   * bits 0-6 HEX0
   * bits 8-14 HEX1
   * bits 16-22 HEX2
   * bits 24-30 HEX3
   *
   * HEX4-5:
   * bits 0-6 HEX4
   * bits 8-14 HEX5
   */

  // Code mapping
  unsigned char codes[16] = {HEX0, HEX1, HEX2, HEX3, HEX4, HEX5, HEX6, HEX7,
                             HEX8, HEX9, HEXA, HEXB, HEXC, HEXD, HEXE, HEXF};
  // Used to write to hex display
  unsigned char hex_segs[8] = {0, 0, 0, 0, 0, 0, 0, 0};

  // Display to each hex
  for (int i = 0; i < 6; i++) {
    // Get nibble
    unsigned char nibble = v & 0xF;
    // Get hex display code for value
    unsigned char code = codes[nibble];
    // Store code to display
    hex_segs[i] = code;
    // Shift v to get next nibble
    v = v >> 4;
  }

  // Write to display
  *(hex0_3) = *(int*)(hex_segs);
  *(hex4_5) = *(int*)(hex_segs + 4);
}
