#include "enemies.h"
#include "constants.h"
#include <stdlib.h>

// returns whether or not a sprite is within the screen
bool in_bounds(int x, int y, unsigned int width, unsigned int height){
    return x > BOUNDARY && x + width + BOUNDARY < SCREEN_WIDTH && y > BOUNDARY && y + height + BOUNDARY < SCREEN_HEIGHT;
}

// draws an enemy sprite starting from its top left corner (x_offset, y_offset)
void draw_enemy_sprite_frame(unsigned short int sprite_ptr[][288], unsigned int x_offset, unsigned int y_offset, unsigned int frame_idx, unsigned int num_frames, bool reverse){
    // iterate through the height of a frame
    unsigned int sheet_height = 48;
    unsigned int sheet_width = ARRAYSIZE(sprite_ptr[0]);
    unsigned int frame_width = sheet_width / num_frames;
    for(unsigned int i = 0; i < sheet_height; i++){
        // iterate through the width of a frame
        for(unsigned int j = 0; j < frame_width; j++){
            // Calculate the index to read from the sprite sheet based on direction
            unsigned int sprite_index = reverse ? (frame_width - 1 - j) : j;
            sprite_index += (frame_width * frame_idx);

            // remove background and draw pixel if it's not white (0xFFFF)
            if(sprite_ptr[i][sprite_index] != 0xFFFF){
                plot_pixel(x_offset + j, y_offset + i, sprite_ptr[i][sprite_index]);
            }
        }
    }
}

// Function to create a new Goblin
Goblin* create_goblin(unsigned int x, unsigned int y, unsigned char health, 
                      unsigned char speed, unsigned char state, 
                      unsigned char frame, unsigned char frames, bool right, bool left, bool up, bool down) {
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
void add_goblin(Goblin** head, Goblin* new_goblin) {
    if (*head == NULL) {
        // If the list is empty, the new goblin becomes the head
        *head = new_goblin;
    } else {
        // Otherwise, find the end of the list and add the new goblin there
        Goblin* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_goblin;
    }
}
