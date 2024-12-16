#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

// connectRooms fonksiyonu room.h / room.c içinde tanımlı, burada tekrar bildirmiyoruz!

static void generateRooms(Room **rooms);
static void printExitRoom(Room *room, Room **rooms, int roomCount);
static void printMap(Room **rooms, int roomCount);
static int movePlayer(Player *player, const char *direction);
static void look(Room *room);
static void listSavedGames();
static void saveGame(Player *player, Room **rooms, int roomCount, const char *filename);
static void loadGame(Player *player, Room **rooms, int *roomCount, const char *filename);
static int findRoomIndex(Room **rooms, int roomCount, Room *room);
static Room *findRoomByIndex(Room **rooms, int roomCount, int index);

void initGame(Game *g) {
    initPlayer(&g->player);
    generateRooms(g->rooms);
    g->roomCount = MAX_ROOMS;
    g->player.currentRoom = g->rooms[0];
}

void freeGame(Game *g) {
    for (int i = 0; i < g->roomCount; i++)
        freeRoom(g->rooms[i]);
}

void gameRun(Game *g) {
    printf("Welcome to the Dungeon!\n");
    printf("Commands: move <direction>, look, inventory, pickup <item>, attack, health,\n");
    printf("          save <filename>, load <filename>, list, exit\n");

    char command[MAX_DESC];
    while (1) {
        printf("\n> ");
        if(!fgets(command, MAX_DESC, stdin)) break;
        command[strcspn(command, "\n")] = 0;

        if (strcmp(command, "look") == 0) {
            look(g->player.currentRoom);
        } else if (strncmp(command, "move ", 5) == 0) {
            int reachedExit = movePlayer(&g->player, command+5);
            if (reachedExit) {
                if (g->player.currentRoom->creature) {
                    printf("A powerful %s blocks your exit! You must defeat it first.\n", g->player.currentRoom->creature->name);
                } else {
                    printExitRoom(g->player.currentRoom, g->rooms, g->roomCount);
                    break;
                }
            }
        } else if (strcmp(command, "inventory") == 0) {
            showInventory(&g->player);
        } else if (strcmp(command, "use potion") == 0) {
            usePotion(&g->player);
        } else if (strcmp(command, "health") == 0) {
            showHealth(&g->player);
        } else if (strcmp(command, "attack") == 0) {
            attack(&g->player);
        } else if (strncmp(command, "pickup ", 7) == 0) {
            pickupItem(&g->player, g->player.currentRoom, command+7);
        } else if (strncmp(command, "save ", 5) == 0) {
            saveGame(&g->player, g->rooms, g->roomCount, command+5);
        } else if (strncmp(command, "load ", 5) == 0) {
            loadGame(&g->player, g->rooms, &g->roomCount, command+5);
        } else if (strcmp(command, "list") == 0) {
            listSavedGames();
        } else if (strcmp(command, "exit") == 0) {
            printf("Exiting the game...\n");
            break;
        } else {
            printf("Unknown command: %s\n", command);
        }
    }
}

#include "room.h"
#include "creature.h"

static void generateRooms(Room **rooms) {
    rooms[0] = createRoom("Room 1");
    int placed = 1;
    rooms[0]->x = 0;
    rooms[0]->y = 0;

    char *directions[] = {"up", "down", "left", "right"};

    for (int i = 1; i < MAX_ROOMS; i++) {
        Room *newRoom;
        char desc[MAX_DESC];
        snprintf(desc, sizeof(desc), "Room %d", i+1);
        newRoom = createRoom(desc);

        int connected = 0;
        for (int tries = 0; tries < 100 && !connected; tries++) {
            int randRoomIndex = rand() % placed;
            Room *base = rooms[randRoomIndex];

            int dirOrder[4] = {0,1,2,3};
            for (int x = 0; x < 4; x++) {
                int r = rand() % 4;
                int temp = dirOrder[x];
                dirOrder[x] = dirOrder[r];
                dirOrder[r] = temp;
            }

            for (int d = 0; d < 4 && !connected; d++) {
                char *dir = directions[dirOrder[d]];
                // Burada connectRooms doğrudan room.h üzerinden çağrılır, yeniden tanımlanmaz.
                if (connectRooms(base, newRoom, dir)) {
                    connected = 1;
                }
            }
        }

        rooms[placed++] = newRoom;
    }

    free(rooms[MAX_ROOMS - 1]->description);
    rooms[MAX_ROOMS - 1]->description = strdup("The Exit Room");

    if (rooms[MAX_ROOMS - 1]->creature) {
        freeCreature(rooms[MAX_ROOMS - 1]->creature);
    }
    rooms[MAX_ROOMS - 1]->creature = createFinalBoss();
}

