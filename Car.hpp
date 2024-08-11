#ifndef CAR_HPP
#define CAR_HPP

#pragma once
#include <SDL.h>
#include "SDL_image.h"
#include "Utilities.h"
#include <string>

class Car {
public:
    Car(SDL_Renderer* renderer, float x, float y, float speed, const std::string& textureFile);
    ~Car();

    void HandleInput(const Uint8* state, const PlayerControls& controls);
    void Update(float deltaTime);
    void Render(SDL_Renderer* renderer);

    void SetPosition(float x, float y);
    void SetSpeed(float speed);

private:

    void Accelerate();
    void Reverse();
    void AdjustVelocityTowardsAngle();
    void UpdatePosition();
    void ApplyFriction();
    void RestrictSpeedToTopSpeed();
    void RotateLeft();
    void RotateRight();

    float GetTopSpeedPercentage();
    float GetAngleSpeed();
    void UpdateTractionPercentage();

    // Physical properties adjustable by user
    float acceleration = 0.2f;
    float topSpeed;
    float rotationSpeed = 0.2f; // Fart bilen roterer. Eks 0 hvis står stille
    float friction = 0.01f; // Hvor mye bilen automatisk bremser når man ikke akselererer
    
    // Physical properties not adjustable by user
    FloatPoint velocity = {0.0f, 0.0f};
    float angle = 0.0f;
    bool isAccelerating = false;
    bool isDrifting = false;
    float tractionPercentage = 1.0f; // between 0 and 1. 1 is perfect traction
    float posX, posY;     // Posisjon


    // SDL properties
    int spriteWidth, spriteHeight; // Width and height of sprite
    SDL_Rect spriteRect;  // SDL_Rect for rendering
    SDL_Texture* texture;

};

#endif // CAR_HPP