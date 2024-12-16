#ifndef ROOM_H
#define ROOM_H

#include "creature.h"

#define MAX_INVENTORY 5
#define MAX_DESC 256

typedef struct Room {
    char *description;
    struct Room *up;
    struct Room *down;
    struct Room *left;
    struct Room *right;
    char *items[MAX_INVENTORY];
    int itemCount;
    Creature *creature;
    int x, y;
} Room;

Room *createRoom(const char *description);
void freeRoom(Room *room);
int connectRooms(Room *src, Room *dest, const char *direction);

#endif
