#include "Hitbox.hpp"

// Constructor that initializes the hitbox with specific dimensions
Hitbox::Hitbox(int x, int y, int width, int height) {
    hitboxRect.x = x;
    hitboxRect.y = y;
    hitboxRect.w = width;
    hitboxRect.h = height;
}

// Constructor that initializes the hitbox from an SDL_Rect
Hitbox::Hitbox(const SDL_Rect& rect) {
    hitboxRect = rect;
}

// Check for collision with another hitbox
bool Hitbox::CheckCollision(const Hitbox& other) const {
    // Use SDL's built-in function for rectangle collision detection
    return SDL_HasIntersection(&hitboxRect, &other.GetRect());
}

// Scale the hitbox by a factor
void Hitbox::SetScale(float scaleFactor) {
    hitboxRect.w = static_cast<int>(hitboxRect.w * scaleFactor);
    hitboxRect.h = static_cast<int>(hitboxRect.h * scaleFactor);
    hitboxRect.x = static_cast<int>(hitboxRect.x * scaleFactor);
    hitboxRect.y = static_cast<int>(hitboxRect.y * scaleFactor);
}
