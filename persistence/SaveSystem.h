#ifndef SNAKEII_SAVESYSTEM_H
#define SNAKEII_SAVESYSTEM_H

#include "core/Types.h"
#include <deque>
#include <string>

struct SaveData {
    std::deque<Point> snakeBody;
    Direction direction = Direction::Right;
    Point food;
    int score = 0;
    SpeedLevel speed = SpeedLevel::Medium;
    GameState state = GameState::Running;
};

class SaveSystem {
public:
    static bool saveGame(const std::string& filename, const SaveData& data);
    static bool loadGame(const std::string& filename, int width, int height, SaveData& outData, std::string& error);
};

#endif
