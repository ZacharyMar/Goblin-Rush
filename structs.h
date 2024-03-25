#ifndef STRUCTS_H
#define STRUCTS_H

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

#endif