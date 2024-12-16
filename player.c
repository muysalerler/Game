#include "player.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initPlayer(Player *player) {
    player->health = 100;
    player->strength = 10; // Son kodda strength başlangıçta 50 idi.
    player->shield = 0;
    player->inventoryCount = 0;
    player->currentRoom = NULL;
}

void showInventory(Player *player) {
    printf("Your inventory:\n");
    if (player->inventoryCount == 0) {
        printf("Your inventory is empty.\n");
        return;
    }
    for (int i = 0; i < player->inventoryCount; i++) {
        printf("- %s\n", player->inventory[i]);
    }
}

void showHealth(Player *player) {
    printf("Your health: %d\n", player->health);
    printf("Your shield: %d\n", player->shield);
    printf("Your strength: %d\n", player->strength);
}

void pickupItem(Player *player, Room *room, const char *item) {
    if (room->creature) {
        printf("You cannot pick up items while a monster is present. Defeat it first.\n");
        return;
    }

    if (player->inventoryCount >= MAX_INVENTORY) {
        printf("Your inventory is full!\n");
        return;
    }

    int idx = -1;
    for (int i = 0; i < room->itemCount; i++) {
        if (strcmp(room->items[i], item) == 0) {idx = i;break;}
    }

    if (idx < 0) {
        printf("That item is not here.\n");
        return;
    }

    if (strcmp(item, "Shield") == 0) {
        player->shield += 5;
        printf("You picked up a Shield! Shield: %d\n", player->shield);
    } else if (strcmp(item, "Sword") == 0) {
        player->strength += 5;
        printf("You picked up a Sword! Strength: %d\n", player->strength);
    } else {
        player->inventory[player->inventoryCount++] = strdup(item);
        printf("You picked up: %s\n", item);
    }

    free(room->items[idx]);
    for (int j = idx; j < room->itemCount - 1; j++) {
        room->items[j] = room->items[j+1];
    }
    room->itemCount--;
}

void usePotion(Player *player) {
    // Envanterde "Potion" var mı?
    int potionIndex = -1;
    for (int i = 0; i < player->inventoryCount; i++) {
        if (strcmp(player->inventory[i], "Potion") == 0) {
            potionIndex = i;
            break;
        }
    }

    if (potionIndex == -1) {
        printf("You have no potion to use!\n");
        return;
    }

    // Potion'u envanterden çıkar
    free(player->inventory[potionIndex]);
    for (int j = potionIndex; j < player->inventoryCount - 1; j++) {
        player->inventory[j] = player->inventory[j + 1];
    }
    player->inventoryCount--;

    // Etkiyi belirle
    // %50 pozitif, %50 negatif etki
    int positive = rand() % 2; // 0 veya 1
    // Etkilerden birini seç
    int effectType = rand() % 3; // 0=health,1=shield,2=strength

    int amount = (effectType == 0) ? 20 : 5; // can +20, diğerleri +5
    if (!positive) amount = -amount;

    // Etkiyi uygula
    if (effectType == 0) {
        // Health
        player->health += amount;
        if (player->health < 0) player->health = 0;
        printf("The potion affects your health by %d. Current health: %d\n", amount, player->health);
    } else if (effectType == 1) {
        // Shield
        player->shield += amount;
        if (player->shield < 0) player->shield = 0;
        printf("The potion affects your shield by %d. Current shield: %d\n", amount, player->shield);
    } else {
        // Strength
        player->strength += amount;
        if (player->strength < 0) player->strength = 0;
        printf("The potion affects your strength by %d. Current strength: %d\n", amount, player->strength);
    }
}

void attack(Player *player) {
    if (!player->currentRoom->creature) {
        printf("There is nothing to attack here.\n");
        return;
    }

    Creature *c = player->currentRoom->creature;
    printf("You attack the %s!\n", c->name);
    c->health -= player->strength;

    if (c->health <= 0) {
        printf("You have defeated the %s!\n", c->name);
        freeCreature(c);
        player->currentRoom->creature = NULL;
    } else {
        printf("The %s attacks you!\n", c->name);
        if (player->shield > 0) {
            int damageToShield = c->attackPower;
            player->shield -= damageToShield;
            if (player->shield < 0) {
                player->health += player->shield;
                player->shield = 0;
            }
            printf("Your shield absorbed the damage! Remaining shield: %d\n", player->shield);
        } else {
            player->health -= c->attackPower;
            printf("You took %d damage! Remaining health: %d\n", c->attackPower, player->health);
        }

        if (player->health <= 0) {
            printf("You have been killed by the %s! Game Over.\n", c->name);
            exit(0);
        }
    }
}
