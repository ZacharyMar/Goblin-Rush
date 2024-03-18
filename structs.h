#ifndef STRUCTS_H
#define STRUCTS_H

// Struct to store player information
struct Player{
    // Position of player
    unsigned int x_pos;
    unsigned int y_pos;
    // Score of the player
    unsigned int score;
    // Health of player
    unsigned char health;
    // Movement speed of player
    unsigned char vel;
    // Stores state the player is in
    // State is used to determine animation to play
    // TODO - encode states
    // e.g. 0x01 - idle, 0x02 - move, ...
    unsigned char player_state;
    // Cooldown for shooting
    unsigned char shoot_cooldown;
    // Cooldown for ability'
    unsigned char ability_cooldown;
    // Used to determine the current frame of the animation
    unsigned char current_frame;
    // Used to determine the number of frames in the current animation being played
    unsigned char frames_in_animation;
};

// Struct to store cursor information
struct Cursor{
    // Position of cursor
    unsigned int x_pos;
    unsigned int y_pos;
};  
#endif