#include "Car.hpp"
#include <iostream>
#include <math.h>

using namespace std;

Car::Car(SDL_Renderer* renderer, float x, float y, float speed) : posX(x), posY(y), speed(speed) 
{
    spriteWidth = 50;
    spriteHeight = 120;
    
    spriteRect.w = spriteWidth;
    spriteRect.h = spriteHeight;

    spriteRect.x = static_cast<int>(posX);
    spriteRect.y = static_cast<int>(posY);

    // Load texture
    SDL_Surface* tempSurface = IMG_Load("assets/RaceCar2.png");
    if(tempSurface == NULL) {cout << "Failed to load surface: " << SDL_GetError() << endl;}
    texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    if(texture == NULL) {cout << "Failed to load texture: " << SDL_GetError() << endl;}
}

Car::~Car() {
    // Clean up
    if(texture != NULL) {SDL_DestroyTexture(texture);}
}

void Car::HandleInput(const Uint8 *state)
{
    if (state[SDL_SCANCODE_D]) {
        isDrifting = true;
    } else {isDrifting = false;}

    // Turn left
    if (state[SDL_SCANCODE_LEFT]) {
        RotateLeft();
    }

    // Turn right
    if (state[SDL_SCANCODE_RIGHT]) {
        RotateRight();
    }

    // Accelerate
    if (state[SDL_SCANCODE_UP]) {
        isAccelerating = true;
        Accelerate();
    } else {isAccelerating = false;}


    // Reverse
    if (state[SDL_SCANCODE_DOWN]) {
        Reverse();
    }


}

void Car::Update(float deltaTime)
{
    // Update velocity
    Slide();

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
    speed = newSpeed;
}

void Car::Accelerate()
{
    velocity.x += acceleration * cos(angle);
    velocity.y += acceleration * sin(angle);

    // Restrict speed by top speeed
    if (velocity.getSize() > topSpeed)
    {
        velocity.normalize();
        velocity.x *= topSpeed;
        velocity.y *= topSpeed;
    }
}

void Car::Reverse()
{
    float reverseAccel = -0.5f * acceleration;
    velocity.x += reverseAccel * cos(angle);
    velocity.y += reverseAccel * sin(angle);

    // Restrict speed by top speeed
    if (velocity.getSize() > topSpeed)
    {
        velocity.normalize();
        velocity.x *= topSpeed;
        velocity.y *= topSpeed;
    }
}

void Car::Slide()
{
    // TODO: Make function getTractionPercentage()
    if(isDrifting)
    {
        // Reduce traction percentage
        tractionPercentage -= 0.04f;

        float cap = 0.2f;
        if(tractionPercentage < cap)
        {
            tractionPercentage = cap;
        }
    }
    else
    {
        // Increase traction percentage
        tractionPercentage += 0.08f;
        if(tractionPercentage > 1.0f)
        {
            tractionPercentage = 1.0f;
        }
    }

    // Move in direction of speed
    posX += velocity.x * (1 - tractionPercentage);
    posY += velocity.y * (1 - tractionPercentage);

    // Move in direction of car angle
    // TODO: fix!
    posX += velocity.getSize() * cos(angle) * tractionPercentage;
    posY += velocity.getSize() * sin(angle) * tractionPercentage;

    if(!isAccelerating)
    {
        // Apply friction
        velocity.x -= velocity.x * friction;
        velocity.y -= velocity.y * friction;
    }

    //TODO: make function restrictToTopSpeed()
    // Begrens hastigheten for å unngå å overskride topSpeed
    float speed = sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    if (speed > topSpeed)
    {
        velocity.x = (velocity.x / speed) * topSpeed;
        velocity.y = (velocity.y / speed) * topSpeed;
    }

}

void Car::RotateLeft()
{
    float angleSpeed = GetAngleSpeed();

    angle -= angleSpeed;
    
    if(angle >= 2*M_PI)
    {
        angle -= 2*M_PI;
    } 
}

void Car::RotateRight()
{
    float angleSpeed = GetAngleSpeed();

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
        adjustedRotationSpeed *= 1.5f;
    }

    return adjustedRotationSpeed * GetTopSpeedPercentage();
    
}
