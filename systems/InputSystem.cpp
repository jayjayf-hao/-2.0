#include "systems/InputSystem.h"
#include <conio.h>

InputSnapshot InputSystem::poll(Direction currentDirection) const {
    InputSnapshot snapshot;
    Direction nextDirection = currentDirection;

    while (_kbhit()) {
        int key = _getch();
        snapshot.rawEvents++;

        if (key == 0 || key == 224) {
            key = _getch();
            snapshot.rawEvents++;
        }

        Direction mapped = Direction::Stop;
        if (mapToDirection(key, mapped)) {
            if (!isOpposite(mapped, currentDirection)) {
                nextDirection = mapped;
                snapshot.hasDirection = true;
            } else {
                snapshot.filteredIllegalTurns++;
            }
            continue;
        }

        switch (key) {
        case ' ': case 'e': case 'E':
            snapshot.startOrConfirm = true;
            break;
        case 'p': case 'P':
            snapshot.pauseToggle = true;
            break;
        case 'k': case 'K':
            snapshot.save = true;
            break;
        case 'l': case 'L':
            snapshot.load = true;
            break;
        case 'q': case 'Q':
            snapshot.exit = true;
            break;
        case 'm': case 'M':
            snapshot.backToMenu = true;
            break;
        case '+': case '=':
            snapshot.speedUp = true;
            break;
        case '-': case '_':
            snapshot.speedDown = true;
            break;
        default:
            snapshot.ignoredInvalidKeys++;
            break;
        }
    }

    if (snapshot.hasDirection) {
        snapshot.latestDirection = nextDirection;
    }
    return snapshot;
}

bool InputSystem::mapToDirection(int key, Direction& outDirection) const {
    switch (key) {
    case 'w': case 'W':
        outDirection = Direction::Up;
        return true;
    case 'a': case 'A':
        outDirection = Direction::Left;
        return true;
    case 's': case 'S':
        outDirection = Direction::Down;
        return true;
    case 'd': case 'D':
        outDirection = Direction::Right;
        return true;
    case 72:
        outDirection = Direction::Up;
        return true;
    case 75:
        outDirection = Direction::Left;
        return true;
    case 80:
        outDirection = Direction::Down;
        return true;
    case 77:
        outDirection = Direction::Right;
        return true;
    default:
        return false;
    }
}
