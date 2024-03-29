#include "projectile.h"

#include <math.h>
#include <stdlib.h>

#include "constants.h"

// Creates projectile object
bool createProjectile(ProjectileList* list, const Player player,
                      const Cursor cursor) {
  // Try to allocate memory
  Projectile* projectile = malloc(sizeof(Projectile));
  // Allocation failed
  if (projectile == NULL) {
    return false;
  }

  // Direction of travel calculated as unit vector from center of player to
  // center of cursor Get vector
  int dx = (cursor.x_pos + cursor.width) - (player.x_pos + player.width);
  int dy = (cursor.y_pos + cursor.height) - (player.y_pos + player.height);

  // Normalize vector
  double magnitude = sqrt((dx * dx) + (dy * dy));
  dx /= magnitude;
  dy /= magnitude;

  // Load information
  projectile->dx = dx;
  projectile->dy = dy;
  projectile->x_pos = player.x_pos + player.width;
  projectile->y_pos = player.y_pos + player.height;
  projectile->next = NULL;

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