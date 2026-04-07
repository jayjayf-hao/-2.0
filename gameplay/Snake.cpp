#include "gameplay/Snake.h"

Snake::Snake(int width, int height) {
    reset(width, height);
}

void Snake::reset(int width, int height) {
    segments.clear();
    segments.push_back(Point(width / 2, height / 2));
    segments.push_back(Point(width / 2 - 1, height / 2));
    segments.push_back(Point(width / 2 - 2, height / 2));
    currentDirection = Direction::Right;
    pendingGrowth = 0;
}

void Snake::setDirection(Direction dir) {
    if (dir == Direction::Stop) {
        return;
    }
    if (isOpposite(dir, currentDirection)) {
        return;
    }
    currentDirection = dir;
}

Direction Snake::direction() const {
    return currentDirection;
}

void Snake::move() {
    Point next = head();
    switch (currentDirection) {
    case Direction::Left:
        next.x--;
        break;
    case Direction::Right:
        next.x++;
        break;
    case Direction::Up:
        next.y--;
        break;
    case Direction::Down:
        next.y++;
        break;
    case Direction::Stop:
    default:
        return;
    }

    segments.push_front(next);
    if (pendingGrowth > 0) {
        pendingGrowth--;
    } else {
        segments.pop_back();
    }
}

void Snake::wrapHead(int width, int height) {
    if (segments.empty()) {
        return;
    }

    Point& h = segments.front();
    if (h.x < 0) {
        h.x = width - 1;
    } else if (h.x >= width) {
        h.x = 0;
    }

    if (h.y < 0) {
        h.y = height - 1;
    } else if (h.y >= height) {
        h.y = 0;
    }
}

void Snake::grow() {
    pendingGrowth++;
}

bool Snake::hitWall(int width, int height) const {
    Point h = head();
    return h.x < 0 || h.x >= width || h.y < 0 || h.y >= height;
}

bool Snake::hitSelf() const {
    Point h = head();
    for (size_t i = 1; i < segments.size(); ++i) {
        if (segments[i] == h) {
            return true;
        }
    }
    return false;
}

Point Snake::head() const {
    return segments.front();
}

const std::deque<Point>& Snake::body() const {
    return segments;
}

void Snake::setBody(const std::deque<Point>& bodyData) {
    if (!bodyData.empty()) {
        segments = bodyData;
    }
}
