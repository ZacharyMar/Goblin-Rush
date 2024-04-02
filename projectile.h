#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "structs.h"
#include "enemies.h"

// Struct to store projectile information
typedef struct Projectile {
  // Position of projectile
  float x_pos;
  float y_pos;

  // Dimensions
  unsigned short int width;
  unsigned short int height;

  // Direction of travel
  float dx;
  float dy;

  // Used to traverse to next projectile in list
  struct Projectile* next;
} Projectile;

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

// Returns if a projectile collided with an enemy
bool checkProjectileCollision(Projectile * projectile, GoblinList* g_list);

// Performs all updates regarding collision with projectiles and enemies
void enemyProjectileCollisionUpdate(ProjectileList* p_list, GoblinList* g_list);

#endif