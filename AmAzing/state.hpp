#ifndef state_hpp
#define state_hpp

#include <stdio.h>
#include <Dense>
#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_image/SDL_image.h>
#include <vector>
#include <unordered_map>
#include "keyHandler.hpp"
#include "Layout.hpp"

using std::vector;
using namespace Eigen;

class State {
private:
    static State *instance;
public:
    ~State();
    bool done = false;
    Vector2d pos;
    Vector2d dir;
    Vector2d viewPlane;
    SDL_Renderer *renderer = nullptr;
    SDL_Window *window = nullptr;
    TTF_Font *font = nullptr;
    SDL_Texture *fpsTex = nullptr;
    std::unordered_map<u_char, SDL_Texture *> fontCache;
    KeyHandler keyHandler;
    Layout *layout = nullptr;
    static State *getInstance();
};

#endif /* state_hpp */
