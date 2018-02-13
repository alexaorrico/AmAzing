#include "keyHandler.hpp"

KeyHandler::KeyHandler () {
    keyStates = {
        {SDLK_UP, SDL_RELEASED},
        {SDLK_DOWN, SDL_RELEASED},
        {SDLK_LEFT, SDL_RELEASED},
        {SDLK_RIGHT, SDL_RELEASED},
        {SDLK_a, SDL_RELEASED},
        {SDLK_d, SDL_RELEASED},
        {SDLK_q, SDL_RELEASED},
        {SDLK_ESCAPE, SDL_RELEASED}
    };
}

bool KeyHandler::isPressed(SDL_Keycode keysym) {
    return(keyStates[keysym] == SDL_PRESSED);
}
bool KeyHandler::isReleased(SDL_Keycode keysym) {
    return(keyStates[keysym] == SDL_RELEASED);
}
void KeyHandler::handleKeyEvent(SDL_KeyboardEvent &e) {
    if (keyStates.find(e.keysym.sym) != keyStates.end())
        keyStates[e.keysym.sym] = e.state;
}
