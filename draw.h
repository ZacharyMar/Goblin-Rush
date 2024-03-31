#ifndef DRAW_H
#define DRAW_H

// sets up back buffer for double buffering
void init_double_buffer(volatile int * pcp);

// plots a single pixel onto the back frame buffer
void plot_pixel(int x, int y, short int colour);

// Used to busy wait for screen buffer to swap for I/O
void wait_for_vsync();

#endif