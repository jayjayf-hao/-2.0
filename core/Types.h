#ifndef SNAKEII_TYPES_H
#define SNAKEII_TYPES_H

enum class Direction {
    Stop = 0,
    Left,
    Right,
    Up,
    Down
};

inline bool isOpposite(Direction a, Direction b) {
    return (a == Direction::Left && b == Direction::Right) ||
           (a == Direction::Right && b == Direction::Left) ||
           (a == Direction::Up && b == Direction::Down) ||
           (a == Direction::Down && b == Direction::Up);
}

struct Point {
    int x;
    int y;

    Point(int px = 0, int py = 0) : x(px), y(py) {}

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

enum class GameState {
    Menu = 0,
    Running,
    Paused,
    GameOver,
    Exit
};

enum class SpeedLevel {
    Low = 0,
    Medium,
    High
};

#endif
