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
    void ApplyBoost();

    void ScaleEverything(float scaleFactor); // Scales sprites, top speed, acceleration, friction, etc. Use when resizing the window or zooming in/out

    FloatPoint GetPos() const { return {posX, posY}; }

private:
    void Accelerate();
    void Reverse();
    void RotateLeft();
    void RotateRight();

    // Helper functions
    void UpdateTractionPercentage();
    void AdjustVelocityTowardsAngle();
    void UpdatePosition();
    void ApplyFriction();
    void RestrictSpeedToTopSpeed();
    float GetTopSpeedPercentage();
    float GetAngleSpeed();

    // Physical parameters adjustable by user
    float physicsSpeedFactor = 0.1f;          // Multiplier for delta time => speed of all physics (set in constructor)
    float acceleration = 0.2f;                  // Car gas acceleration
    float topSpeed;                             // Top speed of the car (set in constructor)
    float rotationSpeed = 0.15f;                // Rotation speed of the car 
    float rotationMultiplierWhenDrift = 2.0f;   // Multiplier for rotation speed when drifting
    float friction = 0.01f;                     // Speed reduction when not accelerating

    // Physical properties not adjustable by user
    FloatPoint velocity = {0.0f, 0.0f};
    float angle = 0.0f;                      // Current nose angle of car [radians]
    bool isAccelerating = false;             // Used to apply friction only when not accelerating
    bool isDrifting = false;                 // Used to vary traction percentage when drift key is pressed
    float tractionPercentage = 1.0f;         // between 0 and 1. 1 is perfect traction
    float posX, posY;                        // Posisjon
    float deltaTime;                         // current delta time from update() used to set speed

    // SDL properties
    float spriteWidth = 40;    // Width of sprite
    float spriteHeight = 90;   // Height of sprite
    SDL_Rect spriteRect;       // SDL_Rect for rendering
    SDL_Texture* texture;      // Car texture

    // Boost functionality
    bool isBoosting = false;  // Indikerer om bilen er i en boost-fase
    float boostMultiplier;    // Hvor mye hastigheten øker under boost
    float boostDuration;      // Hvor lenge boosten varer
    float boostElapsedTime;   // Tid som har gått siden boosten startet
    float normalTopSpeed;     // Lagre den normale toppfarten
};

#endif // CAR_HPP
