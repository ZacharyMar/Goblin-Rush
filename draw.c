#include "draw.h"

#include <stdbool.h>
#include <stdlib.h>

#include "address_map_nios2.h"
#include "constants.h"

// Plots a pixel at the specified location in the back buffer
void plot_pixel(int x, int y, short int colour) {
  // Get back buffer from device
  volatile int *pixel_ctrl_ptr = (int *)PIXEL_BUF_CTRL_BASE;
  // Get pixel location in memory
  short int *pixel = *(pixel_ctrl_ptr + 1) + (y << 10) + (x << 1);
  // Draw pixel in memory
  *pixel = colour;
}

// Busy wait loop to check if buffer ready to write to
void wait_for_vsync() {
  volatile int *pixel_ctrl_ptr = (int *)PIXEL_BUF_CTRL_BASE;
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
void init_double_buffer(short int buffer1[240][512],
                        short int buffer2[240][512]) {
  volatile int *pixel_ctrl_ptr = (int *)PIXEL_BUF_CTRL_BASE;

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
  // draw sprite for cursor
  for (int i = 0; i < cursor.height; i++) {
    for (int j = 0; j < cursor.width; j++) {
      // if cursor is on screen
      if (cursor_sprite[i][j] != 0x0000) {
        // normalize cursor to plot at the center of cursor.x and cursor.y
        plot_pixel(cursor.x_pos + j, cursor.y_pos + i, cursor_sprite[i][j]);
      }
    }
  }
}

// Draws the projectiles to the screen
void draw_projectiles(const ProjectileList *list) {
  // Draw all projectiles in the linked list
  Projectile *cur = list->head;
  while (cur != NULL) {
    // Draw each projectile as yellow square
    for (int y = cur->y_pos; y < cur->y_pos + cur->height; y++) {
      for (int x = cur->x_pos; x < cur->x_pos + cur->width; x++) {
        plot_pixel(x, y, PURPLE);
      }
    }
    // Traverse to next projectile
    cur = cur->next;
  }
}

// Updates the screen
void refresh_screen(const Player player, const Cursor Cursor,
                    const ProjectileList *list) {
  // Clear screen first
  clear_screen();

  // Draw elements to screen
  draw_player(player);
  draw_projectiles(list);
  draw_cursor(Cursor);

  // Call buffer swap
  wait_for_vsync();
}

// draw all enemies and sprites
void draw_goblins(const Player player, GoblinList *root) {
  // iterate through goblins
  Goblin *gob = root->head;
  while (gob != NULL) {
    // select sprite sheet for drawing
    unsigned short int **sprite_sheet = NULL;
    // reverse for right movement
    bool reverse = false;

    // goblin moving right
    if (gob->right) {
      reverse = true;
      sprite_sheet = gob->state == 0 ? goblin_S_Walk : goblin_S_Attack;
      // goblin moving left
    } else if (gob->left) {
      sprite_sheet = gob->state == 0 ? goblin_S_Walk : goblin_S_Attack;
      // goblin moving up (sprite sheet orientation is mirrored on x axis)
    } else if (!gob->up && gob->down) {
      sprite_sheet = gob->state == 0 ? goblin_U_Walk : goblin_U_Attack;

      // goblin moving down
    } else if (gob->up && !gob->down) {
      sprite_sheet = gob->state == 0 ? goblin_D_Walk : goblin_D_Attack;
    }
    draw_enemy_sprite_frame(sprite_sheet, gob->x_pos, gob->y_pos,
                            gob->current_frame, gob->frames_in_animation,
                            reverse);
    gob = gob->next;
  }
}

// draws an enemy sprite starting from its top left corner (x_offset, y_offset)
void draw_enemy_sprite_frame(unsigned short int **sprite_ptr,
                             unsigned int x_offset, unsigned int y_offset,
                             unsigned int frame_idx, unsigned int num_frames,
                             bool reverse) {
  // bounds detection, will remove in final game
  if (!in_bounds(x_offset, y_offset, 48, 48)) {
    printf("out of bounds at (%d, %d)", x_offset, y_offset);
    return;
  }
  // iterate through the height of a frame
  unsigned int sheet_height = 48;
  unsigned int sheet_width = 288;
  unsigned int frame_width = sheet_width / num_frames;
  for (unsigned int i = 0; i < sheet_height; i++) {
    // iterate through the width of a frame
    for (unsigned int j = 0; j < frame_width; j++) {
      // Calculate the index to read from the sprite sheet based on direction
      unsigned int sprite_index = reverse ? (frame_width - 1 - j) : j;
      sprite_index += (frame_width * frame_idx);

      // remove background and draw pixel if it's not white (0xFFFF)
      if (((unsigned short int *)sprite_ptr)[i * sheet_width + sprite_index] !=
          0xFFFF) {
        plot_pixel(
            x_offset + j, y_offset + i,
            ((unsigned short int *)sprite_ptr)[i * sheet_width + sprite_index]);
      }
    }
  }
}