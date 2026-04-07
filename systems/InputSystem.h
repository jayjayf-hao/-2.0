#ifndef SNAKEII_INPUTSYSTEM_H
#define SNAKEII_INPUTSYSTEM_H

#include "core/Types.h"

struct InputSnapshot {
    bool hasDirection = false;
    Direction latestDirection = Direction::Stop;

    bool startOrConfirm = false;
    bool pauseToggle = false;
    bool save = false;
    bool load = false;
    bool exit = false;
    bool backToMenu = false;
    bool speedUp = false;
    bool speedDown = false;

    int rawEvents = 0;
    int filteredIllegalTurns = 0;
    int ignoredInvalidKeys = 0;
};

class InputSystem {
public:
    InputSnapshot poll(Direction currentDirection) const;

private:
    bool mapToDirection(int key, Direction& outDirection) const;
};

#endif
