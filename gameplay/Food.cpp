#include "gameplay/Food.h"

Food::Food(int width, int height)
    : boardWidth(width), boardHeight(height), current(width / 3, height / 3), rng(std::random_device{}()) {}

void Food::generate(const std::deque<Point>& snakeBody) {
    std::uniform_int_distribution<int> xDist(0, boardWidth - 1);
    std::uniform_int_distribution<int> yDist(0, boardHeight - 1);

    const int maxAttempts = 300;
    for (int i = 0; i < maxAttempts; ++i) {
        Point candidate(xDist(rng), yDist(rng));
        bool occupied = false;
        for (const Point& p : snakeBody) {
            if (p == candidate) {
                occupied = true;
                break;
            }
        }
        if (!occupied) {
            current = candidate;
            return;
        }
    }

    for (int y = 0; y < boardHeight; ++y) {
        for (int x = 0; x < boardWidth; ++x) {
            Point candidate(x, y);
            bool occupied = false;
            for (const Point& p : snakeBody) {
                if (p == candidate) {
                    occupied = true;
                    break;
                }
            }
            if (!occupied) {
                current = candidate;
                return;
            }
        }
    }
}

Point Food::position() const {
    return current;
}

void Food::setPosition(const Point& point) {
    current = point;
}
