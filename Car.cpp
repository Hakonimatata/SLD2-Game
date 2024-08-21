#include "Car.hpp"
#include <iostream>
#include <math.h>
#include <string>


using namespace std;

/// @brief Car constructor
/// @param renderer 
/// @param x x position
/// @param y y position
/// @param speed top speed of car
/// @param textureFile image path
/// @param scaleFactor scales everything proportionaly
/// @param physicsSpeedFactor speed of how fast physics acts (acceleration, turn speed, top speed etc)
Car::Car(SDL_Renderer* renderer, float x, float y, float speed, const std::string& textureFile, float scaleFactor) 
    : posX(x), posY(y)
{
    topSpeed = speed;
    
    spriteRect.w = spriteWidth;
    spriteRect.h = spriteHeight;

    spriteRect.x = static_cast<int>(posX);
    spriteRect.y = static_cast<int>(posY);

    // Load texture from the specified file
    SDL_Surface* tempSurface = IMG_Load(textureFile.c_str());
    if(tempSurface == NULL) {
        cout << "Failed to load surface: " << IMG_GetError() << endl;
    }
    texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    if(texture == NULL) {
        cout << "Failed to load texture: " << SDL_GetError() << endl;
    }

    ScaleEverything(scaleFactor);
}


Car::~Car() {
    // Clean up
    if(texture != NULL) {SDL_DestroyTexture(texture);}
}

void Car::HandleInput(const Uint8* state, const PlayerControls& controls)
{
    if (state[controls.drift] && (velocity.getSize() > 0.7 * topSpeed || state[controls.accelerate]) ) {
        isDrifting = true;
    } else {
        isDrifting = false;
    }

    // Boost
    if (state[controls.boost]) {
        StartBoost(1.5f, 30.0f);
    }

    // Turn left
    if (state[controls.turnLeft]) {
        RotateLeft();
    }

    // Turn right
    if (state[controls.turnRight]) {
        RotateRight();
    }

    // Accelerate
    if (state[controls.accelerate]) {
        isAccelerating = true;
        Accelerate();
    } else {
        isAccelerating = false;
    }

    // Reverse
    if (state[controls.reverse]) {
        Reverse();
    }
}


void Car::Update(float deltaTime)
{
    // get current delta time from game class
    this->deltaTime = deltaTime * physicsSpeedFactor;
    
    // Oppdater grep/trekkraft gradvis ut ifra drift knapp
    UpdateTractionPercentage();

    // Juster fartsvektoren mot bilens retning basert på tractionPercentage
    AdjustVelocityTowardsAngle();

    // Oppdater posisjon basert på den nåværende hastigheten
    UpdatePosition();

    // Påfør friksjon hvis bilen ikke akselererer
    if(!isAccelerating)
    {
        ApplyFriction();
    }

    if(isBoosting)
    {
        ApplyBoost(); // Deltatime used for duration
    }


    // Begrens hastigheten til topSpeed
    RestrictSpeedToTopSpeed();


    // Oppdater sprite rectange for rendering
    spriteRect.x = static_cast<int>(posX);
    spriteRect.y = static_cast<int>(posY);
}

void Car::Render(SDL_Renderer* renderer) {
    // Draw player texture
    SDL_RenderCopyEx(renderer, texture, NULL, &spriteRect, angle * 180.0f / M_PI + 90, NULL, SDL_FLIP_NONE);
}

void Car::SetPosition(float x, float y) {
    posX = x;
    posY = y;
    spriteRect.x = static_cast<int>(posX);
    spriteRect.y = static_cast<int>(posY);
}

void Car::SetSpeed(float newSpeed) {
    topSpeed = newSpeed;
}

void Car::StartBoost(float multiplier, float duration)
{
    if (!isBoosting)
    {
        isBoosting = true;
        boostMultiplier = multiplier;
        boostDuration = duration * deltaTime;
        boostElapsedTime = 0.0f;
        normalTopSpeed = topSpeed;
        topSpeed *= boostMultiplier;  // Øk toppfarten midlertidig
    }
}

void Car::ScaleEverything(float scaleFactor) // Todo: Make physics work for all zomming
{
    // Todo: check if errors occur
    topSpeed *= scaleFactor;
    spriteWidth *= scaleFactor;
    spriteHeight *= scaleFactor;
    spriteRect.w = spriteWidth;
    spriteRect.h = spriteHeight;
    acceleration *= scaleFactor;
    // friction *= scaleFactor; // Dont scale friction, because it is scaled based on current speed

    posX *= scaleFactor;
    posY *= scaleFactor;
}


