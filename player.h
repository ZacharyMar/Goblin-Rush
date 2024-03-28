#ifndef PLAYER_H
#define PLAYER_H

#include "structs.h"

/***********
 * Functions used related to player object
*/

// Updates the player's position, state, and cooldowns
void updatePlayer(Player* player, MouseData mouse, KEYS key_pressed);

// Updates the player's cursor
void updateCursor(Cursor* cursor, MouseData mouse);

#endif