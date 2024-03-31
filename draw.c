#include "draw.h"

#include <stdlib.h>

#include "address_map_nios2.h"
#include "constants.h"

// Clears the screen to the background buffer
void clear_screen() {
  volatile int *pixel_ctrl_ptr = (int *)PIXEL_BUF_CTRL_BASE;
  int *back_buffer = *(pixel_ctrl_ptr + 1);
  memcpy(back_buffer, BackgroundBuffer, sizeof(BackgroundBuffer));
}

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
void init_double_buffer(short int *buffer1, short int *buffer2) {
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
  // TODO draw sprite for cursor

  // Testing draw as single pixel
  plot_pixel(cursor.x_pos + (cursor.width >> 1), cursor.y_pos + (cursor.height >> 1), RED);
}

// Draws the projectiles to the screen
void draw_projectiles(const ProjectileList *list) {
  // Draw all projectiles in the linked list
  Projectile *cur = list->head;
  while (cur != NULL) {
    // Draw each projectile as yellow square
    for (int y = cur->y_pos; y < cur->y_pos + cur->height; y++) {
      for (int x = cur->x_pos; x < cur->x_pos + cur->width; x++) {
        plot_pixel(x, y, YELLOW);
      }
    }

    // Traverse to next projectile
    cur = cur->next;
  }
}

// Updates the screen
void refresh_screen(const Player player, const Cursor Cursor, const ProjectileList* list){
    // Clear screen first
    clear_screen();

    // Draw elements to screen
    draw_player(player);
    draw_projectiles(list);
    draw_cursor(Cursor);

    // Call buffer swap
    wait_for_vsync();
}
