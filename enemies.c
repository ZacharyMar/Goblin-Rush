#include "enemies.h"

#include <stdlib.h>
#include "player.h"
#include "constants.h"
#include <stdlib.h>
#include "player.h"
// updates the goblin object based on player location
void update_goblins(const Player* player, GoblinList* root){
  // iterate through goblins
    Goblin* gob = root->head;
    while(gob != NULL){
        // update goblin parameters
        int dx = player->x_pos - (gob->x_pos + 24); // true center of sprite
        int dy = player->y_pos - (gob->y_pos + 24); 

        // distance moved in each direction
        int move_x = 0, move_y = 0;

        // directional booleans
        gob->right = dx > 0 ? true : false;
        gob->left = !gob->right;
        gob->up = dy < 0 ? true : false;
        gob->down = !gob->up;
        
        double magnitude = sqrt(dx * dx + dy * dy); // magnitude
        // attack if close enough
        gob->state = magnitude < GOBLIN_ATTACK_RANGE ? 1 : 0;
        // avoid division by zero
        if (magnitude > 0) {
            move_x = (int) ((dx / magnitude) * gob->speed);
            move_y = (int) ((dy / magnitude) * gob->speed);
        }

        int new_x = gob->x_pos + move_x;
        int new_y = gob->y_pos + move_y;
        // update boolean parameters based on whether or not next move is in bounds
        if(!in_bounds(new_x, new_y, 48, 48)){
            if(new_x <= BOUNDARY){
                gob->right = true;
                gob->left = false;
            }
            if(new_x + 48 + BOUNDARY >= SCREEN_WIDTH){
                gob->right = false;
                gob->left = true;
            }
            if(new_y <= BOUNDARY){
                gob->up = true;
                gob->down = false;
            }
            if(new_y + 48 + BOUNDARY >= SCREEN_HEIGHT){
                gob->up = false;
                gob->down = true;
            }
            // new dx and dy 
            move_x = gob->left || gob->right ? gob->speed : 0;
            move_y = gob->up || gob->down ? gob->speed : 0;
            move_x = gob->right ? move_x : -move_x;
            move_y = gob->up ? move_y : -move_y;
        }
        
        // update x and y pos along with sprite sheet index
        gob->x_pos += move_x;
        gob->y_pos += move_y;
        gob->current_frame = (gob->current_frame + 1) % gob->frames_in_animation;
        gob = gob->next;
  }
}

// populates single goblin
void new_goblin(GoblinList* root){
    // defaults
        bool left = false, right = false, up = false, down = false;
        // set direction booleans
        rand_direction(&right, &left, &up, &down);
        Goblin* g = create_goblin(rand() % (SCREEN_WIDTH - 48 - BOUNDARY) + BOUNDARY, rand() % (SCREEN_HEIGHT- 48 - BOUNDARY) + BOUNDARY, 100, (rand() % GOBLIN_MAX_SPEED) + 1, 0x0, 0, 6, right, left, up, down); // state: 0 for walk 1 for attack
        // add new goblin to linked list
        add_goblin(root, g);   
}

// returns random direction
void rand_direction(bool* right, bool* left, bool* up, bool* down){
    unsigned int dir = rand() % 8;
     switch(dir){
            // right
            case(0):
                *right = true;
                break;
            // up and to the right
            case(1):
                *right = true;
                *up = true;
                break;
            // up
            case(2):
                *up = true;
                break;
            // up and to the left
            case(3):
                *up = true;
                *left = true;
                break;
            // left
            case(4):
                *left = true;
                break;
            // down and to the left
            case(5):
                *left = true;
                *down = true;
                break;
            // down
            case(6):
                *down = true;
                break;
            // down and to the right
            default:
                *down = true;
                *right = true;
        }
}
// returns whether or not a sprite is within the screen
bool in_bounds(int x, int y, unsigned int width, unsigned int height) {
  return x > BOUNDARY && x + width + BOUNDARY < SCREEN_WIDTH && y > BOUNDARY &&
         y + height + BOUNDARY < SCREEN_HEIGHT;
}

