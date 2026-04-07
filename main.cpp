#include "core/GameLoop.h"

int main() {
    const int width = 20;
    const int height = 20;

    GameLoop game(width, height);
    game.run();
    return 0;
}
