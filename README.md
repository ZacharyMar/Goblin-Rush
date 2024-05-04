# Goblin Rush

### By Zachary Mar and James Zhang

## Demo

Check out a demo of the game!

[![Watch the video](https://img.youtube.com/vi/gfCTr3Lthe4/default.jpg)](https://youtu.be/gfCTr3Lthe4)

## Description

Goblin Rush is a game created for the DE1-SoC. The program is written in C and can be uploaded to the FPGA using the Intel Monitor Program.

The objective of the game is to achieve as high of a score as possible. The player is rewarded score for staying alive; the player’s current score is visible on the hex display on the DE1-SoC. 

The player’s character is a mage who can be moved around using the W, A, S, and D keys on the PS2 keyboard; the keyboard allows for diagonal inputs as well (i.e. pressing W and D will move the player diagonally to the left and up). Additionally, the space bar can be used to make the player roll. When the player is rolling, they move faster and are invulnerable to any damage; there is a five-second cooldown on the player’s roll ability. 

The PS2 mouse is used to move the reticle on the screen. Pressing the left mouse button will make the player shoot a magic orb in the direction of the reticle. 

During the game, goblins will spawn at random locations and run towards the player to attack them. Goblins have randomly generated health and speed. If a goblin hits a player, the player will take damage. A goblin will take damage when hit by the player’s magic orb. The longer the player survives, the faster the goblins will spawn. The game ends when the player has lost all their health indicated by the health bar in the top left of the screen.


## Implemetation

Goblin Rush interfaces with PS2 keyboard and mouse, hex display, hardware timers, and video I/O. The PS2 keyboard and mouse are used to get player input and modify the player’s character and reticle. The hex display is used to display the player’s score. Both hardware timers are used; one is used for creating delays for reading from the PS2 FIFO and the other is used to accurately measure five seconds for the player’s roll cooldown. The video I/O utilizes double buffering to play smooth animation for the player, goblins, and magic orbs. 

The player and goblins use an FSM to determine what animation to display and how they should be updated in the game logic. The magic orbs and the goblins are stored in dynamically allocated linked lists. These linked lists allow for efficient lookup and deletion for handling drawing, updating, and collision detection.

Goblin Rush features various animations for the player and goblins to enhance the game. The player has animations for idle, moving, attacking, hurt, and death. The goblins have animations for moving and attacking in all directions. Additionally, the potions in the top left of the screen show the player’s current health.
