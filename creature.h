#ifndef CREATURE_H
#define CREATURE_H

typedef struct Creature
{
    char *name;
    int health;
    int attackPower;
} Creature;

Creature *createCreature();
Creature *createFinalBoss();
void freeCreature(Creature *c);

#endif
