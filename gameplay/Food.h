#ifndef SNAKEII_FOOD_H
#define SNAKEII_FOOD_H

#include "core/Types.h"
#include <deque>
#include <random>

class Food {
public:
    Food(int width, int height);

    void generate(const std::deque<Point>& snakeBody);
    Point position() const;
    void setPosition(const Point& point);

private:
    int boardWidth;
    int boardHeight;
    Point current;
    std::mt19937 rng;
};

#endif
