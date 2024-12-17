# Text-Based Dungeon Game

Welcome to the **Text-Based Dungeon Game**, a dungeon exploration adventure written in C. Explore procedurally generated rooms, battle fearsome creatures, collect useful items, and strive to defeat the **Final Boss** to escape the dungeon!

---

## Features
- **Procedural Room Generation**: Rooms are randomly connected, creating a unique dungeon layout every game.
- **Dynamic Events**: Each room triggers random events (5 positive, 5 negative) that affect your stats (health, strength, shield).
- **Challenging Creatures**: Face creatures like *Spider*, *Orc*, *Werewolf*, *Zombie*, and *Vampire*, each with fixed health and attack power.
- **Interactive Items**:
  - **Sword**: Increases player strength.
  - **Shield**: Provides defense against creature attacks.
  - **Potion**: Offers randomized positive or negative effects.
  - **Miracle Fruit**: A high-risk item with a 50% chance to grant +50 strength or instantly end the game (Game Over).
- **Combat System**: Attack creatures while managing your health, shield, and strength.
- **Save and Load Progress**: Save your game at any time and reload later to continue your journey.
- **Makefile Support**: Compile and manage the game efficiently using the included Makefile.

---

## How to Compile and Run
This project includes a Makefile for easy compilation.

1. **Compile the game**:
   ```bash
   make
   ```

2. **Run the game**:
   ```bash
   ./dungeon_game
   ```

3. **Clean build files**:
   ```bash
   make clean
   ```

---

## Game Commands
Use the following commands to play the game:

- `move <direction>`: Move between rooms (directions: up, down, left, right).
- `look`: Inspect the current room and trigger its event.
- `inventory`: Display your inventory.
- `pickup <item>`: Pick up an item in the room.
- `drop <item>`: Drop an item from your inventory.
- `attack`: Attack the creature in the current room.
- `use potion`: Drink a potion for a random effect.
- `use miraclefruit`: Use the Miracle Fruit (confirmation required).
- `save <filename>`: Save your game progress.
- `load <filename>`: Load a previously saved game.
- `list`: List all saved games.
- `health`: Display your current health, shield, and strength stats.
- `exit`: Quit the game.

---

## Gameplay Overview
1. Start the game in a randomly generated room.
2. Navigate through connected rooms using movement commands.
3. Collect items, use potions, and battle creatures.
4. Trigger room events by inspecting rooms with the `look` command.
5. Reach the **Exit Room** and defeat the **Final Boss** to win the game.
6. Save your progress at any time using the `save` command and reload it with `load`.

---

## Code Structure
The project follows a modular structure with the following files:

- **creature.c / creature.h**: Manages creatures and Final Boss creation.
- **room.c / room.h**: Handles room generation, connections, events, and contents.
- **player.c / player.h**: Manages player stats, inventory, and item interactions.
- **game.c / game.h**: Controls the game loop, input commands, and save/load mechanics.
- **main.c**: Initializes the game and starts the main loop.
- **Makefile**: Streamlines compilation and cleanup processes.

---

## Save and Load System
The save/load system allows players to store and restore their progress. It saves the following:
- Player stats (health, strength, shield).
- Inventory items.
- Room states (events, items, creatures).
- Player's current location.

To save:
```bash
save <filename>
```
To load:
```bash
load <filename>
```
To list saved games:
```bash
list
```

---

## Example Gameplay
```
> look
You are in: Room 1
Items in the room:
- Sword
A Spider is here! Health: 25

> attack
You attacked with your sword to destroy the monster!
The spider bit you!
Your shield absorbed the damage! Remaining shield: 5

> pickup Sword
You picked up a Sword! Strength: 25

> move right
You moved right.
```
