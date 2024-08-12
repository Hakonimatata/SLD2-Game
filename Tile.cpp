#include "Tile.hpp"

// Constructor
Tile::Tile(int id, SDL_Texture* texture, const std::vector<Hitbox>& hitboxes, int gridX, int gridY)
    : id(id), texture(texture), hitboxes(hitboxes), gridX(gridX), gridY(gridY) {}

// Rendering the tile
void Tile::Render(SDL_Renderer* renderer, int tileSize) {
    SDL_Rect destRect;
    destRect.x = gridX * tileSize;
    destRect.y = gridY * tileSize;
    destRect.w = tileSize;
    destRect.h = tileSize;

    SDL_RenderCopy(renderer, texture, NULL, &destRect);
}
