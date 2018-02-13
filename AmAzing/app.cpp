#include "app.hpp"
#include <time.h>
#include <bitset>
#include <iostream>

#define width 1080
#define height 640

App::App() {
    state = State::getInstance();
}

App::~App() {
    delete state;
    SDL_Quit();
}

bool App::run(std::string filename) {
    initialize(filename);
    clock_t newTime, oldTime;
    double movingAverage = 0.015;
    oldTime = clock();
    while(!state->done){
        render3d();
        newTime = clock();
        double frameTime = ((double(newTime - oldTime)) / CLOCKS_PER_SEC);
        oldTime = newTime;
        movingAverage = (movingAverage * 19.0 / 20.0) + (frameTime / 20.0);
        displayFPS(1/movingAverage);
        SDL_RenderPresent(state->renderer);
        getEvents();
        updateData(movingAverage);
    }
    return (true);
}

void App::makeGlyphs(std::string fontname) {
    if (TTF_Init()) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize true type font system: %s", TTF_GetError());
        throw std::runtime_error("TTF_Init failed");
    }
    TTF_Font *font = TTF_OpenFont(fontname.c_str(), 24);
    if (!font)
        throw std::runtime_error("TTF_OpenFont failed");
    SDL_Color fg = {0,0,0,0xff};
    SDL_Surface *textSurf = TTF_RenderText_Blended(font, "FPS: ", fg);
    if (!textSurf)
        throw std::bad_alloc();
    SDL_Texture *fpsTex = SDL_CreateTextureFromSurface(state->renderer, textSurf);
    if (!fpsTex)
        throw std::bad_alloc();
    state->fpsTex = fpsTex;
    SDL_FreeSurface(textSurf);
    for (char c : std::string(".0123456789")) {
        SDL_Surface *glyphSurf = TTF_RenderGlyph_Blended(font, c, fg);
        if (!glyphSurf)
            throw std::bad_alloc();
        SDL_Texture *glyphTex = SDL_CreateTextureFromSurface(state->renderer, glyphSurf);
        SDL_FreeSurface(glyphSurf);
        if (!glyphTex)
            throw std::bad_alloc();
        state->fontCache[c] = glyphTex;
    }
}

void App::displayFPS(double fps) {
    int w, h;
    SDL_QueryTexture(state->fpsTex, NULL, NULL, &w, &h);
    auto fpsString = std::to_string(fps);
    SDL_Rect destR = {0, 0, w, h};
    SDL_RenderCopy(state->renderer, state->fpsTex, NULL, &destR);
    for (char c : fpsString) {
        try {
            SDL_Texture *tex = state->fontCache.at(c);
            destR.x += w;
            SDL_QueryTexture(tex, NULL, NULL, &w, &h);
            destR.w = w;
            SDL_RenderCopy(state->renderer, tex, NULL, &destR);
        } catch (std::out_of_range) {
            continue;
        }
    }
    //SDL_RenderPresent(state->renderer);
}

void App::initialize(std::string filename) {
    state->layout = new Layout(filename, std::ref(state->pos));
    state->dir << 0, 1;
    state->viewPlane<< 2.0/3, 0;
    
    if (SDL_Init(SDL_INIT_VIDEO)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        throw std::runtime_error("SDL_Init failed");
    }
    if (!(state->window = SDL_CreateWindow("AmAzing", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0))) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window: %s", SDL_GetError());
        throw std::bad_alloc();
    }
    if (!(state->renderer = SDL_CreateRenderer(state->window, -1, SDL_RENDERER_ACCELERATED))) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create renderer: %s", SDL_GetError());
        throw std::bad_alloc();
    }
    makeGlyphs("Courier New.ttf");
}

