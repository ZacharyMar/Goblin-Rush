#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdbool.h>

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

/************** DEVICES ********************/

// Struct to store mouse data
typedef struct MouseData {
  int dx;
  int dy;
  unsigned char LMB;
}MouseData;

// Enumeration of keys pressed
typedef enum KEYS{
  W,
  A,
  S,
  D,
  SPACE,
  OTHER
}KEYS;

#endif