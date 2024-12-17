#include "creature.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

Creature *createCreature()
{
    Creature *c = (Creature *)malloc(sizeof(Creature));
    if (!c)
        exit(EXIT_FAILURE);
    char *names[] = {"Spider", "Orc", "Werewolf", "Zombie", "Vampire"};
    int index = rand() % 5;
    c->name = strdup(names[index]);

    if (strcmp(c->name, "Spider") == 0)
    {
        c->health = 25;
        c->attackPower = 10;
    }
    else if (strcmp(c->name, "Orc") == 0)
    {
        c->health = 30;
        c->attackPower = 15;
    }
    else if (strcmp(c->name, "Werewolf") == 0)
    {
        c->health = 35;
        c->attackPower = 20;
    }
    else if (strcmp(c->name, "Zombie") == 0)
    {
        c->health = 40;
        c->attackPower = 25;
    }
    else if (strcmp(c->name, "Vampire") == 0)
    {
        c->health = 45;
        c->attackPower = 30;
    }

    return c;
}

Creature *createFinalBoss()
{
    Creature *c = (Creature *)malloc(sizeof(Creature));
    if (!c)
        exit(1);
    c->name = strdup("Final Boss");
    c->health = 50;
    c->attackPower = 50;
    return c;
}

void freeCreature(Creature *c)
{
    if (!c)
        return;
    free(c->name);
    free(c);
}