void App::drawLine(int x) {
    double t = 2.0 * double(x) / double(width) - 1.0;
    Vector2d ray = state->dir + state->viewPlane * t;
    Vector2i mapPos(state->pos(0), state->pos(1));
    Vector2d dDist = ray.cwiseAbs().cwiseInverse();
    bool hit = false;
    int side = 0;
    
    Vector2i stepDir;
    Vector2d sideDist;
    
    if (ray(0) < 0) {
        sideDist(0) = (state->pos(0) - double(mapPos(0))) * dDist(0);
        stepDir(0) = -1;
    } else {
        sideDist(0) = (double(mapPos(0)) + 1.0 - state->pos(0)) * dDist(0);
        stepDir(0) = 1;
    }
    if (ray(1) < 0) {
        sideDist(1) = (state->pos(1) - double(mapPos(1))) * dDist(1);
        stepDir(1) = -1;
    } else {
        sideDist(1) = (double(mapPos(1)) + 1.0 - state->pos(1)) * dDist(1);
        stepDir(1) = 1;
    }
    while (!hit) {
        std::ptrdiff_t i;
        sideDist.minCoeff(&i);
        side = int(i);
        sideDist(i) += dDist(i);
        mapPos(i) += stepDir(i);
        
        if (state->layout->map[mapPos(0)][mapPos(1)])
            hit = true;
    }
    double perpWallDist;
    if (side == 0)
        perpWallDist = (double(mapPos(0)) - state->pos(0) + ((1.0 - double(stepDir(0))) / 2.0)) / ray(0);
    else
        perpWallDist = (double(mapPos(1)) - state->pos(1) + ((1.0 - double(stepDir(1))) / 2.0)) / ray(1);
    
    int lineHeight = (int)(height / perpWallDist);
    
    int drawStart = (height - lineHeight) / 2;
    if (drawStart < 0) drawStart = 0;
    int drawEnd = (lineHeight + height) / 2;
    if (drawEnd >= height) drawEnd = height- 1;
    
    int color = 0x8F;
    if (side == 1)
        color = 0x4D;
    SDL_SetRenderDrawColor(state->renderer, color, color, color, 0xFF);
    SDL_RenderDrawLine(state->renderer, x, drawStart, x, drawEnd);
}

void App::render3d() {
    SDL_SetRenderDrawColor(state->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(state->renderer);
    for (int x = 0; x < width; x++) {
        drawLine(x);
    }
    //SDL_RenderPresent(state->renderer);
}

void App::getEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch(e.type) {
            case SDL_QUIT:
                state->done = true;
                break;
            case SDL_KEYDOWN: case SDL_KEYUP:
                state->keyHandler.handleKeyEvent(e.key);
                break;
            default:
                break;
        }
    }
}

Vector2d rotate2d(Vector2d vector, double rotSpeed) {
    Matrix2d rotate;
    rotate << std::cos(rotSpeed), -std::sin(rotSpeed),
    std::sin(rotSpeed), std::cos(rotSpeed);
    return (rotate * vector);
}

void App::updateData(double frameTime) {
    double moveSpeed = frameTime * 4;
    double rotSpeed = frameTime * 2;
    if (state->keyHandler.isPressed(SDLK_q) || state->keyHandler.isPressed(SDLK_ESCAPE)) {
        state->done = true;
        return;
    }
    if (state->keyHandler.isPressed(SDLK_LEFT)) {
        state->dir = rotate2d(state->dir, rotSpeed);
        state->viewPlane = rotate2d(state->viewPlane, rotSpeed);
    }
    if (state->keyHandler.isPressed(SDLK_RIGHT)) {
        state->dir = rotate2d(state->dir, -rotSpeed);
        state->viewPlane = rotate2d(state->viewPlane, -rotSpeed);
    }
    if (state->keyHandler.isPressed(SDLK_UP)) {
        double tmp = state->pos(0);
        if (!state->layout->map[int(state->pos(0) + state->dir(0) * moveSpeed)][int(state->pos(1))]) {
            state->pos(0) += state->dir(0) * moveSpeed;
        }
        if (!state->layout->map[int(tmp)][int(state->pos(1) + state->dir(1) * moveSpeed)]) {
            state->pos(1) += state->dir(1) * moveSpeed;
        }
    }
    if (state->keyHandler.isPressed(SDLK_DOWN)) {
        double tmp = state->pos(0);
        if (!state->layout->map[int(state->pos(0) - state->dir(0) * moveSpeed)][int(state->pos(1))])
            state->pos(0) -= state->dir(0) * moveSpeed;
        if (!state->layout->map[int(tmp)][int(state->pos(1) - state->dir(1) * moveSpeed)])
            state->pos(1) -= state->dir(1) * moveSpeed;
    }
    if (state->keyHandler.isPressed(SDLK_a)) {
        double tmp = state->pos(0);
        if (!state->layout->map[int(state->pos(0) - state->dir(1) * moveSpeed)][int(state->pos(1))])
            state->pos(0) -= state->dir(1) * moveSpeed;
        if (!state->layout->map[int(tmp)][int(state->pos(1) + state->dir(0) * moveSpeed)])
            state->pos(1) += state->dir(0) * moveSpeed;
    }
    if (state->keyHandler.isPressed(SDLK_d)) {
        double tmp = state->pos(0);
        if (!state->layout->map[int(state->pos(0) + state->dir(1) * moveSpeed)][int(state->pos(1))])
            state->pos(0) += state->dir(1) * moveSpeed;
        if (!state->layout->map[int(tmp)][int(state->pos(1) - state->dir(0) * moveSpeed)])
            state->pos(1) -= state->dir(0) * moveSpeed;
        
    }
}
