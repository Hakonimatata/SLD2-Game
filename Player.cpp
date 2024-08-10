#include "Player.hpp"
#include <iostream>

using namespace std;

Player::Player(SDL_Renderer* renderer, float x, float y, float speed) : posX(x), posY(y), speed(speed) 
{
    spriteWidth = 100;
    spriteHeight = 100;
    
    spriteRect.w = spriteWidth;
    spriteRect.h = spriteHeight;

    spriteRect.x = static_cast<int>(posX);
    spriteRect.y = static_cast<int>(posY);

    // Load texture
    SDL_Surface* tempSurface = IMG_Load("assets/Thermos.png");
    if(tempSurface == NULL) {cout << "Failed to load surface: " << SDL_GetError() << endl;}
    playerTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    if(playerTexture == NULL) {cout << "Failed to load texture: " << SDL_GetError() << endl;}
}

Player::~Player() {
    // Clean up
    if(playerTexture != NULL) {SDL_DestroyTexture(playerTexture);}
}

void Player::HandleInput(const Uint8 *state)
{
    // Move left
    if (state[SDL_SCANCODE_LEFT]) {
        posX -= speed;
    }
    // Move right
    if (state[SDL_SCANCODE_RIGHT]) {
        posX += speed;
    }
    // Move up
    if (state[SDL_SCANCODE_UP]) {
        posY -= speed;
    }
    // Move down
    if (state[SDL_SCANCODE_DOWN]) {
        posY += speed;
    }
}

void Player::Update(float deltaTime)
{
    // Oppdater sprite rectange for rendering
    spriteRect.x = static_cast<int>(posX);
    spriteRect.y = static_cast<int>(posY);
}

void Player::Render(SDL_Renderer* renderer) {
    // Draw player texture
    SDL_RenderCopy(renderer, playerTexture, NULL, &spriteRect);
}

void Player::SetPosition(float x, float y) {
    posX = x;
    posY = y;
    spriteRect.x = static_cast<int>(posX);
    spriteRect.y = static_cast<int>(posY);
}

void Player::SetSpeed(float newSpeed) {
    speed = newSpeed;
}
