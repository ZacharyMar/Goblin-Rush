#include "constants.h"
#include "structs.h"
#include "devices.h"
#include "address_map_nios2.h"

int main(){
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
        .score = 0, 
        .health = 100, 
        .vel = 5, 
        .right = 0, 
        .left = 0, 
        .down = 0, 
        .up = 0, 
        .state = IDLE, 
        .shoot_cooldown = 0, 
        .ability_cooldown = 0, 
        .current_frame = 0, 
        .frames_in_animation = 10
    };

    Cursor cursor = {
        .x_pos = 0,
        .y_pos = 0,
        .vel = 5
    };

    while(1){
        // Get mouse data
        MouseData mouse_data = get_mouse_data();
        // Get key pressed
        KEYS key_pressed = get_keyboard_data();
    }
}
