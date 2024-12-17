#ifndef ROOM_H
#define ROOM_H

#include "creature.h"

#define MAX_INVENTORY 4
#define MAX_DESC 256

typedef enum
{
    EVENT_NONE,
    EVENT_GOOD_1,
    EVENT_GOOD_2,
    EVENT_GOOD_3,
    EVENT_GOOD_4,
    EVENT_GOOD_5,
    EVENT_BAD_1,
    EVENT_BAD_2,
    EVENT_BAD_3,
    EVENT_BAD_4,
    EVENT_BAD_5
} RoomEvent;

typedef struct Room
{
    char *description;
    struct Room *up;
    struct Room *down;
    struct Room *left;
    struct Room *right;
    char *items[MAX_INVENTORY];
    int itemCount;
    Creature *creature;
    int x, y;
    RoomEvent event;
    int eventTriggered;
} Room;

Room *createRoom(const char *description);
void freeRoom(Room *room);
int connectRooms(Room *src, Room *dest, const char *direction);

#endif
