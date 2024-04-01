#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

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

// Device specs
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define CLOCK_SPEED_DIV 100000

// Player related
#define SHOOTING_COOLDOWN 25

// Projectile related
#define PROJECTILE_WIDTH 2
#define MAX_NUM_PROJECTILES 10

// Colours
#define YELLOW 0xFFE0
#define RED 0xF800
#define GREEN 0x07E0

// Hex display
#define HEX0 0x3F
#define HEX1 0x06
#define HEX2 0x5B
#define HEX3 0x4F
#define HEX4 0x66
#define HEX5 0x6D
#define HEX6 0x7D
#define HEX7 0x07
#define HEX8 0x7F
#define HEX9 0x67
#define HEXA 0x77
#define HEXB 0x7C
#define HEXC 0x39
#define HEXD 0x5E
#define HEXE 0x79
#define HEXF 0x71

/*************** PLAYER RELATED ***********************/

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
  int x_pos;
  int y_pos;
  // Stores the width and height of player
  unsigned short int width;
  unsigned short int height;
  // Score of the player
  unsigned int score;
  // Cooldown for shooting
  unsigned int shoot_cooldown;
  // Health of player
  char health;
  // Movement speed of player
  unsigned char vel;
  // Boolean to determine direction of travel
  bool right;
  bool left;
  bool down;
  bool up;
  // Stores state the player is in
  // State is used to determine animation to play
  PlayerStates state;
  // Boolean used to determine if no cooldown active on evade ability
  bool canEvade;
  // Used to determine the current frame of the animation
  unsigned char current_frame;
  // Used to determine the number of frames in the current animation being
  // played
  unsigned char frames_in_animation;
} Player;

// Struct to store cursor information
typedef struct Cursor {
  // Position of cursor
  int x_pos;
  int y_pos;
  // Dimensions of cursor
  unsigned short int width;
  unsigned short int height;
  // Speed the cursor moves
  unsigned char vel;
} Cursor;

// Struct to store projectile information
typedef struct Projectile {
  // Position of projectile
  float x_pos;
  float y_pos;

  // Dimensions
  unsigned short int width;
  unsigned short int height;

  // Direction of travel
  float dx;
  float dy;

  // Used to traverse to next projectile in list
  struct Projectile* next;
} Projectile;

// Linked list of projectiles
typedef struct ProjectileList {
  // Points to the head of the projectile list
  Projectile* head;
  // Points to tail of the projectile list
  Projectile* tail;
  // Keeps track of the total number of projectiles on screen
  int count;
} ProjectileList;

/************** DEVICES ********************/

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
// Sets the specified timer for specified time (ms) and starts it
void set_timer(unsigned int time, int timer_addr, bool cont);
// Polls specified timer if it is done counting down
bool timer_done(int timer_addr);
// Function used to delay program for specified time in milliseconds
void delay(unsigned int time_delay);
/*********** HEX display ***************/
// Sets hex display to zeros
void init_hex();
// Displays number to hex as BCD
void set_hex(int v);

// Creates projectile object
bool createProjectile(ProjectileList* list, const Player player,
                      const Cursor cursor);
// Updates projectile position
void updateProjectilePosition(ProjectileList* list);
// Used to free memory use for projectile list
void freeProjectileList(ProjectileList* list);
// sets up back buffer for double buffering
void init_double_buffer(short int* buffer1, short int* buffer2);
// plots a single pixel onto the back frame buffer
void plot_pixel(int x, int y, short int colour);
// Used to busy wait for screen buffer to swap for I/O
void wait_for_vsync();
// Clears the screen to background image
void clear_screen();
// Draws player to the screen
void draw_player(const Player player);
// Draws the cursor to the screen
void draw_cursor(const Cursor cursor);
// Draws the projectiles to the screen
void draw_projectiles(const ProjectileList* list);
// Updates the screen
void refresh_screen(const Player player, const Cursor Cursor,
                    const ProjectileList* list);
// Updates the player's position, state, and cooldowns
void updatePlayer(Player* player, MouseData mouse, KEYS key_pressed);
// Updates the player's cursor
void updateCursor(Cursor* cursor, MouseData mouse);

/******************
 * Main
 */
// buffer used to copy background quickly
short int BackgroundBuffer[240][512] = {0};

// Memory used for front and back buffers
short int Buffer1[240][512];
short int Buffer2[240][512];

