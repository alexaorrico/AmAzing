#ifndef app_hpp
#define app_hpp

#include <stdio.h>
#include "state.hpp"

class App {
public:
    App();
    ~App();
    bool run(std::string filename);
private:
    State *state;
    SDL_Surface *buffer = nullptr;
    SDL_Texture *buffTex = nullptr;
    SDL_Surface *image;
    uint32_t theTexture[64][64];
    void makeGlyphs(std::string fontname);
    void initialize(std::string filename);
    void getEvents();
    void updateData(double frameTime);
    void drawLine(int x);
    void render2d();
    void render3d();
    void drawTexture(int x, int side, int lineheight, double perpWallDist, int drawstart, int drawend, Vector2d& ray);
    void displayFPS(double fps);
};

#endif /* app_hpp */
