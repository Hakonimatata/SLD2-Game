#include "Tile.hpp"

// Constructor
Tile::Tile(int id, SDL_Texture* texture, const std::vector<Hitbox>& hitboxes, int gridX, int gridY, int width, int height)
    : id(id), texture(texture), hitboxes(hitboxes), gridX(gridX), gridY(gridY), width(width), height(height) {}

/// @brief Renders a tile based on the tile's position in the grid
/// @param renderer    
/// @param tileSize 
/// @param gridShiftX width before the grid starts
/// @param gridShiftY height before the grid starts
// void Tile::Render(SDL_Renderer* renderer, int tileSize, int gridShiftX, int gridShiftY)
// {
//     SDL_Rect destRect;
//     destRect.x = gridShiftX + gridX * tileSize;
//     destRect.y = gridShiftY + gridY * tileSize;
//     destRect.w = width * tileSize;
//     destRect.h = height * tileSize;

//     SDL_RenderCopyEx(renderer, texture, nullptr, &destRect, rotation, nullptr, SDL_FLIP_NONE);
// }

void Tile::Render(SDL_Renderer* renderer, int tileSize, int gridShiftX, int gridShiftY)
{
    // Beregn den faktiske bredden og høyden på flisen etter rotasjon
    int renderWidth = (rotation == 90 || rotation == 270) ? height : width;
    int renderHeight = (rotation == 90 || rotation == 270) ? width : height;

    // Beregn posisjonen til flisen med hensyn til rotasjon
    int offsetX = 0;
    int offsetY = 0;
    
    if (rotation == 90) {
        // Når rotert 90 grader, flytter vi posisjonen for å tilpasse til høyden
        offsetX = -((renderWidth - width) * tileSize / 2);
        offsetY = ((renderHeight - height) * tileSize / 2);
    } else if (rotation == 270) {
        // Når rotert 270 grader, flytter vi posisjonen for å tilpasse til bredden
        offsetX = ((renderWidth - width) * tileSize / 2);
        offsetY = -((renderHeight - height) * tileSize / 2);
    } else if (rotation == 180) {
        // Når rotert 180 grader, trenger vi ikke å justere posisjonen
        offsetX = 0;
        offsetY = 0;
    }

    SDL_Rect destRect;
    destRect.x = gridShiftX + gridX * tileSize + offsetX;
    destRect.y = gridShiftY + gridY * tileSize + offsetY;
    destRect.w = renderWidth * tileSize;
    destRect.h = renderHeight * tileSize;

    SDL_RenderCopyEx(renderer, texture, nullptr, &destRect, rotation, nullptr, SDL_FLIP_NONE);
}

