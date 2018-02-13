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
    void makeGlyphs(std::string fontname);
    void initialize(std::string filename);
    void getEvents();
    void updateData(double frameTime);
    void drawLine(int x);
    void render3d();
    void displayFPS(double fps);
};

#endif /* app_hpp */
