#ifndef TILE_HPP
#define TILE_HPP

#include <SDL.h>
#include <vector>
#include <string>
#include "Hitbox.hpp"

class Tile {
public:
    // Constructor
    Tile(int id, SDL_Texture* texture, const std::vector<Hitbox>& hitboxes, int gridX, int gridY);

    // Rendering the tile
    void Render(SDL_Renderer* renderer, int tileSize);

    // Getters
    int GetID() const { return id; }
    SDL_Texture* GetTexture() const { return texture; }
    const std::vector<Hitbox>& GetHitboxes() const { return hitboxes; }

    int GetGridX() const { return gridX; }
    int GetGridY() const { return gridY; }

    // Setters
    void SetGridPosition(int x, int y) { gridX = x; gridY = y; }
    void AddHitbox(const Hitbox& hitbox) { hitboxes.push_back(hitbox); }

private:
    int id;                             // Unique ID for the tile
    SDL_Texture* texture;               // The sprite texture for the tile
    std::vector<Hitbox> hitboxes;       // List of hitboxes associated with this tile
    int gridX, gridY;                   // Tile position in the grid
};

#endif // TILE_HPP
