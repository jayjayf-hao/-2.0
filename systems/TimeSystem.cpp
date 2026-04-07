#include "systems/TimeSystem.h"

TimeSystem::TimeSystem()
    : lastFrameTime(Clock::now()),
      accumulator(0),
      currentSpeed(SpeedLevel::Medium),
      tickInterval(120) {}

void TimeSystem::beginFrame() {
    const Clock::time_point now = Clock::now();
    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastFrameTime);
    lastFrameTime = now;

    accumulator += elapsed;

    const std::chrono::milliseconds maxAccumulation(5 * tickInterval.count());
    if (accumulator > maxAccumulation) {
        accumulator = maxAccumulation;
    }
}

bool TimeSystem::consumeUpdateTick() {
    if (accumulator >= tickInterval) {
        accumulator -= tickInterval;
        return true;
    }
    return false;
}

void TimeSystem::setSpeed(SpeedLevel level) {
    applySpeed(level);
}

void TimeSystem::increaseSpeed() {
    if (currentSpeed == SpeedLevel::Low) {
        applySpeed(SpeedLevel::Medium);
    } else if (currentSpeed == SpeedLevel::Medium) {
        applySpeed(SpeedLevel::High);
    }
}

void TimeSystem::decreaseSpeed() {
    if (currentSpeed == SpeedLevel::High) {
        applySpeed(SpeedLevel::Medium);
    } else if (currentSpeed == SpeedLevel::Medium) {
        applySpeed(SpeedLevel::Low);
    }
}

void TimeSystem::autoAdjustByScore(int score) {
    if (score >= 180) {
        applySpeed(SpeedLevel::High);
    } else if (score >= 80) {
        applySpeed(SpeedLevel::Medium);
    } else {
        applySpeed(SpeedLevel::Low);
    }
}

void TimeSystem::resync() {
    lastFrameTime = Clock::now();
    accumulator = std::chrono::milliseconds(0);
}

SpeedLevel TimeSystem::speed() const {
    return currentSpeed;
}

int TimeSystem::tickMs() const {
    return static_cast<int>(tickInterval.count());
}

void TimeSystem::applySpeed(SpeedLevel level) {
    currentSpeed = level;
    switch (currentSpeed) {
    case SpeedLevel::Low:
        tickInterval = std::chrono::milliseconds(170);
        break;
    case SpeedLevel::Medium:
        tickInterval = std::chrono::milliseconds(120);
        break;
    case SpeedLevel::High:
        tickInterval = std::chrono::milliseconds(80);
        break;
    }
}
