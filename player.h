#ifndef PLAYER_H
#define PLAYER_H

#include "room.h"

typedef struct Player {
    int health;
    int strength;
    int shield;
    char *inventory[MAX_INVENTORY];
    int inventoryCount;
    Room *currentRoom;
} Player;

void initPlayer(Player *player);
void showInventory(Player *player);
void showHealth(Player *player);
void pickupItem(Player *player, Room *room, const char *item);
void attack(Player *player);
void usePotion(Player *player);


#endif
