#include "projectile.h"

#include <math.h>
#include <stdlib.h>

#include "constants.h"

// Creates projectile object
bool createProjectile(ProjectileList* list, const Player player,
                      const Cursor cursor) {
  // Do no create more projectiles if at max
  if (list->count >= MAX_NUM_PROJECTILES) {
    return false;
  }

  // Try to allocate memory
  Projectile* projectile = malloc(sizeof(Projectile));
  // Allocation failed
  if (projectile == NULL) {
    return false;
  }

  // Direction of travel calculated as unit vector from center of player to
  // center of cursor Get vector
  float dx = (cursor.x_pos + (cursor.width >> 1)) - (player.x_pos + (player.width >> 1));
  float dy = (cursor.y_pos + (cursor.height >> 1)) - (player.y_pos + (player.height >> 1));

  // Normalize vector
  float magnitude = sqrt((dx * dx) + (dy * dy));
  dx /= magnitude;
  dy /= magnitude;

  // Load information
  projectile->dx = dx;
  projectile->dy = dy;
  projectile->x_pos = player.x_pos + (player.width >> 1);
  projectile->y_pos = player.y_pos + (player.height >> 1);
  projectile->next = NULL;
  projectile->height = PROJECTILE_WIDTH;
  projectile->width = PROJECTILE_WIDTH;

  // Insert projectile into list
  // List is currently empty
  if (list->head == NULL) {
    list->head = projectile;
    list->tail = projectile;
  }
  // List is populated
  else {
    list->tail->next = projectile;
    list->tail = projectile;
  }

  // Update count
  list->count++;

  return true;
}

// Updates projectile position
void updateProjectilePosition(ProjectileList* list) {
  // Traverse list
  Projectile* cur = list->head;
  Projectile* prev = NULL;
  while (cur != NULL) {
    // Update information
    cur->x_pos += cur->dx;
    cur->y_pos += cur->dy;

    // Check projectile now off screen
    if (cur->x_pos < 0 || cur->x_pos + cur->width > SCREEN_WIDTH ||
        cur->y_pos < 0 || cur->y_pos + cur->height > SCREEN_HEIGHT) {
      // Delete projectile since off screen
      // Need to delete head
      if (prev == NULL) {
        // Set node to delete in temp
        prev = cur;
        // Traverse
        cur = cur->next;
        // Update new head of list
        list->head = cur;
        // Head was tail as well
        list->tail = prev == list->tail ? cur : list->tail;
        // Free memory
        free(prev);
        prev = NULL;
      }
      // Node to delete is not the head
      else {
        prev->next = cur->next;
        // Tail node is being deleted
        if (prev->next == NULL) {
          list->tail = prev;
        }
        free(cur);
        cur = prev->next;
      }

      // Update count
      list->count--;
    }
    // traverse list
    else {
      prev = cur;
      cur = cur->next;
    }
  }
}

// Checks if projectile collides with enemy
bool checkProjectileCollision();

// Used to free memory use for projectile list
// NOTE: After calling, the list pointer should not be used again!
void freeProjectileList(ProjectileList* list){
    // Iterate through list to free each projectile
    Projectile* cur = list->head;
    while (cur != NULL){
        Projectile* tmp = cur;
        cur = cur->next;
        free(tmp);
    }

    // Free list pointer itself
    free(list);
}