#include "constants.h"
#include "structs.h"
#include "devices.h"
#include "player.h"
#include "projectile.h"
#include <stdlib.h>

int main(){
    // Initial setup

    // Initialize devices
    init_mouse();
    init_keyboard();
    stop_timer();

    // Clear garbage from PS2 FIFOs
    get_mouse_data();
    get_keyboard_data();

    // Create instances of player and cursor
    Player player = {
        .x_pos = 0, 
        .y_pos = 0,
        .height = 48,
        .width = 48,
        .score = 0, 
        .shoot_cooldown = 0,
        .health = 100, 
        .vel = 5, 
        .right = false, 
        .left = false, 
        .down = false, 
        .up = false, 
        .state = IDLE, 
        .canEvade = false, 
        .current_frame = 0, 
        .frames_in_animation = 10
    };

    Cursor cursor = {
        .x_pos = 0,
        .y_pos = 0,
        .width = 10,
        .height = 10,
        .vel = 5
    };

    // Create list of projectiles
    ProjectileList* projectile_list = malloc(sizeof(ProjectileList));
    // Unable to allocate memory - Error
    if (projectile_list == NULL){
        return -1;
    }
    // Initalize pointers for list as NULL
    projectile_list->head = NULL;
    projectile_list->tail = NULL;
    projectile_list->count = 0;

    while(1){
        // Get mouse data
        MouseData mouse_data = get_mouse_data();
        // Get key pressed
        KEYS key_pressed = get_keyboard_data();

        // Update player and cursor based on inputs
        updatePlayer(&player, mouse_data, key_pressed);
        updateCursor(&cursor, mouse_data);

        // TODO:
        // Update enemies

        // Create new projectile if player is currently shooting
        if (player.state == SHOOTING){
            createProjectile(projectile_list, player, cursor);
        }
        // Update position of projectiles
        updateProjectilePosition(projectile_list);

        // Any other updates???

        // Collision detection

        // Refresh screen
    }

    // Deallocate memory
    freeProjectileList(projectile_list);
}
