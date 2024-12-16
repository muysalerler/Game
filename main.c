#include "game.h"
#include <time.h>
#include <stdlib.h>

int main() {
    srand(time(NULL));
    Game g;
    initGame(&g);
    gameRun(&g);
    freeGame(&g);
    return 0;
}
