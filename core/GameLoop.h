#ifndef SNAKEII_GAMELOOP_H
#define SNAKEII_GAMELOOP_H

#include "core/StateMachine.h"
#include "gameplay/Food.h"
#include "gameplay/Snake.h"
#include "persistence/SaveSystem.h"
#include "systems/InputSystem.h"
#include "systems/RenderSystem.h"
#include "systems/TimeSystem.h"
#include <chrono>
#include <string>

class GameLoop {
public:
    GameLoop(int width, int height);
    void run();

private:
    int boardWidth;
    int boardHeight;
    int canvasWidth;
    int canvasHeight;

    Snake snake;
    Food food;
    RenderSystem renderer;
    InputSystem input;
    TimeSystem timer;
    StateMachine fsm;

    int score;
    std::string statusMessage;
    std::chrono::steady_clock::time_point lastPauseToggleTime;

    void resetRound();
    void processInput(const InputSnapshot& snapshot);
    void updateOneTick();
    void render(const InputSnapshot& snapshot);
    bool shouldAcceptPauseToggle(const InputSnapshot& snapshot);

    void saveGame();
    void loadGame();

    std::string stateText(GameState state) const;
    std::string speedText(SpeedLevel level) const;
};

#endif
