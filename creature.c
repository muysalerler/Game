#include "creature.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

Creature *createCreature() {
    Creature *c = (Creature *)malloc(sizeof(Creature));
    if (!c) exit(1);
    char *names[] = {"Goblin", "Orc", "Troll", "Zombie", "Vampire"};
    c->name = strdup(names[rand() % 5]);
    c->health = rand() % 20 + 10;
    c->attackPower = rand() % 5 + 5;
    return c;
}

Creature *createFinalBoss() {
    Creature *c = (Creature *)malloc(sizeof(Creature));
    if (!c) exit(1);
    c->name = strdup("Final Boss");
    c->health = 50;
    c->attackPower = 15;
    return c;
}

void freeCreature(Creature *c) {
    if (!c) return;
    free(c->name);
    free(c);
}
