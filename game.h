#ifndef GAME_H
#define GAME_H

#include "player.h"


#define MAX_ROOMS 2

typedef struct Game
{
    Player player;
    Room *rooms[MAX_ROOMS];
    int roomCount;
} Game;

void initGame(Game *g);
void freeGame(Game *g);
void gameRun(Game *g);

#endif
