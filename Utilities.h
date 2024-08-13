#pragma once
#include <SDL2/SDL.h>

struct FloatPoint{
    float x;
    float y;
    void normalize();
    float getSize();
};

struct Point{
    int x;
    int y;
};

struct PlayerControls {
    SDL_Scancode accelerate;
    SDL_Scancode reverse;
    SDL_Scancode turnLeft;
    SDL_Scancode turnRight;
    SDL_Scancode drift;
    SDL_Scancode boost;
};