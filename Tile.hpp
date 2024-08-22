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
    Tile(int id, SDL_Texture* texture, const std::vector<Hitbox>& hitboxes, int gridX, int gridY, int width, int height);

    // Rendering the tile
    void Render(SDL_Renderer* renderer, int tileSize, int gridShiftX, int gridShiftY);

    // Getters
    int GetID() const { return id; }
    SDL_Texture* GetTexture() const { return texture; }
    const std::vector<Hitbox>& GetHitboxes() const { return hitboxes; }

    int GetGridX() const { return gridX; }
    int GetGridY() const { return gridY; }
    int GetRotation() const { return rotation; }
    int GetWidth() const { return width; }
    int GetHeight() const { return height; }

    // Setters
    void SetGridPosition(int x, int y) { gridX = x; gridY = y; }
    void AddHitbox(const Hitbox& hitbox) { hitboxes.push_back(hitbox); }
    void SetRotation(int newRotation) { rotation = newRotation % 360; } // Keep the rotation within 0-359 degrees

private:
    int id;                             // Unique ID for the tile
    SDL_Texture* texture;               // The sprite texture for the tile
    std::vector<Hitbox> hitboxes;       // List of hitboxes associated with this tile
    int gridX, gridY;                   // Tile position in the grid (gridcoords)
    int rotation;                       // Rotation angle in degrees (0, 90, 180, 270)
    int width, height;                  // Width and height of the tile (in tiles)
};


// Get the rect of the tile
SDL_Rect GetTileRectFromGrid(int tileSize, int gridShiftX, int gridShiftY, int gridCoordX, int gridCoordY, int rotation, int width, int height);


enum class TileDimentions
{
    None = 0,
    OneByOne = 1,
    OneByTwo = 2
};

struct TileData {
    int id;
    int rotation;
    int xPos;
    int yPos;

    TileData(int id = 0, int rotation = 0, int xPos = 0, int yPos = 0) : id(id), rotation(rotation), xPos(xPos), yPos(yPos) {}
};




#endif // TILE_HPP
