#ifndef CAR_HPP
#define CAR_HPP

#pragma once

#include <SDL.h>
#include "SDL_image.h"
#include "Utilities.h"
#include <string>

class Car {
public:
    Car(SDL_Renderer* renderer, float x, float y, float speed, const std::string& textureFile, float scaleFactor);
    ~Car();

    void HandleInput(const Uint8* state, const PlayerControls& controls);
    void Update(float deltaTime);
    void Render(SDL_Renderer* renderer);

    void SetPosition(float x, float y);
    void SetSpeed(float speed);

    // Boost functionality
    void StartBoost(float multiplier, float duration);

    void ScaleEverything(float scaleFactor);

private:
    void Accelerate();
    void Reverse();
    void RotateLeft();
    void RotateRight();

    // Helper functions
    void AdjustVelocityTowardsAngle();
    void UpdatePosition();
    void ApplyFriction();
    void RestrictSpeedToTopSpeed();
    float GetTopSpeedPercentage();
    float GetAngleSpeed();
    void UpdateTractionPercentage();

    // Physical properties adjustable by user
    float acceleration = 0.2f;
    float topSpeed;
    float rotationSpeed = 0.2f;  // Fart bilen roterer. Eks 0 hvis står stille
    float friction = 0.01f;      // Hvor mye bilen automatisk bremser når man ikke akselererer

    // Physical properties not adjustable by user
    FloatPoint velocity = {0.0f, 0.0f};
    float angle = 0.0f;
    bool isAccelerating = false;
    bool isDrifting = false;
    float tractionPercentage = 1.0f;  // between 0 and 1. 1 is perfect traction
    float posX, posY;  // Posisjon

    // SDL properties
    int spriteWidth = 40; 
    int spriteHeight = 90;   // Width and height of sprite
    SDL_Rect spriteRect;             // SDL_Rect for rendering
    SDL_Texture* texture;

    // Boost functionality
    bool isBoosting = false;  // Indikerer om bilen er i en boost-fase
    float boostMultiplier;    // Hvor mye hastigheten øker under boost
    float boostDuration;      // Hvor lenge boosten varer
    float boostElapsedTime;   // Tid som har gått siden boosten startet
    float normalTopSpeed;     // Lagre den normale toppfarten
    void ApplyBoost(float deltaTime);
};

#endif // CAR_HPP