int main() {
  // Initial setup

  // Initialize devices
  init_mouse();
  init_keyboard();
  stop_timer();
  init_hex();
  init_double_buffer(&Buffer1, &Buffer2);

  // Clear garbage from PS2 FIFOs
  get_mouse_data();
  get_keyboard_data();

  // Create instances of player and cursor
  Player player = {.x_pos = 100,
                   .y_pos = 60,
                   .height = 10,
                   .width = 10,
                   .score = 0,
                   .shoot_cooldown = 0,
                   .health = 100,
                   .vel = 5,
                   .right = false,
                   .left = false,
                   .down = false,
                   .up = false,
                   .state = IDLE,
                   .canEvade = true,
                   .current_frame = 0,
                   .frames_in_animation = 10};

  Cursor cursor = {
      .x_pos = 100, .y_pos = 60, .width = 10, .height = 10, .vel = 5};

  // Create list of projectiles
  ProjectileList* projectile_list = malloc(sizeof(ProjectileList));
  // Unable to allocate memory - Error
  if (projectile_list == NULL) {
    return -1;
  }
  // Initalize pointers for list as NULL
  projectile_list->head = NULL;
  projectile_list->tail = NULL;
  projectile_list->count = 0;

  // Setup score timer - player gains one score every 1 second
  set_timer(1000, TIMER_BASE, true);

  while (1) {
    // Get mouse data
    MouseData mouse_data = get_mouse_data();
    // Get key pressed
    KEYS key_pressed = get_keyboard_data();

    // Update player and cursor based on inputs
    updatePlayer(&player, mouse_data, key_pressed);
    updateCursor(&cursor, mouse_data);

    // TODO:
    // Update enemies

    // Create new projectile if player is currently shooting
    if (player.state == SHOOTING) {
      createProjectile(projectile_list, player, cursor);
    }
    // Update position of projectiles
    updateProjectilePosition(projectile_list);

    // Any other updates???

    // Collision detection

    // Refresh screen
    refresh_screen(player, cursor, projectile_list);
    // Update score display
    set_hex(player.score);
  }

  // Deallocate memory
  freeProjectileList(projectile_list);
}


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
bool timer_done(timer_addr){
  // Check TO flag
  volatile int* addr = (int*)timer_addr;
  // TO flag raised --> timer done
  if (*addr & 0x1){
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


// Clears the screen to the background buffer
void clear_screen() {
  volatile int* pixel_ctrl_ptr = (int*)PIXEL_BUF_CTRL_BASE;
  int* back_buffer = *(pixel_ctrl_ptr + 1);
  memcpy(back_buffer, BackgroundBuffer, sizeof(BackgroundBuffer));
}

// Plots a pixel at the specified location in the back buffer
void plot_pixel(int x, int y, short int colour) {
  // Get back buffer from device
  volatile int* pixel_ctrl_ptr = (int*)PIXEL_BUF_CTRL_BASE;
  // Get pixel location in memory
  short int* pixel = *(pixel_ctrl_ptr + 1) + (y << 10) + (x << 1);
  // Draw pixel in memory
  *pixel = colour;
}

// Busy wait loop to check if buffer ready to write to
void wait_for_vsync() {
  volatile int* pixel_ctrl_ptr = (int*)PIXEL_BUF_CTRL_BASE;
  int status;
  // Start synchronization by writing one to buffer
  *pixel_ctrl_ptr = 1;
  // get status reg
  status = *(pixel_ctrl_ptr + 3);
  // Continue polling until buffer ready to write to
  while ((status & 0x1) != 0) {
    // update
    status = *(pixel_ctrl_ptr + 3);
  }
}

// sets up back buffer for double buffering
void init_double_buffer(short int* buffer1, short int* buffer2) {
  volatile int* pixel_ctrl_ptr = (int*)PIXEL_BUF_CTRL_BASE;

  // Set front buffer
  *(pixel_ctrl_ptr + 1) = (int)buffer1;
  clear_screen();
  wait_for_vsync();

  // Set back buffer
  *(pixel_ctrl_ptr + 1) = (int)buffer2;
  clear_screen();
}

// Draws the player to the screen
void draw_player(const Player player) {
  // TODO draw sprite/animation of player

  // Testing draw player as square
  for (int y = player.y_pos; y < player.y_pos + player.height; y++) {
    for (int x = player.x_pos; x < player.x_pos + player.width; x++) {
      plot_pixel(x, y, GREEN);
    }
  }
}

// Draws the cursor to the screen
void draw_cursor(const Cursor cursor) {
  // TODO draw sprite for cursor

  // Testing draw as single pixel
  plot_pixel(cursor.x_pos + (cursor.width >> 1), cursor.y_pos + (cursor.height >> 1), RED);
}

// Draws the projectiles to the screen
void draw_projectiles(const ProjectileList* list) {
  // Draw all projectiles in the linked list
  Projectile* cur = list->head;
  while (cur != NULL) {
    // Draw each projectile as yellow square
    for (int y = (int)cur->y_pos; y < (int)(cur->y_pos + cur->height); y++) {
      for (int x = (int)cur->x_pos; x < (int)(cur->x_pos + cur->width); x++) {
        plot_pixel(x, y, YELLOW);
      }
    }

    // Traverse to next projectile
    cur = cur->next;
  }
}

// Updates the screen
void refresh_screen(const Player player, const Cursor Cursor,
                    const ProjectileList* list) {
  // Clear screen first
  clear_screen();

  // Draw elements to screen
  draw_player(player);
  draw_projectiles(list);
  draw_cursor(Cursor);

  // Call buffer swap
  wait_for_vsync();
}

// Creates projectile object
bool createProjectile(ProjectileList* list, const Player player,
                      const Cursor cursor) {
  // Do no create more projectiles if at max
  if (list->count >= MAX_NUM_PROJECTILES) {
    return false;
  }

  // Try to allocate memory
  Projectile* projectile = malloc(sizeof(Projectile));
  // Allocation failed
  if (projectile == NULL) {
    return false;
  }

  // Direction of travel calculated as unit vector from center of player to
  // center of cursor Get vector
  float dx = (cursor.x_pos + (cursor.width >> 1)) - (player.x_pos + (player.width >> 1));
  float dy = (cursor.y_pos + (cursor.height >> 1)) - (player.y_pos + (player.height >> 1));

  // Normalize vector
  float magnitude = sqrt((dx * dx) + (dy * dy));
  dx /= magnitude;
  dy /= magnitude;

  // Load information
  projectile->dx = dx;
  projectile->dy = dy;
  projectile->x_pos = player.x_pos + (player.width >> 1);
  projectile->y_pos = player.y_pos + (player.height >> 1);
  projectile->next = NULL;
  projectile->height = PROJECTILE_WIDTH;
  projectile->width = PROJECTILE_WIDTH;

  // Insert projectile into list
  // List is currently empty
  if (list->head == NULL) {
    list->head = projectile;
    list->tail = projectile;
  }
  // List is populated
  else {
    list->tail->next = projectile;
    list->tail = projectile;
  }

  // Update count
  list->count++;

  return true;
}

// Updates projectile position
void updateProjectilePosition(ProjectileList* list) {
  // Traverse list
  Projectile* cur = list->head;
  Projectile* prev = NULL;
  while (cur != NULL) {
    // Update information
    cur->x_pos += cur->dx;
    cur->y_pos += cur->dy;

    // Check projectile now off screen
    if (cur->x_pos < 0 || cur->x_pos + cur->width > SCREEN_WIDTH ||
        cur->y_pos < 0 || cur->y_pos + cur->height > SCREEN_HEIGHT) {
      // Delete projectile since off screen
      // Need to delete head
      if (prev == NULL) {
        // Set node to delete in temp
        prev = cur;
        // Traverse
        cur = cur->next;
        // Update new head of list
        list->head = cur;
        // Head was tail as well
        list->tail = prev == list->tail ? cur : list->tail;
        // Free memory
        free(prev);
        prev = NULL;
      }
      // Node to delete is not the head
      else {
        prev->next = cur->next;
        // Tail node is being deleted
        if (prev->next == NULL) {
          list->tail = prev;
        }
        free(cur);
        cur = prev->next;
      }

      // Update count
      list->count--;
    }
    // traverse list
    else {
      prev = cur;
      cur = cur->next;
    }
  }
}

// Used to free memory use for projectile list
// NOTE: After calling, the list pointer should not be used again!
void freeProjectileList(ProjectileList* list) {
  // Iterate through list to free each projectile
  Projectile* cur = list->head;
  while (cur != NULL) {
    Projectile* tmp = cur;
    cur = cur->next;
    free(tmp);
  }

  // Free list pointer itself
  free(list);
}

// Updates the player's position, state, and cooldowns
void updatePlayer(Player* player, MouseData mouse, KEYS key_pressed) {
  /**** Keyboard input ********/
  // WASD control movement of player
  // SPACE controls evasion state of player

  // Update position and state based on key press
  switch (key_pressed) {
    // Move player up
    case W:
      player->y_pos -= player->vel;
      player->up = true;
      player->down = false;
      player->left = false;
      player->right = false;
      player->state = player->state == EVASION ? EVASION : MOVING;
      break;
    // Move left
    case A:
      player->x_pos -= player->vel;
      player->left = true;
      player->right = false;
      player->up = false;
      player->down = false;
      player->state = player->state == EVASION ? EVASION : MOVING;
      break;
    // Move right
    case D:
      player->x_pos += player->vel;
      player->right = true;
      player->left = false;
      player->down = false;
      player->up = false;
      player->state = player->state == EVASION ? EVASION : MOVING;
      break;
    // Move down
    case S:
      player->y_pos += player->vel;
      player->down = true;
      player->up = false;
      player->left = false;
      player->right = false;
      player->state = player->state == EVASION ? EVASION : MOVING;
      break;
    // Evasion
    case SPACE:
      // Enter evasion if cooldown is set to zero and in a valid state
      // Player enters evasion state for 2 seconds
      if (player->canEvade && player->health > 0) {
        player->state = EVASION;
        player->canEvade = false;
        // Increase movement speed
        player->vel = player->vel << 2;
        // Set timer for 2 seconds
        set_timer(2000, TIMER_2_BASE, false);
      }
      break;

    // No meaningful input
    default:
      player->state = player->state == EVASION ? EVASION : IDLE;
      break;
  }

  // Check out of bounds for player
  // Left boundary
  if (player->x_pos < 0) {
    player->x_pos = 0;
  }
  // Right boundary
  else if (player->x_pos + player->width > SCREEN_WIDTH) {
    player->x_pos = SCREEN_WIDTH - player->width;
  }
  // Top boundary
  if (player->y_pos < 0) {
    player->y_pos = 0;
  }
  // Bottom boundary
  else if (player->y_pos + player->height > SCREEN_HEIGHT) {
    player->y_pos = SCREEN_HEIGHT - player->height;
  }

  // Check for ability cool down
  bool timerDone = timer_done(TIMER_2_BASE);
  // Evasion wore off
  if (timerDone && player->state == EVASION) {
    // Player is no longer in evasion state
    player->state = MOVING;
    // Set speed back to normal
    player->vel = player->vel >> 2;
    // Set timer for cooldown for 30s
    set_timer(30000, TIMER_2_BASE, false);
  }
  // Cooldown is finished for evasion
  else if (timerDone) {
    player->canEvade = true;
  }

  /*********** MOUSE ****************/
  // LMB used to shoot

  // LMB clicked -> player enters shooting state
  // Can only shoot when not evading and shooting cooldown is finished
  if (mouse.LMB && player->state != EVASION && player->shoot_cooldown == 0) {
    player->state = SHOOTING;
    player->shoot_cooldown = SHOOTING_COOLDOWN + 1;
  }
  // Update shoot cooldown
  if (player->shoot_cooldown > 0) {
    player->shoot_cooldown--;
  }

  // Update player's score when base timer done counting down
  timerDone = timer_done(TIMER_BASE);
  if (timerDone && player->health > 0){
    player->score++;
  }
}

// Updates the player's cursor
void updateCursor(Cursor* cursor, MouseData mouse) {
  // Update the cursor's position based upon mouse movement
  cursor->x_pos += mouse.dx;
  cursor->y_pos += mouse.dy;

  // Check for out of bounds
  if (cursor->x_pos < 0)
    cursor->x_pos = 0;
  else if (cursor->x_pos + cursor->width > SCREEN_WIDTH)
    cursor->x_pos = SCREEN_WIDTH - cursor->width;
  if (cursor->y_pos < 0)
    cursor->y_pos = 0;
  else if (cursor->y_pos + cursor->height > SCREEN_HEIGHT)
    cursor->y_pos = SCREEN_HEIGHT - cursor->height;
}
