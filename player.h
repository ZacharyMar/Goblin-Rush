#ifndef PLAYER_H
#define PLAYER_H

#include "structs.h"

/***********
 * Functions used related to player object
*/

// Updates the player's position, state, and cooldowns
void updatePlayer(Player* player, MouseData mouse, KeyboardData keyboard);

// Updates the player's cursor
void updateCursor(Cursor* cursor, MouseData mouse);

// Function called to handle when player hit by enemy
void collisionHandler(Player* player);

#endif