void Car::Accelerate()
{
    // Apply acceleration only in direction of car

    int accelerationMultiplier = 1.0f;
    if(isDrifting)
    {
        accelerationMultiplier = 1.5f;
    }
    
    velocity.x += acceleration * cos(angle) * deltaTime * accelerationMultiplier;
    velocity.y += acceleration * sin(angle) * deltaTime * accelerationMultiplier;
}

void Car::Reverse()
{
    // Todo: Add reverse. It only breaks the car at the moment

    // Apply friction for  breaking
    velocity.x -= velocity.x * friction * 4 * deltaTime;
    velocity.y -= velocity.y * friction * 4 * deltaTime;
}

/// @brief Dynamisk endrer grepet til hjulene mellom drifting
void Car::UpdateTractionPercentage()
{
    const float adjustmentSpeed = 0.004f;
    const float minTraction = 0.005f;
    const float maxTraction = 1.0f;

    if(isDrifting)
    {
        tractionPercentage -= 0.1f * deltaTime;
        if(tractionPercentage < minTraction)
        {
            tractionPercentage = minTraction;
        }
    }
    else
    {
        tractionPercentage += adjustmentSpeed * deltaTime;
        if(tractionPercentage > maxTraction)
        {
            tractionPercentage = maxTraction;
        }
    }
}

void Car::ApplyBoost()
{
    if (isBoosting)
    {
        boostElapsedTime += deltaTime;

        // Hvis boosten er over, begynn å gradvis gå tilbake til normal toppfart
        if (boostElapsedTime >= boostDuration)
        {
            float lerpFactor = (boostElapsedTime - boostDuration) / boostDuration;
            topSpeed = normalTopSpeed + (boostMultiplier * normalTopSpeed - normalTopSpeed) * (1.0f - lerpFactor);

            // Hvis toppfarten er tilnærmet lik normal toppfart, avslutt boosten
            if (topSpeed <= normalTopSpeed + 0.01f) 
            {
                topSpeed = normalTopSpeed;  // Tilbakestill toppfarten helt
                isBoosting = false;  // Avslutt boost
            }
        }
    }
}


void Car::AdjustVelocityTowardsAngle()
{
    float lerpFactor = tractionPercentage;

    float targetVelocityX = cos(angle) * velocity.getSize();
    float targetVelocityY = sin(angle) * velocity.getSize();

    // Gradvis juster hastigheten mot målretningen ved hjelp av LERP
    velocity.x += lerpFactor * (targetVelocityX - velocity.x);
    velocity.y += lerpFactor * (targetVelocityY - velocity.y);
}

void Car::UpdatePosition()

{
    posX += velocity.x * deltaTime;
    posY += velocity.y * deltaTime;
}

void Car::ApplyFriction()
{
    velocity.x -= velocity.x * friction * deltaTime;
    velocity.y -= velocity.y * friction * deltaTime;
}

void Car::RestrictSpeedToTopSpeed()
{
    if (velocity.getSize() > topSpeed)
    {
        velocity.normalize();
        velocity.x *= topSpeed;
        velocity.y *= topSpeed;
    }
}



void Car::RotateLeft()
{
    float angleSpeed = GetAngleSpeed() * deltaTime;

    angle -= angleSpeed;
    
    if(angle >= 2*M_PI)
    {
        angle -= 2*M_PI;
    } 
}

void Car::RotateRight()
{
    float angleSpeed = GetAngleSpeed() * deltaTime;

    angle += angleSpeed;

    if(angle < 0)
    {
       angle += 2*M_PI;
    } 
}

float Car::GetTopSpeedPercentage()
{
    return velocity.getSize() / topSpeed;
}

float Car::GetAngleSpeed()
{
    // Adjust rotation speed to make it feel more natural

    float adjustedRotationSpeed = rotationSpeed - 0.8 * rotationSpeed / topSpeed * velocity.getSize();
    
    if(isDrifting)
    {
        adjustedRotationSpeed *= rotationMultiplierWhenDrift; // Apply factor for extra rotation when drifting
    }

    return adjustedRotationSpeed * GetTopSpeedPercentage();
    
}