static void look(Room *room) {
    printf("You are in: %s\n", room->description);
    if (room->itemCount > 0) {
        printf("Items in the room:\n");
        for (int i = 0; i < room->itemCount; i++) {
            printf("- %s\n", room->items[i]);
        }
    } else {
        printf("There are no items here.\n");
    }

    if (room->creature) {
        printf("A %s is here! Health: %d\n", room->creature->name, room->creature->health);
    }
}

static int movePlayer(Player *player, const char *direction) {
    Room *nextRoom = NULL;
    if (strcmp(direction, "up") == 0) nextRoom = player->currentRoom->up;
    else if (strcmp(direction, "down") == 0) nextRoom = player->currentRoom->down;
    else if (strcmp(direction, "left") == 0) nextRoom = player->currentRoom->left;
    else if (strcmp(direction, "right") == 0) nextRoom = player->currentRoom->right;

    if (nextRoom) {
        player->currentRoom = nextRoom;
        printf("You moved %s.\n", direction);
        if (strcmp(player->currentRoom->description, "The Exit Room") == 0) {
            return 1;
        }
    } else {
        printf("There is no room in that direction.\n");
    }
    return 0;
}

static void listSavedGames() {
    DIR *d = opendir(".");
    if (!d) {
        printf("Cannot open directory.\n");
        return;
    }
    struct dirent *dir;
    int found = 0;
    while ((dir = readdir(d)) != NULL) {
        if (strstr(dir->d_name, ".save") != NULL) {
            printf("%s\n", dir->d_name);
            found = 1;
        }
    }
    closedir(d);
    if (!found) {
        printf("No saved games found.\n");
    }
}

static int findRoomIndex(Room **rooms, int roomCount, Room *room) {
    for (int i = 0; i < roomCount; i++) {
        if (rooms[i] == room)
            return i;
    }
    return -1;
}

static Room *findRoomByIndex(Room **rooms, int roomCount, int index) {
    if (index < 0 || index >= roomCount) return NULL;
    return rooms[index];
}

static void printMap(Room **rooms, int roomCount) {
    int minx = 9999, maxx = -9999, miny = 9999, maxy = -9999;
    for (int i = 0; i < roomCount; i++) {
        if (rooms[i]->x < minx) minx = rooms[i]->x;
        if (rooms[i]->x > maxx) maxx = rooms[i]->x;
        if (rooms[i]->y < miny) miny = rooms[i]->y;
        if (rooms[i]->y > maxy) maxy = rooms[i]->y;
    }

    int width = maxx - minx + 1;
    int height = maxy - miny + 1;

    char **map = (char **)malloc(sizeof(char*) * height);
    for (int y = 0; y < height; y++) {
        map[y] = (char *)malloc(sizeof(char) * (width+1));
        for (int x = 0; x < width; x++) {
            map[y][x] = '.';
        }
        map[y][width] = '\0';
    }

    for (int i = 0; i < roomCount; i++) {
        int mx = rooms[i]->x - minx;
        int my = rooms[i]->y - miny;
        if (strcmp(rooms[i]->description, "The Exit Room") == 0) {
            map[my][mx] = 'E';
        } else {
            map[my][mx] = 'x';
        }
    }

    printf("Dungeon Map:\n");
    for (int y = 0; y < height; y++) {
        printf("%s\n", map[y]);
        free(map[y]);
    }
    free(map);
}

static void printExitRoom(Room *room, Room **rooms, int roomCount) {
    printf("You have reached the Exit Room! Congratulations, you've escaped the dungeon!\n");
    if (rooms && roomCount > 0) {
        printMap(rooms, roomCount);
    }
}

static void saveGame(Player *player, Room **rooms, int roomCount, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) {
        printf("Failed to save the game!\n");
        return;
    }

    fprintf(f, "Health:%d\n", player->health);
    fprintf(f, "Strength:%d\n", player->strength);
    fprintf(f, "Shield:%d\n", player->shield);
    fprintf(f, "InventoryCount:%d\n", player->inventoryCount);
    for (int i = 0; i < player->inventoryCount; i++) {
        fprintf(f, "Item:%s\n", player->inventory[i]);
    }
    int currentIndex = findRoomIndex(rooms, roomCount, player->currentRoom);
    fprintf(f, "CurrentRoom:%d\n", currentIndex);

    fprintf(f, "RoomCount:%d\n", roomCount);
    for (int i = 0; i < roomCount; i++) {
        fprintf(f, "Description:%s\n", rooms[i]->description);
        fprintf(f, "ItemsCount:%d\n", rooms[i]->itemCount);
        for (int k = 0; k < rooms[i]->itemCount; k++) {
            fprintf(f, "RoomItem:%s\n", rooms[i]->items[k]);
        }
        if (rooms[i]->creature) {
            fprintf(f, "Creature:%s,%d,%d\n", rooms[i]->creature->name, rooms[i]->creature->health, rooms[i]->creature->attackPower);
        } else {
            fprintf(f, "Creature:None\n");
        }

        fprintf(f, "Coords:%d,%d\n", rooms[i]->x, rooms[i]->y);
    }

    fclose(f);
    printf("Game saved to %s\n", filename);
}

