#pragma once
#include <SDL2/SDL.h>

struct FloatPoint{
    float x;
    float y;
    void normalize();
    float getSize();
};

float getDistanceBetween(FloatPoint a, FloatPoint b);

FloatPoint getMidPointBetween(FloatPoint a, FloatPoint b);

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