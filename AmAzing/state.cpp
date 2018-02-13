#include "state.hpp"

State *State::instance = new State;

State *State::getInstance() {
    return instance;
}

State::~State() {
    if (window)
        SDL_DestroyWindow(window);
    if (renderer)
        SDL_DestroyRenderer(renderer);
    if (layout)
        delete layout;
}
