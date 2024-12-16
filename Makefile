CC = gcc
CFLAGS = -Wall -Wextra -std=c99
DEPS = game.h player.h room.h creature.h
OBJ = main.o game.o player.o room.o creature.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

dungeon_game: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o dungeon_game