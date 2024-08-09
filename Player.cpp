#include "Player.hpp"

Player::Player(float x, float y, float speed) : posX(x), posY(y), speed(speed) {
    spriteRect.w = 50;
    spriteRect.h = 50;
    spriteRect.x = static_cast<int>(posX);
    spriteRect.y = static_cast<int>(posY);

    


}

Player::~Player() {
    // Rydd opp om nødvendig
}

void Player::Update(float deltaTime) {
    // Oppdater posisjon basert på hastighet og delta-tid
    posX += speed * deltaTime;
    posY += speed * deltaTime;

    // Oppdater spriteRect for rendering
    spriteRect.x = static_cast<int>(posX);
    spriteRect.y = static_cast<int>(posY);
}

void Player::Render(SDL_Renderer* renderer) {
    // Legger til player texture
    SDL_Surface* tempSurface = IMG_Load("assets/Thermos.png");
    playerTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

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
