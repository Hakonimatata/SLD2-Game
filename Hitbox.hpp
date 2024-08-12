#ifndef HITBOX_HPP
#define HITBOX_HPP

#include <SDL.h>
#include <string>

class Hitbox {
public:
    // Constructor
    Hitbox(int x, int y, int width, int height);
    Hitbox(const SDL_Rect& rect);

    // Check for collision with another hitbox
    bool CheckCollision(const Hitbox& other) const;

    // Scale the hitbox
    void SetScale(float scaleFactor);

    // Getters
    int GetX() const { return hitboxRect.x; }
    int GetY() const { return hitboxRect.y; }
    int GetWidth() const { return hitboxRect.w; }
    int GetHeight() const { return hitboxRect.h; }

    // Get the SDL_Rect for rendering or other purposes
    const SDL_Rect& GetRect() const { return hitboxRect; }

private:
    SDL_Rect hitboxRect; // Rectangle representing the hitbox
};

#endif // HITBOX_HPP
