#pragma once
#include <SDL2/SDL.h>

struct FloatPoint{
    float x;
    float y;
    void normalize();
    float getSize();
};

struct PlayerControls {
    SDL_Scancode accelerate;
    SDL_Scancode reverse;
    SDL_Scancode turnLeft;
    SDL_Scancode turnRight;
    SDL_Scancode drift;
};