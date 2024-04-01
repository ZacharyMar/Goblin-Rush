#ifndef DRAW_H
#define DRAW_H

#include "structs.h"
#include "projectile.h"

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
void refresh_screen(const Player player, const Cursor Cursor, const ProjectileList* list);

#endif