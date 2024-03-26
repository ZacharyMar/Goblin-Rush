/*************
 * Constants
 */
#define ABILITY_COOLDOWN 30
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define BOARD "DE1-SoC"

/* Memory */
#define DDR_BASE 0x40000000
#define DDR_END 0x7FFFFFFF
#define A9_ONCHIP_BASE 0xFFFF0000
#define A9_ONCHIP_END 0xFFFFFFFF
#define SDRAM_BASE 0x00000000
#define SDRAM_END 0x03FFFFFF
#define FPGA_PIXEL_BUF_BASE 0x08000000
#define FPGA_PIXEL_BUF_END 0x0803FFFF
#define FPGA_CHAR_BASE 0x09000000
#define FPGA_CHAR_END 0x09001FFF

/* Cyclone V FPGA devices */
#define LED_BASE 0xFF200000
#define LEDR_BASE 0xFF200000
#define HEX3_HEX0_BASE 0xFF200020
#define HEX5_HEX4_BASE 0xFF200030
#define SW_BASE 0xFF200040
#define KEY_BASE 0xFF200050
#define JP1_BASE 0xFF200060
#define JP2_BASE 0xFF200070
#define PS2_BASE 0xFF200100
#define PS2_DUAL_BASE 0xFF200108
#define JTAG_UART_BASE 0xFF201000
#define IrDA_BASE 0xFF201020
#define TIMER_BASE 0xFF202000
#define TIMER_2_BASE 0xFF202020
#define AV_CONFIG_BASE 0xFF203000
#define RGB_RESAMPLER_BASE 0xFF203010
#define PIXEL_BUF_CTRL_BASE 0xFF203020
#define CHAR_BUF_CTRL_BASE 0xFF203030
#define AUDIO_BASE 0xFF203040
#define VIDEO_IN_BASE 0xFF203060
#define EDGE_DETECT_CTRL_BASE 0xFF203070
#define ADC_BASE 0xFF204000

/* Cyclone V HPS devices */
#define HPS_GPIO1_BASE 0xFF709000
#define I2C0_BASE 0xFFC04000
#define I2C1_BASE 0xFFC05000
#define I2C2_BASE 0xFFC06000
#define I2C3_BASE 0xFFC07000
#define HPS_TIMER0_BASE 0xFFC08000
#define HPS_TIMER1_BASE 0xFFC09000
#define HPS_TIMER2_BASE 0xFFD00000
#define HPS_TIMER3_BASE 0xFFD01000
#define FPGA_BRIDGE 0xFFD0501C

/********************
 * Structs
 */
// Enumeration of the player states
typedef enum PlayerStates {
  IDLE,
  MOVING,
  SHOOTING,
  HURT,
  EVASION,
  DEAD
} PlayerStates;

// Struct to store player information
typedef struct Player {
  // Position of player
  unsigned int x_pos;
  unsigned int y_pos;
  // Score of the player
  unsigned int score;
  // Health of player
  unsigned char health;
  // Movement speed of player
  unsigned char vel;
  // Boolean to determine direction of travel
  unsigned char right;
  unsigned char left;
  unsigned char down;
  unsigned char up;
  // Stores state the player is in
  // State is used to determine animation to play
  PlayerStates state;
  // Cooldown for shooting
  unsigned char shoot_cooldown;
  // Cooldown for ability'
  unsigned char ability_cooldown;
  // Used to determine the current frame of the animation
  unsigned char current_frame;
  // Used to determine the number of frames in the current animation being
  // played
  unsigned char frames_in_animation;
} Player;

// Struct to store cursor information
typedef struct Cursor {
  // Position of cursor
  unsigned int x_pos;
  unsigned int y_pos;
  // Speed the cursor moves
  unsigned char vel;
} Cursor;

// Struct to store mouse data
typedef struct MouseData {
  int dx;
  int dy;
  unsigned char LMB;
} MouseData;

// Enumeration of keys pressed
typedef enum KEYS { W, A, S, D, SPACE, OTHER } KEYS;

/*********************
 * Prototypes
 */

// Function used to initialize the mouse device
void init_mouse();
// Function used to initalize the keyboard device
void init_keyboard();
// Functions used to get mouse data
MouseData get_mouse_data();
// Function to get keyboard data
KEYS get_keyboard_data();
// Function used to stop timer
void stop_timer();
// Function used to delay program for specified time in milliseconds
void delay(unsigned int time_delay);

/******************
 * Main
 */
int main() {
  // Initialize devices
  init_mouse();
  init_keyboard();
  stop_timer();

  // Clear garbage from FIFOs
  get_keyboard_data();
  get_mouse_data();

  // Create instances of player and cursor
  Player player = {.x_pos = 0,
                   .y_pos = 0,
                   .score = 0,
                   .health = 100,
                   .vel = 5,
                   .right = 0,
                   .left = 0,
                   .down = 0,
                   .up = 0,
                   .state = IDLE,
                   .shoot_cooldown = 0,
                   .ability_cooldown = 0,
                   .current_frame = 0,
                   .frames_in_animation = 10};

  Cursor cursor = {.x_pos = 0, .y_pos = 0, .vel = 5};

  while (1) {
    // Get mouse data
    MouseData mouse_data = get_mouse_data();
    if (mouse_data.dx != 0) {
      printf("X: %d\n", mouse_data.dx);
    }
    if (mouse_data.dy != 0) {
      printf("Y: %d\n", mouse_data.dy);
    }
    if (mouse_data.LMB) {
      printf("LMB pressed\n");
    }
    // Get keyboard data
    KEYS key_pressed = get_keyboard_data();
    if (key_pressed == A) printf("A key pressed\n");
    if (key_pressed == S) printf("S key pressed\n");
    if (key_pressed == W) printf("W key pressed\n");
    if (key_pressed == D) printf("D key pressed\n");
    if (key_pressed == SPACE) printf("SPACE pressed\n");
  }
}

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
