#include "player.h"
#include "devices.h"
#include "constants.h"
#include "projectile.h"

// Updates the player's position, state, and cooldowns
void updatePlayer(Player* player, MouseData mouse, KEYS key_pressed){
    /**** Keyboard input ********/
    // WASD control movement of player
    // SPACE controls evasion state of player

    // Update position and state based on key press
    switch (key_pressed){
    // Move player up
    case W:
        player->y_pos -= player->vel;
        player->up = true;
        player->down = false;
        player->left = false;
        player->right = false;
        player->state = player->state == EVASION ? EVASION : MOVING;
        break;
    // Move left
    case A:
        player->x_pos -= player->vel;
        player->left = true;
        player->right = false;
        player->up = false;
        player->down = false;
        player->state = player->state == EVASION ? EVASION : MOVING;
        break;
    // Move right
    case D:
        player->x_pos += player->vel;
        player->right = true;
        player->left = false;
        player->down = false;
        player->up = false;
        player->state = player->state == EVASION ? EVASION : MOVING;
        break;
    // Move down
    case S:
        player->y_pos += player->vel;
        player->down = true;
        player->up = false;
        player->left = false;
        player->right = false;
        player->state = player->state == EVASION ? EVASION : MOVING;
        break;
    // Evasion
    case SPACE:
        // Enter evasion if cooldown is set to zero and in a valid state
        // Player enters evasion state for 2 seconds
        if (player->canEvade && player->health > 0){
            player->state = EVASION;
            player->canEvade = false;
            // Set timer for 2 seconds
            set_timer(2000);
        } 
        break;
    
    // No meaningful input
    default:
        player->state = IDLE;
        break;
    }

    // Check out of bounds for player
    // Left boundary
    if (player->x_pos < 0){
        player->x_pos = 0;
    }
    // Right boundary
    else if (player->x_pos + player->width > SCREEN_WIDTH){
        player->x_pos = SCREEN_WIDTH - player->width;
    }
    // Top boundary
    if (player->y_pos < 0){
        player->y_pos = 0;
    }
    // Bottom boundary
    else if (player->y_pos + player->height > SCREEN_HEIGHT){
        player->y_pos = SCREEN_HEIGHT - player->height;
    }

    // Check for ability cool down
    bool timerDone = timer_done();
    // Evasion wore off
    if (timerDone && player->state == EVASION){
        // Player is no longer in evasion state
        player->state = MOVING;
        // Set timer for cooldown for 30s
        set_timer(30000);
    }
    // Cooldown is finished for evasion
    else if (timerDone){
        player->canEvade = true;
    }


    /*********** MOUSE ****************/
    // LMB used to shoot
    
    // LMB clicked -> player enters shooting state
    // Can only shoot when not evading and shooting cooldown is finished
    if (mouse.LMB && player->state != EVASION && player->shoot_cooldown == 0){
        player->state = SHOOTING;
        player->shoot_cooldown = SHOOTING_COOLDOWN + 1;
    }

    // Update shoot cooldown
    if (player->shoot_cooldown > 0){
        player->shoot_cooldown--;
    }

    // TODO - update player's score
}

// Updates the player's cursor
void updateCursor(Cursor* cursor, MouseData mouse){
    // Update the cursor's position based upon mouse movement
    cursor->x_pos += mouse.dx;
    cursor->y_pos += mouse.dy;

    // Check for out of bounds
    if (cursor->x_pos < 0) cursor->x_pos = 0;
    else if (cursor->x_pos + cursor->width > SCREEN_WIDTH) cursor->x_pos = SCREEN_HEIGHT - cursor->width;
    if (cursor->y_pos < 0) cursor->y_pos = 0;
    else if (cursor->y_pos + cursor->height > SCREEN_HEIGHT) cursor->y_pos = SCREEN_HEIGHT - cursor->height;
}