static void loadGame(Player *player, Room **rooms, int *roomCount, const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("No save file found with that name.\n");
        return;
    }

    for (int i = 0; i < *roomCount; i++)
        freeRoom(rooms[i]);

    char line[MAX_DESC];
    int hp, str, shd, invC;
    int currentIndex;

    fgets(line, sizeof(line), f);
    sscanf(line, "Health:%d", &hp);
    player->health = hp;

    fgets(line, sizeof(line), f);
    sscanf(line, "Strength:%d", &str);
    player->strength = str;

    fgets(line, sizeof(line), f);
    sscanf(line, "Shield:%d", &shd);
    player->shield = shd;

    fgets(line, sizeof(line), f);
    sscanf(line, "InventoryCount:%d", &invC);

    player->inventoryCount = 0;
    for (int i = 0; i < invC; i++) {
        fgets(line, sizeof(line), f);
        char *p = strchr(line, ':');
        p++;
        while (*p == ' ') p++;
        p[strcspn(p, "\n")] = 0;
        player->inventory[player->inventoryCount++] = strdup(p);
    }

    fgets(line, sizeof(line), f);
    sscanf(line, "CurrentRoom:%d", &currentIndex);

    int rc;
    fgets(line, sizeof(line), f);
    sscanf(line, "RoomCount:%d", &rc);
    *roomCount = rc;

    for (int i = 0; i < rc; i++) {
        fgets(line, sizeof(line), f);
        char desc[MAX_DESC];
        sscanf(line, "Description:%[^\n]", desc);
        Room *r = createRoom(desc);
        for (int k = 0; k < r->itemCount; k++)
            free(r->items[k]);
        r->itemCount = 0;
        if (r->creature) {
            freeCreature(r->creature);
            r->creature = NULL;
        }

        fgets(line, sizeof(line), f);
        int ic;
        sscanf(line, "ItemsCount:%d", &ic);
        for (int k = 0; k < ic; k++) {
            fgets(line, sizeof(line), f);
            char *pp = strchr(line, ':');
            pp++;
            while (*pp == ' ') pp++;
            pp[strcspn(pp, "\n")] = 0;
            r->items[r->itemCount++] = strdup(pp);
        }

        fgets(line, sizeof(line), f);
        if (strncmp(line, "Creature:None", 13) == 0) {
            r->creature = NULL;
        } else {
            char cname[MAX_DESC];
            int chp, cap;
            char *cp = strchr(line, ':');
            cp++;
            sscanf(cp, "%[^,],%d,%d", cname, &chp, &cap);
            Creature *c = (Creature *)malloc(sizeof(Creature));
            c->name = strdup(cname);
            c->health = chp;
            c->attackPower = cap;
            r->creature = c;
        }

        int rx, ry;
        fgets(line, sizeof(line), f);
        sscanf(line, "Coords:%d,%d", &rx, &ry);
        r->x = rx;
        r->y = ry;

        rooms[i] = r;
    }

    fclose(f);

    // Rebuild connections
    for (int i = 0; i < rc; i++) {
        for (int j = 0; j < rc; j++) {
            if (j != i) {
                if (rooms[j]->x == rooms[i]->x && rooms[j]->y == rooms[i]->y - 1) {
                    rooms[i]->up = rooms[j];
                    rooms[j]->down = rooms[i];
                } else if (rooms[j]->x == rooms[i]->x && rooms[j]->y == rooms[i]->y + 1) {
                    rooms[i]->down = rooms[j];
                    rooms[j]->up = rooms[i];
                } else if (rooms[j]->x == rooms[i]->x - 1 && rooms[j]->y == rooms[i]->y) {
                    rooms[i]->left = rooms[j];
                    rooms[j]->right = rooms[i];
                } else if (rooms[j]->x == rooms[i]->x + 1 && rooms[j]->y == rooms[i]->y) {
                    rooms[i]->right = rooms[j];
                    rooms[j]->left = rooms[i];
                }
            }
        }
    }

    player->currentRoom = rooms[currentIndex];
    printf("Game loaded from %s\n", filename);
}
