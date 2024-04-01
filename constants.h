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

// Bounds
#define BOUNDARY 5

// Array helper
#define ARRAYSIZE(a) (sizeof(a) / sizeof(a[0]))

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

// Buffer used to quickly copy background images from memory
extern short int BackgroundBuffer[240][512];

#endif