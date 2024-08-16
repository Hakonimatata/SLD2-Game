#ifndef TILE_HPP
#define TILE_HPP

#pragma once

#include <SDL.h>
#include <vector>
#include <string>
#include "Hitbox.hpp"

class Tile {
public:
    // Constructor
    Tile(int id, SDL_Texture* texture, const std::vector<Hitbox>& hitboxes, int gridX, int gridY);

    // Rendering the tile
    void Render(SDL_Renderer* renderer, int tileSize, int gridShiftX, int gridShiftY);

    // Getters
    int GetID() const { return id; }
    SDL_Texture* GetTexture() const { return texture; }
    const std::vector<Hitbox>& GetHitboxes() const { return hitboxes; }

    int GetGridX() const { return gridX; }
    int GetGridY() const { return gridY; }
    int GetRotation() const { return rotation; }

    // Setters
    void SetGridPosition(int x, int y) { gridX = x; gridY = y; }
    void AddHitbox(const Hitbox& hitbox) { hitboxes.push_back(hitbox); }
    void SetRotation(int newRotation) { rotation = newRotation % 360; } // Keep the rotation within 0-359 degrees

private:
    int id;                             // Unique ID for the tile
    SDL_Texture* texture;               // The sprite texture for the tile
    std::vector<Hitbox> hitboxes;       // List of hitboxes associated with this tile
    int gridX, gridY;                   // Tile position in the grid
    int rotation;                       // Rotation angle in degrees (0, 90, 180, 270)
};


struct TileData {
    int id;
    int rotation;

    TileData(int id = 0, int rotation = 0) : id(id), rotation(rotation) {}
};




#endif // TILE_HPP
