#include "player.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initPlayer(Player *player)
{
    player->health = 100;
    player->strength = 20;
    player->shield = 0;
    player->inventoryCount = 0;
    player->currentRoom = NULL;
}

void showInventory(Player *player)
{
    printf("Your inventory:\n");
    if (player->inventoryCount == 0)
    {
        printf("Your inventory is empty.\n");
        return;
    }
    for (int i = 0; i < player->inventoryCount; i++)
    {
        printf("- %s\n", player->inventory[i]);
    }
}

void showHealth(Player *player)
{
    printf("Your health: %d\n", player->health);
    printf("Your shield: %d\n", player->shield);
    printf("Your strength: %d\n", player->strength);
}

void pickupItem(Player *player, Room *room, const char *item)
{
    if (room->creature)
    {
        printf("You cannot pick up items while a monster is present. Defeat it first.\n");
        return;
    }

    if (player->inventoryCount >= MAX_INVENTORY)
    {
        printf("Your inventory is full!\n");
        return;
    }

    int idx = -1;
    for (int i = 0; i < room->itemCount; i++)
    {
        if (strcmp(room->items[i], item) == 0)
        {
            idx = i;
            break;
        }
    }

    if (idx < 0)
    {
        printf("That item is not here.\n");
        return;
    }

    if (strcmp(item, "Shield") == 0)
    {
        player->shield += 5;
        printf("You picked up a Shield! Shield: %d\n", player->shield);
    }
    else if (strcmp(item, "Sword") == 0)
    {
        player->strength += 5;
        player->inventory[player->inventoryCount++] = strdup(item);
        printf("You picked up a Sword! Strength: %d\n", player->strength);
    }
    else
    {
        player->inventory[player->inventoryCount++] = strdup(item);
        printf("You picked up: %s\n", item);
    }

    free(room->items[idx]);
    for (int j = idx; j < room->itemCount - 1; j++)
    {
        room->items[j] = room->items[j + 1];
    }
    room->itemCount--;
}

void useMiracleFruit(Player *player) {
    int idx = -1;
    for (int i = 0; i < player->inventoryCount; i++) {
        if (strcasecmp(player->inventory[i], "Miracle Fruit") == 0) {
            idx = i;
            break;
        }
    }
    if (idx == -1) {
        printf("You don't have a MiracleFruit!\n");
        return;
    }

    printf("The MiracleFruit might be dangerous. Are you sure you want to eat it? (y/n): ");
    char answer[16];
    if (!fgets(answer, sizeof(answer), stdin)) {
        printf("You hesitate and don't eat it.\n");
        return;
    }
    answer[strcspn(answer, "\n")] = 0;

    if (tolower(answer[0]) != 'y') {
        printf("You decide not to eat the MiracleFruit.\n");
        return;
    }

    int effect = rand() % 2; 
    if (effect == 0) {
        printf("You eat the MiracleFruit... and suddenly collapse!\n");
        printf("Game Over!\n");
        exit(0);
    } else {
        player->strength += 50;
        printf("You eat the MiracleFruit... You feel incredible power surging through you!\n");
        printf("Strength +50!\n");
    }

    free(player->inventory[idx]);
    for (int j = idx; j < player->inventoryCount - 1; j++) {
        player->inventory[j] = player->inventory[j+1];
    }
    player->inventoryCount--;
}

void usePotion(Player *player)
{
    int potionIndex = -1;
    for (int i = 0; i < player->inventoryCount; i++)
    {
        if (strcmp(player->inventory[i], "Potion") == 0)
        {
            potionIndex = i;
            break;
        }
    }

    if (potionIndex == -1)
    {
        printf("You have no potion to use!\n");
        return;
    }

    free(player->inventory[potionIndex]);
    for (int j = potionIndex; j < player->inventoryCount - 1; j++)
    {
        player->inventory[j] = player->inventory[j + 1];
    }
    player->inventoryCount--;

    int positive = rand() % 2; 
   
    int effectType = rand() % 3;

    int amount = (effectType == 0) ? 20 : 5; 
    if (!positive)
        amount = -amount;

    if (effectType == 0)
    {
        player->health += amount;
        if (player->health < 0)
            player->health = 0;
        printf("The potion affects your health by %d. Current health: %d\n", amount, player->health);
    }
    else if (effectType == 1)
    {
        player->shield += amount;
        if (player->shield < 0)
            player->shield = 0;
        printf("The potion affects your shield by %d. Current shield: %d\n", amount, player->shield);
    }
    else
    {
        player->strength += amount;
        if (player->strength < 0)
            player->strength = 0;
        printf("The potion affects your strength by %d. Current strength: %d\n", amount, player->strength);
    }
}

void dropItem(Player *player, Room *room, const char *item)
{
    int idx = -1;
    for (int i = 0; i < player->inventoryCount; i++)
    {
        if (strcmp(player->inventory[i], item) == 0)
        {
            idx = i;
            break;
        }
    }

    if (idx == -1)
    {
        printf("You don't have that item!\n");
        return;
    }

    if (strcmp(item, "Sword") == 0)
    {
        printf("You dropped the Sword. Strength -5.\n");
        player->strength -= 5;
        if (player->strength < 0)
            player->strength = 0;
    }
    else if (strcmp(item, "Miracle Fruit") == 0)
    {
        printf("You dropped the Miracle Fruit.\n");
    }
    else if (strcmp(item, "Potion") == 0)
    {
        printf("You dropped a Potion.\n");
    }
    if (room->itemCount < MAX_INVENTORY)
    {
        room->items[room->itemCount++] = strdup(item);
    }
    else
    {
        printf("The room is full, cannot drop here!\n");
    }

    free(player->inventory[idx]);
    for (int j = idx; j < player->inventoryCount - 1; j++)
    {
        player->inventory[j] = player->inventory[j + 1];
    }
    player->inventoryCount--;
}

void attack(Player *player)
{
    if (!player->currentRoom->creature)
    {
        printf("There is nothing to attack here.\n");
        return;
    }

    Creature *c = player->currentRoom->creature;

    printf("You attacked with your sword to destroy the monster!\n");

    c->health -= player->strength;

    if (c->health <= 0)
    {
        printf("You have defeated the %s!\n", c->name);
        freeCreature(c);
        player->currentRoom->creature = NULL;
    }
    else
    {
        if (strcmp(c->name, "Vampire") == 0)
        {
            printf("The Vampire bites your neck!\n");
        }
        else if (strcmp(c->name, "Spider") == 0)
        {
            printf("The spider bit you!\n");
        }
        else if (strcmp(c->name, "Orc") == 0)
        {
            printf("The Orc hit you in the head with a hammer!\n");
        }
        else if (strcmp(c->name, "Werewolf") == 0)
        {
            printf("The Werewolf swung a claw at your chest!\n");
        }
        else if (strcmp(c->name, "Zombie") == 0)
            printf("The Zombie took a piece of your breast and ate it!\n");

        if (player->shield > 0)
        {
            int damageToShield = c->attackPower;
            player->shield -= damageToShield;
            if (player->shield < 0)
            {
                player->health += player->shield;
                player->shield = 0;
            }
            printf("Your shield absorbed the damage! Remaining shield: %d\n", player->shield);
        }
        else
        {
            player->health -= c->attackPower;
            printf("You took %d damage!\n", c->attackPower);
        }

        if (player->health <= 0)
        {
            printf("You have been killed by the %s! Game Over.\n", c->name);
            exit(0);
        }

        if (strcmp(c->name, "Final Boss") == 0 && c->health <= 0)
        {
            printf("You have defeated the Final Boss!\n");
        }
    }
}
