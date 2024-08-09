#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SDL.h>
#include "SDL_image.h"

class Player {
public:
    Player(float x, float y, float speed);
    ~Player();

    void Update(float deltaTime);
    void Render(SDL_Renderer* renderer);

    void SetPosition(float x, float y);
    void SetSpeed(float speed);

private:
    float posX, posY;     // Posisjon
    float speed;          // Hastighet i piksler per sekund
    SDL_Rect spriteRect;  // SDL_Rect for rendering
    SDL_Texture* playerTexture;

};

#endif // PLAYER_HPP