// draws an enemy sprite starting from its top left corner (x_offset, y_offset)
void draw_enemy_sprite_frame(unsigned short int sprite_ptr[][288],
                             unsigned int x_offset, unsigned int y_offset,
                             unsigned int frame_idx, unsigned int num_frames,
                             bool reverse) {
  // iterate through the height of a frame
  unsigned int sheet_height = 48;
  unsigned int sheet_width = ARRAYSIZE(sprite_ptr[0]);
  unsigned int frame_width = sheet_width / num_frames;
  for (unsigned int i = 0; i < sheet_height; i++) {
    // iterate through the width of a frame
    for (unsigned int j = 0; j < frame_width; j++) {
      // Calculate the index to read from the sprite sheet based on direction
      unsigned int sprite_index = reverse ? (frame_width - 1 - j) : j;
      sprite_index += (frame_width * frame_idx);

      // remove background and draw pixel if it's not white (0xFFFF)
      if (sprite_ptr[i][sprite_index] != 0xFFFF) {
        plot_pixel(x_offset + j, y_offset + i, sprite_ptr[i][sprite_index]);
      }
    }
  }
}

// Function to create a new Goblin
Goblin* create_goblin(unsigned int x, unsigned int y, unsigned char health,
                      unsigned char speed, unsigned char state,
                      unsigned char frame, unsigned char frames, bool right,
                      bool left, bool up, bool down) {
  Goblin* new_goblin = (Goblin*)malloc(sizeof(Goblin));
  if (new_goblin == NULL) {
    // cant allocate mem
    return NULL;
  }

  // Initialize goblin properties
  new_goblin->x_pos = x;
  new_goblin->y_pos = y;
  new_goblin->health = health;
  new_goblin->speed = speed;
  new_goblin->state = state;
  new_goblin->current_frame = frame;
  new_goblin->frames_in_animation = frames;
  new_goblin->right = right;
  new_goblin->left = left;
  new_goblin->up = up;
  new_goblin->down = down;
  new_goblin->next = NULL;

  return new_goblin;
}

// Function to add a Goblin to the list
void add_goblin(GoblinList* list, Goblin* new_goblin) {
  // Failed to allocate goblin
  if (new_goblin == NULL) return;

  // Adds goblin to tail of linked list
  // List is empty
  if (list->head == NULL) {
    list->head = new_goblin;
    list->tail = new_goblin;
  }
  // Add to tail
  else {
    list->tail->next = new_goblin;
    list->tail = new_goblin;
  }
  list->count++;
}
// Used to free memory allocated for enemy list
void freeGoblinList(GoblinList* list) {
  Goblin* cur = list->head;
  while (cur != NULL) {
    Goblin* tmp = cur;
    cur = cur->next;
    free(tmp);
  }
  free(list);
}

