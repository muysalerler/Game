#include "room.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

static char *randomItem()
{
    char *items[] = {"Sword", "Shield", "Potion", "Miracle Fruit"};
    return items[rand() % 4];
}

Room *createRoom(const char *description)
{
    Room *room = (Room *)malloc(sizeof(Room));
    if (!room)
        exit(1);
    room->description = strdup(description);
    room->up = room->down = room->left = room->right = NULL;

    room->itemCount = rand() % MAX_INVENTORY;
    for (int i = 0; i < room->itemCount; i++)
    {
        room->items[i] = strdup(randomItem());
    }

    if (rand() % 2 == 0)
    {
        room->creature = createCreature();
    }
    else
    {
        room->creature = NULL;
    }

    int e = rand() % 10;
    RoomEvent possibleEvents[10] = {
        EVENT_GOOD_1, EVENT_GOOD_2, EVENT_GOOD_3, EVENT_GOOD_4, EVENT_GOOD_5,
        EVENT_BAD_1, EVENT_BAD_2, EVENT_BAD_3, EVENT_BAD_4, EVENT_BAD_5};
    room->event = possibleEvents[e];
    room->eventTriggered = 0;

    room->x = 0;
    room->y = 0;

    return room;
}

void freeRoom(Room *room)
{
    if (!room)
        return;
    free(room->description);
    for (int i = 0; i < room->itemCount; i++)
        free(room->items[i]);
    if (room->creature)
        freeCreature(room->creature);
    free(room);
}

int connectRooms(Room *src, Room *dest, const char *direction)
{
    if (strcmp(direction, "up") == 0)
    {
        if (src->up)
            return 0;
        src->up = dest;
        dest->down = src;
        dest->x = src->x;
        dest->y = src->y - 1;
    }
    else if (strcmp(direction, "down") == 0)
    {
        if (src->down)
            return 0;
        src->down = dest;
        dest->up = src;
        dest->x = src->x;
        dest->y = src->y + 1;
    }
    else if (strcmp(direction, "left") == 0)
    {
        if (src->left)
            return 0;
        src->left = dest;
        dest->right = src;
        dest->x = src->x - 1;
        dest->y = src->y;
    }
    else if (strcmp(direction, "right") == 0)
    {
        if (src->right)
            return 0;
        src->right = dest;
        dest->left = src;
        dest->x = src->x + 1;
        dest->y = src->y;
    }
    else
    {
        return 0;
    }
    return 1;
}
