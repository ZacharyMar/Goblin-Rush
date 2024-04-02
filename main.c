#include <stdlib.h>

#include "address_map_nios2.h"
#include "constants.h"
#include "devices.h"
#include "draw.h"
#include "player.h"
#include "projectile.h"
#include "structs.h"
#include "enemies.h"

// Declare globals here

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
  init_double_buffer(Buffer1, Buffer2);

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

  // Create list of enemies
  GoblinList* goblin_list = malloc(sizeof(GoblinList));
  // Unable to allocate memory - error
  if (goblin_list == NULL){
    return -1;
  }
  // Initialize pointers
  goblin_list->head = 0;
  goblin_list->tail = 0;
  goblin_list->count = 0;

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
    enemyProjectileCollisionUpdate(projectile_list, goblin_list);

    // Refresh screen
    refresh_screen(player, cursor, projectile_list);
    // Update score display
    set_hex(player.score);
  }

  // Deallocate memory
  freeProjectileList(projectile_list);
  freeEnemyList(goblin_list);
}
