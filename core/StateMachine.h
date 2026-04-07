#ifndef SNAKEII_STATEMACHINE_H
#define SNAKEII_STATEMACHINE_H

#include "core/Types.h"

class StateMachine {
public:
    StateMachine();

    GameState current() const;
    bool transitionTo(GameState next);

private:
    GameState state;
    bool isTransitionValid(GameState from, GameState to) const;
};

#endif
