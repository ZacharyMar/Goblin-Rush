#ifndef ENEMIES_H
#define ENEMIES_H

#include <stdbool.h>

typedef struct Goblin {
  // Location of goblin
  unsigned int x_pos;
  unsigned int y_pos;

  // health of the goblin
  unsigned char health;
  // speed of the goblin
  unsigned char speed;
  // current goblin state
  unsigned char state;
  // Used to determine the current frame of the animation
  unsigned char current_frame;
  // Used to determine the number of frames in the current animation being
  // played
  unsigned char frames_in_animation;
  // Boolean to determine direction of travel
  bool right;
  bool left;
  bool up;
  bool down;
  // next pointer
  struct Goblin* next;
} Goblin;

// Linked list to store goblins
typedef struct GoblinList {
  // Points to head and tail
  Goblin* head;
  Goblin* tail;
  // Count of elements in list
  unsigned int count;
} GoblinList;

typedef struct Bee {
  // Location of Bee
  unsigned int x_pos;
  unsigned int y_pos;

  // health of the Bee
  unsigned char health;

  // direction of attack
  unsigned char direction;

  // current Bee state
  unsigned char state;

  // Used to determine the current frame of the animation
  unsigned char current_frame;
  // Used to determine the number of frames in the current animation being
  // played
  unsigned char frames_in_animation;
  // next pointer
  struct Bee* next;
} Bee;

typedef struct Wolf {
  // Location of Wolf
  unsigned int x_pos;
  unsigned int y_pos;

  // health of the Wolf
  unsigned char health;

  // speed of attack
  unsigned char speed;

  // current Wolf state
  unsigned char state;

  // Used to determine the current frame of the animation
  unsigned char current_frame;
  // Used to determine the number of frames in the current animation being
  // played
  unsigned char frames_in_animation;
  // next pointer
  struct Wolf* next;
} Wolf;

/****** Protothypes ****/
// draws an enemy sprite starting from its top left corner (x_offset, y_offset)
void draw_enemy_sprite_frame(unsigned short int sprite_ptr[][288],
                             unsigned int x_offset, unsigned int y_offset,
                             unsigned int frame_idx, unsigned int num_frames,
                             bool reverse);

// Function to add a Goblin to the list
void add_goblin(GoblinList* list, Goblin* new_goblin);

// Function to create a new Goblin
Goblin* create_goblin(unsigned int x, unsigned int y, unsigned char health,
                      unsigned char speed, unsigned char state,
                      unsigned char frame, unsigned char frames, bool right,
                      bool left, bool up, bool down);

// returns whether or not a sprite is within the screen
bool in_bounds(int x, int y, unsigned int width, unsigned int height);

// Used to free memory allocated for enemy list
void freeEnemyList(GoblinList* list);

#endif