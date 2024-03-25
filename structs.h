#ifndef STRUCTS_H
#define STRUCTS_H

// Struct to store player information
typedef struct Player{
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
    unsigned char state;
    // Cooldown for shooting
    unsigned char shoot_cooldown;
    // Cooldown for ability'
    unsigned char ability_cooldown;
    // Used to determine the current frame of the animation
    unsigned char current_frame;
    // Used to determine the number of frames in the current animation being played
    unsigned char frames_in_animation;
}Player;

// Struct to store cursor information
typedef struct Cursor{
    // Position of cursor
    unsigned int x_pos;
    unsigned int y_pos;
}Cursor;  

typedef struct Goblin{
    // Location of goblin
    unsigned int x_pos;
    unsigned int y_pos;

    // health of the goblin
    unsigned char health;
    // speed of the goblin
    unsigned char speed;
    // current goblin state
    unsigned char player_state;

    // Used to determine the current frame of the animation
    unsigned char current_frame;
    // Used to determine the number of frames in the current animation being played
    unsigned char frames_in_animation;
    // next pointer
    Goblin* next;
}Goblin;

typedef struct Bee{
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
    // Used to determine the number of frames in the current animation being played
    unsigned char frames_in_animation;
    // next pointer
    Bee* next;
}Bee;

typedef struct Wolf{
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
    // Used to determine the number of frames in the current animation being played
    unsigned char frames_in_animation;
    // next pointer
    Wolf* next;
}Wolf;
#endif