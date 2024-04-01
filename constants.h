#ifndef CONSTANTS_H
#define CONSTANTS_H

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

// Buffer used to quickly copy background images from memory
extern short int BackgroundBuffer[240][512];

#endif