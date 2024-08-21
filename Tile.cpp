#include "Tile.hpp"

// Constructor
Tile::Tile(int id, SDL_Texture* texture, const std::vector<Hitbox>& hitboxes, int gridX, int gridY, int width, int height)
    : id(id), texture(texture), hitboxes(hitboxes), gridX(gridX), gridY(gridY), width(width), height(height) {}

/// @brief Renders a tile based on the tile's position in the grid
/// @param renderer    
/// @param tileSize 
/// @param gridShiftX width before the grid starts
/// @param gridShiftY height before the grid starts
void Tile::Render(SDL_Renderer* renderer, int tileSize, int gridShiftX, int gridShiftY)
{
    SDL_Rect destRect = GetTileRectFromGrid(tileSize, gridShiftX, gridShiftY, gridX, gridY, rotation, width, height);

    SDL_RenderCopyEx(renderer, texture, nullptr, &destRect, rotation, nullptr, SDL_FLIP_NONE);
}


/// @brief Return the rect of the tile
/// @param tileSize
/// @param gridShiftX 
/// @param gridShiftY 
/// @return 
SDL_Rect GetTileRectFromGrid(int tileSize, int gridShiftX, int gridShiftY, int gridCoordX, int gridCoordY, int rotation, int width, int height)
{
    // Change center based on rotation
    int offsetX = 0;
    int offsetY = 0;
    
    if (height != width)
    {
        // Center point is changed based on rotation
        if (rotation == 90 || rotation == 270)
        {
            offsetX = (height - width) * tileSize / 2;
            offsetY = -(height - width) * tileSize / 2;
        }
    }

    SDL_Rect destRect;
    destRect.x = gridShiftX + gridCoordX * tileSize + offsetX;
    destRect.y = gridShiftY + gridCoordY * tileSize + offsetY;
    destRect.w = width * tileSize;
    destRect.h = height * tileSize;

    return destRect;
}
