#include "Tile.hpp"

// Constructor
Tile::Tile(int id, SDL_Texture* texture, const std::vector<Hitbox>& hitboxes, int gridX, int gridY)
    : id(id), texture(texture), hitboxes(hitboxes), gridX(gridX), gridY(gridY) {}

/// @brief Renders a tile based on the tile's position in the grid
/// @param renderer    
/// @param tileSize 
/// @param gridShiftX width before the grid starts
/// @param gridShiftY height before the grid starts
void Tile::Render(SDL_Renderer* renderer, int tileSize, int gridShiftX, int gridShiftY)
{
    SDL_Rect destRect;
    destRect.x = gridShiftX + gridX * tileSize;
    destRect.y = gridShiftY + gridY * tileSize;
    destRect.w = tileSize;
    destRect.h = tileSize;

    SDL_RenderCopyEx(renderer, texture, nullptr, &destRect, rotation, nullptr, SDL_FLIP_NONE);
}
