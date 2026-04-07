#ifndef SNAKEII_TIMESYSTEM_H
#define SNAKEII_TIMESYSTEM_H

#include "core/Types.h"
#include <chrono>

class TimeSystem {
public:
    TimeSystem();

    void beginFrame();
    bool consumeUpdateTick();

    void setSpeed(SpeedLevel level);
    void increaseSpeed();
    void decreaseSpeed();
    void autoAdjustByScore(int score);
    void resync();

    SpeedLevel speed() const;
    int tickMs() const;

private:
    using Clock = std::chrono::steady_clock;

    Clock::time_point lastFrameTime;
    std::chrono::milliseconds accumulator;

    SpeedLevel currentSpeed;
    std::chrono::milliseconds tickInterval;

    void applySpeed(SpeedLevel level);
};

#endif
