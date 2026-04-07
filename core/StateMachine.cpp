#include "core/StateMachine.h"

StateMachine::StateMachine() : state(GameState::Menu) {}

GameState StateMachine::current() const {
    return state;
}

bool StateMachine::transitionTo(GameState next) {
    if (state == next) {
        return true;
    }
    if (!isTransitionValid(state, next)) {
        return false;
    }
    state = next;
    return true;
}

bool StateMachine::isTransitionValid(GameState from, GameState to) const {
    switch (from) {
    case GameState::Menu:
        return to == GameState::Running || to == GameState::Exit;
    case GameState::Running:
        return to == GameState::Paused || to == GameState::Menu || to == GameState::GameOver || to == GameState::Exit;
    case GameState::Paused:
        return to == GameState::Running || to == GameState::Menu || to == GameState::Exit;
    case GameState::GameOver:
        return to == GameState::Menu || to == GameState::Exit;
    case GameState::Exit:
        return false;
    default:
        return false;
    }
}
