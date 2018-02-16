#ifndef keyHandler_hpp
#define keyHandler_hpp

#include <unordered_map>
#include <SDL2/SDL.h>

class KeyHandler {
public:
    KeyHandler();
    std::unordered_map<SDL_Keycode, uint8_t> keyStates;
    bool isPressed(SDL_Keycode keysym);
    bool isReleased(SDL_Keycode keysym);
    void handleKeyEvent(SDL_KeyboardEvent &e);
};

#endif /* keyHandler_hpp */