// Checks for collisions of goblin and player
bool updateCollisionPlayer(Player* player, GoblinList* g_list) {
  // Check if goblin is currently attacking
  Goblin* cur = g_list->head;
  while (cur != NULL) {
    // Check goblin is attacking and player is hittable
    if (cur->state == 1 && player->state != EVASION) {
      // Determine if player is in attack hitbox
      if (cur->left) {
        if (((player->x_pos >
                  cur->x_pos + GOBLIN_HITBOX_OFFSET - GOBLIN_ATTACK_BOX_WIDTH &&
              player->x_pos < cur->x_pos + GOBLIN_HITBOX_OFFSET +
                                  GOBLIN_ATTACK_BOX_PADDING) ||
             (player->x_pos + player->width >
                  cur->x_pos + GOBLIN_HITBOX_OFFSET - GOBLIN_ATTACK_BOX_WIDTH &&
              player->x_pos + player->width < cur->x_pos +
                                                  GOBLIN_HITBOX_OFFSET +
                                                  GOBLIN_ATTACK_BOX_PADDING)) &&
            ((player->y_pos > cur->y_pos + GOBLIN_HITBOX_OFFSET +
                                  GOBLIN_ATTACK_BOX_Y_OFFSET &&
              player->y_pos < cur->y_pos + GOBLIN_HITBOX_OFFSET +
                                  GOBLIN_ATTACK_BOX_Y_OFFSET +
                                  GOBLIN_ATTACK_BOX_HEIGHT) ||
             (player->y_pos + player->height > cur->y_pos +
                                                   GOBLIN_HITBOX_OFFSET +
                                                   GOBLIN_ATTACK_BOX_Y_OFFSET &&
              player->y_pos + player->height <
                  cur->y_pos + GOBLIN_HITBOX_OFFSET +
                      GOBLIN_ATTACK_BOX_Y_OFFSET + GOBLIN_ATTACK_BOX_HEIGHT))) {
          return true;
        }
      } else if (cur->right) {
        if (((player->x_pos > cur->x_pos + GOBLIN_HITBOX_OFFSET +
                                  GOBLIN_HITBOX_WIDTH -
                                  GOBLIN_ATTACK_BOX_PADDING &&
              player->x_pos < cur->x_pos + GOBLIN_HITBOX_OFFSET +
                                  GOBLIN_HITBOX_WIDTH +
                                  GOBLIN_ATTACK_BOX_WIDTH) ||
             (player->x_pos + player->width >
                  cur->x_pos + GOBLIN_HITBOX_OFFSET + GOBLIN_HITBOX_WIDTH -
                      GOBLIN_ATTACK_BOX_PADDING &&
              player->x_pos + player->width <
                  cur->x_pos + GOBLIN_HITBOX_OFFSET + GOBLIN_HITBOX_WIDTH +
                      GOBLIN_ATTACK_BOX_WIDTH)) &&
            ((player->y_pos > cur->y_pos + GOBLIN_HITBOX_OFFSET +
                                  GOBLIN_ATTACK_BOX_Y_OFFSET &&
              player->y_pos < cur->y_pos + GOBLIN_HITBOX_OFFSET +
                                  GOBLIN_ATTACK_BOX_Y_OFFSET +
                                  GOBLIN_ATTACK_BOX_HEIGHT) ||
             (player->y_pos + player->height > cur->y_pos +
                                                   GOBLIN_HITBOX_OFFSET +
                                                   GOBLIN_ATTACK_BOX_Y_OFFSET &&
              player->y_pos + player->height <
                  cur->y_pos + GOBLIN_HITBOX_OFFSET +
                      GOBLIN_ATTACK_BOX_Y_OFFSET + GOBLIN_ATTACK_BOX_HEIGHT))) {
          return true;
        }
      } else if (cur->down) {
        if (((player->x_pos > cur->x_pos + GOBLIN_HITBOX_OFFSET &&
              player->x_pos <
                  cur->x_pos + GOBLIN_HITBOX_OFFSET + GOBLIN_HITBOX_WIDTH) ||
             (player->x_pos + player->width >
                  cur->x_pos + GOBLIN_HITBOX_OFFSET &&
              player->x_pos + player->width <
                  cur->x_pos + GOBLIN_HITBOX_OFFSET + GOBLIN_HITBOX_WIDTH)) &&
            ((player->y_pos > cur->y_pos + GOBLIN_HITBOX_OFFSET +
                                  GOBLIN_HITBOX_HEIGHT -
                                  GOBLIN_ATTACK_BOX_PADDING &&
              player->y_pos < cur->y_pos + GOBLIN_HITBOX_OFFSET +
                                  GOBLIN_HITBOX_HEIGHT +
                                  GOBLIN_ATTACK_BOX_HEIGHT) ||
             (player->y_pos + player->height >
                  cur->y_pos + GOBLIN_HITBOX_OFFSET + GOBLIN_HITBOX_HEIGHT -
                      GOBLIN_ATTACK_BOX_PADDING &&
              player->y_pos + player->height <
                  cur->y_pos + GOBLIN_HITBOX_OFFSET + GOBLIN_HITBOX_HEIGHT +
                      GOBLIN_ATTACK_BOX_HEIGHT))) {
          return true;
        }
      } else {
        if (((player->x_pos > cur->x_pos + GOBLIN_HITBOX_OFFSET &&
              player->x_pos <
                  cur->x_pos + GOBLIN_HITBOX_OFFSET + GOBLIN_HITBOX_WIDTH) ||
             (player->x_pos + player->width >
                  cur->x_pos + GOBLIN_HITBOX_OFFSET &&
              player->x_pos + player->width <
                  cur->x_pos + GOBLIN_HITBOX_OFFSET + GOBLIN_HITBOX_WIDTH)) &&
            ((player->y_pos > cur->y_pos + GOBLIN_HITBOX_OFFSET -
                                  GOBLIN_ATTACK_BOX_HEIGHT &&
              player->y_pos < cur->y_pos + GOBLIN_HITBOX_OFFSET +
                                  GOBLIN_ATTACK_BOX_PADDING) ||
             (player->y_pos + player->height > cur->y_pos +
                                                   GOBLIN_HITBOX_OFFSET -
                                                   GOBLIN_ATTACK_BOX_HEIGHT &&
              player->y_pos + player->height <
                  cur->y_pos + GOBLIN_HITBOX_OFFSET +
                      GOBLIN_ATTACK_BOX_PADDING))) {
          return true;
        }
      }
    }
    cur = cur->next;
  }
  // No collision
  return false;
}
