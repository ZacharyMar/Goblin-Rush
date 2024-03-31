#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "structs.h"

// Struct to store projectile information
typedef struct Projectile {
  // Position of projectile
  unsigned int x_pos;
  unsigned int y_pos;

  // Dimensions
  unsigned short int width;
  unsigned short int height;

  // Direction of travel
  int dx;
  int dy;

  // Used to traverse to next projectile in list
  Projectile* next;
}Projectile;

// Linked list of projectiles
typedef struct ProjectileList {
  // Points to the head of the projectile list
  Projectile* head;
  // Points to tail of the projectile list
  Projectile* tail;
  // Keeps track of the total number of projectiles on screen
  int count;
}ProjectileList;

// Creates projectile object
bool createProjectile(ProjectileList* list, const Player player, const Cursor cursor);

// Updates projectile position
void updateProjectilePosition(ProjectileList* list);

// Used to free memory use for projectile list
void freeProjectileList(ProjectileList* list);

// Checks if projectile collides with enemy
bool checkProjectileCollision();

#endif