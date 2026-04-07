#include "core/GameLoop.h"
#include <chrono>
#include <thread>

GameLoop::GameLoop(int width, int height)
    : boardWidth(width),
      boardHeight(height),
      canvasWidth(width * 2 + 4 + 40),
      canvasHeight(height + 3),
      snake(width, height),
      food(width, height),
      renderer(canvasWidth, canvasHeight),
      score(0),
      statusMessage("Press direction key or SPACE to start"),
      lastPauseToggleTime(std::chrono::steady_clock::now() - std::chrono::milliseconds(500)) {
    food.generate(snake.body());
    timer.setSpeed(SpeedLevel::Low);
    timer.resync();
}

void GameLoop::run() {
    while (fsm.current() != GameState::Exit) {
        timer.beginFrame();

        const InputSnapshot snapshot = input.poll(snake.direction());
        processInput(snapshot);

        int safetyUpdates = 0;
        while (fsm.current() == GameState::Running && timer.consumeUpdateTick() && safetyUpdates < 5) {
            updateOneTick();
            safetyUpdates++;
        }

        render(snapshot);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void GameLoop::resetRound() {
    snake.reset(boardWidth, boardHeight);
    food.generate(snake.body());
    score = 0;
    timer.setSpeed(SpeedLevel::Low);
    timer.resync();
    statusMessage = "New round started";
}

void GameLoop::processInput(const InputSnapshot& snapshot) {
    if (snapshot.exit) {
        fsm.transitionTo(GameState::Exit);
        return;
    }

    if (snapshot.save && (fsm.current() == GameState::Running || fsm.current() == GameState::Paused)) {
        saveGame();
    }

    if (snapshot.load && (fsm.current() == GameState::Menu || fsm.current() == GameState::Paused || fsm.current() == GameState::Running)) {
        loadGame();
    }

    if (snapshot.speedUp) {
        timer.increaseSpeed();
    }
    if (snapshot.speedDown) {
        timer.decreaseSpeed();
    }

    if (snapshot.backToMenu && fsm.current() != GameState::Menu) {
        fsm.transitionTo(GameState::Menu);
        timer.resync();
        statusMessage = "Back to menu";
        return;
    }

    switch (fsm.current()) {
    case GameState::Menu:
        if (snapshot.startOrConfirm || snapshot.hasDirection) {
            resetRound();
            if (snapshot.hasDirection) {
                snake.setDirection(snapshot.latestDirection);
            }
            fsm.transitionTo(GameState::Running);
            timer.resync();
            statusMessage = "Running";
        }
        break;
    case GameState::Running:
        if (shouldAcceptPauseToggle(snapshot)) {
            fsm.transitionTo(GameState::Paused);
            timer.resync();
            statusMessage = "Paused";
            return;
        }
        if (snapshot.hasDirection) {
            snake.setDirection(snapshot.latestDirection);
        }
        break;
    case GameState::Paused:
        if (shouldAcceptPauseToggle(snapshot)) {
            fsm.transitionTo(GameState::Running);
            timer.resync();
            statusMessage = "Resumed";
        } else if (snapshot.backToMenu) {
            fsm.transitionTo(GameState::Menu);
            timer.resync();
            statusMessage = "Back to menu";
        }
        break;
    case GameState::GameOver:
        if (snapshot.startOrConfirm) {
            fsm.transitionTo(GameState::Menu);
            statusMessage = "Press direction or SPACE to start again";
        } else if (snapshot.backToMenu) {
            fsm.transitionTo(GameState::Menu);
            statusMessage = "Back to menu";
        }
        break;
    case GameState::Exit:
    default:
        break;
    }
}

void GameLoop::updateOneTick() {
    snake.move();
    snake.wrapHead(boardWidth, boardHeight);

    if (snake.hitSelf()) {
        fsm.transitionTo(GameState::GameOver);
        timer.resync();
        statusMessage = "Game over";
        return;
    }

    if (snake.head() == food.position()) {
        snake.grow();
        score += 10;
        food.generate(snake.body());
        timer.autoAdjustByScore(score);
    }
}

void GameLoop::render(const InputSnapshot& snapshot) {
    renderer.clear();

    const WORD wallColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
    const WORD snakeBodyColor = FOREGROUND_GREEN;
    const WORD snakeHeadColor = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
    const WORD foodColor = FOREGROUND_RED | FOREGROUND_INTENSITY;

    for (int x = 0; x < boardWidth + 2; ++x) {
        renderer.drawChar(x * 2, 0, '#', wallColor);
        renderer.drawChar(x * 2 + 1, 0, '#', wallColor);
        renderer.drawChar(x * 2, boardHeight + 1, '#', wallColor);
        renderer.drawChar(x * 2 + 1, boardHeight + 1, '#', wallColor);
    }
    for (int y = 0; y < boardHeight + 2; ++y) {
        renderer.drawChar(0, y, '#', wallColor);
        renderer.drawChar((boardWidth + 1) * 2, y, '#', wallColor);
        renderer.drawChar((boardWidth + 1) * 2 + 1, y, '#', wallColor);
    }

    if (fsm.current() != GameState::Menu) {
        const std::deque<Point>& body = snake.body();
        for (size_t i = 1; i < body.size(); ++i) {
            renderer.drawChar(body[i].x * 2 + 2, body[i].y + 1, 'o', snakeBodyColor);
        }
        renderer.drawChar(snake.head().x * 2 + 2, snake.head().y + 1, '@', snakeHeadColor);

        const Point foodPos = food.position();
        renderer.drawChar(foodPos.x * 2 + 2, foodPos.y + 1, '*', foodColor);
    }

    const int panelX = (boardWidth + 2) * 2 + 2;
    renderer.drawText(panelX, 1, "snakeII");
    renderer.drawText(panelX, 3, "State : " + stateText(fsm.current()));
    renderer.drawText(panelX, 4, "Score : " + std::to_string(score));
    renderer.drawText(panelX, 5, "Speed : " + speedText(timer.speed()) + " (" + std::to_string(timer.tickMs()) + "ms)");

    renderer.drawText(panelX, 7, "Input stats(last frame):");
    renderer.drawText(panelX, 8, "raw=" + std::to_string(snapshot.rawEvents));
    renderer.drawText(panelX, 9, "illegal turn filtered=" + std::to_string(snapshot.filteredIllegalTurns));
    renderer.drawText(panelX, 10, "invalid key ignored=" + std::to_string(snapshot.ignoredInvalidKeys));

    renderer.drawText(panelX, 12, "Controls:");
    renderer.drawText(panelX, 13, "WASD / Arrow : Move");
    renderer.drawText(panelX, 14, "P : Pause/Resume");
    renderer.drawText(panelX, 15, "K : Save, L : Load");
    renderer.drawText(panelX, 16, "+/- : Speed up/down");
    renderer.drawText(panelX, 17, "M : Back to menu(anytime)");
    renderer.drawText(panelX, 18, "Q : Exit");

    renderer.drawText(panelX, 20, "Message: " + statusMessage);

    if (fsm.current() == GameState::Menu) {
        renderer.drawText(panelX, 21, "Hint: SPACE or direction");
    } else if (fsm.current() == GameState::Paused) {
        renderer.drawText(panelX, 21, "Hint: P to resume");
    } else if (fsm.current() == GameState::GameOver) {
        renderer.drawText(panelX, 21, "Hint: SPACE to menu");
    } else {
        renderer.drawText(panelX, 21, "Hint: keep moving");
    }

    renderer.present();
}

void GameLoop::saveGame() {
    SaveData data;
    data.snakeBody = snake.body();
    data.direction = snake.direction();
    data.food = food.position();
    data.score = score;
    data.speed = timer.speed();
    data.state = fsm.current();

    if (SaveSystem::saveGame("save_snakeII.txt", data)) {
        statusMessage = "Saved to save_snakeII.txt";
    } else {
        statusMessage = "Save failed";
    }
}

void GameLoop::loadGame() {
    SaveData data;
    std::string error;

    if (!SaveSystem::loadGame("save_snakeII.txt", boardWidth, boardHeight, data, error)) {
        statusMessage = "Load failed: " + error;
        return;
    }

    snake.setBody(data.snakeBody);
    snake.setDirection(data.direction);
    food.setPosition(data.food);
    score = data.score;
    timer.setSpeed(data.speed);

    if (data.state == GameState::Running || data.state == GameState::Paused) {
        fsm.transitionTo(GameState::Running);
    } else {
        fsm.transitionTo(GameState::Menu);
    }
    timer.resync();

    statusMessage = "Load success";
}

bool GameLoop::shouldAcceptPauseToggle(const InputSnapshot& snapshot) {
    if (!snapshot.pauseToggle) {
        return false;
    }

    const auto now = std::chrono::steady_clock::now();
    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastPauseToggleTime);
    if (elapsed.count() < 180) {
        return false;
    }

    lastPauseToggleTime = now;
    return true;
}

std::string GameLoop::stateText(GameState state) const {
    switch (state) {
    case GameState::Menu:
        return "MENU";
    case GameState::Running:
        return "RUNNING";
    case GameState::Paused:
        return "PAUSED";
    case GameState::GameOver:
        return "GAME_OVER";
    case GameState::Exit:
        return "EXIT";
    default:
        return "UNKNOWN";
    }
}

std::string GameLoop::speedText(SpeedLevel level) const {
    switch (level) {
    case SpeedLevel::Low:
        return "LOW";
    case SpeedLevel::Medium:
        return "MEDIUM";
    case SpeedLevel::High:
        return "HIGH";
    default:
        return "UNKNOWN";
    }
}
