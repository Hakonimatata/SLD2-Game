#ifndef CAR_HPP
#define CAR_HPP

#pragma once
#include <SDL.h>
#include "SDL_image.h"
#include "Utilities.h"

class Car {
public:
    Car(SDL_Renderer* renderer, float x, float y, float speed);
    ~Car();

    void HandleInput(const Uint8* state);
    void Update(float deltaTime);
    void Render(SDL_Renderer* renderer);

    void SetPosition(float x, float y);
    void SetSpeed(float speed);

    void Accelerate();
    void Reverse();
    void Slide();
    void RotateLeft();
    void RotateRight();

    float GetTopSpeedPercentage();
    float GetAngleSpeed();

private:
    // Physical properties
    int size = 50;
    float acceleration = 0.4f;
    const float topSpeed = 10.0f;
    FloatPoint velocity = {0.0f, 0.0f};
    float rotationSpeed = 0.2f;
    float friction = 0.05f;
    float angle = 0.0f;

    bool isAccelerating = false;
    bool isDrifting = false;
    float tractionPercentage = 1.0f; // between 0 and 1. 1 is good traction

    float posX, posY;     // Posisjon
    float speed;          // Hastighet i piksler per sekund


    int spriteWidth, spriteHeight; // Width and height of sprite
    SDL_Rect spriteRect;  // SDL_Rect for rendering
    SDL_Texture* texture;

};

#endif // CAR_HPP